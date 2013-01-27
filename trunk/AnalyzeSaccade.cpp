#include "StdAfx.h"
#include ".\analyzesaccade.h"
#include "corwlDefines.h"
#include <math.h>

CAnalyzeSaccade::CAnalyzeSaccade(void)
{
	
}

CAnalyzeSaccade::~CAnalyzeSaccade(void)
{
	this->deleteAllSaccade();
}

void CAnalyzeSaccade::deleteAllSaccade()
{
	if(!m_listSaccade.IsEmpty())
	{
		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structSaccade* pSac = m_listSaccade.GetNext(pos1);
			delete pSac;
		}

		this->m_listSaccade.RemoveAll();
	}
}

bool getSaccadeParam(double* pEye, unsigned long startIdx, unsigned long endIdx, structSaccade* pSaccade);
void CAnalyzeSaccade::analyze(double* pEye, double* pTarget, unsigned long count)
{
	deleteAllSaccade();
	
	//int a = m_listSaccade.GetCount();

	//saccade ������ ���Ѵ�.
	for(unsigned long i=FRAMERATE; i<count; i++)
	{
		if(pTarget[i-1] != pTarget[i])
		{
			structSaccade* pSac = new structSaccade;
			pSac->targetLeap = pTarget[i]-pTarget[i-1];
			pSac->startIdx = i-1;
			if(getSaccadeParam(pEye, i, min(i+MS1000, count-1), pSac))	//saccade���� 500ms������
			{
				//saccade�� ã�������� list�� �߰��Ѵ�.
				this->m_listSaccade.AddTail(pSac);		
			}
			else
				delete pSac;
		}
			
	}
}



bool getSaccadeParam(double* pEye, unsigned long startIdx, unsigned long endIdx, structSaccade* pSaccade)
{
	bool bRtn = true;

	///////////////////////////////////////////////
    //find max velocity;
	unsigned char M = 3;	//velocity�� ���ϱ� ���� M (1000/120*M ms)
	//unsigned char M = 4;	//velocity�� ���ϱ� ���� M (1000/120*M ms)
	unsigned long peakIdx = 0;
	double maxDiff =  getMaxAbsDiff(pEye, startIdx, endIdx, &peakIdx, M);
	pSaccade->peakVel =maxDiff/(double(M)/FRAMERATE);	//degree/sec

	//������ �´��� Ȯ���Ѵ�.
	if(pSaccade->peakVel*pSaccade->targetLeap < 0)
		bRtn = false;

	

	/////////////////////////////////////////////////////
	//find latency
	// peakIdx�κ��� backward search�Ͽ� maxDiff�� 10%���� �۾����� ���� onset���� ����.
	// startIdx�� target�� ������. ������ ���̰� latency
	unsigned long onsetIdx = 0;
	for(unsigned long i=peakIdx; i>startIdx+3; i--)
	{
		if(fabs(pEye[i]-pEye[i-M]) < 0.1*fabs(maxDiff))
		{
			onsetIdx = i-(M>>1)-1;
			break;
		}
	}
	//latency�� ã�������� Ȯ��
	if(onsetIdx == 0) 
		bRtn = false;
	else
        pSaccade->latency = (onsetIdx-startIdx+1);// idx ����*(1000.0/FRAMERATE);	//ms
	if(pSaccade->latency > FRAMERATE)	//latency�� 1�ʺ��� ũ�ٸ� false
		bRtn = false;

	////////////////////////////////////////////////
	//find Accuracy
	//peakIdx�κ��� forward search�Ͽ� maxDiff�� 10%���� �۾����� ����offset���� ���� 
	// onset~offset�� leap�� targetLeap�� ���Ѵ�.
	unsigned long offsetIdx = 0;
	for(unsigned long i=peakIdx+3; i<endIdx; i++)
	{
		if(fabs(pEye[i]-pEye[i-M]) < 0.1*fabs(maxDiff))
		{
			offsetIdx = i-(M>>1)-1;
			break;
		}
	}
	//offset�� ã�������� Ȯ��
	if(offsetIdx == 0)
		bRtn = false;
	else
	{
		double eyeLeap = pEye[offsetIdx] - pEye[onsetIdx];
		pSaccade->accuracy = eyeLeap/pSaccade->targetLeap*100;
		pSaccade->endIdx = offsetIdx;
		//if(fabs(pSaccade->accuracy-100) > 50)
		//	bRtn = false;
	}

	return bRtn;
}

double getMaxAbsDiff(double* pDat, unsigned long startIdx, unsigned long endIdx, unsigned long* pMaxIdx, unsigned char M)
{
	//M�� rag�� ���� diff value�� ���밪 max���� �����Ѵ�.
	//���ϰ��� abs���� �ƴϴ�.
	
	double maxVal = 0;
	unsigned long maxIdx = 0;

	for(unsigned long i=startIdx+M; i<=endIdx; i++)
	{
		if( fabs(pDat[i]-pDat[i-M]) > fabs(maxVal))
		{
			maxVal = pDat[i]-pDat[i-M];
			maxIdx = i-(M>>1);
		}
	}

	if(pMaxIdx)
		*pMaxIdx = maxIdx;

	return maxVal;
}

CString CAnalyzeSaccade::getResultString()
{
	CString str;

	return str;
}

bool CAnalyzeSaccade::load(CString fname)
{
	bool bRtn = false;
	CFile file;
	if(file.Open(fname, CFile::modeRead))
	{
		//���� DATA�� ��� �����.
		this->deleteAllSaccade();

		unsigned char buf[256];
		::ZeroMemory(buf, 256);

		unsigned int count = (unsigned int)(file.GetLength()/256);
		for(unsigned int i=0; i<count; i++)
		{
			structSaccade* pSaccade = new structSaccade;
			file.Read(buf, 256);
			::memcpy(pSaccade, buf, sizeof(structSaccade));

			this->m_listSaccade.AddTail(pSaccade);
			
		}

		file.Close();
		bRtn = true;
	}

	return bRtn;
}

bool CAnalyzeSaccade::save(CString fname)
{
	int n = sizeof(structSaccade);

	bool bRtn = false;
	CFile file;
	if(file.Open(fname, CFile::modeWrite | CFile::modeCreate))
	{
		unsigned char buf[256];
		::ZeroMemory(buf, 256);

		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structSaccade* pSaccade = m_listSaccade.GetNext(pos1);
			memcpy(buf, pSaccade, sizeof(structSaccade));
			file.Write(buf, 256);
		}

		file.Close();
		bRtn = true;
	}

	return bRtn;
}


unsigned int CAnalyzeSaccade::getCount()
{
	return (unsigned int)(this->m_listSaccade.GetCount());
}
		
void CAnalyzeSaccade::getPeakVelocity(double& minVal, double& maxVal, double& meanVal)
{
	minVal = 9999999999;
	maxVal = 0;
	meanVal = 0;

	double sum = 0;

	POSITION pos1, pos2;
	double val = 0;
	for(pos1 = this->m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; )
	{
		structSaccade* pSsac = m_listSaccade.GetNext(pos1);
		val = fabs(pSsac->peakVel);
		
		sum += val;

		if(val > maxVal)
			maxVal = val;
		if(val < minVal)
			minVal = val;
	}
 
	if(m_listSaccade.GetCount())
		meanVal = sum/m_listSaccade.GetCount();

}

void CAnalyzeSaccade::getLatency(double& minVal, double& maxVal, double& meanVal)
{
	minVal = 9999999999;
	maxVal = -9999999999;
	meanVal = 0;

	double sum = 0;

	POSITION pos1, pos2;
	double val = 0;
	for(pos1 = this->m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; )
	{
		structSaccade* pSsac = m_listSaccade.GetNext(pos1);
		val = pSsac->latency*(1000.0/FRAMERATE);
		
		sum += val;

		if(val > maxVal)
			maxVal = val;
		if(val < minVal)
			minVal = val;
	}
 
	if(m_listSaccade.GetCount())
		meanVal = sum/m_listSaccade.GetCount();
}

void CAnalyzeSaccade::getAccuracy(double& minVal, double& maxVal, double& meanVal)
{
	minVal = 9999999999;
	maxVal = -9999999999;
	meanVal = 0;

	double sum = 0;

	POSITION pos1, pos2;
	double val = 0;
	for(pos1 = this->m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; )
	{
		structSaccade* pSac = m_listSaccade.GetNext(pos1);
		val = pSac->accuracy;
		
		sum += val;

		if(val > maxVal)
			maxVal = val;
		if(val < minVal)
			minVal = val;
	}
 
	if(m_listSaccade.GetCount())
		meanVal = sum/m_listSaccade.GetCount();
}


unsigned long CAnalyzeSaccade::getClosestSaccade(double t)
{
	//t�� ���� ����� ��ġ�� pursuit index�� return�Ѵ�.
	unsigned long idx = -1;

	double m = 1000000000000;

	unsigned long count = 0;
	if(m_listSaccade.GetCount())
	{
		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structSaccade* pSac = m_listSaccade.GetNext(pos1);

			double d = fabs(t*FRAMERATE-(pSac->startIdx+pSac->endIdx)/2);
			if(d < m)
			{
				m = d;
				idx = count;
			}
			count++;
		}
	}

	return idx;

}


unsigned long CAnalyzeSaccade::addSaccade(unsigned long idx, double* pEye, double* pTarget, unsigned long count)
{
	unsigned long saccadeIdx = 0;


	
	//��ġ�� ã�´�.
	//POSITION posPrev, posNext;
	POSITION pos1, pos2;
	structSaccade* pSaccadePrev = NULL;
	structSaccade* pSaccadeNext = NULL;
	if(this->m_listSaccade.GetCount())
	{		
		double val = 0;
		for(pos1 = this->m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{			
			
			pSaccadeNext = m_listSaccade.GetNext(pos1);
			if((pSaccadeNext->endIdx+pSaccadeNext->startIdx)/2 > idx)
				break;
			saccadeIdx++;
			pSaccadePrev = pSaccadeNext;
		}
	}

	unsigned long startIdx = max(1, idx-MS500);
	unsigned long endIdx = min(idx+MS500, count-1);

	unsigned long i;
	for(i = startIdx; i<endIdx; i++)
	{
		if(pTarget[i-1] != pTarget[i])
			break;
	}
	


	structSaccade* pSaccade = new structSaccade;
	pSaccade->targetLeap = pTarget[i]-pTarget[i-1];
	pSaccade->startIdx = i-1;
	if(getSaccadeParam(pEye, i, min(i+MS1000, count-1), pSaccade))	//saccade���� 500ms������
		m_listSaccade.InsertBefore(pos2, pSaccade);
	else
	{
		saccadeIdx = -1;
		delete pSaccade;
	}

	


	return saccadeIdx;
}