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

			//>>>>��ø�� ������ �������������� ����� ���õǵ��� �Ѵ�.
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
/* ���� ����Ϳ��� ������ ������ �߰��� */

	//Point sp = Point(current_sz.width, current_sz.height);

	cx = current_sz.width;

	current_sz.width  += sz.width + 2*marginx;
	if(sz.height  +  2*marginy > current_sz.height)
		current_sz.height = sz.height + 2 * marginy;

	divlist[divname] =  Rect(cx+marginx,cy+marginy, sz.width, sz.height);


}

void dcMonitor::monshow(){
/*
 * ȭ�鿡 ����͸� �����츦 �ٿ�
 * ����͸� �������� �̸��� monitor �����Ǹ�
 * �̺�Ʈ �ڵ鷯�� �ٽ� �ѹ� ����������
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
// ����͸� ���� �߰����� ��ȭ�� ȭ�� ũ�⸦ ������

	Size sz = main.size();
	if(sz.width < current_sz.width || sz.height < current_sz.height ){
		Mat nmap(current_sz, CV_8UC3);
		main = nmap;

	}
}


void dcMonitor::updateShow(){
// ����͸� ȭ�� ǥ�� ������Ʈ


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

	//base cd �� 0
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
 * �̺�Ʈ �߰�
 * rname:  �̺�Ʈ ������
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
 * child ��带 �����Ѵ�.. i���� -1 �̸� ������ ���
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
 *  target Rect  �� �̹����� ���
 *  target Rect  ���� ������ �۰ų� Ŭ ��� �̿� ���缭 ���
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


