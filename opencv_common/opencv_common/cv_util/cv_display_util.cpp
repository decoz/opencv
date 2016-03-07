#include "cv_util.h"

void putMultiLine(Mat img, const string& text, Point org,	int fontFace, double fontScale, Scalar color, int l_height = 15){

	String linetext = "";

	for(int i=0;i<text.length();i++){
		if(text[i] != 10) linetext += text[i];
		else {
			putText( img, linetext , org, fontFace , fontScale, color );
			linetext = "";
			org.y += l_height;
		}
	}

	if(linetext != "") putText( img, linetext , org, fontFace , fontScale, color );
}
