#include "StdAfx.h"
#include ".\ed_patientinfo.h"
#include "corwlutil.h"
#define PATIENT_INFO_SIZE		2048

ED_PatientInfo::ED_PatientInfo(void)
{
	m_nVer = 0;
	memset(m_chName, 0, MAX_PI_NAME);
	memset(m_chID, 0, MAX_PI_ID);
	m_nSex = 0;		//femail
	m_nAge = 0;
	memset(m_chMemo, 0, MAX_PI_MEMO);
	m_timeTime = CTime::GetCurrentTime();
	memset(m_chTester, 0, MAX_PI_TESTER);
	memset(m_chDoctor, 0, MAX_PI_DOCTOR);

}

ED_PatientInfo::~ED_PatientInfo(void)
{
		
}

bool ED_PatientInfo::save(CString fname)
{
	byte buf[PATIENT_INFO_SIZE];
	::memset(buf, 0, PATIENT_INFO_SIZE);
	::memcpy(buf, this, sizeof(ED_PatientInfo));

	CFile file;
	if(file.Open(fname, CFile::modeWrite | CFile::modeCreate))
	{
		file.Write(buf, PATIENT_INFO_SIZE);
		file.Close();
		return true;
	}

	return false;
}

bool ED_PatientInfo::load(CString fname)
{
	byte buf[PATIENT_INFO_SIZE];

	CFile file;
	if(file.Open(fname, CFile::modeRead) && (file.GetLength() >= PATIENT_INFO_SIZE))
	{
		file.Read(buf, PATIENT_INFO_SIZE);
		file.Close();
		memcpy(this, buf, sizeof(ED_PatientInfo));
		return true;
	}

	return false;
}


CString ED_PatientInfo::makeSubjectFolderName()
{
	CString strFolderName;
	CTimeSpan tspan;
	strFolderName.Format("%04d%02d%02d_%02d%02d%02d_%s_%s", 
		m_timeTime.GetYear(), m_timeTime.GetMonth(), m_timeTime.GetDay(),
		m_timeTime.GetHour(), m_timeTime.GetMinute(), m_timeTime.GetSecond(),
		m_chID,
		m_chName);
	return strFolderName;
}


CString ED_PatientInfo::makePIString()
{
	CString strPI;
	strPI.Format("Doctor: %s \n\nTester: %s \n\nID: %s \n\nName: %s\n\nAge: %d\n\nSex: %s", 
		this->m_chDoctor, this->m_chTester,
		m_chID, m_chName, m_nAge, m_nSex?"M":"F");
	return strPI;
}