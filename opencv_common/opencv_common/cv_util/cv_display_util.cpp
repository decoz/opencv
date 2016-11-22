#include "cv_util.h"
<<<<<<< HEAD
#include "dsRand/dsRand.h"

void putMultiLine(Mat img, const string& text, Point org,	int fontFace, double fontScale, Scalar color, int l_height = 15){

	String linetext = "";

	for(int i=0;i<text.length();i++){
		if(text[i] != 10) linetext += text[i];
		else {
			putText( img, linetext , org, fontFace , fontScale, color );
			linetext = "";
			org.y += l_height;
		}
	}

	if(linetext != "") putText( img, linetext , org, fontFace , fontScale, color );
}


Scalar randcolor(int min, int max){
/*
 *  랜덤한 컬러를 리턴한다.
 *	밝기의 범위를 줄 수 있다.
 */
	rnd::getIt();

	int range = abs( max - min ) * 3;
	int sum = min*3 + rand()%range;

	int cv[3];
	Scalar color(0,0,0);
	for(int i=0;i<3;i++){
		int rmax = sum>255?255:sum;
		cv[i] = rand() % rmax;
		sum -= cv[i];
	}


	fflush(stdout);
	return Scalar(cv[0], cv[1], cv[2]);


=======

void putMultiLine(Mat img, const string& text, Point org,	int fontFace, double fontScale, Scalar color, int l_height = 15){

	String linetext = "";

	for(int i=0;i<text.length();i++){
		if(text[i] != 10) linetext += text[i];
		else {
			putText( img, linetext , org, fontFace , fontScale, color );
			linetext = "";
			org.y += l_height;
		}
	}

	if(linetext != "") putText( img, linetext , org, fontFace , fontScale, color );
>>>>>>> branch 'master' of https://github.com/decoz/opencv
}
