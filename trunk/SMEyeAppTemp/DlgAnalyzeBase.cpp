// DlgAnalyzeBase.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgAnalyzeBase.h"
#include ".\dlganalyzebase.h"
#include ".\ed_testinfo.h"
#include "corwlDefines.h"

// CDlgAnalyzeBase ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgAnalyzeBase, CDialog)
CDlgAnalyzeBase::CDlgAnalyzeBase(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalyzeBase::IDD, pParent)
{
	m_pAnalyze = NULL;

	m_pEyeMoveLeft = NULL;
	m_pEyeMoveRight = NULL;
	m_ulDataCount = 0;
	m_testClass = 0;

	m_pLH = m_pLV = m_pRH = m_pRV = m_pAx = m_pAy = m_pAz = m_pGx = m_pGy = NULL;
}

CDlgAnalyzeBase::~CDlgAnalyzeBase()
{
}

void CDlgAnalyzeBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_ANAL0, m_TChartAnal0);
	DDX_Control(pDX, IDC_TCHART_ANAL1, m_TChartAnal1);
	DDX_Control(pDX, IDC_TCHART_ANAL2, m_TChartAnal2);
	DDX_Control(pDX, IDC_TCHART_ANAL3, m_TChartAnal3);
	DDX_Control(pDX, IDC_TCHART_ANAL4, m_TChartAnal4);
	DDX_Control(pDX, IDC_TCHART_ANAL5, m_TChartAnal5);
	DDX_Control(pDX, IDC_TCHART_ANAL6, m_TChartAnal6);
	DDX_Control(pDX, IDC_TCHART_ANAL7, m_TChartAnal7);
	DDX_Control(pDX, IDC_LIST_RESULT_SUMMARY, m_listCtrlResultSummary);
}


BEGIN_MESSAGE_MAP(CDlgAnalyzeBase, CDialog)
END_MESSAGE_MAP()


// CDlgAnalyzeBase �޽��� ó�����Դϴ�.

BOOL CDlgAnalyzeBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	ShowWindow(SW_SHOWMAXIMIZED);
	this->layoutCtrls();

	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.top = 240+3;
	CTChart* ppTChart[] = {&m_TChartAnal0, &m_TChartAnal1, &m_TChartAnal2, &m_TChartAnal3
							, &m_TChartAnal4, &m_TChartAnal5, &m_TChartAnal6, &m_TChartAnal7};
	this->m_pAnalyze->layoutTChart(ppTChart, CHART_ANALYZE_NUM, clientRect);
    this->m_pAnalyze->initTChart(ppTChart);

	if(m_pAnalyze && m_ulDataCount)
	{
		
		
		

		//head trust�� ���		
		this->m_pAnalyze->analyze(m_pLH, m_pLV, m_pRH, m_pRV, m_pAx, m_pAy, m_pAz, m_pGx, m_pGy, m_ulDataCount, ppTChart);
		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgAnalyzeBase::layoutCtrls(void)
{
	CRect clientRect, rect;
	this->GetClientRect(&clientRect);
	
	int sideMargin = 3;
	this->m_listCtrlResultSummary.MoveWindow(sideMargin, sideMargin, clientRect.Width()/4, 240);

	GetDlgItem(IDOK)->GetWindowRect(&rect);
	GetDlgItem(IDOK)->MoveWindow(clientRect.right - rect.Width() - 3, 3, rect.Width(), rect.Height()); 


}
