/*
 * dcLearn.cpp
 *
 *  Created on: 2016. 1. 14.
 *      Author: decoz
 */

#include "dcLearn.h"
#include <stdio.h>
#include <string>

using namespace cv;
using namespace cv::ml;
/*
dcLearn::dcLearn() {
	// TODO Auto-generated constructor stub

}
*/

namespace learn {

dcLearn::~dcLearn() {
	// TODO Auto-generated destructor stub
}

dcLearn::dcLearn(){

}



void 	dcLearn::save_cases(char *fname){
// training data 를 저장

	printf("writing cases \n");
	fflush(stdout);
	FileStorage fs(fname, FileStorage::WRITE);

	fs << "case" << cases;
	fs << "label"	<< labels;

	fswrite_labelName(fs);
	fswrite_modelInfo(fs);

	fs.release();

}



void 	dcLearn::load_cases(char *fname){
// training data 를 저장

	FileStorage fs(fname, FileStorage::READ);

	fs["case"] >> cases;
	fs["label"]	>> labels;

	lbl_names.clear();
	fnread_labelName(fs["label_name"]);
	fnread_modelInfo(fs[""]);

	printf("loaded cases from %s\n", fname);
	fs.release();
}




bool dcLearn::feed_back(Mat f_mat, int target ){

	return false;
}

void 	dcLearn::case_img(Mat img, string lbl_name){

	int label = get_label(lbl_name);

	if(img.channels() > 1) cvtColor(img,img,CV_BGR2GRAY);

	Mat fmat = md->make(img);

	cases.push_back(fmat);
	labels.push_back(label);

}

int	dcLearn::get_label(string lbl_name, bool pushnew ){
/*
 * label matrix 를 돌며 해당 레이블을 찾아서 인덱스릴 리턴한다.
 * pushnew 가 true 이면 없는 레이블은 새로 생성한다.
 */

	for(int i=0;i<lbl_names.size();i++)
		if( lbl_name == lbl_names[i] ) return i;

	if(pushnew) {
		lbl_names.push_back(lbl_name);
		return lbl_names.size() -  1;
	} else 	return  - 1;
}



void dcLearn::build_classifier(){

	clsf->train(cases, labels);
}

string	 dcLearn::predict_img(Mat img){

	if(img.channels() > 1) cvtColor(img,img,CV_BGR2GRAY);

	int label = clsf->predict( md->make(img) );

	if(lbl_names.size() > label)	return lbl_names[label];
	else {
		char buff[10];
		sprintf(buff,"%d",label);
		return string(buff);
	}
}


void 	dcLearn::save_classifier(char *fname){

	clsf->save(fname);

	FileStorage fs(fname, FileStorage::APPEND);
	fswrite_labelName(fs);
	fs << "classifier_name" << clsf->name;
	fswrite_modelInfo(fs);
	fs.release();

}


void 	dcLearn::load_classifier(char *fname){

	clsf = NULL;
	FileStorage fs(fname, FileStorage::READ);
	lbl_names.clear();
	fnread_labelName( fs["label_name"]);
	fnread_modelInfo( fs[""]);

	clsf_name = (string) fs["classifier_name"];
	fs.release();

	if(clsf_name ==  "svm" ) clsf =  new clsf_svm();
	if(clsf_name ==  "knn" ) clsf = new clsf_knn();

	if(clsf != NULL) clsf->load(fname);

	printf("loaded %s as classifier \n", fname);
}


void	dcLearn::fswrite_labelName(FileStorage &fs ){
// label 이름을 저장
	fs << "label_name" << "[";
	for(int i=0;i<lbl_names.size();i++)	fs <<  lbl_names[i] ;
	fs << "]";
}

void	dcLearn::fswrite_modelInfo(FileStorage &fs ){
// label 이름을 저장

	fs << "model_name" << md->name;
	fs << "model_config" << "{";
	md->saveConfig(fs);
	fs << "}";
}

void	dcLearn::fnread_modelInfo(FileNode fn){

	select_model(fn["model_name"]);
	md->loadConfig(fn["model_config"]);

}



void	dcLearn::fnread_labelName(FileNode fn){
	printf("load labels [");
	for(FileNodeIterator i = fn.begin(); i != fn.end(); ++i){
		string s = (string) *i;
		lbl_names.push_back(s);
		printf("%s ",s.c_str());
	}
	printf("]\n");
}

void 	dcLearn::select_model(string model_name){
	md_name = model_name;
	if( model_name == "flat_vector" ) 	md = new flat();
};




}
