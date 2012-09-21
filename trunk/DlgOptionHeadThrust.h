#pragma once


// CDlgOptionHeadThrust ��ȭ �����Դϴ�.

class CDlgOptionHeadThrust : public CDialog
{
	DECLARE_DYNAMIC(CDlgOptionHeadThrust)

public:
	CDlgOptionHeadThrust(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgOptionHeadThrust();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_HEAD_THRUST_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	bool m_bUseMaxHeadSpeedLimit;
	double m_dMaxHeadSpeedLimit;
	double m_dMinHeadSpeedLimit;

protected:
	void loadParameters();
	void saveParameters();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckLimitMaxHeadSpeed();
};
