#include "StdAfx.h"
#include ".\avifile.h"
#include "corwlDefines.h"

CAviFile::CAviFile(void)
: m_pVideoWriter(NULL)
, m_bWrite(false)
, m_ulTotalFrameNum(0)
{
	m_pImage = ::cvCreateImage(cvSize(IMAGE_WIDTH*2, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);

}

CAviFile::~CAviFile(void)
{
	if(m_pImage)
		cvReleaseImage(&m_pImage);

	
}

bool CAviFile::open(CString fname, CvSize imageSize, bool bWrite)
{
	bool bRtn = false;

	//write용인지 read용인지
	if(bWrite)
	{
		m_pVideoWriter = cvCreateVideoWriter(fname,-1 /*CV_FOURCC('F','F','D','S')*/, 30, imageSize);
		m_ulTotalFrameNum = 0;

		if(m_pVideoWriter)
			bRtn = true;
		else
			::AfxMessageBox("Cannot open avi file!!");
	}

	if(bRtn)
		m_imgSize = imageSize;


	return bRtn;
}

void CAviFile::close()
{
	if(m_pVideoWriter)
		cvReleaseVideoWriter(&m_pVideoWriter);
	TRACE("AVI Closed\n");

	m_pVideoWriter = NULL;
}


bool CAviFile::addFrame(IplImage* pImg0, IplImage* pImg1)
{
	bool bRtn;
	
	if(m_pVideoWriter)
	{
		ASSERT(pImg0->width*2 == m_pImage->width);
		ASSERT(pImg0->height == m_pImage->height);
		ASSERT(pImg0->nChannels == 1);
		ASSERT(pImg1->width*2 == m_pImage->width);
		ASSERT(pImg1->height == m_pImage->height);	
		ASSERT(pImg1->nChannels == 1);

		
		::cvSetImageROI(m_pImage, cvRect(0,0,IMAGE_WIDTH,IMAGE_HEIGHT));
		::cvMerge(pImg0, pImg0, pImg0, NULL, m_pImage);
		::cvSetImageROI(m_pImage, cvRect(IMAGE_WIDTH, 0, IMAGE_WIDTH,IMAGE_HEIGHT));
		::cvMerge(pImg1, pImg1, pImg1, NULL, m_pImage);
		::cvResetImageROI(m_pImage);

		cvWriteFrame(m_pVideoWriter, m_pImage);
		m_ulTotalFrameNum++;
		bRtn = true;
	}
	else
		bRtn = false;

	

	return bRtn;
}

unsigned long CAviFile::getTotalFrameNum()
{
	return this->m_ulTotalFrameNum;
}