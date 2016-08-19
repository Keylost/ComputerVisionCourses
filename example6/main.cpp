#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <vector>

using namespace cv;
using namespace std;

vector<Point> num1Templ;
vector<Point> num2Templ;
vector<Point> num3Templ;
vector<Point> num4Templ;
vector<Point> num5Templ;
vector<Point> num6Templ;
vector<Point> num7Templ;
vector<Point> num8Templ;
vector<Point> num9Templ;

struct Number
{
	public:
	vector<Point> templ;
	int num = -1;
	double level = 100.0;
};

vector<Number> Numbers;

//распознает знаки дор движения.
void signsDetector(Mat &image)
{
	
}

void numDetector(Mat &image)
{
	namedWindow("Number",0);
	
	int32_t r,g,b;
	uint8_t *row,*bin;
	Mat numBin(image.rows, image.cols, CV_8UC1);
	for(int32_t rows=0;rows<image.rows;rows++)
	{
		row = (uint8_t*)image.ptr<uint8_t>(rows);
		bin = (uint8_t*)numBin.ptr<uint8_t>(rows);
		for(int32_t col=0;col<image.cols;col++)
		{
			b=row[col*3];g=row[col*3+1];r=row[col*3+2];
			if((b<=100 && abs(b-g)<25 && abs(b-r)<25 && abs(r-g)<25))
			{
				bin[col] = 255;
			}
			else
			{
				bin[col] = 0;
			}
		}
	}
	
	dilate(numBin, numBin, Mat(), Point(-1,-1));
	dilate(numBin, numBin, Mat(), Point(-1,-1));
	
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//findContours(numBin, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
	findContours(numBin, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
	double maxArea = 0;
	unsigned maxI = 0;
	for(unsigned i=0;i<contours.size();i++)
	{
		double area = fabs(contourArea(Mat(contours[i])));
		if(area>maxArea) 
		{
			maxArea = area;
			maxI = i;
		}
	}
	
	if(contours.size()!=0)
	{
		unsigned minI = 0;
		double minL = 100000.0;
		for(unsigned i=0;i<Numbers.size();i++)
		{
			Numbers[i].level = matchShapes(contours[maxI], Numbers[i].templ, CV_CONTOURS_MATCH_I1,0);
			if(Numbers[i].level<minL)
			{
				minI = i;
				minL = Numbers[i].level;
			}
		}
		
		printf("Number: %d\n", Numbers[minI].num);
		char buffer[10];
		cv::Scalar color= CV_RGB(255,0,0);
		double size =1.1;
		snprintf(buffer, sizeof(buffer), "NUM: %d",Numbers[minI].num);
		cv::putText(image, string(buffer), cv::Point(15, 25), cv::FONT_HERSHEY_COMPLEX_SMALL, size, color);
		
	}
	imshow("Number",image);
	waitKey(0);
	destroyWindow("Number");
	
	return;
}

void getTempl(vector<Point> &numVec, const char* file)
{
	Mat frame = imread(file,1);
	int32_t r,g,b;
	uint8_t *row,*bin;
	
	Mat binary(frame.rows, frame.cols, CV_8UC1);
	
	for(int32_t rows=0;rows<frame.rows;rows++)
	{
		row = (uint8_t*)frame.ptr<uint8_t>(rows);
		bin = (uint8_t*)binary.ptr<uint8_t>(rows);
		for(int32_t col=0;col<frame.cols;col++)
		{
			b=row[col*3];g=row[col*3+1];r=row[col*3+2];
			if((b<=100 && abs(b-g)<25 && abs(b-r)<25 && abs(r-g)<25))
			{
				bin[col] = 255;
			}
			else
			{
				bin[col] = 0;
			}
		}
	}
	
	dilate(binary, binary, Mat(), Point(-1,-1));
	dilate(binary, binary, Mat(), Point(-1,-1));
	
	vector<vector<Point> > contours;
	vector<Point> finCont;
	//findContours(binary, contours, RETR_LIST, CHAIN_APPROX_SIMPLE); CV_RETR_EXTERNAL
	findContours(binary, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
	double maxArea = 0;
	unsigned maxI = 0;
	for(unsigned i=0;i<contours.size();i++)
	{
		double area = fabs(contourArea(Mat(contours[i])));
		if(area>maxArea) 
		{
			maxArea = area;
			maxI = i;
		}
	}
	
	//imshow("Test",frame);
	//waitKey();
	
	numVec = contours[maxI];
}

int main(int argc, char** argv)
{
	Number n1; n1.num = 1;
	Number n2; n2.num = 2;
	Number n3; n3.num = 3;
	Number n4; n4.num = 4;
	Number n5; n5.num = 5;
	Number n6; n6.num = 6;
	Number n7; n7.num = 7;
	Number n8; n8.num = 8;
	Number n9; n9.num = 9;
	getTempl(n1.templ,"../img/1.png");
	getTempl(n2.templ,"../img/2.png");
	getTempl(n3.templ,"../img/3.png");
	getTempl(n4.templ,"../img/4.png");
	getTempl(n5.templ,"../img/5.png");
	getTempl(n6.templ,"../img/6.png");
	getTempl(n7.templ,"../img/7.png");
	getTempl(n8.templ,"../img/8.png");
	getTempl(n9.templ,"../img/9.png");
	Numbers.push_back(n1);
	Numbers.push_back(n2);
	Numbers.push_back(n3);
	Numbers.push_back(n4);
	Numbers.push_back(n5);
	Numbers.push_back(n6);
	Numbers.push_back(n7);
	Numbers.push_back(n8);
	Numbers.push_back(n9);
    
    
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
	
	int width = 640;
	int height = 360;
	int fps = 60;
	
	Mat numMat;
	
	cap.set(CV_CAP_PROP_FRAME_WIDTH,width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,height);
	cap.set(CV_CAP_PROP_FPS,fps);
    
    namedWindow("stream",0);
    
    
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        
        Rect numRoi = Rect(Point(245,80),Point(390,280));
        rectangle(frame,numRoi,Scalar(0,255,0),3);
        
        imshow("stream", frame);
        char c = waitKey(10);
        switch(c)
        {
			case 27:
				exit(0);
				break;
			case '1':
				//найти однородное поле и вывести его средний цвет в RGB
				break;
			case '2':
				//распознать фигуры
				break;
			case '3':
				//распознать знаки дор. движения
				break;
			case '4':
				numMat = frame(numRoi);
				numDetector(numMat);
				//распознать цифры
				break;
			case 's':
				imwrite("scrn.png",frame(numRoi));
				break;
			default:
				break;
		}
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
