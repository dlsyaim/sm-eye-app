#pragma once
#include ".\tchart\tchart.h"


class CAnalyzeBase
{
public:
	CAnalyzeBase(void);
	~CAnalyzeBase(void);

protected:
	int	m_AvailableChartNum;
	CListCtrl* m_pListCtrlSummary;
public:
	void setListCtrl(CListCtrl* pListCtrl);

	virtual void layoutTChart(CTChart** ppTChart, unsigned char chartNum, CRect rect);
	virtual void initTChart(CTChart** ppTChart);
	virtual void analyze(double* pDat, unsigned long count, CTChart** ppTChart, unsigned char chartNum);
	virtual void analyze(double* pLh, double* pLv, double* pRh, double* pRv, double* pAx, double* pAy, double* pAz, double* pGx, double* pGy, unsigned long count, CTChart** ppTChart);
	virtual void fillListCtrl();
};
