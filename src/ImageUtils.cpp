#include "ImageUtils.h"

/*
 * Downloads an image specified by the url to a file
 */
void downloadImage(std::string url, unsigned id) {
	CURL *image; 
	CURLcode imgResult; 
	FILE *fp = NULL; 

	image = curl_easy_init(); 
	if(image){ 
	    // Open file
	    char * filename;
	    sprintf(filename, "../images/img%u.jpg", id);
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
	    }
	} 

	// Clean up the resources 
	curl_easy_cleanup(image); 
	// Close the file 
	if (fp) {
		fclose(fp);
	}
}

/*
 * Resamples a specified image file to the provided dimensions
 */
bool resampleImage(std::string filename, unsigned w, unsigned h, Mat & destImage) {
	cv::Mat srcImage = cv::imread(filename, cv::IMREAD_COLOR);

	if (srcImage.empty()) {
		std::cout << "Could not open the image file\n";
		return false;
	}

	cv::Size size(w,h);
	cv::Mat destImage;
	cv::resize(srcImage, destImage, size);
	
    return true;
}

/*
 * Displays image specified by Mat object
 */
void displayImage(cv::Mat img) {
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    cv::imshow( "Display window", destImage );
    cv::waitKey(0);
}
