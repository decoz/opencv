#include "cv_util.h"

using namespace cv;

Mat asBGR(Mat src){
	Mat bgr;
	if(src.channels() < 3){
		cvtColor(src,bgr,CV_GRAY2BGR);
	} else {
		bgr = src;
	}

	return bgr;
}

Mat asGray(Mat src){
	Mat gray;
	if(src.channels() >= 3){
		cvtColor(src,gray,CV_BGR2GRAY);
	} else {
		gray = src;
	}

	return gray;

}

Mat asBgBlack(Mat gray){
	if( sum(gray)[0] > 127 * gray.rows * gray.cols ) bitwise_not(gray,gray);
	return gray;
}

Mat asBgWhite(Mat gray){
	if( sum(gray)[0] < 127 * gray.rows * gray.cols ) bitwise_not(gray,gray);
	return gray;
}


/*
Rect rectsum(Rect r1,Rect r2){

	if(r1.x < 0 && r1.y < 0) return r2;

	Rect r;
	if(r1.x < r2.x ) r.x = r1.x;
	else r.x = r2.x;

	if(r1.y < r2.y ) r.y = r1.y;
	else r.y = r2.y;

	Point p1(r1.x+r1.width,	r1.y+r1.height);
	Point p2(r2.x+r2.width,	r2.y+r2.height);
	Point p;

	if(p1.x > p2.x) p.x = p1.x;
	else p.x = p2.x;

	if(p1.y > p2.y ) p.y = p1.y;
	else p.y = p2.y;

	r.width = p.x - r.x;
	r.height = p.y - r.y;

	return r;
}
*/
