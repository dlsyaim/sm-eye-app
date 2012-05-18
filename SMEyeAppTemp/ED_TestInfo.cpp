#include "StdAfx.h"
#include ".\ed_testinfo.h"
#include "corwlDefines.h"
#include "ed_testprotocole.h"




ED_TestInfo::ED_TestInfo(void)
{
	//m_nVer 가 0일 경우 ED_TestInfo를 사용
	//m_nVer 가 2일 경우 ED_TestInfoV2 사용
	m_nVer = 3;		
	m_bBinocular = TRUE;				//monocular(FALSE), binocular(TRUE)
	m_lEyeDataCount = 0;				//eye data count
	m_nEyeFreq = FRAMERATE;				//video frame rate
	m_lAccDataCount = 0;				//acc data count
	m_nAccFreq = ACC_SAMPLERATE;		//acc sample rate

	::memset(this->m_nTestParams, 0, sizeof(int)*TEST_PARAM_NUM);	//test parameters. 사용안함
	::memset(m_chFileName, 0, MAX_FILE_NAME);						//file name
	m_calibrated = TEST_CALIBRATED;

	m_timeTime = CTime::GetCurrentTime();

	m_HV_NOT_USED = TEST_DIRECTION_H;			//bit(0): horizontal, bit(1): vertical	% 사용 안함. m_analTarget으로 사용
	
	m_showChart = VISIBLE_BIT_H | VISIBLE_BIT_V | VISIBLE_BIT_T;
    m_analMethod = ANAL_METHOD_NYSTAGMUS;
	m_analTarget = HORIZONTAL;

	this->m_stiModeCommand = ED_TestProtocole::getStiModeCommand(STI_LED);	//all led off
	this->m_stiSubmode = 0;

	m_flipImage = 1;

	m_ucPrint = 1;
//	memset(this->m_bufPrintSeries, 1, 5);
	
		
}

ED_TestInfo::~ED_TestInfo(void)
{
}


int ED_TestInfo::getTestParams(unsigned char PARAM_ID)
{
	return m_nTestParams[PARAM_ID];
}


unsigned int ED_TestInfo::getVisibleChartNum()
{
	unsigned int count = 0;
	for(int i=0; i<8; i++)
	{
		if(m_showChart & (1<<i))
			count++;
	}

	return count;
}