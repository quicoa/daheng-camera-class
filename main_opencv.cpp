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

#include <cstdbool>
#include <cstdint>
#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "DahengOpenCV.hpp"

#define RETURN_VAL_IF_FAILED(b, val) if (!(b)) { return val; }

#define RETURN_SUCCESS 0
#define RETURN_ERROR_OPEN 1
#define RETURN_ERROR_START 2
#define RETURN_ERROR_INVALID 3

static void
run(DahengOpenCV &camera)
{
	do
	{
		camera.readFrame();

		/*
		 * Any image processing can be done here using the `matrix` variable,
		 * though it is recommended to create your own processing function and
		 * invoke it here.
		*/

		cv::imshow("Camera frame", camera.frame);
	} while (cv::waitKey(1) == -1); // Keep running until a key is pressed
}

int
main(void)
{
	DahengOpenCV camera;
	int result = RETURN_SUCCESS;

	// Open camera
	if (!camera.open(0 /* device */))
	{
		std::cout << "Could not open camera" << std::endl;
		return RETURN_ERROR_OPEN;
	}

	// Try to start capturing
	if (!camera.startCapture())
	{
		std::cout << "Could not start image capturing" << std::endl;
		result = RETURN_ERROR_START;
	}
	else
	{
		// Now run the main loop
		run(camera);
	}

	// Stop capturing
	camera.stopCapture();

	// Close the camera
	camera.close();

	// Stop program
	return result;
}

/* END OF FILE */
