/*
 * dcCohereTree.h
 *
 *  Created on: 2015. 9. 15.
 *      Author: decoz
 *
 *  Coehere �� �������� ����� ����ġ�� ������
 *  Ʈ�� ������ ���� Ŭ����
 *
 *
 */

#ifndef SRC_DCCOHERETREE_H_
#define SRC_DCCOHERETREE_H_

#include "dcMSTree.h"


class dcCohereTree: public dcMSTree {
private:
	double dist(Point p1, Point p2);
	Mat map;
	double length_thr;

public:
	dcCohereTree(Mat _map) ;

	dcCohereTree();
	double setLinkMax(float val) { length_thr = val ; }
	virtual ~dcCohereTree();
};




#endif /* SRC_DCCOHERETREE_H_ */
