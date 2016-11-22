/*
 *  case xml ���� �κ��� classifier �� �����ϰ� �����ϴ� ��ƿ
 *
 *  ����:
 *
 *  mkcases  [case file]  -o [classifier file]

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
	ln.select_classifier( new learn::clsf_svm);
	//ln.select_model( new learn::flat(20,30));

	char *clsf_fname = NULL, *fname;

	if(argc < 2) {
		printf("error argument [%d]\n usage : mkclsf [case file]... -o [clsf file] \n", argc);
		return 0;
	}

	for(int i=1;i<argc;i++){

		if( !strcmp( argv[i], "-o" )  ){
			if( i+1 < argc ) 	clsf_fname = argv[i+1];	 // file option �ΰ�� ���� ���ڸ� file������ �ν�
			printf("-- output classifier file : %s\n", clsf_fname);
			fflush(stdout);
			i++;
			continue;
		}

		fl.load((argv[i]));				// case file �� �д´�.
		printf("%s\n",argv[i]);

		while( ( fname = fl.next() ) != NULL ) ln.load_cases(fname);
	}

	ln.build_classifier();

	if(clsf_fname != NULL) {
		printf("save classifier  to %s\n", clsf_fname);
		ln.save_classifier(clsf_fname) ;
	}

	return 0;
}



