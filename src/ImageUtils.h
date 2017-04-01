#include <string>
#include <curl/curl.h>
#include <iostream>
#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui.hpp>

char * downloadImage(std::string, unsigned id, std::string path);
bool resampleImage(std::string filename, unsigned w, unsigned h, cv::Mat & destImage);
void displayImage(cv::Mat img);