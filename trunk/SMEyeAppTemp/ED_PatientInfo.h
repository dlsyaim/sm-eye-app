#pragma once

#define MAX_PI_NAME		64
#define MAX_PI_ID		64
#define MAX_PI_MEMO		512

class ED_PatientInfo
{
public:
	ED_PatientInfo(void);
	~ED_PatientInfo(void);
	

	int m_nVer;
	CTime m_timeTime;
	int  m_nSex;				//1ÀÌ¸י Male
	int  m_nAge;
	char m_chName[MAX_PI_NAME];
	char m_chID[MAX_PI_ID];
	char m_chMemo[MAX_PI_MEMO];

	

public:
	bool save(CString str);
	bool load(CString str);
	bool refreshTime(){this->m_timeTime.GetCurrentTime();}
	CString makeSubjectFolderName();
	CString makePIString();
	

	ED_PatientInfo& operator=(const ED_PatientInfo &rhs)
	{
		if(this!=&rhs)
		{
			m_nVer = rhs.m_nVer;
			::memcpy(m_chName, rhs.m_chName, MAX_PI_NAME);
			::memcpy(m_chID, rhs.m_chID, MAX_PI_ID);
			m_nSex = rhs.m_nSex;
			m_nAge = rhs.m_nAge;
			m_timeTime = rhs.m_timeTime;
			::memcpy(m_chMemo, rhs.m_chMemo, MAX_PI_MEMO);

		}
		return *this;
	};

};
