#pragma once


// CDlgEnterName ��ȭ �����Դϴ�.

class CDlgEnterName : public CDialog
{
	DECLARE_DYNAMIC(CDlgEnterName)

public:
	CDlgEnterName(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgEnterName();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ENTER_NEW_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	CString m_strNewName;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString getNewName(){return m_strNewName;};
};
