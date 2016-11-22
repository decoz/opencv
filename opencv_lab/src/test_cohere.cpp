#include <opencv2/opencv.hpp>
#include "dcCohere/dcCohere.h"
#include "dcCohere/dcCohereTree.h"
#include "dcCohere/dcRansac.h"
#include "dcCohere/dlbLine.h"
#include <stdio.h>

<<<<<<< HEAD
using namespace cv;

void test_dlbLine();
void showMapBycolor(Mat gimg);
void test_ransac_from_cohere(Mat gimg, int vs);
void show_cohere_tree(Mat gimg, int last_size);

int main ( int argc, char **argv ){

	dcCohere coh;

	//test_dlbLine();
=======

using namespace cv;

void test_dlbLine();
void showMapBycolor(Mat gimg);
void test_ransac_from_cohere(Mat gimg, int vs);
void show_cohere_tree(Mat gimg, int last_size);

int main ( int argc, char **argv ){

	dcCohere coh;


	//test_dlbLine();

>>>>>>> branch 'master' of https://github.com/decoz/opencv

	if(argc < 2) printf("usage: loader [image filename]+ \n");

	for(int i=1;i<argc;i++){

		printf("%s\n",argv[i]);
		Mat gimg = imread(argv[i],CV_LOAD_IMAGE_GRAYSCALE );

		bitwise_not(gimg,gimg);
		threshold(gimg,gimg,10,255,0);

		imshow("gray scale image",gimg);
		coh.setStep(1,LINEAR_STEP);

		char k;

		do {
		//gimg = coh.oneStep(gimg);
		//gimg = coh.cohereUntil(gimg,100);

		gimg =  coh.cohereN(gimg, 1);
		Mat thr;
		threshold(gimg,thr,1,255,0);

		imshow("cohere  map",thr);
		showMapBycolor( gimg);
		//   show_cohere_tree(gimg, coh.last_size);
			//test_ransac_from_cohere(gimg,coh.last_size);


		} while(  (k = waitKey(0)) == ' ' );
		if(k == 'q') break;
	}

	return 0;

}

void showMapBycolor(Mat gimg){

	Mat cimg(gimg.size(), CV_8UC3);
	cimg.setTo(Vec3b(0,0,0) );

	Vec3b color[6] = {
			 Vec3b(100,100,100),
			 Vec3b(255,0,0),
			 Vec3b(0,255,0),
			 Vec3b(0,255,255),
			 Vec3b(0,0,255),
			 Vec3b(255,0,255),

	};

	//threshold(gimg,cimg,10,255,0);

	for(int x = 0; x < gimg.cols; x++)
		for(int y=0; y<gimg.rows; y++){
			uchar v = gimg.at<uchar>(y,x);
			if(!v) continue;

			cimg.at<Vec3b>(y,x) = color[v/50];
		}

	for(int i=0;i<6;i++) circle(cimg,Point(20,10*i + 5),3,(Scalar)color[i],CV_FILLED);

	imshow("cimg",cimg);




}



void test_ransac_from_cohere(Mat gimg, int vs){

	dcRansac rsc;
	int w = vs * 2 + 1;
	rsc.thr_max_l = w * 3;
	rsc.thr_range = w * 2;

	Mat img;

	cvtColor(gimg,img,CV_GRAY2BGR);

	for(int x=0; x < gimg.cols; x++)
		for( int y=0; y<gimg.rows; y++)
			if( gimg.at<char>(y,x) > 0 ) rsc.input(Point(x,y));


	vector <Vec4i> v_vl = rsc.extractLines();

	for( int i=0;i<v_vl.size();i++ ){
		Vec4i v4l = v_vl[i];
		Point sp(v4l[0],v4l[1]),  ep(v4l[2], v4l[3]);

		circle(img,sp,5,Scalar(0,0,255),CV_FILLED);
		circle(img,ep,5,Scalar(0,0,255),CV_FILLED);
		line(img, sp, ep, Scalar(0,0,255));
	}

	vector <Point> v_p = rsc.getLeft();
	for(int i=0;i<v_p.size(); i++){
		//img.at<Vec3i>(v_p[i].y, v_p[i].x) = Vec3i(255,0,255);
		circle(img,v_p[i],1,Scalar(255,0,255),CV_FILLED);
	}


	imshow("src",img);

}


void show_cohere_tree(Mat gimg, int last_size){
/*
 * cohere 데이터를 받아서 MST를 만들되
 * 거리에 각 노드의 value 값을 반비례로 적용한다.
 */
	dcCohereTree ctree(gimg);

	for(int x=0; x<gimg.cols; x++)
		for(int y=0; y<gimg.rows; y++)
			if( gimg.at<uchar>(y,x) > 0 ){
				ctree.input(Point(x,y));
			}

	ctree.setLinkMax((float)last_size * 3);

	vector <Link> links = ctree.createTree();
	Mat img;
	cvtColor(gimg,img, CV_GRAY2BGR);
	for(int i=0; i<links.size(); i++) {
		//circle(img,links[i].p1,5,Scalar(0,0,255),CV_FILLED);
		//circle(img,links[i].p2,5,Scalar(0,0,255),CV_FILLED);
		line(img, links[i].p1, links[i].p2, Scalar(0,0,255));
	}

	imshow("tree",img);

}



void test_dlbLine(){

	Point  p1(300,100), p2(300,200);
	Mat img(Size(600,600) , CV_8UC3);

	Vec2f v = p2line(p2,p1);

	vector <Point> v_p = line2p(Rect(0,0,600,600), v[0], v[1]);

	for(int i=0;i<v_p.size();i++) 	printf("%d,%d ",v_p[i].x, v_p[i].y );
	printf("\n");

	printf("angle %.2f r:%.2f \n", theta2ang(v[0]), v[1] );
	printf("clipping point count: %d \n",v_p.size());

	if(v_p.size() ==  2){
		line(img, v_p[0],v_p[1],Scalar(50,50,100), 0);
	}
	line(img, p1,p2,Scalar(0,0,255), 0);
	Point p[3];
	p[0] = Point(500,300);
	p[1] = Point(50,200);
	p[2] = Point(400,350);

	for( int i=0;i<2;i++) {
		printf("ppnR:%.2f \n" , ppnR(v[0],v[1],p[i]));
		line(img, p[i], ppnP(v[0],v[1],p[i]), Scalar(0,255,0));
	}

	imshow("test_dlbLIne", img);
	waitKey(0);
}
