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
#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "DahengCameraClass.hpp"

#define RETURN_VAL_IF_FAILED(b, val) if (!(b)) { return val; }

#define RETURN_SUCCESS 0
#define RETURN_ERROR_OPEN 1
#define RETURN_ERROR_START 2
#define RETURN_ERROR_INVALID 3

static uint8_t *
updateFrame(DahengCameraClass &camera)
{
	camera.updateFrame();

	if (camera.getSupportsColor())
	{
		return camera.getColorFrame();
	}
	else
	{
		return camera.getGrayFrame();
	}
}

static int
run(DahengCameraClass &camera)
{
	uint8_t *imgData;
	unsigned int height, width, channels;

	// Acquire the first frame
	imgData = updateFrame(camera);

	// Get frame information
	height = camera.getHeight();
	width = camera.getWidth();
	channels = camera.getChannels();

	if (height <= 0 || width <= 0)
	{
		std::cout << "Invalid frame size" << std::endl;
		return RETURN_ERROR_INVALID;
	}

	// Initialize OpenCV image
	cv::Mat matrix(height, width, CV_8UC(channels), imgData);

	do
	{
		updateFrame(camera);

		/*
		 * Any image processing can be done here using the `matrix` variable,
		 * though it is recommended to create your own processing function and
		 * invoke it here.
		 */

		cv::imshow("Camera frame", matrix);
	} while (cv::waitKey(1) == -1); // Keep running until a key is pressed

	return RETURN_SUCCESS;
}

int
main(void)
{
	DahengCameraClass camera;
	int result;

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
		result = run(camera);
	}

	// Stop capturing
	camera.stopCapture();

	// Close the camera
	camera.close();

	// Stop program
	return result;
}

/* END OF FILE */
