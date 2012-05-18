#pragma once

#include ".\tchart\tchart.h"
#include "analyzesaccade.h"
#include "afxcmn.h"
#include "eu_testfilev2.h"
#include "ed_testinfo.h"
#include "corwlUtil.h"
//#include "includeTChart.h"

struct structPrintSaccade
{
	double peakVel[2];
	double latency[2];
	double accuracy[2];
};


// CDlgAnalSaccade 대화 상자입니다.

class CDlgAnalSaccade : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnalSaccade)

public:
	CDlgAnalSaccade(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAnalSaccade();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ANALYZE_SACCADE }; 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	CAnalyzeSaccade m_analyzeSaccade[2];

	
	ED_TestInfo m_TI;

	CString m_strTestFile;
	double* m_pdLeft;
	double* m_pdRight;
	
	double* m_pdTarget;
	unsigned long m_ulDataCount;

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
	int getRadioLR();
	void setLRVisible(bool leftVisible, bool rightVisible);
	void exportToCSV();
	bool exportToCSV(CString fname, CList<structSaccade*, structSaccade*> *pList);

	int m_selectedSacIdx;
	void selectSaccadeOnChart(int count);
	void deleteSelectedSaccade(int LR, int sacIdx);

	bool m_bEditing;
	structSaccade* m_pSaccadeSelected;
	structSaccade m_saccadeSaved;
	void enterEditMode(bool bEdit, unsigned long saccadeIdx);
	void moveSelectedSaccade(int latencyOffset, int endIdxOffset);


	CTChart m_chartZoom;
	CTChart m_chartPeakVel;
	CTChart m_chartAccuracy;
	CTChart m_chartLatency;
	CTChart m_chartRaw;

	CListCtrl m_listResultSummary;
	CListCtrl m_listResultDetail;

public:
	void setTestFile(CString strTestFile) {this->m_strTestFile = strTestFile;};

	void analyzeForPrint(structPrintSaccade* pSaccade, bool forPrint = true);

	
	
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedRadioEyeSelect();
	afx_msg void OnLvnItemchangedListAsResultDetail(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_EVENTSINK_MAP()
	
	afx_msg void OnNMDblclkListAsResultDetail(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAsRefresh();
	
	void OnClickTchart();
	void OnDblClickTchart();
	unsigned long getClickedSaccade();
	void selectListDetail(unsigned long idx);
	
	void OnClickTchartSmall();
	void OnDblClickTchartSmall();
	unsigned long getClickedSaccadeSmall();

	
	
};
