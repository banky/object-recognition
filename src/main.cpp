#include "CSVRow.h"
#include "ImageUtils.h"

#include <iostream>

using namespace std;

const string VALIDATION_FILE = "../data/validation/images.csv";
const string TRAINING_FILE = "../data/train/images.csv";

int main() {

    std::ifstream file("");

    CSVRow row;

    downloadImage("https://c1.staticflickr.com/5/4129/5215831864_46f356962f_o.jpg");
    // while(file >> row)
    // {
    //     cout << "4th Element(" << row[3] << ")\n";
    // } 
}