#include "stdafx.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

const int DefaultLabel = -1;
const int totalColors = 765;
int n = 0;

class ExchangedData
{
public:
	Mat image;
	Mat* labels;
	int label;
	int* radius;
	ExchangedData(Mat& img, Mat& lbs, int lb)
	{
		image = img.clone();
		labels = &lbs;
		label = lb;
		radius = new int(20);
	}
	~ExchangedData()
	{
		image.release();
	}
};

Scalar color(int label)
{
	int c = label*totalColors / n;
	switch (c / 255)
	{
	case 0:
		return Scalar(c % 255, 0, 0);
	case 1:
		return Scalar(0, c % 255, 0);
	case 2:
		return Scalar(0, 0, c % 255);
	default:
		break;
	}
	return Scalar(255, 0, 0);
};

//How to get mouse position
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	Mat image = ((ExchangedData*)userdata)->image;
	Mat labels = *(((ExchangedData*)userdata)->labels);
	int label = ((ExchangedData*)userdata)->label;
	int radius = *((ExchangedData*)userdata)->radius;

	if (flags == (EVENT_FLAG_LBUTTON))
	{
		circle(image, Point(x, y), radius, color(label), -1, 8);
		circle(labels, Point(x, y), radius,label, -1, 8);
		cout << "Left mouse button is clicked - position (" << x << ", " << y << ") radius: " << radius << endl;
	}
};


Mat* labeling_optimized(vector<Mat>& dataset, int size)
{
	n = size;
	if (n == 0)
		return NULL;

	int rows = dataset[0].rows;
	int cols = dataset[0].cols;

	Mat* labels = new Mat(-1*Mat::ones(rows, cols, CV_32SC1));

	vector<ExchangedData*>* collectedData = new vector<ExchangedData*>();
	for (int i = 0; i < n; i++)
	{
		collectedData->push_back(new ExchangedData(dataset[i], *labels, i));
	}

	for (int i = 0; i < n; i++)
	{
		namedWindow("Window: " + i, WINDOW_NORMAL | WINDOW_KEEPRATIO);
		createTrackbar("Brush radius: " + i, "Window: " + i, (*collectedData)[i]->radius, 100, NULL, NULL);
		setMouseCallback("Window: " + i, CallBackFunc, (*collectedData)[i]);
	}
	while (1)
	{
		for (int i = 0; i < n; i++)
		{
			setTrackbarPos("Brush radius: " + i, "Window: " + i, *(*collectedData)[i]->radius);
			imshow("Window: " + i, (*collectedData)[i]->image);
		}
		if (waitKey(40) == 27)
		{
			destroyAllWindows();
			break;
		}
	}

	for (int i = 0; i < n; i++)
	{
		delete (*collectedData)[i];
	}

	delete collectedData;

	return labels;
}


vector<Mat*>* labeling(vector<Mat>& dataset, int size)
{
	n = size;
	if (n == 0)
		return NULL;

	int rows = dataset[0].rows;
	int cols = dataset[0].cols;


	vector<ExchangedData*>* collectedData = new vector<ExchangedData*>();
	vector<Mat*>* collectedLabel = new vector<Mat*>();
	for (int i = 0; i < n; i++)
	{
		Mat* labels = new Mat(-1 * Mat::ones(rows, cols, CV_32SC1));
		collectedData->push_back(new ExchangedData(dataset[i], *labels, i));
		collectedLabel->push_back(labels);
	}

	for (int i = 0; i < n; i++)
	{
		namedWindow("Window: " + i, WINDOW_NORMAL | WINDOW_KEEPRATIO);
		createTrackbar("Brush radius: " + i, "Window: " + i, (*collectedData)[i]->radius, 100, NULL, NULL);
		setMouseCallback("Window: " + i, CallBackFunc, (*collectedData)[i]);
	}
	while (1)
	{
		for (int i = 0; i < n; i++)
		{
			setTrackbarPos("Brush radius: " + i, "Window: " + i, *(*collectedData)[i]->radius);
			imshow("Window: " + i, (*collectedData)[i]->image);
		}
		if (waitKey(40) == 27)
		{
			destroyAllWindows();
			break;
		}
	}

	for (int i = 0; i < n; i++)
	{
		delete (*collectedData)[i];
	}


	delete collectedData;

	return collectedLabel;
}
