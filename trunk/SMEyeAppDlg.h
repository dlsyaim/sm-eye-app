// SMEyeAppDlg.h : 헤더 파일
//

#pragma once
#include "corwlDefines.h"
#include "stereocam.h"
#include "imageDisplay.h"
#include "smeye.h"
#include "ED_PatientInfo.h"
#include "ed_testdatabinocular.h"
#include "ed_appoption.h"
#include "Eu_testfile.h"
#include "treeCtrlxml.h"
#include "commdrive.h"
#include "commled.h"
#include "aviloadermanager.h"
#include "commEOG.h"


#include ".\tchart\tchart.h"


// CSMEyeAppDlg 대화 상자
class CSMEyeAppDlg : public CDialog
{
// 생성
public:
	CSMEyeAppDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_SMEYEAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////
// member variable
private:
	ED_AppOption m_appOption;

	CTChart m_ChartCtrl[CHART_NUM];
	bool m_bChartVisible[CHART_NUM];
	bool m_bUnderTest;
	CStereoCam m_stereoCam;
	CTime m_timeStart;

	CBrush m_seriesBrush[7];

	CString m_strSubjectFolder;
	ED_PatientInfo m_patientInfo;
	ED_TestDataBinocular m_testDataBinocular;

	CTreeCtrlXML m_treeTest;
	bool m_bDeleteFromContextMenu;
	bool isCaloricGroup(HTREEITEM itemParent, CString* pstrFname = NULL);
	bool isCaloricGroup2(HTREEITEM itemParent, CString* pstrFname = NULL);
	//CString m_strAppPath;

	ED_TestInfo* m_pCurrentTI;
	HTREEITEM m_hItemforContextMenu;
	HTREEITEM m_hItemSelected;
	

	//acc
	CCommDrive m_CommACC;
	CCommLED m_CommLED;
	CCommEOG m_CommEOG;

	bool m_bShowStimulation;
	CAviLoaderManager m_aviLoaderManager;


	//event string
	CString m_strEvents[EVENT_STRING_COUNT];	

	

public:
	CImageDisplay m_ImageDisplay;
	unsigned char m_displaySource;

protected:
	void playAvi(bool Avi);

	
protected:
	void layoutCtrls();
	CRect m_rectChartRegion;
	void initTChart(unsigned int showChart);
	void getDataFromStereoCam();
	void updateParams(bool bSave);
	void addData2Chart(double t, ED_EyeInfo* pEyeInfos, double* pAcc, double* pSti);
	bool enrollSubjectInfo();
	bool loadCurrentTestData();
	void activateTestCtrls(bool bActive);
	
	void tchartNextPage(int advance);
	void tchartLastPage(int advance);
	void setChartVisibility(unsigned int vis);


	void updateCamInfo();
	void updateEyeInfo();
	void updateTestInfo();

////////////////////////////////////////////////////////////////////////////////////////////
// member function

public:
	afx_msg void OnBnClickedCheckCamStart();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNMClickTreeTest(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonOpen();

	afx_msg LRESULT OnDrawChart(WPARAM wParam, LPARAM lParam);
	DECLARE_EVENTSINK_MAP()
	void OnColorLineToolDragLineTchartHorizontal();
	afx_msg void OnBnClickedButtonAnalyze();
	afx_msg void OnBnClickedCheckChartSeries();
	afx_msg void OnBnClickedCheckChart();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTvnSelchangedTreeTest(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMRclickTreeTest(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	HTREEITEM getTreeItemOnMouse(UINT* pFlag);
	bool loadDataFromTreeItem(HTREEITEM hItem);
	void deleteDataFromTreeItem(HTREEITEM hItem);

	void selectNextTest(HTREEITEM);

	void setEventStrings(ED_TestInfo* pTI);

	void addEvent(CString str);

public:
	afx_msg void OnContextChildEditTestInfo();
	afx_msg void OnContextBlankSetasdefaultTestInfo();
	afx_msg void OnContextBlankAddnewgroup();
	afx_msg void OnContextGroupDelete();
	afx_msg void OnContextGroupRename();
	afx_msg void OnContextBlankClear();
	afx_msg void OnContextBlankLoad();
	afx_msg void OnContextBlankSave();
	afx_msg void OnContextGroupAddTest();
	afx_msg void OnContextChildDeleteTestInfo();
	afx_msg void OnContextChildClearData();
	afx_msg void OnTvnDeleteitemTreeTest(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonEditSubjectInfo();
	afx_msg void OnBnClickedButtonNewSubjectInfo();
	afx_msg void OnBnClickedButtonOption();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextImageResetTorsion();
	

	void OnZoomTchart();	
	void OnUndoZoomTchart();
	void OnScrollTchart();
	
	char getChartIdxOnMouse();
	afx_msg void OnContextChartposRemoveoffset();
	double removeOffset(CValueList* pValueList);
	
	char m_cClickedChart;
	char m_cClickedSeries;
	void OnClickAxisTchart(long Axis, long Button, long Shift, long X, long Y);
	void OnClickSeriesTchart(long SeriesIndex, long ValueIndex, long Button, long Shift, long X, long Y);
	
	
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedCheckRemoveOffset();
	afx_msg void OnBnClickedButtonEvent();
	

	void OnBeforePrintTchartHorizontal();
};
