#pragma once

#include "aviloader.h"
#include ".\tchart\tchart.h"
#include "imagedisplay.h"
#include "calibration.h"
#include "eu_testfilev2.h"


class CAviLoaderManager
{
public:
	CAviLoaderManager(void);
	~CAviLoaderManager(void);

protected:
	bool m_bAviExist;
	CAviLoader* m_pAviLoader;
	CTChart* m_ChartCtrl;
	CImageDisplay* m_pImageDisplay;

	IplImage* m_pStereoImage;

	EU_TestFileV2 m_testFile;

	unsigned long m_ulPos;

	CString m_strAviFname;
	CString m_strDataFname;
public:
	CString getFname(){return m_strAviFname;};
	void setData(CTChart* pTChart, CImageDisplay* pImageDisplay);
	bool open(CString strAviFname, CString strDataFname);
	void close();
	unsigned long next();
	void setPos(unsigned long pos);
	unsigned long getPos();

};
