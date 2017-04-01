#include "CSVRow.h"
#include "ImageUtils.h"

#include <iostream>

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
    //while(file >> row)
    //{
    	downloadImage(row[2], imageId);

    	resampleImage("../images/img0.jpg", 100, 100);

    	//imageId++;


        //cout << "4th Element(" << row[2] << ")\n";
    //} 
}