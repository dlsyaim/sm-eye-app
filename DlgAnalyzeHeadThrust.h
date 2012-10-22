#pragma once
#include "tchart\tchart.h"
#include "analyzeHeadthrust.h"
#include "Ed_testinfo.h"
#include "tchart_horizontal.h"
#include "afxcmn.h"
#include "dlgOptionHeadThrust.h"
#include "ed_patientinfo.h"

// CDlgAnalyzeHeadThrust ��ȭ �����Դϴ�.

class CDlgAnalyzeHeadThrust : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnalyzeHeadThrust)

public:
	CDlgAnalyzeHeadThrust(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgAnalyzeHeadThrust();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ANALYZE_HEAD_THRUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void setTestFile(CString strTestFile) {this->m_strTestFile = strTestFile;};

private:
	virtual BOOL OnInitDialog();


	CAnalyzeHeadThrust m_analyzeHeadThrust[2];
	CDlgOptionHeadThrust m_dlgHeadThrustOption;
	
	ED_TestInfo m_TI;

	CString m_strTestFile;
	double* m_pdLeft;
	double* m_pdRight;
	double* m_pdVelLeft;
	double* m_pdVelRight;
	
	double* m_pdVelHead;
	//double* m_pdHeadVelY;
	unsigned long m_ulEyeDataCount, m_ulAccDataCount;
	CTChart m_chart[4];		//thrust�� time-velocity curve
	CTChart m_chartVel[4];		//thrust�� head velocity-eye velocity curve
	//int m_idxToolLegendScroll;
	CTChart m_chartMain;

	int m_eventSeriesIdx;

	void allocateVariable();

	void layoutCtrls();
	bool loadData();
	void calEyeVelocity();
	void analyzeThis();
	bool loadAnalyzeData();
	void initListCtrls();
	void initTChart();
	void showResult();
	void showResult2();
	void fillListDetail();
	void fillListSummary();
	void selectListDetail();
	void selectHT(int idxHT);
	int m_nSelectedHT; // +�� up -�� down, 0�̸� non-select
	void deleteHT(int idxHT);
	bool saveAnalyzeData();

	void matchLeftRight();
	void drawHTTimeVelocityCurve(CAnalyzeHeadThrust* pAnalyze, double* pEyeVel, int chartIdx, int* pValid, int sign);
	void fillListDetail(CListCtrl* pListCtrl, 
						CList<structHeadThrust*, structHeadThrust*>* pList0, 
						CList<structHeadThrust*, structHeadThrust*>* pList1,
                        int* pValid, int validCount);

public:
	
	DECLARE_EVENTSINK_MAP()
	afx_msg void OnBnClickedButtonHtReanalyze();
	CListCtrl m_listHTUp, m_listHTDown, m_listHTSummary;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLvnItemchangedListHtDown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListHtUp(NMHDR *pNMHDR, LRESULT *pResult);
	void OnClickTchartHtVelocityChart();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonHeadThrustOption();

public:
	ED_PatientInfo m_patientInfo;
	afx_msg void OnBnClickedButtonHtPrint();
	void OnScrollTchartHeadThrustMain();
	void OnZoomTchartHeadThrustMain();
	void OnUndoZoomTchartHeadThrustMain();
	//void OnBeforeDrawSeriesTchartHeadThrustMain();
};
