#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdarg.h>
#include "dcLearn.h"
#include "classifier.h"
#include <dcFileLoader.h>


using namespace cv;
using namespace cv::ml;
using namespace std;

vector <float>  vec32f(int n,...){
/*
 * n 을 통해 n개의 인자를 vector 로 등록한다.
 */


	vector <float> fv;
	va_list vl;
	va_start( vl, n );
	for(int i=0;i<n;i++){
		//fv.push_back( va_arg(vl, float) );
		float v = (float) va_arg(vl, int);
		fv.push_back( v );
	}
	va_end(vl);
	return fv;
}

void test_knn_classifier();
void test_svm_classifier();
void test_svm();

void init_learn(learn::dcLearn *p_ln){
/*
 * learning 초기화
 * model 은 flat , classifier 는 svm으로 ( 그것밖에 없지만.. )
 * 학습기보단 cases 를 중심으로  관리하기 위해 cases를 읽어
 * 매번 학습기를 생성
 *
 */

	p_ln->load_classifier("num_flat_svm_t.xml");

}

void test_learn(learn::dcLearn *p_ln, Mat img){


	imshow("img", img);

	string predict = p_ln->predict_img(img);
	printf("predict result: %s\n" , predict.c_str());

	fflush(stdout);

}


int main ( int argc, char **argv ){
	//test_knn_classifier();

	printf("test svm learning class\n");

	dcFileLoader fl;

	learn::dcLearn *p_ln = new learn::dcLearn();
	init_learn(p_ln);
	char *fname;

	if(argc < 2) {
		printf("usage: test_learn [image filename]+ \n");
		return 0;
	}

	for(int i=1;i<argc;i++) 	fl.load(argv[i]);
	while( (fname = fl.next()) != NULL){

		Mat img =  imread(fname);
		if(img.empty()) {	printf("fail to load %s \n",fname);	continue; }
		else  printf("open %s\n",fname);

		test_learn(p_ln, img );

		if( waitKey(0) == 'q') break;

	}


	return 0;
}


void test_svm_classifier(){

	learn::clsf_svm svm;

	int labels[4] = { 1, -1, -1, -1 };
	float trainingData[4][2] = { { 501, 10 }, { 255, 10 }, { 501, 255 }, { 10, 501 } };

	float t_data[2] = {200,50};

	Mat fmat(4, 2, CV_32FC1, trainingData);
	Mat imat(4, 1, CV_32SC1, labels);

	svm.train(fmat, imat);

	Mat t_mat(1, 2, CV_32FC1, t_data);
	printf("test predict: %d\n", svm.predict(t_mat) );

}



void test_knn_classifier(){

	learn::clsf_knn knn;

	int labels[4] = { 1, 2, 3, -1 };
	float trainingData[4][2] = { { 501, 10 }, { 255, 10 }, { 501, 255 }, { 10, 501 } };

	float t_data[2] = {501,255};

	Mat fmat(4, 2, CV_32FC1, trainingData);
	Mat imat(4, 1, CV_32SC1, labels);

	knn.train(fmat, imat);

	Mat t_mat(1, 2, CV_32FC1, t_data);
	printf("test predict: %d\n", knn.predict(t_mat) );

}




void test_svm(){
	int width = 512, height = 512;
    Mat image = Mat::zeros(height, width, CV_8UC3);
	int labels[4] = { 1, -1, -1, -1 };
	float trainingData[4][2] = { { 501, 10 }, { 255, 10 }, { 501, 255 }, { 10,
			501 } };
	Mat trainingDataMat(4, 2, CV_32FC1, trainingData);
	Mat labelsMat(4, 1, CV_32SC1, labels);

	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);

	for(int i=0; i<4; i++){
		for(int j=0; j<2; j++) printf("%.2f ", trainingDataMat.at<float>(i,j) );
		printf( " -> %.2f \n", labelsMat.at<float>(i,0));
	}


	Mat value_mat = (Mat_<float>(1, 2) << 150, 100);
	float r = svm->predict( value_mat);
	printf("test predict: %.2f\n",  r ); fflush(stdout);

	Vec3b green(0, 255, 0), blue(255, 0, 0);
	for (int i = 0; i < image.rows; ++i)
		for (int j = 0; j < image.cols; ++j) {
			Mat sampleMat = (Mat_<float>(1, 2) << j, i);
			float response = svm->predict(sampleMat);
			if (response == 1)
				image.at<Vec3b>(i, j) = green;
			else if (response == -1)
				image.at<Vec3b>(i, j) = blue;
		}

	// Show the training data
	int thickness = -1;
	int lineType = 8;
	circle(image, Point(501, 10), 5, Scalar(0, 0, 0), thickness, lineType);
	circle(image, Point(255, 10), 5, Scalar(255, 255, 255), thickness,
			lineType);
	circle(image, Point(501, 255), 5, Scalar(255, 255, 255), thickness,
			lineType);
	circle(image, Point(10, 501), 5, Scalar(255, 255, 255), thickness,
			lineType);
	// Show support vectors
	thickness = 2;
	lineType = 8;
	Mat sv = svm->getUncompressedSupportVectors();
	for (int i = 0; i < sv.rows; ++i) {
		const float* v = sv.ptr<float>(i);
		circle(image, Point((int) v[0], (int) v[1]), 6, Scalar(128, 128, 128),
				thickness, lineType);
	}
	imwrite("result.png", image);        // save the image
	imshow("SVM Simple Example", image); // show it to the user
	waitKey(0);
}


