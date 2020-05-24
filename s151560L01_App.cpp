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
8bit ������ �޾Ƽ� grayscale �ϴ� �Լ�
�����ϰ� �ȼ��� �������� ������ָ� �ȴ�.
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

	unsigned char *pMatData = (m_MatR).data;	//���� unsigned char�� �޴´�
	unsigned char *pDibData = (*pMat).data;		//���� unsigned char�� �����´�
	for (int p = 0; p < h * w * (*pMat).channels(); p++) {
		*pMatData++ = ((*pDibData++) / 2 );		//�ȼ����� ������ �����ش� (255������ �ִ��̹Ƿ� �� ¢�� ����°�)
	}


	Create_bmiInfoHeader(&m_MatR); // ��������� ����
	processedF = true; // ó�� �ϷḦ flag�� ���Ͽ� �˸���
	g_pView->Invalidate(); // OnDraw�� ȣ���Ѵ� 
	return;
}

/**********************
8bit ������ �޾� �� ��� ������ִ� �Լ�
�ȼ��� 2 ��� �����ָ� �ȴ�. ��, 255�� ���� �� �����Ƿ� �׷� ���� ���� ���� 255�� ó���Ѵ�.
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
		if ((*pDibData) > 127){		//�̷� ��� 2�踦 �ϸ� 255�� �Ѿ�Ƿ� ����ó���� ���ش�.
			*pMatData = 255;
			*pMatData++; *pDibData++;
		}
		else {
			*pMatData++ = ((*pDibData++) * 2);
		}
		
	}


	Create_bmiInfoHeader(&m_MatR); // ��������� ����
	processedF = true; // ó�� �ϷḦ flag�� ���Ͽ� �˸���
	g_pView->Invalidate(); // OnDraw�� ȣ���Ѵ� 
	return;
}

/**********************
24bit ������ �޾� grayscale �� �����Ѵ�.
3channel �̱� ������ ������ �κп��� �ʿ��� ���� ���ؼ� �����ָ� �ȴ�.
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
		*pMatData = ((*pDibData++) *0.114);		//�ʿ��� RGB ���� ���ؼ� ������� �����.
		*pMatData = ((*pDibData++) *0.299);
		*pMatData++ = ((*pDibData++) *0.587);
	}


	Create_bmiInfoHeader(&m_MatR); // ��������� ����
	processedF = true; // ó�� �ϷḦ flag�� ���Ͽ� �˸���
	g_pView->Invalidate(); // OnDraw�� ȣ���Ѵ� 
	return;
}

/**********************
16bit ������ �޾Ƽ� grayscale �� �����Ѵ�.
channel �� �Ѱ��̹Ƿ� ������ channel �� �޾ƿͼ� unsigned short �� ����� ����� ���.
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
				temp = m_Mat1.at<unsigned short>(r, c); //unsigned short �� �����.
				*pMatData = ((((temp & red_mask) >> 10)) * 255 / 31)* 0.299;		//�ʿ��� �κ��� ���ؼ� ���Ѵ�.
				*pMatData += ((((temp & green_mask) >> 5)) * 255 / 31) *0.587;
				*pMatData++ += (((temp & blue_mask)) * 255 / 31) *0.114;
				
			}


	Create_bmiInfoHeader(&m_MatR); // ��������� ����
	processedF = true; // ó�� �ϷḦ flag�� ���Ͽ� �˸���
	g_pView->Invalidate(); // OnDraw�� ȣ���Ѵ� 
	return;
}