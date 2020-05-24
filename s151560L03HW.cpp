#include "SIFT.h"
#define DIST_RATIO_THR 0.49

//실습에서 작성한 코드 사용
int MatchDescriptor(const Mat &descriptor1, const Mat &descriptor2, vector<int> &matchingIdx) {
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
	int idx1, idx2;

	matchingIdx.resize(descriptor1.rows);

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
	//두개의 이미지를 하나의 이미지로 합치는 함수
	//두개의 이미지 크기가 다를 수 있기 때문에 opencv의 hconcat, hstack을 사용하지 않고, copyTo 함수를 사용한다.
	//두개의 이미지가 들어갈수있는 큰 하나의 Mat stacked을 만들고 (Mat type은 이미지1, 2와 같게 한다),
	//copyTo 함수를 사용해 이미지1과 이미지2를 stacked에 복사한다.

	Mat stacked;

	//img1, img2를 stacked에 복사
	//내용 구현
	int row_1 = im1.rows, col_1 = im1.cols;
	int row_2 = im2.rows, col_2 = im2.cols;
	int row, col;
	
	//Mat stacked 초기화
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
	//매칭페어의 정보를 받아서 매칭된 키포인트의 위치와 매칭 페어 사이의 연결 선을 그리는 함수
	//stack_imgs() 함수를 사용해 2개의 이미지를 연결해 하나의 이미지로 만듦
	//키포인트의 위치는 opencv의 circle() 함수 사용, 연결선은 opencv의 line() 함수 사용

	Mat result_im;
	result_im = stack_imgs(im1, im2);//
	int row_1 = im1.rows, col_1 = im1.cols;
	int row_2 = im2.rows, col_2 = im2.cols;
	int index;

	//Draw keypoint location and line between matching pair
	//내용 구현

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