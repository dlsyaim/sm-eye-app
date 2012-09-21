#pragma once
#include "ed_patientinfo.h"
#include "ed_testinfo.h"
#include "ed_testdatabinocular.h"

class EU_TestFile
{
public:
	EU_TestFile(void);
	~EU_TestFile(void);

protected:
	CFile m_file;
	BOOL m_bOpen;
	CString m_strFname;

public:
	bool open(CString strFname, bool bWrite);
	void close();

	//Write�� �Լ���
	bool writeTestInfo(ED_TestInfo* pTI);
	//void writeTestData(ED_TestDataBinocular* pDataBinocular);
	void writeTestData(CTChart* pChartCtrls);
	//void writeTestData(CFile* pFile, unsigned long count);

	//Read�� �Լ���
	bool readTI(ED_TestInfo* pTI);
	//bool readTestData(ED_TestDataBinocular* pDataBinocular, long count);
	bool readTestData(CTChart* pChartCtrls, unsigned long eyeCount, unsigned long accCount);

	unsigned long getTestDataCount();
	
};
