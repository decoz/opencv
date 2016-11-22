/*
 * dcHarr.h
 *
 *  Created on: 2014. 8. 5.
 *      Author: decoz
 *
 *      Haar Rect 인 WRect 의 집합
 *
 *
 *
 */

#ifndef DCHARR_H_
#define DCHARR_H_
#include <opencv2/opencv.hpp>
#include <stdio.h>

#define HARR_INVX 0
#define HARR_INVY 1
#define HARR_ROTR 2
#define HARR_ROTL 3

using namespace cv;
<<<<<<< HEAD
using namespace std;

typedef struct _wRect {
	double weight;
	Rect r;
} WRect;



class dcHaar {

private:
	double rectSum(Mat itgMap, Rect r);
	double rectMean(Mat itgMap, Rect r);


public:
	vector<WRect> rects;
=======


typedef struct _wRect {
	double weight;
	Rect r;
} WRect;



class dcHaar {

private:
	double rectSum(Mat itgMap, Rect r);
	double rectMean(Mat itgMap, Rect r);


public:
	Vector<WRect> rects;
>>>>>>> branch 'master' of https://github.com/decoz/opencv
	Size max;
	char *name;

	void addRect(double w, Rect r);

	double fitness(Mat itgMap,Rect r);
	double fitness_ar(Mat itgMap,Rect r);
	Mat createImage(Size s);
	dcHaar invert(int option);


	dcHaar();
	virtual ~dcHaar();
};

#endif /* DCHARR_H_ */
