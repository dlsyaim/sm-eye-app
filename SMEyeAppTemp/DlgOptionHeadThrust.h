#pragma once


// CDlgOptionHeadThrust 대화 상자입니다.

class CDlgOptionHeadThrust : public CDialog
{
	DECLARE_DYNAMIC(CDlgOptionHeadThrust)

public:
	CDlgOptionHeadThrust(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgOptionHeadThrust();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_HEAD_THRUST_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
