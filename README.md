# Daheng Camera Class

A class created for students and anyone else wanting to use a Daheng Vision
Camera, optionally using OpenCV.

## Overview

OpenCV is a free and open-source software project consisting of a variety of
image processing tools that can be used to create computer vision projects.
While OpenCV can be used to continuously read frame from a webcam, it does not
seem like it can open any Daheng Vision Camera out-of-the-box.  This class is
created to provide a simple interface to use a Daheng camera.

The idea is that you can include the source files into your vision projects and
use the class to open the camera and read frames.  You'll still need to install
the proprietary GalaxySDK provided by the manifacture.

## Public archive

During development of this software, I noticed that OpenCV *can* in fact open
the camera and read frames without any trouble, provided that the hardware
driver shipped with the proprietary GalaxySDK was installed.

Because of this, the class is declared obsolete and it's development will be
abandoned.  If you still find anything from this repository useful, feel free to
use it without any restrictions. But keep in mind that the code that uses the
camera/SDK is created with the help of the 'C SDK Programming Reference Manual'
from the GalaxySDK, so this repository provides no new technologies, only an
abstraction layer.

Refer to the VideoCapture Class Reference from OpenCV for an example on how to
use a vision camera directly with OpenCV.

