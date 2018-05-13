#include "BasicFeatureExtractor.h"

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

/*
Optionally pre-processes image data before extracting features from it

Can do the following:
--- Pre-processing ---


--- Feature extraction ---
- get yellow "shading"
*/

//Pre-processing


//Feature extraction
double getYellowness(image_data data, int width, int height)
{
	double yellowness = 0;
	pixel** pixel_values = data.get_data();
	int R_tot = 0;
	int G_tot = 0;
	int B_tot = 0;

	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			pixel pixel_data = pixel_values[x][y];
			if ((pixel_data.R < 205) || (pixel_data.G < 205) || (pixel_data.B > 25)) {
				pixel_data.R = 0;
				pixel_data.G = 0;
				pixel_data.B = 0;
			} else {
				R_tot += pixel_data.R;
				G_tot += pixel_data.G;
				B_tot += pixel_data.B;
			}
		}
	}

	int RGB_tot = R_tot + G_tot + B_tot;
	yellowness = RGB_tot / (width * height);

	return yellowness;
}