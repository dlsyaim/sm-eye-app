#pragma once
#include "ed_eyeinfo.h"
#define MAX_QUE	 128

class EU_QueEyeInfos
{
public:
	EU_QueEyeInfos(void);
	~EU_QueEyeInfos(void);

public:
	bool push(ED_EyeInfo* pEyeInfos);
	bool pop(ED_EyeInfo* pEyeInfos);
	void clear();
	bool empty();

	int getCount();

private:
	ED_EyeInfo m_eyeLeft[MAX_QUE];
	ED_EyeInfo m_eyeRight[MAX_QUE];

	int m_nQueStartIdx, m_nQueEndIdx;
	bool m_bEmpty;
};
