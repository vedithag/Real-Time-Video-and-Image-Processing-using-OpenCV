
/*
  Veditha Gudapati
  Functions for image manipulation
*/

#include <opencv2/opencv.hpp>
#include "filter.hpp"
#include <algorithm>
#include <cmath>


/*void displayBalloon(cv::Mat& frame, int x, int y) {
    // Add your code to display a balloon on the frame at the specified (x, y) position
    // You can draw a circle or any other shape to represent the balloon
    cv::circle(frame, cv::Point(x, y), 20, cv::Scalar(0, 255, 255), -1); // Yellow balloon with a radius of 20
}*/




// Function to apply an emboss effect to an image.
int embossEffect(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        std::cerr << "Source image is empty\n"; // Print an error message if the source image is empty.
        return -1;                              // Return -1 if the source image is empty.
    }

    cv::Mat sobelX, sobelY;
    sobelX3x3(src, sobelX); // Apply Sobel X filter.
    sobelY3x3(src, sobelY); // Apply Sobel Y filter.

    cv::Mat direction = (cv::Mat_<float>(1, 2) << 0.7071, 0.7071); // Create a direction vector.

    // Compute the dot product of Sobel X and Sobel Y with the direction vector
    cv::Mat embossImage = sobelX.mul(direction.at<float>(0)) + sobelY.mul(direction.at<float>(1)); // Compute the emboss effect.

    // Adjust the result to be in the range [0, 255]
    embossImage = embossImage + 128;                                  // Offset by 128 to ensure values are non-negative
    cv::normalize(embossImage, embossImage, 0, 255, cv::NORM_MINMAX); // Normalize the emboss image.

    // Convert to 8-bit unsigned integer
    embossImage.convertTo(dst, CV_8U); // Convert the embossed image to 8-bit unsigned integer.

    return 0; // Indicate success
}

// Function to blur and quantize an image.
int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels)
{
    if (src.empty())
    {
        std::cerr << "Source image is empty\n"; // Print an error message if the source image is empty.
        return -1;
    }

    // Blur the image using existing blur function
    cv::Mat blurredImage;
    blur5x5_1(src, blurredImage); // Apply blur to the image.

    // Quantize the image
    dst.create(src.size(), src.type()); // Create the output image.

    // Calculate the size of a bucket
    float bucketSize = 255.0 / levels;

    for (int y = 0; y < src.rows; ++y)
    {
        for (int x = 0; x < src.cols; ++x)
        {
            for (int c = 0; c < src.channels(); ++c)
            {
                // Quantize each color channel
                float originalValue = blurredImage.at<cv::Vec3b>(y, x)[c];
                int quantizedValue = static_cast<int>(originalValue / bucketSize) * bucketSize; // Calculate the quantized value.

                // Update the destination image
                dst.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(quantizedValue); // Update the pixel value in the output image.
            }
        }
    }

    return 0; // Indicate success
}

// Function to compute the magnitude of gradients from Sobel X and Sobel Y images.
int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst)
{
    // Create the destination matrix with the same size as Sobel X and Sobel Y
    dst.create(sx.size(), CV_16SC3); // Initialize the destination image with the same size as Sobel X and Sobel Y.

    // Loop over each row in the images
    for (int i = 0; i < sx.rows; i++)
    {
        // Get pointers to the current row in Sobel X, Sobel Y, and the destination matrices
        cv::Vec3s *xptr = sx.ptr<cv::Vec3s>(i);  // Pointer to the current row in Sobel X.
        cv::Vec3s *yptr = sy.ptr<cv::Vec3s>(i);  // Pointer to the current row in Sobel Y.
        cv::Vec3s *dptr = dst.ptr<cv::Vec3s>(i); // Pointer to the current row in the destination image.

        // Loop over each column in the images
        for (int j = 0; j < sx.cols; j++)
        {
            // Loop over each channel (R, G, B) in the images
            for (int k = 0; k < sx.channels(); k++)
            {
                // Calculate the Euclidean distance for each channel
                dptr[j][k] = sqrt((xptr[j][k] * xptr[j][k]) + (yptr[j][k] * yptr[j][k])); // Calculate the magnitude.
            }
        }
    }

    return 0; // Return 0 to indicate success.
}

// Function to apply a 3x3 Sobel X filter to an image.
int sobelX3x3(cv::Mat& src, cv::Mat& dst) {

    //allocate dst image
    dst = cv::Mat::zeros(src.size(), CV_16SC3); //signed short data type 
    cv::Mat temp_h = cv::Mat::zeros(src.size(), CV_16SC3); //signed short data type 
    //loop over src and apply a 3x3 filter
    for (int i = 1; i < src.rows - 1; i++) {

        //src pointer
        cv::Vec3b* rptr = src.ptr<cv::Vec3b>(i);
        //destination pointer
        cv::Vec3s* dptr = temp_h.ptr<cv::Vec3s>(i);
        //for each column 
        for (int j = 1; j < src.cols - 1; j++) {
            //for each color channel 
            for (int c = 0; c < 3; c++) {
                dptr[j][c] = (-1 * rptr[j - 1][c] + 0 * rptr[j][c] + 1 * rptr[j + 1][c]); // Apply 1x3 horizontal filter
            }
        }
    }

    int v_kernel[3] = { 1, 2, 1 }; // 1x3 vertical kernel 
    // Loop over rows
    for (int r = 1; r < src.rows - 1; r++) {
        // Loop over cols
        for (int c = 1; c < src.cols - 1; c++) {
            cv::Vec3s sum(0, 0, 0);
            for (int i = -1; i <= 1; i++)
            {
                int y = r + i;
                if (y < 0 || y >= src.rows)
                    continue;

                //Source pointer
                cv::Vec3s* pixel = temp_h.ptr<cv::Vec3s>(y);
                float weight = v_kernel[i + 1]; //Apply the filter
                sum += pixel[c] * weight;
            }
            //Destination Pointer
            cv::Vec3s* dptr = dst.ptr<cv::Vec3s>(r);
            dptr[c] = sum;
        }
    }
    return 0;
}


// Function to apply a 3x3 Sobel Y filter to an image.
int sobelY3x3(cv::Mat &src, cv::Mat &dst)
{
    // Allocate the destination image
    dst = cv::Mat::zeros(src.size(), CV_16SC3); // Initialize the destination image with zero values.

    // Temporary matrix to hold the horizontal filter results
    cv::Mat temp_h = cv::Mat::zeros(src.size(), CV_16SC3); // Temporary matrix for horizontal filtering.

    // Loop over src and apply a 3x3 filter
    for (int i = 1; i < src.rows - 1; i++)
    {
        // Pointer to the current row in the source image
        cv::Vec3b *rptr = src.ptr<cv::Vec3b>(i);

        // Pointer to the current row in the temporary horizontal matrix
        cv::Vec3s *dptr = temp_h.ptr<cv::Vec3s>(i);

        // Loop over each column
        for (int j = 1; j < src.cols - 1; j++)
        {
            // Loop over each color channel (R, G, B)
            for (int c = 0; c < 3; c++)
            {
                dptr[j][c] = (1 * rptr[j - 1][c] + 2 * rptr[j][c] + 1 * rptr[j + 1][c]); // Apply 1x3 horizontal filter.
            }
        }
    }

    int v_kernel[3] = {1, 0, -1}; // Define the vertical kernel

    // Loop over rows
    for (int r = 1; r < src.rows - 1; r++)
    {
        // Loop over cols
        for (int c = 1; c < src.cols - 1; c++)
        {
            cv::Vec3s sum(0, 0, 0);
            for (int i = -1; i <= 1; i++)
            {
                int y = r + i;
                if (y < 0 || y >= src.rows)
                    continue;

                // Source pointer
                cv::Vec3s *pixel = temp_h.ptr<cv::Vec3s>(y);

                float weight = v_kernel[i + 1]; // Apply 1x3 vertical filter.
                sum += pixel[c] * weight;
            }
            // Destination Pointer
            cv::Vec3s *dptr = dst.ptr<cv::Vec3s>(r);
            dptr[c] = sum;
        }
    }
    return 0; // Return 0 to indicate success.
}

// Function to apply a 5x5 blur filter to an image.
int blur5x5_1(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        return -1;
    }

    dst = src.clone(); // Initialize dst with the same content as src

    int kernel[5][5] = {{1, 2, 4, 2, 1},
                        {2, 4, 8, 4, 2},
                        {4, 8, 16, 8, 4},
                        {2, 4, 8, 4, 2},
                        {1, 2, 4, 2, 1}}; // Define the blur kernel.

    int kernelSum = 88; // Sum of all values in the kernel

    // Iterate over inner pixels of the image
    for (int y = 2; y < src.rows - 2; ++y)
    {
        for (int x = 2; x < src.cols - 2; ++x)
        {
            for (int c = 0; c < src.channels(); ++c)
            {
                int sum = 0;

                // Apply the 5x5 kernel
                for (int ky = -2; ky <= 2; ++ky)
                {
                    for (int kx = -2; kx <= 2; ++kx)
                    {
                        sum += src.at<cv::Vec3b>(y + ky, x + kx)[c] * kernel[ky + 2][kx + 2]; // Apply the kernel.
                    }
                }

                // Normalize the result
                sum /= kernelSum;

                // Update the destination image
                dst.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(std::max(0, std::min(255, sum))); // Update the pixel value.
            }
        }
    }

    return 0; // Success
}

// Function to apply a 5x5 blur filter to an image using a predefined kernel.
int blur5x5(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        return -1;
    }

    cv::Mat kernel = (cv::Mat_<float>(5, 5) << 1, 2, 4, 2, 1,
                      2, 4, 8, 4, 2,
                      4, 8, 16, 8, 4,
                      2, 4, 8, 4, 2,
                      1, 2, 4, 2, 1) /
                     88.0; // Normalize the kernel

    // Apply the 5x5 kernel using filter2D
    cv::filter2D(src, dst, -1, kernel);

    return 0;
}

// Function to apply a 5x5 blur filter to an image using separable kernels.
int blur5x5_2(cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        return -1; // Error: Empty source image
    }

    dst = src.clone(); // Initialize dst with the same content as src

    int kernel5[5] = {-5, 0, 20, 0, -5}; // Define the separable kernel

    // Apply horizontal blur
    for (int y = 0; y < src.rows; ++y)
    {
        for (int x = 2; x < src.cols - 2; ++x)
        {
            for (int c = 0; c < src.channels(); ++c)
            {
                int sum = 0;

                // Apply the 1x5 kernel horizontally
                for (int kx = -2; kx <= 2; ++kx)
                {
                    sum += src.at<cv::Vec3b>(y, x + kx)[c] * kernel5[kx + 2];
                }

                // Normalize the result
                sum /= 10; // Sum of the separable kernel

                // Update the destination image
                dst.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(std::max(0, std::min(255, sum)));
            }
        }
    }

    // Apply vertical blur
    for (int y = 2; y < src.rows - 2; ++y)
    {
        for (int x = 0; x < src.cols; ++x)
        {
            for (int c = 0; c < src.channels(); ++c)
            {
                int sum = 0;

                // Apply the 1x5 kernel vertically
                for (int ky = -2; ky <= 2; ++ky)
                {
                    sum += dst.at<cv::Vec3b>(y + ky, x)[c] * kernel5[ky + 2];
                }

                // Normalize the result
                sum /= 10; // Sum of the separable kernel

                // Update the destination image
                dst.at<cv::Vec3b>(y, x)[c] = static_cast<uchar>(std::max(0, std::min(255, sum)));
            }
        }
    }

    return 0; // Success
}

// Function to apply sepia effect with vignetting to an image.
int sepiaWithVignetting(const cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        std::cerr << "Source image is empty\n";
        return -1; // Error: Empty source image
    }

    dst.create(src.size(), src.type()); // Create the destination image with the same size as the source

    int centerX = src.cols / 2; // Calculate the center X coordinate
    int centerY = src.rows / 2; // Calculate the center Y coordinate

    // Iterate over each pixel in the source image
    for (int y = 0; y < src.rows; ++y)
    {
        for (int x = 0; x < src.cols; ++x)
        {
            float distanceX = std::abs(x - centerX) / static_cast<float>(src.cols / 2); // Calculate the distance from the center in the X direction
            float distanceY = std::abs(y - centerY) / static_cast<float>(src.rows / 2); // Calculate the distance from the center in the Y direction

            // Use a power function to increase the effect towards the corners
            float vignetteFactor = 1.0 - 0.15 * std::pow((distanceX + distanceY), 2);

            // Ensure that vignetteFactor is within the valid range [0, 1]
            vignetteFactor = std::max(0.0f, std::min(1.0f, vignetteFactor));

            cv::Vec3b pixel = src.at<cv::Vec3b>(y, x); // Get the pixel value at (x, y) in the source image

            // Calculate sepia values for each channel with vignetting
            uchar sepiaR = std::min(255.0f, vignetteFactor * (0.272f * pixel[2] + 0.534f * pixel[1] + 0.131f * pixel[0]));
            uchar sepiaG = std::min(255.0f, vignetteFactor * (0.349f * pixel[2] + 0.686f * pixel[1] + 0.168f * pixel[0]));
            uchar sepiaB = std::min(255.0f, vignetteFactor * (0.393f * pixel[2] + 0.769f * pixel[1] + 0.189f * pixel[0]));

            // Set the sepia values in the destination image
            dst.at<cv::Vec3b>(y, x) = cv::Vec3b(sepiaR, sepiaG, sepiaB);
        }
    }

    return 0;
}

// Function to apply sepia effect to an image.
int sepia(const cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        std::cerr << "Source image is empty\n";
        return -1;
    }

    dst.create(src.size(), src.type()); // Create the destination image with the same size as the source

    // Iterate over each pixel in the source image
    for (int y = 0; y < src.rows; ++y)
    {
        for (int x = 0; x < src.cols; ++x)
        {
            cv::Vec3b pixel = src.at<cv::Vec3b>(y, x); // Get the pixel value at (x, y) in the source image

            // Calculate sepia values for each channel
            uchar sepiaR = std::min(255.0f, 0.272f * pixel[2] + 0.534f * pixel[1] + 0.131f * pixel[0]);
            uchar sepiaG = std::min(255.0f, 0.349f * pixel[2] + 0.686f * pixel[1] + 0.168f * pixel[0]);
            uchar sepiaB = std::min(255.0f, 0.393f * pixel[2] + 0.769f * pixel[1] + 0.189f * pixel[0]);

            // Set the sepia values in the destination image
            dst.at<cv::Vec3b>(y, x) = cv::Vec3b(sepiaR, sepiaG, sepiaB);
        }
    }

    return 0; 
}

// Function to convert an image to greyscale using a custom transformation.
int greyscale(const cv::Mat &src, cv::Mat &dst)
{
    if (src.empty())
    {
        std::cerr << "Error: Input image is empty." << std::endl;
        return -1;
    }

    dst.create(src.size(), src.type()); // Create a new Mat for the greyscale version

    // Iterate through each pixel and apply the custom greyscale transformation
    for (int i = 0; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            cv::Vec3b pixel = src.at<cv::Vec3b>(i, j); // Get the pixel value at (i, j) in the source image

            // Custom greyscale transformation: subtract red channel from 255 and copy to all channels
            uchar grey_value = 255 - pixel[2];

            // Set the same greyscale value for all channels in the destination image
            dst.at<cv::Vec3b>(i, j) = cv::Vec3b(grey_value, grey_value, grey_value);
        }
    }

    return 0; 
}
