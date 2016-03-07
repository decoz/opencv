/*
 * dcLearn.h
 *
 *  Created on: 2016. 1. 14.
 *      Author: decoz
 *
 *   ������ ������ �����ϱ� ���� Ŭ����
 *   ������Ÿ��
 *
 *   �ʼ�
 *   - ������ - Ŭ���� ������ �Է�
 *   - �з��� ����
 *   - �Ǻ�
 *
 *  �ɼ�
 *  - ���� �������� �ѹ��� ���� ����
 *  - �з��� ����/ �ε� ���
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

	Mat last_predict; // �ֱ� ���� ������
	char  *data_file, *classifier_file;

	vector < vector <float> > data;		// trainning data
	vector < int > target;					// data target class

	int l; 										// value dimension (decided when building)

	void initSvm();

public:
	//dcLearn();
	dcLearn(char *_data_file, char *_classifier_file);	// �н��� ������ ����
													// ������ ���� ��� �о�� ������ �������·�

	bool learn(vector <float> val, int target ); 		// training data �� �Է¹��� (���� �н����� ����)

	void save_data();						// training data �� ����
	void load_data();						// training data �� ����
	void save_classifier();					// �Ǻ��� ����
	void load_classifier();					// �Ǻ��� �ε�

	void	build(); 							// classifier �� ����
	float	predict(vector <float> val);				// ���� ����� ����
	void	revise_predict(int);				// ���� ����� ����

	virtual ~dcLearn();
};



#endif /* DCLEARN_DCLEARN_H_ */
