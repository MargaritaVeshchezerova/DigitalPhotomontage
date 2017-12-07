#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "maxflow/graph.h"
#include <iostream>

using namespace cv;

double computeX(int i1, int j1, int i2, int j2, Mat& first, Mat&second);
double computeY(int i1, int j1, int i2, int j2, Mat& first, Mat&second);