#include "stdafx.h"


#include "DatabaseUpload.h"
#include"InteractiveSelection.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "AlphaExpansion.h"
#include <iostream>

using namespace cv;
using namespace std;

void test1(char* directory)
{
	vector<Mat>* images = new vector<Mat>();
	loadDatabase(directory, *images);
	Mat temp_image = (*images)[0].clone();
	

	int n_images = images->size();
	int rows = (*images)[0].rows;
	int cols = (*images)[0].cols;

	Mat* labels = new Mat(Mat::zeros(rows, cols, CV_32SC1));

	Mat* Input_labels = labeling(*images, n_images);

	namedWindow("W", 1);
	imshow("W", float2byte(*Input_labels));
	waitKey(0);
	alpha_expansion(*images, temp_image, *Input_labels, *labels);

	waitKey(0);

	delete images;
	delete Input_labels;
	return;
}


