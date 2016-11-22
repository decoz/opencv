/*
 * dcTile.cpp
 *
 *  Created on: 2015. 9. 22.
 *      Author: decoz
 */

#include "dcTile.h"

dcTile::dcTile() {
	// TODO Auto-generated constructor stub

}

dcTile::~dcTile() {
	// TODO Auto-generated destructor stub
}


Point dcTile::getMaxXY(){

	int mx = 0, my = 0;

	for(int i=0;i<rects.size();i++){
		Rect r = rects[i];
		if( mx < r.x + r.width ) mx = r.x + r.width;
		if( my < r.y + r.height ) my = r.y + r.height;
	}

	return Point(mx,my);
}

Mat  dcTile::adjustSize(Mat img){

	if(!fixw && !fixh) return img;

	Mat  rimg;
	if(fixw > 0 && fixh > 0){
		resize(img, rimg, Size(fixw, fixh));
		return rimg;
	}

	if(fixh > 0) {
		int w = round( (float)fixh /  img.rows  *  img.cols);
		if(w < 1) w = 1;
		resize(img, rimg, Size(w, fixh));
	}

	if(fixw > 0) {
		int h = round( (float)fixw /  img.cols  * img.rows);
		if(h < 1) h = 1;
		resize(img, rimg, Size(fixw, h));
	}

	return rimg;

}

Mat dcTile::toBGR(Mat img){

	if(img.channels() == 1) cvtColor(img,img,CV_GRAY2BGR);
	return img;
}

void dcTile::tile(short opt, Mat img){

	Mat aimg	= adjustSize(toBGR(img));
	Point mx = getMaxXY();

	imgs.push_back(aimg);
	Rect  r;

	switch(opt) {
	case TILE_V:
		r =  Rect(0, mx.y, aimg.cols, aimg.rows );
		break;

	case TILE_H:
		r =  Rect(mx.x, 0, aimg.cols, aimg.rows );
	}

	rects.push_back(r);
}



void dcTile::fixSize(int w, int h){

	fixw = w;
	fixh = h;

}

Mat dcTile::mat(){
	Point p = getMaxXY();

	if(p.x == 0 || p.y == 0) return Mat(100,100, CV_8UC3);

	Mat img(p.y, p.x, CV_8UC3);

	for(int i=0;i<rects.size();i++){
		Rect r = rects[i];
		imgs[i].copyTo( img(r) );
	}

	return img;
}
