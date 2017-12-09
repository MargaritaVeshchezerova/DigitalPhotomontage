#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void alpha_expansion_optimized(vector<Mat>& images, Mat& temp_image, void* input, Mat& labels, char* result_name, int option);
Mat float2byte(const Mat& If);