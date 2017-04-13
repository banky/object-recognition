#include "CSVRow.h"
#include "ImageUtils.h"
#include "train.h"

#include <iostream>
#include <sys/stat.h>

#define WIDTH 100
#define HEIGHT 100

std::string VALIDATION_FILE = "../data/validation/images.csv";
std::string TRAINING_FILE = "../data/train/images.csv";
std::string TEST_FILE = "../data/test/images.csv";
std::string ANNOTATION_FILE = "../data/machine_ann/train/labels.csv";

unsigned NUM_EXAMPLES = 5000;
unsigned NUM_ANN_FOR_EXAMPLE = 5;
float CONFIDENCE_BOUND = 0.8;
unsigned NUM_TAGS = 7881;

using namespace std;

inline bool fileExists(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

float randomizer(string str) {
    // Converts each character of string into an integer
    // and adds them together
    float randomVal = 0;

    for (int i = 0; i < str.size(); i++)
    {
        randomVal += str[i];
    }

    // Make our number a little bit more unique
    randomVal *= 3.1415;
    randomVal += str.size() * (int)str[0];
    randomVal *= (int)str[str.size() - 1];
    randomVal /= 1.6180;

    return randomVal;
}

void downloadImages() {
    std::ifstream file(TRAINING_FILE);
    
    CSVRow row;

    file >> row;

    unsigned imageId = 0;

    while(file >> row)
    {
        if (!fileExists("../images/compressed/" + row[0] + ".jpg")) {
            char filename[50];
            bool thumbExists = downloadImage(row[10], row[0], "../images", filename);
            if (!thumbExists) {
                if (downloadImage(row[2], row[0], "../images", filename)) {
                    cout << "Thumb missing. Got original \n";
                } else {
                    cout << "Thumb and original missing \n";
                    cout << "Thumb link: " << row[10] << endl;
                    cout << "Orig link: " << row[2] << endl;
                    continue;
                }
            }

            // Resample image to a smaller size
            cv::Mat destImage;
            if (!resampleImage(filename, WIDTH, HEIGHT, destImage)) {
                cout << "File no longer exists\n";
                remove(filename);
                continue;
            }

            saveImage(destImage, row[0]);
            remove(filename);
        }
        

        imageId++;
        cout << imageId << endl;
    } 
    
    cout << "Done downloading";
}

void getExamples(Matrix &x, Matrix &y) {
    std::ifstream file(TRAINING_FILE);
    CSVRow row;

    file >> row;

    unsigned count = 0;

    while(file >> row && count < NUM_EXAMPLES) {
        string filename = "../images/compressed/" + row[0] + ".jpg";
        cv::Mat mat = getImage(filename);
        if (mat.empty()) {
            std::cout << "File doesn't exist\n";
            continue;
        }

        for(int i = 0; i < mat.rows; i++)
        {
            for(int j = 0; j < mat.cols; j++)
            {
                cv::Vec3b intensity = mat.at<cv::Vec3b>(i, j);
                int blue = (int)intensity.val[0];
                int green = (int)intensity.val[1];
                int red = (int)intensity.val[2];

                int index = i * 3 * mat.cols + 3 * j;
                x.data[count][index + 0] = blue;
                x.data[count][index + 1] = green;
                x.data[count][index + 2] = red;
            }
        }

        std::ifstream annotationFile(ANNOTATION_FILE);
        CSVRow annRow;

        annotationFile >> annRow;
        unsigned annCount = 0;

        while (annotationFile >> annRow && annCount < NUM_ANN_FOR_EXAMPLE) {
            float confidence = stof(annRow[3], nullptr);
            if (row[0] == annRow[0] && confidence >= CONFIDENCE_BOUND) {
                y.data[count][0] = randomizer(annRow[2]); 
            }
            annCount++;    
        }

        count++;
        // if (count % 100000 == 0) {
        //     cout << count;
        // }
    }
}

unsigned populateTags(Matrix & tags) {
    std::ifstream file(TRAINING_FILE);
    CSVRow row;

    file >> row;

    unsigned count = 0;

    while(file >> row && count < NUM_TAGS) {
        float tag = randomizer(row[0]);
        tags.data[count][0] = tag;

        count++;
    }

    return count;
}

int main() {
    Matrix x = Matrix(0, NUM_EXAMPLES, 3*WIDTH*HEIGHT);
    Matrix y = Matrix(0, NUM_EXAMPLES, 1);

    Matrix tags = Matrix(0, NUM_TAGS, 1);
    cout << "Hit 0\n";

    getExamples(x, y);
    populateTags(tags);
    cout << "Hit 1\n";
    unsigned numLayers = 2;
    static const unsigned arr[] = {x.numCols, 2000, tags.numRows};
	std::vector<unsigned> numel (arr, arr + sizeof(arr) / sizeof(arr[0]) );
    cout << "Hit 2\n";
    std::vector<Matrix> theta = randomInit(numLayers, numel);

    //Matrix x = Matrix(0, 5, 2);
 //    x.data[0][0] = 0;
 //    x.data[1][0] = 1;
 //    x.data[2][0] = 2;
 //    x.data[3][0] = 3;
 //    x.data[4][0] = 4;
 //    x.data[0][1] = 0;
 //    x.data[1][1] = 1;
 //    x.data[2][1] = 2;
 //    x.data[3][1] = 3;
 //    x.data[4][1] = 4;

 //    //Matrix y = Matrix(0, 5, 1);
	// y.data[0][0] = 0;
 //    y.data[1][0] = 2;
 //    y.data[2][0] = 4;
 //    y.data[3][0] = 6;
 //    y.data[4][0] = 8;

 //    Matrix tags = Matrix(0, 5, 1);
	// tags.data[0][0] = 0;
 //    tags.data[1][0] = 2;
 //    tags.data[2][0] = 4;
 //    tags.data[3][0] = 6;
 //    tags.data[4][0] = 8;

// gradientDescent(std::vector<Matrix> theta, unsigned m, unsigned numLayers, 
// 	double alpha, Matrix x, Matrix y, Matrix tags, double lambda)
    
    //x.print();
    // theta[0].print();
    // theta[1].print();

    cout << "Hit 3\n";
    theta = gradientDescent(theta, NUM_EXAMPLES, 2, 1, x, y, tags, 0);
    exit(0);
    //theta[0].print();
    //theta[1].print();
    //x.print();

    std::vector<Matrix> a = forwardProp(x.getRow(0).transpose(), theta, numLayers);
    a[numLayers].print();
}

	//tbt arr = std::vector< std::vector<double> > (2, std::vector<double>(2, 3));
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
