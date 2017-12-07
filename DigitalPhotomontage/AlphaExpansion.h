#pragma once

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "maxflow/graph.h"
#include <iostream>

using namespace cv;
using namespace std;

void alpha_expansion(vector<Mat>& images, Mat& temp_image, Mat& input, Mat& labels);
Mat float2byte(const Mat& If);