#include "ImageUtils.h"

/*
 * Downloads an image specified by the url to a file
 */
bool downloadImage(std::string url, std::string id, std::string path, char * filename) {
	CURL *image; 
	CURLcode imgResult; 
	FILE *fp = NULL;

	image = curl_easy_init(); 
	if(image) {
	    sprintf(filename, "%s/%s.jpg", path.c_str(), id.c_str());

	    fp = fopen(filename, "w"); 
	    if(fp == NULL) {
	    	std::cout << "File cannot be opened\n";
	    }
	    
	    //cUrl setup
	    curl_easy_setopt(image, CURLOPT_URL, url.c_str()); 
	    curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL); 
	    curl_easy_setopt(image, CURLOPT_WRITEDATA, fp); 

	    // Grab image 
        imgResult = curl_easy_perform(image); 
	    if(imgResult){ 
	        std::cout << "Cannot grab the image! Error code : " << imgResult << "\n"; 
	        return false;
	    }
	}
	

	// Clean up the resources 
	curl_easy_cleanup(image); 
	// Close the file 
	if (fp) {
		fclose(fp);
	}
	return true;
}

/*
 * Resamples a specified image file to the provided dimensions
 */
bool resampleImage(std::string filename, unsigned w, unsigned h, cv::Mat & destImage) {
	cv::Mat srcImage = cv::imread(filename, cv::IMREAD_COLOR);

	if (srcImage.empty()) {
		std::cout << "Could not open the image file\n";
		return false;
	}
	cv::Size size(w,h);
	cv::resize(srcImage, destImage, size);
    return true;
}

/*
 * Displays image specified by Mat object
 */
void displayImage(cv::Mat img) {
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display window", img);
    cv::waitKey(0);
}

bool saveImage(cv::Mat img, std::string id) {
	std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(50);
    char filename[50];

    sprintf(filename, "../images/compressed/%s.jpg", id.c_str());

	try {
        imwrite(filename, img, compression_params);
    }
    catch (std::runtime_error& ex) {
        fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
        return false;
    }

    return true;
}
