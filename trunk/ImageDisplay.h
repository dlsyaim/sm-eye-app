#pragma once
#include "afxwin.h"
#include "cv.h"

struct structPupilData{
	double cx;
	double cy;
	double radius;
	double theta;
	bool valid;
};

class CImageDisplay :
	public CStatic
{
public:
	CImageDisplay(void);
public:
	~CImageDisplay(void);

	unsigned char* m_pImageData;
	BITMAPINFO* m_pBitmapInfo;
	void New(void);
public:
	void Delete(void);
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	void SetData(unsigned char* pImage);

	HANDLE m_eventDisplay;
protected:
	bool m_bDiplayed;
	
	
};
