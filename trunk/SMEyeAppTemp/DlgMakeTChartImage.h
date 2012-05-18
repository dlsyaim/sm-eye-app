#pragma once
#include ".\tchart\tchart.h"
#include "ED_testinfo.h"
#include "resource.h"

struct printImgParam
{
	CString text;
	CString fname;
};

// CDlgMakeTChartImage ��ȭ �����Դϴ�.

class CDlgMakeTChartImage : public CDialog
{
	DECLARE_DYNAMIC(CDlgMakeTChartImage)

public:
	CDlgMakeTChartImage(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMakeTChartImage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MAKE_TCHART_IMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
