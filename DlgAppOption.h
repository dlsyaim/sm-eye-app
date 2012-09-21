#pragma once

#include "ed_appoption.h"
#include "afxwin.h"
#include "calibration.h"

// CDlgAppOption 대화 상자입니다.

class CDlgAppOption : public CDialog
{
	DECLARE_DYNAMIC(CDlgAppOption)

public:
	CDlgAppOption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAppOption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_APP_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	ED_AppOption m_appOption;

public:
	void setCombo();
	ED_AppOption getAppOption(){return this->m_appOption;};
	void setAppOption(ED_AppOption option){this->m_appOption = option;};
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox m_comboComportTarget;
	CComboBox m_comboComportAcc;
	CComboBox m_comboComportEOG;
//	afx_msg void OnCbnSelchangeComboComportTarget();
	afx_msg void OnCbnSelchangeComboComportTarget();
	afx_msg void OnCbnSelchangeComboComportAcceleration();
	afx_msg void OnBnClickedButtonChangeDataFolder();

	afx_msg void OnBnClickedButtonOptionCalibrationLoad();
	
	afx_msg void OnCbnSelchangeComboComportEog();
	afx_msg void OnBnClickedCheckUseEog();
};
