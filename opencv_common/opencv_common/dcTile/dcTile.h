/*
 * dcTile.h
 *
 *  Created on: 2015. 9. 22.
 *      Author: decoz
 */

#ifndef DCTILE_H_
#define DCTILE_H_

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#define TILE_H 0x0000
#define TILE_V 0x0001

class dcTile {
private:
	int maxw, maxh; 	// 창의 넓이 높이를 각각 또는 동시에 fix 해둠  free 인 경우 0 으로 세팅
	int fixw, fixh; 		// image 의 넓이 높이를 각각 또는 동시에 fix 해둠

	vector <Rect> rects;
	vector <Mat> imgs;

	Point  getMaxXY();
	Mat  	adjustSize(Mat img);
	Mat 	toBGR(Mat img);


public:
	dcTile();
	virtual ~dcTile();

	void fixSize(int w, int h);			// 크기를 fix 함 0인 경우 해당없음
	void tile(short opt, Mat img);		// 이미지의 추가 옵션
											// TILE_H		: 우측에 추가
											// TILE_V 		: 아랫쪽에 추가

	Mat mat();	// tile image를 생성

};

#endif /* DCTILE_H_ */
