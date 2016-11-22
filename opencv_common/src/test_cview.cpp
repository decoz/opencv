#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "dsCView/dsCView.h"

using namespace cv;

int main ( int argc, char **argv ){


<<<<<<< HEAD
	Mat img = imread("01.png");
=======
	Mat img = imread("images/01.png");
>>>>>>> branch 'master' of https://github.com/decoz/opencv
	cview::show("test",img);
	//imshow("test2", cview::imgs["test"]);

	waitKey(0);
	return 0;

}


