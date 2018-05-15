#include "SVM_mgr.h"

image_data::image_data(int width, int height)
{
	data = new pixel*[height];
	for (int i = 0; i < height; i++)
		data[i] = new pixel[width];

	this->width = width;
	this->height = height;
}

image_data::~image_data()
{
	if (data)
	{
		for (int i = 0; i < width; i++)
		{
			delete[] data[i];
		}

		delete[] data;
	}
}

pixel** image_data::get_data() const
{
	return data;
}

int image_data::get_height() const
{
	return this->height;
}

int image_data::get_width() const
{
	return this->width;
}

std::wstring setupClassifier(std::wstring train_data_path, std::wstring train_labels_path)
{
    /*
    //EXAMPLE_CODE
    //------------
    cv::Ptr<cv::Feature2D> detector = cv::xfeatures2d::SIFT::create();
    std::vector<cv::KeyPoint> kpts;
    detector->detect(temp, kpts);
    cv::Mat descriptor;
    detector->compute(temp, kpts, descriptor);
    */

    // setup
    cv::Mat input;
    std::vector<cv::KeyPoint> keypts;
    cv::Mat descriptor;
    cv::Mat img_descriptors;
    

    // Step 1: read in images and labels
    std::wcout << L"Reading in picture(s)...\n";
    cv::Mat image;
    image = cv::imread(image_path, cv::IMREAD_COLOR);

    if (image.empty()) {
        std::wcout << L"invalid input\n";
        exit(EXIT_FAILURE);
    }

    cv::imshow("original image", image);
    cv::waitKey(0);

    // Step 2: extract SIFT features


    // Step 3: use features to get descriptors


    // Step 4: train SVM to recognize descriptors

    
    // Step 5: save SVM

}

int classifyImg()
{
    // setup


    // Step 1: extract SIFT features


    // Step 2: use features to get descriptors


    // Step 3: use SVM to classify img based on descriptors

    
}