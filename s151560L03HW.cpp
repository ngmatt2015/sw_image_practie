#include "SIFT.h"
#define DIST_RATIO_THR 0.49

//�ǽ����� �ۼ��� �ڵ� ���
int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &matchingIdx) {
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
	int idx1, idx2;

	matchingIdx.resize(descriptor1.rows);

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
				else if (d2 > distance) {
					d2 = distance;
					idx2 = j;
				}
			}
		}

		//cout << i << endl << d1 << endl;
		if (d1 != 0 && d2 != 0 && (float)(d1 / d2) < DIST_RATIO_THR) {
			matchingIdx[i] = idx1;
			numMatches++;
		}
		else {
			matchingIdx[i] = -1;
		}
	}

	return numMatches;
}


Mat stack_imgs(const Mat &im1, const Mat &im2) {
	//�ΰ��� �̹����� �ϳ��� �̹����� ��ġ�� �Լ�
	//�ΰ��� �̹��� ũ�Ⱑ �ٸ� �� �ֱ� ������ opencv�� hconcat, hstack�� ������� �ʰ�, copyTo �Լ��� ����Ѵ�.
	//�ΰ��� �̹����� �����ִ� ū �ϳ��� Mat stacked�� ����� (Mat type�� �̹���1, 2�� ���� �Ѵ�),
	//copyTo �Լ��� ����� �̹���1�� �̹���2�� stacked�� �����Ѵ�.

	Mat stacked;

	//img1, img2�� stacked�� ����
	//���� ����
	int row_1 = im1.rows, col_1 = im1.cols;
	int row_2 = im2.rows, col_2 = im2.cols;
	int row, col;
	
	//Mat stacked �ʱ�ȭ
	if (row_1 > row_2)
		row = row_1;
	else
		row = row_2;
	col = col_1 + col_2;
	stacked = Mat(row, col, im1.type());
	//printf("%d\n", im1.type());

	/*cout << row_1 << " " << col_1 << endl;
	cout << row_2 << " " << col_2 << endl;
	cout << row << " " << col << endl;*/

	//copy
	im1.copyTo(stacked(Rect(0, 0, col_1, row_1)));
	im2.copyTo(stacked(Rect(col_1, 0, col_2, row_2)));

	return stacked;
}


Mat DrawMatches(const Mat &im1, const Mat &im2, vector<KeyPoint> &keypoints1, vector<KeyPoint> &keypoints2, vector<int> &matchingIdx) {
	//��Ī����� ������ �޾Ƽ� ��Ī�� Ű����Ʈ�� ��ġ�� ��Ī ��� ������ ���� ���� �׸��� �Լ�
	//stack_imgs() �Լ��� ����� 2���� �̹����� ������ �ϳ��� �̹����� ����
	//Ű����Ʈ�� ��ġ�� opencv�� circle() �Լ� ���, ���ἱ�� opencv�� line() �Լ� ���

	Mat result_im;
	result_im = stack_imgs(im1, im2);//
	int row_1 = im1.rows, col_1 = im1.cols;
	int row_2 = im2.rows, col_2 = im2.cols;
	int index;

	//Draw keypoint location and line between matching pair
	//���� ����

	//draw keypoint by circle
	int S = keypoints1.size();
	for (int i = 0; i < S; i++) {
		if (matchingIdx[i] == -1)
			continue;
		circle(result_im, keypoints1[i].pt, 2, 255, -1);
		index = matchingIdx[i];
		Point key_points2 = keypoints2[index].pt;
		key_points2.x = key_points2.x + col_1;
		circle(result_im, key_points2, 2, 255, -1);
	}

	/*int S2 = keypoints2.size();
	for (int i = 0; i < S2; i++) {
		Point key_point = keypoints2[i].pt;
		key_point.x = key_point.x + col_1;
		circle(result_im, key_point, 2, 0, -1);
	}*/

	//draw matching line by line
	for (int i = 0; i < S; i++) {
		if (matchingIdx[i] == -1)
			continue;
		index = matchingIdx[i];
		Point key_points2 = keypoints2[index].pt;
		key_points2.x = key_points2.x + col_1;
		line(result_im, keypoints1[i].pt, key_points2, (0,0,255));
	}


	return result_im;
}