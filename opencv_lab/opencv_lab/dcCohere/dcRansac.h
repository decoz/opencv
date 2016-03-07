/*
 * dcRansac.h
 *
 *  Created on: 2015. 8. 11.
 *      Author: decoz
 */

#ifndef SRC_DCRANSAC_H_
#define SRC_DCRANSAC_H_

#include <opencv2/opencv.hpp>
#include "dlbLine.h"

using namespace cv;

class dcRansac {
private:
	int cand_p[3];
	Rect rt;
	vector <Point> v_p, v_in;
	Vec2f best_l;


	Point 	m_sp,m_ep; // modify 된 시작 - 끝점

	void 	select();
	float 	evalHypo(float theta, float h);
	vector <Point> modifyHypo(float theta, float h);



	bool 	extractInlear();

public:
	float thr_range, thr_max_l;
	int hypo_min, r_cnt;

	Point best_p[2];
	Point sp,ep;
	void 	input(Point p);
	float 	run();
	vector <Vec4i> extractLines();
	vector <Point> getLeft() { return v_p; }



	dcRansac();
	virtual ~dcRansac();

};

#endif /* SRC_DCRANSAC_H_ */
