#include "StdAfx.h"
#include ".\eu_queeyeinfos.h"

EU_QueEyeInfos::EU_QueEyeInfos(void)
{
	clear();
}

EU_QueEyeInfos::~EU_QueEyeInfos(void)
{
}

void EU_QueEyeInfos::clear()
{
	m_bEmpty = true;

	::memset(m_eyeLeft, 0, sizeof(ED_EyeInfo)*MAX_QUE);
	::memset(m_eyeRight, 0, sizeof(ED_EyeInfo)*MAX_QUE);

	m_nQueStartIdx = 0;
	m_nQueEndIdx = -1;
}

bool EU_QueEyeInfos::push(ED_EyeInfo* pEyeInfos)
{
	//TRACE("///////////////push \n");

//	TRACE("PUSH: startIdx %d, endIdx %d,  \n", m_nQueStartIdx,m_nQueEndIdx);

	if(++m_nQueEndIdx == MAX_QUE)
		m_nQueEndIdx -= MAX_QUE;

	//비어있지 않았는데 end index가 start index를 따라잡았다면 
	//overflow
	if((!m_bEmpty) && (m_nQueEndIdx == m_nQueStartIdx))
	{
		//TRACE("////////////////////////////////////////////////////  overflow\n");

		::AfxMessageBox("Que Overflow!!! \n");
		return false;					// que overflow
	}


	m_eyeLeft[m_nQueEndIdx] = pEyeInfos[0];
	m_eyeRight[m_nQueEndIdx] = pEyeInfos[1];

	m_bEmpty = false;
	

//	TRACE("PUSH: startIdx %d, endIdx %d,  \n", m_nQueStartIdx,m_nQueEndIdx);

	return true;
}

bool EU_QueEyeInfos::pop(ED_EyeInfo* pEyeInfos)
{
	if(!m_bEmpty)
	{
		//TRACE("//////////////////////////////// pop \n");

		pEyeInfos[0] = m_eyeLeft[m_nQueStartIdx];
		pEyeInfos[1] = m_eyeRight[m_nQueStartIdx];
        if(++m_nQueStartIdx == MAX_QUE)
			m_nQueStartIdx -= MAX_QUE;

		//TRACE("POP: startIdx %d, endIdx %d \n", m_nQueStartIdx,m_nQueEndIdx);

		if(getCount() == 0)
			m_bEmpty = true;
	}
	else
		return false;



	return true;
}

int EU_QueEyeInfos::getCount()
{
	if(m_bEmpty)
		return 0;
	else
	{
		int count = m_nQueEndIdx - m_nQueStartIdx +1;
		if(count < 0)
			count += MAX_QUE;
		if(count == MAX_QUE)
			count = 0;
		//TRACE("startIdx %d, endIdx %d, count %d \n", m_nQueStartIdx,m_nQueEndIdx, count);
		return count;
	}
	
}

bool EU_QueEyeInfos::empty()
{
	return m_bEmpty;
}
