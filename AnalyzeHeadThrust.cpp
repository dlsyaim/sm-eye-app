#include "StdAfx.h"
#include ".\analyzeheadthrust.h"
#include "corwlDefines.h"

#include "math.h"
#include "linreg.h"

unsigned long findPeakAcc(double* pVel, unsigned long startIdx, unsigned long endIdx, int M, unsigned long count, int sign);
double getAcc(double* pVel , int startIdx, int M);

CAnalyzeHeadThrust::CAnalyzeHeadThrust(void)
{

	/*double x[] = { 71,  73,  64,  65,  61,  70,  65,  72,  63,  67,  64};
	double y[] = {160, 183, 154, 168, 159, 180, 145, 210, 132, 168, 141};
	LinearRegression lr(x, y, 11); 
	double b = lr.getB();
	double a = lr.getA();
	*/

	m_pdEyeVel = m_pdHeadVel = NULL;
	m_ulEyeDataCount = m_ulHeadDataCount = 0;

	m_bUseMaxHeadSpeedLimit = false;
	m_dMaxHeadSpeedLimit = 300;
	m_dMinHeadSpeedLimit = 100;


	//1초 데이터 버퍼
	m_ppMeanEyeVel[0] = new double[FRAMERATE];
	m_ppMeanEyeVel[1] = new double[FRAMERATE];
	m_ppMeanHeadVel[0] = new double[ACC_SAMPLERATE];
	m_ppMeanHeadVel[1] = new double[ACC_SAMPLERATE];

	memset(m_ppMeanEyeVel[0], 0, sizeof(double)*FRAMERATE);
	memset(m_ppMeanEyeVel[1], 0, sizeof(double)*FRAMERATE);
	memset(m_ppMeanHeadVel[0], 0, sizeof(double)*ACC_SAMPLERATE);
	memset(m_ppMeanHeadVel[1], 0, sizeof(double)*ACC_SAMPLERATE);

}

CAnalyzeHeadThrust::~CAnalyzeHeadThrust(void)
{
	DEL((m_ppMeanEyeVel[0]));
	DEL((m_ppMeanEyeVel[1]));
	DEL((m_ppMeanHeadVel[0]));
	DEL((m_ppMeanHeadVel[1]));

	this->deleteAllHTs();

}


void CAnalyzeHeadThrust::deleteAllHTs()
{
	int count = int(this->m_listHeadThrust.GetCount());

	POSITION pos = m_listHeadThrust.GetHeadPosition();
	for(int i=0; i<count; i++)
	{
		structHeadThrust* pHT = m_listHeadThrust.GetNext(pos);
		if(pHT)
			delete pHT;
	}

	m_listHeadThrust.RemoveAll();

}


unsigned long inverseFindLowerThan(double* pDat, unsigned long startIdx, int count, double thres);
unsigned long findPeak(double* pDat, unsigned long startIdx, int count, unsigned long lastIdx, int sign);
unsigned long findSlopeEnd(double* pDat, unsigned long startIdx, unsigned long lastIdx, int sign);

void CAnalyzeHeadThrust::analyze(double* pEyeVel, unsigned long eyeCount, double* pTarget, unsigned long targetCount)
{
	

	this->m_pdEyeVel = pEyeVel;
	this->m_ulEyeDataCount = eyeCount;
	this->m_pdHeadVel = pTarget;
	this->m_ulHeadDataCount = targetCount;


	deleteAllHTs();

	double peakThres = 50;
	double onsetThres = 10;

	unsigned long onsetIdx = 0;

	double ratio = double(FRAMERATE)/ACC_SAMPLERATE;
	int sign = 1;
	for(unsigned long i=0; i<targetCount; i++)
	{
		if(fabs(pTarget[i])>peakThres)
		{
			// head onset time을 찾는다.
			onsetIdx = inverseFindLowerThan(pTarget, i, int(ACC_SAMPLERATE/5), onsetThres);
			if(onsetIdx)
			{
				//data가 1초 이상 남아 있다면
				if(onsetIdx + ACC_SAMPLERATE < this->m_ulHeadDataCount)
				{
					structHeadThrust* pHT = new structHeadThrust;
					memset(pHT, 0, sizeof(structHeadThrust));


					pHT->headOnsetIdx = onsetIdx;
					pHT->eyeOnsetIdx = (unsigned long)(onsetIdx*ratio + .5);
 
					/*

					// slope의 끝을 찾아 onset과 slope끝의 중간에서 찾는 버젼
					unsigned long idxEyeSlopeEnd = findSlopeEnd(m_pdEyeVel, pHT->eyeOnsetIdx, eyeCount, sign);
					unsigned long idxHeadSlopeEnd = findSlopeEnd(m_pdHeadVel, pHT->eyeOnsetIdx, eyeCount, sign);
					unsigned long idxSlopeEnd = min(idxEyeSlopeEnd, idxHeadSlopeEnd);

					int len = idxSlopeEnd-pHT->headOnsetIdx; 
					if(len > 8)
					{
						//가운데에서 측정
						int M = int(len/4.+.5);
						pHT->eyePeakIdx = pHT->headPeakIdx = int((pHT->headOnsetIdx+idxSlopeEnd)/2.+.5);
						pHT->headPeakVel = getAcc(m_pdEyeVel, pHT->eyePeakIdx - M, 2*M);
						pHT->eyePeakVel = getAcc(m_pdHeadVel, pHT->headPeakIdx-M, 2*M);
					}*/


					unsigned long idxHeadSlopeEnd = findSlopeEnd(m_pdHeadVel, pHT->headOnsetIdx, eyeCount, sign);
					int len = idxHeadSlopeEnd-pHT->headOnsetIdx;

					
					//maximum acc를 찾는 버젼
					int M = 5;
					//가속도가 최대가 되는 지점을 찾는다.
					// 100ms 이내에 M개의 velocity가 증가 함수 혹은 감소함수인 것 중 가장 큰 것을 찾는다.
					// acc가 sign과 일치해야 한다.
					pHT->eyePeakIdx = findPeakAcc(m_pdEyeVel, pHT->eyeOnsetIdx, pHT->eyeOnsetIdx + MS100, M, eyeCount, 
								pTarget[onsetIdx+int(ACC_SAMPLERATE/10)] > 0 ? 1:-1);	//onset+50ms 위치의 sign으로 +,-를 찾는다.
					pHT->headPeakIdx = findPeakAcc(m_pdHeadVel, pHT->eyeOnsetIdx, pHT->eyeOnsetIdx + MS100, M, eyeCount, 
								pTarget[onsetIdx+int(ACC_SAMPLERATE/10)] > 0 ? 1:-1);	//onset+50ms 위치의 sign으로 +,-를 찾는다.
					pHT->eyePeakVel = pHT->eyePeakIdx ? getAcc(m_pdEyeVel, pHT->eyePeakIdx, M) : 0;
					pHT->headPeakVel = pHT->headPeakIdx ? getAcc(m_pdHeadVel, pHT->headPeakIdx, M) : 0;
					
					

					/*
					//head peak velocity를 찾는 버젼
					pHT->headPeakIdx = findPeak(pTarget, pHT->headOnsetIdx, ACC_SAMPLERATE/4, targetCount, 
											pTarget[onsetIdx+int(ACC_SAMPLERATE/20)] > 0 ? 1:-1);	//onset+50ms 위치의 sign으로 +,-를 찾는다.
					pHT->headPeakVel = pTarget[pHT->headPeakIdx];
					pHT->eyePeakIdx = findPeak(pEyeVel, pHT->eyeOnsetIdx, FRAMERATE/4, eyeCount,
											pTarget[onsetIdx+int(ACC_SAMPLERATE/20)] > 0 ? 1: -1);	//head sign과 같은 sign의 peak를 찾는다.
					pHT->eyePeakVel = pEyeVel[pHT->eyePeakIdx];*/
					pHT->valid = false;


					// 모든 parameter가 존재해야 한다.
					if(pHT->headOnsetIdx && pHT->headPeakIdx && pHT->eyeOnsetIdx && pHT->eyePeakIdx)
					{
						if((pHT->eyePeakIdx - pHT->eyeOnsetIdx < MS250) &&
							(pHT->headPeakIdx - pHT->headOnsetIdx < MS250) )
						{
							
							pHT->valid = true;
							//찾았다면 1000ms skip한다.

							// max head speed 를 제한할 것인지..
							if(this->m_bUseMaxHeadSpeedLimit && 
								((fabs(pHT->headPeakVel) > this->m_dMaxHeadSpeedLimit) || 
								(fabs(pHT->headPeakVel) < this->m_dMinHeadSpeedLimit)))
								pHT->valid = false;
							
						}
					}

					if(pHT->valid == false)
						pHT->eyePeakVel = 0;
					
					
					//valid하지 않아도 무조건 넣어준다. 그래야 좌우가 같은 개수가 된다.
					i += ACC_SAMPLERATE;
					m_listHeadThrust.AddTail(pHT);
				}
			}

		}
	}


	//mean plot을 분석한다.
	this->calculateMeanVelocity();
	
}
unsigned long inverseFindLowerThan(double* pDat, unsigned long startIdx, int count, double thres)
{
	// startIdx로부터 count개수만큼 역방향으로 
	// 절대값이 thres보다 작은 값이 나올 때까지
	unsigned long idx = 0;

	for(int i=0; i<count; i++)
	{
		if(startIdx-i < 0)
		{
			idx = 0;
			break;
		}
		if(fabs(pDat[startIdx-i]) < thres)
		{
			idx = startIdx-i;
			break;
		}
	}

	return idx;
}

unsigned long findPeak(double* pDat, unsigned long startIdx, int count, unsigned long lastIdx, int sign)
{
	unsigned long idx = 0;

	double mul = (sign>0) ? 1:-1;

	double peakVal = 0;
	unsigned long peakIdx = 0;


	for(int i=0; i<count; i++)
	{
		if( (startIdx+i < lastIdx) &&(pDat[startIdx+i]*mul > peakVal*mul) )
		{
			peakVal = pDat[startIdx+i];
			peakIdx = startIdx+i;
		}

	}

	return peakIdx;
}

unsigned long findSlopeEnd(double* pDat, unsigned long startIdx, unsigned long lastIdx, int sign)
{
	double mul = (sign>0) ? 1:-1;

	unsigned long endIdx = 0;

	//증가함수의 끝을 찾는다.
	if(sign > 0)
	{
		for(endIdx = startIdx; endIdx<lastIdx-1; endIdx++)
		{
			if(pDat[endIdx+1] - pDat[endIdx] <= 0)
				break;
		}
	}
	else
	{
		for(endIdx = startIdx; endIdx<lastIdx-1; endIdx++)
		{
			if(pDat[endIdx+1] - pDat[endIdx] >= 0)
				break;
		}
	}
	
	return endIdx;
}

/*
void CAnalyzeHeadThrust::deleteAllHeadThrust()
{
	if(!this->m_listHeadThrust.IsEmpty())
	{
		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listHeadThrust.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structHeadThrust* pHT = m_listHeadThrust.GetNext(pos1);
			delete pHT;
		}

		this->m_listHeadThrust.RemoveAll();
	}


}*/

void addA2B(double* pA, double* pB, int count);
void CAnalyzeHeadThrust::calculateMeanVelocity()
{
	//positive, negative 끼리 head, eye velocity의 평균 파형을 구한다.
	
	
	//평균 파형의 초기화
	memset(m_ppMeanEyeVel[0], 0, sizeof(double)*FRAMERATE);
	memset(m_ppMeanEyeVel[1], 0, sizeof(double)*FRAMERATE);
	memset(m_ppMeanHeadVel[0], 0, sizeof(double)*ACC_SAMPLERATE);
	memset(m_ppMeanHeadVel[1], 0, sizeof(double)*ACC_SAMPLERATE);
	
	//평균파형 분석값 초기화
	memset(&(m_structMeanHT[0]), 0, sizeof(structHeadThrust));
	memset(&(m_structMeanHT[1]), 0, sizeof(structHeadThrust));

	int count = int(this->m_listHeadThrust.GetCount());
	POSITION pos = m_listHeadThrust.GetHeadPosition();

	int posCount = 0, negCount = 0;
	for(int i=0; i<count; i++)
	{
		structHeadThrust* pHT = m_listHeadThrust.GetNext(pos);
		//valid한 것만 계산한다.
		if(pHT && pHT->valid)
		{
			if(pHT->headPeakVel > 0)
			{
				//positive 파형끼리 더한다.
				addA2B(&(m_pdEyeVel[pHT->eyeOnsetIdx]), m_ppMeanEyeVel[0], min(FRAMERATE, m_ulEyeDataCount-pHT->eyeOnsetIdx));
				addA2B(&(m_pdHeadVel[pHT->headOnsetIdx]), m_ppMeanHeadVel[0], min(ACC_SAMPLERATE, m_ulHeadDataCount-pHT->headOnsetIdx));
				posCount++;
			}
			else
			{
				//negative 파형끼리 더한다.
				addA2B(&(m_pdEyeVel[pHT->eyeOnsetIdx]), m_ppMeanEyeVel[1], min(FRAMERATE, m_ulEyeDataCount-pHT->eyeOnsetIdx));
				addA2B(&(m_pdHeadVel[pHT->headOnsetIdx]), m_ppMeanHeadVel[1], min(ACC_SAMPLERATE, m_ulHeadDataCount-pHT->headOnsetIdx));
				negCount++;
			}
		}
	}

	//개수로 나누어 준다.
	if(posCount)
	{
		for(int i=0; i<FRAMERATE; i++)
			m_ppMeanEyeVel[0][i] /= posCount;
		for(int i=0; i<ACC_SAMPLERATE; i++)
			m_ppMeanHeadVel[0][i] /= posCount;
	}

	if(negCount)
	{
		for(int i=0; i<FRAMERATE; i++)
			m_ppMeanEyeVel[1][i] /= negCount;
		for(int i=0; i<ACC_SAMPLERATE; i++)
			m_ppMeanHeadVel[1][i] /= negCount;
	}

	//분석한다.
	int pn;
	int M = 3;
	//positive
	if(posCount)
	{
		//onset은 모두 0
		pn = HEAD_THRUST_POSITIVE;
		this->m_structMeanHT[pn].eyeOnsetIdx = m_structMeanHT[pn].headOnsetIdx = 0;


		
		//가속도가 최대가 되는 지점을 찾는다.
		// 100ms 이내에 M개의 velocity가 증가 함수 혹은 감소함수인 것 중 가장 큰 것을 찾는다.
		// acc가 sign과 일치해야 한다.
		m_structMeanHT[pn].eyePeakIdx = findPeakAcc(m_ppMeanEyeVel[pn], 0, MS100, M, FRAMERATE-1, 1);
		m_structMeanHT[pn].headPeakIdx = findPeakAcc(m_ppMeanHeadVel[pn], 0, MS100, M, FRAMERATE-1, 1); 
		m_structMeanHT[pn].eyePeakVel = getAcc(m_ppMeanEyeVel[pn], m_structMeanHT[pn].eyePeakIdx, M) ;
		m_structMeanHT[pn].headPeakVel = getAcc(m_ppMeanHeadVel[pn], m_structMeanHT[pn].headPeakIdx, M) ;


		/*
		//eye peak index와 peak 값을 구한다.
		m_structMeanHT[pn].eyePeakIdx = findPeak(m_ppMeanEyeVel[pn], 0, FRAMERATE/4, FRAMERATE-1, 1);
		m_structMeanHT[pn].eyePeakVel = m_ppMeanEyeVel[pn][m_structMeanHT[pn].eyePeakIdx];
		
		//head peak index와 peak값을 구하낟.
		m_structMeanHT[pn].headPeakIdx = findPeak(m_ppMeanHeadVel[pn], 0, ACC_SAMPLERATE, ACC_SAMPLERATE-1, 1);
		m_structMeanHT[pn].headPeakVel = m_ppMeanHeadVel[pn][m_structMeanHT[pn].headPeakIdx];
		*/
	}
	if(negCount)
	{
		//onset은 모두 0
		pn = HEAD_THRUST_NEGATIVE;
		this->m_structMeanHT[pn].eyeOnsetIdx = m_structMeanHT[pn].headOnsetIdx = 0;

		m_structMeanHT[pn].eyePeakIdx = findPeakAcc(m_ppMeanEyeVel[pn], 0, MS100, M, FRAMERATE-1, -1);
		m_structMeanHT[pn].headPeakIdx = findPeakAcc(m_ppMeanHeadVel[pn], 0, MS100, M, FRAMERATE-1, -1); 
		m_structMeanHT[pn].eyePeakVel = getAcc(m_ppMeanEyeVel[pn], m_structMeanHT[pn].eyePeakIdx, M) ;
		m_structMeanHT[pn].headPeakVel = getAcc(m_ppMeanHeadVel[pn], m_structMeanHT[pn].headPeakIdx, M) ;

		/*
		//eye peak index와 peak 값을 구한다.
		m_structMeanHT[pn].eyePeakIdx = findPeak(m_ppMeanEyeVel[pn], 0, FRAMERATE/4, FRAMERATE-1, -1);
		m_structMeanHT[pn].eyePeakVel = m_ppMeanEyeVel[pn][m_structMeanHT[pn].eyePeakIdx];
		
		//head peak index와 peak값을 구하낟.
		m_structMeanHT[pn].headPeakIdx = findPeak(m_ppMeanHeadVel[pn], 0, ACC_SAMPLERATE, ACC_SAMPLERATE-1, -1);
		m_structMeanHT[pn].headPeakVel = m_ppMeanHeadVel[pn][m_structMeanHT[pn].headPeakIdx];
		*/
	}
}


void addA2B(double* pA, double* pB, int count)
{
	for(int i=0; i<count; i++)
		pB[i] += pA[i];

}
	

int CAnalyzeHeadThrust::getValidHTCount(int sign)
{
	//sign이 0이면 전체 개수, 아니면 같은 부호의 Head velocity값의 개수만 리턴
	int count = int(this->m_listHeadThrust.GetCount());
	int num = 0;

	//전체 개수
	if(sign == 0)
		return count;

	POSITION pos = this->m_listHeadThrust.GetHeadPosition();
	structHeadThrust* pHT = NULL;
	for(int i=0; i<count; i++)
	{
		pHT = m_listHeadThrust.GetNext(pos);
		if(pHT && pHT->headPeakVel*sign >= 0 && pHT->valid)
			num++;
	}

	return num;
}

int CAnalyzeHeadThrust::findHT(double headPeakVel, double eyePeakVel)
{
	int idx = -1;
	int count = int(this->m_listHeadThrust.GetCount());

	POSITION pos = m_listHeadThrust.GetHeadPosition();
	structHeadThrust* pHT = NULL;
	for(int i=0; i<count; i++)
	{
		pHT = m_listHeadThrust.GetNext(pos);
		if(pHT && pHT->valid && (headPeakVel == pHT->headPeakVel) && 
			(eyePeakVel == pHT->eyePeakVel))
		{
			idx = i;
			break;
		}
	}

	return idx;
}

int CAnalyzeHeadThrust::findHTLocation(structHeadThrust* pHT)	//pHT가 들어 있는 item의 위치를 리턴해 준다.
{
	int idx = -1;
	int count = int(m_listHeadThrust.GetCount());
	POSITION pos = m_listHeadThrust.GetHeadPosition();
	for(int i=0; i<count; i++)
	{
		if(m_listHeadThrust.GetNext(pos) == pHT)
			idx = i;
	}

	return idx;
}

bool CAnalyzeHeadThrust::deleteHT(int idx)
{
	bool bRtn = false;

	
	POSITION pos = m_listHeadThrust.FindIndex(idx);
	structHeadThrust* pHT = m_listHeadThrust.GetAt(pos);
	delete pHT;
	pHT = NULL;
	m_listHeadThrust.RemoveAt(pos);
	
	
	this->calculateMeanVelocity();
	/*
	int count = m_listHeadThrust.GetCount();
	if(idx<count)
	{
		POSITION pos = m_listHeadThrust.GetHeadPosition();
		pHT = m_listHeadThrust.GetNext(pos);
		for(int i=0; i<idx; i++)
			pHT = m_listHeadThrust.GetNext(pos);
		if(pHT)
		{
			delete pHT;
			pHT = NULL;
			pHT = m_listHeadThrust.FindIndex.GetAt(pos);
			m_listHeadThrust.RemoveAt(pos);
			bRtn = true;
		}
	}*/

	return bRtn;
}

bool CAnalyzeHeadThrust::save(CString fname)
{
	int n = sizeof(structHeadThrust);

	bool bRtn = false;
	CFile file;
	if(file.Open(fname, CFile::modeWrite | CFile::modeCreate))
	{
		unsigned char buf[256];
		::ZeroMemory(buf, 256);

		POSITION pos1, pos2;
		double val = 0;
		for(pos1 = this->m_listHeadThrust.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structHeadThrust* pHT = m_listHeadThrust.GetNext(pos1);
			memcpy(buf, pHT, sizeof(structHeadThrust));
			file.Write(buf, 256);
		}

		file.Close();
		bRtn = true;
	}

	return bRtn;
}


bool CAnalyzeHeadThrust::load(CString fname)
{
	bool bRtn = false;
	CFile file;
	if(file.Open(fname, CFile::modeRead))
	{
		//현재 DATA를 모두 지운다.
		this->deleteAllHTs();

		unsigned char buf[256];
		::ZeroMemory(buf, 256);

		unsigned int count = (unsigned int)(file.GetLength()/256);
		for(unsigned int i=0; i<count; i++)
		{
			structHeadThrust* pHT= new structHeadThrust;
			file.Read(buf, 256);
			::memcpy(pHT, buf, sizeof(structHeadThrust));

			if(pHT->eyeOnsetIdx < this->m_ulEyeDataCount+FRAMERATE)
                this->m_listHeadThrust.AddTail(pHT);
			else
				int k = 0;
			
		}

		file.Close();

		this->calculateMeanVelocity();
		bRtn = true;
	}

	return bRtn;
}


unsigned long findPeakAcc(double* pVel, unsigned long startIdx, unsigned long endIdx, int M, unsigned long count, int sign)
{
	double maxVal = 0;
	unsigned long maxIdx = 0;

	double acc;
	for(unsigned long i = startIdx; i<=endIdx; i++)
	{
		if(i+M < count)
		{
			bool bSameSlopeSign = true;
			int slopeSign = pVel[startIdx+1] - pVel[startIdx];	//증가 혹은 감소 함수인지 확인
			for(int j=0; j<M-1; j++)
			{
				if((pVel[i+j+1] - pVel[i+j]) * slopeSign <0)
					bSameSlopeSign = false;
			}
			
			//증가 혹은 감소 함소일 경우에만
			if(bSameSlopeSign)
			{
				acc = getAcc(pVel, i, M);
				if(acc* sign > 0 && fabs(acc) > fabs(maxVal))
				{
					maxVal = acc;
					maxIdx = i;
				}
			}
			//else
              //  break;
		}

	}

	return maxIdx;

	/*
	unsigned long idx = 0;

	double mul = (sign>0) ? 1:-1;

	double peakVal = 0;
	unsigned long peakIdx = 0;


	for(int i=0; i<count; i++)
	{
		if( (startIdx+i < lastIdx) &&(pDat[startIdx+i]*mul > peakVal*mul) )
		{
			peakVal = pDat[startIdx+i];
			peakIdx = startIdx+i;
		}

	}

	return peakIdx;*/
}
double getAcc(double* pVel , int startIdx, int M)
{
	double acc = 0;

	double* pX = new double[M];
	for(int i=0; i<M; i++)
		pX[i] = i*(1.0f/FRAMERATE);

	double* pY = &(pVel[startIdx]);
	
	LinearRegression lr(pX, pY, M); 
	acc = lr.getB();
	
	delete []pX;

	return acc;
}