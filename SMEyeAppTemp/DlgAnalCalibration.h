#pragma once
#include "calibration.h"
#include ".\tchart\tchart.h"
#include "dlgcustomcalibrationsetting.h"
#include "ed_testinfo.h"


// CDlgAnalCalibration 대화 상자입니다.

class CDlgAnalCalibration : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnalCalibration)

public:
	CDlgAnalCalibration(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAnalCalibration();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ANALYZE_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	ED_TestInfo m_testInfo;
	CDlgCustomCalibrationSetting	m_dlgCustomCalibrationSetting;
	bool m_bCalibrationUsingGyro;

	CCalibration m_Calibration[2];
	CString m_strTestFile;
	double* m_pdLH;
	double* m_pdLV;
	double* m_pdRH;
	double* m_pdRV;

	double* m_pdStiH;
	double* m_pdStiV;
	unsigned long m_ulDataCount;

	void loadData();
	void allocVariable();
	void analyzeThis();
	void layoutCtrls();
	void showResult();

	
	CTChart m_chartLeftCalib;
	CTChart m_chartRightRaw;
	CTChart m_chartRightCalib;
	CTChart m_chartH;
	CTChart m_chartV;
	CTChart m_chartLeftRaw;

	bool m_bCustom;
	double m_ulSeperateTime[5];		//for custom

	void initTChart();

public:
	void setTestFile(CString strTestFile) {this->m_strTestFile = strTestFile;};
	CCalibration* getCalibration(){return m_Calibration;};
	void setCalbrationUsingGyro(bool bGyro);
	virtual BOOL OnInitDialog();

	bool loadAnalyzeData();
	bool saveAnalyzeData();

	
	DECLARE_EVENTSINK_MAP()
	void OnZoomTchartAc();
	void OnUndoZoomTchartAc();
	void OnScrollTchartAc();
	afx_msg void OnBnClickedButtonAcSave();
	afx_msg void OnBnClickedCheckCustomCalibration();
	afx_msg void OnBnClickedButtonCustomCalibrationAnalyze();
	afx_msg void OnBnClickedButtonCustomCalibrationSetting();
	afx_msg void OnBnClickedOk();
	
	
	void OnColorLineToolEndDragLineTchartAcH();
	void OnColorLineToolEndDragLineTchartAcV();
};
