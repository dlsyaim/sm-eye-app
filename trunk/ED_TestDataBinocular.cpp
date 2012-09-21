#include "StdAfx.h"
#include ".\ed_testdatabinocular.h"

ED_TestDataBinocular::ED_TestDataBinocular(void)
{
}

ED_TestDataBinocular::~ED_TestDataBinocular(void)
{
}

/*
void ED_TestDataBinocular::setData(ED_TestDataMonocular left, ED_TestDataMonocular right)
{
	m_monocularData[LEFT_EYE] = left;
	m_monocularData[RIGHT_EYE] = right;
}*/

void ED_TestDataBinocular::writeData(CFile* pFile)
{
	for(int i=0; i<2; i++)
		m_monocularData[i].writeData(pFile);

	this->m_accData.writeData(pFile);
}

void ED_TestDataBinocular::readData(CFile* pFile, long count)
{
	for(int i=0; i<2; i++)
		m_monocularData[i].readData(pFile, count);

	m_accData.readData(pFile, count);
}

ED_TestInfo* ED_TestDataBinocular::getTestInfo()
{
	return &m_TestInfo;
}

void ED_TestDataBinocular::setData(CTChart* pChart)
{
	
	if(!pChart)
	{
		::AfxMessageBox("error in ED_TestDataBinocular::setData pointer assertion!!");
		return;
	}

	if(!pChart[0].Series(0).GetCount())
	{
		::AfxMessageBox("엄서엄서 저장할 데이터가 엄서");
		return;
	}
	//eye data
	for(int i=0; i<2; i++)
		m_monocularData[i].setData(&(pChart[0].Series(i)), 
									&(pChart[1].Series(i)), 
									&(pChart[2].Series(i)), 
									&(pChart[3].Series(i)));
	//acc data
	this->m_accData.setData(&(pChart[4].Series(0)), 
							&(pChart[4].Series(1)), 
							&(pChart[4].Series(2)), 
							&(pChart[4].Series(3)), 
							&(pChart[4].Series(4)));
}

void ED_TestDataBinocular::getData(CTChart* pChart)
{
	
	ASSERT(pChart);
	
	//eye data
	for(int i=0; i<2; i++)
		m_monocularData[i].getData(&(pChart[0].Series(i)), 
									&(pChart[1].Series(i)), 
									&(pChart[2].Series(i)), 
									&(pChart[3].Series(i)), this->m_TestInfo.m_nEyeFreq);

	//acc data
	this->m_accData.getData(&(pChart[4].Series(0)), 
							&(pChart[4].Series(1)), 
							&(pChart[4].Series(2)), 
							&(pChart[4].Series(3)), 
							&(pChart[4].Series(4)), m_TestInfo.m_nEyeFreq);

}