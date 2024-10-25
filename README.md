# Real-Time Video and Image Processing with OpenCV

## Prerequisites
To build and run this project, you need:

C++11 or newer for modern C++ syntax and features.
OpenCV library (version 4.x or newer) for image and video processing functions.
A working webcam for video capture.

## Project Overview
This project demonstrates advanced knowledge in computer vision and image processing using C++ and the OpenCV library. It showcases real-time manipulation of video and image frames, utilizing both fundamental and advanced techniques such as image filters, edge detection, and face recognition. This project involves designing and implementing algorithms that allow dynamic manipulation of visual data, making use of optimized methods for performance improvement.

## **Core Skills Highlighted**
**Computer Vision & Image Processing:** Comprehensive understanding of OpenCV, including matrix operations, image filtering, and real-time video processing.
Proficient in writing custom image filters from scratch, understanding the mathematical foundations of filters like Sobel, blur, sepia, and quantization.

**C++ Programming:** Experience in modern C++ features, such as smart pointers, real-time system interaction, and memory management. Structured, modular programming in C++ with emphasis on scalability and optimization.

**Performance Optimization:** Demonstrated use of algorithmic optimization through separable filter implementation, reducing computational complexity while maintaining image quality.

**Real-Time Video Processing:** Developed an interactive video-processing application capable of handling frame-by-frame manipulation using keyboard events, showcasing skills in real-time and event-driven programming.



## Technical Features and Competencies
### 1. Image Processing (imgDisplay.cpp)
The program reads an image and allows for real-time interaction:

Keyboard Inputs:
  - **`s`**: Save the current image.
  - **`r`**: Rotate the image clockwise by 90 degrees using matrix transformations.
  - **`l`**: Rotate the image counter-clockwise by 90 degrees.
  - **`q`**: Exit the program.


**Demonstrated Knowledge:**
1. File I/O with images using OpenCV's imread, imwrite functions.
2. Matrix Manipulation: Image rotation through transformation matrices.

### 2. Video Processing (vidDisplay.cpp)
The main part of the project processes live video streams and applies different visual effects based on keypresses:
- **q:** Quit the video stream.
- **s:** Save the current frame to disk.
- **g:** Apply the standard greyscale transformation using a weighted sum of color channels (`cvtColor`).
- **h:** Apply an alternative greyscale filter by inverting the red channel.
- **p:** Apply a sepia filter to give a warm, brownish tone to the video stream.
- **v:** Combine sepia with vignetting, an effect that darkens the corners of the image.
- **b & n:** Apply 5x5 blur filters:
  1. First, a direct convolution blur.
  2. Second, an optimized version using separable 1D filters for improved performance.
     
- **x & y:** Apply Sobel filters to compute the horizontal and vertical gradients of the image (edge detection).
- **m:** Display the gradient magnitude combining Sobel X and Sobel Y results.
- **l:** Apply blur and quantization to the image, reducing color resolution for a posterized effect.
- **f:** Detect faces using OpenCV's `CascadeClassifier`.
- **u:** Dynamically adjust the brightness and contrast of the video stream.
- **j:** Enable caption mode, allowing users to annotate frames with custom text.


**Demonstrated Knowledge:**
1. Real-time Video Processing using VideoCapture and handling video streams frame by frame.
2. Image Filtering: Implementation of multiple filters such as sepia, blur, Sobel edge detection, and quantization from scratch using pixel-wise operations.
3. Face Detection: Integrated OpenCV’s pre-trained face detection model using CascadeClassifier.
4. UI/UX Handling: Real-time user interaction for controlling video output using keyboard events.
5. Optimized Algorithms: Implemented performance improvements using separable filters to reduce computational complexity of image filtering.

### 3. Filter Implementations (filter.cpp)
This file contains the detailed implementation of the custom filters:
- **Greyscale Transformation:** Custom method that alters pixel values based on red channel inversion.
- **Sepia Filter:** Applies a linear transformation to RGB values to create a sepia effect. Incorporates mathematical operations to adjust each pixel’s intensity.
- **Sobel Filters:** Implemented manual 3x3 Sobel filters for edge detection, calculating gradients in both X and Y directions without using OpenCV’s built-in functions.
- **Blur Filters:** Two implementations of a 5x5 blur:
- **Direct convolution:** A traditional nested-loop convolution method.
- **Separable Filter:** Optimized using two 1x5 passes, significantly reducing the computation cost.
- **Emboss Effect:** Adds depth to the image using a directional gradient approach, emphasizing edges and transitions.
- **Face Detection and Annotation:** Using `CascadeClassifier` for face detection and drawing bounding boxes with custom visual effects (e.g., halos).

   
**Demonstrated Knowledge:**
1. Image Manipulation at the Pixel Level: Mastery of low-level operations on pixel data using multi-channel matrices (cv::Mat).
2. Custom Filters: Developed algorithms for effects like sepia, emboss, blur, and edge detection from first principles, showing a deep understanding of image processing.
3. Optimizing Algorithms: Demonstrated performance improvements through separable convolution filters.
4. Real-time Video Filters: Implemented filters that can be applied in real-time, including computationally expensive ones like Sobel filters and gradient magnitude calculations.

### 4. Advanced Features
**1. Face Detection:** Integrated real-time face detection using OpenCV’s pre-trained Haar cascade classifier.
**2. Captions:** Added functionality to embed user-provided text onto the video frames, including positioning and saving the result to disk.
**3. Brightness and Contrast Adjustments:** Real-time dynamic control of brightness and contrast via user input, modifying pixel intensity values on the fly.

### 5. Optimized Code Structure
The code is modularized for scalability and maintainability:
  1. Separate .cpp files for image manipulation functions.
  2. Clean handling of frame buffers and efficient matrix operations.
