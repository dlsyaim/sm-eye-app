#pragma once

#include ".\tchart\tchart.h"
//#include "analyzenystagmus2.h"

#include "analyzeCaloric.h"
#include "ed_patientinfo.h"
#include "ed_patientinfo.h"



// CDlgAnalyzeCaloric ��ȭ �����Դϴ�.

class CDlgAnalyzeCaloric : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnalyzeCaloric)

public:
	CDlgAnalyzeCaloric(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgAnalyzeCaloric();
#include ".\tchart\tchart.h"

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ANALYZE_CALORIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	void setTestFiles(CString* pstrFname);
	void setAnalyzeFile(CString strFname) {m_strAnalyzeFileName = strFname;};
	void setExportForPrint(bool bExport, int chartNum) {this->m_bExportForPrint = bExport; m_nExportImageNum = chartNum;};
	void setSubjectInfo(ED_PatientInfo pi) {this->m_patientInfo = pi;};
protected:
	//CString m_strTestFile[4];
	CString m_strAnalyzeFileName;
	//CAnalyzeNystagmus2 m_analyzeNys[4];
	CAnalyzeCaloric m_analyzeCaloric;


public:
	virtual BOOL OnInitDialog();



private:
	
	void layoutCtrls();
	void initTChart();
	
	
	void loadNystagmusData(int LR);

	void drawNystagmus(double spon);

	void analyzeThis();
	bool loadAnalyzeData();

	void initListCtrls();
	void showResult();
	void fillListResult();

	void setPeakRect(int test, double x, double y);	//peak rectangle�� ǥ�õ� ���� ��ġ ��Ų��.
	void setFixationBlock(int test, double t0, double t1);


	bool m_bExportForPrint;
	int m_nExportImageNum;
	CString m_strExportFolderName;
	void exportForPrint();
	
public:
	ED_PatientInfo m_patientInfo;
	CString m_strSubjectFolder;


	CTChart m_chartCaloric;	// LW RC LC RW
	CTChart m_chartButterfly;

	int m_idxToolFixationBlock;
	int m_idxToolPeakAmp;
	int m_idxToolCompensation;
	
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnBnClickedCheckUseFixBlock();
	afx_msg void OnBnClickedRadioCaloricEye();
	
	
	DECLARE_EVENTSINK_MAP()
	void OnDblClickTchartCaloricSlowMove();
	void OnRectangleToolResizedTchartCaloricSlowMove();
	void OnRectangleToolResizingTchartCaloricSlowMove();
	afx_msg void OnBnClickedCheckSpontaneousNystag();
	void OnRectangleToolDraggedTchartCaloricSlowMove();
	CListCtrl m_listTestResult;
	CListCtrl m_listTestSummary;
	afx_msg void OnBnClickedButtonSelectSpontaneousTest();
	afx_msg void OnBnClickedButtonResetPeak();
	afx_msg void OnBnClickedRadioManualPeak();
	int m_nManualPeakSelect;
	afx_msg void OnBnClickedButtonCaloricPrint();
};
#include "afxcmn.h"
