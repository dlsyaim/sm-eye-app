#include "StdAfx.h"
#include ".\eu_testfile.h"
#include ".\tchart\valuelist.h"
#include "corwlUtil.h"







EU_TestFile::EU_TestFile(void)
{
}

EU_TestFile::~EU_TestFile(void)
{
}

bool EU_TestFile::open(CString strFname, bool bWrite)
{
	
	if(bWrite)
		m_bOpen = m_file.Open(strFname, CFile::modeWrite | CFile::modeCreate);
	else
		m_bOpen = m_file.Open(strFname, CFile::modeRead);

	if(m_bOpen)
		this->m_strFname = strFname;
	
	return m_bOpen ? true : false;
}
void EU_TestFile::close()
{
	m_file.Close();
	m_bOpen = false;
	m_strFname = "";

}

//Write용 함수들
bool EU_TestFile::writeTestInfo(ED_TestInfo* pTI)
{
	if(m_bOpen)
	{
		if(sizeof(ED_TestInfo) > TEST_INFO_SIZE)
		{
			::AfxMessageBox("Test info class is too large");
			return false;
		}

		m_file.SeekToBegin();
		
		//test info를 저장한다.
		byte* pBuf = new byte[TEST_INFO_SIZE];
		::memset(pBuf, 0, TEST_INFO_SIZE);
		::memcpy(pBuf, (byte*)pTI, sizeof(ED_TestInfo));
		m_file.Write(pBuf, TEST_INFO_SIZE);
		delete []pBuf;
		

		return true;
	}
	return false;
}

/*
void EU_TestFile::writeTestData(ED_TestDataBinocular* pDataBinocular)
{
	//이 함수는 반드시 header를 쓴 후에 불러야 한다.
	if(m_bOpen)
	{
		m_file.Seek(TEST_INFO_SIZE, CFile::begin);
		pDataBinocular->writeData(&m_file);
	}
}*/

void writeChartToFile(CTChart* pChartCtrl, CFile* pFile, int seriesNum);
void EU_TestFile::writeTestData(CTChart* pChartCtrls)
{
	if(m_bOpen)
	{
		m_file.Seek(TEST_INFO_SIZE, CFile::begin);
		
		unsigned long count = 0;
		for(int i=0; i<CHART_ACC; i++)
			writeChartToFile(&(pChartCtrls[i]), &m_file, i==CHART_ACC ? 5 : 2);

	}
}

/*
void EU_TestFile::writeTestData(CFile* pTempFile, unsigned long count)
{
	if(m_bOpen && count)
	{
		//info header만큼 이동한 후에 data를 저장한다.
		m_file.Seek(TEST_INFO_SIZE, CFile::begin);

		//temporary data의 처음으로 이동
		pTempFile->SeekToBegin();

		double* pData = new double[count];

		for(int i=0; i<13; i++)
		{
			pTempFile->Seek(i*sizeof(double), CFile::begin);
            for(unsigned long idx = 0; idx<count; idx++)
			{
				pTempFile->Read(&(pData[idx]), sizeof(double));
				pTempFile->Seek(sizeof(double)*13, CFile::current);
			}

			this->m_file.Write(pData, sizeof(double)*count);
		}

		delete []pData;

	}
		
}
*/


void writeChartToFile(CTChart* pChartCtrl, CFile* pFile, int seriesNum)
{
	for(int i=0; i<seriesNum; i++)
	{
		/*double* pBuf = new double[pChartCtrl->Series(i).GetCount()];
        unsigned long count = ::corGetChartValue(pChartCtrl, i, false, pBuf);
	
		pFile->Write(pBuf, sizeof(double)*count);
		delete []pBuf;*/
		CValueList vList = pChartCtrl->Series(i).GetYValues();
		unsigned long count = vList.GetCount();
		double v = 0;
		for(unsigned long idx = 0; idx<count; idx++)
		{
			v = vList.GetValue(idx);
			pFile->Write(&v, sizeof(double));
		}
	}
}

//Read용 함수들
bool EU_TestFile::readTI(ED_TestInfo* pTI)
{
	if(m_bOpen && pTI)
	{
		//TEST_INFO_SIZE 만큼 읽은 후 ED_TestInfo 싸이즈만 취한다.
		byte buf[TEST_INFO_SIZE];
		m_file.SeekToBegin();
		m_file.Read(buf, TEST_INFO_SIZE);
		::memcpy(pTI, buf, sizeof(ED_TestInfo));

		return true;
	}
	return false;
}

/*
bool EU_TestFile::readTestData(ED_TestDataBinocular* pDataBinocular, long count)
{
	if(m_bOpen && pDataBinocular)
	{
		m_file.Seek(TEST_INFO_SIZE , CFile::begin);
		pDataBinocular->readData(&m_file, count);

		return true;
	}
	return false;
}*/

void readChartFromFile(CTChart* pChartCtrl, CFile* pFile, int seriesNum, unsigned long count, double dx);
bool EU_TestFile::readTestData(CTChart* pChartCtrls, unsigned long eyeCount, unsigned long accCount)
{
	if(m_bOpen)
	{
		m_file.Seek(TEST_INFO_SIZE, CFile::begin);
		for(int i=0; i<CHART_ACC; i++)
			if(i != CHART_ACC )
				readChartFromFile(&(pChartCtrls[i]), &m_file, 2, eyeCount, 1./double(FRAMERATE));
			else
                readChartFromFile(&(pChartCtrls[i]), &m_file, 5, accCount, 1./double(FRAMERATE));
		return true;
	}
	return false;
}


void readChartFromFile(CTChart* pChartCtrl, CFile* pFile, int seriesNum, unsigned long count, double dx)
{
	
	double* pBuf = new double[count];
	for(int i=0; i<seriesNum; i++)
	{
		pFile->Read(pBuf, sizeof(double)*count);		
		::corSetChartValue(pChartCtrl, i, pBuf, count, dx);
	}
	delete []pBuf;
		
}

unsigned long EU_TestFile::getTestDataCount()
{
	//open된 상태에서만 불린다.
	//test data의 개수를 return 한다.
	if(m_bOpen)
	{
		unsigned long count = (unsigned long)((m_file.GetLength() - TEST_INFO_SIZE)/sizeof(double));
		
		return count;
	}
	else
		return -1;
}
