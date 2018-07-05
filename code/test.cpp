#include "test.h"

int train_flag = 0;
/*
std::wstring vocab_data_path = L"C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\Projects\\VisionSubsystem\\VisionSubsystem\\Training Data\\tennis-ball-dataset\\less_noise_pics";
std::wstring vocab_data_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\resources\\SVMImageClassifier\\train_dict_temp";
std::wstring svm_train_path = L"C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\Projects\\VisionSubsystem\\temp_data\\alex-training-data";
std::wstring svm_train_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\resources\\SVMImageClassifier\\train_svm_temp";
std::wstring svm_train_labels_path = L"C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\Projects\\VisionSubsystem\\temp_data\\train-labels.csv";
std::wstring svm_train_labels_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\resources\\SVMImageClassifier\\train_labels_svm_temp.csv";
std::wstring test_data_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\resources\\SVMImageClassifier\\test";

std::wstring dict_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\SVMImageClassifier\\kmeans_dict.yml";
std::wstring model_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\SVMImageClassifier\\svm_model";
*/

/*
* Args: 
*   1. [int] flag determining which procedure to run
*
*   NOTE: the expected args change based on the passed in value for the first argument
*   if [1] == 1:
*     2. [string] the path to the training data directory for the vocabulary generator
*     3. [string] the path to/for the generated vocabulary file
*
*   if [1] == 2:
*     2. [string] the path to the training data directory for the svm
*     3. [string] the path to the training data labels file for the svm
*     4. [string] the path to/for the generated vocabulary file
*     5. [string] the path to/for the generated svm model file
*
*   if [1] == 3:
*     2. [string] the path to the testing data directory
*     3. [string] the path to/for the generated vocabulary file
*     4. [string] the path to/for the generated svm model file
*
* sample arg 1:
* sample arg 2: 2 C:\Users\ap\Documents\Projects\Programs\AI\resources\SVMImageClassifier\train_svm_temp C:\Users\ap\Documents\Projects\AI\resources\SVMImageClassifier\train_labels_svm_temp.csv C:\Users\ap\Documents\Projects\Programs\AI\SVMImageClassifier\kmeans_dict.yml C:\Users\ap\Documents\Projects\Programs\AI\SVMImageClassifier\svm_model
* sample arg 3:
*/
int main(int argc, char *argv[])
{
    // process args
    if (argc < 2) {
        std::wcerr << L"Usage: SVMImageClassifier [train_flag] -> additional arguments\n";
        std::wcerr << L"train_flag only accepts 1 (generate dictionaries), 2 (train SVM), or 3 (classify images)\n";
        std::wcerr << L"NOTE: see comments for details" << std::endl;
        return -1;
    }

    train_flag = atoi(argv[1]);
    if (train_flag < 1 || train_flag > 3) {
        std::wcerr << L"train_flag only accepts 1 (generate dictionaries), 2 (train SVM), or 3 (classify images)" << std::endl;
        return -1;
    }

	std::wcout << L"SVM object detector with SIFT and BoF\n\n";
	
    // run main procedure
    switch (train_flag) {
        case 1:
            {
            std::wcout << L"kmeans train_flag set, running vocab generation version of program\n";

            if (4 != argc) {
                std::wcerr << L"Usage for dict generation: SVMImageClassifier 1 [vocab_data_path] [dict_path]" << std::endl;
                return -1;
            }

            std::string vocab_data_path = argv[2];
            std::string dict_path = argv[3];

            // Mode 1: offline setup
            // make call to SVM_mgr to run kmeans training (*see SVM_mgr.cpp for more info)
            std::wcout << L"\nRunning generation procedure...\n";
            int success = getTargetMap(vocab_data_path, dict_path);
            if (0 != success) {
                std::wcerr << L"Descriptor dict generation failed" << std::endl;
                return -1;
            }

            std::wcout << L"Descriptor dict generation completed, the dict file can be found at:\n";
            std::cout << dict_path << "\n";

            break;
            }
        case 2:
            {
            std::wcout << L"SVM train_flag set, running svm training version of program\n";

            if (6 != argc) {
                std::wcerr << L"Usage for svm training: SVMImageClassifier 1 [svm_train_path] [svm_train_labels_path] [dict_path] [model_path]" << std::endl;
                return -1;
            }

            std::string svm_train_path = argv[2];
            std::string svm_train_labels_path = argv[3];
            std::string dict_path = argv[4];
            std::string model_path = argv[5];

            // Mode 2: SVM training
            // make call to SVM_mgr to run SVM training
            std::wcout << L"\nRunning training procedure...\n";
            int success = trainSVM(svm_train_path, svm_train_labels_path, dict_path, model_path);
            if (0 != success) {
                std::wcerr << L"SVM training failed" << std::endl;
                return -1;
            }

            std::wcout << L"SVM model generation completed, the trained model file can be found at:\n";
            std::cout << model_path << "\n";

            break;
            }
        case 3:
            {
            std::wcout << L"train_flag NOT set, running classification version of program\n";

            if (5 != argc) {
                std::wcerr << L"Usage for svm training: SVMImageClassifier 1 [test_data_path] [dict_path] [model_path]" << std::endl;
                return -1;
            }

            std::string test_data_path = argv[2];
            std::string dict_path = argv[3];
            std::string model_path = argv[4];

            // Mode 2: SVM training
            // make call to SVM_mgr to run SVM training
            std::wcout << L"\nRunning classification procedure...\n";
            std::vector<double> classes = classifyImages(test_data_path, dict_path, model_path);
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