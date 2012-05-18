#pragma once
#include "corwlDefines.h"
#include ".\tchart\tchart.h"
//#include ".\tchart\series.h"

#define EYE_BUF_LEN		(FRAMERATE*2)
#define NYS_BUF_LEN		10

struct structNystag
{
	unsigned long startI;
	unsigned long endI;
	double	vel;
};

class EA_Nystagmus
{
public:
	EA_Nystagmus(void);
	~EA_Nystagmus(void);

	void init();
	int putSignal(double ep);	//ep: eye position
	
	void setResultSeries(CTChart* pTResult, CTChart* pTOriginal, int seriesIndex);
public:
	CTChart* m_pTChartResult;
	int m_seriesResultIndex;

	CTChart* m_pTChartOriginal;

	unsigned long m_ulDataCount;
	double m_EyePosBuf[EYE_BUF_LEN];
	double m_EyeVelBuf[EYE_BUF_LEN];
	double m_prevVel[2];
	int m_EyeBufCount;

	double m_NysVelPeakBuf[NYS_BUF_LEN];		//fast phase의 velocity
	unsigned long m_NysVelPeakIndexBuf[NYS_BUF_LEN];		//peak의 index
	//double m_NysVelBuf[NYS_BUF_LEN];			//slowphase의 
	int m_NysBufCount;

	structNystag m_structCurrentNystag, m_structPrevNystag;
	unsigned long m_prevMaxEndI;

	int temp0[10];


	//분석 parameters
	unsigned int m_paramDerivM;	//velocity를 구할 delay
	double m_paramDerivThres;	//peak threshold
	double m_paramDerivMax;	//derivative가 가질수있는 최대값
	double m_paramDerivNoiseLevel;	//derivative noise level
	int m_paramContPeakNum;			//같은 부호의 peak가 연속으로 이 값보다 많이 나와야 nystagmus fast phase이다.
	int m_paramContPeakDuration;		//m_paramContPeakNum개의 peak가 생기는데 걸린 시간
	int m_paramNysMinLength;		//slow phase 최소 유지 시간
	int m_paramNysMaxLength;		//slow phase 최대 유지 시간
	double m_paramVelMinMax;		//slow phase velocity의 min max 차이

};
