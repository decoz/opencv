/*
 * Hypoth.h
 *
 *  Created on: 2015. 7. 24.
 *      Author: decoz
 */

#ifndef DCDESAC_HYPOTH_H_
#define DCDESAC_HYPOTH_H_
#include <opencv2/opencv.hpp>

using namespace cv;
class Hypoth {
public:

	Hypoth();
	virtual ~Hypoth();
	virtual  bool match(Point p);


};

#endif /* DCDESAC_HYPOTH_H_ */
