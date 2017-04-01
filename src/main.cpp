#include "CSVRow.h"
#include "ImageUtils.h"

#include <iostream>

#define WIDTH = 100;
#define HEIGHT = 100;

std::string VALIDATION_FILE = "../data/validation/images.csv";
std::string TRAINING_FILE = "../data/train/images.csv";
std::string TEST_FILE = "../data/test/images.csv";


int main() {

    std::ifstream file(TEST_FILE);

    CSVRow row;

    //downloadImage("https://c1.staticflickr.com/5/4129/5215831864_46f356962f_o.jpg");
    
    // Get rid of the first row
    file >> row;

    unsigned imageId = 0;
    file >> row;
    while(file >> row)
    {
    	char * filename = downloadImage(row[2], imageId, "../images");

    	// Resample image to a smaller size
    	cv::Mat destImage;
    	resampleImage(filename, WIDTH, HEIGHT, destImage);

    	
    	//imageId++;


        //cout << "4th Element(" << row[2] << ")\n";
    } 
}