#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "svm.h"
#include "dirent.h"

struct pixel
{
	int R;
	int G;
	int B;
};

class image_data
{
public:
	image_data(int width, int height);
	~image_data();
	pixel** get_data() const;
	int get_width() const;
	int get_height() const;
private:
	pixel** data = nullptr;
	int width;
	int height;
};

std::wstring getTargetDescriptors(std::string train_data_path, std::string train_labels_path);
std::wstring trainSVM(std::string image_dir_path);
std::vector<int> classifyImage(std::string test_data_path);