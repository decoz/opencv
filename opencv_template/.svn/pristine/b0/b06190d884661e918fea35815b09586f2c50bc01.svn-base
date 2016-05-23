#include <opencv2/opencv.hpp>
#include "dcTile/dcTile.h"
#include "dcPreprocess/dcPreprocess.h"
#include "cv_util/cv_util.h"

using namespace cv;

void 	testWStringUtil();
void 	testPreprocessor(Mat img);
void 	testRectfunction();
int main ( int argc, char **argv ){

	/*
	Mat img = imread("jaewae_0500.jpg");
	testPreprocessor(img);
	*/
	//testWStringUtil();
	testRectfunction();

	waitKey(0);
}

void 	testRectfunction(){

	Rect r2(0,0,100,200);
	Rect r1(50,50,200,100);

	Rect r3 = interRect(r1,r2);
	printf("%d,%d,%d,%d\n",r3.x, r3.y, r3.width, r3.height);

}


void testWStringUtil(){

	String s = "123하나둘셋";
	printf("char cnt:%d\n", wcharcnt(s));
	printf("4th  char is %s", wcharAt(s, 1).c_str());


}

void testPreprocessor(Mat img){

	dcPreprocess dp;

	dp.setFlip(true);
	Mat primg = dp.preprocess(img);

	imshow("dp test", primg);
}


void testTile(Mat img){
	dcTile tile;
	tile.fixSize(0,100);

	tile.tile(TILE_H,img);
	tile.tile(TILE_H,img);
	tile.tile(TILE_V,img);

	imshow("test1", tile.mat());

}
