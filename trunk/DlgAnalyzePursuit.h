#pragma once
#include "analyzePursuit.h"
#include ".\tchart\tchart.h"
#include "resource.h"
#include "ed_testinfo.h"
#include "ed_patientinfo.h"

struct structPrintPursuit
{
	double gain[2];
};

// CDlgAnalyzePursuit 대화 상자입니다.

class CDlgAnalyzePursuit : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnalyzePursuit)

public:
	CDlgAnalyzePursuit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAnalyzePursuit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ANALYZE_PURSUIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	
protected:
	CAnalyzePursuit m_analyzePursuit[2];
	CString m_strTestFile;
	double* m_pdLeft;
	double* m_pdRight;
	
	double* m_pdTarget;
	unsigned long m_ulDataCount;

	int m_eventSeriesIdx;

	void loadData();
	bool loadAnalyzeData();
	bool saveAnalyzeData();
	void allocVariable();
	void initListCtrls();
	void initTChart();
	void analyzeThis(int LR);
	void layoutCtrls();
	void showResult();
	void showAnalyze();
	void fillListSummary();
	void fillListDetail();
	void selectListDetail(unsigned long idx);
	int getRadioLR();
	void setLRVisible(bool leftVisible, bool rightVisible);

	int m_selectedPursuitIdx;
	
	void selectPursuitOnChart(int count);
	void deleteSelectedPursuit(int LR, int sacIdx);

	bool m_bEditing;
	structPursuit* m_pPursuitSelected;
	structPursuit m_pursuitSaved;
	void enterEditMode(bool bEdit, unsigned long pursuitIdx);
	void moveSelectedPursuit(double x, double y);

	
	CTChart m_chartPosition;
	CTChart m_chartVelocity;
	CTChart m_chartGain;
	
	CListCtrl m_listResultSummary;
	CListCtrl m_listResultDetail;

	ED_TestInfo m_TI;
public:
	void setTestFile(CString strTestFile) {this->m_strTestFile = strTestFile;};

	void analyzeForPrint(structPrintPursuit* pStructPursuit, bool forPrint = true);

	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedRadioEyeSelect();
	afx_msg void OnLvnItemchangedListAsResultDetail(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	void OnZoomTchartAp();
	void OnUndoZoomTchartAp();
	void OnScrollTchartAp();
	void OnAfterDrawTchartApPosition();
	afx_msg void OnNMDblclkListApResultDetail(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonApRefresh();
	afx_msg void OnBnClickedOk();
	
	
	void OnClickTchart();
	void OnDblClickTchart();
	unsigned long getClickedPursuit();
	
public:
	ED_PatientInfo m_patientInfo;
	afx_msg void OnBnClickedButtonPursuitPrint();
	afx_msg void OnBnClickedCheckRemoveOffset();
	void OnBeforeDrawSeriesTchartApPosition();
};
