#include <opencv.hpp>
#include <highgui.hpp>
using namespace cv;

cv::Mat OilPaint(cv::Mat I, int brushSize, int coarseness);

int main()
{
	Mat src = imread("door.jpg", CV_LOAD_IMAGE_COLOR);
	Mat img = OilPaint(src, 5,100);
	imshow("result",img);
	return 0;
}

cv::Mat OilPaint(cv::Mat I, int brushSize, int coarseness)
{
	assert(!I.empty());
	if (brushSize < 1) brushSize = 1;
	if (brushSize > 8) brushSize = 8;

	if (coarseness < 1) coarseness = 1;
	if (coarseness > 255) coarseness = 255;

	int width = I.cols;
	int height = I.rows;

	int lenArray = coarseness + 1;
	int* CountIntensity = new int[lenArray];
	uint* RedAverage = new uint[lenArray];
	uint* GreenAverage = new uint[lenArray];
	uint* BlueAverage = new uint[lenArray];

	Mat gray;
	cvtColor(I, gray, COLOR_BGR2GRAY);

    
	Mat dst = Mat::zeros(I.size(), I.type());

	for (int nY = 0; nY <height; nY++)
	{
		int top = nY - brushSize;
		int bottom = nY + brushSize + 1;

		if (top<0) top = 0;
		if (bottom >= height) bottom = height - 1;

		for (int nX = 0; nX<width; nX++)
		{
			int left = nX - brushSize;
			int right = nX + brushSize + 1;

			if (left<0) left = 0;
			if (right >= width) right = width - 1;

			for (int i = 0; i <lenArray; i++){
				CountIntensity[i] = 0;
				RedAverage[i] = 0;
				GreenAverage[i] = 0;
				BlueAverage[i] = 0;
			}
            
			for (int j = top; j<bottom; j++){
				for (int i = left; i<right; i++){
					uchar intensity = static_cast<uchar>(coarseness*gray.at<uchar>(j, i) / 255.0);
					CountIntensity[intensity]++;

					RedAverage[intensity] += I.at<Vec3b>(j, i)[2];
					GreenAverage[intensity] += I.at<Vec3b>(j, i)[1];
					BlueAverage[intensity] += I.at<Vec3b>(j, i)[0];
				}
			}

			uchar chosenIntensity = 0;
			int maxInstance = CountIntensity[0];
			for (int i = 1; i<lenArray; i++){
				if (CountIntensity[i]>maxInstance){
					chosenIntensity = (uchar)i;
					maxInstance = CountIntensity[i];
				}
			}

			dst.at<Vec3b>(nY, nX)[2] = static_cast<uchar>(RedAverage[chosenIntensity] / static_cast<float>(maxInstance));
			dst.at<Vec3b>(nY, nX)[1] = static_cast<uchar>(GreenAverage[chosenIntensity] / static_cast<float>(maxInstance));
			dst.at<Vec3b>(nY, nX)[0] = static_cast<uchar>(BlueAverage[chosenIntensity] / static_cast<float>(maxInstance));
		}

	}

	delete[] CountIntensity;
	delete[] RedAverage;
	delete[] GreenAverage;
	delete[] BlueAverage;

#ifdef _DEBUG
	imshow("dst", dst);
	waitKey();
#endif

	return dst;
}
