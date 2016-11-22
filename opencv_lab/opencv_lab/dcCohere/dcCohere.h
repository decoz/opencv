/*
 * dcCohere.h
 *
 *  Created on: 2015. 7. 30.
 *      Author: decoz
 *
 *
 *  ���� �̷����� �����غ��� Ŭ�����͸� �� ����ȭ ���ɼ� Ÿ��
 * 	Input : ���� �̹���
 *
 *
 */

#ifndef DESAC_DCCOHERE_H_
#define DESAC_DCCOHERE_H_
#include <opencv2/opencv.hpp>
using namespace cv;
<<<<<<< HEAD
using namespace std;
=======
>>>>>>> branch 'master' of https://github.com/decoz/opencv

#define 	LINEAR_STEP 0
#define 	EXPO_STEP	1

class link {
	Point *sp;
	Point *ep;
};


class dcCohere {
private:
	Mat map;
	Mat 	createMap(Mat gimg, int w_size);
	void  selectMax(Mat map, int x, int y, int vs);
	double rectValue(Mat map,Rect r);



	vector <Point> list;

	int init_step,step, step_type;
	double sq;


public:
	int last_size;			// ������ ������ ũ��

	dcCohere();
	virtual ~dcCohere();

	void setStep(float step, int type);	// ������ ũ�� �������� ���
												// type = LINEAR_STEP : 	�������� next_vs = vs + step
												// type = EXPO_STEP : 		�������� next_vs = vs * step + 1

	Mat cohereN(Mat gimg, int cnt); //  size 1 ���� ����������  step ��ŭ vs�� Ȯ���Ͽ� cnt ȸ��ŭ ����
	Mat conquest(Mat map64f, int c_size); // w_size ������ �������� ����
	Mat oneStep(Mat gimg); // 1���� ������ ����.. �̶� ���� ���� last_size�� ���ð���

	Mat cohereUntil(Mat gimg, float thr); // r2d

	void makeTree(Mat map);
};

#endif /* DESAC_DCCOHERE_H_ */
