#include "StdAfx.h"
#include ".\analyzenystagmus2.h"
#include "corwldefines.h"
#include "math.h"

CAnalyzeNystagmus2::CAnalyzeNystagmus2(void)
{
}

CAnalyzeNystagmus2::~CAnalyzeNystagmus2(void)
{
	this->deleteAllNystagmus();
}

void CAnalyzeNystagmus2::deleteAllNystagmus()
{
	if(!m_listNystagmus.IsEmpty())
	{
		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if(pNys)
				delete pNys;
		}

		this->m_listNystagmus.RemoveAll();
	}
}


void CAnalyzeNystagmus2::analyze(double* pEye, unsigned long count)
{
	EA_Nystagmus analNys;
	analNys.init();
	
	int nRtn;

	//list �ʱ�ȭ
	this->deleteAllNystagmus();	


	int nysCount = 0;
	for(unsigned long i=0; i<count; i++)
	{
		nRtn = analNys.putSignal(pEye[i]);

		if(nRtn)
		{
			nysCount++;

			structNystag* pNys = new structNystag;
			*pNys = analNys.m_structCurrentNystag;

			this->m_listNystagmus.AddTail(pNys);

			i = pNys->endI+1;
		}
	}
}
CString CAnalyzeNystagmus2::getResultString()
{
	CString strResult;

	return strResult;
}

int CAnalyzeNystagmus2::getCount(int nysType)
{
	int count = 0;
	POSITION pos1, pos2;
	
	switch(nysType)
	{
	case ALL:
		count = int(this->m_listNystagmus.GetCount());
		break;
	case POSI:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if(pNys->vel > 0)
				count++;
		}
		break;
	case NEGA:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if(pNys->vel < 0)
				count++;
		}
		break;
	default:
		break;
	}
	return count;
}


double CAnalyzeNystagmus2::getMean(int nysType)
{
	double rtn = 0;
	POSITION pos1, pos2;
	double s = 0;
	int n = 0;
	
	switch(nysType)
	{
	case ALL:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			s += pNys->vel;
			n++;
		}			
		break;
	case POSI:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if(pNys->vel > 0)
			{
				s += pNys->vel;
				n++;
			}


		}
		break;
	case NEGA:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if(pNys->vel < 0)
			{
				s += pNys->vel;
				n++;
			}
		}
		break;
	default:
		break;
	}

	if(n)
		rtn = s/n;
	else
		rtn = 0;
	return rtn;
}
double CAnalyzeNystagmus2::getMax(int nysType)
{
	double rtn = 0;
	POSITION pos1, pos2;
	
	double m = -1000000000;

	switch(nysType)
	{
	case ALL:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if(m < pNys->vel )
				m = pNys->vel;
		}
		
		break;
	case POSI:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if((pNys->vel > 0) && (m < pNys->vel))
				m = pNys->vel;
		}
		break;
	case NEGA:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if((pNys->vel < 0) && (m < pNys->vel))
				m = pNys->vel;
		}
		break;
	default:
		break;
	}
	return m;
}
double CAnalyzeNystagmus2::getMin(int nysType)
{
	double rtn = 0;
	POSITION pos1, pos2;
	
	double m = 100000;

	switch(nysType)
	{
	case ALL:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if(m > pNys->vel )
				m = pNys->vel;
		}
		
		break;
	case POSI:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if((pNys->vel > 0) && (m > pNys->vel))
				m = pNys->vel;
		}
		break;
	case NEGA:
		for(pos1 = m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);
			if((pNys->vel < 0) && (m > pNys->vel))
				m = pNys->vel;
		}
		break;
	default:
		break;
	}
	return m;
}
	
bool CAnalyzeNystagmus2::load(CString fname)
{
	bool bRtn = false;
	CFile file;
	if(file.Open(fname, CFile::modeRead))
	{
		//���� DATA�� ��� �����.
		this->deleteAllNystagmus();

		unsigned char buf[256];
		::ZeroMemory(buf, 256);

		unsigned int count = (unsigned int)(file.GetLength()/256);
		for(unsigned int i=0; i<count; i++)
		{
			structNystag* pNystag = new structNystag;
			file.Read(buf, 256);
			::memcpy(pNystag, buf, sizeof(structNystag));

			this->m_listNystagmus.AddTail(pNystag);
			
		}

		file.Close();
		bRtn = true;
	}

	return bRtn;
}
bool CAnalyzeNystagmus2::save(CString fname)
{
	bool bRtn = false;
	CFile file;
	if(file.Open(fname, CFile::modeWrite | CFile::modeCreate))
	{
		unsigned char buf[256];
		::ZeroMemory(buf, 256);

		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNystag= m_listNystagmus.GetNext(pos1);
			memcpy(buf, pNystag, sizeof(structNystag));
			file.Write(buf, 256);
		}

		file.Close();
		bRtn = true;
	}

	return bRtn;
}



unsigned long CAnalyzeNystagmus2::getClosestNystagmus(double t)
{
	//t�� ���� ����� ��ġ�� pursuit index�� return�Ѵ�.
	unsigned long idx = -1;

	double m = 1000000000000;

	unsigned long count = 0;
	if(m_listNystagmus.GetCount())
	{
		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag* pNys = m_listNystagmus.GetNext(pos1);

			double d = fabs(t*FRAMERATE-(pNys->startI+pNys->endI)/2);
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

unsigned long CAnalyzeNystagmus2::addNystagmus(unsigned long idx, double* pEye, unsigned long count)
{
	unsigned long nysIdx = 0;


	
	//��ġ�� ã�´�.
	//POSITION posPrev, posNext;
	POSITION pos1, pos2;
	structNystag* pNysPrev = NULL;
	structNystag* pNysNext = NULL;
	if(m_listNystagmus.GetCount())
	{		
		double val = 0;
		for(pos1 = this->m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{			
			
			pNysNext = m_listNystagmus.GetNext(pos1);
			if((!pNysNext) || (pNysNext->endI+pNysNext->startI)/2 > idx)
				break;
			nysIdx++;
			pNysPrev = pNysNext;
		}
	}

	//���� nystagmus�� ��ġ�� �ʵ��� �Ѵ�.
	unsigned long prevMax = pNysPrev ? pNysPrev->endI : 0;
	unsigned long nextMin = pNysNext ? pNysNext->startI : count-1; 
		


	//nystagmus�� �����.
	structNystag* pNys = new structNystag;

	//startI�� ã�´�.
	int M = 3;	//EA_Nystagmus::m_paramDerivM
	double velTh = 40;
	double vel = 0;
	
	unsigned long i=0;
	for(i=idx; i>i-24; i--)	//�ִ�200ms
	{
		if(i-M<=0)
			break;
		vel = (pEye[i] - pEye[i-M])/(double(M)/FRAMERATE);
		if(fabs(vel) > velTh)
			break;
	}
    pNys->startI = max((i+3), prevMax);
	for(i=idx; i<i+24; i++)
	{
		if(i == count-1)
			break;
		vel = (pEye[i]-pEye[i-M])/(double(M)/FRAMERATE);
		if(fabs(vel) > velTh)
			break;
	}
	pNys->endI = min(i-3, nextMin);
	//����� �����������
	if(pNys->endI>pNys->startI)
		pNys->vel = (pEye[pNys->endI] - pEye[pNys->startI]) / (double(pNys->endI-pNys->startI)/FRAMERATE);
	else
		nysIdx = -1;

	
	
	if(nysIdx == -1)
	{
		delete pNys;
	}
	else
	{
		//list�� �߰��Ѵ�.
		m_listNystagmus.InsertBefore(pos2, pNys);
	}
	return nysIdx;

}
