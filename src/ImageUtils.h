#include <string>
#include <curl/curl.h>
#include <iostream>
#include <opencv2/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui.hpp>

void downloadImage(std::string, unsigned id);
bool resampleImage(std::string filename, unsigned w, unsigned h);