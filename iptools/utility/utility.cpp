#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

bool isPixelInROI(int i, int j, int x, int y, int sx, int sy) {
	cout << "in boolean function" << endl;
	return i >= y &&
		cout << "if1" << endl &&
		i < (y + sy) &&
		cout << "if2" << endl &&
		j >= x &&
		cout << "if3" << endl &&
		j < (x + sx) &&
		cout << "if4" << endl;
}

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
		cout << "r = " << r << endl;
		cout << "x = " << x << endl;
		cout << "y = " << y << endl;
		cout << "sx = " << sx << endl;
		cout << "sy = " << sy << endl;
		for (int i = 0; i < temp_img.getNumberOfRows(); i++) {
			for (int j = 0; j < temp_img.getNumberOfColumns(); j++) {
				if (
					i >= y &&
					i < (y + sy) &&
					j >= x &&
					j < (x + sx)
				) {
					int pixel = temp_img.getPixel(i, j);
					cout << "i = " << i << endl;
					cout << "j = " << j << endl;
					if (
						pixel >= T1 &&
						pixel <= T2
					) {
						tgt.setPixel(i, j, MAXRGB);
						cout << "set pixel" << endl;
					}
					else {
						tgt.setPixel(i, j, MINRGB);
						cout << "set pixel" << endl;
					}
				}
				else {
					tgt.setPixel(i, j, checkValue(temp_img.getPixel(i, j)));
				}		
			}
		}
		temp_img.copyImage(tgt);

		// for (int i = y; i < sy + y; i++) {
		// 	for (int j = x; j < sx + x; j++) {
		// 		cout << "r = " << r << endl;
		// 		int pixelI = src.getPixel(i, j);
		// 		if (((pixelI - T1) <= (T2 - T1))) {
		// 			tgt.setPixel(i, j, MAXRGB);
		// 		}
		// 		else {
		// 			tgt.setPixel(i, j, MINRGB);
		// 		}
		// 	}
		// }
		
	}
}
/*-----------------------------------------------------------------------**/
void utility::colorBinarization(image& src, image& tgt, const vector<roi>& regions) {
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	image temp_img;
	temp_img.copyImage(src);
	for (int r = 0; r < regions.size(); r++) {
		// cout << "r = " << r << endl;
		const int x = regions.at(r).x;
		// cout << "x = " << x << endl;
		const int y = regions.at(r).y;
		// cout << "y = " << y << endl;
		const int sx = regions.at(r).sx;
		// cout << "sx = " << sx << endl;
		const int sy = regions.at(r).sy;
		// cout << "sy = " << sy << endl;
		const int cR = regions.at(r).cR;
		// cout << "cR = " << cR << endl;
		const int cG = regions.at(r).cG;
		// cout << "cG = " << cG << endl;
		const int cB = regions.at(r).cB;
		// cout << "cB = " << cB << endl;
		const int TC = regions.at(r).TC;
		// cout << "TC = " << TC << endl;
		const int DC = regions.at(r).DC;
		// cout << "DC = " << DC << endl;
		color region_color = color(cR, cG, cB);

		for (int i = 0; i < src.getNumberOfColumns(); i++) {
			for (int j = 0; j < src.getNumberOfRows(); j++) {
				// cout << "beginning of inner loop" << endl;
				// cout << "i = " << i << endl;
				// cout << "y = " << y << endl;
				// cout << "sy = " << sy << endl;
				// cout << "j = " << j << endl;
				// cout << "x = " << x << endl;
				// cout << "sx = " << sx << endl;
				if (
					// cout << "inside if " << endl &&
					j >= y &&
					// cout << "if1" << endl &&
					j < (y + sy) &&
					// cout << "if2" << endl &&
					i >= x &&
					// cout << "if3" << endl &&
					i < (x + sx) //&&
					// cout << "if4" << endl
				) {
					// cout << "inside region check" << endl;
					int srcRed = temp_img.getPixel(j, i, RED);
					int srcGreen = temp_img.getPixel(j, i, GREEN);
					int srcBlue = temp_img.getPixel(j, i, BLUE);
					color src_color = color(srcRed, srcGreen, srcBlue);
					double distance = colorEuclideanDistance(region_color, src_color);
					// cout << "calculated ecu dist" << endl;
					if (distance > TC) {
						// cout << "distance is > TC\t";
						tgt.setPixel(j, i, RED, MINRGB);
						tgt.setPixel(j, i, GREEN, MINRGB);
						tgt.setPixel(j, i, BLUE, MINRGB);
						// cout << "set pixel" << endl;
					}
					else {
						// cout << "distance is <= TC\t";
						tgt.setPixel(j, i, RED, temp_img.getPixel(j, i, RED) + DC);
						tgt.setPixel(j, i, GREEN, temp_img.getPixel(j, i, GREEN) + DC);
						tgt.setPixel(j, i, BLUE, temp_img.getPixel(j, i, BLUE) + DC);
						// cout << "set pixel" << endl;				
					}
				}
				else {
					// cout << "setting pixel to straight up regular\t";
					// copy the pixel rgb values over to new image
					tgt.setPixel(j, i, RED, temp_img.getPixel(j, i, RED));
					tgt.setPixel(j, i, GREEN, temp_img.getPixel(j, i, GREEN));
					tgt.setPixel(j, i, BLUE, temp_img.getPixel(j, i, BLUE));
					// cout << "set pixel" << endl;
				}		
				// cout << "end of inner loop" << endl;
				// cout << "j = " << j << endl;
				// cout << "numRows = " << src.getNumberOfRows() << endl;
				// cout << "i = " << i << endl;
				// cout << "numCols = " << src.getNumberOfColumns() << endl;
			}
			// cout << "end of outer loop" << endl;
		}
		// cout << "bout to copy image" << endl;
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
		int new_pixel = 0;

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
							new_pixel += temp_img.getPixel(i + k - radius, j + l - radius);
						}
					}
					new_pixel = (new_pixel/(WS * WS));
					tgt.setPixel(i, j, new_pixel);
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
	tgt.resize(src.getNumberOfColumns(), src.getNumberOfRows());
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

