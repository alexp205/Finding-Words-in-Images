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

std::wstring getTargetDescriptors(std::string train_data_path, std::string train_labels_path)
{
    // setup
    // image file reading
    std::vector<std::string> files;
    DIR *img_dir;
    struct dirent *img_dir_rdr;

    // image processing and BoF
    cv::Ptr<cv::Feature2D> detector;
    cv::Mat img_descriptors;
   
    // Step 1: read in images and labels
    std::wcout << L"Reading in picture(s)...\n";
    if (NULL == (img_dir = opendir(train_data_path.c_str()))) {
        std::wcout << L"error opening image directory\n";
        return L"fail";
    }

    while (NULL != (img_dir_rdr = readdir(img_dir))) {
        std::string img_fname = std::string(img_dir_rdr->d_name);
        if (img_fname.size() > 4) {

            // DEBUG
            std::cout << "image filename: " << img_fname << "\n";

            files.push_back(img_fname);
        }
    }
    closedir(img_dir);

    detector = cv::xfeatures2d::SIFT::create();

    for (int i = 0; i < files.size(); i++) {
        std::string img_fpath = train_data_path + "\\" + files[i];

        // DEBUG
        std::cout << "image filepath: " << img_fpath << "\n";

        cv::Mat img = cv::imread(img_fpath, cv::IMREAD_GRAYSCALE);

        if (img.empty()) {
            std::wcout << L"invalid input\n";
            return L"fail";
        }

        // DEBUG
        cv::imshow("original image", img);
        cv::waitKey(0);

        // Step 2:
        std::vector<cv::KeyPoint> keypts;
        cv::Mat descriptor;

        // extract SIFT features and get descriptors
        detector->detect(img, keypts);
        detector->compute(img, keypts, descriptor);
        img_descriptors.push_back(descriptor);

        std::wcout << L"Progress = " << ((static_cast<double>(i+1) / static_cast<double>(files.size())) * 100) << L"%\n";
    }

    // Step 3: train model to recognize descriptors/features
    int num_bags = 250;
    cv::TermCriteria tc(cv::TermCriteria::MAX_ITER, 100, 0.001);
    int retries = 1;
    int flags = cv::KMEANS_PP_CENTERS;

    cv::BOWKMeansTrainer bof_trainer(num_bags, tc, retries, flags);

    cv::Mat kmeans_dict = bof_trainer.cluster(img_descriptors);

    // Step 4: save model
    cv::FileStorage fs("C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\SVMImageClassifier\\kmeans_dict.yml", cv::FileStorage::WRITE);
    fs << "vocabulary" << kmeans_dict;
    fs.release();
    
    std::wstring model_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\SVMImageClassifier\\kmeans_dict.yml";

    return model_path;
}

std::wstring trainSVM(std::string image_dir_path)
{
    // setup
    std::vector<std::string> files;
    DIR *img_dir;
    struct dirent *img_dir_rdr;

    // image processing and BoF
    cv::Mat kmeans_dict;
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create();
    cv::Ptr<cv::FeatureDetector> detector = cv::xfeatures2d::SIFT::create();
    cv::Ptr<cv::DescriptorExtractor> extractor = cv::xfeatures2d::SIFT::create();
    cv::BOWImgDescriptorExtractor bof_extractor(extractor, matcher);
    // NOTE: change this later to display instead of file
    cv::FileStorage fs_out("C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\SVMImageClassifier\\kmeans_descriptors.yml", cv::FileStorage::WRITE);


    // SVM


    std::wcout << L"Reading in picture(s)...\n";
    if (NULL == (img_dir = opendir(image_dir_path.c_str()))) {
        std::wcout << L"error opening image directory\n";
        return L"fail";
    }

    while (NULL != (img_dir_rdr = readdir(img_dir))) {
        std::string img_fname = std::string(img_dir_rdr->d_name);
        if (img_fname.size() > 4) {

            // DEBUG
            std::cout << "image filename: " << img_fname << "\n";

            files.push_back(img_fname);
        }
    }
    closedir(img_dir);

    cv::FileStorage fs("C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\SVMImageClassifier\\kmeans_dict.yml", cv::FileStorage::READ);
    fs["vocabulary"] >> kmeans_dict;
    fs.release();

    bof_extractor.setVocabulary(kmeans_dict);

    for (int i = 0; i < files.size(); i++) {
        std::string img_fpath = image_dir_path + "\\" + files[i];

        // DEBUG
        std::cout << "image filepath: " << img_fpath << "\n";

        cv::Mat img = cv::imread(img_fpath, cv::IMREAD_GRAYSCALE);

        if (img.empty()) {
            std::wcout << L"invalid input\n";
            return L"fail";
        }

        // DEBUG
        cv::imshow("original image", img);
        cv::waitKey(0);

        std::vector<cv::KeyPoint> keypts;
        cv::Mat descriptor;
            
        // extract SIFT features and classify w/ kmeans
        detector->detect(img, keypts);
        bof_extractor.compute(img, keypts, descriptor);

        // NOTE: change this later
        //fs_out << files[i].c_str() << descriptor << "\n";
        fs_out << "img" << descriptor;

        //std::cout << "Image Classification (" << files[i] << "): " << descriptor << "\n";
    }

    // NOTE: change this later
    fs_out.release();

    std::wstring model_path = L"C:\\Users\\ap\\Documents\\Projects\\Programs\\AI\\SVMImageClassifier\\svm_model.txt";
    
    return model_path;
}

std::vector<int> classifyImage(std::string test_data_path)
{
    std::vector<int> classes;



    return classes;
}