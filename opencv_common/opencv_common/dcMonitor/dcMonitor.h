/*
 * dcMonitor.h
 *
 *  Created on: 2015. 2. 25.
 *      Author: decoz
 *
 *  OpenCV ���� ����͸��� â�� �����ϴ� Ŭ����
 *
 *  - ���� ��� ���� ���� ����
 *  - ���� - Object ��ũ �� ���콺 �̺�Ʈ ���� ����
 *  - �ؽ�Ʈ ��� , �ؽ�Ʈ ��� ���� ���� ����
 *
 *
 */

#ifndef DCMONITOR_H_
#define DCMONITOR_H_
#include <map>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

class MonObj {
/*
 * �̺�Ʈ Ʈ�������� Ŭ����
 *
 * sname 	�Ҽ� ���� : �̺�Ʈ �߻� �����̸�
 * r			�̺�Ʈ ����: �̺�Ʈ �߻� ����
 * tname		Ÿ�� ���� : �̺�Ʈ �߻��� drawing �����̸�
 * img			Ÿ�� �̹��� : �̺�Ʈ �߻��� Ÿ�ٱ����� ����� �̹���
  *
 */

public:
	MonObj(){ }
	MonObj(String _name, Rect _r) { sname = _name, r = _r; msg = "";}
	String sname,tname;
	Mat img;
	String msg;
	Rect r;
<<<<<<< HEAD
	vector <MonObj*> subObj;
=======
	Vector <MonObj*> subObj;
>>>>>>> branch 'master' of https://github.com/decoz/opencv


	/*
	void addChild(MonObj *data); // child ���� �߰�

	//void setImg(img);
	//void drawImg(img);

	void setFont( int fontsize, Scalar color);
	void print(String s);
	*/
	void draw(Mat target); // object �̹����� ���


};


class dcMonitor {
private:

	map <String, Rect> divlist;
	Mat main;
	Size base_sz, current_sz;
	int cx,cy;
	void  updateMat(); 	// size  �� ���� Mat�� ������Ʈ

	dcMonitor(){}; //  singleton ����
	dcMonitor(const dcMonitor& other);

	~dcMonitor();

	static dcMonitor* instance;

public:

	MonObj *root;
	int cd, cursor[10]; // depth 10 ���� �� ���� ǥ��

   static dcMonitor* GetInstance()
	{
		if(instance == NULL) instance = new dcMonitor();
		return instance;
	}
	void init(String mname , Mat  img);				// �ʱ�ȭ


/* ����͸� ���� �߰� �޼ҵ� */

	Rect addDivH(String divname, Size sz, int marginx = 0,int marginy = 0);	// ���� ���� �߰�
	Rect addDivV(String divname, Size sz);  // �Ʒ��� ���� �߰�


/* �̺�Ʈ ������ Append �� ��ͱ����� Ŀ�� ������� �̷������. */

	void addSub(String _tname, Rect r); // �� ������ ���꿵���� �����ϰ� ���������� �̵�
	void addNext(Rect r); // ���� �������� ���� ��� ���
	void goUp(); // ���� ������
	void goDown(int n); // n ��° ����������


	MonObj* getCur(int d); //  ���� �������� obj ����
	MonObj* cur() { return getCur(cd); }

	void setImg(String tname, Mat img); // �̺�Ʈ �ߵ��� ǥ���� Ÿ�� ������ �̹����� ����
	void monshow();
	void updateShow();

	void onmouse_lclick(int x, int y); // monitor  â�� mouse event ��

	void obj_info(String msg); // ���� ������Ʈ�� ���ڿ� ���


private:


};



#endif /* DCMONITOR_H_ */
