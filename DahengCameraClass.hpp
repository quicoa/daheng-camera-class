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

#ifndef DAHENG_CAMERA_CLASS
#define DAHENG_CAMERA_CLASS

#include <cstdbool>

#include <GxIAPI.h>

#define GRAY_N_CHANNELS 1
#define COLOR_N_CHANNELS 3

class DahengCameraClass
{
	private:
		GX_DEV_HANDLE device = NULL;
		GX_FRAME_DATA frameData;

		uint32_t nDevices = 0;

		uint8_t *pixelDataGray = NULL;
		uint8_t *pixelDataColor = NULL;

		bool supportsColor = false;
		int32_t nChannels = 0;
		int64_t colorFilter = GX_COLOR_FILTER_NONE;
		int64_t frameSize = 0;

		bool opened = false;
		bool capturing = false;

	public:
		DahengCameraClass(void);

		bool open(uint32_t device);
		bool close(void);

		bool startCapture(void);
		bool stopCapture(void);

		void updateFrame(void);
		uint8_t * getGrayFrame(void);
		uint8_t * getColorFrame(void);

		bool getSupportsColor(void);
		int32_t getChannels(void);
		int64_t getImageSize(void);
		int32_t getHeight(void);
		int32_t getWidth(void);

		~DahengCameraClass(void);

	private:
		bool setCameraProperties(void);
		bool convertToColor(void);
};

#endif /* DAHENG_CAMERA_CLASS */

/* END OF FILE */
