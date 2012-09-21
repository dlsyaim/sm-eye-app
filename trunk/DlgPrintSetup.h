#pragma once
#include "treectrlxml.h"
#include "ed_patientinfo.h"
#include "ed_testinfo.h"

// CDlgPrintSetup ��ȭ �����Դϴ�.



class CDlgPrintSetup	 : public CDialog
{
	DECLARE_DYNAMIC(CDlgPrintSetup)

public:
	CDlgPrintSetup(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgPrintSetup();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PRINT_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


private:
	CTreeCtrlXML m_treePrint;
	CString m_strSubjectFolder;
	ED_PatientInfo m_patientInfo;

	

	CString m_strXMLName;
	CString m_strTIName;

	ED_TestInfo* m_pTestInfo;
public:
	void setSubjectFolder(CString strFolder) { m_strSubjectFolder = strFolder;};
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();



	void setTIFilename(CString xmlName, CString tiName) {m_strXMLName = xmlName; m_strTIName = tiName;};
	afx_msg void OnNMClickTreePrint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnTreeChecked(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonSelectAll();
	afx_msg void OnBnClickedButtonSelectNone();
	afx_msg void OnBnClickedCancel();
};
