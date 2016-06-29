/*
 * dcPreprocess.h
 *
 *  Created on: 2015. 8. 25.
 *      Author: cmonyo
 */

#ifndef DCPREPROCESS_H_
#define DCPREPROCESS_H_

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/imgcodecs.hpp"
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

#define uget(x,y)    at<unsigned char>(y,x)
#define uset(x,y,v)  at<unsigned char>(y,x)=v;
#define fget(x,y)    at<float>(y,x)
#define fset(x,y,v)  at<float>(y,x)=v;


class dcPreprocess{
private:

	double calcLocalStats (Mat &im, Mat &map_m, Mat &map_s, int winx, int winy);
	int xGradient(Mat image, int x, int y);
	int yGradient(Mat image, int x, int y);
	Mat WolfJolion (Mat im,  int winx, int winy, double k);//���� ����ȭ
	Mat filtering(Mat image, char type);//opencv ���͸� �߰�����
	Mat hisEqz(Mat src);//�����丮 ��Ȱȭ
	Mat hisEqzClache(Mat src);
	Mat sobelfilter(Mat src);

	Mat HSV(Mat image);
	Mat addWeight(Mat src1,Mat src2,Mat dst);
	Mat Meanfilter(Mat src);
	Mat Medianfilter(Mat src);
	void insertionSort(int window[]);
	int Mean(int window[]);

	int winx,winy;
	float optK;

	char f2dtype; // ������ ������ �����ϴ� �� .. ���� ���� �ȵǾ�����
	char btype;	// ������  �߰��� ����
	bool isflip;

	void initDefaultValue(){
		winy = winx = 40;
		optK = 0.3;
		isflip = false;

		f2dtype='f';
		btype='b';
	};

public:
	dcPreprocess(){
		initDefaultValue();
	};

	Mat preprocess(Mat img);	// ��ó�� ����
	void setFlip(bool _isflip){ isflip = _isflip; }; // �̹��� �¿� ���� ����
	void setValue( int wolf_size, float wolf_K){  // wolf �˰���  �ɼǰ�
		winy = winx = wolf_size;
		optK = wolf_K;
	};


};

#endif /* DCPREPROCESS_H_ */
