#include "stdafx.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "maxflow/graph.h"
#include "InteractionPenalty.h"
#include <iostream>

using namespace cv;
using namespace std;

const double epsilon = 0.05;
const double u = 5*norm(Scalar(255,255,255) - Scalar(0,0,0));

double X_price = 1;
double Y_price = 10;

double binary_fusion(int alpha, Mat& first_image, Mat& second_image, Mat& input, Mat& labels)
{
	int rows = first_image.rows;
	int cols = first_image.cols;

	Graph<double, double, double> g(rows*cols, rows*(cols - 1) + cols*(rows - 1));
	g.add_node(rows*cols);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			double curr_price = 0;
			double alpha_price = 0;
			if (input.at<int>(i, j) != -1)
			{
				if (input.at<int>(i, j) != alpha)
					alpha_price = u;
				if (input.at<int>(i, j) != labels.at<int>(i, j))
					curr_price = u;
			}
			g.add_tweights(i*cols + j, curr_price, alpha_price);
		}


	//Suspens!!!
	/*
	Mat ColorPenalty = (first_image - second_image) + (second_image - first_image);
	Mat TotalPenality = ColorPenalty;

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			if (i > 0)
			{
				double cost = norm(Scalar(TotalPenality.at<Vec3b>(i, j))) + norm(Scalar(TotalPenality.at<Vec3b>(i - 1, j)));
				g.add_edge((i - 1)*cols + j, i*cols + j, cost, cost);
			}
			if (j > 0)
			{
				double cost = norm(Scalar(TotalPenality.at<Vec3b>(i, j))) + norm(Scalar(TotalPenality.at<Vec3b>(i, j - 1)));
				g.add_edge(i*cols + j - 1, i*cols + j, cost, cost);
			}
		}
		*/


	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			if (i > 0)
			{
				double cost = X_price * computeX(i-1,j,i,j,first_image, second_image)+ Y_price * computeY(i - 1, j, i, j, first_image, second_image);
				g.add_edge((i - 1)*cols + j, i*cols + j, cost, cost);
			}
			if (j > 0)
			{
				double cost = X_price * computeX(i, j-1, i, j, first_image, second_image) + Y_price * computeY(i, j-1, i, j, first_image, second_image);
				g.add_edge(i*cols + j - 1, i*cols + j, cost, cost);
			}
		}



	double energy = g.maxflow();
	cout << "Energy of the cut: " << energy << endl;

	for (int node_id = 0; node_id < rows*cols; node_id++)
	{
		if (g.what_segment(node_id) == Graph<double, double, double>::SOURCE)
		{
			int i = node_id / cols;
			int j = node_id % cols;
			labels.at<int>(i, j) = alpha;
			first_image.at<Vec3b>(i,j) = second_image.at<Vec3b>(i, j);
		}
	}
	return energy;
};

Mat float2byte(const Mat& If)
{
	double minVal, maxVal;
	minMaxLoc(If, &minVal, &maxVal);
	Mat Ib;
	If.convertTo(Ib, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
	return Ib;
}

void alpha_expansion(vector<Mat>& images, Mat& temp_image, Mat& input, Mat& labels)
{
	namedWindow("past result", WINDOW_NORMAL | WINDOW_KEEPRATIO);
	namedWindow("temporal result", WINDOW_NORMAL | WINDOW_KEEPRATIO);
	int n_images = images.size();
	imshow("past result", temp_image);
	double past_energy = -1;
	double new_energy = binary_fusion(0, temp_image, temp_image, input, labels);
	while(past_energy - new_energy > epsilon  || past_energy == -1)
	{
		for (int alpha = 0; alpha < n_images; alpha++)
		{
			double energy = binary_fusion(alpha, temp_image, images[alpha], input, labels);
			if (alpha == n_images - 1)
			{
				past_energy = new_energy;
				new_energy = energy;
				cout << "Energy difference: " << past_energy -new_energy<< endl;
				cout << "display result" << endl;
				imshow("temporal result", temp_image);
			}
		}
		imshow("temporal result label", float2byte(labels));
		imshow("temporal result", temp_image);
		waitKey(0);
	}
};


