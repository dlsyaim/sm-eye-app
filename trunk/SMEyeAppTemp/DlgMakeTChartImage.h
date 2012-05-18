#pragma once
#include ".\tchart\tchart.h"
#include "ED_testinfo.h"
#include "resource.h"

struct printImgParam
{
	CString text;
	CString fname;
};

// CDlgMakeTChartImage 대화 상자입니다.

class CDlgMakeTChartImage : public CDialog
{
	DECLARE_DYNAMIC(CDlgMakeTChartImage)

public:
	CDlgMakeTChartImage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMakeTChartImage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MAKE_TCHART_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTChart m_chart;
	ED_TestInfo* m_pTI;
	CList<printImgParam, printImgParam> m_listImgParam;
	int m_chartNum;
	CString m_strPrintTempFolder;
	CString m_strSubjectFolder;
	virtual BOOL OnInitDialog();

	void setBottomAxisDuration(int duration) { this->m_bottomAxisDuration = duration;};

protected:
	void layoutCtrls();
	void loadData(CString strFilename);
	void makeImages();
	void initTChart();
	bool drawData(int HV);
	printImgParam export();

	void allocVariable();
	unsigned long m_ulDataCount;
	double *m_pdLH, *m_pdRH, *m_pdLV, *m_pdRV, *m_pdStiH, *m_pdStiV;

	double m_bottomAxisDuration;
};
