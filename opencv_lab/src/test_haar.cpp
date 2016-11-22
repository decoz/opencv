#include <opencv2/opencv.hpp>
#include <dcHaar/dcHaar.h>
using namespace cv;
int main ( int argc, char **argv ){

<<<<<<< HEAD
	Mat img = imread(argv[1]);
=======
	Mat img = imread("01.png");
>>>>>>> branch 'master' of https://github.com/decoz/opencv
	imshow("test",img);
	waitKey(0);


}
