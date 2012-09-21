#pragma once
#include "ED_testdatamonocular.h"
#include "ED_testdataAcc.h"
#include "ED_TestInfo.h"
#include "corwlDefines.h"


#include ".\tchart\tchart.h"
//#include ".\tchart\series.h"

class ED_TestDataBinocular
{
public:
	ED_TestDataBinocular(void);
	~ED_TestDataBinocular(void);

	void writeData(CFile* pFile);
	void readData(CFile* pFile, long count);


	//void setData(ED_TestDataMonocular left, ED_TestDataMonocular right);
	void setData(CTChart* pChartCtrls);	//��Ʈ�κ��� �����͸� �����´�.
	void getData(CTChart* pChartCtrls);	//��Ʈ�� �����͸� �Ѹ���.
	ED_TestInfo* getTestInfo();
	
	

private:
	ED_TestInfo m_TestInfo;
	ED_TestDataMonocular m_monocularData[2];
	ED_TestDataAcc m_accData;
	
};
