#pragma once

#include ".\tchart\tchart.h"
#include "afxcmn.h"
#include "analyzeNystagmus2.h"
#include "ed_testinfo.h"
#include "Ed_patientinfo.h"


#define CHART_AN_NUM 3

struct structPrintNystag
{
	double LeftUpVel;
	double RightDownVel;
	double region;
	double targetVel;	//gain을 구하기 위하여 넣는다.
};



// CDlgAnalNystag 대화 상자입니다.

class CDlgAnalNystag : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnalNystag)

public:
	CDlgAnalNystag(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAnalNystag();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ANAL_NYSTAG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	CTChart m_ChartCtrl[CHART_AN_NUM];
public:
	virtual BOOL OnInitDialog();

protected:
	CString m_strTestFile;

	double* m_pdLeft, *m_pdRight, *m_pdTarget;
	double* m_pdLeftM, *m_pdRightM;
	void loadData();
	bool loadAnalyzeData();
	bool saveAnalyzeData();
	void allocVariable();

	unsigned long m_ulDataCount;
	

	//CAnalyzeNystagmus m_analyzeNys;
	CAnalyzeNystagmus2 m_analyzeNys[2];

	CListCtrl m_listResultSummary;
	CListCtrl m_listResultDetail;

	ED_TestInfo m_TI;

	int m_eventSeriesIdx;

	void layoutCtrl();
	void initTChart();
	void initListCtrls();
	void analyzeThis(int LR);
	void showResult();
	void showAnalyze();
	void fillListSummary();
	void fillListDetail();
	void fillChartSummary();
	//void add2List(CListCtrl* pList, CString strParam, double* pValue, int count);
	//void add2SummaryList(CString strParam, double* pValue, int count);
	//void add2DetailList(CString strParam, double* pValue, int count);
	void setLRVisible(bool leftVisible, bool rightVisible);
	int getRadioLR();
	void selectNystagmusOnChart(int count);
	int m_selectedNysIdx;
	
	void deleteSelectedNys(int LR, int nysIdx);
	bool m_bEditing;
	structNystag* m_pNysSelected;
	structNystag m_NysSaved;
	void enterEditMode(bool bEdit, unsigned long nysIdx);
	void moveSelectedNys(double vel, int length, int move_x);

public:
	void setTestFile(CString strFname){this->m_strTestFile = strFname;};
	
	
	afx_msg void OnBnClickedRadioEyeSelect();
	DECLARE_EVENTSINK_MAP()
	void OnAfterDrawTchartAn0();
	
	afx_msg void OnNMDblclkListAnResultDetail(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListAnResultDetail(NMHDR *pNMHDR, LRESULT *pResult);


	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnZoomTchartAn();
	void OnUndoZoomTchartAn();
	void OnScrollTchartAn();
	afx_msg void OnBnClickedButtonAnRefresh();
	afx_msg void OnBnClickedOk();
	
	void OnClickTchart();
	void OnDblClickTchart();
	unsigned long getClickedNystag();
	void selectListDetail(unsigned long idx);

	void analyzeForPrint(structPrintNystag* pNystag, bool forPrint = true);
	afx_msg void OnBnClickedButtonNystagPrint();

	ED_PatientInfo m_patientInfo;
	afx_msg void OnBnClickedButtonDeleteNegative();
	afx_msg void OnBnClickedButtonDeletePositive();
	afx_msg void OnBnClickedCheckRemoveOffset();
};

