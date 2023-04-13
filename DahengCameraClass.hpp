/* SPDX-License-Identifier: Unlicense
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#ifndef DAHENG_CAMERA_CLASS
#define DAHENG_CAMERA_CLASS

#include <cstdbool>
#include <cstdint>

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
