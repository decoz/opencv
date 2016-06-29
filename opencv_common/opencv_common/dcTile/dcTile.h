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
	int maxw, maxh; 	// â�� ���� ���̸� ���� �Ǵ� ���ÿ� fix �ص�  free �� ��� 0 ���� ����
	int fixw, fixh; 		// image �� ���� ���̸� ���� �Ǵ� ���ÿ� fix �ص�

	vector <Rect> rects;
	vector <Mat> imgs;

	Point  getMaxXY();
	Mat  	adjustSize(Mat img);
	Mat 	toBGR(Mat img);


public:
	dcTile();
	virtual ~dcTile();

	void fixSize(int w, int h);			// ũ�⸦ fix �� 0�� ��� �ش����
	void tile(short opt, Mat img);		// �̹����� �߰� �ɼ�
											// TILE_H		: ������ �߰�
											// TILE_V 		: �Ʒ��ʿ� �߰�

	Mat mat();	// tile image�� ����

};

#endif /* DCTILE_H_ */
