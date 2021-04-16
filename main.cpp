//Mouse Events
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

Mat im1; //��ø��
Mat im2; //�I����
Mat im3; //�ĦX�᪺��
Mat im4; //�Ȧs�Y��᪺��ø�Ϧܦ��Ϫ�����
int buttonDown = 0; //��������O�_�����U
int slider1 = 50; //�Ĥ@��slider
int slider2 = 50; //�ĤG��slider


void on_trackbar(int, void *) {
	Mat temp_img1; //�s��i���Y��j�p�᪺�Ϥ�
	Mat temp_img2; //�s��resize im1��im2�j�p�᪺�Ϥ�
	int slider1_value = max(0, slider1); //�N�Ĥ@��slider���ȳ]�w��0~100
	int slider2_value = max(1, slider2); //�N�ĤG��slider���ȳ]�w��1~100
	double scalar = slider2_value / 100.0; //�]�w�Y����
	im4 = Mat(im2.rows, im2.cols, CV_8UC3, Scalar(0)); //�ŧi�n�s���ø�Ϫ��Ϫ��j�p
	resize(im1, temp_img2, Size(im2.cols, im2.rows), INTER_LINEAR); //�N��ø�ϩ�j���I���Ϫ��j�p
	resize(temp_img2, temp_img1, Size(), scalar, scalar, INTER_LINEAR); //�ھ�slider2���ȨӨM�w�n�N��ø���Y��h��
	Rect roi((im2.cols / 2 - temp_img1.cols / 2), (im2.rows / 2 - temp_img1.rows / 2), temp_img1.cols, temp_img1.rows); //�M�w�Ϫ�������m
	Mat roi_of_im4 = im4(roi); //�o��im4��������m
	temp_img1.copyTo(roi_of_im4); //�N�Y��j�p�᪺��ø�Ϧs��im4��������m
	addWeighted(im4, double(slider1_value) / 100, im2, double(100 - slider1_value) / 100, 0.0, im3); //void addWeighted(InputArray src1, double alpha, InputArray src2, double beta, double gamma, OutputArray dst, int dtype=-1)�i��v���ĦX
	imshow("�ĦX", im3);
}

static void onMouse(int event, int x, int y, int flags, void*)
{
	switch (event) {
	case EVENT_LBUTTONDOWN: //������U���ƥ�
		if (flags == (EVENT_FLAG_LBUTTON))
			buttonDown = 1;
			cout << "Left button down" << endl;
		break;

	case EVENT_MOUSEMOVE: //�ƹ����ʪ��ƥ�
		cout << "Mouse are moving" << x << "  " << y << endl;
		if (buttonDown == 1) {
			circle(im1, Point(x, y), 6, CV_RGB(255, 255, 0), FILLED, LINE_AA); //�e��߶�A�b�|��6
			imshow("processed image", im1);
		}
		break;

	case EVENT_LBUTTONUP: //�����}���ƥ�
		buttonDown = 0;
		cout << "Left button up" << endl;
		break;

	case EVENT_RBUTTONDOWN: //�k����U���ƥ�
		destroyWindow("processed image"); //����processed image������
		namedWindow("�ĦX"); //�гy�ĦX����
		imshow("�ĦX", im2); 
		on_trackbar(0, 0); //����l�Ƥ@���ĦX�������e��
		createTrackbar("�v���ĦX", "�ĦX", &slider1, 100, on_trackbar); //�إ�slider1
		createTrackbar("�j�p", "�ĦX", &slider2, 100, on_trackbar); //�إ�slider2
		break;
	}
}

//   event: EVENT_LBUTTONDOWN,   EVENT_RBUTTONDOWN,   EVENT_MBUTTONDOWN,
//        EVENT_LBUTTONUP,     EVENT_RBUTTONUP,     EVENT_MBUTTONUP,
//        EVENT_LBUTTONDBLCLK, EVENT_RBUTTONDBLCLK, EVENT_MBUTTONDBLCLK,
//        EVENT_MOUSEMOVE: 

//   flags: EVENT_FLAG_CTRLKEY, EVENT_FLAG_SHIFTKEY, EVENT_FLAG_ALTKEY,
//        EVENT_FLAG_LBUTTON, EVENT_FLAG_RBUTTON,  EVENT_FLAG_MBUTTON

int main(int, char** argv)
{
	im1 = Mat(400, 400, CV_8UC3, Scalar(0));

	//load an image
	im2 = imread(argv[1], 1); //0: grayscale
	if (!im2.data) cout << "Could not load image file " << endl;
	else {

		namedWindow("processed image");
		imshow("processed image", im1);
		setMouseCallback("processed image", onMouse);

		waitKey(0); //0���۰ʰh�X
	}
	return 0;
}