#include "StdAfx.h"
#include ".\ea_nystagmus.h"
#include "corwlUtil.h"
#include "math.h"
#include ".\tchart\series.h"
#include ".\tchart\valuelist.h"

#include ".\tchart\fastlineseries.h"

EA_Nystagmus::EA_Nystagmus(void)
{
	init();

	m_paramDerivM = 6;
	m_paramDerivMax = 360;
	m_paramDerivThres = 30;
	m_paramDerivNoiseLevel = 400;
	m_paramContPeakNum = 2;
	m_paramContPeakDuration = FRAMERATE*4;

	m_paramNysMinLength = FRAMERATE/20;	//slow phase의 최소 시간
	m_paramNysMaxLength = FRAMERATE/2;	//slow phase의 최대 시간

	m_paramVelMinMax = 90;
}

EA_Nystagmus::~EA_Nystagmus(void)
{
}

void EA_Nystagmus::init()
{
	m_pTChartResult = NULL;
	m_pTChartOriginal = NULL;
	m_seriesResultIndex = -1;


	m_ulDataCount = 0;
	::ZeroMemory(m_EyePosBuf , sizeof(double)*EYE_BUF_LEN);
	::ZeroMemory(m_EyeVelBuf , sizeof(double)*EYE_BUF_LEN);
	this->m_EyeBufCount = 0;

	::ZeroMemory(m_NysVelPeakBuf, sizeof(double)*NYS_BUF_LEN);		//fast phase의 velocity
	::ZeroMemory(m_NysVelPeakIndexBuf, sizeof(double)*NYS_BUF_LEN);		//fast phase의 velocity
	m_NysBufCount = 0;

	m_prevVel[0] = m_prevVel[1] = 0;

	::ZeroMemory(&m_structCurrentNystag, sizeof(structNystag));
	::ZeroMemory(&m_structPrevNystag, sizeof(structNystag));
	m_prevMaxEndI = 0;
}
void EA_Nystagmus::setResultSeries(CTChart* pTResult, CTChart* pTOriginal, int seriesIndex)
{
	m_pTChartResult = pTResult;
	m_pTChartOriginal= pTOriginal;
	m_seriesResultIndex = seriesIndex;
}
bool findNystagmus(double* pVel, int count, structNystag* pNys);
int EA_Nystagmus::putSignal(double ep)
{
	int bRtn = 0;

	m_ulDataCount++;
	m_structCurrentNystag.startI = m_structCurrentNystag.endI = 0;
	m_structCurrentNystag.vel = 0;

	//eye position을 저장
	m_EyePosBuf[m_EyeBufCount] = ep;

	//velocity를 계산.
	int m = m_EyeBufCount - m_paramDerivM;
	m_EyeVelBuf[m_EyeBufCount] = (m_EyePosBuf[m_EyeBufCount] - m_EyePosBuf[m>=0 ? m : m+EYE_BUF_LEN])
									/(double(m_paramDerivM)/FRAMERATE);
	if(m_ulDataCount <= m_paramDerivM)
		m_EyeVelBuf[m_EyeBufCount] = 0;

	//noise level보다 크면 이전값 다시 사용
	if(fabs(m_EyeVelBuf[m_EyeBufCount]) > m_paramDerivMax)
		m_EyeVelBuf[m_EyeBufCount] = m_EyeVelBuf[(m_EyeBufCount-1 >=0) ? m_EyeBufCount-1 : EYE_BUF_LEN-1];
	
	
	
	
	//peak인지 확인한다.
	if(((m_EyeVelBuf[m_EyeBufCount]-m_prevVel[1])*(m_prevVel[1]-m_prevVel[0]) <= 0) &&	//peak 이고
		(fabs(m_EyeVelBuf[m_EyeBufCount]) > this->m_paramDerivThres) &&					//threshold보다 크고
		(fabs(m_EyeVelBuf[m_EyeBufCount]) < m_paramDerivNoiseLevel))						//noise level보다 작다면
	{
		//fast phase peak이다.
		//nystag 버프에 추가한다.
		//circular buffer가 아니라 FIFO 구조로 만든다.
		int nysBufI;
		if(m_NysBufCount == NYS_BUF_LEN)
			nysBufI = NYS_BUF_LEN-1;
		else
			nysBufI = m_NysBufCount;
		
		//peak의 위치와 속도를 저장한다.
		m_NysVelPeakIndexBuf[nysBufI] = m_EyeBufCount;
		m_NysVelPeakBuf[nysBufI] = m_EyeVelBuf[m_EyeBufCount];

		//연속적으로 같은 peak sign이 나오는지 확인한다.
		if(nysBufI >= m_paramContPeakNum)
		{
			int contNum = 1;
			for(int i=1; i<m_paramContPeakNum; i++)
			{
				if(m_NysVelPeakBuf[nysBufI]*m_NysVelPeakBuf[nysBufI-i] > 0)
					contNum++;
			}
			
			//연속된 n개의 peak의 duration을 구한다
			int duration = 0;
			if(m_NysVelPeakIndexBuf[nysBufI] > m_NysVelPeakIndexBuf[nysBufI-m_paramContPeakNum+1])
				duration = m_NysVelPeakIndexBuf[nysBufI] - m_NysVelPeakIndexBuf[nysBufI-m_paramContPeakNum+1];
			else
				duration = m_NysVelPeakIndexBuf[nysBufI] + (EYE_BUF_LEN - m_NysVelPeakIndexBuf[nysBufI-m_paramContPeakNum+1]);
			//연속으로 같은 sign이 나오고 m_paramContPeakNum개가 5초 이내에 나왔다면
			//if((contNum >= m_paramContPeakNum) && 
			//	(duration<m_paramContPeakDuration))
			{
                //nystagmus의 slow phase를 구한다.
				structNystag nystag;
				int count;
				double* pVel = NULL;
				
				//peak와 peak 사이의 data를 분리한다.
				if(m_NysVelPeakIndexBuf[nysBufI-1] < m_NysVelPeakIndexBuf[nysBufI])
				{
					count = m_NysVelPeakIndexBuf[nysBufI] - m_NysVelPeakIndexBuf[nysBufI-1]+1;
					pVel = new double[count];
					memcpy(pVel, &(m_EyeVelBuf[m_NysVelPeakIndexBuf[nysBufI-1]]), sizeof(double)*count);
				}
				else
				{
					count = m_NysVelPeakIndexBuf[nysBufI] + (EYE_BUF_LEN - m_NysVelPeakIndexBuf[nysBufI-1]);
					pVel = new double[count];
					memcpy(pVel, &(m_EyeVelBuf[m_NysVelPeakIndexBuf[nysBufI-1]]), sizeof(double)*(EYE_BUF_LEN - m_NysVelPeakIndexBuf[nysBufI-1]));
					memcpy(&(pVel[EYE_BUF_LEN - m_NysVelPeakIndexBuf[nysBufI-1]]), m_EyeVelBuf, sizeof(double)*m_NysVelPeakIndexBuf[nysBufI]);
				}

				//peak to peak가 적절한 길이여야 한다.
				if( (count > m_paramNysMinLength) && (count < m_paramNysMaxLength))
				{
					
					if(findNystagmus(pVel, count, &nystag))
					{
						int len = nystag.endI - nystag.startI+1;
						if((len>m_paramNysMinLength) && (len<m_paramNysMaxLength) )
						{
							double m = corFindMinimum(&(pVel[nystag.startI]), len);
							double M = corFindMaximum(&(pVel[nystag.startI]), len);

							if(M-m < m_paramVelMinMax)
								nystag.vel = corMean(&(pVel[nystag.startI]), len);
							else
							{
								nystag.endI = nystag.startI = 0;
								nystag.vel = 0;
							}

							//실제 위치로 보정
							if(nystag.startI)
							{
								m_structCurrentNystag.startI = m_ulDataCount - count + nystag.startI-this->m_paramDerivM;
								//이전것과 겹치지 않게 조정해준다.
								if(m_prevMaxEndI >= m_structCurrentNystag.startI)
									m_structCurrentNystag.startI = m_prevMaxEndI + 1;

								m_structCurrentNystag.endI = m_ulDataCount - count + nystag.endI - m_paramDerivM;
								m_structCurrentNystag.vel = nystag.vel;

								m_structPrevNystag = m_structCurrentNystag;

								if(m_structCurrentNystag.endI > m_prevMaxEndI)
									m_prevMaxEndI = m_structCurrentNystag.endI;
							}

							
							if((m_structCurrentNystag.startI) && (m_structCurrentNystag.startI < m_structCurrentNystag.endI))
								bRtn = 1;
							
							
						}
					}
					
				}
				if(pVel)
					delete []pVel;
			}
            
		}

		
		//다음 peak를 위해 buf업뎃
		if(m_NysBufCount == NYS_BUF_LEN)
		{
			//꽉찼다면
			for(int i=0; i<NYS_BUF_LEN-1; i++) 
			{
				m_NysVelPeakIndexBuf[i] = m_NysVelPeakIndexBuf[i+1];
				m_NysVelPeakBuf[i] = m_NysVelPeakBuf[i+1];
			}
		}
		else
			m_NysBufCount++;	//아직 안찼다면 끝에다 추가
	}
		

	//결과를 그려준다.
	//if(this->m_pTChartResult)
	if(0)
	{
		//m_pTChartResult->Series(m_seriesResultIndex).GetAsFastLine().AddRealTime(double(m_ulDataCount)/FRAMERATE, m_EyeVelBuf[m_EyeBufCount], NULL, 0);
		if(bRtn)
		{
			CString str;
			str.Format("%.1f", this->m_structCurrentNystag.vel);
			double x = (m_structCurrentNystag.startI + m_structCurrentNystag.endI)/2;
			m_pTChartResult->Series(m_seriesResultIndex+2).AddXY(double(x)/FRAMERATE, m_structCurrentNystag.vel, NULL, m_seriesResultIndex==0 ? COLOR_LEFT : COLOR_RIGHT);														

			//original에 nystagmus를 그려준다.
			if(m_pTChartOriginal)
			{
				double x_,y_;
				x_ = double(m_structCurrentNystag.startI)/FRAMERATE;
				y_ = m_pTChartOriginal->Series(m_seriesResultIndex).GetYValues().GetValue(m_structCurrentNystag.startI);
				
				m_pTChartOriginal->Series(m_seriesResultIndex+3).AddXY(x_,y_+10, NULL,0);
				x_ = double(m_structCurrentNystag.endI)/FRAMERATE;
				y_ = m_pTChartOriginal->Series(m_seriesResultIndex).GetYValues().GetValue(m_structCurrentNystag.endI);
				m_pTChartOriginal->Series(m_seriesResultIndex+3).AddXY(x_,y_+10, NULL,0);
				m_pTChartOriginal->Series(m_seriesResultIndex+3).AddNullXY(x_,y_+10, NULL);
			}
			
		}
		


	}



	//다음 계산을 위해 남겨둔다.
	m_prevVel[0] = m_prevVel[1];
	m_prevVel[1] = this->m_EyeVelBuf[m_EyeBufCount];

	//원형버퍼 끝 확인
	if(++m_EyeBufCount == EYE_BUF_LEN)
		m_EyeBufCount = 0;

	

	return bRtn;
}

int findLargerIndex(double* pDat, int count, double targetValue,  bool bFoward, bool bLarger);
bool findNystagmus(double* pVel, int count, structNystag* pNys)
{
	bool bRtn = false;
	double m = corMedian(pVel, count);
	
	//pVel[0]는 이전 peak
	//peak와 slow phase의 부호가 달라야 한다.
	if(pVel[0]*m > 0)
		return false;

	int startI = 0;
	int endI = 0;

	startI = findLargerIndex(pVel, count, m, true, (m>pVel[0]) ? true : false);
	endI = findLargerIndex(pVel, count, m, false, (m>pVel[0]) ? true : false);

	//nystagmus의 starI와 endI가 찾아졌다면
	// slow phase의 지속 시간을 체크한다.

	if(startI && endI)
	{
		pNys->startI = startI;
		pNys->endI = endI;
		bRtn = true;
	}

	

	return bRtn;
}

int findLargerIndex(double* pDat, int count, double targetValue,  bool bFoward, bool bLarger)
{
	// pDat[0]로부터 시작하여 targetvalue보다 큰값의 index를 return한다.
	// bFoward가 false면 pDat[count-1]부터 반대방향으로 search
	// bLarger가 true면 targetValue보다 작은 값이 나올때까지 search

	//앞쪽으로 찾는다.
	if(bFoward)
	{
		for(int i=0; i<count; i++)
		{
			if(bLarger)	//큰값이 나올때까지 찾는다.
			{
				if(pDat[i] > targetValue)
					return i;
			}
			else
				if(pDat[i] < targetValue)
					return i;
		}
	}
	//뒤쪽으로 찾는다.
	else
	{
		for(int i = count-1; i>=0 ; i--)
		{
			if(bLarger)
			{
				if(pDat[i] >targetValue)
					return i;
			}
			else
				if(pDat[i] <targetValue)
					return i;
		}
	}

	return 0;
}

