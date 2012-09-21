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
	void setData(CTChart* pChartCtrls);	//차트로부터 데이터를 가져온다.
	void getData(CTChart* pChartCtrls);	//차트에 데이터를 뿌린다.
	ED_TestInfo* getTestInfo();
	
	

private:
	ED_TestInfo m_TestInfo;
	ED_TestDataMonocular m_monocularData[2];
	ED_TestDataAcc m_accData;
	
};
