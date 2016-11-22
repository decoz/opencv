<<<<<<< HEAD


=======
>>>>>>> branch 'master' of https://github.com/decoz/opencv
#include <opencv2/opencv.hpp>
using namespace cv;
int main ( int argc, char **argv ){

	Mat img = imread("01.png");
	imshow("test",img);
	waitKey(0);
	return 0;

}
