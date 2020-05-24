#include "stdafx.h"

#include "swLab19fDoc.h"
#include "swLab19fView.h"
#include "MainFrm.h"

#include "s151596L01_Mfc.h"
#include "s151596L01_ext.h"
#include "CDIB.h"


void SWL01::DarkenGrayscaleImage(void)
{
    Mat   *pMat;
    int   w, h;
    w = m_Mat1.cols;
    h = m_Mat1.rows;

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

    m_MatR.release();		// clear any previous image data
    m_MatR = Mat(h, w, CV_8UC1);	// allocate Mat

    unsigned char *pMatData = (m_MatR).data;
    unsigned char *pDibData = (*pMat).data;
    for (int p = 0; p < h * w; p++) {
        *pMatData = ((*pDibData) / 2);
        *pMatData++;
        *pDibData++;
    }

    Create_bmiInfoHeader(&m_MatR); // ภฮฦ๗ว์ด๕ธฆ ฐปฝล
    processedF = true; // รณธฎ ฟฯทแธฆ flagภป ล๋วฯฟฉ พหธฐดู
    g_pView->Invalidate(); // OnDrawธฆ ศฃรโวัดู 
    return;
}


void SWL01::BrightenGrayscaleImage()
{
    Mat   *pMat;
    int   w, h;
    w = m_Mat1.cols;
    h = m_Mat1.rows;

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

    m_MatR.release();		// clear any previous image data
    m_MatR = Mat(h, w, CV_8UC1);	// allocate Mat

    unsigned char *pMatData = (m_MatR).data;
    unsigned char *pDibData = (*pMat).data;
    for (int p = 0; p < h * w; p++) {
        if ((*pDibData) > 127) {
            *pMatData = 255;
            *pMatData++;
            *pDibData++;
        }
        else {
            *pMatData = ((*pDibData) * 2);
            *pMatData++;
            *pDibData++;
        }
    }


    Create_bmiInfoHeader(&m_MatR); // ภฮฦ๗ว์ด๕ธฆ ฐปฝล
    processedF = true; // รณธฎ ฟฯทแธฆ flagภป ล๋วฯฟฉ พหธฐดู
    g_pView->Invalidate(); // OnDrawธฆ ศฃรโวัดู 
    return;
}

void SWL01::Color24toGrayscale()
{
    Mat   *pMat;
    int   w, h;
    w = m_Mat1.cols;
    h = m_Mat1.rows;

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

    pMat = &m_Mat1;

    m_MatR.release();		// clear any previous image data
    m_MatR = Mat(h, w, CV_8UC1);	// allocate Mat

    unsigned char *pMatData = (m_MatR).data;
    unsigned char *pDibData = (*pMat).data;
    for (int p = 0; p < h * w; p++) {
        *pMatData = ((*pDibData) *0.114);
        *pDibData++;
        *pMatData = ((*pDibData) *0.299);
        *pDibData++;
        *pMatData = ((*pDibData) *0.587);
        *pMatData++;
        *pDibData++;
    }


    Create_bmiInfoHeader(&m_MatR); // ภฮฦ๗ว์ด๕ธฆ ฐปฝล
    processedF = true; // รณธฎ ฟฯทแธฆ flagภป ล๋วฯฟฉ พหธฐดู
    g_pView->Invalidate(); // OnDrawธฆ ศฃรโวัดู 
    return;
}

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

    double scaleup = 255 / 31;
    unsigned char *pMatData = (m_MatR).data;
    unsigned short *pDibData = (unsigned short *)m_dibFile1.m_lpImage;

    double result;
    for (int p = 0; p < h * w * (*pMat).channels(); p++) {
        /*unsigned short *pData = (unsigned short*)(*pDibData++);
        *pData++ += (*pDibData++)* 256 ;*/

        *pMatData = (unsigned char)(((double)((*pDibData & red_mask)>>10)* 0.299) * scaleup);
        *pMatData += (unsigned char)(((double)((*pDibData & green_mask)>>5) *0.587) * scaleup);
        *pMatData++ += (unsigned char)(((double)(*pDibData++ & blue_mask) *0.114) * scaleup);
        
        result = (double)(((*pDibData) & 0b0111110000000000) >> 10)*0.299 + (double)(((*pDibData) & 0b0000001111100000) >> 5)*0.587 + (double)((*pDibData) & 0b0000000000011111)*0.114;

        result = result * scaleup;
        *pMatData = (unsigned char)result;
        *pMatData++;
        *pDibData++;
    }
    flip(m_MatR, m_MatR, 0);

    Create_bmiInfoHeader(&m_MatR); // ภฮฦ๗ว์ด๕ธฆ ฐปฝล
    processedF = true; // รณธฎ ฟฯทแธฆ flagภป ล๋วฯฟฉ พหธฐดู
    g_pView->Invalidate(); // OnDrawธฆ ศฃรโวัดู 
    return;
}