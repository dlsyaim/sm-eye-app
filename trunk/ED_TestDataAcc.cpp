#include "StdAfx.h"
#include ".\ed_testdataacc.h"
#include "corwlUtil.h"
#include "corwldefines.h"
#include ".\tchart\valuelist.h"

ED_TestDataAcc::ED_TestDataAcc(void)
{
	m_lDataCount = 0;
	m_pdAccX = m_pdAccY = m_pdAccZ = m_pdZyroX = m_pdZyroY = NULL;
	init();
}

ED_TestDataAcc::~ED_TestDataAcc(void)
{
	init();
}

void ED_TestDataAcc::init()
{
	m_lDataCount = 0;

	DEL(m_pdAccX);
	DEL(m_pdAccY);
	DEL(m_pdAccZ);
	DEL(m_pdZyroX);
	DEL(m_pdZyroY);
}

void ED_TestDataAcc::alloc(long count)
{
	//init()을 부른 후 호출해야 한다.
	m_lDataCount = count;

	m_pdAccX = new double[count];
	m_pdAccY = new double[count];
	m_pdAccZ = new double[count];
	m_pdZyroX = new double[count];
	m_pdZyroY = new double[count];
}
void ED_TestDataAcc::writeData(CFile* pFile)
{
	pFile->Write(m_pdAccX, sizeof(double)*m_lDataCount);
	pFile->Write(m_pdAccY, sizeof(double)*m_lDataCount);
	pFile->Write(m_pdAccZ, sizeof(double)*m_lDataCount);
	pFile->Write(m_pdZyroX, sizeof(double)*m_lDataCount);
	pFile->Write(m_pdZyroY, sizeof(double)*m_lDataCount);
}

void ED_TestDataAcc::readData(CFile* pFile, long count)
{
	init();
	m_lDataCount = count;

	if(count)
	{
		this->alloc(count);
	
		pFile->Read(m_pdAccX, sizeof(double)*m_lDataCount);
		pFile->Read(m_pdAccY, sizeof(double)*m_lDataCount);
		pFile->Read(m_pdAccZ, sizeof(double)*m_lDataCount);
		pFile->Read(m_pdZyroX, sizeof(double)*m_lDataCount);
		pFile->Read(m_pdZyroY, sizeof(double)*m_lDataCount);
	}
}

void ED_TestDataAcc::setData(CSeries* pAccX, CSeries* pAccY, CSeries* pAccZ, CSeries* pZyroX, CSeries* pZyroY)
{
	init();
	m_lDataCount = pAccX->GetCount();

	m_lDataCount = pAccZ->GetCount();
	m_lDataCount = pZyroX->GetCount();
		
	if(m_lDataCount)
	{
		this->alloc(m_lDataCount);

		for(long i=0; i<m_lDataCount; i++)
		{
			m_pdAccX[i] = pAccX->GetYValues().GetValue(i);
			m_pdAccY[i] = pAccY->GetYValues().GetValue(i);
			m_pdAccZ[i] = pAccZ->GetYValues().GetValue(i);
			m_pdZyroX[i] = pZyroX->GetYValues().GetValue(i);
			m_pdZyroY[i] = pZyroY->GetYValues().GetValue(i);
		}
	}

}
void ED_TestDataAcc::getData(CSeries* pAccX, CSeries* pAccY, CSeries* pAccZ, CSeries* pZyroX, CSeries* pZyroY, int freq)
{
	if(!pAccX || !pAccY || !pAccZ || !pZyroX || !pZyroY)
	{
		::AfxMessageBox("error in ED_TestDataAcc::getData pointer assertion!!");
		return;
	}
	

	for(long i=0; i<m_lDataCount; i++)
	{
		pAccX->AddXY(double(i)/double(freq), m_pdAccX[i], NULL, 0);
		pAccY->AddXY(double(i)/double(freq), m_pdAccY[i], NULL, 0);
		pAccZ->AddXY(double(i)/double(freq), m_pdAccZ[i], NULL, 0);
		pZyroX->AddXY(double(i)/double(freq), m_pdZyroX[i], NULL, 0);
		pZyroY->AddXY(double(i)/double(freq), m_pdZyroY[i], NULL, 0);
	}
}


