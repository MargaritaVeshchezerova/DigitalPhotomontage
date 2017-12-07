#pragma once
#include "DatabaseUpload.h"
#include"InteractiveSelection.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "maxflow/graph.h"
#include <iostream>

using namespace cv;
using namespace std;

void loadDatabase(char* directory, vector<Mat>& images);