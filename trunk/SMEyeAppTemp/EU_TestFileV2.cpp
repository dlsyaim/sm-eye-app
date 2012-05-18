#include "StdAfx.h"
#include ".\eu_testfilev2.h"
#include ".\tchart\series.h"
#include "corwlDefines.h"
#include "ed_testprotocole.h"
#include "analyzepursuit.h"

#include ".\tchart\valuelist.h"
#include "corwlDefines.h"



EU_TestFileV2::EU_TestFileV2(void)
{
	m_bOpen = false;
	m_ulDataCount = 0;

	m_pTI = NULL;

}

EU_TestFileV2::~EU_TestFileV2(void)
{
	if(m_bOpen)
		this->close();
}

bool EU_TestFileV2::open(CString strFname, ED_TestInfo* pTI, bool bWrite)
{
	// pTI, pCalibration�� writing������ ����Ѵ�.
	bool bRtn = false;

	if(m_bOpen)
	{
		this->close();
		return false;
	}

	

	this->m_bWrite = bWrite;

	//data write������ ����
	if(bWrite)
	{
		if(!(pTI))
			::AfxMessageBox("No TI in writing test file");
		
		if(m_file.Open(strFname, CFile::modeCreate | CFile::modeWrite))
		{
			
			// test info�� ���´�.
			if(sizeof(ED_TestInfo) > TEST_INFO_SIZE)
			{
				::AfxMessageBox("Test info class is too large");
				return false;
			}

			
			byte* pBuf = new byte[TEST_INFO_SIZE];
			::memset(pBuf, 0, TEST_INFO_SIZE);
			
			//test info�� �����Ѵ�.
			::memcpy(pBuf, (byte*)pTI, sizeof(ED_TestInfo));	//�� 432
			
			m_file.Write(pBuf, TEST_INFO_SIZE);
			
			delete []pBuf;
			m_bOpen = true;

			bRtn = true;

			if(bRtn)
			{
				m_pTI = pTI;
			}
		}
	}
	//data read������ ����
	else
	{
		if(m_file.Open(strFname, CFile::modeRead))
		{			
			//this->readTI(pTI, pCalibration);
			m_bOpen = true;

			bRtn =  true;
		}

	}
	

	

	return bRtn;
}


void EU_TestFileV2::addData(byte* pBuf, unsigned long count)
{
	m_file.Write(pBuf, count);
	m_ulDataCount++;
}
void EU_TestFileV2::close()
{
	//0���� �Ǿ� �ִ� Test info�� data������ ���� ������ ������ �����ϰ� ������ �ݴ´�.

	if(this->m_bOpen)
	{
		if(this->m_bWrite)
		{
			//data ���� update
			m_pTI->m_lEyeDataCount = m_pTI->m_lAccDataCount = this->m_ulDataCount;

			ED_TestInfo ti;
			//m_lEyeDataCount�� ��ġ�� ���.
			unsigned long offset = (unsigned long)(&(ti.m_lEyeDataCount)) - (unsigned long)(&(ti));
			//�̵�
			m_file.Seek(offset, CFile::begin);
			m_file.Write(&m_ulDataCount, sizeof(unsigned long));
			//m_lAccDataCount�� ��ġ�� ���.
			offset = (unsigned long)(&(ti.m_lAccDataCount)) - (unsigned long)(&(ti));
			m_file.Seek(offset, CFile::begin);
			m_file.Write(&m_ulDataCount, sizeof(unsigned long));		
		}

		m_file.Close();

		this->m_ulDataCount = 0;
		this->m_pTI = NULL;
	}
	m_bOpen = false;
}


//Read�� �Լ���
void EU_TestFileV2::readTI(ED_TestInfo* pTI)
{
	//TEST_INFO_SIZE ��ŭ ���� �� ED_TestInfo ����� ���Ѵ�.
	byte* pBuf = new byte[TEST_INFO_SIZE];
	::memset(pBuf, 0, TEST_INFO_SIZE);
	m_file.SeekToBegin();
	m_file.Read(pBuf, TEST_INFO_SIZE);
	//test info�� �д´�.
	int n = sizeof(ED_TestInfo);


	if(pTI)
		::memcpy(pTI, pBuf, sizeof(ED_TestInfo));
	//calibration parameter�� �д´�.
	

	//double* pH = pTI->m_calibration[0].m_dHCoeff;
	//double* pV = pTI->m_calibration[1].m_dHCoeff;


	
	delete []pBuf;
}



unsigned long EU_TestFileV2::getTestDataCount()
{
	//open�� ���¿����� �Ҹ���.
	//test data�� ������ return �Ѵ�.
	if(m_bOpen)
	{
		unsigned long count = (unsigned long)((m_file.GetLength() - TEST_INFO_SIZE)/sizeof(double));
		
		return count;
	}
	else
		return -1;
}

void EU_TestFileV2::readTestData(CTChart* pChartCtrls, bool bRemoveOffset)
{
	if(m_bOpen)
	{
		ED_TestInfo ti;
		this->readTI(&ti);

		//get data count in header
		unsigned long count = ti.m_lEyeDataCount;

		// ������ ������ �´��� Ȯ���Ѵ�.
		if(count != this->getTestDataCount()/15)
		{
			::AfxMessageBox("Data count is not match");
			return;
		}
		
		
		m_file.Seek(TEST_INFO_SIZE, CFile::begin);
		double buf[15];

		double offsetH[2] = {0, 0};
		double offsetV[2] = {0, 0};

		for(unsigned long i=0; i<count; i++)
		{
			m_file.Read(buf, 15*sizeof(double));

			double pixelL[2] = {buf[0], buf[2]};
			double pixelR[2] = {buf[1], buf[3]};
			double degreeL[2];
			double degreeR[2];

			ti.m_calibration[0].pixel2degree(pixelL, degreeL);
			ti.m_calibration[1].pixel2degree(pixelR, degreeR);
			

			if((i==0) && bRemoveOffset)
			{
				offsetH[0] = degreeL[0];
				offsetH[1] = degreeR[0];
				offsetV[0] = degreeL[1];
				offsetV[1] = degreeR[1];
			}


			//horizontal
			pChartCtrls[0].Series(0).AddXY(double(i)/ti.m_nEyeFreq, degreeL[0]-offsetH[0], NULL, 0);
			pChartCtrls[0].Series(1).AddXY(double(i)/ti.m_nEyeFreq, degreeR[0]-offsetH[1], NULL, 0);
			
			//vertical
			pChartCtrls[1].Series(0).AddXY(double(i)/ti.m_nEyeFreq, degreeL[1]-offsetV[0], NULL, 0);
			pChartCtrls[1].Series(1).AddXY(double(i)/ti.m_nEyeFreq, degreeR[1]-offsetV[1], NULL, 0);

			//torsional
			pChartCtrls[2].Series(0).AddXY(double(i)/ti.m_nEyeFreq, buf[4], NULL, 0);
			pChartCtrls[2].Series(1).AddXY(double(i)/ti.m_nEyeFreq, buf[5], NULL, 0);

			//radius
			pChartCtrls[3].Series(0).AddXY(double(i)/ti.m_nEyeFreq, ti.m_calibration[0].pixel2mm(buf[6]), NULL, 0);
			pChartCtrls[3].Series(1).AddXY(double(i)/ti.m_nEyeFreq, ti.m_calibration[1].pixel2mm(buf[7]), NULL, 0);

			//ACC
			pChartCtrls[4].Series(0).AddXY(double(i)/ti.m_nEyeFreq, buf[8], NULL, 0);
			pChartCtrls[4].Series(1).AddXY(double(i)/ti.m_nEyeFreq, buf[9], NULL, 0);
			pChartCtrls[4].Series(2).AddXY(double(i)/ti.m_nEyeFreq, buf[10], NULL, 0);
			//pChartCtrls[4].Series(3).AddXY(double(i)/ti.m_nEyeFreq, buf[11]/360., NULL, 0);
			//pChartCtrls[4].Series(4).AddXY(double(i)/ti.m_nEyeFreq, buf[12]/360., NULL, 0);

			//stimulation
			pChartCtrls[0].Series(2).AddXY(double(i)/ti.m_nEyeFreq, buf[13], NULL, 0);
			pChartCtrls[1].Series(2).AddXY(double(i)/ti.m_nEyeFreq, buf[14], NULL, 0);


		}

		//pursuit�� target�� rectagular shape�� ����Ǿ� �ִ�.
		// sine wave�� �ٲپ��ش�.
		if(ti.m_stiModeCommand == 0x03)	//pursuit���
		{
			double* pTarget = new double[count];
			ZeroMemory(pTarget, sizeof(double)*count);

			//target series�� ��´�.
			//CSeries series =  pChartCtrls[ti.m_HV ? 1 : 0].Series(2);
			CSeries series =  pChartCtrls[ti.m_analTarget].Series(2);

			//�����͸� series�κ��� �޾ƿ´�.
			for(unsigned long i=0; i<count; i++)
				pTarget[i] = series.GetYValues().GetValue(i);

			//rect shape�� sine wave�� �ٲ۴�.
			double amp = ED_TestProtocole::getStiValue(ti.m_stiModeCommand, ti.m_stiSubmode);
			CAnalyzePursuit ::reshapeTarget(pTarget, count, amp);

			//series�� �ٽ� �ִ´�.
			series.Clear();
			for(unsigned long i=0; i<count; i++)
				series.AddXY(double(i)/ti.m_nEyeFreq, pTarget[i], NULL, 0);
		}


	/*	//EOG ������ ������ �׷��ش�.
		CString strEOGFname = m_file.GetFilePath();
		strEOGFname.Replace(".emd", ".eog");
		CFile fileEOG;
		if(fileEOG.Open(strEOGFname, CFile::modeRead))
		{
			unsigned long eogCount = fileEOG.GetLength()/12;
			double fs = FRAMERATE*eogCount/count;

			unsigned char buf[12];
			double eog[4];
			long temp;
			for(int i=0; i<eogCount; i++)
			{
				fileEOG.Read(buf, 12);
				for(int j=0; j<4; j++)
				{
					temp = (buf[j*3]<<16) + (buf[j*3 +1]<<8) + buf[j*3 +2];
								
					
					if(temp > 0x7fffff)
						temp = -1*(0xffffff - temp)-1 ;

					eog[j] = temp*EOG_GAIN;					
				
				}

				//horizontal
				pChartCtrls[0].Series(SERIES_LEFT_EOG).AddXY(double(i)/fs, eog[0], NULL, 0);
				pChartCtrls[0].Series(SERIES_RIGHT_EOG).AddXY(double(i)/fs, eog[1], NULL, 0);
				
				//vertical
				pChartCtrls[1].Series(SERIES_LEFT_EOG).AddXY(double(i)/fs, eog[2], NULL, 0);
				pChartCtrls[1].Series(SERIES_RIGHT_EOG).AddXY(double(i)/fs, eog[3], NULL, 0);

			}
		}
*/
		for(int i=0; i<CHART_NUM; i++)
			pChartCtrls[i].Repaint();
	}
}

void EU_TestFileV2::readTestData(bool bCalibrated, double* pLH, double* pRH, 
					double* pLV, double* pRV, 
					double* pLT, double* pRT,
					double* pLR, double* pRR,
					double* pAccX, double* pAccY, double* pAccZ, double* pZyroX, double* pZyroY,
					double* pStiH, double* pStiV)
{
	// bCalibrated true�� ��� calibration �Ͽ� ����
	if(m_bOpen)
	{
		ED_TestInfo ti;
		this->readTI(&ti);

		//get data count in header
		unsigned long count = ti.m_lEyeDataCount;

		// ������ ������ �´��� Ȯ���Ѵ�.
		if(count != this->getTestDataCount()/15)
		{
			::AfxMessageBox("Data count is not match");
			return;
		}
		
		
		m_file.Seek(TEST_INFO_SIZE, CFile::begin);
		double buf[15];

		for(unsigned long i=0; i<count; i++)
		{
			m_file.Read(buf, 15*sizeof(double));

			double degreeL[2];
			double degreeR[2];
			if(bCalibrated)
			{
				double pixelL[2] = {buf[0], buf[2]};
				double pixelR[2] = {buf[1], buf[3]};
				ti.m_calibration[0].pixel2degree(pixelL, degreeL);
				ti.m_calibration[1].pixel2degree(pixelR, degreeR);
			}
			else
			{
				degreeL[0] = buf[0]; degreeL[1] =  buf[2];
				degreeR[0] = buf[1]; degreeR[1] =  buf[3];
			}
			
			//horizontal
			if(pLH) pLH[i] = degreeL[0];
			if(pRH)	pRH[i] = degreeR[0];
						
			//vertical
			if(pLV) pLV[i] = degreeL[1];
			if(pRV) pRV[i] = degreeR[1];
			
			//torsional
			if(pLT) pLT[i] = buf[4];
			if(pRT) pRT[i] = buf[5];
			
			//radius
			if(pLR)pLR[i] = buf[6];
			if(pRR) pRR[i] = buf[7];

			//acc
			if(pAccX) pAccX[i] = buf[8];
			if(pAccY) pAccY[i] = buf[9];
			if(pAccZ) pAccZ[i] = buf[10];
			if(pZyroX) pZyroX[i] = buf[11];
			if(pZyroY) pZyroY[i] = buf[12];
			
			//stimulation
			if(pStiH) pStiH[i] = buf[13];
			if(pStiV) pStiV[i] = buf[14];
		}
			

		
		//pursuit�� target�� rectagular shape�� ����Ǿ� �ִ�.
		// sine wave�� �ٲپ��ش�.
		if(ti.m_stiModeCommand == 0x03)	//pursuit���
		{
			double amp = ED_TestProtocole::getStiValue(ti.m_stiModeCommand, ti.m_stiSubmode);
			//if(!ti.m_HV)
			if(ti.m_analTarget == HORIZONTAL)
				CAnalyzePursuit ::reshapeTarget(pStiH, count, amp);
			else
				CAnalyzePursuit ::reshapeTarget(pStiV, count, amp);
		}

	}
}




bool EU_TestFileV2::updateTestInfo(CString fname, ED_TestInfo* pTI)
{
	//������ �ִ��� Ȯ���Ѵ�.
	CFileStatus fs;
	bool bRtn = false;
	if(CFile::GetStatus(fname,fs))
	{
		//����
		CFile file;
		if(file.Open(fname, CFile::modeReadWrite))
		{
			byte* pBuf = new byte[TEST_INFO_SIZE];
			::ZeroMemory(pBuf, TEST_INFO_SIZE);
			if(pTI)
				::memcpy(pBuf, pTI, sizeof(ED_TestInfo));

			file.SeekToBegin();
			file.Write(pBuf, TEST_INFO_SIZE);
			//test info�� �д´�.
					
			delete []pBuf;
			file.Close();
			bRtn = true;
		}
	}

	return bRtn;
}

void EU_TestFileV2::seekToData()
{
	if(this->m_bOpen)
		m_file.Seek(TEST_INFO_SIZE, CFile::begin);
}

void EU_TestFileV2::getNextData(double* pData)
{
	if(this->m_bOpen)
		m_file.Read(pData, sizeof(double)*15);
}

void EU_TestFileV2::getDataAt(double* pData, unsigned long count)
{
	if(this->m_bOpen)
	{
		m_file.Seek(TEST_INFO_SIZE, CFile::begin);
		m_file.Seek(sizeof(double)*15, CFile::current);
		m_file.Read(pData, sizeof(double)*15);
	}
}
								
