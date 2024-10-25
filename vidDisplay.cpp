/*
  Veditha Gudapati
  Process video frames using different visual effects
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include "filter.hpp" 
#include "faceDetect.h"
#include "vidDisplay.h"
#include <string>

double currentBrightness = 1.0;  // Initial brightness value
double currentContrast = 1.0;    // Initial contrast value

void updateBrightnessContrast(cv::Mat& inputFrame, cv::Mat& outputFrame, double brightness, double contrast) {
    inputFrame.convertTo(outputFrame, -1, contrast, brightness);
}


void addCaption(cv::Mat& frame, const std::string& caption, int x, int y, double fontScale, cv::Scalar color) {
    cv::putText(frame, caption, cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, fontScale, color, 2);
}

void addCaptionOnSave(cv::Mat& frame, const std::string& caption, int x, int y, double fontScale, cv::Scalar textColor, cv::Scalar bgColor) {
    // Get the size of the text box
    int baseline = 0;
    cv::Size textSize = cv::getTextSize(caption, cv::FONT_HERSHEY_SIMPLEX, fontScale, 2, &baseline);

    // Draw a filled rectangle as the background
    cv::rectangle(frame, cv::Point(x, y - textSize.height), cv::Point(x + textSize.width, y), bgColor, -1);

    // Add text on top of the filled rectangle
    cv::putText(frame, caption, cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, fontScale, textColor, 2);
}

// Function to get a caption from the user
std::string getUserCaption() {
    std::string caption; // Declare a string variable to store the user's input
    std::cout << "Enter caption (press Enter to skip): "; // Prompt the user to enter a caption
    std::getline(std::cin, caption);     // Read a line of text from the standard input (keyboard) and store it in the 'caption' variable
    return caption;  // Return the user-provided caption
}

// Function to toggle the keepStrongColor effect
//int keepStrongColor(cv::Mat &frame, cv::Mat &outputFrame, bool &isEnabled, uchar threshold = 128);

int main(int argc, char *argv[])
{
  cv::VideoCapture *capdev;

  // Open the video device
  capdev = new cv::VideoCapture(0);
  if (!capdev->isOpened())
  {
    printf("Unable to open video device\n");
    return (-1);
  }

  // Get some properties of the image
  cv::Size refS((int)capdev->get(cv::CAP_PROP_FRAME_WIDTH),
                (int)capdev->get(cv::CAP_PROP_FRAME_HEIGHT));
  printf("Expected size: %d %d\n", refS.width, refS.height);

  cv::namedWindow("Video", 1); // Identify a window
  cv::Mat frame, greyFrame;
  std::vector<cv::Rect> faces; // Add this line for face detection

  cv::Mat sobelXFrame, sobelYFrame, gradientMagnitudeFrame; // Add gradientMagnitudeFrame
  char lastKey = ' ';                                       // To store the last key pressed

  cv::Mat quantizedFrame;      // Add a new Mat for quantized frame
  int quantizationLevels = 10; // Default quantization levels

  cv::Mat embossedFrame; // Add a new Mat for the embossed frame

  bool addCaptionMode = false;  // Flag to indicate whether the user is adding a caption
  bool applyVignetting = false; // Flag to determine whether to apply vignetting

  // void keepStrongColor(cv::Mat& src, cv::Mat& dst, cv::Scalar color); // Function to keep a specific color and set everything else to greyscale
  
  for (;;)
  {
    *capdev >> frame; // Capture a frame from the video device.

    // Check if the captured frame is empty.
    if (frame.empty())
    {
      printf("frame is empty\n");
      break;
    }

    // Check the value of the last key pressed and perform corresponding actions.
    if (lastKey == 'g')
    {
      // Convert the frame to greyscale.
      cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);
      // Add a caption to the greyscale frame.
      addCaption(greyFrame, "Greyscale", 10, 30, 1.0, cv::Scalar(0, 0, 0));
      // Display the greyscale frame.
      cv::imshow("Video", greyFrame);
    }
    else if (lastKey == 'h')
    {
      // Convert the frame to an alternate greyscale.
      greyscale(frame, greyFrame);
      // Add a caption to the alternate greyscale frame.
      addCaption(greyFrame, "Alternate_Greyscale", 10, 30, 1.0, cv::Scalar(0, 0, 0));
      // Display the alternate greyscale frame.
      cv::imshow("Video", greyFrame);
      
    } else if (lastKey == 'p') {   
      // If the user pressed 'p', apply sepia effect to the frame
      sepia(frame, greyFrame);
      // Add a caption "Sepia" to the frame at position (10, 30) with scale 1.0 and black color
      addCaption(greyFrame, "Sepia", 10, 30, 1.0, cv::Scalar(0, 0, 0));
      // Display the modified frame with the sepia effect  
      cv::imshow("Video", greyFrame);
    } else if (lastKey == 'p') {
      // If the user pressed 'v', toggle the vignetting flag
      sepia(frame, greyFrame);
      addCaption(greyFrame, "Sepia", 10, 30, 1.0, cv::Scalar(0, 0, 0));
      cv::imshow("Video", greyFrame);
    } else if (lastKey == 'v') {
            applyVignetting = !applyVignetting; // Toggle the vignetting flag

    } else if (lastKey == 'i') {
      // If the user pressed 'i', check if vignetting is enabled
        if (applyVignetting) { 
        // If vignetting is enabled, apply sepia effect with vignetting
            sepiaWithVignetting(frame, greyFrame);
        // Add a caption "sepia_vignett" to the frame at position (10, 30) with scale 1.0 and black color
            addCaption(greyFrame, "sepia_vignett", 10, 30, 1.0, cv::Scalar(0, 0, 0));
        // Display the modified frame with sepia effect and vignetting
            cv::imshow("Video", greyFrame);
        } else {
        // If vignetting is not enabled, apply sepia effect without vignetting
            sepia(frame, greyFrame);
        // Add a caption "sepia_vignett" to the frame at position (10, 30) with scale 1.0 and black color
            addCaption(greyFrame, "sepia_vignett", 10, 30, 1.0, cv::Scalar(0, 0, 0));
        // Display the modified frame with sepia effect only
            cv::imshow("Video", greyFrame);
        }
    } else if (lastKey == 'b') { // Call the blur function
      // Apply a blur filter to the frame.
      blur5x5_1(frame, greyFrame);
      // Add a caption to the blurred frame.
      addCaption(greyFrame, "Blur_1", 10, 30, 1.0, cv::Scalar(0, 0, 0));
      // Display the blurred frame.
      cv::imshow("Video", greyFrame);
    }
    else if (lastKey == 'n')
    { // Call the second blur function
      // Apply the second blur filter to the frame.
      blur5x5_2(frame, greyFrame);
      // Add a caption to the blurred frame.
      addCaption(greyFrame, "Blur_2", 10, 30, 1.0, cv::Scalar(0, 0, 0));
      // Display the blurred frame.
      cv::imshow("Video", greyFrame);
    }
    else if (lastKey == 'x')
    {
      // Apply Sobel X filter to the frame.
      sobelX3x3(frame, sobelXFrame);
      // Convert the result to unsigned char for display.
      cv::convertScaleAbs(sobelXFrame, sobelXFrame);
      // Add a caption to the Sobel X frame.
      addCaption(greyFrame, "Sobelx", 10, 30, 1.0, cv::Scalar(255, 255, 255));
      // Display the Sobel X frame.
      cv::imshow("Sobel X", sobelXFrame);
    }
    else if (lastKey == 'y')
    {
      // Apply Sobel Y filter to the frame.
      sobelY3x3(frame, sobelYFrame);
      // Convert the result to unsigned char for display.
      cv::convertScaleAbs(sobelYFrame, sobelYFrame);
      // Add a caption to the Sobel Y frame.
      addCaption(greyFrame, "Sobely", 10, 30, 1.0, cv::Scalar(255, 255, 255));
      // Display the Sobel Y frame.
      cv::imshow("Sobel Y", sobelYFrame);
    }
    else if (lastKey == 'm')
    {
      // Compute gradient magnitude.
      magnitude(sobelXFrame, sobelYFrame, gradientMagnitudeFrame);
      // Display the gradient magnitude frame.
      cv::imshow("Gradient Magnitude", gradientMagnitudeFrame);
    }
    else if (lastKey == 'f')
    {
      // Convert the frame to greyscale for face detection.
      cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);
      // Detect faces in the greyscale frame.
      detectFaces(greyFrame, faces);
      // Draw boxes around the detected faces.
      drawBoxes(frame, faces);
      // Display the frame with face detection.
      cv::imshow("Video", frame);
    }
    else if (lastKey == 'l')
    {
      // Apply blur and quantization.
      blurQuantize(frame, quantizedFrame, quantizationLevels);
      // Display the quantized frame.
      cv::imshow("Video", quantizedFrame);
    }
    // Inside the main loop
    else if (lastKey == 'u') {
    // Prompt user to enter new brightness and contrast values
    double newBrightness, newContrast;
    std::cout << "Enter new brightness value: ";
    std::cin >> newBrightness;
    std::cout << "Enter new contrast value: ";
    std::cin >> newContrast;

    // Update current brightness and contrast values
    currentBrightness = newBrightness;
    currentContrast = newContrast;

    // Apply brightness and contrast adjustments
    updateBrightnessContrast(frame, greyFrame, currentBrightness, currentContrast);
    
    // Add a caption to the adjusted frame.
    addCaption(greyFrame, "Brightness/Contrast", 10, 30, 1.0, cv::Scalar(0, 0, 0));
    
    // Display the adjusted frame.
    cv::imshow("Video", greyFrame);
}

    else if (lastKey == 'e')
    {
      // Apply emboss effect to the frame.
      embossEffect(frame, embossedFrame);
      // Add a caption to the embossed frame.
      addCaption(greyFrame, "Embossed_image", 10, 30, 1.0, cv::Scalar(0, 0, 0));
      // Display the embossed frame.
      cv::imshow("Video", embossedFrame);
    }
    /*else if (lastKey == 'o') {
            // Display a balloon when the 'o' key is pressed
            displayBalloon(frame, 100, 100);  // choose the position of balloon
        }
    else*/
    {
      // Display the original frame.
      cv::imshow("Video", frame);
    }

    // Wait for a key press event and perform corresponding actions.
    char key = cv::waitKey(10);
    if (key == 'q')
    { // Quit the loop if 'q' is pressed.
      break;
    }
    else if (key == 's')
    { // Save the current frame if 's' is pressed.
    if (addCaptionMode) {
            std::string userCaption = getUserCaption();
            if (!userCaption.empty()) {
              // Get user-specified caption position (you can modify this part based on your needs)
                    int x, y;
                    std::cout << "Enter X coordinate for caption: ";
                    std::cin >> x;
                    std::cout << "Enter Y coordinate for caption: ";
                    std::cin >> y;

            // Specify text color (white) and background color (black)
            addCaptionOnSave(frame, userCaption, x, y, 1.0, cv::Scalar(255, 255, 255), cv::Scalar(0, 0, 0));            
            }
        }
        cv::imwrite("caption_image.jpg", frame);
        addCaptionMode = false;  // Reset the flag after saving
    } else if (key == 'j') {
        addCaptionMode = !addCaptionMode;
    }  
    else if (key == 'g' || key == 'h' || key == 'j' || key == 'u' || key == 'p' || key == 'v' || key == 'i' || key == 'o' || key == 'b' || key == 'n' || key == 'x' || key == 'y' || key == 'm' || key == 'f' || key == 'l' || key == 'e' || key == 'a' || key == 't' || key == 'c')
    {
      lastKey = key; // Update the last key pressed if it matches certain keys.
    }
    else if (key == 'c')
    { // Show the original frame if 'c' is pressed
      cv::imshow("Video", frame);
    }
  }

  delete capdev;
  return (0); // Indicate successful execution.
}
