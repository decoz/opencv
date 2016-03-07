/*
 * dlbLine.h
 *
 *  Created on: 2015. 8. 11.
 *      Author: decoz
 *
 *  Line ���� ���� �Լ����� �����ϴ� Ŭ����
 *
 *	�ﰢ �Լ� ���� ������ ��ǥ�� ���� ���̺귯�� *
 *	theta, r �� �̿��� ������ �������� �����ϸ�
 *
 *	x*sin(theta) - y*cos(theta) = r
 *
 *	���·� �����ȴ�.
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

float range(Point p1,Point p2); 		// �� �������� �Ÿ��� ����
float ppnLength(Point sp, Point ep, Point pp);  // �� pp���� �� ��(sp,ep)�� ������ ���α��� ������ ����

float line2ang(Point p1, Point p2); 	// �� ���� ������ ������ ������  0~360 ���� ���
float line2theta(Point p1, Point p2);

float theta2ang(float theta); // theta �� angle ��
float ang2theta(float ang); 	// angle �� theta ��
float theta_p2r(float theta, Point p); // theta �� ������ ������ p �� �������� ������ r
float absTheta(float theta);	// - ������ + �� ��ȯ ( -135 => 45 )

Vec2f p2line(Point p1, Point p2);		// Point �� theta + r �� ����
vector <Point> line2p(Rect rt, float theta, float r);  // line ������ r ���� ������ ����� ����� ����

Point ppnP(float theta , float  r,Point p ); // p ���� line������ ������ ���� ��ǥ
float ppnR(float theta, float r, Point p);  // p ���� line������ ������ ����

#endif /* SRC_DLBLINE_H_ */
