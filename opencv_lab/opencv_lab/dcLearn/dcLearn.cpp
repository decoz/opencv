/*
 * dcLearn.cpp
 *
 *  Created on: 2016. 1. 14.
 *      Author: decoz
 */

#include "dcLearn.h"
#include <stdio.h>

using namespace cv;
/*
dcLearn::dcLearn() {
	// TODO Auto-generated constructor stub

}
*/

dcLearn::~dcLearn() {
	// TODO Auto-generated destructor stub
}

dcLearn::dcLearn(char *_data_file, char *_classifier_file){

	initSvm();

	if(_data_file) data_file = _data_file;
	else data_file = "data.txt";

	if(_classifier_file) classifier_file = _classifier_file;
	else classifier_file = "classifier.xml";
}


bool dcLearn::learn(vector <float> val, int t_val){

	data.push_back(val);
	target.push_back(t_val);

}

void dcLearn::initSvm(){

	svm_param.svm_type = CvSVM::C_SVC;
	svm_param.kernel_type = CvSVM::LINEAR;
	svm_param.term_crit =  cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

}

void dcLearn::build(){

	l = 0;
	int nl = 0, s = data.size();

	for(int i=0;i<s;i++) // max length °è»ê
		if( (nl =  data[i].size()) > l ) l = nl;

	Mat data_mat(s, l, CV_32FC1 );
	for(int i=0; i<s; i++)
		for(int j=0; j<data[i].size(); j++)
			data_mat.at <float> (i,j) = data[i][j];

	Mat label_mat(s, 1, CV_32FC1 );
	for(int i=0;i<target.size();i++)
		label_mat.at <float>(i,0) = target[i];

	svm.train( data_mat, label_mat, Mat(), Mat(), svm_param );

}

float dcLearn::predict(vector <float> val){

	Mat value_mat(1,l,CV_32F);
	for(int i=0;i<val.size();i++) value_mat.at <float> (0, i) = val[i];

	return svm.predict( value_mat );
}


