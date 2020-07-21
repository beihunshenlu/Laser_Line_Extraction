#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

//方法一：极值法，寻找每一行的最大值
Mat Max_Value(Mat Src);
//方法二：阈值法，从图像两侧向中间找到激光线两侧边界，两侧的中点就是激光线的中点
Mat Fixed_Threshold(Mat Src);
//方法三：灰度质心法，将图像灰度作为质量，寻找每一行的重心坐标作为激光线的坐标
Mat Gray_Weighted(Mat Src);

Mat Max_Value(Mat Src)
{
	Mat img = Src.clone();
	Mat Laser_Line(Src.rows, Src.cols, CV_8UC1);
	cvtColor(Src, img, CV_BGR2GRAY);
	int h = img.rows, w = img.cols;
	int index = 0;
	//这里设置一个阈值，每一行的最大值不一定是激光线，大于这个阈值才认为是激光线
	int threshod = 170;
	for (int i = 0; i < h; i++)
	{
		double max_row = 0;
		for (int j = 0; j < w; j++)
		{
			if (img.at<uint8_t>(i, j) > max_row)
			{
				max_row = img.at<uint8_t>(i, j);
				index = j;
			}
		}
		if (max_row > threshod)
			Laser_Line.at<uint8_t>(i, index) = 255;
	}
	imshow("极值法", Laser_Line);
	waitKey(0);
	return Laser_Line;
}

Mat Fixed_Threshold(Mat Src)
{
	Mat img = Src.clone();
	Mat Laser_Line(Src.rows, Src.cols, CV_8UC1);
	int index1 = 0;
	int index2 = 0;
	int threshod = 170;
	int h = img.rows, w = img.cols;
	cvtColor(img, img, CV_BGR2GRAY);
	//threshold(img, img, threshod, 255, THRESH_BINARY);
	for (int i = 0; i < h; i++)
	{
		//从左侧开始向右查找
		for (int j = 0; j < w; j++)
		{
			if ((img.at<uint8_t>(i, j) > threshod))
			{
				index1 = j;
				cout << i << " index1: " << index1 << " ";
				break;
			}
		}
		//从右侧开始向左查找
		for (int j = w - 1; j >= 0; j--)
		{
			if ((img.at<uint8_t>(i, j) > threshod))
			{
				index2 = j;
				cout << "index2: " << index2 << endl;
				break;
			}
		}
		if ((index1 != 0) && (index2 != 0) && (abs(index1 - index2) < 50))
			Laser_Line.at<uint8_t>(i, (index1 + index2) / 2) = 1;
		index1 = 0;
		index2 = 0;
	}
	imshow("阈值法", Laser_Line);
	waitKey(0);
	return Laser_Line;
}

Mat Gray_Weighted(Mat Src)
{
	Mat img = Src.clone();
	Mat Laser_Line(Src.rows, Src.cols, CV_8UC1);
	int threshod = 150;
	int h = img.rows, w = img.cols;
	cvtColor(img, img, CV_BGR2GRAY);
	medianBlur(img, img, 3);
	threshold(img, img, threshod, 255, 0);
	for (int i = 0; i < img.rows; i++)
	{
		long long sxp = 0, sp = 0, xpos;
		for (int j = 0; j < img.cols; j++)
		{
			sxp += (j*(long long)(img.at<uint8_t>(i, j)));
			sp += (long long)(img.at<uint8_t>(i, j));
		}
		if (sp)
		{
			xpos = sxp / sp;//xpos即为每一行的重心坐标
			//xpos就是这一行的重心纵坐标
			Laser_Line.at<uint8_t>(i, xpos) = 255;
		}
	}
	imshow("灰度质心法", Laser_Line);
	waitKey(0);
	return Laser_Line;
}

int main()
{
	Mat Src = imread("1.bmp", 1);
	Max_Value(Src);
	Fixed_Threshold(Src);
	Gray_Weighted(Src);
	return 0;
}