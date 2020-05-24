#include "SIFT.h"
#define _USE_MATH_DEFINES
#include <math.h>
#define DIST_RATIO_THR 0.49

void customGaussianBlur(const Mat &src, Mat &dst, double sigma) {
	////////////////////////////////////////////////////////////
	// ����þ� Ŀ���� ������ �̹����� �����ϴ� �Լ�
	// Ŀ���� ũ��� 2*(4*sigma)+1 (�Ҽ����� �ø� �Ǵ� �ݿø����� ó���Ѵ�)
	// ����þ� Ŀ���� float���� ���� ����
	// ����þ� Ŀ���� Ŀ�� �� ��� �� ��ü�� �������� ������ 0~1������ ���� �������� ����ȭ
	// ����þ� Ŀ���� �̹����� �����Ҷ��� filter2D() �Լ��� ���
	////////////////////////////////////////////////////////////

	// Declare variable. (Kernel, Kernel size)
	int kernel_size;
	int i, j, x, y;
	double sum = 0;
	float a, b;
	Mat gau_kernel;

	//���� ����


	// Compute gaussian kernel value
	//���� ����

	kernel_size = 2 * (4 * (int)ceil(sigma)) + 1;
	int kernel_start = -kernel_size / 2;
	gau_kernel = Mat(kernel_size, kernel_size, CV_32FC1);

	// Normalize the kernel
	//���� ����

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
	//���� ����
	filter2D(src, dst, -1, gau_kernel);
}
//////////////////////////////////////////////


int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &machingIdx) {
	///////////////////////////////////////////////////////////////
	// �̹���1�� ��ũ���͵�� �̹���2�� ��ũ���͵� �� ��Ī�Ǵ� ����� ������ ����ϴ� �Լ�
	// �̹���1�� 1���� ��ũ���Ϳ� �̹���2�� ��� ��ũ���� ������ L2 �Ÿ��� ��� 
	// ���� ����� �Ÿ� (d1) �� �ι�°�� ����� �Ÿ�(d2) �� ���� (d1/d2) �� ���� ������ �Ӱ谪 (DIST_RATIO_THR) ���� �۴ٸ� ��Ī���� �Ǵ�
	// machingIdx ���� ��Ī�� �� �ε����� ���� (machingIdx[idx1] = idx2, idx1�� �̹���1�� ��ũ������ �ε����̰�, idx2�� ��Ī�� �̹���2�� ��ũ������ �ε���)
	// �Լ��� ���ϰ����δ� ��Ī�� ����� ������ ���
	//////////////////////////////////////////////////////////////
	int numMatches = 0;
	float temp, tsum = 0, distance;
	float d1, d2;
	int idx1, idx2;     //index about descriptor2

	machingIdx.resize(descriptor1.rows);

	//���� ����

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