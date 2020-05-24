#include "SIFT.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define DIST_RATIO_THR 0.49

void customGaussianBlur(const Mat &src, Mat &dst, double sigma) {
	////////////////////////////////////////////////////////////
	// 가우시안 커널을 생성해 이미지에 적용하는 함수
	// 커널의 크기는 2*(4*sigma)+1 (소수점은 올림 또는 반올림으로 처리한다)
	// 가우시안 커널은 float형의 값을 가짐
	// 가우시안 커널은 커널 값 계산 후 전체의 총합으로 나눠서 0~1사이의 값을 가지도록 정규화
	// 가우시안 커널을 이미지에 적용할때는 filter2D() 함수를 사용
	////////////////////////////////////////////////////////////

	// Declare variable. (Kernel, Kernel size)
	int kernel_size;
	int i, j, x, y;
	double sum = 0;
	float a, b;
	Mat gau_kernel;

	//내용 구현


	// Compute gaussian kernel value
	//내용 구현

	kernel_size = 2 * (4 * (int)ceil(sigma)) + 1;
	int kernel_start = -kernel_size / 2;
	gau_kernel = Mat(kernel_size, kernel_size, CV_32FC1);

	// Normalize the kernel
	//내용 구현

	for (i = 0; i < kernel_size; i++) {
		for (j = 0; j < kernel_size; j++) {
			x = kernel_start + i;
			y = kernel_start + j;
			a = (float)(2 * M_PI * sigma * sigma);
			b = (float)(-1 * (x * x + y * y) / (float)(2 * sigma * sigma));
			gau_kernel.at<float>(i, j) = (float)(exp(b) / a);
			sum += (float)(exp(b) / a);
		}
	}

	gau_kernel = gau_kernel / sum;

	// Apply Gaussian kernel
	//내용 구현
	filter2D(src, dst, -1, gau_kernel);
}
//////////////////////////////////////////////


int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &machingIdx) {
	///////////////////////////////////////////////////////////////
	// 이미지1의 디스크립터들과 이미지2의 디스크립터들 중 매칭되는 페어의 개수를 계산하는 함수
	// 이미지1의 1개의 디스크립터와 이미지2의 모든 디스크립터 사이의 L2 거리를 계산 
	// 가장 가까운 거리 (d1) 와 두번째로 가까운 거리(d2) 의 비율 (d1/d2) 을 구해 설정된 임계값 (DIST_RATIO_THR) 보다 작다면 매칭으로 판단
	// machingIdx 에는 매칭이 된 인덱스를 저장 (machingIdx[idx1] = idx2, idx1은 이미지1의 디스크립터의 인덱스이고, idx2는 매칭된 이미지2의 디스크립터의 인덱스)
	// 함수의 리턴값으로는 매칭된 페어의 개수를 출력
	//////////////////////////////////////////////////////////////
	int numMatches = 0;
	float temp, tsum = 0, distance;
	float d1, d2;
	int idx1, idx2;     //index about descriptor2

	machingIdx.resize(descriptor1.rows);

	//내용 구현

	for (int i = 0; i < descriptor1.rows; i++) {
		d1 = d2 = 0;
		for (int j = 0; j < descriptor2.rows; j++) {
			tsum = 0;
			for (int k = 0; k < 128; k++) {
				temp = (descriptor1.at<float>(i, k) - descriptor2.at<float>(j, k));
				tsum += temp * temp;
			}
			distance = sqrt(tsum);

			if (j == 0) {
				d1 = distance;
				idx1 = j;
			}
			else {
				if (d1 > distance) {
					temp = d1;
					d1 = distance;
					d2 = temp;
					idx2 = idx1;
					idx1 = j;
				}
				else if(d2 > distance) {
					d2 = distance;
					idx2 = j;
				}
			}
		}

		//cout << i << endl << d1 << endl;
		if (d1 != 0 && d2 != 0 && (float)(d1 / d2) < DIST_RATIO_THR) {
			machingIdx[i] = idx1;
			numMatches++;
		}
		else {
			machingIdx[i] = -1;
		}
	}

	return numMatches;
}