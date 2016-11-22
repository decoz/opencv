/*
 * dcCohereTree.cpp
 *
 *  Created on: 2015. 9. 15.
 *      Author: decoz
 */

#include "dcCohereTree.h"
#include <math.h>
#include <stdio.h>


dcCohereTree::dcCohereTree() {
	// TODO Auto-generated constructor stub
	length_thr = -1;
}

dcCohereTree::~dcCohereTree() {
	// TODO Auto-generated destructor stub

}

dcCohereTree::dcCohereTree(Mat _map) {
	length_thr = -1;
	map = _map;
} ;

double dcCohereTree::dist(Point p1, Point p2){

	float v1,v2;
	v1 = map.at<double>(p1.y,p1.x);
	v2 = map.at<double>(p2.y,p2.x);

	int xdiff = p1.x - p2.x;
	int ydiff = p1.y - p2.y;
	float dist = sqrt( xdiff * xdiff + ydiff * ydiff ) ;


	if(length_thr > 0 && length_thr < dist ) return -1;
	else 	return dist;

	//double val =   dist / v1 / v2 ;

}
