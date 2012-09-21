#pragma once

#include "ed_testinfo.h"
#include "afxwin.h"
#include "corwlDefines.h"
#include "ed_testprotocole.h"
#include "afxcmn.h"

///////////////////////////////////////////////////
// test opotin defines
////////////////////////////////////////////////////









// CDlgTestOption 대화 상자입니다.

class CDlgTestOption : public CDialog
{
	DECLARE_DYNAMIC(CDlgTestOption)

public:
	CDlgTestOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTestOption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TEST_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CListCtrl m_listEventString;


private:
	CComboBox m_comboAnalMethod;
	CComboBox m_comboAnalTarget;
	CComboBox m_comboStiSubmode;

	ED_TestInfo* m_pTestInfo;
	CString m_strTestName;

	DWORD m_stiRadioID[STI_NUM];

public:
	void setTestName(CString str) { m_strTestName = str;};
	CString getTestName() { return m_strTestName; };
	void setTestInfo(ED_TestInfo* pTestInfo) { this->m_pTestInfo = pTestInfo; };
	ED_TestInfo* getTestInfo(){return m_pTestInfo;};

	
	afx_msg void OnCbnSelchangeComboAnalMethod();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioStimulation();
	
	
	afx_msg void OnLvnEndlabeleditListEvent(NMHDR *pNMHDR, LRESULT *pResult);
};
