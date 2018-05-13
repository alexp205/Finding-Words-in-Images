#include "SVM_Test.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

/*
Tutorial Code:

// Set up SVM for OpenCV 3
Ptr<SVM> svm = SVM::create();
// Set SVM type
svm->setType(SVM::C_SVC);
// Set SVM Kernel to Radial Basis Function (RBF)
svm->setKernel(SVM::RBF);
// Set parameter C
svm->setC(12.5);
// Set parameter Gamma
svm->setGamma(0.50625);

// Train SVM on training data
Ptr<TrainData> td = TrainData::create(trainData, ROW_SAMPLE, trainLabels);
svm->train(td);

// Save trained model
svm->save("digits_svm_model.yml");

// Test on a held out test set
svm->predict(testMat, testResponse);
*/

int main()
{
	std::cout << L"Welcome to the Amelio SVM demonstration\n\n";
	std::cout << L"General procedure:\n";
	std::cout << L"1. Read in picture(s)\n 2. Pre-processing (if necessary)\n 3. Feature extraction\n 4. Training\n\n";

	//Step 1: read in picture(s)
	std::cout << L"--- Read in picture(s) ---\n";
	std::string image_path("C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\tennis_ball.jpg");
	cv::Mat image;
	image = cv::imread(image_path, cv::IMREAD_COLOR);

	if (image.empty()) {
		std::cout << "invalid input\n";
		return -1;
	}

	cv::imshow("original image", image);
	cv::waitKey(0);
	
	//Step 2: pre-process
	std::cout << L"--- Pre-process image(s) ---\n";


	//Step 3: extract features
	std::cout << L"--- Extract features ---\n";
	

	//Step 4: train SVM
	std::cout << L"--- SVM ops ---\n";


	return 0;
}