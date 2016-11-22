/*
 * dcMonitor.h
 *
 *  Created on: 2015. 2. 25.
 *      Author: decoz
 *
 *  OpenCV 에서 모니터링용 창을 생성하는 클래스
 *
 *  - 여러 출력 영역 설정 가능
 *  - 영역 - Object 링크 후 마우스 이벤트 연결 가능
 *  - 텍스트 출력 , 텍스트 출력 형식 설정 가능
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
 * 이벤트 트리구조용 클래스
 *
 * sname 	소속 구역 : 이벤트 발생 구역이름
 * r			이벤트 영역: 이벤트 발생 영역
 * tname		타겟 구역 : 이벤트 발생시 drawing 구역이름
 * img			타겟 이미지 : 이벤트 발생시 타겟구역에 출력할 이미지
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
	void addChild(MonObj *data); // child 영역 추가

	//void setImg(img);
	//void drawImg(img);

	void setFont( int fontsize, Scalar color);
	void print(String s);
	*/
	void draw(Mat target); // object 이미지를 출력


};


class dcMonitor {
private:

	map <String, Rect> divlist;
	Mat main;
	Size base_sz, current_sz;
	int cx,cy;
	void  updateMat(); 	// size  에 맞춰 Mat를 업데이트

	dcMonitor(){}; //  singleton 으로
	dcMonitor(const dcMonitor& other);

	~dcMonitor();

	static dcMonitor* instance;

public:

	MonObj *root;
	int cd, cursor[10]; // depth 10 까지 현 영역 표시

   static dcMonitor* GetInstance()
	{
		if(instance == NULL) instance = new dcMonitor();
		return instance;
	}
	void init(String mname , Mat  img);				// 초기화


/* 모니터링 영역 추가 메소드 */

	Rect addDivH(String divname, Size sz, int marginx = 0,int marginy = 0);	// 옆에 영역 추가
	Rect addDivV(String divname, Size sz);  // 아래에 영역 추가


/* 이벤트 영역의 Append 는 재귀구조의 커서 방식으로 이루어진다. */

	void addSub(String _tname, Rect r); // 현 영역의 서브영역을 생성하고 하위레벨로 이동
	void addNext(Rect r); // 현재 레벨에서 영역 계속 등록
	void goUp(); // 상위 레벨로
	void goDown(int n); // n 번째 하위레벨로


	MonObj* getCur(int d); //  현재 포지션의 obj 리턴
	MonObj* cur() { return getCur(cd); }

	void setImg(String tname, Mat img); // 이벤트 발동시 표시할 타겟 영역과 이미지를 설정
	void monshow();
	void updateShow();

	void onmouse_lclick(int x, int y); // monitor  창의 mouse event ㅊ

	void obj_info(String msg); // 현재 오브젝트에 문자열 출력


private:


};



#endif /* DCMONITOR_H_ */
