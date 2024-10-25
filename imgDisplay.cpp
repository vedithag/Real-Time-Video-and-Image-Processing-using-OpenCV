/*
  Veditha Gudapati
  Process image frames using different keypresses
*/


#include <opencv2/opencv.hpp>

int main() {
    // Read an image from a file
    cv::Mat image = cv::imread("/home/vedit/PRCV/Project_1/imgDisplay.jpg");

    // Check if the image was successfully loaded
    if (image.empty()) {
        std::cerr << "Error: Could not read the image." << std::endl;
        return -1;
    }

    // Display the image in a window
    cv::namedWindow("Image Display", cv::WINDOW_NORMAL);  // Create a resizable window
    cv::imshow("Image Display", image);

    // Resize the window to match the image dimensions
    cv::resizeWindow("Image Display", image.cols, image.rows);

    int rotationAngle = 0;

    // Main loop
    while (true) {
        // Check for a keypress
        char key = cv::waitKey(10);

        // Quit if the user presses 'q'
        if (key == 'q') {
            break;
        }

        // Additional functionality based on keypresses
        switch (key) {
            case 's':
                // Save the image
                cv::imwrite("imgDisplay.jpg", image);
                std::cout << "Image saved." << std::endl;
                break;
            case 'd':
                // Reload and display the original image
                image = cv::imread("/home/kasturi/PRCV/Project_1/imgDisplay.jpg");
                cv::imshow("Image Display", image);
            case 'l':
                // Rotate the image left
                rotationAngle = (rotationAngle - 90) % 360;
                cv::rotate(image, image, cv::ROTATE_90_COUNTERCLOCKWISE);
                cv::imshow("Image Display", image);
                break;
            case 'r':
                // Rotate the image right
                rotationAngle = (rotationAngle + 90) % 360;
                cv::rotate(image, image, cv::ROTATE_90_CLOCKWISE);
                cv::imshow("Image Display", image);
                break;
            case 'h':
                // Display help information
                std::cout << "Press 'q' to quit." << std::endl;
                std::cout << "Press 's' to save the image." << std::endl;
                std::cout << "Press 'l' to rotate the image left." << std::endl;
                std::cout << "Press 'r' to rotate the image right." << std::endl;
                std::cout << "Press 'r' to reload the image." << std::endl;
                break;
        }
    }

    return 0;
}
