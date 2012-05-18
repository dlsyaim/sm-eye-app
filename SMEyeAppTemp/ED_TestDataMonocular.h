#include ".\tchart\tchart.h"
//#include ".\tchart\series.h"

#pragma once

class ED_TestDataMonocular
{
public:
	ED_TestDataMonocular(void);
	~ED_TestDataMonocular(void);

	
public:
	void writeData(CFile* pFile);
	void readData(CFile* pFile, long count);
	ED_TestDataMonocular& operator=(const ED_TestDataMonocular &rhs);
	void setData(double* pCx, double* pCy, double* pRadius, double* pTorsion, bool* pPupilValid, bool* pTorsionValid, long count);
	void setData(CSeries* pSerieCx, CSeries* pSerieCy, CSeries* pSerieR, CSeries* pSerieT);
	void getData(CSeries* pSerieCx, CSeries* pSerieCy, CSeries* pSerieR, CSeries* pSerieT, int freq);
	

	long m_lDataCount;

private:
	void init();
	void alloc(long count);

private:
	double* m_pdCx;
	double* m_pdCy;
	double* m_pdRadius;
	double* m_pdTorsion;
	bool* m_pbPupilValid;
	bool* m_pbTorsionValid;


	CSeries* m_pSeries[4];
};
