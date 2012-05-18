#pragma once


// CDlgCustomCalibrationSetting 대화 상자입니다.

class CDlgCustomCalibrationSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgCustomCalibrationSetting)

public:
	CDlgCustomCalibrationSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCustomCalibrationSetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CUSTOM_CALIBRATION_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	double m_dLeftRightDegree;
	double m_dTopBottomDegree;
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
