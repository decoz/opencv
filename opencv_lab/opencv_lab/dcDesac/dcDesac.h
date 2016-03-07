/*
 * dcDesac.h
 *
 *  Created on: 2015. 7. 24.
 *      Author: decoz
 */

#ifndef DCDESAC_DCDESAC_H_
#define DCDESAC_DCDESAC_H_
#include <opencv2/opencv.hpp>

using namespace cv;

class dcDesac {

private:
	Mat img;


public:
	dcDesac();
	virtual ~dcDesac();

	void loadImg(Mat img);
	void x();

};

#endif /* DCDESAC_DCDESAC_H_ */
