/*
* 1) Edge Preserve Smoothing
*    -> Using Normalized convolution Filter
*    -> Using Recursive Filter
* 2) Detail Enhancement
* 3) Pencil sketch/Color Pencil Drawing
* 4) Stylization
* 5) Stylization & edgePreserve -> blending
* 6) Pencil sketch & edgePreserve -> blending
* 7) Pencil sketch & bilateral -> blending
*/

#include <signal.h>
#include "opencv2/photo.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

int main()
{

	Mat I = imread("src.png");

	int num, type;

	if (I.empty())
	{
		cout << "Image not found" << endl;
		exit(0);
	}

	cout << endl;
	cout << " Edge Preserve Filter" << endl;
	cout << "----------------------" << endl;

	cout << "Options: " << endl;
	cout << endl;

	cout << "1) Edge Preserve Smoothing" << endl;
	cout << "   -> Using Normalized convolution Filter" << endl;
	cout << "   -> Using Recursive Filter" << endl;
	cout << "2) Detail Enhancement" << endl;
	cout << "3) Pencil sketch/Color Pencil Drawing" << endl;
	cout << "4) Stylization" << endl;
	cout << endl;

	cout << "Press number 1-4 to choose from above techniques: ";

	cin >> num;

	Mat img;

	if (num == 1)
	{
		cout << endl;
		cout << "Press 1 for Normalized Convolution Filter and 2 for Recursive Filter: ";

		cin >> type;

		edgePreservingFilter(I, img, type);
		imshow("Edge Preserve Smoothing", img);

	}
	else if (num == 2)
	{
		detailEnhance(I, img);
		imshow("Detail Enhanced", img);
	}
	else if (num == 3)
	{
		Mat img1;
		pencilSketch(I, img1, img, 10, 0.1f, 0.03f);
		imshow("Pencil Sketch", img1);
		imshow("Color Pencil Sketch", img);
	}
	else if (num == 4)
	{
		stylization(I, img);
		imshow("Stylization", img);
	}
	else if (num == 5)
	{
		Mat img1, img2;
		stylization(I, img1);
		edgePreservingFilter(I, img2, 1);
		addWeighted(img1, 0.3, img2, 0.7, 0, img);

		imshow("result",img);
		imwrite("TEST1.png", img);
	}
	else if (num == 6)
	{
		Mat img1, img2, img3;
		edgePreservingFilter(I,img1, 1);
		pencilSketch(I, img2, img3, 5,0.08,0.02);
		addWeighted(img1, 0.6, img3, 0.4, 0, img);
		imshow("result", img);
	}
	else if (num == 7)
	{
		Mat img1,img2,img3;
		bilateralFilter(I, img2, 5,160,150);
		imshow("blur",img2);
		pencilSketch(I, img1, img3, 60, 0.25, 0.05);
		imshow("pencil",img3);
		addWeighted(I, 0.7, img3, 0.3, 0, img);
		imshow("result", img);
	}

	waitKey(0);
}
