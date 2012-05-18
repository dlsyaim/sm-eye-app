#include "StdAfx.h"
#include ".\ea_analzemanager.h"
#include "corwlDefines.h"

EA_AnalzeManager::EA_AnalzeManager(void)
{
	init();
}

EA_AnalzeManager::~EA_AnalzeManager(void)
{
}

void EA_AnalzeManager::setResultSeries(CTChart* pResult, CTChart* pOrigin)
{
	//lr == 0: left
	//lr == 1: right
	this->m_Nystagmus[0].setResultSeries(pResult, pOrigin, SERIES_LEFT_EYE);
	this->m_Nystagmus[1].setResultSeries(pResult, pOrigin, SERIES_RIGHT_EYE);
}
void EA_AnalzeManager::init()
{
	this->m_Nystagmus[0].init();
	this->m_Nystagmus[1].init();

}
int EA_AnalzeManager::putSignal(double epl, double epr)
{
	int n0, n1;
	n0 = n1 = 0;
	
	n0 = m_Nystagmus[0].putSignal(epl);
	n1 = m_Nystagmus[1].putSignal(epr);

	return (n0 || n1);	
}