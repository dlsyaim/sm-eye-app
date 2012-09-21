#pragma once
#include "analyzebase.h"
#include ".\ea_nystagmus.h"




class CAnalyzeNystagmus :
	public CAnalyzeBase
{
public:
	CAnalyzeNystagmus(void);
	~CAnalyzeNystagmus(void);

	//////////////////////////////////////////////////////////////////////////////////////////////
	void initTChart(CTChart** ppTChart);
	void layoutTChart(CTChart** ppTChart, unsigned char chartNum, CRect rect);
	void analyze(double* pDat, unsigned long count, CTChart** ppTChart, unsigned char seriesNum);
	void analyze(double* pDat, unsigned long count, int LR);


	void fillListCtrl();

	CList<structNystag, structNystag> m_listNystag;
	
	int getCount(int nysType);
	double getMean(int nysType);
	double getMax(int nysType);
	double getMin(int nysType);
	//////////////////////////////////////////////////////////////////////////////////////////////
	CList<structNystag, structNystag> m_listNys[2];
	int getCount(int nysType, int LR);
	double getMean(int nysType, int LR);
	double getMax(int nysType, int LR);
	double getMin(int nysType, int LR);
	

};
