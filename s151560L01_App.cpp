#include "stdafx.h"

#include "swLab19fDoc.h"
#include "swLab19fView.h"
#include "MainFrm.h"

#include "s151560L01_Mfc.h"
#include "s151560L01_ext.h"
#include "CDIB.h"

unsigned short red_mask = 0x7C00;
unsigned short green_mask = 0x3E0;
unsigned short blue_mask = 0x1F;

/**********************
8bit 사진을 받아서 grayscale 하는 함수
간단하게 픽셀을 절반으로 만들어주면 된다.
**********************/
void SWL01::DarkenGrayscaleImage(void)
{
	CDib  *pDibF;
	Mat   *pMat;
	int   w, h, depth;
	if (readImageF1 == false) {
		AfxMessageBox(L"Read image1 first!", MB_OK, 0);
		return;
	}

	pMat = &m_Mat1;
	string s1 = "8UC1";
	string st = type2str((*pMat).type());
	if (st.compare(s1) != 0) {
		AfxMessageBox(L"Only 8bit grayscale can be shown!", MB_OK, 0);
		return;
	}


	pDibF = &m_dibFile1;
	w = m_width1; h = m_height1; depth = m_depth1;

	m_MatR.release();		// clear any previous image data
	m_MatR = Mat(h, w, CV_8UC1);	// allocate Mat

	unsigned char *pMatData = (m_MatR).data;	//받을 unsigned char을 받는다
	unsigned char *pDibData = (*pMat).data;		//넣을 unsigned char을 가져온다
	for (int p = 0; p < h * w * (*pMat).channels(); p++) {
		*pMatData++ = ((*pDibData++) / 2 );		//픽셀값을 반으로 나눠준다 (255까지가 최대이므로 더 짖게 만드는것)
	}


	Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
	processedF = true; // 처리 완료를 flag을 통하여 알린다
	g_pView->Invalidate(); // OnDraw를 호출한다 
	return;
}

/**********************
8bit 사진을 받아 더 밝게 만들어주는 함수
픽셀을 2 배로 곱해주면 된다. 단, 255를 넘을 수 있으므로 그런 경우는 가장 밝은 255로 처리한다.
**********************/
void SWL01::BrightenGrayscaleImage()
{
	CDib  *pDibF;
	Mat   *pMat;
	int   w, h, depth;
	if (readImageF1 == false) {
		AfxMessageBox(L"Read image1 first!", MB_OK, 0);
		return;
	}

	pMat = &m_Mat1;
	string s1 = "8UC1";
	string st = type2str((*pMat).type());
	if (st.compare(s1) != 0) {
		AfxMessageBox(L"Only 8bit grayscale can be shown!", MB_OK, 0);
		return;
	}


	pDibF = &m_dibFile1; pMat = &m_Mat1;
	w = m_width1; h = m_height1; depth = m_depth1;

	m_MatR.release();		// clear any previous image data
	m_MatR = Mat(h, w, CV_8UC1);	// allocate Mat

	unsigned char *pMatData = (m_MatR).data;
	unsigned char *pDibData = (*pMat).data;
	for (int p = 0; p < h * w * (*pMat).channels(); p++) {
		if ((*pDibData) > 127){		//이런 경우 2배를 하면 255를 넘어가므로 예외처리를 해준다.
			*pMatData = 255;
			*pMatData++; *pDibData++;
		}
		else {
			*pMatData++ = ((*pDibData++) * 2);
		}
		
	}


	Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
	processedF = true; // 처리 완료를 flag을 통하여 알린다
	g_pView->Invalidate(); // OnDraw를 호출한다 
	return;
}

/**********************
24bit 사진을 받아 grayscale 을 진행한다.
3channel 이기 때문에 각각의 부분에서 필요한 값을 취해서 곱해주면 된다.
**********************/
void SWL01::Color24toGrayscale()
{
	CDib  *pDibF;
	Mat   *pMat;
	int   w, h, depth;
	if (readImageF1 == false) {
		AfxMessageBox(L"Read image1 first!", MB_OK, 0);
		return;
	}

	pMat = &m_Mat1;
	string s1 = "8UC3";
	string st = type2str((*pMat).type());
	if (st.compare(s1) != 0) {
		AfxMessageBox(L"Only 24bit colorscale can be shown!", MB_OK, 0);
		return;
	}


	pDibF = &m_dibFile1; pMat = &m_Mat1;
	w = m_width1; h = m_height1; depth = m_depth1;

	m_MatR.release();		// clear any previous image data
	m_MatR = Mat(h, w, CV_8UC1);	// allocate Mat

	unsigned char *pMatData = (m_MatR).data;
	unsigned char *pDibData = (*pMat).data;
	for (int p = 0; p < h * w * (*pMat).channels(); p += 3) {
		*pMatData = ((*pDibData++) *0.114);		//필요한 RGB 값을 구해서 흑백으로 만든다.
		*pMatData = ((*pDibData++) *0.299);
		*pMatData++ = ((*pDibData++) *0.587);
	}


	Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
	processedF = true; // 처리 완료를 flag을 통하여 알린다
	g_pView->Invalidate(); // OnDraw를 호출한다 
	return;
}

/**********************
16bit 사진을 받아서 grayscale 을 진행한다.
channel 이 한개이므로 각각의 channel 을 받아와서 unsigned short 로 만드는 방법을 썼다.
**********************/
void SWL01::Color16toGrayscale()
{
	CDib  *pDibF;
	Mat   *pMat;
	int   w, h, depth;
	if (readImageF1 == false) {
		AfxMessageBox(L"Read image1 first!", MB_OK, 0);
		return;
	}

	pMat = &m_Mat1;
	string s1 = "16UC1";
	string st = type2str((*pMat).type());
	if (st.compare(s1) != 0) {
		AfxMessageBox(L"Only 16bit grayscale can be shown!", MB_OK, 0);
		return;
	}


	pDibF = &m_dibFile1; pMat = &m_Mat1;
	w = m_width1; h = m_height1; depth = m_depth1;

	m_MatR.release();		// clear any previous image data
	m_MatR = Mat(h, w, CV_8UC1);	// allocate Mat

	unsigned char *pMatData = (m_MatR).data;

	for (int r = 0; r < h; r++)
		for (int c = 0; c < w; c++)
			for (int ch = 0; ch < (*pMat).channels(); ch++)
			{
				unsigned short temp;
				temp = m_Mat1.at<unsigned short>(r, c); //unsigned short 로 만든다.
				*pMatData = ((((temp & red_mask) >> 10)) * 255 / 31)* 0.299;		//필요한 부분을 곱해서 취한다.
				*pMatData += ((((temp & green_mask) >> 5)) * 255 / 31) *0.587;
				*pMatData++ += (((temp & blue_mask)) * 255 / 31) *0.114;
				
			}


	Create_bmiInfoHeader(&m_MatR); // 인포헤더를 갱신
	processedF = true; // 처리 완료를 flag을 통하여 알린다
	g_pView->Invalidate(); // OnDraw를 호출한다 
	return;
}