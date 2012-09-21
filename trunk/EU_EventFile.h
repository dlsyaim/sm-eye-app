#pragma once
#include ".\tchart\tchart.h"


class EU_EventFile
{
public:
	EU_EventFile(void);
	~EU_EventFile(void);

	static void saveEventFile(CString fname, CTChart* pTChart, int seriesIdx);
	static void loadEventFile(CString fname, CTChart* pTChart, int seriesIdx,  bool bRemoveOffset);
	static int initEventSeries(CTChart* pTChart);
	static void adjustEventLocation(CTChart* pTChart, int seriesIdx);
};
