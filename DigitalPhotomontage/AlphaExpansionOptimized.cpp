#include "stdafx.h"

#include "AlphaExpansionOptimized.h"
#include "time.h"
#include "maxflow/graph.h"
#include "InteractionPenalty.h"
#include <iostream>

const double epsilon = 0.05;
const double u =5*norm(Scalar(255,255,255) - Scalar(0,0,0));

const double X_price = 30;
const double Y_price = 30;

bool to_optimize = true;

int option = 1;

double binary_fusion_optimized(int alpha, Mat& first_image, Mat& second_image, void* input, Mat& labels)
{
	time_t t0, t1;
	time(&t0);
	int rows = first_image.rows;
	int cols = first_image.cols;

	Graph<double, double, double> g(rows*cols, rows*(cols - 1) + cols*(rows - 1));
	g.add_node(rows*cols);

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			double curr_price = 0;
			double alpha_price = 0;
			if (option == 1)
			{
				Mat* inp = (Mat*)input;
				if (inp->at<int>(i, j) != -1)
				{
					if (inp->at<int>(i, j) != alpha)
						alpha_price = u;
					if (inp->at<int>(i, j) != labels.at<int>(i, j))
						curr_price = u;
				}
			}

			if (option == 2)
			{
				vector<Mat*>* inp = (vector<Mat*>*)input;
				
				if ((*inp)[alpha]->at<int>(i, j) != alpha)
					alpha_price = -u;
				if ((*inp)[labels.at<int>(i, j)]->at<int>(i, j) != labels.at<int>(i, j))
					curr_price = -u;
			}
			g.add_tweights(i*cols + j, curr_price, alpha_price);
		}


	//Suspens!!!
	
	if (to_optimize)
	{
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
	}
	else
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
			{
				if (i > 0)
				{
					double cost = X_price * computeX(i - 1, j, i, j, first_image, second_image) + Y_price * computeY(i - 1, j, i, j, first_image, second_image);
					g.add_edge((i - 1)*cols + j, i*cols + j, cost, cost);
				}
				if (j > 0)
				{
					double cost = X_price * computeX(i, j - 1, i, j, first_image, second_image) + Y_price * computeY(i, j - 1, i, j, first_image, second_image);
					g.add_edge(i*cols + j - 1, i*cols + j, cost, cost);
				}
			}
	}
		
	time_t flow_start, flow_end;
	time(&flow_start);
	double energy = g.maxflow();
	time(&flow_end);
	cout << "Energy of the cut: " << energy << ". Calculation time: " << difftime(flow_end, flow_start)<< " seconds."<<endl;
	if (energy == 0)
		return 0;
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
	time(&t1);
	if (to_optimize)
		cout << "The time of optimized ";
	else
		cout << "The time of ";
	cout <<"binary fusion is " << difftime(t1, t0) << " seconds.\n";
	waitKey(1000);
	return energy;
};

void alpha_expansion_optimized(vector<Mat>& images, Mat& temp_image, void* input, Mat& labels, char* result_name, int opt)
{
	option = opt;
	int n_images = images.size();

	double energy0 = binary_fusion_optimized(0, temp_image, temp_image, input, labels);
	double energyN;

	char c;
	cout << "Do you want to use optimized version of computing of interaction penality? (Y/n): ";
	cin >> c;
	if (c == 'n')
		to_optimize = false;

	do
	{
		cout << "Optimized calculation started" << endl;
		for (int alpha = 0; alpha < n_images; alpha++)
		{
			double energy = binary_fusion_optimized(alpha, temp_image, images[alpha], input, labels);
			if (alpha == 0)
				energy0 = energy;
			if (alpha == n_images - 1)
				energyN = energy;
		}
		cout << "Energy difference: " << energy0 - energyN << endl;
	} 
	while (energy0 - energyN != 0);
	waitKey(0);

	imwrite(string(result_name)+".jpg", temp_image);
	imwrite(string(result_name) + "_labels.jpg", float2byte(labels));

	if (option == 1)
		imwrite(string(result_name) + "_masque.jpg", float2byte( *((Mat*)input) ));

	waitKey(0);
};


