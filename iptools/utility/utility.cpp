#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

double colorEuclideanDistance(color c1, color c2) {
	// cout << "calculating euci dist\t";
	int red_part = c2.R - c1.R;
	red_part *= red_part;
	int green_part = c2.G - c1.G;
	green_part *= green_part;
	int blue_part = c2.B - c1.B;
	blue_part *= blue_part;
	return sqrt(red_part + green_part + blue_part);
}

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				/* Directly copy the value */
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				/* Average the values of four pixels */
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::increaseBrightness(image& src, image& tgt, const int& threshold, const int& intensity)  {
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

	for (int i = 0; i < src.getNumberOfRows(); i++) {
		for (int j = 0; j < src.getNumberOfColumns(); j++){
			if (src.getPixel(i, j) > threshold) { // if the pixel's intensity is greater than the treshold
				// increase intensity with value
				tgt.setPixel(i, j, checkValue(src.getPixel(i, j) + intensity));
			}
			else {
				// copies the pixel from src to tgt
				tgt.setPixel(i, j, checkValue(src.getPixel(i, j)));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::doubleThreshold(image& src, image& tgt, const vector<roi>& regions) {
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	image temp_img;
	temp_img.copyImage(src);
	for (int r = 0; r < regions.size(); r++) {
		int x = regions.at(r).x;
		int y = regions.at(r).y;
		int sx = regions.at(r).sx;
		int sy = regions.at(r).sy;
		int T1 = regions.at(r).T1;
		int T2 = regions.at(r).T2;
		for (int i = 0; i < temp_img.getNumberOfRows(); i++) {
			for (int j = 0; j < temp_img.getNumberOfColumns(); j++) {
				if (
					i >= y &&
					i < (y + sy) &&
					j >= x &&
					j < (x + sx)
				) {
					int pixel = temp_img.getPixel(i, j);
					if (
						pixel >= T1 &&
						pixel <= T2
					) {
						tgt.setPixel(i, j, MAXRGB);
					}
					else {
						tgt.setPixel(i, j, MINRGB);
					}
				}
				else {
					tgt.setPixel(i, j, checkValue(temp_img.getPixel(i, j)));
				}		
			}
		}
		temp_img.copyImage(tgt);
	}
}
/*-----------------------------------------------------------------------**/
void utility::colorBinarization(image& src, image& tgt, const vector<roi>& regions) {
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	image temp_img;
	temp_img.copyImage(src);
	for (int r = 0; r < regions.size(); r++) {
		const int x = regions.at(r).x;
		const int y = regions.at(r).y;
		const int sx = regions.at(r).sx;
		const int sy = regions.at(r).sy;
		const int cR = regions.at(r).cR;
		const int cG = regions.at(r).cG;
		const int cB = regions.at(r).cB;
		const int TC = regions.at(r).TC;
		const int DC = regions.at(r).DC;
		color region_color = color(cR, cG, cB);

		for (int i = 0; i < src.getNumberOfColumns(); i++) {
			for (int j = 0; j < src.getNumberOfRows(); j++) {
				if (
					j >= y &&
					j < (y + sy) &&
					i >= x &&
					i < (x + sx)
				) {
					int srcRed = temp_img.getPixel(j, i, RED);
					int srcGreen = temp_img.getPixel(j, i, GREEN);
					int srcBlue = temp_img.getPixel(j, i, BLUE);
					color src_color = color(srcRed, srcGreen, srcBlue);
					double distance = colorEuclideanDistance(region_color, src_color);
					if (distance > TC) {
						tgt.setPixel(j, i, RED, MINRGB);
						tgt.setPixel(j, i, GREEN, MINRGB);
						tgt.setPixel(j, i, BLUE, MINRGB);
					}
					else {
						tgt.setPixel(j, i, RED, temp_img.getPixel(j, i, RED) + DC);
						tgt.setPixel(j, i, GREEN, temp_img.getPixel(j, i, GREEN) + DC);
						tgt.setPixel(j, i, BLUE, temp_img.getPixel(j, i, BLUE) + DC);
					}
				}
				else {
					// copy the pixel rgb values over to new image
					tgt.setPixel(j, i, RED, temp_img.getPixel(j, i, RED));
					tgt.setPixel(j, i, GREEN, temp_img.getPixel(j, i, GREEN));
					tgt.setPixel(j, i, BLUE, temp_img.getPixel(j, i, BLUE));
				}		
			}
		}
		temp_img.copyImage(tgt);
	}
}

/*-----------------------------------------------------------------------**/
void utility::smoothing2d(image& src, image& tgt, const vector<roi>& regions) {
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	image temp_img;
	temp_img.copyImage(src);
	
	for (int r = 0; r < regions.size(); r++) {
		int x = regions.at(r).x;
		int y = regions.at(r).y;
		int sx = regions.at(r).sx;
		int sy = regions.at(r).sy;
		int WS = regions.at(r).WS;
		int radius = (WS - 1)/2;
		int avg_pixel = 0;

		for (int i = 0; i < temp_img.getNumberOfRows(); i++) {
			for (int j = 0; j < temp_img.getNumberOfColumns(); j++) {
				if (
					i >= y &&
					i < (y + sy) &&
					j >= x &&
					j < (x + sx)
				) {
					for (int k = 0; k <= radius * radius; k++) {
						for (int l = 0; l <= radius * radius; l++) {
							avg_pixel += temp_img.getPixel(i + k - radius, j + l - radius);
						}
					}
					avg_pixel = (avg_pixel/(WS * WS));
					tgt.setPixel(i, j, avg_pixel);
				}
				else {
					tgt.setPixel(i, j, checkValue(temp_img.getPixel(i, j)));
				}
			}
		}
		temp_img.copyImage(tgt);
	}
}

/*-----------------------------------------------------------------------**/
void utility::smoothing1d(image& src, image& tgt, const vector<roi>& regions) {
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	image temp_img;
	temp_img.copyImage(src);

	for (int r = 0; r < regions.size(); r++) {
		int x = regions.at(r).x;
		int y = regions.at(r).y;
		int sx = regions.at(r).sx;
		int sy = regions.at(r).sy;
		int WS = regions.at(r).WS;
		int radius = (WS - 1)/2;
		image tgt_buff;
		tgt_buff.resize(temp_img.getNumberOfRows(), temp_img.getNumberOfColumns());

		for (int i = 0; i< temp_img.getNumberOfColumns(); i++) {
			for (int j = 0; j < temp_img.getNumberOfRows(); j++) {
				if (
					i >= y &&
					i < (y + sy) &&
					j >= x &&
					j < (x + sx)
				) {
					int newVal = 0;
					for (int n = 0; n <= radius * radius; n++) {
						newVal += temp_img.getPixel(i + n - radius, j);
					}
					newVal /= WS;
					tgt_buff.setPixel(i, j, newVal);
				}
				else {
					tgt_buff.setPixel(i, j, checkValue(temp_img.getPixel(i, j)));
				}
			}
		}

		for (int i = 0; i< temp_img.getNumberOfColumns(); i++) {
			for (int j = 0; j < temp_img.getNumberOfRows(); j++) {
				if (
					i >= y &&
					i < (y + sy) &&
					j >= x &&
					j < (x + sx)
				) {
					int newVal = 0;
					for (int n = 0; n <= radius * radius; n++) {
						newVal += tgt_buff.getPixel(i, j + n - radius);
					}

					newVal /= WS; // WS + 1
					tgt.setPixel(i, j, newVal);
				}
				else {
					tgt.setPixel(i, j, checkValue(temp_img.getPixel(i, j)));
				}				
			}
		}
		temp_img.copyImage(tgt);
	}
	
}

/*-----------------------------------------------------------------------**/
void utility::smoothing1dInc(image& src, image& tgt, const vector<roi>& regions) {
	
}

