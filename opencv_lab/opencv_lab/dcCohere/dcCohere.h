/*
 * dcCohere.h
 *
 *  Created on: 2015. 7. 30.
 *      Author: decoz
 *
 *
 *  응집 이론으로 제작해보는 클러스터링 및 세선화 가능성 타진
 * 	Input : 이진 이미지
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
	int last_size;			// 마지막 윈도우 크기

	dcCohere();
	virtual ~dcCohere();

	void setStep(float step, int type);	// 윈도우 크기 증가량을 계산
												// type = LINEAR_STEP : 	선형증가 next_vs = vs + step
												// type = EXPO_STEP : 		지수증가 next_vs = vs * step + 1

	Mat cohereN(Mat gimg, int cnt); //  size 1 부터 점진적으로  step 만큼 vs를 확장하여 cnt 회만큼 수행
	Mat conquest(Mat map64f, int c_size); // w_size 범위를 기준으로 응집
	Mat oneStep(Mat gimg); // 1스텝 단위로 시작.. 이때 시작 점은 last_size로 세팅가능

	Mat cohereUntil(Mat gimg, float thr); // r2d

	void makeTree(Mat map);
};

#endif /* DESAC_DCCOHERE_H_ */
