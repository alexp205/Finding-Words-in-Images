#include "test.h"

int train_flag = 0;
std::wstring train_data_path = L"C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\Projects\\VisionSubsystem\\tb_test_images";
std::wstring train_labels_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\SVMImageClassifier\\labels.csv";
std::wstring image_dir_path = L"C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\Projects\\VisionSubsystem\\tb_test_images";
std::wstring test_data_path = L"C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\Projects\\VisionSubsystem\\tb_test_images";

std::string wstr_to_str(const std::wstring& wstr)
{
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.to_bytes(wstr);
}

int main(int argc, char *argv[])
{
    // process args
    if (3 != argc) {
        std::wcerr << L"Usage: SVMImageClassifier [-t train_flag]\n";
        std::wcerr << L"train_flag only accepts 1 (no training) or 2 (training)\n";
        return -1;
    }

    train_flag = atoi(argv[1]);
    if (train_flag < 1 || train_flag > 3) {
        std::wcerr << L"train_flag only accepts 1 (no training) or 2 (training)" << std::endl;
        return -1;
    }

	std::wcout << L"SVM object detector with SIFT and BoF\n\n";
    std::wcout << L"Scanning for training data at: " << train_data_path << "\n";
    std::wcout << L"Scanning for training labels at: " << train_labels_path << "\n";
	
    // run main procedure
    switch (train_flag) {
        case 1:
            {
            std::wcout << L"kmeans train_flag set, running dict generation version of program\n";

            // Mode 1: offline setup
            // make call to SVM_mgr to run kmeans training (*see SVM_mgr.cpp for more info)
            std::wcout << L"\nRunning generation procedure...\n";
            std::wstring dict_path = getTargetDescriptors(wstr_to_str(train_data_path), wstr_to_str(train_labels_path));
            if ((0 == dict_path.compare(L"fail")) || dict_path.empty()) {
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
            std::wstring model_path = trainSVM(wstr_to_str(image_dir_path));
            if ((0 == model_path.compare(L"fail")) || model_path.empty()) {
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
            std::vector<int> classes = classifyImage(wstr_to_str(test_data_path));
            if (classes.empty()) {
                std::wcerr << L"Classification failed" << std::endl;
                return -1;
            }

            std::wcout << L"SVM image classification completed with the following identifications:\n";
            int count = 0;
            for (int i = 0; i < classes.size(); i++) {
                std::wcout << classes[i] << L"\n";
                if (1 == classes[i]) count++;
            }
            std::wcout << L"Overall tennis ball images identified: " << count << L"\n";
            std::wcout << L"Overall tennis ball identification fraction: " << ((static_cast<double>(count + 1) / static_cast<double>(classes.size())) * 100) << L"\n";

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