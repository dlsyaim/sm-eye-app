#pragma once
#include "ED_patientInfo.h"

// CDlgSubjectInfo ��ȭ �����Դϴ�.

class CDlgSubjectInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgSubjectInfo)

public:
	CDlgSubjectInfo(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgSubjectInfo();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SUBJECT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
