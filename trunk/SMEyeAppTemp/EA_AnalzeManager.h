#pragma once
#include "ea_nystagmus.h"
#include ".\tchart\tchart.h"

class EA_AnalzeManager
{
public:
	EA_AnalzeManager(void);
	~EA_AnalzeManager(void);

	void setResultSeries(CTChart* pResult, CTChart* pOrigin);
	void init();
	int putSignal(double epl, double epr);
	
public:
	EA_Nystagmus m_Nystagmus[2];
};
