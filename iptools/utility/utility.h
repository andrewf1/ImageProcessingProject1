#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <iostream>
#include <math.h>
#include <vector>
#include "../../project/roi.hpp"
#include "../../project/color.hpp"
using namespace std;

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int value);
		static void binarize(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		static void increaseBrightness(image& src, image& tgt, const int& threshold, const int& intensity);
		static void doubleThreshold(image& src, image& tgt, const vector<roi>& regions);
		static void colorBinarization(image& src, image& tgt, const vector<roi>& regions);
		static void smoothing2d(image& src, image& tgt, const vector<roi>& regions);
		static void smoothing1d(image& src, image& tgt, const vector<roi>& regions);
		static void smoothing1dInc(image& src, image& tgt, const vector<roi>& regions);
};

#endif

