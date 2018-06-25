#include "test.h"

int train_flag = 0;
//std::wstring train_data_path = L"C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\Projects\\VisionSubsystem\\VisionSubsystem\\Training Data\\tennis-ball-dataset\\less_noise_pics";
std::wstring train_data_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\resources\\SVMImageClassifier\\train_dict_temp";
//std::wstring image_dir_path = L"C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\Projects\\VisionSubsystem\\temp_data\\alex-training-data";
std::wstring image_dir_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\resources\\SVMImageClassifier\\train_svm_temp";
//std::wstring image_dir_labels = L"C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\Projects\\VisionSubsystem\\temp_data\\train-labels.csv";
std::wstring image_dir_labels = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\resources\\SVMImageClassifier\\train_labels_svm_temp.csv";
std::wstring test_data_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\resources\\SVMImageClassifier\\test";

std::wstring dict_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\SVMImageClassifier\\kmeans_dict.yml";
std::wstring model_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\SVMImageClassifier\\svm_model";

int main(int argc, char *argv[])
{
    // process args
    if (2 != argc) {
        std::wcerr << L"Usage: SVMImageClassifier [-t train_flag]\n";
        std::wcerr << L"train_flag only accepts 1 (generate dictionaries), 2 (train SVM), or 3 (classify images)\n";
        return -1;
    }

    train_flag = atoi(argv[1]);
    if (train_flag < 1 || train_flag > 3) {
        std::wcerr << L"train_flag only accepts 1 (generate dictionaries), 2 (train SVM), or 3 (classify images)" << std::endl;
        return -1;
    }

	std::wcout << L"SVM object detector with SIFT and BoF\n\n";
    std::wcout << L"Scanning for training data at: " << image_dir_path << "\n";
    std::wcout << L"Scanning for training labels at: " << image_dir_labels << "\n";
    std::wcout << L"Scanning for testing data at: " << test_data_path << "\n";
	
    // run main procedure
    switch (train_flag) {
        case 1:
            {
            std::wcout << L"kmeans train_flag set, running dict generation version of program\n";

            // Mode 1: offline setup
            // make call to SVM_mgr to run kmeans training (*see SVM_mgr.cpp for more info)
            std::wcout << L"\nRunning generation procedure...\n";
            int success = getTargetMap(wstr_to_str(train_data_path), wstr_to_str(dict_path));
            if (0 != success) {
                std::wcerr << L"Descriptor dict generation failed" << std::endl;
                return -1;
            }

            std::wcout << L"Descriptor dict generation completed, the dict file can be found at:\n";
            std::wcout << dict_path << "\n";

            break;
            }
        case 2:
            {
            std::wcout << L"SVM train_flag set, running training version of program\n";

            // Mode 2: SVM training
            // make call to SVM_mgr to run SVM training
            std::wcout << L"\nRunning training procedure...\n";
            int success = trainSVM(wstr_to_str(image_dir_path), wstr_to_str(image_dir_labels), wstr_to_str(dict_path), wstr_to_str(model_path));
            if (0 != success) {
                std::wcerr << L"SVM training failed" << std::endl;
                return -1;
            }

            std::wcout << L"SVM model generation completed, the trained model file can be found at:\n";
            std::wcout << model_path << "\n";

            break;
            }
        case 3:
            {
            std::wcout << L"train_flag NOT set, running classification version of program\n";

            // Mode 2: SVM training
            // make call to SVM_mgr to run SVM training
            std::wcout << L"\nRunning classification procedure...\n";
            std::vector<double> classes = classifyImages(wstr_to_str(test_data_path), wstr_to_str(dict_path), wstr_to_str(model_path));
            if (classes.empty()) {
                std::wcerr << L"Classification failed" << std::endl;
                return -1;
            }

            std::wcout << L"NOTE: 1 = tennis ball detected, 0 = not\n";
            std::wcout << L"SVM image classification completed with the following identifications:\n";
            int count = 0;
            for (int i = 0; i < classes.size(); i++) {
                std::wcout << L"image " << i+1 << L": " << classes[i] << L"\n";
                if (1.0 == classes[i]) count++;
            }
            std::wcout << L"Overall tennis ball images identified: " << count << L"\n";
            std::wcout << L"Overall tennis ball identification fraction: " << ((static_cast<double>(count) / static_cast<double>(classes.size())) * 100) << L"\n";

            break;
            } 
        default:
        {
            break;
        }
    }

    std::wcout << L"Exiting" << std::endl;

	return 0;
}