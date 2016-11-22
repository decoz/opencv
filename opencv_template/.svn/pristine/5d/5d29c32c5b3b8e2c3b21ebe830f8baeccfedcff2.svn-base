/*
 * dcMonitor.cpp
 *
 *  Created on: 2015. 2. 25.
 *      Author: decoz
 */

#include "dcMonitor.h"
#include <cv_util/cv_util.h>

void mouse_event(int event, int x, int y,  int flags, void* param){


	dcMonitor *mon  = dcMonitor::GetInstance();


	switch(event){
	case 1:
		//printf("<mouse event:%d,%d>\n",x,y);
		mon->onmouse_lclick(x,y);
		fflush(stdout);
		break;

	}
}

void dcMonitor::onmouse_lclick(int x, int y) {


	main.setTo(0);
	Mat src = root->img;
	src.copyTo( main( Rect(0, 0, src.cols, src.rows ) ) );

	MonObj *cobj = root;
	bool update = true;

	for(int ci=0;ci<=cd;ci++){
		cobj = getCur(ci);

		for(int i=0;i<cobj->subObj.size();i++ ){

			//>>>>중첩된 영역은 리볼버형식으로 돌어가며 선택되도록 한다.
				int ri = i;
				if(ci < cd && cursor[ci+1] != -1) {
					ri =  ( cursor[ci+1] + i ) % cobj->subObj.size();
				}
			//<<<<<

			MonObj *sub = cobj->subObj[ri];
			Rect s_r = divlist[sub->sname];

			if(sub->r.x + s_r.x < x
					&& s_r.y + sub->r.y < y
					&& s_r.x + sub->r.x + sub->r.width > x
					&& s_r.y + sub->r.y + sub->r.height> y
			) {
				Rect r = divlist[sub->tname];

				cd = ci + 1;
				cursor[cd] = ri;

				//rectangle(main,sub->r,Scalar(0,0,255));

			}
		}

	}

	updateShow();
	//imshow("monitor",main);


}


Rect dcMonitor::addDivH(String divname, Size sz, int marginx, int marginy ){
/* 현재 모니터에서 우측에 영역을 추가함 */

	//Point sp = Point(current_sz.width, current_sz.height);

	cx = current_sz.width;

	current_sz.width  += sz.width + 2*marginx;
	if(sz.height  +  2*marginy > current_sz.height)
		current_sz.height = sz.height + 2 * marginy;

	divlist[divname] =  Rect(cx+marginx,cy+marginy, sz.width, sz.height);


}

void dcMonitor::monshow(){
/*
 * 화면에 모니터링 윈도우를 뛰움
 * 모니터링 윈도우의 이름은 monitor 설정되며
 * 이벤트 핸들러를 다시 한번 리프레쉬함
 *
 */
	updateMat();
	MonObj *cobj = root;
	Rect r = divlist[ cobj->tname ];

	cobj->img.copyTo( main(r));


	for(int i=1;i<=cd;i++){
		int n = cursor[i];
		cobj = cobj->subObj[n];
		Rect r = divlist[ cobj->tname ];

		printf("%s <%d,%d>",cobj->tname.c_str(), cobj->img.cols,cobj->img.rows);
		printf("[%d,%d,%d,%d]",r.x,r.y,r.width, r.height);
		//imshow("test", cobj->img);

		Mat cimg = cobj->img;
		if( cobj->img.cols < r.width && cobj->img.rows < r.height )
			cobj->img.copyTo(main(Rect(r.x,r.y,cimg.cols,cimg.rows)  ));

	}
	imshow("monitor", main);
	setMouseCallback("monitor",mouse_event,NULL);

}

void dcMonitor::updateMat(){
// 모니터링 영역 추가등의 변화에 화면 크기를 재정의

	Size sz = main.size();
	if(sz.width < current_sz.width || sz.height < current_sz.height ){
		Mat nmap(current_sz, CV_8UC3);
		main = nmap;

	}
}


void dcMonitor::updateShow(){
// 모니터링 화면 표시 업데이트


	main.setTo(0);
	Mat src = root->img;
	src.copyTo( main( Rect(0, 0, src.cols, src.rows ) ) );

	for(int ci=1;ci<=cd;ci++){
		MonObj *cobj  = getCur(ci);
		Rect r = divlist[cobj->tname];

		if(cobj->sname != "")  {
			Rect sr = divlist[cobj->sname];
			Rect  rr = cobj->r;
			rr.x += sr.x;
			rr.y += sr.y;

			rectangle(main,rr,Scalar(0,0,255));
		}

		cobj->draw(main(r));
	}

	imshow("monitor",main);

}

MonObj* dcMonitor::getCur(int d){

	//base cd 는 0
	MonObj *cobj =  root;

	for(int i=1;i<=d;i++){
		int n = cursor[i];
		cobj = cobj->subObj[n];
	}

	return cobj;
}

void dcMonitor::obj_info(String msg){

	MonObj *cobj = cur();
	cobj->msg = cobj->msg + msg;


}


void dcMonitor::addSub(String _tname, Rect r){
/*
 * 이벤트 추가
 * rname:  이벤트 영역명
 *
 */


	//printf("root", dcmon->)
	MonObj *pobj = getCur(cd);
	MonObj *cobj = new MonObj(pobj->tname, r );
	cobj->tname = _tname;

	pobj->subObj.push_back(cobj);

	cd++;
	cursor[cd] = pobj->subObj.size() - 1;

}

void dcMonitor::addNext(Rect r){


	if( cd < 1 )  return ;
	MonObj *pobj = getCur(cd-1), *cobj = getCur(cd);



	MonObj *nobj = new MonObj(pobj->tname, r );
	nobj->tname = pobj->subObj[0]->tname;


	pobj->subObj.push_back(nobj);
	cursor[cd] = pobj->subObj.size() - 1;


}

void dcMonitor::goUp(){

	cursor[cd] = -1;
	cd--;

}

void dcMonitor::goDown(int n){
/*
 * child 노드를 선택한다.. i값이 -1 이면 마지막 노드
 */
	MonObj *m = cur();
	if(n == -1) n = m->subObj.size() - 1;

	cd++;
	cursor[cd] = n;

}

void dcMonitor::init(String mname , Mat  img){

	base_sz = img.size();
	current_sz = base_sz;
	cx = cy = 0;


	for(int i=0;i<10;i++) cursor[i] = 0;
	cd = 0;

	if(img.channels() < 3) cvtColor(img,img,CV_GRAY2BGR);

	divlist.clear();
	root = new MonObj("",Rect(0,0,0,0));
	root->img = img;
	root->tname = mname;

	Size sz = img.size();
	divlist[mname] = Rect(0,0,sz.width,sz.height);
}



dcMonitor *dcMonitor::instance;

dcMonitor::~dcMonitor() {



	// TODO Auto-generated destructor stub
}




void MonObj::draw(Mat target){
/*
 *  target Rect  에 이미지를 출력
 *  target Rect  보다 영역이 작거나 클 경우 이에 맞춰서 출력
 *
 */
	Size sz = img.size();
	Rect r( 0,0, sz.width,sz.height);

	if(sz.width  > 0 && sz.height  > 0) img(r).copyTo( target(r) );

	int left_h = target.rows - sz.height;
	r = Rect(0,sz.height, target.cols, left_h );

	Mat  timg = target(r);

	//msg = "test test \n test test";
	//CvFont font = fontQt("Times");

	if(msg != "") putMultiLine( timg, msg , Point(10,40),  CV_FONT_HERSHEY_COMPLEX , 0.4 , Scalar(255,255,255), 20 );

}


