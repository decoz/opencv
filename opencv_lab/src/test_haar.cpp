#include <opencv2/opencv.hpp>
#include <dcHaar/dcHaar.h>
using namespace cv;
int main ( int argc, char **argv ){

	Mat img = imread("01.png");
	imshow("test",img);
	waitKey(0);


}
