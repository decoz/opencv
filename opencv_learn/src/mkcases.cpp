/*
 *  case xml 파일 생성을 용이하게  하기 위한 유틸 프로그램
 *
 *  사용법:
 *
 *  mkcases  -d"1 2 3 4 5  -l?  -f case.xml -af *
 *  mkcases  -D 1 2 3 4 5 -L?
 *
 *	-a	: append
 *
 *
 */

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdarg.h>
#include "dcLearn.h"
#include "classifier.h"
#include "dcFileLoader.h"

using namespace cv;
using namespace cv::ml;
using namespace std;


int main(int argc, char **argv) {

	dcFileLoader fl;

	learn::dcLearn ln;
	//ln.select_classifier( new learn::clsf_svm);
	ln.select_model(new learn::flat(20,30));

	char *sav_fname = NULL, *dname, *fname, *label;


	if(argc < 2) {
		printf("error argument [%d]\n usage : expr [learn case path] / [test case path] \n", argc);
		return 0;
	}


	bool 	isTest  =  false, show_table = true;
	int 	autotest_cnt = 0, min_dist = 10, iteration = 1;

	for(int i=1;i<argc;i++){

		if( !strcmp( argv[i], "-o" )  ){
			if( i+1 < argc ) 	sav_fname = argv[i+1];	 // file option 인경우 다음 인자를 file명으로 인식
			printf("save_file : %s\n", sav_fname); fflush(stdout);
			i++;
			continue;
		}

		fl.loadDir(argv[i]);				// directory list 를 읽는다.

		printf("%s\n",argv[i]);

		while( ( dname = fl.next() ) != NULL )  {
			char *label = fl.getName();

			char s_path[200];
			sprintf(s_path, "%s/*", argv[i]);

			fl.load(s_path);
			char* fname;

			while(( fname = fl.next() ) != NULL ) {
				printf("%s: %s\n", label,  fname);
				Mat img = imread(fname);
				if(img.empty()) {
					printf("fail to load %s \n",fname);
					continue;
				}
				ln.case_img(img, string(label));
			}
			fflush(stdout);
		}
	}

	if(sav_fname != NULL) {
		printf("save cases to %s\n", sav_fname);
		ln.save_cases(sav_fname);
	}


	return 0;
}



