#pragma once


// CDlgCustomCalibrationSetting ��ȭ �����Դϴ�.

class CDlgCustomCalibrationSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgCustomCalibrationSetting)

public:
	CDlgCustomCalibrationSetting(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCustomCalibrationSetting();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CUSTOM_CALIBRATION_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	double m_dLeftRightDegree;
	double m_dTopBottomDegree;
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
