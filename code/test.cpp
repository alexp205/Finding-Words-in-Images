#include "test.h"

int train_flag = 0;
/*
for DEBUG:
C:\Users\ap\Documents\Projects\Programs\AI\resources\SVMImageClassifier\train_dict_temp
C:\Users\ap\Documents\Projects\Programs\AI\resources\SVMImageClassifier\train_svm_temp
C:\Users\ap\Documents\Projects\Programs\AI\resources\SVMImageClassifier\train_labels_svm_temp.csv

for FINAL:
C:\Users\ap\Documents\Projects\Programs\Tutorials\google_ml_videos\RelatedTutorials\tensorflow-for-poets-2\tf_files\flower_photos\all
C:\Users\ap\Documents\Projects\Programs\Tutorials\google_ml_videos\RelatedTutorials\tensorflow-for-poets-2\tf_files\flower_photos\all
C:\Users\ap\Documents\Projects\Programs\AI\resources\SVMImageClassifier\train_labels_svm.csv

for BOTH:
C:\Users\ap\Documents\Projects\Programs\AI\resources\SVMImageClassifier\test
C:\Users\ap\Documents\Projects\Programs\AI\SVMImageClassifier\kmeans_dict.yml
C:\Users\ap\Documents\Projects\Programs\AI\SVMImageClassifier\svm_model
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
* sample arg 1: 1 C:\Users\ap\Documents\Projects\Programs\AI\resources\SVMImageClassifier\train_dict_temp C:\Users\ap\Documents\Projects\Programs\AI\SVMImageClassifier\kmeans_dict_pending.yml
* sample arg 2: 2 C:\Users\ap\Documents\Projects\Programs\AI\resources\SVMImageClassifier\train_svm_temp C:\Users\ap\Documents\Projects\Programs\AI\resources\SVMImageClassifier\train_labels_svm_temp.csv C:\Users\ap\Documents\Projects\Programs\AI\SVMImageClassifier\kmeans_dict.yml C:\Users\ap\Documents\Projects\Programs\AI\SVMImageClassifier\svm_model
* sample arg 3: 3 C:\Users\ap\Documents\Projects\Programs\AI\resources\SVMImageClassifier\test C:\Users\ap\Documents\Projects\Programs\AI\SVMImageClassifier\kmeans_dict.yml C:\Users\ap\Documents\Projects\Programs\AI\SVMImageClassifier\svm_model
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

            // Mode 3: prediction
            // make call to SVM_mgr to predict image classes
            std::wcout << L"\nRunning classification procedure...\n";
            std::vector<double> classes = classifyImages(test_data_path, dict_path, model_path);
            if (classes.empty()) {
                std::wcerr << L"Classification failed" << std::endl;
                return -1;
            }

            std::wcout << L"NOTE: 0 = daisy, 1 = dandelion, 2 = rose, 3 = sunflower, 4 = tulip\n";
            std::wcout << L"SVM image classification completed with the following identifications:\n";
            int daisy_count = 0;
            int dandelion_count = 0;
            int rose_count = 0;
            int sunflower_count = 0;
            int tulip_count = 0;
            for (int i = 0; i < classes.size(); i++) {
                int detected_class = (int)(classes[i] + 0.5);
                std::wcout << L"image " << i+1 << L": " << detected_class << L"\n";
                switch (detected_class) {
                    case 0:
                    {
                        daisy_count++;
                        break;
                    }
                    case 1:
                    {
                        dandelion_count++;
                        break;
                    }
                    case 2:
                    {
                        rose_count++;
                        break;
                    }
                    case 3:
                    {
                        sunflower_count++;
                        break;
                    }
                    case 4:
                    {
                        tulip_count++;
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            std::wcout << L"Overall daisy images identified: " << daisy_count << L"\n";
            std::wcout << L"Overall dandelion images identified: " << dandelion_count << L"\n";
            std::wcout << L"Overall rose images identified: " << rose_count << L"\n";
            std::wcout << L"Overall sunflower images identified: " << sunflower_count << L"\n";
            std::wcout << L"Overall tulip images identified: " << tulip_count << L"\n";

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