#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

//����һ����ֵ����Ѱ��ÿһ�е����ֵ
Mat Max_Value(Mat Src);
//����������ֵ������ͼ���������м��ҵ�����������߽磬������е���Ǽ����ߵ��е�
Mat Fixed_Threshold(Mat Src);
//���������Ҷ����ķ�����ͼ��Ҷ���Ϊ������Ѱ��ÿһ�е�����������Ϊ�����ߵ�����
Mat Gray_Weighted(Mat Src);

Mat Max_Value(Mat Src)
{
	Mat img = Src.clone();
	Mat Laser_Line(Src.rows, Src.cols, CV_8UC1);
	cvtColor(Src, img, CV_BGR2GRAY);
	int h = img.rows, w = img.cols;
	int index = 0;
	//��������һ����ֵ��ÿһ�е����ֵ��һ���Ǽ����ߣ����������ֵ����Ϊ�Ǽ�����
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
	imshow("��ֵ��", Laser_Line);
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
		//����࿪ʼ���Ҳ���
		for (int j = 0; j < w; j++)
		{
			if ((img.at<uint8_t>(i, j) > threshod))
			{
				index1 = j;
				cout << i << " index1: " << index1 << " ";
				break;
			}
		}
		//���Ҳ࿪ʼ�������
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
	imshow("��ֵ��", Laser_Line);
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
			xpos = sxp / sp;//xpos��Ϊÿһ�е���������
			//xpos������һ�е�����������
			Laser_Line.at<uint8_t>(i, xpos) = 255;
		}
	}
	imshow("�Ҷ����ķ�", Laser_Line);
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