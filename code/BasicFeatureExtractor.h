#pragma once

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

double getYellowness(image_data data, int width, int height);