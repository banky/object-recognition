#include "ImageUtils.h"

void downloadImage(char * url) {
	CURL *image; 
	CURLcode imgResult; 
	FILE *fp = NULL; 

	image = curl_easy_init(); 
	if(image){ 
	    // Open file 
	    fp = fopen("tmp.jpg", "w"); 
	    if(fp == NULL) {
	    	std::cout << "File cannot be opened\n";
	    }

	    //cUrl setup
	    curl_easy_setopt(image, CURLOPT_URL, url); 
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