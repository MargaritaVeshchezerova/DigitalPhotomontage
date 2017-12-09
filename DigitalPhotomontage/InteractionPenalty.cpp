#include "stdafx.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "maxflow/graph.h"
#include <iostream>

using namespace cv;
using namespace std;

Mat float2byte(const Mat& If)
{
	double minVal, maxVal;
	minMaxLoc(If, &minVal, &maxVal);
	Mat Ib;
	If.convertTo(Ib, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
	return Ib;
}

double computeX(int i1, int j1, int i2, int j2, Mat& first, Mat&second)
{
	return norm(Scalar(first.at<Vec3b>(i1, j1)) - Scalar(second.at<Vec3b>(i1, j1))) + norm(Scalar(first.at<Vec3b>(i2, j2)) - Scalar(second.at<Vec3b>(i2, j2)));
}

double computeY(int i1, int j1, int i2, int j2, Mat& first, Mat&second)
{
	if ( i1 <= 0 || i1 >= first.rows - 1 || i2 <= 0 || i2 >= first.rows - 1 || j1 <= 0 || j1 >= first.cols - 1 || j2 <= 0 || j2 >= first.cols - 1) 
	{
		return 0.;
	}
	else {
		//cout << "hey" << endl;
		//cout << i1 << " " << i2 << " " << j1 << " " << j2 << " " << endl;
		Scalar G_i1_alpha1 = (Scalar(first.at<Vec3b>(i1 + 1, j1)) - Scalar(first.at<Vec3b>(i1 - 1, j1))) / 2;
		Scalar G_i2_alpha1 = (Scalar(first.at<Vec3b>(i2 + 1, j2)) - Scalar(first.at<Vec3b>(i2 - 1, j2))) / 2;
		Scalar G_i1_alpha2 = (Scalar(second.at<Vec3b>(i1 + 1, j1)) - Scalar(second.at<Vec3b>(i1 - 1, j1))) / 2;
		Scalar G_i2_alpha2 = (Scalar(second.at<Vec3b>(i2 + 1, j2)) - Scalar(second.at<Vec3b>(i2 - 1, j2))) / 2;
		Scalar G_j1_alpha1 = (Scalar(first.at<Vec3b>(i1, j1 + 1)) - Scalar(first.at<Vec3b>(i1, j1 - 1))) / 2;
		Scalar G_j2_alpha1 = (Scalar(first.at<Vec3b>(i2, j2 + 1)) - Scalar(first.at<Vec3b>(i2, j2 - 1))) / 2;
		Scalar G_j1_alpha2 = (Scalar(second.at<Vec3b>(i1, j1 + 1)) - Scalar(second.at<Vec3b>(i1, j1 - 1))) / 2;
		Scalar G_j2_alpha2 = (Scalar(second.at<Vec3b>(i2, j2 + 1)) - Scalar(second.at<Vec3b>(i2, j2 - 1))) / 2;
		//cout << "nice" << endl;
		return(sqrt(norm(G_i1_alpha1 - G_i1_alpha2, NORM_L2SQR) + norm(G_j1_alpha1 - G_j1_alpha2, NORM_L2SQR)) + sqrt(norm(G_i2_alpha1 - G_i2_alpha2, NORM_L2SQR) + norm(G_j2_alpha1 - G_j2_alpha2, NORM_L2SQR)));
	}
}


double computeX(int alpha1, int i1, int j1, int alpha2, int i2, int j2, vector<Mat>&source)
{
	if (alpha1 == alpha2) {
		return 0.;
	}
	else {
		return((norm(Scalar(source.at(alpha1).at<Vec3b>(i1, j1)) - Scalar(source.at(alpha2).at<Vec3b>(i1, j1))) + norm(Scalar(source.at(alpha1).at<Vec3b>(i2, j2)) - Scalar(source.at(alpha2).at<Vec3b>(i2, j2)))));
	}
}

double computeY(int alpha1, int i1, int j1, int alpha2, int i2, int j2, vector<Mat>&source)
{
	if (alpha1 == alpha2 || i1 <= 0 || i1 >= source.at(0).rows - 1 || i2 <= 0 || i2 >= source.at(0).rows - 1 || j1 <= 0 || j1 >= source.at(0).cols - 1 || j2 <= 0 || j2 >= source.at(0).cols - 1) {
		return 0.;
	}
	else {
		//cout << "hey" << endl;
		//cout << i1 << " " << i2 << " " << j1 << " " << j2 << " " << endl;
		Scalar G_i1_alpha1 = (Scalar(source.at(alpha1).at<Vec3b>(i1 + 1, j1)) - Scalar(source.at(alpha1).at<Vec3b>(i1 - 1, j1))) / 2;
		Scalar G_i2_alpha1 = (Scalar(source.at(alpha1).at<Vec3b>(i2 + 1, j1)) - Scalar(source.at(alpha1).at<Vec3b>(i2 - 1, j1))) / 2;
		Scalar G_i1_alpha2 = (Scalar(source.at(alpha2).at<Vec3b>(i1 + 1, j1)) - Scalar(source.at(alpha2).at<Vec3b>(i1 - 1, j1))) / 2;
		Scalar G_i2_alpha2 = (Scalar(source.at(alpha2).at<Vec3b>(i2 + 1, j1)) - Scalar(source.at(alpha2).at<Vec3b>(i2 - 1, j1))) / 2;
		Scalar G_j1_alpha1 = (Scalar(source.at(alpha1).at<Vec3b>(i1, j1 + 1)) - Scalar(source.at(alpha1).at<Vec3b>(i1, j1 - 1))) / 2;
		Scalar G_j2_alpha1 = (Scalar(source.at(alpha1).at<Vec3b>(i2, j1 + 1)) - Scalar(source.at(alpha1).at<Vec3b>(i2, j1 - 1))) / 2;
		Scalar G_j1_alpha2 = (Scalar(source.at(alpha2).at<Vec3b>(i1, j1 + 1)) - Scalar(source.at(alpha2).at<Vec3b>(i1, j1 - 1))) / 2;
		Scalar G_j2_alpha2 = (Scalar(source.at(alpha2).at<Vec3b>(i2, j1 + 1)) - Scalar(source.at(alpha2).at<Vec3b>(i2, j1 - 1))) / 2;
		//cout << "nice" << endl;
		return((norm(G_i1_alpha1 - G_i1_alpha2) + norm(G_j1_alpha1 - G_j1_alpha2) + norm(G_i2_alpha1 - G_i2_alpha2) + norm(G_j2_alpha1 - G_j2_alpha2)));
	}
}
