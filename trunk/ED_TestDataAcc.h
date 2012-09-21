#pragma once

#include ".\tchart\tchart.h"
#include ".\tchart\series.h"

class ED_TestDataAcc
{
public:
	ED_TestDataAcc(void);
	~ED_TestDataAcc(void);

	void writeData(CFile* pFile);
	void readData(CFile* pFile, long count);

	void setData(CSeries* pAccX, CSeries* pAccY, CSeries* pAccZ, CSeries* pZyroX, CSeries* pZyroY);
	void getData(CSeries* pAccX, CSeries* pAccY, CSeries* pAccZ, CSeries* pZyroX, CSeries* pZyroY, int freq);

private:
	void init();
	void alloc(long count);

private:
	long m_lDataCount;

	double* m_pdAccX;
	double* m_pdAccY;
	double* m_pdAccZ;
	double* m_pdZyroX;
	double* m_pdZyroY;
};
