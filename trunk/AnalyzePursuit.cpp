#include "StdAfx.h"
#include ".\analyzepursuit.h"
#include "corwldefines.h"
#include "corwlutil.h"
#include "math.h"

CAnalyzePursuit::CAnalyzePursuit(void)
{
	m_pdEyeVel = 0;
	m_pdTargetVel = 0;

	
	m_ulDataCount = 0;
}

CAnalyzePursuit::~CAnalyzePursuit(void)
{
	DEL(m_pdEyeVel);
	DEL(m_pdTargetVel);

	this->deleteAllPursuit();
}


void CAnalyzePursuit::deleteAllPursuit()
{
	if(!m_listPursuit.IsEmpty())
	{
		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listPursuit.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structPursuit* pPursuit = m_listPursuit.GetNext(pos1);
			delete pPursuit;
		}

		this->m_listPursuit.RemoveAll();
	}
}

void CAnalyzePursuit::analyze(double* pEye, double* pTarget, unsigned long count)
{
	DEL(m_pdEyeVel);
	DEL(m_pdTargetVel);
	deleteAllPursuit();

	m_ulDataCount = 0;
	
	if(!count || !pEye || !pTarget)
		return;

	m_pdEyeVel = new double[m_ulDataCount = count];
	m_pdTargetVel = new double[m_ulDataCount];
	::ZeroMemory(m_pdEyeVel, sizeof(double)*m_ulDataCount);
	::ZeroMemory(m_pdTargetVel, sizeof(double)*m_ulDataCount);

	//Velocity�� ���Ѵ�.
	unsigned int M = 100;	//���� 1000/SAMPLERATE*M (ms) ~ ���� 1000/SAMPLERATE*M (ms) �� �ӵ��� ���Ѵ�.
	double t = 2.0*M/FRAMERATE;
	for(unsigned long i=1; i<count-M; i++)
	{
		if(i<M)
		{
			m_pdEyeVel[i] = (pEye[i+M]-pEye[0])/(double(i+M)/FRAMERATE);
			m_pdTargetVel[i] = (pTarget[i+M] - pTarget[0])/(double(i+M)/FRAMERATE);
		}
		else
		{
			m_pdEyeVel[i] = (pEye[i+M]-pEye[i-M])/t;
			m_pdTargetVel[i] = (pTarget[i+M] - pTarget[i-M])/t;
		}
	}

	

	//ó������ 0�� �ƴѰ����� ã�´�.
	unsigned long startIdx = M;
	while(m_pdTargetVel[startIdx++]==0);

	//�м� ������ ã�´�.
	// Target Velocity�� zero-crossing �ϴ� �� ���� ���̰� �м� �����̴�.
	// �ּ� 0.3�� �̻� ���̰� ���� �Ѵ�.
	unsigned long* pZeroVelIdx = new unsigned long[unsigned long(count/2)];
	unsigned long zeroVelIdxCount = 0;
	for(unsigned long i=startIdx; i<count ; i++)
	{
		if(((m_pdTargetVel[i+1]*m_pdTargetVel[i]) <= 0)	 && !((m_pdTargetVel[i+1] == 0) && (m_pdTargetVel[i] == 0)) )//zero-crossing�̸� �Ѵ� 0�� �ƴѰ�
		{
			pZeroVelIdx[zeroVelIdxCount++] = i;
		}
	}

	//�м��Ѵ�.
	if(zeroVelIdxCount>=2) //<-���� ���. count�� 2�̻��� �Ǿ� �� �� ������
	{
		for(unsigned long i=0; i<zeroVelIdxCount-2; i++)
		{
			structPursuit* pPursuit = new structPursuit;
			pPursuit->startIdx = pZeroVelIdx[i];
			pPursuit->endIdx = pZeroVelIdx[i+1];

			if(findPursuitParam(pPursuit) && (pPursuit->peakVelTarget != 0))
			{
				this->m_listPursuit.AddTail(pPursuit);
			}
			else
			{
				delete pPursuit;
			}
/*

			if((pZeroVelIdx[i+1]-pZeroVelIdx[i]) > 300.0/1000*FRAMERATE)	//300ms �̻��̶��
			{
				structPursuit* pPursuit = new structPursuit;
				pPursuit->startIdx = pZeroVelIdx[i];
				pPursuit->endIdx = pZeroVelIdx[i+1];


				unsigned long idx = 0;
				double m = 0;
				//target �߰��� �ӵ��� ������ max���� ã�´�.
				if(m_pdTargetVel[int((pZeroVelIdx[i]+pZeroVelIdx[i+1])/2)] > 0)
				{
					//max eye vel
					m = corFindMaximum(m_pdEyeVel, pZeroVelIdx[i], pZeroVelIdx[i+1], &idx);	// ���� �� �ִ밪�� ã�´�.
					pPursuit->peakVelEye = m;
					pPursuit->peakVelEyeIdx = idx;

					//max target vel
					m = corFindMaximum(m_pdTargetVel, pZeroVelIdx[i], pZeroVelIdx[i+1], &idx);	// ���� �� �ִ밪�� ã�´�.
					pPursuit->peakVelTarget= m;
					pPursuit->peakVelTargetIdx = idx;
				}
				else
				{
					//max eye vel
					m = corFindMinimum(m_pdEyeVel, pZeroVelIdx[i], pZeroVelIdx[i+1], &idx);	// ���� �� �ִ밪�� ã�´�.
					pPursuit->peakVelEye = m;
					pPursuit->peakVelEyeIdx = idx;

					//max target vel
					m = corFindMinimum(m_pdTargetVel, pZeroVelIdx[i], pZeroVelIdx[i+1], &idx);	// ���� �� �ִ밪�� ã�´�.
					pPursuit->peakVelTarget= m;
					pPursuit->peakVelTargetIdx = idx;
				}

				if(pPursuit->peakVelTarget != 0)
					this->m_listPursuit.AddTail(pPursuit);
				else
					delete pPursuit;
			}*/
		}
	}
	
	delete []pZeroVelIdx;
}

CString CAnalyzePursuit::getResultString()
{
	CString strResult;

	return strResult;
}

unsigned int CAnalyzePursuit::getCount()
{
	return (unsigned int)(this->m_listPursuit.GetCount());
}
	
void CAnalyzePursuit::getGain(double& minVal, double& maxVal, double& meanVal)
{
	minVal = 9999999999;
	maxVal = 0;
	meanVal = 0;

	double sum = 0;

	int count = 0;
	if(m_listPursuit.GetCount())
	{
		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listPursuit.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structPursuit* pPursuit = m_listPursuit.GetNext(pos1);

			if(pPursuit->peakVelTarget)
			{
				val = fabs(pPursuit->peakVelEye/pPursuit->peakVelTarget);
				count++;
				sum += val;
			

				if(val > maxVal)
					maxVal = val;
				if(val < minVal)
					minVal = val;
			}
		}
 
		if(count)
			meanVal = sum/count;
		else
			meanVal = 0;

		if(minVal == 9999999999)
			minVal = 0;
	}
	else
		minVal = maxVal = meanVal = 0;
}

void CAnalyzePursuit::reshapeTarget(double* pTarget, unsigned long count, double amp)
{
	//�ǽð� target�� rectagular signal�� ����Ǿ� �ִ�.
	//�̸� sin �������� �ٲپ�� �Ѵ�.
 
	if(count==0)
		return;

	//
	double pi = 3.14159265;

	unsigned long startIdx = 0;
	unsigned long endIdx = 0;

	
	double* pTargetRe = new double[count];
	ZeroMemory(pTargetRe, sizeof(double)*count);

	for(unsigned long i=int(FRAMERATE/10); i<count-1; i++)
	{
		//target�� zero crossing�� ã�´�.
		if((pTarget[i] * pTarget[i+1]) <0)
		{
			//if(startIdx)
			{
				//startidx~endidx�� pTarget[i] ������ amp ũ��sin wave�� �����.
				// �ֱ�� (endIdx-startIdx)*2
				// f = amp*sin(2*pi*(1/2/(endIdx-startIdx))) -> pTarget[i]>0
				// f = amp*sin(2*pi*(1/2/(endIdx-startIdx)) + pi) -> pTarget[i]<0

				endIdx = i;
				
				double f = 2*pi*1.0/(2*(endIdx-startIdx));
				double b = (pTarget[i]>0) ? 0 : pi;		//�ؼ��� ����
				for(unsigned long j=startIdx; j<endIdx; j++)
				{
					pTargetRe[j] = amp*sin(f*(j-startIdx) + b);

				}
			}

			startIdx = i;
		}

	}

	::memcpy(pTarget, pTargetRe, sizeof(double)*count);
	delete []pTargetRe;

}

bool CAnalyzePursuit::load(CString fname)		//�м� ��� load
{
	bool bRtn = false;
	CFile file;
	if(file.Open(fname, CFile::modeRead))
	{
		//���� DATA�� ��� �����.
		this->deleteAllPursuit();

		unsigned char buf[256];
		::ZeroMemory(buf, 256);

		unsigned int count = (unsigned int)(file.GetLength()/256);
		for(unsigned int i=0; i<count; i++)
		{
			structPursuit* pPursuit = new structPursuit;
			file.Read(buf, 256);
			::memcpy(pPursuit, buf, sizeof(structPursuit));

			this->m_listPursuit.AddTail(pPursuit);
			
		}

		file.Close();
		bRtn = true;
	}

	return bRtn;
}
bool CAnalyzePursuit::save(CString fname)		//�м� ��� save
{
	bool bRtn = false;
	CFile file;
	if(file.Open(fname, CFile::modeWrite | CFile::modeCreate))
	{
		unsigned char buf[256];
		::ZeroMemory(buf, 256);

		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listPursuit.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structPursuit* pPursuit = m_listPursuit.GetNext(pos1);
			memcpy(buf, pPursuit, sizeof(structPursuit));
			file.Write(buf, 256);
		}


		file.Close();
		bRtn = true;
	}

	return bRtn;
}



unsigned long CAnalyzePursuit::getClosestPursuit(double t)
{
	//t�� ���� ����� ��ġ�� pursuit index�� return�Ѵ�.
	unsigned long idx = -1;

	double m = 1000000000000;

	unsigned long count = 0;
	if(m_listPursuit.GetCount())
	{
		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listPursuit.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structPursuit* pPursuit = m_listPursuit.GetNext(pos1);

			double d = fabs(t*FRAMERATE-(pPursuit->startIdx+pPursuit->endIdx)/2);
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

bool CAnalyzePursuit::findPursuitParam(structPursuit* pPursuit)
{
	bool bRtn = false;


	unsigned long idx = 0;
	double m = 0;
	if((pPursuit->endIdx-pPursuit->startIdx) > 300.0/1000*FRAMERATE)	//300ms �̻��̶��
	{
		//target �߰��� �ӵ��� ������ max���� ã�´�.
		if(m_pdTargetVel[int((pPursuit->startIdx+pPursuit->endIdx)/2)] > 0)
		{
			//max eye vel
			m = corFindMaximum(m_pdEyeVel, pPursuit->startIdx, pPursuit->endIdx, &idx);	// ���� �� �ִ밪�� ã�´�.
			pPursuit->peakVelEye = m;
			pPursuit->peakVelEyeIdx = idx;

			//max target vel
			m = corFindMaximum(m_pdTargetVel, pPursuit->startIdx, pPursuit->endIdx, &idx);	// ���� �� �ִ밪�� ã�´�.
			pPursuit->peakVelTarget= m;
			pPursuit->peakVelTargetIdx = idx;
		}
		else
		{
			//max eye vel
			m = corFindMinimum(m_pdEyeVel, pPursuit->startIdx, pPursuit->endIdx, &idx);	// ���� �� �ִ밪�� ã�´�.
			pPursuit->peakVelEye = m;
			pPursuit->peakVelEyeIdx = idx;

			//max target vel
			m = corFindMinimum(m_pdTargetVel, pPursuit->startIdx, pPursuit->endIdx, &idx);	// ���� �� �ִ밪�� ã�´�.
			pPursuit->peakVelTarget= m;
			pPursuit->peakVelTargetIdx = idx;
		}
		bRtn = true;
	}


	return bRtn;
}

unsigned long CAnalyzePursuit::addPursuit(unsigned long idx, double* pEye, unsigned long count)
{
	unsigned long pursuitIdx = 0;


	
	//��ġ�� ã�´�.
	//POSITION posPrev, posNext;
	POSITION pos1, pos2;
	structPursuit* pPursuitPrev = NULL;
	structPursuit* pPursuitNext = NULL;
	if(this->m_listPursuit.GetCount())
	{		
		double val = 0;
		for(pos1 = this->m_listPursuit.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{			
			
			pPursuitNext = m_listPursuit.GetNext(pos1);
			if((pPursuitNext->endIdx+pPursuitNext->startIdx)/2 > idx)
				break;
			pursuitIdx++;
			pPursuitPrev = pPursuitNext;
		}
	}

	

	//nystagmus�� �����.
	structPursuit* pPursuit = new structPursuit;
	//target velocity�� 0�� �Ǵ� �¿� start end idx�� ã�´�. 
	pPursuit->startIdx = corFindZeroCrossIdx(this->m_pdTargetVel, idx, count, -1);
	pPursuit->endIdx = corFindZeroCrossIdx(this->m_pdTargetVel, idx, count, 1);


	if((pPursuit->startIdx != -1) && (pPursuit->endIdx != -1))
	{
		if(findPursuitParam(pPursuit))
		{
			m_listPursuit.InsertBefore(pos2, pPursuit);
		}
		else
		{
			pursuitIdx = -1;
			delete pPursuit;
		}

	}


	return pursuitIdx;
}

