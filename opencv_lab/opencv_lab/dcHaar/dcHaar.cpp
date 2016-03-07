/*
 * dcHarr.cpp
 *
 *  Created on: 2014. 8. 5.
 *      Author: decoz
 *
 *  haar like feature 를 다루기 위한 초기버젼
 *
 */

#include "dcHaar.h"


void dcHaar::addRect(double w,Rect r){
	WRect wr;
	wr.weight = w;
	wr.r = r;

	rects.push_back(wr);
}

double dcHaar::rectSum(Mat itgMap, Rect r){

	if(r.width < 0 || r.height < 0) return 0;

		if(r.x < 0)  r.x = 0;
		if(r.y < 0)  r.y = 0;
		if(r.x + r.width  > itgMap.cols - 1) r.width = itgMap.cols - r.x - 1;
		if(r.y + r.height  > itgMap.rows  - 1) r.height = itgMap.rows - r.y - 1;


		int x1 = r.x;
		int y1 = r.y;
		int x2 = r.x + r.width;
		int y2 = r.y + r.height;


		double p1,p2,p3,p4;
		p1 = itgMap.at<double>(y1,x1);
		p2 = itgMap.at<double>(y1,x2);
		p3 = itgMap.at<double>(y2,x1);
		p4 = itgMap.at<double>(y2,x2);

		return p1 + p4 - p2 - p3;

		/*
		p1 = cvGet2D(itgMap,y1,x1);
		p2 = cvGet2D(itgMap,y1,x2);
		p3 = cvGet2D(itgMap,y2,x1);
		p4 = cvGet2D(itgMap,y2,x2);

	return p1.val[0] + p4.val[0]  - p2.val[0] - p3.val[0];
		*/
}

double dcHaar::rectMean(Mat itgMap, Rect r){
	return rectSum(itgMap,r) / r.width / r.height;
}

double dcHaar::fitness(Mat itgMap,Rect r){
	bool debug = true;

	double rx = (double)r.width /  max.width;
	double ry = (double)r.height /  max.height;

	double area = 0,value = 0,wsum = 0;

	for(int i=0;i<rects.size();i++){
		double  v;
		WRect wr = rects[i];
		double x = r.x + wr.r.x * rx;
		double y = r.y + wr.r.y * ry;

		int w = wr.r.width * rx;
		int h = wr.r.height * ry;

		area += w * h;
		if(wr.weight > 0 ) wsum += area * wr.weight;
		/*
		wsum += area * abs(wr.weight);

		if(wr.weight > 0) v =   rectSum(itgMap,Rect(x,y,w,h)) * wr.weight;
		else v =  (rectSum(itgMap,Rect(x,y,w,h)) - area * 255 ) * wr.weight;
		*/

		v =   rectSum(itgMap,Rect(x,y,w,h)) * wr.weight;
		value += v;
		if(debug){
			if( i > 0) printf(" + ");
			printf("%.2f",v);
		}
	}

	if(debug) printf(" = [%.2f] ", value / wsum / 255);

	return value / wsum / 255;
}


double dcHaar::fitness_ar(Mat itgMap,Rect r){
	bool debug = false;

	double rx = (double)r.width /  max.width;
	double ry = (double)r.height /  max.height;

	double area = 0,value = 0,wsum = 0;

	bool show = false;

	String s = "";
	for(int i=0;i<rects.size();i++){
		double  v;
		WRect wr = rects[i];
		double x = r.x + wr.r.x * rx;
		double y = r.y + wr.r.y * ry;

		int w = wr.r.width * rx;
		int h = wr.r.height * ry;

		area += w * h;
		if(wr.weight > 0 ) wsum += area * wr.weight;

		/*
		wsum += area * abs(wr.weight);

		if(wr.weight > 0) v =   rectSum(itgMap,Rect(x,y,w,h)) * wr.weight;
		else v =  (rectSum(itgMap,Rect(x,y,w,h)) - area * 255 ) * wr.weight;
		*/

		v =   rectSum(itgMap,Rect(x,y,w,h)) * wr.weight;

		if(i== 0  && v==0) {
			show = true;
		}
		//if(i== 1 && v==0) show = false;

		value += v/255;

		char buffer[50];

		if(debug){
			if( i > 0) s += " + ";
			sprintf(buffer," %.2f ",v);
			s += buffer;
		}
	}

	if(debug && value > 0) {
		char buffer[50];

		sprintf(buffer," = [%.2f]/%.2f \n", value,wsum );
		s += buffer;
		//if( value > 0) printf("%s\n",s.c_str());
		printf("%s\n",s.c_str());
	}

	return value / wsum * 10 ;// 255;
}



Mat dcHaar::createImage(Size s){

	double xr = s.width / max.width;
	double yr = s.height / max.height;

	Mat hImg(max,CV_8U);
	hImg.setTo(127);

	for(int i=0;i<rects.size();i++){
		Mat rMap = hImg(rects[i].r);
		double v = (rects[i].weight * 127);

		Mat temp(Size(rects[i].r.width, rects[i].r.height),CV_64F);
		temp.setTo(v);
		//rMap += temp;
		rMap += v;

		//printf("%d rect weight: %.2f\n",i,v);

	}

	resize(hImg,hImg,s);
	return hImg;
}

dcHaar dcHaar::invert(int option){

	dcHaar ihrr;
	for(int i=0;i<rects.size();i++){
		Rect r = rects[i].r;
		Rect nr = r;
		switch(option){
		case HARR_INVX:
			nr.x = max.width - r.width - r.x;
			break;
		case HARR_INVY:
			nr.y = max.height - r.height - r.y;
			break;
		case HARR_ROTR:
			nr.x = max.height - r.y - r.height;
			nr.y = r.x;
			nr.width = r.height;
			nr.height = r.width;
			break;
		case HARR_ROTL:
			nr.y = max.width - r.x - r.width;
			nr.x = r.y;
			nr.width = r.height;
			nr.height = r.width;
			break;
		}


		ihrr.addRect(rects[i].weight,nr);
	}

	return ihrr;
}


dcHaar::dcHaar() {
	// TODO Auto-generated constructor stub
	max = Size(100,100);
}

dcHaar::~dcHaar() {
	// TODO Auto-generated destructor stub
}

