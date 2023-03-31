/* SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2023 Quico Augustijn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <iostream>
#include <cstdbool>
#include <cassert>

#include <GxIAPI.h>
#include <DxImageProc.h>

#include "DahengCameraClass.hpp"

#define RETURN_VAL_IF_FAILED(b, val) if (!(b)) { return val; }

DahengCameraClass::DahengCameraClass(void)
{
	GX_STATUS status;

	// Initialize the library
	status = GXInitLib();
	assert(status == GX_STATUS_SUCCESS);

	// Get the number of devices
	status = GXUpdateDeviceList(&this->nDevices, 0);
	assert(status == GX_STATUS_SUCCESS);
	assert(this->nDevices > 0);
}

bool
DahengCameraClass::open(uint32_t device)
{
	GX_STATUS status;
	GX_OPEN_PARAM openParam;

	assert(device < 10);

	// Convert int to char
	char dev = device + 49;
	char devStr[2] = { dev, 0 };

	// Set 'open' parameters
	openParam.accessMode = GX_ACCESS_EXCLUSIVE;
	openParam.openMode = GX_OPEN_INDEX;
	openParam.pszContent = devStr;

	// Open the device
	status = GXOpenDevice(&openParam, &this->device);
	RETURN_VAL_IF_FAILED(status == GX_STATUS_SUCCESS, false);

	// Check whether this device is a color camera
	status = GXIsImplemented(this->device, GX_ENUM_PIXEL_COLOR_FILTER, &this->supportsColor);
	RETURN_VAL_IF_FAILED(status == GX_STATUS_SUCCESS, false);

	// Get the used color filter (if supported)
	if (this->supportsColor)
	{
		status = GXGetEnum(this->device, GX_ENUM_PIXEL_COLOR_FILTER, &this->colorFilter);
	}
	this->nChannels = (this->supportsColor ? 3 : 1);

	// Set acquisition mode
	status = GXSetEnum(this->device, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	RETURN_VAL_IF_FAILED(status == GX_STATUS_SUCCESS, false);

	// Set Balance White Mode to Continuous
	status = GXSetEnum(this->device, GX_ENUM_BALANCE_WHITE_AUTO, GX_BALANCE_WHITE_AUTO_CONTINUOUS);
	RETURN_VAL_IF_FAILED(status == GX_STATUS_SUCCESS, false);

	// Get the needed buffer size
	status = GXGetInt(this->device, GX_INT_PAYLOAD_SIZE, &this->frameSize);
	RETURN_VAL_IF_FAILED(status == GX_STATUS_SUCCESS, false);
	RETURN_VAL_IF_FAILED(this->frameSize > 0, false);

	return true;
}

bool
DahengCameraClass::close(void)
{
	GX_STATUS status = GXCloseDevice(this->device);

	return (status == GX_STATUS_SUCCESS);
}

bool
DahengCameraClass::startCapture(void)
{
	// Allocate image buffers
	this->pixelDataGray = (uint8_t *) malloc(this->frameSize * GRAY_N_CHANNELS);
	this->pixelDataColor = (uint8_t *) malloc(this->frameSize * COLOR_N_CHANNELS);
	this->frameData.pImgBuf = this->pixelDataGray;

	// Start image acquisition
	GX_STATUS status = GXSendCommand(this->device, GX_COMMAND_ACQUISITION_START);

	return (status == GX_STATUS_SUCCESS);
}

bool
DahengCameraClass::stopCapture(void)
{
	// Stop image acquisition
	GX_STATUS status = GXSendCommand(this->device, GX_COMMAND_ACQUISITION_STOP);

	// Free image buffers
	free(this->pixelDataGray);
	free(this->pixelDataColor);

	// Reset pointers
	this->pixelDataGray = NULL;
	this->pixelDataColor = NULL;
	this->frameData.pImgBuf = NULL;

	return (status == GX_STATUS_SUCCESS);
}

void
DahengCameraClass::updateFrame(void)
{
	// Calls GXGetImage to get an image.
	while (GXGetImage(this->device, &this->frameData, 0 /* timeout */) != GX_STATUS_SUCCESS);
}

uint8_t *
DahengCameraClass::getGrayFrame(void)
{
	return this->pixelDataGray;
}

uint8_t *
DahengCameraClass::getColorFrame(void)
{
	this->convertToColor();

	return this->pixelDataColor;
}

bool
DahengCameraClass::getSupportsColor(void)
{
	return this->supportsColor;
}

int32_t
DahengCameraClass::getChannels(void)
{
	return this->nChannels;
}

int64_t
DahengCameraClass::getImageSize(void)
{
	return this->frameSize;
}

int32_t
DahengCameraClass::getHeight(void)
{
	return this->frameData.nHeight;
}

int32_t
DahengCameraClass::getWidth(void)
{
	return this->frameData.nWidth;
}

bool
DahengCameraClass::convertToColor(void)
{
	GX_STATUS status;
	static bool failed = false;

	switch (this->frameData.nPixelFormat)
	{
		case GX_PIXEL_FORMAT_BAYER_GR8:
		case GX_PIXEL_FORMAT_BAYER_RG8:
		case GX_PIXEL_FORMAT_BAYER_GB8:
		case GX_PIXEL_FORMAT_BAYER_BG8:
			// Convert to the RGB image
			status = DxRaw8toRGB24(this->pixelDataGray, this->pixelDataColor, this->getWidth(), this->getHeight(),
			                       RAW2RGB_NEIGHBOUR, DX_PIXEL_COLOR_FILTER(this->colorFilter), false /* flip image */);

			// Only post failure message once if it occured
			if (status != DX_OK && !failed)
			{
				std::cout << "Failed to convert image to color (" << status << ")" << std::endl;
				return false;
			}
			break;
		case GX_PIXEL_FORMAT_BAYER_GR10:
		case GX_PIXEL_FORMAT_BAYER_RG10:
		case GX_PIXEL_FORMAT_BAYER_GB10:
		case GX_PIXEL_FORMAT_BAYER_BG10:
		case GX_PIXEL_FORMAT_BAYER_GR12:
		case GX_PIXEL_FORMAT_BAYER_RG12:
		case GX_PIXEL_FORMAT_BAYER_GB12:
		case GX_PIXEL_FORMAT_BAYER_BG12:
		default:
			std::cout << "Failed to convert image to color (invalid pixel format)" << std::endl;
			return false;
	}

	return true;
}

DahengCameraClass::~DahengCameraClass(void)
{
	GXCloseLib();
}

/* END OF FILE */
