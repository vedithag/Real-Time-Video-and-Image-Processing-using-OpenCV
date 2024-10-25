#ifndef FILTER_HPP
#define FILTER_HPP

#include <opencv2/opencv.hpp>
//#include "State.h"

// Declaration of the greyscale function with reference arguments
int greyscale(const cv::Mat& src, cv::Mat& dst);
int sepia(const cv::Mat& src, cv::Mat& dst);
int sepiaWithVignetting(const cv::Mat& src, cv::Mat& dst);
int blur5x5_1( cv::Mat& src, cv::Mat& dst);
int blur5x5_2( cv::Mat &src, cv::Mat &dst);
int sobelX3x3( cv::Mat &src, cv::Mat &dst );
int sobelY3x3( cv::Mat &src, cv::Mat &dst );
int magnitude( cv::Mat &sx, cv::Mat &sy, cv::Mat &dst );
int blurQuantize( cv::Mat& src, cv::Mat& dst, int levels);
int embossEffect( cv::Mat &src, cv::Mat &dst);
void addCaption(cv::Mat& frame, const std::string& caption, int x, int y, double fontScale, cv::Scalar color);
void addCaptionOnSave(cv::Mat& frame, const std::string& caption, int x, int y, double fontScale, cv::Scalar textColor, cv::Scalar bgColor);
std::string getUserCaption();

//void displayBalloon(cv::Mat& frame, int x, int y);
//extern int pickStrongColor(cv::Mat& src, cv::Mat& dst, uchar threshold) ;//int blurOutsideFaces(const cv::Mat& src, cv::Mat& dst, const std::vector<cv::Rect>& faces);
//int keepStrongColor(cv::Mat& frame, cv::Mat& outputFrame, bool& isEnabled, uchar threshold = 128);

#endif // FILTER_HPP
