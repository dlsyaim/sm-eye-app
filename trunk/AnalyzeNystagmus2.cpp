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

	//list 초기화
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

void CAnalyzeNystagmus2::analyze2(double* pEye, unsigned long count)
{
	//list 초기화
	this->deleteAllNystagmus();	

	int idx = 0, endIdx = 0, startIdx = 0;
	double maxD = 0;
	double thresRatio = 2, thres = 0;

	unsigned long prevEndIdx = 0;

	for(int i=10; i<count-10; i++)
	{
		/////////////////////////////positive slow phase
		if( (pEye[i-1] < pEye[i]) && (pEye[i] >= pEye[i+1]) )
		{
			idx = i;
			maxD = 0;
			// fast phase의 negative가 끝나는 곳을 찾는다.
			while( (pEye[idx] >= pEye[idx+1]) && (idx<count-4))
			{
				// 그 중 diff가 max가 되는 곳을 찾는다.
				if (maxD < -1*(pEye[idx+2]-pEye[idx]) )
					maxD = -1*(pEye[idx+2]-pEye[idx]);
				idx = idx+1;
			}
			endIdx = idx;

			// fast phase가 2point 이상 지속된다면
		    idx = i;
            if(endIdx-i >= 2)
			{
				// 반대방향으로 search한다.
				// slow phase의 끝을 찾는다.
				thres = maxD/thresRatio;
				while( (fabs(pEye[idx]-pEye[idx-2]) < thres) && (idx>3) )
					idx = idx-1;
			}
			startIdx = idx;

			//slow phase가 충분히 지속되고, slow phase와 fast phase의 부호가 다르다면
			if( (i-startIdx > 10) && 
				((pEye[endIdx]-pEye[i])*(pEye[i]-pEye[startIdx])<0) &&
				(startIdx > prevEndIdx))		// 이전 nystagmus end와 겹치지 않아야 한다.
			{
				structNystag* pNys = new structNystag;
								
				//양쪽 10%씩 잘라준다.
				pNys->vel = (pEye[i]-pEye[startIdx]) / (i-startIdx) * 120;
				pNys->startI = startIdx + int((i-startIdx)*0.1);
				pNys->endI = i - int((i-startIdx)*0.1);

				this->m_listNystagmus.AddTail(pNys);
				prevEndIdx = pNys->endI;

			}
		}
		/////////////////////////////negative slow phase
		if( (pEye[i-1] > pEye[i]) && (pEye[i] >= pEye[i+1]) )
		{
			idx = i;
			maxD = 0;
			// fast phase의 positive가 끝나는 곳을 찾는다.
			while( (pEye[idx] <= pEye[idx+1]) && (idx<count-4))
			{
				// 그 중 diff가 max가 되는 곳을 찾는다.
				if (maxD < pEye[idx+2]-pEye[idx] )
					maxD = pEye[idx+2]-pEye[idx];
				idx = idx+1;
			}
			endIdx = idx;

			// fast phase가 2point 이상 지속된다면
		    idx = i;
            if(endIdx-i >= 2)
			{
				// 반대방향으로 search한다.
				// slow phase의 끝을 찾는다.
				thres = maxD/thresRatio;
				while( (fabs(pEye[idx]-pEye[idx-2]) < thres) && (idx>3) )
					idx = idx-1;
			}
			startIdx = idx;

			//slow phase가 충분히 지속되고, slow phase와 fast phase의 부호가 다르다면
			if( (i-startIdx > 20) && 
				((pEye[endIdx]-pEye[i])*(pEye[i]-pEye[startIdx])<0) &&
				(startIdx > prevEndIdx))		// 이전 nystagmus end와 겹치지 않아야 한다.)
			{
				structNystag* pNys = new structNystag;
				
				//양쪽 10%씩 잘라준다.
				pNys->vel = (pEye[i]-pEye[startIdx]) / (i-startIdx) * 120;
				pNys->startI = startIdx + int((i-startIdx)*0.1);
				pNys->endI = i - int((i-startIdx)*0.1);

				this->m_listNystagmus.AddTail(pNys);
				prevEndIdx = pNys->endI;

			}
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
		//현재 DATA를 모두 지운다.
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
	//t에 가장 가까운 위치의 pursuit index를 return한다.
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

unsigned long CAnalyzeNystagmus2::addNystagmus2(unsigned long idx, double* pEye, unsigned long count)
{
	//좌우의 nystagmus를 찾아 fast phase의 속도를 찾는다.
	// fast phase 를 threshold로 주어진 위치 좌우의 fast phase 를 찾는다.
	// 위치를 찾는다.

	//POSITION posPrev, posNext;
	POSITION pos;
	POSITION posNext;
	structNystag* pNysPrev = NULL;
	structNystag* pNysNext = NULL;
	structNystag* pNys = NULL;
	int nysIdx = 0;
	if(int nysCount = m_listNystagmus.GetCount())
	{	
		pos = m_listNystagmus.GetHeadPosition();
		for(int i=0; i<nysCount; i++)
		{
			pNys = m_listNystagmus.GetNext(pos);
			if( (pNys->startI < idx ) && (idx < pNys->endI) )
			{
				::AfxMessageBox("Overlapped");
				return -1;
			}
		}

		double val = 0;
		pos = m_listNystagmus.GetHeadPosition();
		for(int i=0; i<nysCount; i++)
		{						
			pNys = m_listNystagmus.GetNext(pos);
			
			//이전 nystagmus를 찾는다.
			if( pNys->endI < idx)
			{
				//전에 찾아진게 있다면 위치를 비교한다.
				if(pNysPrev)
				{
					if(pNys->endI > pNysPrev->endI)
					{
						pNysPrev = pNys ;
						//넣을 위치
						posNext = pos;
					}
				}	
				// 없다면 이전 nystagmus로 등록
				else
				{
					pNysPrev = pNys;
					posNext = pos;
				}
				nysIdx++;
			}

			//이후 nystagmus를 찾는다.
			if( idx < pNys->startI)
			{
				// 전에 찾아진게 있다면
				if(pNysNext)
				{
					if(pNys->startI < pNysNext->startI)
						pNysNext = pNys;
				}
				else
					pNysNext = pNys;
			}


			//그 위치에 nystamus가 있는지도 확인한다.
			if((pNys->startI < idx) && (idx<pNys->endI))
				return -1;
			
		}

		
	}

	// fast phase의 속도를 구한다.
	int M = 2;
	double prevFPV = 0, nextFPV = 0;

	if(pNysPrev)
	{
		for(int i=pNysPrev->endI; i<min(pNysPrev->endI + 30, count-M-5); i++)
		{
			if(abs(pEye[i+M]-pEye[i]) > prevFPV)
				prevFPV = abs(pEye[i+M]-pEye[i]);
		}
	}
	if(pNysNext)
	{
		for(int i=pNysNext->endI; i<min(pNysNext->endI + 30, count-M-5); i++)
		{
			if(abs(pEye[i+M]-pEye[i]) > nextFPV)
				nextFPV = abs(pEye[i+M]-pEye[i]);
		}
	}

	//threshold 값을 결정
	double thres = 0;
	if(prevFPV || nextFPV)
		thres = 0.3*max(prevFPV ? prevFPV : 0, nextFPV ? nextFPV : 0);
		//thres = 0.3*min(prevFPV ? prevFPV : 100000, nextFPV ? nextFPV : 100000);
	else
	{
		::AfxMessageBox(" There are no nystagmus around here");
		return -1;
	}

	structNystag* pNysFound = NULL;

	//double thresHigh = min(prevFPV ? prevFPV : 100000, nextFPV ? nextFPV : 100000);
	double thresHigh = max(prevFPV ? prevFPV : 0, nextFPV ? nextFPV : 0);
	//nystagmus를 찾을 때까지 threshol값을 올린다.
	//너무 높아지면 중지
	while (!pNysFound && (thres < thresHigh*0.95))
	{

		//주어진 위치 좌우에서 threshod를 넘는 위치까지 찾는다.
		unsigned long startIdx = 0, endIdx = 0;
		//startIdx를 찾는다.
		for(int i=idx; i> max(idx-100, M+5); i--)
		{
			if(abs(pEye[i] - pEye[i-M]) > thres)
			{
				startIdx = i;
				break;
			}
		}


		for(int i=idx; i< min(idx+ 100, count-M-5); i++)
		{
			if(abs(pEye[i+M] - pEye[i]) > thres)
			{
				endIdx = i;
				break;
			}
		}

		//이전과 이후 위치와 비교하여 사이에 있는지 확인
		if( ( pNysPrev ? startIdx > pNysPrev->endI : true) && 
			(pNysNext ? endIdx < pNysNext->startI : true) && 
			(endIdx-startIdx > 10) &&			//최소 길이
			(endIdx-startIdx<120))				//최대 길이
		{
			int len = endIdx-startIdx;
			startIdx += int(len*0.1 +0.5);
			endIdx -= int(len*0.1 + 0.5);

			//nystagmus를 만든다.
			pNysFound = new structNystag;
			pNysFound->startI = startIdx;
			pNysFound->endI = endIdx;
			pNysFound->vel = (pEye[endIdx] - pEye[startIdx]) / (double(endIdx-startIdx)/FRAMERATE);

			

		}
		
		thres = thres*1.1;
	}

	//못찾았다면 click된 위치 좌우 11개로 만든다.
	if(!pNysFound)
	{
		unsigned long startIdx = pNysPrev ? max(idx-5, pNysPrev->endI + 4) : max(idx-5, 0);
		unsigned long endIdx = pNysNext ? min(idx+5, pNysNext->startI -4) : min(idx+5, count-1); 
		
		if(startIdx < endIdx)
		{
			pNysFound = new structNystag;
			pNysFound->startI = startIdx;
			pNysFound->endI = endIdx;
			pNysFound->vel = (pEye[endIdx] - pEye[startIdx]) / (double(endIdx-startIdx)/FRAMERATE);
		}
		else
			nysIdx = -1;

		
	}

	if(!pNysPrev)	//가장 앞이라면
		m_listNystagmus.AddHead(pNysFound);
	else if(!pNysNext)	//가장 뒤라면
		m_listNystagmus.AddTail(pNysFound);
	else if(pNysPrev && pNysNext)
        m_listNystagmus.InsertBefore(posNext, pNysFound);

	return nysIdx;


	
	

}

unsigned long CAnalyzeNystagmus2::addNystagmus(unsigned long idx, double* pEye, unsigned long count)
{
	unsigned long nysIdx = 0;


	
	//위치를 찾는다.
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

	//양쪽 nystagmus와 겹치지 않도록 한다.
	unsigned long prevMax = pNysPrev ? pNysPrev->endI : 0;
	unsigned long nextMin = pNysNext ? pNysNext->startI : count-1; 
		


	//nystagmus를 만든다.
	structNystag* pNys = new structNystag;

	//startI를 찾는다.
	int M = 3;	//EA_Nystagmus::m_paramDerivM
	double velTh = 40;
	double vel = 0;
	
	unsigned long i=0;
	for(i=idx; i>i-24; i--)	//최대200ms
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
	//제대로 만들어졌으면
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
		//list에 추가한다.
		m_listNystagmus.InsertBefore(pos2, pNys);
	}
	return nysIdx;

}


void CAnalyzeNystagmus2::deleteNysWithDirection(int direction)
{

	int count = this->m_listNystagmus.GetCount();

	POSITION oldPos;
	POSITION pos = this->m_listNystagmus.GetHeadPosition();

	structNystag* pNys = NULL;
	
	for(int i=0; i<count; i++)
	{
		oldPos = pos;
		pNys = m_listNystagmus.GetNext(pos);
		if(pNys && direction*pNys->vel > 0)
		{
			delete pNys;
			m_listNystagmus.RemoveAt(oldPos);
		}
	}


	
	



}