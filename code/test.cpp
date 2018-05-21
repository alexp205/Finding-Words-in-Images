#include "test.h"

int train_flag = 0;
std::wstring train_data_path = L"C:\\Users\\ap\\Documents\\School\\Undergraduate\\Robotics\\Autonomy\\tennis_ball.jpg";
std::wstring train_labels_path = L"";

std::string wstr_to_str(const std::wstring& wstr)
{
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.to_bytes(wstr);
}

int main(int argc, char *argv[])
{
    // process args
    if (2 != argc) {
        std::wcerr << L"Usage: SVMImageClassifier [-t train_flag]\n";
        std::wcerr << L"train_flag only accepts 1 (no training) or 2 (training)" << std::endl;
        return -1;
    }

    train_flag = atoi(argv[1]);
    if (train_flag < 1 || train_flag > 2) {
        std::wcerr << L"train_flag only accepts 1 (no training) or 2 (training)" << std::endl;
        return -1;
    }

	std::wcout << L"SVM object detector with SIFT and BoF\n\n";
    std::wcout << L"Scanning for training data at: " << train_data_path << "\n";
    std::wcout << L"Scanning for training labels at: " << train_labels_path << "\n";
	
    // run main procedure
    if (1 == train_flag) {
        std::wcout << L"train_flag set, running training version of program\n";
        
        // Mode 1: offline setup
        // make call to SVM_mgr to run training (*see SVM_mgr.cpp for more info)
        std::wcout << L"Running training procedure...\n";
        std::wstring svm_model_path = setupClassifier(wstr_to_str(train_data_path), wstr_to_str(train_labels_path));
        if (svm_model_path.compare(L"fail") == 0) {
            std::wcerr << L"Training failed" << std::endl;
            return -1;
        }

        std::wcout << L"Training completed, the trained SVM model file can be found at:\n";
        std::wcout << svm_model_path << "\n";
    } else {
        std::wcout << L"train_flag NOT set, running classification version of program\n";

        // Mode 2: online classification
        // make call to SVM_mgr to classify image
        std::wcout << L"Running classification procedure...\n";
        int success = classifyImg();
        if (0 != success) {
            std::wcerr << L"Classification failed" << std::endl;
            return -1;
        }


    }

    std::wcout << L"Exiting" << std::endl;

	return 0;
}