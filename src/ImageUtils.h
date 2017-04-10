#include <string>
#include <curl/curl.h>
#include <iostream>
#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui.hpp>

bool downloadImage(std::string, std::string id, std::string path, char * filename);
bool resampleImage(std::string filename, unsigned w, unsigned h, cv::Mat & destImage);
void displayImage(cv::Mat img);
bool saveImage(cv::Mat img, std::string id);
std::vector<std::vector<float> > matToVector(cv::Mat img);
