#pragma once
#include "afxcmn.h"
#include "treectrlxml.h"
#include "ed_patientinfo.h"

// CDlgSelectSpontaneous ��ȭ �����Դϴ�.

class CDlgSelectSpontaneous : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectSpontaneous)

public:
	CDlgSelectSpontaneous(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgSelectSpontaneous();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SPON_COMPENSATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	int m_nEyeSelect;
	CString m_strSpontaneousFileName;
	bool m_bManualSelect;
	double m_dSpontaneousValue[2];

	afx_msg void OnBnClickedRadioSponManualSelect();
	afx_msg void OnBnClickedRadioSponTestSelect();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	
	ED_PatientInfo m_patientInfo;
	CString m_strSubjectFolder;
	CTreeCtrlXML  m_treeSelectCompensation;
	afx_msg void OnTvnSelchangedTreeSelectCompensation(NMHDR *pNMHDR, LRESULT *pResult);

	void getSpontaneousValue();
};
