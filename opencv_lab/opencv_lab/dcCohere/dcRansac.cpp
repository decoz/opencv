/*
 * dcRansac.cpp
 *
 *  Created on: 2015. 8. 11.
 *      Author: decoz
 */

#include "dcRansac.h"
#include <time.h>
#include <stdio.h>



dcRansac::dcRansac() {
	// TODO Auto-generated constructor stub
	srand(time(NULL));
	hypo_min = 15;
	thr_range = 15;
	thr_max_l = 40;
	r_cnt = 100;

}

dcRansac::~dcRansac() {
	// TODO Auto-generated destructor stub
}

void dcRansac::input(Point p){
	v_p.push_back(p);
}



float  dcRansac::run(){

	//1 . select
	float max = 0;

	for(int i=0;i<r_cnt;i++){
		select();
		Vec2f v2l = p2line(v_p[cand_p[0]] , v_p[cand_p[1]]);
		vector <Point> v_cur = modifyHypo(v2l[0], v2l[1]);

		float eval = v_cur.size();

		if( max <  eval) {
			max = eval;

			v_in.clear();
			for(int j=0; j< v_cur.size(); j++) v_in.push_back(v_cur[j]);

			sp = v_in[0];
			ep = v_in[v_in.size() - 1];
		}
	}

	return max;


}

vector <Vec4i>dcRansac::extractLines(){

	printf("run desac rannge:%.2f max_length:%.2f \n", thr_range, thr_max_l);
	printf("    mini hypo %d, random_c:%d \n", hypo_min, r_cnt);


	vector <Vec4i> v_vp;
	hypo_min = 20;
	int gen = 0;
	while( v_p.size() > hypo_min ){
		printf("gen:%d pop:%d\n",gen++,v_p.size() );
		if( run() > hypo_min ) 	{
			extractInlear();
		//if(v2l[0] == -1)   break;

			printf("%d,%d-%d,%d ",sp.x, sp.y, ep.x, ep.y);
			printf("Gene best size:%d\n",v_in.size());

			Vec4i v4p;
			v4p[0] =sp.x;	v4p[1] = sp.y;
			v4p[2] =ep.x; v4p[3] = ep.y;

			v_vp.push_back(v4p);
		}
		else break;
	}

	return v_vp;
}

bool  dcRansac::extractInlear(){
/* 점들중에 Inlear 점들을 골라내 시작-끝점을  생성하고
 * 나머지 점들은 다음세대로 전달
 *
 *만일 Inlear 의 숫자가 hypo_min  보다 적으면
 * false 를 리턴한다.	 *
 */

	vector <Point> v_out;

	for(int i=0;i<v_p.size();i++){
		bool exist = false;
		for(int j=0;j<v_in.size();j++)
			if(v_p[i] == v_in[j]) exist = true;

		if(!exist) v_out.push_back(v_p[i]);
	}

	v_p.clear();
	v_p = v_out;

	return true;
}




float dcRansac::evalHypo(float theta, float h){
/*
 * line 의 가설을 해당 point 가 많은 정도로 평가한다.
 */

	int count = 0; // 가설 적합점 숫자
	for(int i = 0; i<v_p.size(); i++){
		Point p =  ppnP(theta, h, v_p[i]);
		if(range(p,v_p[i]) < thr_range) count++;
	}

	return  (float) count;
}

bool cmpByX(Point p1, Point  p2){
		return p1.x  > p2.x;
}

bool cmpByY(Point p1, Point  p2){
		return p1.y  > p2.y;
}



vector <Point> dcRansac::modifyHypo(float theta, float h){
/*
 *  해당하는 점들중  간격의 임계치를 기준으로 연결된 가장 긴 점들의 집합을
 *  v_in 에 저장
 */
	vector <Point> v_cand,  v_cur;


	int minx,miny,maxx,maxy;
	for(int i = 0; i<v_p.size(); i++){ 			//  분포 범위 계산
		Point p =  ppnP(theta, h, v_p[i]);
		if(range(p,v_p[i]) < thr_range) {

			v_cand.push_back(v_p[i]);
		}
	}

	int pca = 0;									// pca 설정,  범위 설정
	if(abs(cos(theta)) > abs(sin(theta)) ) pca = 1;



	if(!pca)  sort( v_cand.begin(), v_cand.end(), cmpByX);
		else sort( v_cand.begin(), v_cand.end(), cmpByY);

	float stdiff = 0;
	for(int i=0;i<v_cand.size()-1;i++) stdiff += range(v_cand[i], v_cand[i+1]);
	stdiff /= v_cand.size() - 1;

	//max_l = stdiff * 1.5;

	//max_l =range(v_cand[0], v_cand[v_cand.size()-1]) * 0.2;

	int s = 0, old_s = 0, old_l = 0;			// 가장 긴 구간을 찾음
	for(int i=0;i<v_cand.size();i++){
		Point p1 = ppnP(theta, h, v_cand[i]);
		Point p2 = ppnP(theta, h, v_cand[i+1]);
		if(range(p1,p2) > thr_max_l ) {
			int cur_l = i+1 - s;
			if( cur_l > old_l ) {
				old_l = cur_l;
				old_s = s;
			}
			s = i+1;
		}
	}


	for(int i=old_s;i<old_s + old_l;i++) {
		//printf("%d,%d ", v_cand[i].x, v_cand[i].y );
		v_cur.push_back(v_cand[i]);
	}
	//printf("\n");

	return v_cur;
}



void  dcRansac::select(){ // random 하게 점 두개를 선택

	int size = v_p.size();
	//printf("select 2 num from %d\n",size);
	for(int i=0;i<2;i++){
		int sel = rand() % ( size - i );
		for(int j=0;j<i;j++) if(cand_p[j] < sel ) sel++;
		cand_p[i] = sel;
		Point p = v_p[sel];
		//printf("%d:%d,%d ",sel,p.x,p.y);
	}

}



