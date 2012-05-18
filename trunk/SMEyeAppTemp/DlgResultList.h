#pragma once
#include "afxcmn.h"
#include "ed_patientinfo.h"


// CDlgResultList ��ȭ �����Դϴ�.

class CDlgResultList : public CDialog
{
	DECLARE_DYNAMIC(CDlgResultList)

public:
	CDlgResultList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgResultList();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_RESULT_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

protected:
	CListCtrl m_ListTest;
	CString m_strDataFolder;
	CString	m_strSelectedFolder;		//����Ʈ ��Ʈ�ѿ��� ������ ����Ÿ


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
