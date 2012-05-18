#pragma once
#include "ed_testinfo.h"
#include "calibration.h"
#include ".\tchart\tchart.h"

class EU_TestFileV2
{
public:
	EU_TestFileV2(void);
	~EU_TestFileV2(void);

	bool open(CString strFname, ED_TestInfo* pInfo, bool bWrite);
    void addData(byte* pByte, unsigned long count);
	void close();

		
	
	//Read용 함수들
	void readTI(ED_TestInfo* pTI);
	void readTestData(CTChart* pChartCtrls, bool bRemoveOffset = false);
	void readTestData(bool bCalibrated, double* pLH, double* pRH, 
					double* pLV, double* pRV, 
					double* pLT = NULL, double* pRT = NULL,
					double* pLR = NULL, double* pRR = NULL,
					double* pAccX = NULL, double* pAccY = NULL, double* pAccZ = NULL, double* pZyroX = NULL, double* pZyroY = NULL,
					double* pStiH = NULL, double* pStiZ = NULL);

	unsigned long getTestDataCount();

	static bool updateTestInfo(CString fname, ED_TestInfo* pTI);

	void seekToData();
	void getNextData(double* pData);
	void getDataAt(double* pData, unsigned long count);
		

	CFile m_file;
private:
	bool m_bOpen;
	bool m_bWrite;
	unsigned long m_ulDataCount;
	

	ED_TestInfo* m_pTI;
	
};
