// DlgAnalNystag.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgAnalNystag.h"
#include ".\dlganalnystag.h"
#include "eu_testfile.h"
#include "eu_testfilev2.h"

#include "math.h"

#include "corwlDefines.h"
#include "corwlUtil.h"


//#include ".\tchart\series.h"
#include ".\tchart\teechartdefines.h"
#include ".\tchart\axes.h"
#include ".\tchart\axis.h"
#include ".\tchart\fastlineseries.h"
#include ".\tchart\pointseries.h"
#include ".\tchart\pointer.h"
#include ".\tchart\aspect.h"
#include ".\tchart\axisgridpen.h"
#include ".\tchart\axislabels.h"
#include ".\tchart\valuelist.h"
#include ".\tchart\legend.h"
#include ".\tchart\axistitle.h"
#include ".\tchart\teefont.h"
#include ".\tchart\toollist.h"
#include ".\tchart\colorlinetool.h"
#include ".\tchart\tools.h"
#include ".\tchart\zoom.h"
#include ".\tchart\strings.h"
#include ".\tchart\titles.h"
#include ".\tchart\marks.h"
#include ".\tchart\colorbandtool.h"
#include ".\tchart\pen.h"
#include ".\tchart\scroll.h"
#include ".\tchart\zoom.h"
#include ".\tchart\TeePoint2D.h"


#define CHART_EYE_MOV	0
#define CHART_NYS_VEL	1
#define CHART_NYS_SUMMARY	2

// CDlgAnalNystag 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAnalNystag, CDialog)
CDlgAnalNystag::CDlgAnalNystag(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalNystag::IDD, pParent)
{
	m_pdLeft = m_pdRight = m_pdTarget = NULL;
	m_ulDataCount = 0;
	
	m_bEditing = false;
	m_selectedNysIdx = -1;
	m_pNysSelected = NULL;

	
}

CDlgAnalNystag::~CDlgAnalNystag()
{
	DEL(m_pdLeft);
	DEL(m_pdRight);
	DEL(m_pdTarget);
}

void CDlgAnalNystag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TCHART_AN_0, m_ChartCtrl[0]);
	DDX_Control(pDX, IDC_TCHART_AN_1, m_ChartCtrl[1]);
	DDX_Control(pDX, IDC_TCHART_AN_2, m_ChartCtrl[2]);
	
	DDX_Control(pDX, IDC_LIST_AN_RESULT_SUMMARY, m_listResultSummary);
	DDX_Control(pDX, IDC_LIST_AN_RESULT_DETAIL, m_listResultDetail);
}


BEGIN_MESSAGE_MAP(CDlgAnalNystag, CDialog)
	ON_BN_CLICKED(IDC_RADIO_AN_ALL, OnBnClickedRadioEyeSelect)
	ON_BN_CLICKED(IDC_RADIO_AN_LEFT, OnBnClickedRadioEyeSelect)
	ON_BN_CLICKED(IDC_RADIO_AN_RIGHT, OnBnClickedRadioEyeSelect)


	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AN_RESULT_DETAIL, OnNMDblclkListAnResultDetail)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_AN_RESULT_DETAIL, OnLvnItemchangedListAnResultDetail)
	ON_BN_CLICKED(IDC_BUTTON_AN_REFRESH, OnBnClickedButtonAnRefresh)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAnalNystag 메시지 처리기입니다.

BOOL CDlgAnalNystag::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ShowWindow(SW_SHOWMAXIMIZED);
	layoutCtrl();
	
	


	this->loadData();

	this->analyzeThis(BOTH);
	loadAnalyzeData();


	initListCtrls();
	this->showResult();
	
	



	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CDlgAnalNystag::loadData()
{
	EU_TestFileV2 testFile;
		
	if(testFile.open(this->m_strTestFile, &m_TI, false))
	{
		testFile.readTI(&m_TI);
		m_ulDataCount = m_TI.m_lEyeDataCount;
		if(m_ulDataCount)
		{
			this->allocVariable();


			if(m_TI.m_analTarget == HORIZONTAL)
				testFile.readTestData(true, this->m_pdLeft, m_pdRight, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, this->m_pdTarget, NULL);
			else
				testFile.readTestData(true, NULL, NULL, m_pdLeft, m_pdRight, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, m_pdTarget);
		}

	}
}


bool CDlgAnalNystag::loadAnalyzeData()
{
	bool bRtn = true;

	CString str;
	CFileStatus fs;
	//Left
	str = this->m_strTestFile;
	str.Replace(".emd", "L.ema");
	if(CFile::GetStatus(str, fs))
		this->m_analyzeNys[0].load(str);
	else
		bRtn = false;

	//Right
	str = this->m_strTestFile;
	str.Replace(".emd", "R.ema");
	if(CFile::GetStatus(str, fs))
		this->m_analyzeNys[1].load(str);
	else
		bRtn = false;

	return bRtn;
}


bool CDlgAnalNystag::saveAnalyzeData()
{
	bool bRtn = true;

	CString str;
	CFileStatus fs;
	//Left
	str = this->m_strTestFile;
	str.Replace(".emd", "L.ema");
	if(!m_analyzeNys[0].save(str))
		bRtn = false;

	//Right
	str = this->m_strTestFile;
	str.Replace(".emd", "R.ema");
	if(m_analyzeNys[1].save(str))
		bRtn = false;

	return bRtn;

}


void CDlgAnalNystag::allocVariable()
{
	DEL(m_pdLeft);
	DEL(m_pdRight);
	DEL(m_pdTarget);

	if(m_ulDataCount)
	{
		m_pdLeft = new double[m_ulDataCount];
		m_pdRight = new double[m_ulDataCount];
		m_pdTarget = new double[m_ulDataCount];
	}
}

void CDlgAnalNystag::layoutCtrl()
{
	//all을 선택
	((CButton*)GetDlgItem(IDC_RADIO_AN_ALL))->SetCheck(true);

	double gap = 0.01;
	double button_h = gap*2;
	double button_w = button_h*4;
	CRect rect;
	this->GetClientRect(rect);

	//radio
	corLocateWindow(GetDlgItem(IDC_RADIO_AN_ALL), gap, gap, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDC_RADIO_AN_LEFT), gap*2+button_w, gap, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDC_RADIO_AN_RIGHT), gap*3+button_w*2, gap, button_w, button_h, rect);

	


	//summary chart
	corLocateWindow(GetDlgItem(IDC_TCHART_AN_2), gap, 2*gap+button_h, 0.4, 0.3, rect);
	//summary list
	corLocateWindow(GetDlgItem(IDC_LIST_AN_RESULT_SUMMARY), 2*gap+0.4, 2*gap+button_h, 0.25, 0.3, rect);
	corLocateWindow(GetDlgItem(IDC_LIST_AN_RESULT_DETAIL), 3*gap+0.65, 2*gap+button_h, 0.25, 0.3, rect);
	
	//chart
	corLocateWindow(GetDlgItem(IDC_TCHART_AN_0), gap, 3*gap+button_h+0.3, 1-2*gap, 0.3, rect);
	corLocateWindow(GetDlgItem(IDC_TCHART_AN_1), gap, 4*gap+button_h+0.6, 1-2*gap, 0.3, rect);

	//buttons
	corLocateWindow(GetDlgItem(IDC_BUTTON_AN_REFRESH), gap, 1-gap-button_h, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDCANCEL),1-button_w*2-2*gap, 1-gap-button_h, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDOK),1-button_w-gap, 1-gap-button_h, button_w, button_h, rect);
	

}

void CDlgAnalNystag::initListCtrls()
{
	//set list control
	//insert column
	CRect rect;
	this->m_listResultSummary.GetClientRect(&rect);
	int w = rect.Width();

	//setup list summary
	CString list[3] = {"", "Left Beat(°/s)", "Right Beat(°/s)"};
	if(m_TI.m_analTarget == VERTICAL)
	{
		list[1] = "Up Beat(°/s)";
		list[2] = "Down Beat(°/s)";
	}
	//OKN일 경우 gain을 표시
	if(m_TI.m_stiModeCommand == COMMAND_STI_OKN)
	{
		list[1] = "Left Gain";
		list[2] = "Right Gain";
		if(m_TI.m_analTarget == VERTICAL)
		{
			list[1] = "Up Gain";
			list[2] = "Down Gain";
		}
	}
	double widths[3] = {.34, .33, .33};
	for(int i=0; i<3; i++)
		m_listResultSummary.InsertColumn(i, list[i], LVCFMT_CENTER, int(w*widths[i]));
		

	//setup list detail
	DWORD dwStyle = m_listResultDetail.GetExStyle();
	dwStyle = (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listResultDetail.SetExtendedStyle(dwStyle);

	char* listD[5] = {"No", "start (s)", "end (s)", "duration(ms)", "vel (°/s"};
	double widthD[5] = {.1, .2, .2, .25, .2};
	for(int i=0; i<5; i++)
		m_listResultDetail.InsertColumn(i, listD[i], LVCFMT_CENTER, int(widthD[i]*w));
}

void CDlgAnalNystag::initTChart()
{
	//좌,우에서 사용할 것을 모두 한번에 만든다.
	CString strTitle[] = {"Eye Position", "Nystagmus Velocity", ""};
	for(int i=0; i<CHART_AN_NUM; i++)
	{
		m_ChartCtrl[i].ClearChart();
		if(i!=CHART_NYS_SUMMARY)
		{
			m_ChartCtrl[i].GetAspect().SetView3D(false);
			m_ChartCtrl[i].GetZoom().SetEnable(true);
			m_ChartCtrl[i].GetAspect().SetZoom(tzdBoth);
			m_ChartCtrl[i].GetLegend().SetVisible(false);
			m_ChartCtrl[i].GetHeader().GetText().SetText(strTitle[i]);
		}

	}
	m_ChartCtrl[CHART_NYS_VEL].GetAxis().GetBottom().SetAutomatic(false);
	
	//좌우 두개의 series를 더해준다.
	//movement
	m_ChartCtrl[CHART_EYE_MOV].AddSeries(scLine);		//0
	m_ChartCtrl[CHART_EYE_MOV].AddSeries(scLine);		//1

	//series for target
	m_ChartCtrl[CHART_EYE_MOV].AddSeries(scLine);		//2

	//nystagmus 표시용
	m_ChartCtrl[CHART_EYE_MOV].AddSeries(scLine);		//3
	m_ChartCtrl[CHART_EYE_MOV].AddSeries(scLine);		//4

	
	m_ChartCtrl[CHART_EYE_MOV].GetAxis().GetLeft().SetAutomatic(false);
	m_ChartCtrl[CHART_EYE_MOV].GetAxis().GetLeft().SetMinMax(-30, 30);


	

	

	m_ChartCtrl[CHART_EYE_MOV].Series(0).SetColor(COLOR_LEFT);
	m_ChartCtrl[CHART_EYE_MOV].Series(1).SetColor(COLOR_RIGHT);
	m_ChartCtrl[CHART_EYE_MOV].Series(2).SetColor(COLOR_TARGET);
	m_ChartCtrl[CHART_EYE_MOV].Series(3).SetColor(RGB(0,0,0));
	m_ChartCtrl[CHART_EYE_MOV].Series(4).SetColor(RGB(0,0,0));

	m_ChartCtrl[CHART_EYE_MOV].Series(0).SetTitle("Left");
	m_ChartCtrl[CHART_EYE_MOV].Series(1).SetTitle("Right");
	m_ChartCtrl[CHART_EYE_MOV].Series(3).SetShowInLegend(false);
	m_ChartCtrl[CHART_EYE_MOV].Series(4).SetShowInLegend(false);

	//colorband를 추가한다.
	m_ChartCtrl[CHART_EYE_MOV].GetTools().Add(tcColorband);
	m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).GetAsColorband().SetColor(COLOR_SELECTED);
	m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).GetAsColorband().SetTransparency(50);
	m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).GetAsColorband().GetPen().SetVisible(false);
	VARIANT varAxis; varAxis.vt=VT_DISPATCH;
	CAxis ax = m_ChartCtrl[CHART_EYE_MOV].GetAxis().GetBottom();
	varAxis.pdispVal = ax;
	m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).GetAsColorband().SetAxis(varAxis);
	m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).GetAsColorband().SetDrawBehind(true);
	m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).SetActive(false);


	

	
	//nystagmus velocity
	m_ChartCtrl[CHART_NYS_VEL].AddSeries(scPoint);		// left 0
	m_ChartCtrl[CHART_NYS_VEL].AddSeries(scPoint);		// right 1
	m_ChartCtrl[CHART_NYS_VEL].AddSeries(scLine);		// target 2
	m_ChartCtrl[CHART_NYS_VEL].Series(0).GetAsPoint().GetPointer().SetStyle(psCircle);
	m_ChartCtrl[CHART_NYS_VEL].Series(0).GetAsPoint().GetPointer().SetHorizontalSize(2);
	m_ChartCtrl[CHART_NYS_VEL].Series(0).GetAsPoint().GetPointer().SetVerticalSize(2);

	m_ChartCtrl[CHART_NYS_VEL].Series(1).GetAsPoint().GetPointer().SetStyle(psCircle);
	m_ChartCtrl[CHART_NYS_VEL].Series(1).GetAsPoint().GetPointer().SetHorizontalSize(2);
	m_ChartCtrl[CHART_NYS_VEL].Series(1).GetAsPoint().GetPointer().SetVerticalSize(2);

	m_ChartCtrl[CHART_NYS_VEL].Series(2).SetColor(COLOR_TARGET);


	
	m_ChartCtrl[CHART_NYS_VEL].Series(0).SetTitle("Left");
	m_ChartCtrl[CHART_NYS_VEL].Series(1).SetTitle("Right");

	//SUMMARY
	m_ChartCtrl[CHART_NYS_SUMMARY].AddSeries(scBar);
	m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).GetMarks().SetVisible(true);

	m_ChartCtrl[CHART_NYS_SUMMARY].GetAxis().GetLeft().SetAutomatic(false);
	m_ChartCtrl[CHART_NYS_SUMMARY].GetAxis().GetLeft().SetMinMax(0, 1.5);
	m_ChartCtrl[CHART_NYS_SUMMARY].GetAxis().GetBottom().SetAutomatic(true);
	/*if(this->m_TI.m_stiModeCommand == 0x06)
		m_ChartCtrl[CHART_NYS_SUMMARY].GetAxis().GetBottom().SetMinMax(-1, 8);
	else
		m_ChartCtrl[CHART_NYS_SUMMARY].GetAxis().GetBottom().SetMinMax(-1, 4);*/

	m_ChartCtrl[CHART_NYS_SUMMARY].GetLegend().SetVisible(false);
	

	m_ChartCtrl[CHART_NYS_SUMMARY].GetTools().Add(tcColorband);
	m_ChartCtrl[CHART_NYS_SUMMARY].GetTools().GetItems(0).GetAsColorband().SetColor(0xffffff);
	m_ChartCtrl[CHART_NYS_SUMMARY].GetTools().GetItems(0).GetAsColorband().SetTransparency(50);
	m_ChartCtrl[CHART_NYS_SUMMARY].GetTools().GetItems(0).GetAsColorband().GetPen().SetVisible(false);
	ax = m_ChartCtrl[CHART_NYS_VEL].GetAxis().GetBottom();
	varAxis.pdispVal = ax;
	m_ChartCtrl[CHART_NYS_SUMMARY].GetTools().GetItems(0).GetAsColorband().SetAxis(varAxis);
	m_ChartCtrl[CHART_NYS_SUMMARY].GetTools().GetItems(0).GetAsColorband().SetDrawBehind(true);
	m_ChartCtrl[CHART_NYS_SUMMARY].GetTools().GetItems(0).SetActive(false);

	if(this->m_TI.m_stiModeCommand == 0xfe)	//led off
	{
		m_ChartCtrl[CHART_EYE_MOV].Series(2).SetActive(false);
		m_ChartCtrl[CHART_NYS_VEL].Series(2).SetActive(false);
	}
}

void CDlgAnalNystag::analyzeThis(int LR)
{
	if(!m_pdLeft || !m_pdRight || !m_ulDataCount)
	{
		::AfxMessageBox("error in CDlgAnalNystag::analyzeThis pointer assertion!!");
		return;
	}
	//CFile file[2];
	if(LR == BOTH)
	{
		/*file[0].Open(_T("c:\\temp\\left.dat"), CFile::modeWrite|CFile::modeCreate);
		file[1].Open(_T("C:\\temp\\right.dat"), CFile::modeWrite|CFile::modeCreate);
		if(file[0].m_hFile  != CFile::hFileNull)
		{
			file[0].Write(m_pdLeft, sizeof(double)*m_ulDataCount);
			file[1].Write(m_pdRight, sizeof(double)*m_ulDataCount);
		}*/
		
		this->m_analyzeNys[0].analyze(this->m_pdLeft, m_ulDataCount);
		this->m_analyzeNys[1].analyze(this->m_pdRight, m_ulDataCount);
		//file[0].Close();
		//file[1].Close();
	}
	else
		this->m_analyzeNys[LR].analyze((LR == LEFT) ? m_pdLeft : m_pdRight, m_ulDataCount);

}

void CDlgAnalNystag::showResult()
{
	initTChart();


	
	for(int i=0; i<CHART_AN_NUM; i++)
		m_ChartCtrl[i].SetAutoRepaint(false);

	
	//eye movement를 그려준다.
	for(unsigned long i=0; i<this->m_ulDataCount; i++)
	{
		this->m_ChartCtrl[CHART_EYE_MOV].Series(LEFT).AddXY(double(i)/FRAMERATE, m_pdLeft[i], "", COLOR_LEFT);
		this->m_ChartCtrl[CHART_EYE_MOV].Series(RIGHT).AddXY(double(i)/FRAMERATE, m_pdRight[i], "", COLOR_RIGHT);

		if(this->m_TI.m_stiModeCommand != 0x06)	// mode OKN이 아니면 target은 eye location에
			this->m_ChartCtrl[CHART_EYE_MOV].Series(2).AddXY(double(i)/FRAMERATE, m_pdTarget[i], "", COLOR_TARGET);
		else	//okn일 경우 velocity에
			this->m_ChartCtrl[CHART_NYS_VEL].Series(2).AddXY(double(i)/FRAMERATE, m_pdTarget[i], "", COLOR_TARGET);
		

	}


	showAnalyze();
	
	

	for(int i=0; i<CHART_AN_NUM; i++)
	{
		m_ChartCtrl[i].SetAutoRepaint(true);
		m_ChartCtrl[i].Repaint();
	}

	fillChartSummary();	//CHART SUMMARY에 분석 결과 정리
	fillListSummary();
	fillListDetail();	//list에 nystagmus 각각의 값을 적는다.
}

void CDlgAnalNystag::showAnalyze()
{
	m_ChartCtrl[CHART_EYE_MOV].Series(LEFT+3).Clear();
	m_ChartCtrl[CHART_EYE_MOV].Series(RIGHT+3).Clear();
	m_ChartCtrl[CHART_NYS_VEL].Series(LEFT).Clear();
	m_ChartCtrl[CHART_NYS_VEL].Series(RIGHT).Clear();



	//nystagmus velocity를 표시
	for(int eye=LEFT; eye<RIGHT+1; eye++)
	{
		POSITION pos1, pos2;
		double x;
		COLORREF color = (eye==LEFT) ? COLOR_LEFT : COLOR_RIGHT;
		double* pEyeMove = (eye==LEFT) ? m_pdLeft : m_pdRight;
		for(pos1 = m_analyzeNys[eye].m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			//velocity chart에 표시
			structNystag* pNys = m_analyzeNys[eye].m_listNystagmus.GetNext(pos1);
			x = double(pNys->startI + pNys->endI)/2/FRAMERATE;
			m_ChartCtrl[CHART_NYS_VEL].Series(eye).AddXY(x, pNys->vel, "", color);			

			//eye movement data에 표시
			double x,y;
			double offset = 5;
			x = double(pNys->startI)/FRAMERATE;
			y = pEyeMove[pNys->startI];
			m_ChartCtrl[CHART_EYE_MOV].Series(3+eye).AddXY(x,y+offset, "", 0);

			x = double(pNys->endI)/FRAMERATE;
			//y = pEyeMove[nys.endI];
			y += double(pNys->endI-pNys->startI)/FRAMERATE * pNys->vel;
			m_ChartCtrl[CHART_EYE_MOV].Series(3+eye).AddXY(x, y+offset, "", 0);
			m_ChartCtrl[CHART_EYE_MOV].Series(3+eye).AddNullXY(x,y, "");
		}		
	}
	/*for(int i=0; i<CHART_AN_NUM; i++)
	{	
		m_ChartCtrl[i].SetAutoRepaint(true);
		m_ChartCtrl[i].Repaint();
	}*/
}




void CDlgAnalNystag::fillChartSummary()
{
	//chart summary를 fill
	//nystagmus slow velccity
	m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).Clear();

	double LN, LP, RN, RP;
	double target;

	LN = m_analyzeNys[LEFT].getMean(NEGA);	//left NEGA
	LP = m_analyzeNys[LEFT].getMean(POSI);	// left POSI
	RN = m_analyzeNys[RIGHT].getMean(NEGA);	//right NEGA
	RP = m_analyzeNys[RIGHT].getMean(POSI);	//right POSI

	//OKN일 경우 gain 표시
	if(this->m_TI.m_stiModeCommand == 0x06)	
	{
		target = 0;
        for(unsigned long i=0; i<this->m_ulDataCount; i++)
		{
			if(this->m_pdTarget[i] != 0)
			{
				target = m_pdTarget[i];
				break;
			}
		}

		//left NEGA
		m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).Add(target ==0 ? 0 : fabs(LN/target), "LN Gain", clPlot2);
		m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).Add(target ==0 ? 0 : fabs(LP/target), "LP Gain", clPlot2);
		m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).Add(target ==0 ? 0 : fabs(RN/target), "RN Gain", clPlot2);
		m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).Add(target ==0 ? 0 : fabs(RP/target), "RP Gain", clPlot2);
	}
	//LN/LP
	m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).Add(LP ==0 ? 0 : fabs(LN/LP), "LN/LP", clPlot3);
	//RN/RP
	m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).Add(RP ==0 ? 0 : fabs(RN/RP), "RN/RP", clPlot3);
	//LN/RN
	m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).Add(RN ==0 ? 0 : fabs(LN/RN), "LN/RN", clPlot3);
	//LP/RP
	m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).Add(RP ==0 ? 0 : fabs(LP/RP), "LP/RP", clPlot3);

	
	if(m_ChartCtrl[CHART_NYS_SUMMARY].Series(0).GetYValues().GetMaximum() > 
		m_ChartCtrl[CHART_NYS_SUMMARY].GetAxis().GetLeft().GetMaximum())
	{
		m_ChartCtrl[CHART_NYS_SUMMARY].GetAxis().GetLeft().SetAutomaticMaximum(true);
	}


}

void CDlgAnalNystag::fillListSummary()
{
	
	int LR = this->getRadioLR();
	m_listResultSummary.DeleteAllItems();	

	
	//if(LR!=BOTH)
	{
		structPrintNystag printNystag[2];
		this->analyzeForPrint(printNystag, false);

		double left[2] = {printNystag[0].LeftUpVel, printNystag[0].RightDownVel};
		double right[2] = {printNystag[1].LeftUpVel, printNystag[1].RightDownVel};

		//OKN 일 경우 Gain을 구한다. 
		if(m_TI.m_stiModeCommand == COMMAND_STI_OKN)
		{
			if(printNystag[0].targetVel != 0)
			{
				left[0] = fabs(left[0]/printNystag[0].targetVel);
				left[1] = fabs(left[1]/printNystag[0].targetVel);
				right[0] = fabs(right[0]/printNystag[0].targetVel);
				right[1] = fabs(right[1]/printNystag[0].targetVel);
			}
			else
				::AfxMessageBox("target velocity is 0");
		}

		corAdd2List(&m_listResultSummary, "Left Eye", left,2 );
		corAdd2List(&m_listResultSummary, "Right Eye", right, 2);

	}
/*
		double val[3];

		// count
		val[0] = this->m_analyzeNys[LR].getCount(NEGA);
		val[1] = this->m_analyzeNys[LR].getCount(POSI);
		val[2] = this->m_analyzeNys[LR].getCount(ALL);
		corAdd2List(&m_listResultSummary, "count", val, 3);

		// min
		val[0] = this->m_analyzeNys[LR].getMin(NEGA);
		val[1] = this->m_analyzeNys[LR].getMin(POSI);
		val[2] = this->m_analyzeNys[LR].getMin(ALL);
		corAdd2List(&m_listResultSummary, "min", val, 3);

		// max
		val[0] = this->m_analyzeNys[LR].getMax(NEGA);
		val[1] = this->m_analyzeNys[LR].getMax(POSI);
		val[2] = this->m_analyzeNys[LR].getMax(ALL);
		corAdd2List(&m_listResultSummary, "max", val, 3);

		// mean
		val[0] = this->m_analyzeNys[LR].getMean(NEGA);
		val[1] = this->m_analyzeNys[LR].getMean(POSI);
		val[2] = this->m_analyzeNys[LR].getMean(ALL);
		corAdd2List(&m_listResultSummary, "mean", val, 3);
		

	}*/
}
/*
void CDlgAnalNystag::add2List(CListCtrl* pList, CString strParam, double* pValue, int count)
{
	CString str;
	
	int nIdx = pList->InsertItem(0, strParam);

	for(int i=0; i<count; i++)
	{
		if(strParam == "count")
			str.Format("%.0f", pValue[i]);
		else
			str.Format("%.1f", pValue[i]);
		pList->SetItemText(nIdx, i+1, str);
	}
}*/
/*void CDlgAnalNystag::add2SummaryList(CString strParam, double* pValue, int count)
{
	CString str;
	
	int nIdx = m_listResultSummary.InsertItem(0, strParam);

	for(int i=0; i<count; i++)
	{
		str.Format("%.1f", pValue[i]);
		this->m_listResultSummary.SetItemText(nIdx, i+1, str);
	}
	
}*/


int CDlgAnalNystag::getRadioLR()
{
	//radio button이 어느눈을 선택하고 있는지 리턴한다.
	int LR;
	//determin eye
	if(((CButton*)GetDlgItem(IDC_RADIO_AN_LEFT))->GetCheck())
		LR = LEFT;
	else if(((CButton*)GetDlgItem(IDC_RADIO_AN_RIGHT))->GetCheck())
		LR = RIGHT;
	else
		LR = BOTH;

	return LR;
}

void CDlgAnalNystag::fillListDetail()
{
	int LR = getRadioLR();
	m_listResultDetail.DeleteAllItems();	


	POSITION pos1, pos2;
	
	
	if(LR == BOTH)
		return;

	CString strCount;
	//for(pos1 = m_analyzeNys.m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
	int count = int(m_analyzeNys[LR].m_listNystagmus.GetCount());
	for(pos1 = m_analyzeNys[LR].m_listNystagmus.GetTailPosition(); (pos2 = pos1) != NULL; )
	{
		//velocity chart에 표시
		structNystag* pNys = m_analyzeNys[LR].m_listNystagmus.GetPrev(pos1);

		strCount.Format("%d", count--);
		double val[4] = {double(pNys->startI)/FRAMERATE, double(pNys->endI)/FRAMERATE, 
			double(pNys->endI - pNys->startI)/FRAMERATE*1000, pNys->vel};
		corAdd2List(&m_listResultDetail, strCount, val, 4);		
	}
	
}
/*
void CDlgAnalNystag::add2DetailList(CString strParam, double* pValue, int count)
{
	CString str;
	
	int nIdx = m_listResultDetail.InsertItem(0, strParam);

	for(int i=0; i<count; i++)
	{
		str.Format("%.1f", pValue[i]);
		this->m_listResultDetail.SetItemText(nIdx, i+1, str);
	}
	
}*/


void CDlgAnalNystag::OnBnClickedRadioEyeSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int LR = this->getRadioLR();
	
	//if(((CButton*)GetDlgItem(IDC_RADIO_AN_LEFT))->GetCheck())
	if(LR == LEFT)
	{
		this->setLRVisible(true, false);
	}
	//else if(((CButton*)GetDlgItem(IDC_RADIO_AN_RIGHT))->GetCheck())
	else if(LR == RIGHT)
	{
		this->setLRVisible(false, true);
	}
	//else if(((CButton*)GetDlgItem(IDC_RADIO_AN_ALL))->GetCheck())
	else if (LR == BOTH)
	{
        this->setLRVisible(true, true);		
	}

	fillListSummary();
	fillListDetail();
	//선택된 nystagmus unselect
	selectNystagmusOnChart(-1);
	// list에서 선택된 nys를 해제한다.
	m_selectedNysIdx = -1;
}
void CDlgAnalNystag::setLRVisible(bool leftVisible, bool rightVisible)
{
	this->m_ChartCtrl[CHART_EYE_MOV].Series(LEFT).SetActive(leftVisible);
	this->m_ChartCtrl[CHART_EYE_MOV].Series(RIGHT).SetActive(rightVisible);
	this->m_ChartCtrl[CHART_EYE_MOV].Series(LEFT+3).SetActive(leftVisible);
	this->m_ChartCtrl[CHART_EYE_MOV].Series(RIGHT+3).SetActive(rightVisible);

	this->m_ChartCtrl[CHART_NYS_VEL].Series(LEFT).SetActive(leftVisible);
	this->m_ChartCtrl[CHART_NYS_VEL].Series(RIGHT).SetActive(rightVisible);
}
BEGIN_EVENTSINK_MAP(CDlgAnalNystag, CDialog)
	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_0, 1, OnAfterDrawTchartAn0, VTS_NONE)
	
	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_0, 27, OnZoomTchartAn, VTS_NONE)
	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_0, 26, OnUndoZoomTchartAn, VTS_NONE)
	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_0, 24, OnScrollTchartAn, VTS_NONE)

	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_1, 27, OnZoomTchartAn, VTS_NONE)
	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_1, 26, OnUndoZoomTchartAn, VTS_NONE)
	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_1, 24, OnScrollTchartAn, VTS_NONE)
	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_0, 3, OnClickTchart, VTS_NONE)
	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_0, 8, OnDblClickTchart, VTS_NONE)
	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_1, 3, OnClickTchart, VTS_NONE)
	ON_EVENT(CDlgAnalNystag, IDC_TCHART_AN_1, 8, OnDblClickTchart, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgAnalNystag::OnAfterDrawTchartAn0()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double axmin = m_ChartCtrl[CHART_EYE_MOV].GetAxis().GetBottom().GetMinimum();
	double axmax = m_ChartCtrl[CHART_EYE_MOV].GetAxis().GetBottom().GetMaximum();
	
	m_ChartCtrl[CHART_NYS_VEL].GetAxis().GetBottom().SetMinMax(axmin, axmax);
	
}


void CDlgAnalNystag::OnLvnItemchangedListAnResultDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
    long lRow = 0;                                  // 로우
    int nCol = 0;                                   // 컬럼

	//새롭게 선택된것
	if(temp->uNewState == 3)
	{

		lRow = temp->iItem;                             // 로우값(클릭한 로우리턴)
		nCol = temp->iSubItem;                          // 컬럼값(클릭한 컬럼리턴 : 님 경우 별 쓸모없겠지만..)

		this->selectNystagmusOnChart(lRow);
		m_selectedNysIdx = lRow;
	}
}



void CDlgAnalNystag::OnNMDblclkListAnResultDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
    long lRow = 0;                                  // 로우
    int nCol = 0;                                   // 컬럼

    lRow = temp->iItem;                             // 로우값(더블클릭한 로우리턴)
    nCol = temp->iSubItem;                          // 컬럼값(더블클릭한 컬럼리턴 : 님 경우 별 쓸모없겠지만..)

	enterEditMode(true, lRow);
}

void CDlgAnalNystag::selectNystagmusOnChart(int count)
{
	//if(this->m_bEditing)
	//	this->enterEditMode(false, 0);

	// colorband를 해당 nystagmus위에 올려 놓는다.
	// -1일경우 invisible 시킨다.
	int LR = getRadioLR();
	if(LR == BOTH)
		return;

	if(count == -1)
	{
		m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).SetActive(false);
		m_ChartCtrl[CHART_NYS_VEL].GetTools().GetItems(0).SetActive(false);

		this->m_pNysSelected = NULL;
		if(this->m_bEditing)
			this->m_NysSaved = *m_pNysSelected;
	}
	else
	{
		m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).SetActive(true);
		m_ChartCtrl[CHART_NYS_VEL].GetTools().GetItems(0).SetActive(true);
        
		POSITION pos1, pos2;
		int n = 0;
		for(pos1 = m_analyzeNys[LR].m_listNystagmus.GetHeadPosition(); (pos2 = pos1) != NULL; n++)
		{
			this->m_pNysSelected = m_analyzeNys[LR].m_listNystagmus.GetNext(pos1);
			if(n == count)
				break;
		}
		if(this->m_bEditing)
			this->m_NysSaved = *m_pNysSelected;

		m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).GetAsColorband().SetStartValue(double(m_pNysSelected->startI)/FRAMERATE-0.1);
		m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).GetAsColorband().SetEndValue(double(m_pNysSelected->endI)/FRAMERATE+0.1);

		m_ChartCtrl[CHART_NYS_VEL].GetTools().GetItems(0).GetAsColorband().SetStartValue(double(m_pNysSelected->startI)/FRAMERATE-0.1);
		m_ChartCtrl[CHART_NYS_VEL].GetTools().GetItems(0).GetAsColorband().SetEndValue(double(m_pNysSelected->endI)/FRAMERATE+0.1);

	}
}


BOOL CDlgAnalNystag::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(this->m_bEditing)
		{
			bool bShift = ::GetAsyncKeyState(VK_LSHIFT);

			if(bShift)
			{
				if(pMsg->wParam == 37)	//left
				{
					moveSelectedNys(0, 0, -1);	//왼쪽으로 이동
					return true;
				}
				else if(pMsg->wParam == 39)	//right
				{
					moveSelectedNys(0, 0, 1);	//오른쪽으로 이동
					return true;
				}
			}
			else
			{
				// left right는 length
				if(pMsg->wParam == 37)	//left
				{
					moveSelectedNys(0, -1, 0);
					return true;
				}
				else if(pMsg->wParam == 39)	//right
				{
					moveSelectedNys(0, 1, 0);
					return true;
				}
				//up down은 기울기
				else if(pMsg->wParam == 38) // up
				{
					moveSelectedNys(0.1, 0, 0);
					return true;
				}
				else if(pMsg->wParam == 40)	//down
				{
					moveSelectedNys(-0.1, 0, 0);
					return true;
				}
			}
		}
		else
		{
			if(pMsg->wParam == 46) //DEL
			{
				int LR = this->getRadioLR();
				if(( (LR == LEFT) || (LR == RIGHT) )  && (this->m_selectedNysIdx != -1))
				{
					deleteSelectedNys(LR, m_selectedNysIdx);				
				}
			}
		}

	}

	if ((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN))
		{
			if(this->m_bEditing)
			{
				if(pMsg->wParam == VK_ESCAPE)
					this->enterEditMode(false, 0);	// 취소
				else
					this->enterEditMode(false, 1);;	//적용
			}
			

			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return true;
		}

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgAnalNystag::deleteSelectedNys(int LR, int nysIdx)
{
	// 1. list에서 삭제
	// 2. 결과 update
	// 3. chart update
	// 4. list control에서 삭제
	// 5. 다음 item 선택

	unsigned int count = int(m_analyzeNys[LR].m_listNystagmus.GetCount());

	//validation
	if( ((LR != LEFT) && (LR != RIGHT)) || (nysIdx >= m_analyzeNys[LR].m_listNystagmus.GetCount()))
		return;

	// 1. list에서 삭제
	POSITION pos = this->m_analyzeNys[LR].m_listNystagmus.FindIndex(nysIdx);
	structNystag* pNys = m_analyzeNys[LR].m_listNystagmus.GetAt(pos);
	delete pNys;
	m_analyzeNys[LR].m_listNystagmus.RemoveAt(pos);


	// 2. 3. chart 및 list update
	showAnalyze();
	fillChartSummary();
	fillListSummary();
	fillListDetail();

	//updateResult(LR);

	// 4. detail list에서 선택된 아이템 삭제 
	m_listResultDetail.DeleteItem(nysIdx);

	// 5. 다음 아이템 선택
	// 마지막 item이 아니라면 다음 nystagmus를 선택하고 마지막이라면 이전 것을 선택한다.
	if(nysIdx == count-1)
		nysIdx--;
	if(nysIdx >= 0)
	{
		m_listResultDetail.SetSelectionMark(nysIdx);
		m_listResultDetail.SetItemState(nysIdx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_listResultDetail.SetFocus();

		//chat의 colorband는 selection을 바꿈으로써 OnLvnItemChagedList()에서 이동한다.
	}
	else
	{
		// 더이상 nystagmus가 없다면
		// colorBand를 inactive 시킨다.
		selectNystagmusOnChart(-1);

	}
	
}

void CDlgAnalNystag::OnZoomTchartAn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	CTChart* pTChart[2];
	pTChart[0] = &(m_ChartCtrl[0]);
	pTChart[1] = &(m_ChartCtrl[1]);

	//마우스가 위치한 chart를 찾는다.
	for(int i=0; i<2; i++)
	{
		pTChart[i]->GetWindowRect(rect);
		if(pTChart[i]->GetVisible() && rect.PtInRect(pt))
			chartIdx = i;
	}

	//chartIdx 의 bottom axis와 같게 만든다.
	if(chartIdx != -1)
	{
		double m = pTChart[chartIdx]->GetAxis().GetBottom().GetMinimum();
		double M = pTChart[chartIdx]->GetAxis().GetBottom().GetMaximum();

		for(int i=0; i<2; i++)
		{
			if(i!= chartIdx)
			{
				pTChart[i]->GetAxis().GetBottom().SetMinMax(m, M);
				//다른 chart의 zoom을 끈다.
                pTChart[i]->GetZoom().SetEnable(false);
				pTChart[i]->GetScroll().SetEnable(false);
			}
			
		}
	}
}

void CDlgAnalNystag::OnUndoZoomTchartAn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	CTChart* pTChart[2];
	pTChart[0] = &(m_ChartCtrl[0]);
	pTChart[1] = &(m_ChartCtrl[1]);

	//마우스가 위치한 chart를 찾는다.
	for(int i=0; i<2; i++)
	{
		pTChart[i]->GetWindowRect(rect);
		if(pTChart[i]->GetVisible() && rect.PtInRect(pt))
			chartIdx = i;
	}

	//chartIdx 의 bottom axis와 같게 만든다.
	if(chartIdx != -1)
	{
		double m = pTChart[chartIdx]->GetAxis().GetBottom().GetMinimum();
		double M = pTChart[chartIdx]->GetAxis().GetBottom().GetMaximum();

		for(int i=0; i<2; i++)
		{
			if(i!= chartIdx)
			{
				pTChart[i]->GetAxis().GetBottom().SetMinMax(m, M);
				//다른 chart의 zoom을 끈다.
                pTChart[i]->GetZoom().SetEnable(true);
				pTChart[i]->GetScroll().SetEnable(true);
			}
			
		}
	}
}

void CDlgAnalNystag::OnScrollTchartAn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	CTChart* pTChart[2];
	pTChart[0] = &(m_ChartCtrl[0]);
	pTChart[1] = &(m_ChartCtrl[1]);

	//마우스가 위치한 chart를 찾는다.
	for(int i=0; i<2; i++)
	{
		pTChart[i]->GetWindowRect(rect);
		if(pTChart[i]->GetVisible() && rect.PtInRect(pt))
			chartIdx = i;
	}

	//chartIdx 의 bottom axis와 같게 만든다.
	if(chartIdx != -1)
	{
		double m = pTChart[chartIdx]->GetAxis().GetBottom().GetMinimum();
		double M = pTChart[chartIdx]->GetAxis().GetBottom().GetMaximum();

		for(int i=0; i<2; i++)
		{
			if(i!= chartIdx)
			{
				pTChart[i]->GetAxis().GetBottom().SetMinMax(m, M);
				//다른 chart의 zoom을 끈다.
                pTChart[i]->GetZoom().SetEnable(false);
				pTChart[i]->GetScroll().SetEnable(false);
			}
			
		}
	}
}


void CDlgAnalNystag::enterEditMode(bool bEdit, unsigned long nysIdx)
{
	int LR = this->getRadioLR();

	
	if(bEdit)
	{
		//edit mode로 들어간다.
		
		m_bEditing = true;
		this->m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).GetAsColorband().SetColor(COLOR_EDIT);

		unsigned int count = (unsigned int)(this->m_analyzeNys[LR].m_listNystagmus.GetCount());

		//validation
		if( ((LR != LEFT) && (LR != RIGHT)) || (nysIdx >= count))
			return;

		// 선택된 pursuit를 찾고 저장한다.
		if(this->m_pNysSelected)
			this->m_NysSaved = (*m_pNysSelected);
	}
	else
	{
		//edit mode에서 나간다.
		m_bEditing = false;
		m_ChartCtrl[CHART_EYE_MOV].GetTools().GetItems(0).GetAsColorband().SetColor(COLOR_SELECTED);

		if(nysIdx == 0)	//edit cancel
		{
			// 저장해 놓았던 것을 load한다.
			if(m_pNysSelected)
				(*m_pNysSelected) = this->m_NysSaved;
		
			this->showAnalyze();
			this->fillChartSummary();
			this->fillListSummary();
			double val[4] = {double(m_pNysSelected->startI)/FRAMERATE, 
						double(m_pNysSelected->endI)/FRAMERATE, 
                        double(m_pNysSelected->endI - m_pNysSelected->startI)/FRAMERATE*1000, 
						m_pNysSelected->vel};
			corChangeListValue(&m_listResultDetail, this->m_selectedNysIdx, val, 4);
		}
		

	}
}

void CDlgAnalNystag::moveSelectedNys(double vel, int length, int move_x)
{
	if(this->m_pNysSelected)
	{
		int LR = this->getRadioLR();
		double* pEye = (LR==LEFT) ? m_pdLeft :m_pdRight;

		//길이 조정
		if(length)
		{
			//늘리거나 or 충분히 크면서 줄일때만 
			if((length>0) ||  (m_pNysSelected->endI - m_pNysSelected->startI) >2)
			{
				m_pNysSelected->endI += length;
				m_pNysSelected->startI -= length;
			}

			
		}

		// 움직임
		m_pNysSelected->endI += move_x;
		m_pNysSelected->startI += move_x;
		
		
		if(length || move_x)
		{
			//속도 계산
			double t = double(m_pNysSelected->endI-m_pNysSelected->startI)/FRAMERATE;
			double d = pEye[m_pNysSelected->endI]-pEye[m_pNysSelected->startI];
			if(t)
				m_pNysSelected->vel = d/t;
		}

		//기울기 조정
		else if(vel)
		{
			m_pNysSelected->vel += vel;			
		}


		this->showAnalyze();
		this->fillChartSummary();
		this->fillListSummary();
		//this->fillListDetail();

		double val[4] = {double(m_pNysSelected->startI)/FRAMERATE, 
						double(m_pNysSelected->endI)/FRAMERATE, 
                        double(m_pNysSelected->endI - m_pNysSelected->startI)/FRAMERATE*1000, 
						m_pNysSelected->vel};
		corChangeListValue(&m_listResultDetail, this->m_selectedNysIdx, val, 4);


	}
}


void CDlgAnalNystag::OnBnClickedButtonAnRefresh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int LR = this->getRadioLR();

	this->analyzeThis(LR);
	this->showAnalyze();
	this->fillListSummary();
	this->fillListDetail();
}

void CDlgAnalNystag::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	saveAnalyzeData();

	OnOK();
}

void CDlgAnalNystag::OnClickTchart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	bool bShift = ::GetAsyncKeyState(VK_LSHIFT);
	if(bShift)
	{
		//nystagmus를 추가한다.

		
		int LR = this->getRadioLR();

		if(LR != BOTH)
		{
			// 1. mouse pos를 받는다.
			CTChart* pCharts[] = {&(m_ChartCtrl[0]), &(m_ChartCtrl[1])};

			CRect rect;
			CPoint p;
			::GetCursorPos(&p);
			
			CTChart* pTChart = NULL;
			for(int i=0; i<2; i++)
			{
				pCharts[i]->GetWindowRect(&rect);
				if(rect.PtInRect(p))
					pTChart = pCharts[i];
			}
			
			if(pTChart)
			{
				CTeePoint2D point = pTChart->GetMousePosition();
				double x = pTChart->GetAxis().GetBottom().CalcPosPoint(long(point.GetX()));
			
				// 2. add nystagmus
				double* pEye[2] = {m_pdLeft, m_pdRight};
				unsigned long idx = m_analyzeNys[LR].addNystagmus((unsigned long)(x*FRAMERATE), pEye[LR], this->m_ulDataCount);		
				
				
				if(idx != -1)
				{
					// 3. List 및 chart update
					showAnalyze();
					fillChartSummary();
					fillListSummary();
					fillListDetail();

					// 4. add 되었으면 edit 모드로
					selectListDetail(idx);
					selectNystagmusOnChart(idx);	
				}
			}
		}
		

	}
	else
	{
		unsigned long idx = getClickedNystag();
		if(idx != -1)
		{
			selectListDetail(idx);
			selectNystagmusOnChart(idx);	
		}
	}
}

void CDlgAnalNystag::OnDblClickTchart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	

	//edit mode로 들어간다.
	unsigned long idx = getClickedNystag();
	if(idx != -1)
	{
		selectListDetail(idx);
		selectNystagmusOnChart(idx);
		this->enterEditMode(true, idx);
	}
	
}


unsigned long CDlgAnalNystag::getClickedNystag()
{
	// mouse 위치에서 가장 가까운 pursuit를 찾아 index를 리턴한다.
	unsigned long idx = -1;
	int LR = this->getRadioLR();

	if(LR != BOTH)
	{
		CTChart* pCharts[] = {&(m_ChartCtrl[0]), &(m_ChartCtrl[1])};

		CRect rect;
		CPoint p;
		::GetCursorPos(&p);
		
		CTChart* pTChart = NULL;
		for(int i=0; i<2; i++)
		{
			pCharts[i]->GetWindowRect(&rect);
			if(rect.PtInRect(p))
				pTChart = pCharts[i];
		}
		
		if(pTChart)
		{
			CTeePoint2D point = pTChart->GetMousePosition();
			double x = pTChart->GetAxis().GetBottom().CalcPosPoint(long(point.GetX()));

			
			idx = this->m_analyzeNys[LR].getClosestNystagmus(x);
		}
		
	}
	return idx;
}

void CDlgAnalNystag::selectListDetail(unsigned long idx)
{
	this->fillListDetail();
	if(idx >= 0)
	{
		m_listResultDetail.SetSelectionMark(idx);
		m_listResultDetail.SetItemState(idx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_listResultDetail.SetFocus();

		m_listResultDetail.EnsureVisible(idx, FALSE);

	}
}

void CDlgAnalNystag::analyzeForPrint(structPrintNystag* pNystag, bool forPrint)
{
	if(forPrint)
	{
		this->loadData();

		//먼저 ANALYZE하고 ANALYZE FILE이 있으면 결과만 덮어쓴다.
		analyzeThis(BOTH);
		loadAnalyzeData();
	}




	memset(pNystag, 0, sizeof(structPrintNystag)*2);

	double LU[2], RD[2];
	LU[0] = LU[1] = RD[0] = RD[1] = 0;
	int nLU[2], nRD[2];
	nLU[0] = nLU[1] = nRD[0] = nRD[1] = 0;
	
	POSITION pos;
	int count;
	structNystag* pNys  = NULL;
	
	//left eye
	int bi = LEFT;
	count = m_analyzeNys[bi].m_listNystagmus.GetCount();
	pos = m_analyzeNys[bi].m_listNystagmus.GetHeadPosition();
	for(int i=0; i<count; i++)
	{
		pNys = m_analyzeNys[bi].m_listNystagmus.GetNext(pos);
		if((pNys) && (pNys->vel > 0))	//left beating
		{
			LU[bi] += pNys->vel;
			nLU[bi]++;
		}
		else
		{
			RD[bi] += pNys->vel;		//rigiht beating
			nRD[bi]++;
		}
	}
	pNystag[bi].LeftUpVel = nLU[bi] ? LU[bi]/nLU[bi] : 0;
	pNystag[bi].RightDownVel = nRD[bi] ? RD[bi]/nRD[bi] : 0;

	//right eye
	bi = RIGHT;
	count = m_analyzeNys[bi].m_listNystagmus.GetCount();
	pos = m_analyzeNys[bi].m_listNystagmus.GetHeadPosition();
	for(int i=0; i<count; i++)
	{
		pNys = m_analyzeNys[bi].m_listNystagmus.GetNext(pos);
		if((pNys) && (pNys->vel > 0))		//up beating
		{
			LU[bi] += pNys->vel;
			nLU[bi]++;
		}
		else
		{
			RD[bi] += pNys->vel;			//down beating
			nRD[bi]++;
		}
	}

	pNystag[bi].LeftUpVel = nLU[bi] ? LU[bi]/nLU[bi] : 0;
	pNystag[bi].RightDownVel = nRD[bi] ? RD[bi]/nRD[bi] : 0;


	//targetvelocity
	//OKN일 경우 gain 표시
	if(this->m_TI.m_stiModeCommand == 0x06)	
	{
		double target = 0;
		for(unsigned long i=0; i<this->m_ulDataCount; i++)
		{
			if(this->m_pdTarget[i] != 0)
			{
				target = m_pdTarget[i];
				break;
			}
		}
		pNystag[0].targetVel = pNystag[1].targetVel = fabs(target);
	}

}