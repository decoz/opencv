/*
 * dlbLine.h
 *
 *  Created on: 2015. 8. 11.
 *      Author: decoz
 *
 *  Line 관련 여러 함수들을 지원하는 클래스
 *
 *	삼각 함수 관련 직선과 좌표계 관련 라이브러리 *
 *	theta, r 를 이용한 직선의 방정식을 지원하며
 *
 *	x*sin(theta) - y*cos(theta) = r
 *
 *	형태로 구성된다.
 *
 */

#ifndef SRC_DLBLINE_H_
#define SRC_DLBLINE_H_

#include <opencv2/opencv.hpp>
using namespace cv;


class dlbLine {
public:
	dlbLine();
	virtual ~dlbLine();
};

float range(Point p1,Point p2); 		// 두 점사이의 거리를 리턴
float ppnLength(Point sp, Point ep, Point pp);  // 점 pp에서 두 점(sp,ep)을 지나는 라인까지 수선의 길이

float line2ang(Point p1, Point p2); 	// 두 점을 지나는 라인의 각도를  0~360 으로 출력
float line2theta(Point p1, Point p2);

float theta2ang(float theta); // theta 를 angle 로
float ang2theta(float ang); 	// angle 을 theta 로
float theta_p2r(float theta, Point p); // theta 의 각도를 가지고 p 를 지나가는 직선의 r
float absTheta(float theta);	// - 각도를 + 로 변환 ( -135 => 45 )

Vec2f p2line(Point p1, Point p2);		// Point 를 theta + r 로 리턴
vector <Point> line2p(Rect rt, float theta, float r);  // line 각도와 r 값이 영역을 통과할 경우의 두점

Point ppnP(float theta , float  r,Point p ); // p 에서 line으로의 수선을 내린 좌표
float ppnR(float theta, float r, Point p);  // p 에서 line으로의 수선의 길이

#endif /* SRC_DLBLINE_H_ */
