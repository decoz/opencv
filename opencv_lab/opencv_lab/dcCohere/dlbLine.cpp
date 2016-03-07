/*
 * dlbLine.cpp
 *
 *  Created on: 2015. 8. 11.
 *      Author: decoz
 */

#include "dlbLine.h"
#include <stdio.h>

dlbLine::dlbLine() {
	// TODO Auto-generated constructor stub

}

dlbLine::~dlbLine() {
	// TODO Auto-generated destructor stub
}

float line2theta(Point p1, Point p2){
	float theta =  atan2( float(p2.y - p1.y), float(p2.x - p1.x));
	return theta;
}

float line2ang(Point p1, Point p2){
	float theta = line2theta(p1, p2);
	return theta2ang(theta);
}

float theta2ang(float theta){
	return  theta * 180 / CV_PI;
}


float ang2theta(float ang){
	return CV_PI * ang / 180;
}


float range(Point p1,Point p2){
	int xdiff = p1.x - p2.x;
	int ydiff = p1.y - p2.y;
	return sqrt(xdiff * xdiff + ydiff * ydiff);
}

float ppnLength(Point sp, Point ep, Point pp){

	float theta1 = line2theta(sp,ep);
	float theta2 = line2theta(sp,pp);
	float theta = theta1 - theta2;

	return sin(theta1 - theta2) * range(sp,pp);
}

float ppnR(float theta, float r, Point p)  {

	theta = absTheta(theta);
	float newr = theta_p2r(theta,p);
	return newr - r;
	/*
	Point np = ppnP(theta,r,p);
	return range(np,p);
	*/
}

Point ppnP(float theta , float  r,Point p ) {
	float pr = ppnR(theta, r, p);
	float dx = pr * sin(theta);
	float dy = pr * cos(theta);
	//printf("[pr:%.2f/dx:%.2f, dy:%.2f]\n",pr,dx,dy);
	return Point( p.x - dx, p.y + dy);
}


float theta_p2r(float theta, Point p){
	return p.x * sin(theta) - p.y * cos(theta);
}


Vec2f p2line(Point p1, Point p2){

	Vec2f rv;
	float theta =  line2theta(p1,p2) ;
	rv[0] = theta = absTheta(theta);
	rv[1] = theta_p2r(theta,p1);

	return rv;
}

float absTheta(float theta){
	float ang = theta2ang(theta);
	if(ang < 0) ang += 180;
	return ang2theta(ang);
}

vector <Point> line2p(Rect rt, float theta, float r){ // line 을 Rect 안에 그리기 위한 두 점을 리턴

	printf("line2p: theta:%.2f, r:%.2f\n", theta, r);
	vector <Point> v_p;


	int rx[2]  = {rt.x, rt.x + rt.width};
	int ry[2] =  {rt.y, rt.y + rt.height};


	if( cos(theta) != 0)  for( int i=0;i<2;i++){
		int y = ( rx[i] * sin(theta) - r ) / cos(theta);
		if(y > rt.y && y < rt.y + rt.height) v_p.push_back(Point(rx[i],y));
	}

	if( sin(theta) != 0) for( int i=0;i<2;i++){
		int x = ( r + ry[i]*cos(theta) ) / sin(theta);
		if(x >= rt.x && x <= rt.x + rt.width) v_p.push_back(Point(x,ry[i]));
	}

	return v_p;
}






