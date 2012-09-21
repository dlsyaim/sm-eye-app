#pragma once
#include "analyzeBase.h"
#include "analyzeNystagmus.h"
#include ".\tchart\tchart.h"
#include "tchart_horizontal.h"
#include "afxcmn.h"


// CDlgAnalyzeBase ��ȭ �����Դϴ�.

class CDlgAnalyzeBase : public CDialog
{
	DECLARE_DYNAMIC(CDlgAnalyzeBase)

public:
	CDlgAnalyzeBase(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgAnalyzeBase();

	CAnalyzeBase* m_pAnalyze;
	void setData(double* pLeft, double* pRight, unsigned long count, int testClass)
	{
		m_pEyeMoveLeft = pLeft;
		m_pEyeMoveRight = pRight;
		m_ulDataCount = count;

		
	}

	void setData(double* pLH, double* pLV, double* pRH, double* pRV, double* pAx, double* pAy, double* pAz, double* pGx, double* pGy, unsigned long count)
	{
		m_pLH = pLH;
		m_pLV = pLV;
		m_pRH = pRH;
		m_pRV = pRV;
		m_pAx = pAx;
		m_pAy = pAy;
		m_pAz = pAz;
		m_pGx = pGx;
		m_pGy = pGy;
	
		m_ulDataCount = count;
		

	}
	

protected:
	int m_testClass;
	double* m_pEyeMoveLeft;
	double* m_pEyeMoveRight;
	unsigned long m_ulDataCount;


	//head trust�� ������
	double* m_pLH, *m_pLV, * m_pRH, *m_pRV, *m_pAx, *m_pAy, *m_pAz, *m_pGx, *m_pGy;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ANALYZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
protected:
	void layoutCtrls(void);
public:
	CTChart m_TChartAnal0;
	CTChart m_TChartAnal1;
	CTChart m_TChartAnal2;
	CTChart m_TChartAnal3;
	CTChart m_TChartAnal4;
	CTChart m_TChartAnal5;
	CTChart m_TChartAnal6;
	CTChart m_TChartAnal7;
	CListCtrl m_listCtrlResultSummary;
};
