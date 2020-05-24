#pragma once
#include <iostream>
#include <time.h>// 시간 측정 위해서
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <queue>
#include <cstdlib>

using namespace cv;
using namespace std;

Mat input_im, dst;
clock_t start_time, end_time;

void moving_average(int ksize)
{
	start_time = clock();
	int i, j, k;
	int h = input_im.rows;
	int w = input_im.cols;
	int c = 0;

	float ** ptr = new float*[h];
	for (i = 0; i < h; i++)
		ptr[i] = new float[w];

	if (ksize % 2 == 0) ksize = ksize - 1;
	int hsize = ksize / 2;
	float sum;

	for (j = 0; j < w; j++) {
		sum = 0;
		for (i = 0; i < h; i++) {
			c = 0;
			if (i == 0) {	//초기값 평균내기 (첫 행)
				for (k = -hsize; k <= hsize; k++) {
					if ((i + k) < 0 || (i + k) >= h)  continue;
					else {
						sum += (float)input_im.at<uchar>(i + k, j);
						c++;
					}
				}
				ptr[i][j] = (sum / c);
			}
			else { // 첫 행 제외하고 moving average 이용
				for (k = -hsize; k <= hsize; k++) {
					if (i + k >= 0 && i + k < h) {
						c++;
					}
				}
				if ((i - hsize - 1) >= 0) { //빼는 부분
					sum -= (float)input_im.at<uchar>(i - hsize - 1, j);
				}

				if ((i + hsize) < h) { //더하는 부분
					sum += (float)input_im.at<uchar>(i + hsize, j);
				}

				ptr[i][j] = (sum / c);
			}
		}
	}

	for (i = 0; i < h; i++) {
		sum = 0;
		for (j = 0; j < w; j++) {
			c = 0;
			if (j == 0) {	//세로로 평균 구한 2차원 배열중 첫 열
				for (k = -hsize; k <= hsize; k++) {
					if ((j + k) < 0 || (j + k) >= w)  continue;
					else {
						sum += ptr[i][j + k];
						c++;
					}
				}
				dst.at<uchar>(i, j) = (uchar)(sum / c);
			}
			else { // 평균이 구해진 첫 열 제외하고 moving average
				for (k = -hsize; k <= hsize; k++) {
					if (j + k >= 0 && j + k < w) {
						c++;
					}
				}
				if ((j - hsize - 1) >= 0) { //빼는 부분
					sum -= ptr[i][j - hsize - 1];
				}

				if ((j + hsize) < w) { //더하는 부분
					sum += ptr[i][j + hsize];
				}

				dst.at<uchar>(i, j) = (uchar)(sum / c);
			}
		}
	}
		end_time = clock();
		cout << "Kernel Size : " << ksize << ", Exec Time : " << (double)(end_time - start_time) << " (msec)" << endl;

		imshow("movingAvrg HW", dst);
}

string type2str(int type) {
	// https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}


int main(int argc, char *argv[]) {
	int kernal_size;
	if (argc != 3) {
		cout << "[프로그램 사용법]" << endl;
		cout << "명령문 : snnnnnnHW01.exe imageFile kernalSize<ent>" << endl;
		return 0;
	}
	//파일 주소는 argv[1]로 들어옴
	input_im = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	dst = Mat(input_im.rows, input_im.cols, input_im.type());
	if (input_im.empty())
	{
		cout << "File open Error!" << endl;
		return -1;
	}
	cout << argv[1] << "Image size :" << input_im.size() << ", Type:" << type2str(input_im.type()) << endl;

	//string window_name = "Moving Average Filtering Window";
	//namedWindow(window_name);

	//Create track bar to change kernel size
	//OpenCV Function

	kernal_size = atoi(argv[2]);
	//imshow("moving average filter1", input_im);
	moving_average(kernal_size);
	waitKey(0);
	return 0;
}