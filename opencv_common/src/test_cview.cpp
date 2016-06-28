#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "dsCView/dsCView.h"

using namespace cv;

int main ( int argc, char **argv ){


	Mat img = imread("images/01.png");
	cview::show("test",img);
	//imshow("test2", cview::imgs["test"]);

	waitKey(0);
	return 0;

}


