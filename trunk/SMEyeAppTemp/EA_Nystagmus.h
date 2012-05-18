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

	double m_NysVelPeakBuf[NYS_BUF_LEN];		//fast phase�� velocity
	unsigned long m_NysVelPeakIndexBuf[NYS_BUF_LEN];		//peak�� index
	//double m_NysVelBuf[NYS_BUF_LEN];			//slowphase�� 
	int m_NysBufCount;

	structNystag m_structCurrentNystag, m_structPrevNystag;
	unsigned long m_prevMaxEndI;

	int temp0[10];


	//�м� parameters
	unsigned int m_paramDerivM;	//velocity�� ���� delay
	double m_paramDerivThres;	//peak threshold
	double m_paramDerivMax;	//derivative�� �������ִ� �ִ밪
	double m_paramDerivNoiseLevel;	//derivative noise level
	int m_paramContPeakNum;			//���� ��ȣ�� peak�� �������� �� ������ ���� ���;� nystagmus fast phase�̴�.
	int m_paramContPeakDuration;		//m_paramContPeakNum���� peak�� ����µ� �ɸ� �ð�
	int m_paramNysMinLength;		//slow phase �ּ� ���� �ð�
	int m_paramNysMaxLength;		//slow phase �ִ� ���� �ð�
	double m_paramVelMinMax;		//slow phase velocity�� min max ����

};
