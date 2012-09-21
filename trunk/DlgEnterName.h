#pragma once


// CDlgEnterName 대화 상자입니다.

class CDlgEnterName : public CDialog
{
	DECLARE_DYNAMIC(CDlgEnterName)

public:
	CDlgEnterName(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgEnterName();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ENTER_NEW_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	CString m_strNewName;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString getNewName(){return m_strNewName;};
};
