#pragma once
#include "afxcmn.h"
#include "ed_patientinfo.h"


// CDlgResultList 대화 상자입니다.

class CDlgResultList : public CDialog
{
	DECLARE_DYNAMIC(CDlgResultList)

public:
	CDlgResultList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgResultList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RESULT_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	CListCtrl m_ListTest;
	CString m_strDataFolder;
	CString	m_strSelectedFolder;		//리스트 컨트롤에서 선택한 데이타


public:
	void setListFolder(CString strDataFolder) {this->m_strDataFolder = strDataFolder;};
	CString getSelectedFolder(); 

protected:
	void initTestList();
	void fillTestList(CString strFolderName);
	void add2TestList(ED_PatientInfo* pPI, CString fname, int idx);
	void showMemo(CString fname);


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickListTestList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListTestList(NMHDR *pNMHDR, LRESULT *pResult);
};
