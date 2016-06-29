#ifndef CV_UTIL_H_
#define CV_UTIL_H_
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
/******************************** cvt_color **************************/

Mat asBgBlack(Mat src);
Mat asBgWhite(Mat src);
Mat asGray(Mat src);
Mat asBGR(Mat src);

/******************************** display util ************************/

void putMultiLine(Mat img, const string& text, Point org,	int fontFace, double fontScale, Scalar color, int l_height );
Scalar randcolor(int min, int max);

/******************************** Rect ********************************/

Rect 	imgrect(Mat img );
bool 	inRectP(Rect r, Point p);				// �� p�� r �ȿ� �����ϴ°��� Ȯ��
bool 	inRectR(Rect br, Rect sr);				// sr �� br �ȿ� �����ϴ� ���� Ȯ��

Rect 	interRect(Rect r1, Rect r2);				// �� �����߿� ���������� ���

Rect 	rectsum(Rect r1,Rect r2);				// �� ������ �����ϴ� ������ ���
Rect 	rectsum(vector <Rect> rects);			//  vector ��  ������ �ջ��ϴ� ������ ���

Rect 	fitRect(Rect r, Rect a_r);				// r �� a_r �� ������ �ʰ��� ��� �̸� a_r ������ ����

char 	*tostr(Rect r);							// rect �� ���ڿ��� 1,2,2,1


/********************************** angle ****************************/

double 	lag3Angle(Point p1, Point p2, Point p3, float t);			// ��׶��� ������ �̿��� p1 , p2, p3 �� ������ ��� t( 0~2 ) �������� ������ ����
double 	line2theta(Point p1, Point p2);								// p1��  p2 �� �մ� line �� ������ ���


/********************************** string ****************************/
// �ѱ�ó��
String 	wcharAt(String s, int idx);			// �ѱۿ��θ� ����� n��° ���� ����
int 		wcharcnt(String s);					// ����ڷ� ������ ��Ʈ������ ����

#endif
