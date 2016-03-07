/*
 * dcCohere.cpp
 *
 *  Created on: 2015. 7. 30.
 *      Author: decoz
 */

#include "dcCohere.h"
#include <stdio.h>
#include "dcMSTree.h"



dcCohere::dcCohere() {
	// TODO Auto-generated constructor stub
	last_size = 0;
	step = 1;
	step_type = LINEAR_STEP;
}

dcCohere::~dcCohere() {
	// TODO Auto-generated destructor stub
}

Mat dcCohere::createMap(Mat _map,int w_size){ // 밀집도 Map 을 생성

	int vs = w_size;
	//Mat gimg = _gimg/255;
	int h = _map.rows, w = _map.cols;

	// Integral map 생성
	Mat imap = Mat(h, w, CV_64F);  // integral map
	map = Mat(h, w, CV_64F);
	map.setTo(0);

	integral(_map,imap,CV_64F);

	// 점들의 Matrix 생성
	for(int x = vs; x < w - vs - 1; x++ ){
		for(int y = vs;  y < h - vs - 1; y++){
			if(_map.at<double>(y,x) == 0 )  continue;
			Rect r(x-vs, y-vs, vs*2+1,  vs*2+1);
			double v = rectValue(imap,r);

			map.at<double>(y,x) =  v;// * sq;
		}


	}

	/* 비쥬얼 창 생성 */

	return map;
}

Mat dcCohere::cohereUntil(Mat gimg, float thr){ // r2d


	Mat map64f;
	gimg.convertTo(map64f,CV_64F);
	Mat b_map64f = map64f;


	int vs = 0;
	int round = 0;
	bool cont = true;


	while(cont){
		map64f = conquest(map64f, vs);
		int cnt = 0;
		double sum = 0;
		for(int x=0; x<map64f.cols; x++)
			for(int y=0; y<map64f.rows; y++) {
				double v = map64f.at<double>(y,x);
				if(v > 0) {
					sum += v;
					cnt++;
				}
			}

		printf("step%d : cnt:%d average: %.2f\n", round++, cnt, sum/cnt);
		if(sum/ cnt < thr ) {
			cont =false;
		}
		else  b_map64f = map64f;
	}

	b_map64f.convertTo(gimg,CV_8UC1);
	return gimg;

}


Mat dcCohere::cohereN(Mat gimg, int cnt){

	Mat map64f;
	gimg.convertTo(map64f,CV_64F);


	int vs = 0;
	for(int i=0;i<cnt;i++){
		map64f = conquest(map64f, vs);
	}

	map64f.convertTo(gimg,CV_8UC1);
	return gimg;
}



Mat dcCohere::oneStep(Mat gimg){

	Mat map64f;
	gimg.convertTo(map64f,CV_64F);

	map64f = conquest(map64f, last_size);

	map64f.convertTo(gimg,CV_8UC1);
	return gimg;
}


Mat dcCohere::conquest(Mat map64f ,int c_size){

		int vs;
		switch(step_type) {
			case LINEAR_STEP: vs = last_size + step; break;
			case EXPO_STEP: vs = last_size * step + 1; break;
		}

		map64f = createMap(map64f,vs);

		int h = map64f.rows, w = map64f.cols;

		//sq =  ( (double) last_size * 2 + 1 ) / ( vs * 2 + 1 );
		sq =  ( (double) last_size + 1 ) / ( vs * 2 + 1 );
		sq = sq * sq;
		printf("last:%d, vs:%d , sq = %.2f, \n",last_size, vs, sq);

		fflush(stdout);

		for(int x = vs; x < w - vs - 1; x ++ )
			for(int y = vs;  y < h - vs - 1; y++){
				if(map64f.at<double>(y,x)  > 0)	selectMax(map64f,x,y,vs);
			}

		map64f = map64f * sq;

		last_size = vs;

		return map64f;


}


void dcCohere::makeTree(Mat map){

	dcMSTree mst;

	// 리스트 생성
	int count = 0;
	for(int x=0;x<map.cols;x++)
		for(int y=0;y<map.rows;y++)
			if(map.at<double>(y,x) > 0) mst.input(Point(x,y));


	vector  <Link> v_L =mst.createTree();

	for(int i=0;i<v_L.size();i++){
		line(map, v_L[i].p1, v_L[i].p2, Scalar(255,0,0) , 0);
	}
}

/* old backup

void dcCohere::selectMax(Mat map, int x, int y, int vs){
// 주번 vs (전체: vs* 2 + 1)  영역에서 최고 값일 경우 나머지를 모두 0로 세팅

	double cv = map.at<double>(y, x);
	//printf("%.2f ", cv);
	for(int xi = vs * -1; xi <= vs ; xi++)
		for(int yi = vs * -1; yi <= vs ; yi++){
			if( xi == 0  &&  yi == 0 ) continue;
			int v = map.at<double>(y + yi, x + xi);
			if( v > cv ) {
				map.at<double>(y, x) = 0;
				return;
			}
		}

	for(int xi = vs * -1; xi <= vs ; xi++)
		for(int yi = vs * -1; yi <= vs ; yi++){
			if( xi == 0  &&  yi == 0 ) continue;
			map.at<double>(y + yi, x + xi) = 0;
		}
}
*/


void dcCohere::selectMax(Mat map, int x, int y, int vs){
// 주번 vs (전체: vs* 2 + 1)  영역에서 최고 값일 경우 나머지를 모두 0로 세팅

	double cv = map.at<double>(y, x);
	//printf("%.2f ", cv);


	for(int xi = vs * -1; xi <= vs ; xi++)
		for(int yi = vs * -1; yi <= vs ; yi++){
			if( xi == 0  &&  yi == 0 ) continue;
			int v = map.at<double>(y + yi, x + xi);
			if( v > cv ) {
				map.at<double>(y, x) = 0;
				return;
			}
		}

	for(int xi = vs * -1; xi <= vs ; xi++)
		for(int yi = vs * -1; yi <= vs ; yi++){
			if( xi == 0  &&  yi == 0 ) continue;
			if( map.at<double>(y + yi, x + xi) > 2)
				map.at<double>(y + yi, x + xi) = 0;
		}
}


void dcCohere::setStep(float _step, int type){
	init_step = step = _step;
	step_type = type;

}

double dcCohere::rectValue(Mat map,Rect r){

	double p1 = map.at<double>(r.y, r.x);
	double p2 = map.at<double>(r.y, r.x + r.width);
	double p3 = map.at<double>(r.y + r.height, r.x);
	double p4 = map.at<double>(r.y + r.height,r. x + r.width);

	return p4 - p2  - p3  +  p1;

}
