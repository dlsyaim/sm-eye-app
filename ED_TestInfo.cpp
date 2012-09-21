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
	
	//reset event string
	::ZeroMemory(this->m_strEvent, EVENT_STRING_SIZE*EVENT_STRING_COUNT);

	CString strEvents[EVENT_STRING_COUNT];
	for(int i=0; i<EVENT_STRING_COUNT; i++)
		strEvents[i].Format("Event %d", i+1);

	this->setEventString(strEvents);
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


void ED_TestInfo::setEventString(CString* strEvents)
{
	//init strings
	::ZeroMemory(m_strEvent, EVENT_STRING_SIZE*EVENT_STRING_COUNT);
	//copy strings
	// maximum length of strin is EVENT_STRING_SIZE
	for(int i=0; i<EVENT_STRING_COUNT; i++)
		memcpy(m_strEvent + i*EVENT_STRING_SIZE, 
					strEvents[i].GetBuffer(), 
					min(EVENT_STRING_SIZE-1, strEvents[i].GetLength()));
	
}

CString ED_TestInfo::getEventString(int idx)
{
	if(idx >= EVENT_STRING_COUNT)
		return CString("");

	char buf[EVENT_STRING_SIZE+1];
	::ZeroMemory(buf, EVENT_STRING_SIZE+1);

	memcpy(buf, m_strEvent + idx*EVENT_STRING_SIZE, EVENT_STRING_SIZE);
	CString str;
	str.Format("%s", buf);

	//저장된 string이 없으면 Event # 을 만들어 보낸다.
	if(str == "")
		str.Format("Event %d", idx+1);

	return str;
}