/*
 * dcLearn.h
 *
 *  Created on: 2016. 1. 14.
 *      Author: decoz
 *
 *   간단한 러닝을 수행하기 위한 클래스
 *   프로토타입
 *
 *   필수
 *   - 데이터 - 클래스 조합을 입력
 *   - 분류기 생성
 *   - 판별
 *
 *  옵션
 *  - 파일 포맷으로 한번에 리딩 가능
 *  - 분류기 저장/ 로딩 기능
 *
 */

#ifndef DCLEARN_DCLEARN_H_
#define DCLEARN_DCLEARN_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>


using namespace cv;
class dcLearn {
private:
	CvSVM svm;
	CvSVMParams svm_param;

	Mat last_predict; // 최근 예측 데이터
	char  *data_file, *classifier_file;

	vector < vector <float> > data;		// trainning data
	vector < int > target;					// data target class

	int l; 										// value dimension (decided when building)

	void initSvm();

public:
	//dcLearn();
	dcLearn(char *_data_file, char *_classifier_file);	// 학습기 파일을 설정
													// 파일이 있을 경우 읽어옴 없으면 백지상태로

	bool learn(vector <float> val, int target ); 		// training data 를 입력받음 (아직 학습하지 안음)

	void save_data();						// training data 를 저장
	void load_data();						// training data 를 읽음
	void save_classifier();					// 판별기 저장
	void load_classifier();					// 판별기 로딩

	void	build(); 							// classifier 를 생성
	float	predict(vector <float> val);				// 에측 결과를 리턴
	void	revise_predict(int);				// 예측 결과를 수정

	virtual ~dcLearn();
};



#endif /* DCLEARN_DCLEARN_H_ */
