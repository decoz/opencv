/*
 * dcHarrManager.h
 *
 *  Created on: 2014. 8. 5.
 *      Author: decoz
 */

#ifndef DCHARRMANAGER_H_
#define DCHARRMANAGER_H_
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "dcHaar.h"


using namespace cv;
using namespace std;

class dcHaarManager {


	vector <Rect> candi;
	vector <int>  type;

public:
	vector <dcHaar> hlist;

	Mat itgMap,srcImg;
	bool loadHaarFile(char *fname);
	void saveHaarFile(char *fname);
	void loadImage(Mat src);
	void loadItgImage(Mat src);
	void addHarr(dcHaar hrr);
	void scan(Size min, Size max, int step, int move, double thresh);
	void scan(Size s ,int move, double thresh);
	void drawCand(Mat img,Scalar color);

	double harrFitness(int idx,Rect r);
	void clustering();

	dcHaarManager();
	virtual ~dcHaarManager();

private:
	bool cmpRects(Rect r1,Rect r2);
	bool checkRect(Rect r,float thresh);

};

#endif /* DCHARRMANAGER_H_ */
