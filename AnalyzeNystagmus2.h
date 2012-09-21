#pragma once

#include ".\ea_nystagmus.h"


class CAnalyzeNystagmus2
{
public:
	CAnalyzeNystagmus2(void);
	~CAnalyzeNystagmus2(void);

	void analyze(double* pEye, unsigned long count);
	void analyze2(double* pEye, unsigned long count);
	CString getResultString();

	int getCount(int nysType);
	double getMean(int nysType);
	double getMax(int nysType);
	double getMin(int nysType);
		
	bool load(CString fname);		//분석 결과 load
	bool save(CString fname);		//분석 결과 save

	CList<structNystag*, structNystag*> m_listNystagmus;
	unsigned long getClosestNystagmus(double t);

	unsigned long addNystagmus(unsigned long idx, double* pEye, unsigned long count);
	unsigned long addNystagmus2(unsigned long idx, double* pEye, unsigned long count);

	void deleteNysWithDirection(int direction);
private:
	void deleteAllNystagmus();
};
