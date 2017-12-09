#include "stdafx.h"


#include "DatabaseUpload.h"
#include"InteractiveSelection.h"
#include "AlphaExpansionOptimized.h"
#include <iostream>

using namespace cv;
using namespace std;

void test1(char* directory, char* result_name)
{
	cout << "Please choose one of the folowing options:\n\t1. Selection of desired parts.\n\t2. Selection of undesired parts.\nYour choise: ";
	int option;
	cin >> option;
	cout << endl;
	vector<Mat>* images = new vector<Mat>();
	loadDatabase(directory, *images);	

	int n_images = images->size();
	int rows = (*images)[0].rows;
	int cols = (*images)[0].cols;

	Mat* labels = new Mat(Mat::zeros(rows, cols, CV_32SC1));
	Mat* temp_image = new Mat((*images)[0]);

	if (option == 1)
	{
		Mat* Input_labels = labeling_optimized(*images, n_images);

		namedWindow("Input labels", 1);
		imshow("Input labels", float2byte(*Input_labels));

		alpha_expansion_optimized(*images, *temp_image, Input_labels, *labels, result_name, option);
		delete Input_labels;
	}
	else if (option == 2)
	{
		vector<Mat*>* Input_labels = labeling(*images, n_images);
		alpha_expansion_optimized(*images, *temp_image, Input_labels, *labels, result_name, option);
		delete Input_labels;
	}
	else
	{
		cout << "None of proposed options was selected. Finish.\n";
	}
	delete images;
	delete temp_image;
	return;
}


