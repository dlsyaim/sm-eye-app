#pragma once
#include "ED_patientInfo.h"

// CDlgSubjectInfo 대화 상자입니다.

class CDlgSubjectInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgSubjectInfo)

public:
	CDlgSubjectInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgSubjectInfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SUBJECT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
	


protected:
	CTime m_time;

public:
	ED_PatientInfo m_PI;

};
