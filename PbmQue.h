#pragma once

//#include "Option.h"

#include "afxtempl.h"

class CpbmQue
{
public:
	CpbmQue(void);
	~CpbmQue(void);

	

	void init();

	int addRawData(byte* pData, unsigned int count);
		
	double* getCurrentValue(){return m_currentData;};

private:
	double m_currentData[6];

	
};
