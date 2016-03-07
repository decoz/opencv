#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdarg.h>
#include "dcLearn/dcLearn.h"


using namespace cv;

vector <float>  vec32f(int n,...){
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


int main ( int argc, char **argv ){

	dcLearn ln("data.txt", "classifier.xml");

	ln.learn( vec32f(3,15,2,30) , 1 );
	ln.learn( vec32f(3,10,11,10) , 1 );
	ln.learn( vec32f(3,25,15,20) , 0 );
	ln.learn( vec32f(3,4,21,15) , 0 );


	ln.build();

	printf("predict 10, 15, 22 : %.1f", ln.predict( vec32f(3,14,5,27)));


	waitKey(0);


}
