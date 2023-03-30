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

#include <cstdbool>

#include <GxIAPI.h>

class DahengCameraClass
{
	private:
		GX_DEV_HANDLE device;
		GX_FRAME_DATA frame_data;

		void *pixel_data_mono;
		void *pixel_data_rgb;

		bool supports_color;
		int height;
		int width;

	public:
		DahengCameraClass(void);

		bool start_capture(void);
		void stop_capture(void);

		void * get_frame_mono(void);
		void * get_frame_rgb(void);

		~DahengCameraClass(void);
};

/* END OF FILE */
