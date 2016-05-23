/*
 * dcPreprocess.cpp
 *
 *  Created on: 2015. 8. 25.
 *      Author: cmonyo
 */

#include "dcPreprocess.h"


Mat dcPreprocess::preprocess(Mat img){ 	// 전처리 실행
/*
 * 전처리 실행 부분
 *
 */


	Mat gray;
	if(isflip) flip(img,img,1);

	if( img.channels() > 1 ) cvtColor(img, gray, CV_BGR2GRAY);
	else gray = img;

	 Mat kernel = (Mat_<float>(3,3) << 1,  1, 1, 1, -8, 1, 1,  1, 1); //filter2d를 위한 커널 셋팅

	 Mat sharp = gray;
	 Mat matBlurred,matThresh,matGrayscale,dst,imgLaplacian, result;

	 filter2D(sharp, imgLaplacian, CV_32F, kernel);
	 gray.convertTo(sharp, CV_32F);
	 Mat imgResult = sharp - imgLaplacian;


	 imgResult.convertTo(imgResult, CV_8UC3);
	 imgLaplacian.convertTo(imgLaplacian, CV_8UC3);


	 GaussianBlur(imgResult,matBlurred,Size(5, 5),0);
	 medianBlur(matBlurred, dst,3);
	 result = WolfJolion (dst,  winx, winy, optK);


	 threshold(result, matThresh, 80, 255, CV_THRESH_BINARY | CV_THRESH_BINARY_INV);

	 return matThresh;


};



double dcPreprocess::calcLocalStats (Mat &im, Mat &map_m, Mat &map_s, int winx, int winy) {//wolf를 계산하기위한 클래스
    Mat im_sum, im_sum_sq;
    integral(im,im_sum,im_sum_sq,CV_64F);

	double m,s,max_s,sum,sum_sq;
	int wxh	= winx/2;
	int wyh	= winy/2;
	int x_firstth= wxh;
	int y_lastth = im.rows-wyh-1;
	int y_firstth= wyh;
	double winarea = winx*winy;

	max_s = 0;
	for	(int j = y_firstth ; j<=y_lastth; j++){
		sum = sum_sq = 0;

        sum = im_sum.at<double>(j-wyh+winy,winx) - im_sum.at<double>(j-wyh,winx) - im_sum.at<double>(j-wyh+winy,0) + im_sum.at<double>(j-wyh,0);
        sum_sq = im_sum_sq.at<double>(j-wyh+winy,winx) - im_sum_sq.at<double>(j-wyh,winx) - im_sum_sq.at<double>(j-wyh+winy,0) + im_sum_sq.at<double>(j-wyh,0);

		m  = sum / winarea;
		s  = sqrt ((sum_sq - m*sum)/winarea);
		if (s > max_s) max_s = s;

		map_m.fset(x_firstth, j, m);
		map_s.fset(x_firstth, j, s);

		// Shift the window, add and remove	new/old values to the histogram
		for	(int i=1 ; i <= im.cols-winx; i++) {

			// Remove the left old column and add the right new column
			sum -= im_sum.at<double>(j-wyh+winy,i) - im_sum.at<double>(j-wyh,i) - im_sum.at<double>(j-wyh+winy,i-1) + im_sum.at<double>(j-wyh,i-1);
			sum += im_sum.at<double>(j-wyh+winy,i+winx) - im_sum.at<double>(j-wyh,i+winx) - im_sum.at<double>(j-wyh+winy,i+winx-1) + im_sum.at<double>(j-wyh,i+winx-1);

			sum_sq -= im_sum_sq.at<double>(j-wyh+winy,i) - im_sum_sq.at<double>(j-wyh,i) - im_sum_sq.at<double>(j-wyh+winy,i-1) + im_sum_sq.at<double>(j-wyh,i-1);
			sum_sq += im_sum_sq.at<double>(j-wyh+winy,i+winx) - im_sum_sq.at<double>(j-wyh,i+winx) - im_sum_sq.at<double>(j-wyh+winy,i+winx-1) + im_sum_sq.at<double>(j-wyh,i+winx-1);

			m  = sum / winarea;
			s  = sqrt ((sum_sq - m*sum)/winarea);
			if (s > max_s) max_s = s;

			map_m.fset(i+wxh, j, m);
			map_s.fset(i+wxh, j, s);
		}
	}

	return max_s;
}


Mat dcPreprocess::WolfJolion (Mat im,  int winx, int winy, double k){// wolf The binarization routine

	Mat output(im.rows, im.cols, CV_8UC1);

	double m, s, max_s;
	double th=0;
	double min_I, max_I;
	int wxh	= winx/2;
	int wyh	= winy/2;
	int x_firstth= wxh;
	int x_lastth = im.cols-wxh-1;
	int y_lastth = im.rows-wyh-1;
	int y_firstth= wyh;
	dcPreprocess bw;

	// Create local statistics and store them in a double matrices
	Mat map_m = Mat::zeros (im.rows, im.cols, CV_32F);
	Mat map_s = Mat::zeros (im.rows, im.cols, CV_32F);
	max_s = bw.calcLocalStats (im, map_m, map_s, winx, winy);

	minMaxLoc(im, &min_I, &max_I);

	Mat thsurf (im.rows, im.cols, CV_32F);

	// Create the threshold surface, including border processing
	// ----------------------------------------------------

	for(int j = y_firstth ; j<=y_lastth; j++) {

		// NORMAL, NON-BORDER AREA IN THE MIDDLE OF THE WINDOW:
		for	(int i=0 ; i <= im.cols-winx; i++) {
				m  = map_m.fget(i+wxh, j);
	    		s  = map_s.fget(i+wxh, j);

	    		// Calculate the threshold
	    		th = m + k * (s/max_s-1) * (m-min_I);

	    		thsurf.fset(i+wxh,j,th);

	    		if (i==0) {
	        		// LEFT BORDER
	        		for (int i=0; i<=x_firstth; ++i)
	                	thsurf.fset(i,j,th);

	        		// LEFT-UPPER CORNER
	        		if (j==y_firstth)
	        			for (int u=0; u<y_firstth; ++u)
	        			for (int i=0; i<=x_firstth; ++i)
	        				thsurf.fset(i,u,th);

	        		// LEFT-LOWER CORNER
	        		if (j==y_lastth)
	        			for (int u=y_lastth+1; u<im.rows; ++u)
	        			for (int i=0; i<=x_firstth; ++i)
	        				thsurf.fset(i,u,th);
	    		}

				// UPPER BORDER
				if (j==y_firstth)
					for (int u=0; u<y_firstth; ++u)
						thsurf.fset(i+wxh,u,th);

				// LOWER BORDER
				if (j==y_lastth)
					for (int u=y_lastth+1; u<im.rows; ++u)
						thsurf.fset(i+wxh,u,th);
			}

			// RIGHT BORDER
			for (int i=x_lastth; i<im.cols; ++i)
	        	thsurf.fset(i,j,th);

	  		// RIGHT-UPPER CORNER
			if (j==y_firstth)
				for (int u=0; u<y_firstth; ++u)
				for (int i=x_lastth; i<im.cols; ++i)
					thsurf.fset(i,u,th);

			// RIGHT-LOWER CORNER
			if (j==y_lastth)
				for (int u=y_lastth+1; u<im.rows; ++u)
				for (int i=x_lastth; i<im.cols; ++i)
					thsurf.fset(i,u,th);
		}

	for(int y=0; y<im.rows; ++y)
		for	(int x=0; x<im.cols; ++x)
		{
	    	if (im.uget(x,y) >= thsurf.fget(x,y))
	    	{
	    		output.uset(x,y,255);
	    	}
	    	else
	    	{
	    	    output.uset(x,y,0);
	    	}
	    }
	return output;
}

Mat dcPreprocess:: filtering(Mat  image, char ftype){
	//여기에 opencv에 제공하는 필터링 추가해보자
	//bilateralFilter에 관련된 변수들
	Mat result;

	int d =3;
	double sigmaColor =80;
	double sigmaSpace=80;
	int borderType=1;

	//filteer2D에 관련된 변수들
	Mat kernel(3, 3, CV_32F, Scalar(0)); // 커널 생성(모든 값을 0으로 초기화)
	kernel.at<float>(1, 1) = 5.0; // 커널 값에 할당
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(2, 1) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;

	if(ftype =='b'){
		bilateralFilter ( image, result, d, sigmaColor, sigmaSpace,borderType);
	}
	if(ftype=='f'){
		filter2D(image, result, image.depth(), kernel);
	}

	return result;

	//fastNlMeansDenoising(image,result,10.0,9,23);
	//GaussianBlur(image,result,Size(3,3),10,10);
}

Mat dcPreprocess::hisEqz(Mat src){
	vector<Mat> channels;
	Mat img_hist_equalized;

	cvtColor(src, img_hist_equalized, CV_BGR2YCrCb); //change the color image from BGR to YCrCb format

    split(img_hist_equalized,channels); //split the image into channels

    equalizeHist(channels[0], channels[0]); //equalize histogram on the 1st channel (Y)

    merge(channels,img_hist_equalized); //merge 3 channels including the modified 1st channel into one image

    cvtColor(img_hist_equalized, img_hist_equalized, CV_YCrCb2BGR); //change the color image from YCrCb to BGR format (to display image properly)

	return img_hist_equalized;
}

Mat dcPreprocess::hisEqzClache(Mat src){
	Mat lab_image;
	cvtColor(src, lab_image, CV_BGR2Lab);

	 // Extract the L channel
	vector<cv::Mat> lab_planes(3);
	split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

	// apply the CLAHE algorithm to the L channel
	Ptr<CLAHE> clahe = createCLAHE(2,Size(8,8));
	clahe->setClipLimit(4);
	Mat dst;
	clahe->apply(lab_planes[0], dst);

	// Merge the the color planes back into an Lab image
	dst.copyTo(lab_planes[0]);
	merge(lab_planes, lab_image);

   // convert back to RGB
   Mat image_clahe;
   Mat gray;
   cvtColor(lab_image, image_clahe, CV_Lab2BGR);
   //imshow("clahe",image_clahe);
   cvtColor(image_clahe,gray, CV_BGR2GRAY);

   return gray;
}



Mat dcPreprocess::sobelfilter(Mat src){
	Mat dst = src.clone();
	dcPreprocess g;
	int gx,gy,sumg;
	int xG[6];
	int yG[6];


	for(int y = 0; y < src.rows; y++)
		for(int x = 0; x < src.cols; x++)
			dst.at<uchar>(y,x) = 0.0;

	for(int y = 1; y < src.rows - 1; y++){
		for(int x = 1; x < src.cols - 1; x++){
			//gx = g.xGradient(src, x, y);
			//gy = g.yGradient(src, x, y);

			//sum =  sqrt(gx*gx + gy*gy);
			//sum = sum > 255 ? 255:sum;
			//sum = sum < 0 ? 0 : sum;
			//dst.at<uchar>(y,x) = sum;

			xG[0] = src.at<uchar>(y - 1 ,x - 1);
			xG[1] = src.at<uchar>(y, x - 1);
			xG[2] = src.at<uchar>(y + 1, x - 1);
			xG[3] = src.at<uchar>(y - 1, x);
			xG[4] = src.at<uchar>(y, x);
			xG[5] = src.at<uchar>(y + 1, x);
			xG[6] = src.at<uchar>(y - 1, x + 1);
			xG[7] = src.at<uchar>(y, x + 1);
			xG[8] = src.at<uchar>(y + 1, x + 1);


			yG[0] = src.at<uchar>(y - 1 ,x - 1);
			yG[1] = src.at<uchar>(y, x - 1);
			yG[2] = src.at<uchar>(y + 1, x - 1);
			yG[3] = src.at<uchar>(y - 1, x);
			yG[4] = src.at<uchar>(y, x);
			yG[5] = src.at<uchar>(y + 1, x);
			yG[6] = src.at<uchar>(y - 1, x + 1);
			yG[7] = src.at<uchar>(y, x + 1);
			yG[8] = src.at<uchar>(y + 1, x + 1);

			gx =  (xG[0]+(xG[1]+xG[1])+xG[2]-xG[6]-(xG[7]+xG[7])-xG[8]);
			gy =  (yG[2]+(yG[5]+yG[5])+yG[8]-xG[0]-(xG[3]+xG[3])-xG[6]);

			//xG[4] =  (xG[0]+(xG[1]+xG[1])+xG[2]-xG[6]-(xG[7]+xG[7])-xG[8]);
			//yG[4] =  (yG[2]+(yG[5]+yG[5])+yG[8]-xG[0]-(xG[3]+xG[3])-xG[6]);


			sumg=sqrt((gx*gx)+(gy*gy));
			//sumg=sqrt((xG[4]*xG[4])+(yG[4]*xG[4]));

			if(sumg>255) sumg=255;
			if(sumg<0) sumg=0;

			dst.at<uchar>(y,x) = sumg;

		}
	}
	return dst;
}

//현재 그린에 초첨을 맞추고 있음
Mat dcPreprocess::HSV(Mat image){

	int width = image.cols;
	int height = image.rows;

	Mat hsvoutput;
	cvtColor(image, hsvoutput, CV_BGR2HSV);

	uchar hue,sat,val;
	Mat green_image = Mat(Size(width,height), CV_8UC1);

	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			hue = hsvoutput.at<Vec3b>(y,x)[0];
			sat  = hsvoutput.at<Vec3b>(y,x)[1];
			val  = hsvoutput.at<Vec3b>(y,x)[2];

			if((hue >73 || hue < 170) && sat >95){
			//if((hue >53 || hue < 160) && (sat >74 || sat <95)){
				green_image.at<uchar>(y,x)=0;
			}else{
				green_image.at<uchar>(y,x)=255;
			}
		}
	}
	return green_image;
}

Mat dcPreprocess::addWeight(Mat src1,Mat src2,Mat dst){
	double alpha = 1.5; double beta; //double input=0.5;
	 beta = ( 1.0 - alpha );
	 addWeighted( src1, alpha, src2, beta, 0.0, dst);
	 return dst;
}

void dcPreprocess::insertionSort(int window[]){
    int temp, i , j;
    for(i = 0; i < 9; i++){
        temp = window[i];
        for(j = i-1; j >= 0 && temp < window[j]; j--){
            window[j+1] = window[j];
        }
        window[j+1] = temp;
    }
}

int dcPreprocess::Mean(int window[]){
     int i, sum;

     sum=0;
     for (i=0; i<9; i++)
          sum += (int)window[i];
     return (sum/9);
}

Mat dcPreprocess::Meanfilter(Mat src){

	 /* 이미지의 가로 / 세로 길이 얻음 */
		//int ImageWidth = src.cols;
		//int ImageHeight = src.rows;
		dcPreprocess mean;

		/* 사본 이미지의 크기 설정 */
		Mat dst = src.clone();

		/* Mean Filter */
		int window[9];

		for(int y = 0; y < src.rows; y++)
				   for(int x = 0; x < src.cols; x++)
					   dst.at<uchar>(y,x) = 0.0;

	   for(int y = 1; y < src.rows - 1; y++){
		   for(int x = 1; x < src.cols - 1; x++){

			   // Pick up window element

			   window[0] = src.at<uchar>(y - 1 ,x - 1);
			   window[1] = src.at<uchar>(y, x - 1);
			   window[2] = src.at<uchar>(y + 1, x - 1);
			   window[3] = src.at<uchar>(y - 1, x);
			   window[4] = src.at<uchar>(y, x);
			   window[5] = src.at<uchar>(y + 1, x);
			   window[6] = src.at<uchar>(y - 1, x + 1);
			   window[7] = src.at<uchar>(y, x + 1);
			   window[8] = src.at<uchar>(y + 1, x + 1);

			   // sort the window to find median
			  int rMean= mean.Mean(window);

			   // assign the median to centered element of the matrix
			  dst.at<uchar>(y,x) = rMean;
		   }
	   }
	   return dst;
}

Mat dcPreprocess::Medianfilter(Mat src){

	 /* 이미지의 가로 / 세로 길이 얻음 */
		//int ImageWidth = src.cols;
		//int ImageHeight = src.rows;
		dcPreprocess median;

		/* 사본 이미지의 크기 설정 */
		Mat dst = src.clone();

		/* Mean Filter */
		int window[9];

		for(int y = 0; y < src.rows; y++)
				   for(int x = 0; x < src.cols; x++)
					   dst.at<uchar>(y,x) = 0.0;

	   for(int y = 1; y < src.rows - 1; y++){
		   for(int x = 1; x < src.cols - 1; x++){

			   // Pick up window element

			   window[0] = src.at<uchar>(y - 1 ,x - 1);
			   window[1] = src.at<uchar>(y, x - 1);
			   window[2] = src.at<uchar>(y + 1, x - 1);
			   window[3] = src.at<uchar>(y - 1, x);
			   window[4] = src.at<uchar>(y, x);
			   window[5] = src.at<uchar>(y + 1, x);
			   window[6] = src.at<uchar>(y - 1, x + 1);
			   window[7] = src.at<uchar>(y, x + 1);
			   window[8] = src.at<uchar>(y + 1, x + 1);

			   // sort the window to find median
			   median.insertionSort(window);

			   // assign the median to centered element of the matrix
			  dst.at<uchar>(y,x) = window[4];
		   }
	   }
	   return dst;
}


