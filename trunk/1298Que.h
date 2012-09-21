#pragma once

#include "Afxtempl.h"

class C1298Data
{
public:
	C1298Data(void) {memset(ecg, 0, sizeof(double)*4);};
	double ecg[4];
};

class C1298Que
{
public:
	C1298Que(void);
	~C1298Que(void);

	void init();

	int addRawData(byte* pData, unsigned int count);

	
	BOOL isEmpty() {return m_1298List.IsEmpty();};
	C1298Data get() {return m_1298List.RemoveHead();};
	//CpbmData* get(){return m_pbmList.IsEmpty() ? NULL : m_pbmList.RemoveHead();};
	double* getEOG(){return m_currentEOGValue;};

	

private:
	CList<C1298Data, C1298Data&> m_1298List;

	double m_currentEOGValue[4];
	
	void add(C1298Data data) {m_1298List.AddTail(data);};

	

};
