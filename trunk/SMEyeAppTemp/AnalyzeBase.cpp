#include "StdAfx.h"
#include ".\analyzebase.h"

CAnalyzeBase::CAnalyzeBase(void)
{
	m_pListCtrlSummary = NULL;
}

CAnalyzeBase::~CAnalyzeBase(void)
{
}

void CAnalyzeBase::analyze(double* pDat, unsigned long count, CTChart** ppTChart, unsigned char seriesNum)
{

}
void CAnalyzeBase::analyze(double* pLh, double* pLv, double* pRh, double* pRv, double* pAx, double* pAy, double* pAz, double* pGx, double* pGy, unsigned long count, CTChart** ppTChart)
{
}

void CAnalyzeBase::initTChart(CTChart** ppTChart)
{
}

void CAnalyzeBase::layoutTChart(CTChart** ppTChart, unsigned char chartNum, CRect rect)
{
}

void CAnalyzeBase::fillListCtrl()
{
}

void CAnalyzeBase::setListCtrl(CListCtrl* pListCtrl)
{
	this->m_pListCtrlSummary = pListCtrl;
}