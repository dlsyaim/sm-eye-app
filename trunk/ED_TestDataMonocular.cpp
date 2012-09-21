#include "StdAfx.h"
#include ".\ed_testdatamonocular.h"
#include "corwlUtil.h"
#include "corwldefines.h"
#include ".\tchart\series.h"
#include ".\tchart\valuelist.h"

ED_TestDataMonocular::ED_TestDataMonocular(void)
{
	m_pdCx = m_pdCy = m_pdRadius = m_pdTorsion = NULL;
	m_pbPupilValid = m_pbTorsionValid = NULL;

	m_pSeries[0] = m_pSeries[1] = m_pSeries[2] = m_pSeries[3] = NULL;

	init();
}

ED_TestDataMonocular::~ED_TestDataMonocular(void)
{
	init();
}

ED_TestDataMonocular& ED_TestDataMonocular::operator=(const ED_TestDataMonocular &rhs)
{
	if(this != &rhs)
		this->setData(rhs.m_pdCx, rhs.m_pdCy, rhs.m_pdRadius, rhs.m_pdTorsion, rhs.m_pbPupilValid, rhs.m_pbTorsionValid, rhs.m_lDataCount);
	return *this;
}
void ED_TestDataMonocular::init()
{
	m_lDataCount = 0;

	DEL(m_pdCx);
	DEL(m_pdCy);
	DEL(m_pdRadius);
	DEL(m_pdTorsion);
	DEL(m_pbPupilValid);
	DEL(m_pbTorsionValid);
}

void ED_TestDataMonocular::alloc(long count)
{
	m_lDataCount = count;

	m_pdCx = new double[count];
	m_pdCy = new double[count];
	m_pdRadius = new double[count];
	m_pdTorsion = new double[count];
	m_pbPupilValid = new bool[count];
	m_pbTorsionValid = new bool[count];
}

void ED_TestDataMonocular::setData(double* pCx, double* pCy, double* pRadius, double* pTorsion, bool* pPupilValid, bool* pTorsionValid, long count)
{
	init();
	m_lDataCount = count;

	if(count)
	{
		this->alloc(count);

		::memcpy(m_pdCx, pCx, sizeof(double)*count);
		::memcpy(m_pdCy, pCy, sizeof(double)*count);
		::memcpy(m_pdRadius, pRadius, sizeof(double)*count);
		::memcpy(m_pdTorsion, pTorsion, sizeof(double)*count);
		::memcpy(m_pbPupilValid, pPupilValid, sizeof(bool)*count);
		::memcpy(m_pbTorsionValid, pTorsionValid, sizeof(bool)*count);    
	}
}

void ED_TestDataMonocular::setData(CSeries* pSeriesCx, CSeries* pSeriesCy, CSeries* pSeriesR, CSeries* pSeriesT)
{
	init();
	this->m_lDataCount = pSeriesCx->GetCount();
	
	if(m_lDataCount)
	{
		this->alloc(m_lDataCount);

		for(long i=0; i<m_lDataCount; i++)
		{
			m_pdCx[i] = pSeriesCx->GetYValues().GetValue(i);
			m_pdCy[i] = pSeriesCy->GetYValues().GetValue(i);
			m_pdRadius[i] = pSeriesR->GetYValues().GetValue(i);
			m_pdTorsion[i] = pSeriesT->GetYValues().GetValue(i);
	        
			m_pbPupilValid[i] =  m_pdCx[i] ? true : false;
			m_pbTorsionValid[i] = true;
		}
		
	}
	else
		::AfxMessageBox("저장할 데이터가 엄서~!");
}

void ED_TestDataMonocular::getData(CSeries* pSeriesCx, CSeries* pSeriesCy, CSeries* pSeriesR, CSeries* pSeriesT, int freq)
{
	if(!pSeriesCx || !pSeriesCy || !pSeriesR || !pSeriesT)
	{
		::AfxMessageBox("error in ED_TestDataMonocular::getData pointer assertion!!");
		return;
	}
	
	ASSERT(pSeriesCx);
	ASSERT(pSeriesCy);
	ASSERT(pSeriesR);
	ASSERT(pSeriesT);
	
	for(int i=0; i<this->m_lDataCount; i++)
	{
		if(this->m_pbPupilValid[i])
		{
			pSeriesCx->AddXY(double(i)/double(freq), this->m_pdCx[i], NULL, 0);
			pSeriesCy->AddXY(double(i)/double(freq), this->m_pdCy[i], NULL, 0);
			pSeriesR->AddXY(double(i)/double(freq), this->m_pdRadius[i], NULL, 0);
		}
		else
		{
			pSeriesCx->AddXY(double(i)/double(freq), 0, NULL, 0);
			pSeriesCy->AddXY(double(i)/double(freq), 0, NULL, 0);
			pSeriesR->AddXY(double(i)/double(freq), 0, NULL, 0);
		}

		if(this->m_pbTorsionValid[i])
			pSeriesT->AddXY(double(i)/double(freq), this->m_pdTorsion[i], NULL, 0);
		else
			pSeriesT->AddXY(double(i)/double(freq), 0, NULL, 0);
		

		
	}
}

void ED_TestDataMonocular::writeData(CFile* pFile)
{
	pFile->Write(m_pdCx, sizeof(double)*m_lDataCount);
	pFile->Write(m_pdCy, sizeof(double)*m_lDataCount);
	pFile->Write(m_pdRadius, sizeof(double)*m_lDataCount);
	pFile->Write(m_pdTorsion, sizeof(double)*m_lDataCount);
	pFile->Write(m_pbPupilValid, sizeof(bool)*m_lDataCount);
	pFile->Write(m_pbTorsionValid, sizeof(bool)*m_lDataCount);
}

void ED_TestDataMonocular::readData(CFile* pFile, long count)
{
	init();
	m_lDataCount = count;

	if(count)
	{
		m_pdCx = new double[count];
		m_pdCy = new double[count];
		m_pdRadius = new double[count];
		m_pdTorsion = new double[count];
		m_pbPupilValid = new bool[count];
		m_pbTorsionValid = new bool[count];

		
		pFile->Read(m_pdCx, sizeof(double)*m_lDataCount);
		pFile->Read(m_pdCy, sizeof(double)*m_lDataCount);
		pFile->Read(m_pdRadius, sizeof(double)*m_lDataCount);
		pFile->Read(m_pdTorsion, sizeof(double)*m_lDataCount);
		pFile->Read(m_pbPupilValid, sizeof(bool)*m_lDataCount);
		pFile->Read(m_pbTorsionValid, sizeof(bool)*m_lDataCount);
	}
}