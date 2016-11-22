/*
 * dcHarrManager.cpp
 *
 *  Created on: 2014. 8. 5.
 *      Author: decoz
 */

#include "dcHaarManager.h"
#include <stdio.h>
#include <stdlib.h>


dcHaarManager::dcHaarManager() {
	// TODO Auto-generated constructor stub

}

dcHaarManager::~dcHaarManager() {
	// TODO Auto-generated destructor stub

}

void dcHaarManager::saveHaarFile(char *fname){

	FILE *f = fopen(fname,"w");

	for(int i=0;i<hlist.size();i++){
		dcHaar hr = hlist[i];
		fprintf(f,"%s\n",hr.name);
		for(int j=0;j<hr.rects.size();j++){
			Rect r = hr.rects[j].r;
			fprintf(f,"%d, %d, %d, %d, %d\n",
				r.x, r.y, r.width,  r.height,
				(int) ( hr.rects[j].weight * 100 )
			);
		}
		fprintf(f,"\n");
	}
}

bool dcHaarManager::loadHaarFile(char *fname){
	int bufSize = 100000;
	/*
	char* strTemp = new char[bufSize];
	char* str_temp = new char[bufSize];
	char* temp_str_pt  = new char[bufSize];
	*/
	char* temp_str_pt;
	char *str = new char[bufSize];
	int array_temp[bufSize];

	WRect hfeatures;
	dcHaar *thfeatures =NULL;

	FILE *f = fopen(fname,"r");

	if(f != NULL){
		while(!feof(f)){

			int count=0;
			fgets(str,bufSize,f);
			//strcpy(str_temp,str);

			temp_str_pt = strtok(str,",");

			while(temp_str_pt != NULL){

				array_temp[count++] = atoi(temp_str_pt);
				temp_str_pt=strtok(NULL,",");
			}

			if(count == 5){

				hfeatures.r.x = array_temp[0];
				hfeatures.r.y = array_temp[1];
				hfeatures.r.width = array_temp[2];
				hfeatures.r.height = array_temp[3];
				hfeatures.weight= (double) array_temp[4] / 100;

				thfeatures->rects.push_back(hfeatures);
			}
			else if( count == 1 && strlen(str) > 2){

				if(thfeatures !=NULL){
					addHarr(*thfeatures);
				}
				thfeatures = new dcHaar;
				thfeatures->name = new char[strlen(str)];
				strcpy(thfeatures->name, str);

			}
		}

		if(thfeatures != NULL && thfeatures->rects.size() > 0  ){
			addHarr(*thfeatures);
		}

		return true;

	}else{
		return false;
	}

}


void dcHaarManager::loadImage(Mat src){

	Mat gray;
	if(src.channels() > 1) cvtColor(src,gray,CV_BGR2GRAY);
	else gray = src;
	itgMap = Mat(gray.rows + 1,gray.cols + 1, CV_64F);

	srcImg = src;
	integral(gray, itgMap, CV_64F);

}

void dcHaarManager::addHarr(dcHaar hrr){

	hlist.push_back(hrr);

}


void dcHaarManager::loadItgImage(Mat src){

	itgMap = src;

}


double dcHaarManager::harrFitness(int idx,Rect r){

	dcHaar hrr = hlist[idx];
	return hrr.fitness(itgMap,r);


}

bool dcHaarManager::checkRect(Rect r,float  thresh){

	int x1 = r.x;
	int y1 = r.y;
	int x2 = r.x + r.width;
	int y2 = r.y + r.height;

	double p1,p2,p3,p4;
	p1 = itgMap.at<double>(y1,x1);
	p2 = itgMap.at<double>(y1,x2);
	p3 = itgMap.at<double>(y2,x1);
	p4 = itgMap.at<double>(y2,x2);

	if(p1 + p4 - p2 - p3 > r.width * r.height * thresh) return true;
	else return false;

}

void dcHaarManager::scan(Size min, Size max, int step, int move, double thresh){

	int w_c = (max.width - min.width) / step;
	int h_c = (max.height - min.height) / step;

	for(int i=0;i<step;i++){
		Size s(min.width + i * w_c, min.height + i * h_c );
		scan(s,move,thresh);
	}
}


void dcHaarManager::scan(Size s ,int move, double thresh){

	candi.clear();

	for(int x = 0;x < srcImg.cols - s.width; x+= move)
		for(int y = 0;y < srcImg.rows - s.height; y+= move){
			Rect r(x,y,s.width,s.height);
			if(checkRect(r,thresh * 255))
				for(int i=0;i<hlist.size();i++){
					if(hlist[i].fitness_ar(itgMap,r) > thresh){
						candi.push_back(r);
						type.push_back(i);
					}
				}
		}

}

void dcHaarManager::drawCand(Mat img,Scalar color){


	for(int i=0;i<candi.size();i++){
		Rect  r = candi[i];
		rectangle(img,
				Point(r.x,r.y),
				Point(r.x + r.width, r.y + r.height),
				color,1,8);
	}

}

void dcHaarManager::clustering(){

	int n = candi.size();

	int group[n];

	for(int i=0;i<n;i++) group[i] = 0;
	int  gc = 1;

	for(int i=0;i<n;i++)
		for(int j=0;j<i;j++){
			if(cmpRects(candi[i],candi[j])){
				if( group[i] == 0)
					if( group[j] == 0){
						group[i] = group[j] = gc++;
					}
					else group[i] = group[j];
				else if(group[j] != 0){
					int t = group[j];
					for(int k=0;k<n;k++)
						if(group[k] == t) group[k] = group[i];

				}
				else group[j] = group[i];
			}
		}


	vector <Rect> clusters[gc];

	for(int i=0;i<n;i++){
		int g  = group[i];
		clusters[g].push_back(candi[i]);
	}


	for(int g=1;g<gc;g++){
		int xsum1 = 0,ysum1 = 0;
		int xsum2 = 0,ysum2 = 0;
		int c = clusters[g].size();
		for(int i=0;i<c;i++){
			Rect r = clusters[g][i];
			xsum1 += r.x ;
			ysum1 += r.y ;
			xsum2 += r.x + r.width;
			ysum2 += r.y + r.height;
		}

		if(c > 0){
			xsum1 /= c; xsum2 /= c;
			ysum1 /= c; ysum2 /= c;
			clusters[0].push_back(Rect(xsum1,ysum1,xsum2 - xsum1, ysum2 - ysum1));
		}

	}

	candi.clear();
	for(int i=0;i<clusters[0].size();i++){
		Rect r = clusters[0][i];
		candi.push_back(r);
	}


}

bool dcHaarManager::cmpRects(Rect r1,Rect r2){
	double thresh = 0.7;
	int w = 0,h = 0;

	double size = r1.area();
	if( size < r2.area()) size = r2.area();


	if(r1.x > r2.x){
		Rect t = r2;
		r2 = r1;
		r1 = t;
	}

	if( r1.x + r1.width > r2.x){
		w = r1.x + r1.width - r2.x;

		if(r1.y > r2.y){
			Rect t = r2;
			r2 = r1;
			r1 = t;
		}

		if( r1.y + r1.height > r2.y){
			h = r1.y + r1.height - r2.y;
		}

	}

	double v = w * h /  size;

	if(v > 0){
		/*printf("[%d,%d,%d,%d] - [%d,%d,%d,%d]",
			r1.x, r1.y, r1.width, r1.height,
			r2.x, r2.y, r2.width, r2.height
		);*/
		//printf("%d x %d : %.2f",w,h, v);

	}
	//printf("\n");
	//fflush(stdout);
	if(v > thresh) return true;
	else return false;

}
