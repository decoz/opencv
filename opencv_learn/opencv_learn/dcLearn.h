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
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include "model.h"
#include "classifier.h"

using namespace cv;
using namespace cv::ml;
using namespace std;

namespace learn {

class dcLearn {
private:

	classifier *clsf;
	model  	*md;

	string md_name;
	string clsf_name;

	Mat	 last_predict; // 최근 예측 데이터
	char  *case_file, *classifier_file;

	Mat	cases;			// trainning data
	Mat	labels;			// data target class

	int l; 									// value dimension (decided when building)

	vector <string> lbl_names;		// 레이블 : 클래스이름 ( 이름이 없는 레이블은 인덱스 번호로 출력 )

	int 	get_label(string lbl_name, bool pushnew = true);

	void	fswrite_labelName(FileStorage &fs);
	void	fswrite_modelInfo(FileStorage &fs);
	void	fnread_labelName(FileNode fn);
	void	fnread_modelInfo(FileNode fn);




public:
	//dcLearn();
	dcLearn();	// 학습기 파일을 설정
				// 파일이 있을 경우 읽어옴 없으면 백지상태로


	bool feed_back(Mat f_mat, int target ); 		// training data 를 입력받음 (아직 학습하지 안음)

	void 	save_cases(char *fname);					// training data 를 저장
	void 	load_cases(char *fname);					// training data 를 읽음
	void 	save_classifier(char *fname);				// 판별기 저장
	void 	load_classifier(char *fname);				// 판별기 로딩

	void 	build_classifier();
	void 	case_img(Mat img, string lbl_name);

	void 	select_model(string model_name);
	void 	select_model(model *_md) 		{ md = _md; };
	void 	select_classifier(classifier *_clsf)	{ clsf = _clsf; };

	string	predict_img(Mat img);	// 에측 결과를 리턴


	virtual ~dcLearn();
};

}

#endif /* DCLEARN_DCLEARN_H_ */
