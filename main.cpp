//Mouse Events
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

Mat im1; //手繪圖
Mat im2; //背景圖
Mat im3; //融合後的圖
Mat im4; //暫存縮放後的手繪圖至此圖的中央
int buttonDown = 0; //紀錄左鍵是否有按下
int slider1 = 50; //第一條slider
int slider2 = 50; //第二條slider


void on_trackbar(int, void *) {
	Mat temp_img1; //存放進行縮放大小後的圖片
	Mat temp_img2; //存放resize im1成im2大小後的圖片
	int slider1_value = max(0, slider1); //將第一條slider的值設定成0~100
	int slider2_value = max(1, slider2); //將第二條slider的值設定成1~100
	double scalar = slider2_value / 100.0; //設定縮放比例
	im4 = Mat(im2.rows, im2.cols, CV_8UC3, Scalar(0)); //宣告要存放手繪圖的圖的大小
	resize(im1, temp_img2, Size(im2.cols, im2.rows), INTER_LINEAR); //將手繪圖放大成背景圖的大小
	resize(temp_img2, temp_img1, Size(), scalar, scalar, INTER_LINEAR); //根據slider2的值來決定要將手繪圖縮放多少
	Rect roi((im2.cols / 2 - temp_img1.cols / 2), (im2.rows / 2 - temp_img1.rows / 2), temp_img1.cols, temp_img1.rows); //決定圖的中央位置
	Mat roi_of_im4 = im4(roi); //得到im4的中央位置
	temp_img1.copyTo(roi_of_im4); //將縮放大小後的手繪圖存至im4的中央位置
	addWeighted(im4, double(slider1_value) / 100, im2, double(100 - slider1_value) / 100, 0.0, im3); //void addWeighted(InputArray src1, double alpha, InputArray src2, double beta, double gamma, OutputArray dst, int dtype=-1)進行影像融合
	imshow("融合", im3);
}

static void onMouse(int event, int x, int y, int flags, void*)
{
	switch (event) {
	case EVENT_LBUTTONDOWN: //左鍵按下的事件
		if (flags == (EVENT_FLAG_LBUTTON))
			buttonDown = 1;
			cout << "Left button down" << endl;
		break;

	case EVENT_MOUSEMOVE: //滑鼠移動的事件
		cout << "Mouse are moving" << x << "  " << y << endl;
		if (buttonDown == 1) {
			circle(im1, Point(x, y), 6, CV_RGB(255, 255, 0), FILLED, LINE_AA); //畫實心圓，半徑為6
			imshow("processed image", im1);
		}
		break;

	case EVENT_LBUTTONUP: //左鍵放開的事件
		buttonDown = 0;
		cout << "Left button up" << endl;
		break;

	case EVENT_RBUTTONDOWN: //右鍵按下的事件
		destroyWindow("processed image"); //關閉processed image的視窗
		namedWindow("融合"); //創造融合視窗
		imshow("融合", im2); 
		on_trackbar(0, 0); //先初始化一次融合視窗的畫面
		createTrackbar("影像融合", "融合", &slider1, 100, on_trackbar); //建立slider1
		createTrackbar("大小", "融合", &slider2, 100, on_trackbar); //建立slider2
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

		waitKey(0); //0不自動退出
	}
	return 0;
}