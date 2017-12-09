#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat* labeling_optimized(vector<Mat>& dataset, int size);
vector<Mat*>* labeling(vector<Mat>& dataset, int size);