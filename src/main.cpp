#include "CSVRow.h"
#include "ImageUtils.h"
#include "train.h"

#include <iostream>

#define WIDTH = 100;
#define HEIGHT = 100;

std::string VALIDATION_FILE = "../data/validation/images.csv";
std::string TRAINING_FILE = "../data/train/images.csv";
std::string TEST_FILE = "../data/test/images.csv";


int main() {

    std::ifstream file(TEST_FILE);

    CSVRow row;

    
    unsigned numLayers = 2;
    static const unsigned arr[] = {1, 5, 5};
	std::vector<unsigned> numel (arr, arr + sizeof(arr) / sizeof(arr[0]) );

    std::vector<Matrix> theta = randomInit(numLayers, numel);

    Matrix x = Matrix(0, 5, 1);
    x.data[0][0] = 0;
    x.data[1][0] = 1;
    x.data[2][0] = 2;
    x.data[3][0] = 3;
    x.data[4][0] = 4;

    Matrix y = Matrix(0, 5, 1);
	y.data[0][0] = 0;
    y.data[1][0] = 2;
    y.data[2][0] = 4;
    y.data[3][0] = 6;
    y.data[4][0] = 8;

    Matrix tags = Matrix(0, 5, 1);
	tags.data[0][0] = 0;
    tags.data[1][0] = 2;
    tags.data[2][0] = 4;
    tags.data[3][0] = 6;
    tags.data[4][0] = 8;

// gradientDescent(std::vector<Matrix> theta, unsigned m, unsigned numLayers, 
// 	float alpha, Matrix x, Matrix y, Matrix tags, float lambda)
    
    theta = gradientDescent(theta, 5, 2, 0.1, x, y, tags, 0);
}

	//tbt arr = std::vector< std::vector<float> > (2, std::vector<float>(2, 3));
    //downloadImage("https://c1.staticflickr.com/5/4129/5215831864_46f356962f_o.jpg");
    
    // Get rid of the first row
    // file >> row;

    // unsigned imageId = 0;
    // file >> row;
    // while(file >> row)
    // {
    // 	char * filename = downloadImage(row[2], imageId, "../images");

    // 	// Resample image to a smaller size
    // 	cv::Mat destImage;
    // 	resampleImage(filename, WIDTH, HEIGHT, destImage);


    // 	//imageId++;


    //     //cout << "4th Element(" << row[2] << ")\n";
    // } 