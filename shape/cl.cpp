//client
/*
//============================================================================
// Name        : Client4robo.cpp
// Author      : Peshkichev Roman
// Version     : 0.1 alfa
// Copyright   : ???
//============================================================================
// Name        : Client4robo.cpp
// Author      : Peshkichev Roman
// Version     : 0.1 alfa
// Copyright   : ???
// Description : Hello World!!!
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#else
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib") //���������� ���������� ������� � windows
#endif
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void error(const char* message); //error function


int main(int argc, char *argv[])
{

	/*
	Mat origimg;
	Mat grayimg;
	origimg = imread("D:\\project\\open_client\\Debug\\1.jpg", 1);
	cvtColor(origimg, grayimg, CV_RGB2GRAY);

	uint8_t *row = NULL; 
	for (int i = 0; i < grayimg.rows; i++)
	{
		row = (uint8_t*)grayimg.ptr<uint8_t>(i);
		for (int j = 0; j < grayimg.cols; j++)
		{
			if (row[j]>80) row[j] = 255;
			else row[j] = 0;
		}
	}
	row = (uint8_t*)grayimg.ptr<uint8_t>(grayimg.rows-3);
	memset(row, 255, grayimg.cols*3);
	row = (uint8_t*)grayimg.ptr<uint8_t>(grayimg.rows - 18);
	memset(row, 255, grayimg.cols);

	
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(grayimg, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	RNG rng(12345);
	Mat drawing = Mat::zeros(grayimg.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}
	

	cvNamedWindow("Frame", WINDOW_KEEPRATIO);
	imshow("Frame", drawing);
	cvWaitKey(0);

	origimg.release();
	grayimg.release();
	cvDestroyWindow("Frame");

    return 0;
	
}

void error(const char* message)
{
	printf("%s\n", message);
	exit(1);
}

*/

//contours canny
/*
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

static void help()
{
	cout
		<< "\nThis program illustrates the use of findContours and drawContours\n"
		<< "The original image is put up along with the image of drawn contours\n"
		<< "Usage:\n"
		<< "./contours2\n"
		<< "\nA trackbar is put up which controls the contour level from -3 to 3\n"
		<< endl;
}

int h,w;
int levels = 3;

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

static void on_trackbar(int, void*)
{
	Mat cnt_img = Mat::zeros(h, w, CV_8UC3);
	int _levels = levels - 3;
	drawContours(cnt_img, contours, _levels <= 0 ? 3 : -1, Scalar(128, 255, 255),
		3, CV_AA, hierarchy, std::abs(_levels));

	imshow("contours", cnt_img);
}

int main(int argc, char**)
{
	Mat img = imread("D:\\project\\open_client\\Debug\\1.jpg", 0);
	w = img.cols;
	h = img.rows;
	//show the faces
	namedWindow("image", 1);
	imshow("image", img);

	Canny(img, img, 0, 50, 5);
	// dilate canny output to remove potential
	// holes between edge segments
	dilate(img, img, Mat(), Point(-1, -1));

	//Extract the contours so that
	vector<vector<Point> > contours0;
	findContours(img, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	contours.resize(contours0.size());
	for (size_t k = 0; k < contours0.size(); k++)
		approxPolyDP(Mat(contours0[k]), contours[k], 3, true);

	namedWindow("contours", 1);
	createTrackbar("levels+3", "contours", &levels, 7, on_trackbar);

	on_trackbar(0, 0);
	waitKey();

	return 0;
}
*/

//laplace
/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <ctype.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

int sigma = 3;
int smoothType = CV_GAUSSIAN;

int main(int argc, char** argv)
{

	namedWindow("Laplacian", 0);
	createTrackbar("Sigma", "Laplacian", &sigma, 15, 0);

	Mat smoothed, laplace, result;

		Mat frame = imread("D:\\project\\open_client\\Debug\\1.jpg",1);

		int ksize = (sigma * 5) | 1;
		if (smoothType == CV_GAUSSIAN)
			GaussianBlur(frame, smoothed, Size(ksize, ksize), sigma, sigma);
		else if (smoothType == CV_BLUR)
			blur(frame, smoothed, Size(ksize, ksize));
		else
			medianBlur(frame, smoothed, ksize);

		Laplacian(smoothed, laplace, CV_16S, 5);
		convertScaleAbs(laplace, result, (sigma + 1)*0.25);
		imshow("Laplacian", result);

		 waitKey(0);

	return 0;
}
*/



//canny 2
/*
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 200;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";


//@function CannyThreshold
// @brief Trackbar callback - Canny thresholds input with a ratio 1:3

void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur(src_gray, detected_edges, Size(3, 3));

	/// Canny detector
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);

	src.copyTo(dst, detected_edges);
	imshow(window_name, detected_edges);
}


int main(int argc, char** argv)
{
	/// Load an image
	src = imread("D:\\project\\open_client\\Debug\\list.jpg");

	if (!src.data)
	{
		return -1;
	}

	/// Create a matrix of the same type and size as src (for dst)
	dst.create(src.size(), src.type());

	/// Convert the image to grayscale
	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Create a window
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);

	/// Create a Trackbar for user to enter threshold
	createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

	/// Show the image
	CannyThreshold(0, 0);

	/// Wait until user exit program by pressing a key
	waitKey(0);

	return 0;
}
*/


//edge
/*
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

using namespace cv;
using namespace std;

int edgeThresh = 1;
Mat image, gray, edge, cedge;

// define a trackbar callback
static void onTrackbar(int, void*)
{
	blur(gray, edge, Size(3, 3));

	// Run the edge detector on grayscale
	Canny(edge, edge, edgeThresh, edgeThresh * 3, 3);
	cedge = Scalar::all(0);

	image.copyTo(cedge, edge);
	imshow("Edge map", cedge);
}

static void help()
{
	printf("\nThis sample demonstrates Canny edge detection\n"
		"Call:\n"
		"    /.edge [image_name -- Default is fruits.jpg]\n\n");
}

const char* keys =
{
	"{1| |fruits.jpg|input image name}"
};

int main(int argc, const char** argv)
{

	string filename = "D:\\project\\open_client\\Debug\\list.jpg";
	image = imread(filename, 1);

	cedge.create(image.size(), image.type());
	cvtColor(image, gray, COLOR_BGR2GRAY);

	// Create a window
	namedWindow("Edge map", 1);

	// create a toolbar
	createTrackbar("Canny threshold", "Edge map", &edgeThresh, 100, onTrackbar);

	// Show the image
	onTrackbar(0, 0);

	// Wait for a key stroke; the same function arranges events processing
	waitKey(0);

	return 0;
}
*/

//shapes detector 
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <cmath>
#include <iostream>

using namespace cv;
using namespace std;

/**
* Helper function to find a cosine of angle between vectors
* from pt0->pt1 and pt0->pt2
*/
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/**
* Helper function to display text in the center of a contour
*/
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}


int main(void)
{
	cv::Mat src = cv::imread("D:\\project\\open_client\\Debug\\pic3.png");
	if (src.empty())
		return -1;

	// Convert to grayscale
	cv::Mat gray;
	cv::cvtColor(src, gray, CV_BGR2GRAY);

	// Convert to binary image using Canny
	cv::Mat bw;
	cv::Canny(gray, bw, 0, 200, 5);

	// Find contours
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	std::vector<cv::Point> approx;
	cv::Mat dst = src.clone();

	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
			continue;

		if (approx.size() == 3)
		{
			setLabel(dst, "TRI", contours[i]);    // Triangles
		}
		if (approx.size() == 4) setLabel(dst, "sq2", contours[i]);
		if (approx.size() == 5) setLabel(dst, "pentan", contours[i]);
		if (approx.size() == 6) setLabel(dst, "gexa", contours[i]);
		else if (approx.size() >= 4 && approx.size() <= 6)
		{
			// Number of vertices of polygonal curve
			int vtc = approx.size();

			// Get the cosines of all corners
			std::vector<double> cos;
			for (int j = 2; j < vtc + 1; j++)
				cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));

			// Sort ascending the cosine values
			std::sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			// Use the degrees obtained above and the number of vertices
			// to determine the shape of the contour
			if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
				setLabel(dst, "RECT", contours[i]);
			else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
				setLabel(dst, "PENTA", contours[i]);
			else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
				setLabel(dst, "HEXA", contours[i]);
		}
		else
		{
			// Detect and label circles
			double area = cv::contourArea(contours[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			int radius = r.width / 2;

			if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
				std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
				setLabel(dst, "CIR", contours[i]);
		}
	}

	cv::imshow("src", src);
	cv::imshow("dst", dst);
	cv::waitKey(0);
	return 0;
}