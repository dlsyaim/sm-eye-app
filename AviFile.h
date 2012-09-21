#pragma once
#include "cv.h"
#include "highgui.h"

class CAviFile
{
public:
	CAviFile(void);
	~CAviFile(void);

protected:
	CvVideoWriter* m_pVideoWriter;
	CvSize m_imgSize;
	CString m_strFname;

	bool m_bWrite;

	unsigned long m_ulTotalFrameNum;

public:
    bool open(CString fname, CvSize imageSize, bool bWrite);
	void close();

	bool addFrame(IplImage* pImg0, IplImage* pImg1);
	unsigned long getTotalFrameNum();

private:
	IplImage* m_pImage;

	
};
