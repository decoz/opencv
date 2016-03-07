/*
 * dcMSTree.h
 *
 *  Created on: 2015. 8. 10.
 *      Author: decoz
 *
 *  Minimum Spanning Tree 생성 클래스
 *
 *  input( Point ) 로 점들을 입력받고 받고 *
 *  createTree() 로  Link { Point, Point } 의 벡터를 리턴함
 *
 *
 *
 */

#ifndef SRC_DCMSTREE_H_
#define SRC_DCMSTREE_H_

#include <opencv2/opencv.hpp>
using namespace cv;

class plink{
public:
	int pi1,pi2;
	double l;
};

class Link{
public:
	Point p1,p2;
	Link(Point _p1, Point _p2){ p1 = _p1; p2 = _p2; };
};

class dcMSTree {
private:
	virtual double dist(Point p1,Point p2);
	vector <Point> list;

public:
	void input(Point p);

	vector <Link> createTree();
	dcMSTree();
	virtual ~dcMSTree();

};




#endif /* SRC_DCMSTREE_H_ */
