// DlgAnalyzeHeadThrust.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgAnalyzeHeadThrust.h"
#include ".\dlganalyzeheadthrust.h"
#include ".\eu_testfilev2.h"
#include ".\corwldefines.h"
#include ".\includetchart.h"
#include "corwlUtil.h"
#include ".\reportGenerator\reportGenerator.h"
#include "eu_eventfile.h"

// CDlgAnalyzeHeadThrust 대화 상자입니다.

//#define COLOR_EYE_VEL RGB(192, 128, 0)
//#define COLOR_EYE_VEL RGB(254, 176, 120)
#define COLOR_EYE_MEAN_VEL	RGB(255, 64, 64)
#define COLOR_EYE_VEL COLOR_EYE_MEAN_VEL
//#define COLOR_HEAD_VEL RGB(150, 150, 150)
#define COLOR_HEAD_MEAN_VEL RGB(0, 0, 0)
#define COLOR_HEAD_VEL COLOR_HEAD_MEAN_VEL
#define COLOR_EYE_VEL_SELECTED RGB(128, 254, 64)
#define COLOR_HEAD_VEL_SELECTED (128, 128, 128)

void setListCheckbox(CListCtrl* pListCtrl, int itemIndex, BOOL bCheck);


IMPLEMENT_DYNAMIC(CDlgAnalyzeHeadThrust, CDialog)
CDlgAnalyzeHeadThrust::CDlgAnalyzeHeadThrust(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalyzeHeadThrust::IDD, pParent)
{
	m_ulEyeDataCount = m_ulAccDataCount = 0;
	m_pdLeft = m_pdRight = m_pdVelLeft = m_pdVelRight = m_pdVelHead = NULL;

	m_eventSeriesIdx = -1;
}

CDlgAnalyzeHeadThrust::~CDlgAnalyzeHeadThrust()
{
	DEL(m_pdLeft);
	DEL(m_pdRight);
	
	DEL(m_pdVelLeft);
	DEL(m_pdVelRight);
	
	DEL(m_pdVelHead);
}

void CDlgAnalyzeHeadThrust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_HT_LEFT_UP, m_chart[0]);
	DDX_Control(pDX, IDC_TCHART_HT_RIGHT_UP, m_chart[1]);
	DDX_Control(pDX, IDC_TCHART_HT_LEFT_DOWN, m_chart[2]);
	DDX_Control(pDX, IDC_TCHART_HT_RIGHT_DOWN, m_chart[3]);
	DDX_Control(pDX, IDC_TCHART_HT_LEFT_UP_VEL, m_chartVel[0]);
	DDX_Control(pDX, IDC_TCHART_HT_RIGHT_UP_VEL, m_chartVel[1]);
	DDX_Control(pDX, IDC_TCHART_HT_LEFT_DOWN_VEL, m_chartVel[2]);
	DDX_Control(pDX, IDC_TCHART_HT_RIGHT_DOWN_VEL, m_chartVel[3]);
	DDX_Control(pDX, IDC_TCHART_HEAD_THRUST_MAIN, m_chartMain);
	DDX_Control(pDX, IDC_LIST_HT_UP, m_listHTUp);
	DDX_Control(pDX, IDC_LIST_HT_DOWN, m_listHTDown);
	DDX_Control(pDX, IDC_LIST_HT_SUMMARY, m_listHTSummary);
	
}


BEGIN_MESSAGE_MAP(CDlgAnalyzeHeadThrust, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_HT_REANALYZE, OnBnClickedButtonHtReanalyze)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_HT_DOWN, OnLvnItemchangedListHtDown)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_HT_UP, OnLvnItemchangedListHtUp)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_HEAD_THRUST_OPTION, OnBnClickedButtonHeadThrustOption)
	ON_BN_CLICKED(IDC_BUTTON_HT_PRINT, OnBnClickedButtonHtPrint)
END_MESSAGE_MAP()


// CDlgAnalyzeHeadThrust 메시지 처리기입니다.

BOOL CDlgAnalyzeHeadThrust::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ShowWindow(SW_SHOWMAXIMIZED);
	layoutCtrls();
	this->initListCtrls();
	initTChart();

	if(loadData())
	{
		m_nSelectedHT = 0;

		//eye velocity를 계산한다.
		this->calEyeVelocity();
	

		//먼저 ANALYZE하고 ANALYZE FILE이 있으면 결과만 덮어쓴다.
		analyzeThis();

		
		loadAnalyzeData();

		showResult2();

		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgAnalyzeHeadThrust::layoutCtrls()
{
	//double gap = 0.005;
	double gap = 0.00;
	double button_h = 0.04;
	double button_w = button_h*2.5;
	
	double chart_w = (1.-6*gap)/6;
	double chart_h = (1-5*gap-button_h)/3;
		
	CRect rect;
	this->GetClientRect(&rect);

	//main chart
	//corLocateWindow(&m_chartMain, gap, gap, 1-3*gap-2*chart_w, chart_h, rect);
	corLocateWindow(&m_chartMain, gap, gap, 1-2*gap, chart_h, rect);
	corLocateWindow(GetDlgItem(IDC_LIST_HT_SUMMARY), 1-gap-2*chart_w, gap, 2*chart_w, chart_h, rect);

	//up
	double top = gap + chart_h+gap;
	
	DWORD ids_up[] = {IDC_TCHART_HT_LEFT_UP, IDC_TCHART_HT_LEFT_UP_VEL, IDC_TCHART_HT_RIGHT_UP, IDC_TCHART_HT_RIGHT_UP_VEL, IDC_LIST_HT_UP};
	DWORD ids_down[] = {IDC_TCHART_HT_LEFT_DOWN, IDC_TCHART_HT_LEFT_DOWN_VEL, IDC_TCHART_HT_RIGHT_DOWN, IDC_TCHART_HT_RIGHT_DOWN_VEL, IDC_LIST_HT_DOWN};

	for(int i=0; i<5; i++)
	{
		corLocateWindow(GetDlgItem(ids_up[i]), i*(gap+chart_w), top, (i==4)? chart_w*2 : chart_w, chart_h, rect);
		corLocateWindow(GetDlgItem(ids_down[i]), i*(gap+chart_w), top+gap+chart_h, (i==4)? chart_w*2 : chart_w, chart_h, rect);
	}

	// buttons on bottom
	gap = 0.01;
	corLocateWindow(GetDlgItem(IDC_BUTTON_HT_REANALYZE), gap, 1-.9*button_h, button_w, .8*button_h, rect);
	corLocateWindow(GetDlgItem(IDC_BUTTON_HEAD_THRUST_OPTION), 2*gap+button_w, 1-.9*button_h, button_w, .8*button_h, rect);
	corLocateWindow(GetDlgItem(IDC_BUTTON_HT_PRINT), 3*gap + 2*button_w, 1-.9*button_h, button_w, .8*button_h, rect);
	corLocateWindow(GetDlgItem(IDCANCEL), 1-2*(button_w+0.01), 1-.9*button_h, button_w, .8*button_h, rect);
	corLocateWindow(GetDlgItem(IDOK), 1-(button_w+0.01), 1-.9*button_h, button_w, .8*button_h, rect);

}

bool CDlgAnalyzeHeadThrust::loadData()
{
	bool bRtn = false;

	EU_TestFileV2 testFile;
	
	if(testFile.open(this->m_strTestFile, &m_TI, false))
	{
		testFile.readTI(&m_TI);
		m_ulEyeDataCount = m_ulAccDataCount = m_TI.m_lEyeDataCount;
		if(m_ulEyeDataCount)
		{
			this->allocateVariable();


			if(m_TI.m_analTarget == HORIZONTAL)
				testFile.readTestData(true, this->m_pdLeft, m_pdRight, //H
														NULL, NULL,		//V
														NULL, NULL,		//T
														NULL, NULL,		//R
														NULL,	NULL, m_pdVelHead, //GYRO
														NULL, NULL,		//acc
														NULL, NULL);		//stimulation target
			else
				testFile.readTestData(true, NULL, NULL, //H
											m_pdLeft, m_pdRight,		//V
											NULL, NULL,		//T
											NULL, NULL,		//R
											NULL, m_pdVelHead,	NULL, //ACC
											NULL, NULL,		//gyro
											NULL, NULL);		//stimulation target

			bRtn = true;
		}

	}

	/*for(int i=0; i<m_ulEyeDataCount; i++)
	{
		m_pdLeft[i] = m_pdLeft[i]/10;
		m_pdRight[i] = m_pdRight[i]/10;
	}*/

	return bRtn;
}

void CDlgAnalyzeHeadThrust::calEyeVelocity()
{
	//i번째 데이터의 velocity는	[i+M] - [i-M] 번째 데이터로 계산
	// (2M/120)으로 나눠줘야 degree/sec값이 나온다.
	// 처음 M개와 마지막 M개 값은 0	

	unsigned char M = 2;	//velocity를 계산할 lag. 2*M	

	if(this->m_ulEyeDataCount)
	{
		DEL(m_pdVelLeft);
		DEL(m_pdVelRight);

		m_pdVelLeft = new double[this->m_ulEyeDataCount];
		m_pdVelRight = new double[this->m_ulEyeDataCount];


		memset(m_pdVelLeft, 0, sizeof(double)*m_ulEyeDataCount);
		memset(m_pdVelRight, 0, sizeof(double)*m_ulEyeDataCount);

		//처음 M개와 마지막 M개는 0
		memset(m_pdVelLeft, 0, sizeof(double)*2);
		memset(&(m_pdVelLeft[m_ulEyeDataCount-2]), 0, sizeof(double)*2);
		memset(m_pdVelRight, 0, sizeof(double)*2);
		memset(&(m_pdVelRight[m_ulEyeDataCount-2]), 0, sizeof(double)*2);


		for(unsigned int i=M+1; i<m_ulEyeDataCount-M-1; i++)
		{
			// (2M/120) 으로 나눠줘야 한다.
			m_pdVelLeft[i] = (m_pdLeft[i+M]-m_pdLeft[i-M])*60./M;
			m_pdVelRight[i] = (m_pdRight[i+M]-m_pdRight[i-M])*60./M;
		}


	}

}


void CDlgAnalyzeHeadThrust::allocateVariable()
{
	DEL(m_pdLeft);
	m_pdLeft = new double[m_ulEyeDataCount];
	DEL(m_pdRight);
	m_pdRight = new double[m_ulEyeDataCount];
	
	DEL(m_pdVelLeft);
	m_pdVelLeft = new double[m_ulEyeDataCount];
	DEL(m_pdVelRight);
	m_pdVelRight = new double[m_ulEyeDataCount];
	

	DEL(m_pdVelHead);
	m_pdVelHead = new double[m_ulAccDataCount];
	
}

void CDlgAnalyzeHeadThrust::analyzeThis()
{
	m_analyzeHeadThrust[0].m_bUseMaxHeadSpeedLimit = m_analyzeHeadThrust[1].m_bUseMaxHeadSpeedLimit = this->m_dlgHeadThrustOption.m_bUseMaxHeadSpeedLimit;
	m_analyzeHeadThrust[0].m_dMaxHeadSpeedLimit = m_analyzeHeadThrust[1].m_dMaxHeadSpeedLimit = this->m_dlgHeadThrustOption.m_dMaxHeadSpeedLimit;
	m_analyzeHeadThrust[0].m_dMinHeadSpeedLimit = m_analyzeHeadThrust[1].m_dMinHeadSpeedLimit = this->m_dlgHeadThrustOption.m_dMinHeadSpeedLimit;


	this->m_analyzeHeadThrust[0].analyze(m_pdVelLeft, m_ulEyeDataCount, m_pdVelHead, m_ulAccDataCount);
	this->m_analyzeHeadThrust[1].analyze(m_pdVelRight, m_ulEyeDataCount, m_pdVelHead, m_ulAccDataCount);

}

void CDlgAnalyzeHeadThrust::matchLeftRight()
{
	CList<structHeadThrust*, structHeadThrust*>* pList0 = &(m_analyzeHeadThrust[0].m_listHeadThrust);
	CList<structHeadThrust*, structHeadThrust*>* pList1 = &(m_analyzeHeadThrust[1].m_listHeadThrust); 

	ASSERT(pList0->GetCount() == pList1->GetCount());

	int count = (int)pList0->GetCount();
	if(!count)
		return;


	POSITION pos0, pos1;
	pos0 = pList0->GetHeadPosition();
	pos1 = pList1->GetHeadPosition();

	structHeadThrust* pHT0, *pHT1;

	//두 HT가 모두 valid한지 확인
	for(int i=0; i<count; i++)
	{
		pHT0 = pList0->GetNext(pos0);
		pHT1 = pList1->GetNext(pos1);

		if(pHT0 && pHT1 && (pHT0->headPeakIdx == pHT1->headPeakIdx) &&
			(!pHT0->valid || !pHT1->valid))
		{
			pHT0->valid = false;
			pHT1->valid = false;
		}
	}
}

bool CDlgAnalyzeHeadThrust::loadAnalyzeData()
{
	bool bRtn = true;

	CString str;
	CFileStatus fs;
	//Left
	str = this->m_strTestFile;
	str.Replace(".emd", "L.ema");
	if(CFile::GetStatus(str, fs))
		this->m_analyzeHeadThrust[0].load(str);
	else
		bRtn = false;

	//Right
	str = this->m_strTestFile;
	str.Replace(".emd", "R.ema");
	if(CFile::GetStatus(str, fs))
		this->m_analyzeHeadThrust[1].load(str);
	else
		bRtn = false;

	return bRtn;

}


void CDlgAnalyzeHeadThrust::initTChart()
{
	//chart main
	m_chartMain.ClearChart();
	m_chartMain.ApplyPalette(cpWeb);
	m_chartMain.GetAspect().SetView3D(false);
	m_chartMain.ApplyPalette(cpWeb);
	m_chartMain.GetLegend().SetCheckBoxes(true);
	m_chartMain.GetLegend().SetAlignment(laBottom);
	m_chartMain.GetLegend().GetShadow().SetVisible(false);
	m_chartMain.GetAxis().GetLeft().GetTitle().SetCaption("Angluar Velocity(°/sec)");
	m_chartMain.GetAxis().GetBottom().GetTitle().SetCaption("Time (sec)");

	

	CString titles[] = {"Left\r\nPositive", "Right\r\nPositive", "Left\r\nNegative", "Right\r\nNegative"};
	//time-velocity curve
	for(int i=0; i<4; i++)
	{
		
		this->m_chart[i].ClearChart();
		this->m_chart[i].GetAspect().SetView3D(false);
		m_chart[i].GetLegend().SetVisible(false);
		
		m_chart[i].GetAxis().GetLeft().GetTitle().SetCaption("Angluar Velocity(°/sec)");
		m_chart[i].GetAxis().GetBottom().GetTitle().SetCaption("Time (ms)");
		m_chart[i].GetAxis().GetBottom().SetAutomatic(false);
		m_chart[i].GetAxis().GetBottom().SetMinMax(-100, 400);

		m_chart[i].GetAxis().GetLeft().SetAutomatic(false);
		if(i<2)
			m_chart[i].GetAxis().GetLeft().SetMinMax(-100, 500);
		else
			m_chart[i].GetAxis().GetLeft().SetMinMax(-500, 100);


		//set annotation tool
		/*double left, top, right, bottom;
		left = m_chartVel.GetAxis().GetLeft().CalcXPosValue(m_chartCaloric.GetAxis().GetLeft().GetMinimum());
		top = m_chartVel.GetAxis().GetLeft().CalcYPosValue(m_chartCaloric.GetAxis().GetLeft().GetMaximum());
		CRect chartRect;
		m_chartVel.GetWindowRect(chartRect);*/
		int idx = m_chart[i].GetTools().Add(tcAnnotate);
		CAnnotationTool ann = m_chart[i].GetTools().GetItems(idx).GetAsAnnotation();
		ann.SetText(titles[i]);
		ann.GetShape().GetShadow().SetVisible(false);	//그림자 제거
		ann.SetLeft(55);
		ann.SetTop(15);

		/*
		//set annotation position
		m_chartCaloric.Repaint();	//annotation의 크기를 알기 위해 그려준다.
		//axis 끝에 위치하게 한다.
		int w = ann.GetWidth();
		int h = ann.GetHeight();
		ann.SetLeft(long(i%2 ? right-w : left+w*.5));
		ann.SetTop(long(i<2 ? top : bottom-h));
		*/
	}

	//head acc - eye acc plot
	for(int i=0;  i<4; i++)
	{
		this->m_chartVel[i].ClearChart();
		m_chartVel[i].GetAspect().SetView3D(false);
		m_chartVel[i].GetLegend().SetVisible(false);

		m_chartVel[i].GetAxis().GetLeft().GetTitle().SetCaption("Eye Acceleration(°/sec2)");
		m_chartVel[i].GetAxis().GetBottom().GetTitle().SetCaption("Head Acceleration(°/sec2)");

		m_chartVel[i].GetAxis().GetLeft().SetAutomatic(false);
		m_chartVel[i].GetAxis().GetBottom().SetAutomatic(false);
		if(i<2)
		{
			m_chartVel[i].GetAxis().GetLeft().SetMinMax(0, 10000);
			m_chartVel[i].GetAxis().GetBottom().SetMinMax(0, 10000);
		}
		else
		{
			m_chartVel[i].GetAxis().GetLeft().SetMinMax(-10000, 0);
			m_chartVel[i].GetAxis().GetLeft().SetInverted(true);
			m_chartVel[i].GetAxis().GetBottom().SetMinMax(-10000, 0);
			m_chartVel[i].GetAxis().GetBottom().SetInverted(true);
		}



		int seriesIdx = m_chartVel[i].AddSeries(scPoint);
		m_chartVel[i].Series(seriesIdx).SetColorEachPoint(true);
		m_chartVel[i].Series(seriesIdx).GetAsPoint().GetPointer().SetStyle(psCircle);
		m_chartVel[i].Series(seriesIdx).GetAsPoint().GetPointer().SetHorizontalSize(3);
		m_chartVel[i].Series(seriesIdx).GetAsPoint().GetPointer().SetVerticalSize(3);
		//m_chartVel[i].Series(seriesIdx).GetAsPoint().GetPointer().SetTransparency(30);
		m_chartVel[i].Series(seriesIdx).GetAsPoint().GetPointer().GetBrush().SetColor(COLOR_EYE_VEL);
		m_chartVel[i].Series(seriesIdx).GetXValues().SetOrder(loNone);

		//m_chartVel[i].Series(seriesIdx).GetAsPoint().GetPointer().GetPen().SetVisible(false);
		
		
		//mean velocity
		seriesIdx = m_chartVel[i].AddSeries(scPoint);
		m_chartVel[i].Series(seriesIdx).GetAsPoint().GetPointer().SetStyle(psCircle);
		m_chartVel[i].Series(seriesIdx).GetAsPoint().GetPointer().SetHorizontalSize(5);
		m_chartVel[i].Series(seriesIdx).GetAsPoint().GetPointer().SetVerticalSize(5);
		m_chartVel[i].Series(seriesIdx).GetAsPoint().GetPointer().GetBrush().SetColor(COLOR_EYE_MEAN_VEL);

		


		//cursor tool
		int toolIdx = m_chartVel[i].GetTools().Add(tcCursor);
		m_chartVel[i].GetTools().GetItems(toolIdx).SetActive(false);			
		m_chartVel[i].GetTools().GetItems(toolIdx).GetAsTeeCursor().SetFollowMouse(false);
		/*VARIANT varSeries; varSeries.vt=VT_DISPATCH;
		CSeries series = bufTChart[i]->Series(0);
		varSeries.pdispVal = series;
		bufTChart[i]->GetTools().GetItems(0).GetAsTeeCursor().SetSeries(varSeries);			*/
		m_chartVel[i].GetTools().GetItems(toolIdx).GetAsTeeCursor().SetStyle(cssBoth);
		m_chartVel[i].GetTools().GetItems(toolIdx).GetAsTeeCursor().GetPen().SetColor(COLOR_EYE_VEL_SELECTED);

		//nearest tool
		m_chartVel[i].GetTools().Add(tcNearest);
		m_chartVel[i].GetTools().GetItems(1).GetAsNearest().SetDrawLine(false);
		VARIANT varSeries; varSeries.vt=VT_DISPATCH;
		CSeries series = m_chartVel[i].Series(0);
		varSeries.pdispVal = series;
		m_chartVel[i].GetTools().GetItems(1).GetAsNearest().SetSeries(varSeries);
		//m_chartVel[i].GetTools().SetActive(false);
	
	}


	
}

void addData2Series(CSeries* pSeries, double* pData, unsigned long startIdx, unsigned long xZeroIndex, int count, unsigned long lastIdx, double freq);
/*
void CDlgAnalyzeHeadThrust::showResult()
{
	initTChart();

	//main chart (time series)
	int idx = m_chartMain.AddSeries(scLine);
	addData2Series(&(m_chartMain.Series(idx)), m_pdVelLeft, 0, 0, this->m_ulEyeDataCount, m_ulEyeDataCount, FRAMERATE);
	m_chartMain.Series(idx).SetTitle("Left Eye Velocity");
	idx = m_chartMain.AddSeries(scLine);
	addData2Series(&(m_chartMain.Series(idx)), m_pdVelRight, 0, 0, this->m_ulEyeDataCount, m_ulEyeDataCount, FRAMERATE);
	m_chartMain.Series(idx).SetTitle("Right Eye Velocity");
	idx = m_chartMain.AddSeries(scLine);
	addData2Series(&(m_chartMain.Series(idx)), this->m_pdVelHead, 0, 0, this->m_ulAccDataCount, m_ulEyeDataCount, FRAMERATE);
	m_chartMain.Series(idx).SetTitle("Head Velocity");


	structHeadThrust* pThrust = NULL;
	int count;
	int chartIdx;

	
	// init event series
	m_eventSeriesIdx = EU_EventFile::initEventSeries(&m_chartMain);




	//left eye
	count = int(this->m_analyzeHeadThrust[LEFT].m_listHeadThrust.GetCount());
	POSITION pos = m_analyzeHeadThrust[LEFT].m_listHeadThrust.GetHeadPosition();
	CString strTitle;
	int seriesIdx;
	for(int i=0; i<count; i++)
	{
		pThrust = m_analyzeHeadThrust[LEFT].m_listHeadThrust.GetNext(pos);
		if(pThrust && pThrust->valid)
		{
			chartIdx = (pThrust->headPeakVel > 0) ? 0 : 2;		//left eye positive or negative

			// time- velocity curve
			//series for eye velocity
			seriesIdx = m_chart[chartIdx].AddSeries(scLine);		//series 추가
			addData2Series(&(m_chart[chartIdx].Series(seriesIdx)), m_pdVelLeft, pThrust->headOnsetIdx-MS100, pThrust->headOnsetIdx, MS500, m_ulEyeDataCount, FRAMERATE/1000.);
			m_chart[chartIdx].Series(seriesIdx).SetShowInLegend(true);
			strTitle.Format("%d", int(seriesIdx/2));
			m_chart[chartIdx].Series(seriesIdx).SetTitle(strTitle);
			m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_EYE_VEL);

				
			//series for head angular velocity
			seriesIdx = m_chart[chartIdx].AddSeries(scLine);		//right eye positive or negative
			addData2Series(&(m_chart[chartIdx].Series(seriesIdx)), this->m_pdVelHead, pThrust->headOnsetIdx-MS100, pThrust->headOnsetIdx, MS500, m_ulAccDataCount, ACC_SAMPLERATE/1000.); 
			m_chart[chartIdx].Series(seriesIdx).SetShowInLegend(false);
			m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_HEAD_VEL);
			
			// head vel-eye vel curve
			m_chartVel[chartIdx].Series(0).AddXY(pThrust->headPeakVel, pThrust->eyePeakVel, NULL, COLOR_HEAD_VEL);
			
		}
	}
	//LEFT Means
	// left positive mean vel shape
	chartIdx = 0;
	//eye
	seriesIdx = m_chart[chartIdx].AddSeries(scLine);
	m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);	//두껍게
	m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_EYE_MEAN_VEL);
	for(int i=0; i<MS400; i++)
		m_chart[chartIdx].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, m_analyzeHeadThrust[LEFT].m_ppMeanEyeVel[0][i], NULL, COLOR_EYE_MEAN_VEL);
	//head
	seriesIdx = m_chart[chartIdx].AddSeries(scLine);
	m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);
	m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_HEAD_MEAN_VEL);
	for(int i=0; i<MS400; i++)
		m_chart[chartIdx].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, m_analyzeHeadThrust[LEFT].m_ppMeanHeadVel[0][i], NULL, COLOR_HEAD_MEAN_VEL);
	//head vel-eye vel point plot
	m_chartVel[chartIdx].Series(1).AddXY(m_analyzeHeadThrust[LEFT].m_structMeanHT[0].headPeakVel, 
											m_analyzeHeadThrust[LEFT].m_structMeanHT[0].eyePeakVel, NULL, RGB(255, 0, 0));
	
	
	// left negative mean vel shape
	chartIdx = 2;
	//eye
	seriesIdx = m_chart[chartIdx].AddSeries(scLine);
	m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);	//두껍게
	m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_EYE_MEAN_VEL);
	for(int i=0; i<MS400; i++)
		m_chart[chartIdx].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, m_analyzeHeadThrust[LEFT].m_ppMeanEyeVel[1][i], NULL, COLOR_EYE_MEAN_VEL);
	//head
	seriesIdx = m_chart[chartIdx].AddSeries(scLine);
	m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);
	m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_HEAD_MEAN_VEL);
	for(int i=0; i<MS400; i++)
		m_chart[chartIdx].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, m_analyzeHeadThrust[LEFT].m_ppMeanHeadVel[1][i], NULL, COLOR_HEAD_MEAN_VEL);

	//head vel-eye vel point plot
	m_chartVel[chartIdx].Series(1).AddXY(m_analyzeHeadThrust[LEFT].m_structMeanHT[1].headPeakVel, 
											m_analyzeHeadThrust[LEFT].m_structMeanHT[1].eyePeakVel, NULL, RGB(255, 0, 0));
	
	
	

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//right eye
	count = int(this->m_analyzeHeadThrust[RIGHT].m_listHeadThrust.GetCount());
	pos = m_analyzeHeadThrust[RIGHT].m_listHeadThrust.GetHeadPosition();
	for(int i=0; i<count; i++)
	{
		pThrust = m_analyzeHeadThrust[RIGHT].m_listHeadThrust.GetNext(pos);
		if(pThrust && pThrust->valid)
		{
			// time- velocity curve
			//chart for eye velocity
			chartIdx = (pThrust->headPeakVel > 0) ? 1 : 3;		//right eye positive or negative
			int seriesIdx = m_chart[chartIdx].AddSeries(scLine);
			addData2Series(&(m_chart[chartIdx].Series(seriesIdx)), m_pdVelRight, pThrust->headOnsetIdx-MS100, pThrust->headOnsetIdx, MS500, m_ulEyeDataCount, FRAMERATE/1000.);
			m_chart[chartIdx].Series(seriesIdx).SetShowInLegend(true);
			strTitle.Format("%d", int(seriesIdx/2));
			m_chart[chartIdx].Series(seriesIdx).SetTitle(strTitle);
			m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_EYE_VEL);
	
			//chart for head angular velocity
			seriesIdx = m_chart[chartIdx].AddSeries(scLine);
			addData2Series(&(m_chart[chartIdx].Series(seriesIdx)), this->m_pdVelHead, pThrust->headOnsetIdx-MS100, pThrust->headOnsetIdx, MS500, m_ulEyeDataCount, ACC_SAMPLERATE/1000.); 
			m_chart[chartIdx].Series(seriesIdx).SetShowInLegend(false);
			m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_HEAD_VEL);

			// head vel-eye vel curve
			m_chartVel[chartIdx].Series(0).AddXY(pThrust->headPeakVel, pThrust->eyePeakVel, NULL, RGB(255, 0, 0));

			
		}
	}
	

	//RIGT Means
	// right positive mean vel shape
	chartIdx = 1;
	//eye
	seriesIdx = m_chart[chartIdx].AddSeries(scLine);
	m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);	//두껍게
	m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_EYE_MEAN_VEL);
	for(int i=0; i<MS400; i++)
		m_chart[chartIdx].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, m_analyzeHeadThrust[RIGHT].m_ppMeanEyeVel[0][i], NULL, COLOR_EYE_MEAN_VEL);
	//head
	seriesIdx = m_chart[chartIdx].AddSeries(scLine);
	m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);
	m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_HEAD_MEAN_VEL);
	for(int i=0; i<MS400; i++)
		m_chart[chartIdx].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, m_analyzeHeadThrust[RIGHT].m_ppMeanHeadVel[0][i], NULL, COLOR_HEAD_MEAN_VEL);
	//head vel-eye vel point plot
	m_chartVel[chartIdx].Series(1).AddXY(m_analyzeHeadThrust[RIGHT].m_structMeanHT[0].headPeakVel, 
											m_analyzeHeadThrust[RIGHT].m_structMeanHT[0].eyePeakVel, NULL, RGB(255, 0, 0));
	
	
	// right negative mean vel shape
	chartIdx = 3;
	//eye
	seriesIdx = m_chart[chartIdx].AddSeries(scLine);
	m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);	//두껍게
	m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_EYE_MEAN_VEL);
	for(int i=0; i<MS400; i++)
		m_chart[chartIdx].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, m_analyzeHeadThrust[RIGHT].m_ppMeanEyeVel[1][i], NULL, COLOR_EYE_MEAN_VEL);
	//head
	seriesIdx = m_chart[chartIdx].AddSeries(scLine);
	m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);
	m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_HEAD_MEAN_VEL);
	for(int i=0; i<MS400; i++)
		m_chart[chartIdx].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, m_analyzeHeadThrust[RIGHT].m_ppMeanHeadVel[1][i], NULL, COLOR_HEAD_MEAN_VEL);

	//head vel-eye vel point plot
	m_chartVel[chartIdx].Series(1).AddXY(m_analyzeHeadThrust[RIGHT].m_structMeanHT[1].headPeakVel, 
											m_analyzeHeadThrust[RIGHT].m_structMeanHT[1].eyePeakVel, NULL, RGB(255, 0, 0));

	
	


	//list
	this->fillListDetail();
	this->fillListSummary();

	

	//list item select
	selectListDetail();

	
}
*/


void addData2Series(CSeries* pSeries, double* pData, unsigned long startIdx, unsigned long xZeroIndex, int count, unsigned long lastIdx, double freq)
{
	//series에 data를 startIdx에서부터 count개수만큼 더한다.
	// xZeroIndex : x 좌표가 0인 곳
	COleSafeArray XValues, YValues;
	DWORD num = count;
	XValues.Create(VT_R8, 1, &num);
	YValues.Create(VT_R8, 1, &num);

	int zeroOffset = xZeroIndex-startIdx;

	double t, val;
	for(long i=0; i<count; i++)
	{
		t = (i-zeroOffset)/freq;
		val = ((i<0) || (i>=lastIdx)) ? 0: pData[startIdx+i];
		XValues.PutElement(&i, &t);
		YValues.PutElement(&i, &val);
	}
	pSeries->AddArray(count, YValues, XValues);
}


int getMatchedHT(CList<structHeadThrust*, structHeadThrust*>*, CList<structHeadThrust*, structHeadThrust*>* , int* pMatched, int sign);
void CDlgAnalyzeHeadThrust::showResult2()
{
	initTChart();

	//main chart (time series)
	int idx = m_chartMain.AddSeries(scLine);
	addData2Series(&(m_chartMain.Series(idx)), m_pdVelLeft, 0, 0, this->m_ulEyeDataCount, m_ulEyeDataCount, FRAMERATE);
	m_chartMain.Series(idx).SetTitle("Left Eye Velocity");
	idx = m_chartMain.AddSeries(scLine);
	addData2Series(&(m_chartMain.Series(idx)), m_pdVelRight, 0, 0, this->m_ulEyeDataCount, m_ulEyeDataCount, FRAMERATE);
	m_chartMain.Series(idx).SetTitle("Right Eye Velocity");
	idx = m_chartMain.AddSeries(scLine);
	addData2Series(&(m_chartMain.Series(idx)), this->m_pdVelHead, 0, 0, this->m_ulAccDataCount, m_ulEyeDataCount, FRAMERATE);
	m_chartMain.Series(idx).SetTitle("Head Velocity");

	// init event series
	m_eventSeriesIdx = EU_EventFile::initEventSeries(&m_chartMain);
	m_chartMain.Series(m_eventSeriesIdx).SetTitle("Event");

	//event file 을 load한다.
	CString fname = m_strTestFile;
	fname.Replace(".emd", ".evt");
	EU_EventFile::loadEventFile(fname, &m_chartMain, this->m_eventSeriesIdx, false);



	//check number of left, right HT are same
	// 한쪽 눈만 검사한 경우라도, total count는 자이로 값으로 결정되므로, 같아야 한다. 
	if(this->m_analyzeHeadThrust[0].getTotalHTCount() != this->m_analyzeHeadThrust[1].getTotalHTCount())
		::AfxMessageBox("Number of Head Thrust between left and right data is different");
	
	int totalHTCount = m_analyzeHeadThrust[0].getTotalHTCount();
	int* pValid = new int[totalHTCount];

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// positive HT
	// 1.1 get positive HT which is valid in at least left or right
	int sign = 1;
	int count = getMatchedHT(&(m_analyzeHeadThrust[0].m_listHeadThrust), &(m_analyzeHeadThrust[1].m_listHeadThrust), pValid, sign);

	// 1.2. draw thcart
	// 1.2.1 chart
	// 1.2.1.1 left
	int chartIdx = 0;
	drawHTTimeVelocityCurve(&(m_analyzeHeadThrust[0]), this->m_pdVelLeft, chartIdx, pValid, sign);

	// 1.2.1.2 right
	chartIdx = 1;
	drawHTTimeVelocityCurve(&(m_analyzeHeadThrust[1]), this->m_pdVelRight, chartIdx, pValid, sign);

	// 1.2.2 fill list detail
	this->fillListDetail(&m_listHTUp, &(m_analyzeHeadThrust[0]), &(m_analyzeHeadThrust[1]),
					pValid, count, sign);



	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// negative HT
	//get positive HT which is valid in both left and right
	sign = -1;
	count = getMatchedHT(&(m_analyzeHeadThrust[0].m_listHeadThrust), &(m_analyzeHeadThrust[1].m_listHeadThrust), pValid, sign);

	/// 2.2. draw thcart
	// 2.2.1 chart
	// 2.2.1.1 left
	chartIdx = 2;
	drawHTTimeVelocityCurve(&(m_analyzeHeadThrust[0]), m_pdVelLeft, chartIdx, pValid, sign);

	// 2.2.1.2 right
	chartIdx = 3;
	drawHTTimeVelocityCurve(&(m_analyzeHeadThrust[1]), m_pdVelRight, chartIdx, pValid, sign);

	// 2.2.2 fill list detail
	fillListDetail(&m_listHTDown, &(m_analyzeHeadThrust[0]), &(m_analyzeHeadThrust[1]),
					pValid, count, sign);


	//////////////////////////////////////////////////////////////////////
	this->fillListSummary();
	
	//list item select
	selectListDetail();
}

int getMatchedHT(CList<structHeadThrust*, structHeadThrust*>* pList0, CList<structHeadThrust*, structHeadThrust*>* pList1, int* pMatched, int sign)
{
	ASSERT(pList0);
	ASSERT(pList1);
	ASSERT(pMatched);
	ASSERT(pList0->GetCount() == pList1->GetCount());


	if(pList0->GetCount() == 0)
	{
		::AfxMessageBox("There is no head thrust data");
	}

	int count = (int)pList0->GetCount();
	if(!count)
		return 0;

    memset(pMatched, 0, sizeof(int)*count);

	POSITION pos0, pos1;
	pos0 = pList0->GetHeadPosition();
	pos1 = pList1->GetHeadPosition();

	structHeadThrust* pHT0, *pHT1;

	int totalCount = 0;
	//둘 중 적어도 하나의 HT가 valid한지 확인
	for(int i=0; i<count; i++)
	{
		pHT0 = pList0->GetNext(pos0);
		pHT1 = pList1->GetNext(pos1);

		if(pHT0 && pHT1 &&  
			//(pHT0->headPeakIdx == pHT1->headPeakIdx) &&	//같은 HT인지 확인 
			(pHT0->valid || pHT1->valid) &&				//둘중 하나라도 valid한지
			pHT0->headPeakVel*sign > 0)					//sign 확인
		{
			pMatched[i] = 1;
			totalCount++;
		}
	}

	return totalCount;
}

void addDoubleArray(double* pSum, double* pAdd, int cout);
void CDlgAnalyzeHeadThrust::drawHTTimeVelocityCurve(CAnalyzeHeadThrust* pAnalyze, double* pEyeVel, int chartIdx, int* pValid, int sign)
{
	ASSERT(pAnalyze);
	ASSERT(pValid);

	CList<structHeadThrust*, structHeadThrust*>* pList = &(pAnalyze->m_listHeadThrust);
	ASSERT(pList);


	int count = (int)pList->GetCount();
	if(!count)
		return;

	structHeadThrust* pHT = NULL;
	POSITION pos = pList->GetHeadPosition();
	int seriesIdx = -1;
	CString strTitle;

	for(int i=0; i<count; i++)
	{
		pHT = pList->GetNext(pos);
		if(pHT && pValid[i])
		{

			//series for eye velocity
			seriesIdx = m_chart[chartIdx].AddSeries(scLine);		//series 추가

			//addData2Series(&(m_chart[chartIdx].Series(seriesIdx)), m_pdVelLeft, pHT->eyeOnsetIdx-MS100, pHT->eyeOnsetIdx, MS500, m_ulEyeDataCount, FRAMERATE/1000.);
			addData2Series(&(m_chart[chartIdx].Series(seriesIdx)), pEyeVel, pHT->headOnsetIdx-MS100, pHT->headOnsetIdx, MS500, m_ulEyeDataCount, FRAMERATE/1000.);
			m_chart[chartIdx].Series(seriesIdx).SetShowInLegend(true);
			strTitle.Format("%d", int(seriesIdx/2));
			m_chart[chartIdx].Series(seriesIdx).SetTitle(strTitle);
			m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_EYE_VEL);
			//valid하지 않은 HT일 경우 보이지 않게 한다.
			if(!pHT->valid)
			{
				m_chart[chartIdx].Series(seriesIdx).SetActive(false);
				m_chart[chartIdx].Series(seriesIdx).SetShowInLegend(false);
			}
				
			//series for head angular velocity
			seriesIdx = m_chart[chartIdx].AddSeries(scLine);		
			addData2Series(&(m_chart[chartIdx].Series(seriesIdx)), this->m_pdVelHead, pHT->headOnsetIdx-MS100, pHT->headOnsetIdx, MS500, m_ulAccDataCount, ACC_SAMPLERATE/1000.); 
			m_chart[chartIdx].Series(seriesIdx).SetShowInLegend(false);
			m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_HEAD_VEL);

			if(!pHT->valid)
			{
				m_chart[chartIdx].Series(seriesIdx).SetActive(false);
				m_chart[chartIdx].Series(seriesIdx).SetShowInLegend(false);
			}


			// head vel-eye vel curve
			if(pHT->valid)
				m_chartVel[chartIdx].Series(0).AddXY(pHT->headPeakVel, pHT->eyePeakVel, NULL, COLOR_HEAD_VEL);
			else
			{
				m_chartVel[chartIdx].Series(0).AddNullXY(0, 0, NULL);

			}
		}
	}

	//draw mean 
	//eye
	seriesIdx = m_chart[chartIdx].AddSeries(scLine);
	m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);	//두껍게
	m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_EYE_MEAN_VEL);
	int signIdx = sign>0 ? 0 : 1;
	for(int i=0; i<MS400; i++)
		m_chart[chartIdx].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, pAnalyze->m_ppMeanEyeVel[signIdx][i], NULL, COLOR_EYE_MEAN_VEL);
	//head
	seriesIdx = m_chart[chartIdx].AddSeries(scLine);
	m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);
	m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_HEAD_MEAN_VEL);
	for(int i=0; i<MS400; i++)
		m_chart[chartIdx].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, pAnalyze->m_ppMeanHeadVel[signIdx][i], NULL, COLOR_HEAD_MEAN_VEL);
	//head vel-eye vel point plot
	m_chartVel[chartIdx].Series(1).AddXY(pAnalyze->m_structMeanHT[0].headPeakVel, 
											pAnalyze->m_structMeanHT[0].eyePeakVel, NULL, RGB(255, 0, 0));
	
	
}


void CDlgAnalyzeHeadThrust::fillListDetail(CListCtrl* pListCtrl, 
						CAnalyzeHeadThrust* pAnalyzeLeft, 
						CAnalyzeHeadThrust* pAnalyzeRight,
                        int* pValid, int validCount, int sign)
{
	ASSERT(pListCtrl);
	ASSERT(pAnalyzeLeft);
	ASSERT(pAnalyzeRight);
	ASSERT(pValid);

	CList<structHeadThrust*, structHeadThrust*>* pList0 = &(pAnalyzeLeft->m_listHeadThrust);
	CList<structHeadThrust*, structHeadThrust*>* pList1 = &(pAnalyzeRight->m_listHeadThrust);;

	int count = (int)pList0->GetCount();
	if(!count)
		return;

	pListCtrl->DeleteAllItems();


	//mean value부터 넣는다.
	int pnIdx = (sign == 1) ? 0 : 1;		//positive가 0, negative가 1 index	
	double meanVal[] = {m_analyzeHeadThrust[0].m_structMeanHT[pnIdx].headPeakVel, 	//head peak acc
						m_analyzeHeadThrust[0].m_structMeanHT[pnIdx].eyePeakVel,		//left eye acc
						m_analyzeHeadThrust[1].m_structMeanHT[pnIdx].eyePeakVel,			//right eye acc
						m_analyzeHeadThrust[0].m_structMeanHT[pnIdx].eyePeakVel/m_analyzeHeadThrust[0].m_structMeanHT[pnIdx].headPeakVel, //left gain
						m_analyzeHeadThrust[1].m_structMeanHT[pnIdx].eyePeakVel/m_analyzeHeadThrust[0].m_structMeanHT[pnIdx].headPeakVel}; //right gain
	CString strTitle = "Mean";
	corAdd2List(pListCtrl, strTitle, meanVal, 5);
	setListCheckbox(pListCtrl, 0, TRUE);




	//뒤에서부터 검색한다.
	POSITION pos0 = pList0->GetTailPosition();
	POSITION pos1 = pList1->GetTailPosition();

	structHeadThrust *pHT0, *pHT1;
	CString strCount;

	for(int i=0; i<count; i++)
	{
		pHT0 = pList0->GetPrev(pos0);
		pHT1 = pList1->GetPrev(pos1);

		if(pHT0 && pHT1 && pValid[count-i-1])		//matched된 것이라면
		{
			double val[] = {pHT0->headPeakVel, 
							pHT0->eyePeakVel ? pHT0->eyePeakVel : -1.1111111, 
							pHT1->eyePeakVel ? pHT1->eyePeakVel : -1.1111111, 
							pHT0->eyePeakVel ? pHT0->eyePeakVel/pHT0->headPeakVel : -1.1111111, 
							pHT1->eyePeakVel ? pHT1->eyePeakVel/pHT1->headPeakVel : -1.1111111};
			
			strCount.Format("%d", validCount--);
			corAdd2List(pListCtrl, strCount, val, 5);

			
			setListCheckbox(pListCtrl, 0, TRUE);			//checkbox set
			pListCtrl->SetItemData(0, (DWORD_PTR)pHT0);		//Ht point를 저장해 놓아서 나중에 찾을 수 있게 한다.
		}
	}


}


void addDoubleArray(double* pSum, double* pAdd, int count)
{
	ASSERT(pSum);
	ASSERT(pAdd);

	for(int i=0; i<count; i++)
		pSum[i] += pAdd[i];
}

BEGIN_EVENTSINK_MAP(CDlgAnalyzeHeadThrust, CDialog)
ON_EVENT(CDlgAnalyzeHeadThrust, IDC_TCHART_HT_LEFT_UP_VEL, 3, OnClickTchartHtVelocityChart, VTS_NONE)
ON_EVENT(CDlgAnalyzeHeadThrust, IDC_TCHART_HT_RIGHT_UP_VEL, 3, OnClickTchartHtVelocityChart, VTS_NONE)
ON_EVENT(CDlgAnalyzeHeadThrust, IDC_TCHART_HT_LEFT_DOWN_VEL, 3, OnClickTchartHtVelocityChart, VTS_NONE)
ON_EVENT(CDlgAnalyzeHeadThrust, IDC_TCHART_HT_RIGHT_DOWN_VEL, 3, OnClickTchartHtVelocityChart, VTS_NONE)
ON_EVENT(CDlgAnalyzeHeadThrust, IDC_TCHART_HEAD_THRUST_MAIN, 24, OnScrollTchartHeadThrustMain, VTS_NONE)
ON_EVENT(CDlgAnalyzeHeadThrust, IDC_TCHART_HEAD_THRUST_MAIN, 27, OnZoomTchartHeadThrustMain, VTS_NONE)
ON_EVENT(CDlgAnalyzeHeadThrust, IDC_TCHART_HEAD_THRUST_MAIN, 26, OnUndoZoomTchartHeadThrustMain, VTS_NONE)
//ON_EVENT(CDlgAnalyzeHeadThrust, IDC_TCHART_HEAD_THRUST_MAIN, 29, OnBeforeDrawSeriesTchartHeadThrustMain, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgAnalyzeHeadThrust::OnBnClickedButtonHtReanalyze()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	analyzeThis();

	showResult2();
	
}



void CDlgAnalyzeHeadThrust::initListCtrls()
{
	//setup list summary
	CRect rect;
	this->m_listHTSummary.GetClientRect(&rect);
	int w = rect.Width();

	DWORD dwStyle = m_listHTSummary.GetExStyle();
	dwStyle = ( LVS_EX_GRIDLINES);
	m_listHTSummary.SetExtendedStyle(dwStyle);

	char* listD[5] = {" ", "Left(+)", "Left(-)", "Right(+)", "Right(-)"};
	double widthD[5] = {.4, .15, .15, .15, .15};
	for(int j=0; j<5; j++)
		m_listHTSummary.InsertColumn(j, listD[j], LVCFMT_CENTER, int(widthD[j]*w));


	//setup list detail
	CListCtrl* pList[] = {&m_listHTUp, &m_listHTDown};

	for(int i=0; i<2; i++)
	{
		pList[i]->GetClientRect(&rect);
		w = rect.Width();
		DWORD dwStyle = pList[i]->GetExStyle();
		dwStyle = (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
		pList[i]->SetExtendedStyle(dwStyle);


		char* listD[6] = {"Visible", "Head Acc(°/s2)", "L-Eye Acc(°/s2)", "R-Eye Acc(°/s2)", "L-Gain", "R-Gain"};
		double widthD[6] = {.1, .9/5, .9/5, .9/5, .9/5, .9/5};
		for(int j=0; j<6; j++)
			pList[i]->InsertColumn(j, listD[j], LVCFMT_CENTER, int(widthD[j]*w));
	}
}


/*
void CDlgAnalyzeHeadThrust::fillListDetail()
{
	m_listHTUp.DeleteAllItems();
	m_listHTDown.DeleteAllItems();

	CListCtrl* pList[] = {&m_listHTUp, &m_listHTDown};

	//left와 right의 HT 개수가 같은지 확인. 같은 Head movement에서 찾았으므로 같아야 한다.
	if(m_analyzeHeadThrust[0].getValidHTCount(0) != m_analyzeHeadThrust[1].getValidHTCount(0))
	{
		::AfxMessageBox("Number of Left HT is not equal to Right");
		return;
	}
	
	//HT 개수를 구한다.
	int count = (int)max(m_analyzeHeadThrust[0].m_listHeadThrust.GetCount(), m_analyzeHeadThrust[1].m_listHeadThrust.GetCount());
	structHeadThrust* ppHT[2];
	POSITION pos[2];

	//tail position. 마지막에 넣는 것이 맨 위다.
	pos[0] = m_analyzeHeadThrust[0].m_listHeadThrust.GetTailPosition();
	pos[1] = m_analyzeHeadThrust[1].m_listHeadThrust.GetTailPosition();

	//positive와 negative 개수를 구한다. 좌우의 개수가 같은지도 확인한다.
	int posCount = m_analyzeHeadThrust[0].getValidHTCount(1);
	if(posCount != m_analyzeHeadThrust[1].getValidHTCount(1))
		::AfxMessageBox("Number of Left positive HT is not equal to Right");
	int negCount = m_analyzeHeadThrust[0].getValidHTCount(-1);
	if(negCount != m_analyzeHeadThrust[1].getValidHTCount(-11))
		::AfxMessageBox("Number of Left positive HT is not equal to Right");


	CString strCount;
	for(int i=0; i<count; i++)
	{
		ppHT[0] = m_analyzeHeadThrust[0].m_listHeadThrust.GetPrev(pos[0]);
		ppHT[1] = m_analyzeHeadThrust[1].m_listHeadThrust.GetPrev(pos[1]);

		//둘다 valid할 때만
		if(ppHT[0] && ppHT[1] && ppHT[0]->valid && ppHT[1]->valid)
		{
			//같은 HT인지 확인한다.
			if(ppHT[0]->headPeakVel != ppHT[1]->headPeakVel)
			{
				::AfxMessageBox("Velocity of Left HT is not equal to Right");
				return;
			}
			double val[] = {ppHT[0]->headPeakVel, 
								ppHT[0]->eyePeakVel, ppHT[1]->eyePeakVel, 
								ppHT[0]->eyePeakVel/ppHT[0]->headPeakVel, ppHT[1]->eyePeakVel/ppHT[1]->headPeakVel};
			
			if(ppHT[0]->headPeakVel>0)
			{
				strCount.Format("%d", posCount--);
				corAdd2List(&this->m_listHTUp, strCount, val, 5);
				setListCheckbox(&m_listHTUp, 0, TRUE);
			}
			else
			{
				strCount.Format("%d", negCount--);
				corAdd2List(&this->m_listHTDown, strCount, val, 5);
				setListCheckbox(&m_listHTDown, 0, TRUE);
			}
		}
	}
	
}
*/

void CDlgAnalyzeHeadThrust::fillListSummary()
{
	m_listHTSummary.DeleteAllItems();

	//LP, LN, RP, RN 순서

	double headVel[4] = {m_analyzeHeadThrust[0].m_structMeanHT[0].headPeakVel,	//left positive
						m_analyzeHeadThrust[0].m_structMeanHT[1].headPeakVel,	//elft negative
						m_analyzeHeadThrust[1].m_structMeanHT[0].headPeakVel,	//right positive
						m_analyzeHeadThrust[1].m_structMeanHT[1].headPeakVel};	//left negative
	double eyeVel[4] = {m_analyzeHeadThrust[0].m_structMeanHT[0].eyePeakVel,	//left positive
						m_analyzeHeadThrust[0].m_structMeanHT[1].eyePeakVel,	//elft negative
						m_analyzeHeadThrust[1].m_structMeanHT[0].eyePeakVel,	//right positive
						m_analyzeHeadThrust[1].m_structMeanHT[1].eyePeakVel};	//left negative
	double gain[4] = {0, 0, 0, 0};
	for(int i=0; i<4; i++)
		gain[i] = headVel[i] ? eyeVel[i]/headVel[i] : 0;
	double latency[4] = {0, 0, 0, 0};

	corAdd2List(&m_listHTSummary, "Latency(ms)", latency, 4);
	corAdd2List(&m_listHTSummary, "Gain", gain, 4);
	corAdd2List(&m_listHTSummary, "Eye Acceleration(°/sec2)", eyeVel, 4);
	corAdd2List(&m_listHTSummary, "Head Acceleration(°/sec2)", headVel, 4);
	
	/*
	int htCount[4] = {0,0,0,0};

	int count = max(m_analyzeHeadThrust[0].m_listHeadThrust.GetCount(), m_analyzeHeadThrust[1].m_listHeadThrust.GetCount());

	POSITION pos[2] = {m_analyzeHeadThrust[0].m_listHeadThrust.GetHeadPosition(), m_analyzeHeadThrust[1].m_listHeadThrust.GetHeadPosition()};
	structHeadThrust* ppHT[2];

	for(int i=0; i<count; i++)
	{
		ppHT[0] = this->m_analyzeHeadThrust[0].m_listHeadThrust.GetNext(pos[0]);
		ppHT[1] = this->m_analyzeHeadThrust[1].m_listHeadThrust.GetNext(pos[1]);



		//positive인지 negative인지에 따라 각각 계산한다.
		if(ppHT[0] && ppHT[1])
		{
			//index 0, 2는 positive, 1, 3은 negative
			int sign = ppHT[0]->headPeakVel>0 ? 0 : 1;
			
			headVel[0+sign] += ppHT[0]->headPeakVel;
			headVel[2+sign] += ppHT[1]->headPeakVel;
			eyeVel[0+sign] += ppHT[0]->eyePeakVel;
			eyeVel[2+sign] += ppHT[1]->eyePeakVel;
			gain[0+sign] = ppHT[0]->eyePeakVel/ppHT[0]->headPeakVel;
			gain[2+sign] = ppHT[1]->eyePeakVel/ppHT[1]->headPeakVel;
			latency[0+sign] = double(ppHT[0]->eyeOnsetIdx - ppHT[0]->headOnsetIdx)*1000./FRAMERATE;
			latency[2+sign] = double(ppHT[1]->eyeOnsetIdx - ppHT[1]->headOnsetIdx)*1000./FRAMERATE;

			htCount[0 + sign]++;
			htCount[2 + sign]++;
		}
	}
	
	for(int i=0; i<4; i++)
	{
		if(htCount[i])
		{
			headVel[i] /= htCount[i];
			eyeVel[i] /= htCount[i];
			gain[i] /= htCount[i];
			latency[i] /= htCount[i];
		}
	}

	corAdd2List(&m_listHTSummary, "Latency(ms)", latency, 4);
	corAdd2List(&m_listHTSummary, "Gain", gain, 4);
	corAdd2List(&m_listHTSummary, "Eye Velocity(°/s)", eyeVel, 4);
	corAdd2List(&m_listHTSummary, "Head Velocity(°/s)", headVel, 4);*/

}

void CDlgAnalyzeHeadThrust::selectListDetail()
{
	//선택되어진 것이 있다면
	if(m_nSelectedHT)
	{
		//up, down list ctrl을 선택한다.
		CListCtrl* pList = (this->m_nSelectedHT > 0) ? &(this->m_listHTUp) : &m_listHTDown;
		//item index를 구한다.
		int nIdx = abs(m_nSelectedHT)-1;

		//기존 선택되었던 item의 선택을 해제한다.
		pList->SetFocus();
		pList->SetItemState(nIdx,  0,  LVIS_FOCUSED | LVIS_SELECTED);

		// index가 listctrl의 item 개수보다 작아야 한다.
		while(pList->GetItemCount() <= nIdx)
			nIdx--;
		//index가 유효하다면 선택한다.
		if(nIdx>=0)
		{
			
			//선택한다.
			pList->SetFocus();
			pList->SetItemState(nIdx+1,  LVIS_FOCUSED | LVIS_SELECTED,  LVIS_FOCUSED | LVIS_SELECTED);			
			pList->SetItemState(nIdx+1, 0,  LVIS_FOCUSED | LVIS_SELECTED);	
			pList->SetItemState(nIdx,  LVIS_FOCUSED | LVIS_SELECTED,  LVIS_FOCUSED | LVIS_SELECTED);			
		}

	}
}
BOOL CDlgAnalyzeHeadThrust::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_ESCAPE) || (pMsg->wParam == VK_RETURN))
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return true;
		}
		else if(pMsg->wParam == 46) //DEL
		{
			deleteHT2(this->m_nSelectedHT);
		}
			
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgAnalyzeHeadThrust::OnLvnItemchangedListHtUp(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if(pNMLV->uChanged & LVIF_STATE) // item state has been changed
	{
		switch(pNMLV->uNewState & 0x3000)	//checkbox click
		{
		case 0x2000: // new state: checked
			//TRACE1("\n Item %d has been checked", pNMLV->iItem);
			this->showHT(1, pNMLV->iItem, true); 
			break;
		case 0x1000: // new state: unchecked
			//TRACE1("\n Item %d has been unchecked", pNMLV->iItem);
			this->showHT(1, pNMLV->iItem, false);
			break;
		}

		if(pNMLV->uNewState == 3)		// 새롭게 선택되었다면
		{
			//Down에서 선택된 것이 있다면 List에서 해제한다.
			if(m_nSelectedHT<0)
				m_listHTDown.SetItemState(-1*m_nSelectedHT - 1, 0, LVIS_FOCUSED | LVIS_SELECTED);
			
	 
			this->selectHT2(pNMLV->iItem+1);
			
		}
   }
   *pResult = 0;

	/*
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
    long lRow = 0;                                  // 로우
    int nCol = 0;                                   // 컬럼

	//새롭게 선택된것
	if(temp->uNewState == 3)
	{
		//Down에서 선택된 것이 있다면 List에서 해제한다.
		if(m_nSelectedHT<0)
			m_listHTDown.SetItemState(-1*m_nSelectedHT - 1, 0, LVIS_FOCUSED | LVIS_SELECTED);
		
 
		//선택된 row를 찾는다.
		lRow = temp->iItem;                             // 로우값(클릭한 로우리턴)
		nCol = temp->iSubItem;                          // 컬럼값(클릭한 컬럼리턴 : 님 경우 별 쓸모없겠지만..)

		selectHT(lRow+1);

		//this->selectSaccadeOnChart(lRow);
	}*/
}

void CDlgAnalyzeHeadThrust::OnLvnItemchangedListHtDown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if(pNMLV->uChanged & LVIF_STATE) // item state has been changed
	{
		switch(pNMLV->uNewState & 0x3000)		//checkbox click
		{
		case 0x2000: // new state: checked
			TRACE1("\n Item %d has been checked", pNMLV->iItem);
			this->showHT(-1, pNMLV->iItem, true); 
			break;
		case 0x1000: // new state: unchecked
			TRACE1("\n Item %d has been unchecked", pNMLV->iItem);
			this->showHT(-1, pNMLV->iItem, false);
			break;
		}

		if(pNMLV->uNewState == 3)		// 새롭게 선택되었다면
		{
			//Up에서 선택된 것이 있다면 List에서 해제한다.
			if(m_nSelectedHT>0)
				m_listHTUp.SetItemState(m_nSelectedHT - 1, 0, LVIS_FOCUSED | LVIS_SELECTED);
			
	 
			// vel-vel 커브에 표시한다.
			this->selectHT2(-1*(pNMLV->iItem+1));
		}
   }
   *pResult = 0;


	/*LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
    long lRow = 0;                                  // 로우
    int nCol = 0;                                   // 컬럼

	//새롭게 선택된것
	if(temp->uNewState == 3)
	{
		//Up에서 선택된 것이 있다면 List에서 해제한다.
		if(m_nSelectedHT>0)
			m_listHTUp.SetItemState(m_nSelectedHT - 1, 0, LVIS_FOCUSED | LVIS_SELECTED);
		

		lRow = temp->iItem;                             // 로우값(클릭한 로우리턴)
		nCol = temp->iSubItem;                          // 컬럼값(클릭한 컬럼리턴 : 님 경우 별 쓸모없겠지만..)

		
		selectHT(-1*(lRow+1));

		//this->selectSaccadeOnChart(lRow);
	}
	*pResult = 0;*/
}

void CDlgAnalyzeHeadThrust::showHT(int sign, int idxHT, bool bVisible)
{
	// positive HT면 chart 0,1 번, minus면 2,3번
	int chartIdx = sign>0 ? 0 : 2;

	// time plot에서 
	int seriesIdx = idxHT*2;
	//마지막 두개는 mean plot이다.
	if(seriesIdx < m_chart[chartIdx].GetSeriesCount())
	{
        //left eye
		m_chart[chartIdx].Series(seriesIdx).SetActive(bVisible);
		m_chart[chartIdx].Series(seriesIdx+1).SetActive(bVisible);

		//right ee
		m_chart[chartIdx+1].Series(seriesIdx).SetActive(bVisible);
		m_chart[chartIdx+1].Series(seriesIdx+1).SetActive(bVisible);
	}

}

void CDlgAnalyzeHeadThrust::selectHT(int selectedHT)
{
	//기존 선택된 것을 원래 컬러로 
	if(m_nSelectedHT)
	{
		//기존 선택 series를 원래 색으로 바꾼다.
		int chartIdx = m_nSelectedHT > 0 ? 0 : 2;		//plus면 chart 0,1번 minum면 2,3번
		CListCtrl* pList = chartIdx<2 ? &m_listHTUp : &m_listHTDown;
		// seriesIdx는 0-based index. selectedHT는 1-based index (0은 선택 안한상태)
		// eye, head plot 2개씩 그려지므로 *2
		int seriesIdx = (abs(m_nSelectedHT)-1)*2;					
		if(seriesIdx < m_chart[chartIdx].GetSeriesCount()-2)	//mean plot이 두개 있으므로
		{
			//list에서 이전에 선택되었던 ROW 선택 해제
			pList->SetItemState(abs(m_nSelectedHT)-1, 0,  LVIS_FOCUSED | LVIS_SELECTED);
		

			/// time-velocity plot
			//left eye
			m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_EYE_VEL);
			m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(1);
			m_chart[chartIdx].Series(seriesIdx+1).SetColor(COLOR_HEAD_VEL);
			m_chart[chartIdx].Series(seriesIdx+1).GetPen().SetWidth(1);

			//right eye
			m_chart[chartIdx+1].Series(seriesIdx).SetColor(COLOR_EYE_VEL);
			m_chart[chartIdx+1].Series(seriesIdx).GetPen().SetWidth(1);
			m_chart[chartIdx+1].Series(seriesIdx+1).SetColor(COLOR_HEAD_VEL);
			m_chart[chartIdx+1].Series(seriesIdx+1).GetPen().SetWidth(1);

			// head vel-eye vel plot
			m_chartVel[chartIdx].GetTools().GetItems(0).SetActive(false);
			m_chartVel[chartIdx+1].GetTools().GetItems(0).SetActive(false);	


		}
	}

	//선택된 것을 선택 컬러로
	this->m_nSelectedHT = selectedHT;
	if(m_nSelectedHT)
	{
		//기존 선택 series를 원래 색으로 바꾼다.
		int chartIdx = m_nSelectedHT > 0 ? 0 : 2;		//plus면 chart 0,1번 minum면 2,3번
		// seriesIdx는 0-based index. selectedHT는 1-based index (0은 선택 안한상태)
		// eye, head plot 2개씩 그려지므로 *2
		int seriesIdx = (abs(m_nSelectedHT)-1)*2;					
		if(seriesIdx < m_chart[chartIdx].GetSeriesCount()-1)	//mean plot이 있으므로
		{
			//left eye
			m_chart[chartIdx].Series(seriesIdx).SetColor(COLOR_EYE_VEL_SELECTED);
			m_chart[chartIdx].Series(seriesIdx).GetPen().SetWidth(2);
			m_chart[chartIdx].Series(seriesIdx+1).SetColor(COLOR_HEAD_VEL_SELECTED);
			m_chart[chartIdx].Series(seriesIdx+1).GetPen().SetWidth(2);

			//right eye
			m_chart[chartIdx+1].Series(seriesIdx).SetColor(COLOR_EYE_VEL_SELECTED);
			m_chart[chartIdx+1].Series(seriesIdx).GetPen().SetWidth(2);
			m_chart[chartIdx+1].Series(seriesIdx+1).SetColor(COLOR_HEAD_VEL_SELECTED);
			m_chart[chartIdx+1].Series(seriesIdx+1).GetPen().SetWidth(2);

			// head vel-eye vel plot
			double x,y;
			m_chartVel[chartIdx].GetTools().GetItems(0).SetActive(true);
			x = m_chartVel[chartIdx].Series(0).GetXValues().GetValue(abs(m_nSelectedHT)-1);
			y = m_chartVel[chartIdx].Series(0).GetYValues().GetValue(abs(m_nSelectedHT)-1);
			m_chartVel[chartIdx].GetTools().GetItems(0).GetAsTeeCursor().SetXVal(x);
			m_chartVel[chartIdx].GetTools().GetItems(0).GetAsTeeCursor().SetYVal(y);
			
			m_chartVel[chartIdx+1].GetTools().GetItems(0).SetActive(true);
			x = m_chartVel[chartIdx+1].Series(0).GetXValues().GetValue(abs(m_nSelectedHT)-1);
			y = m_chartVel[chartIdx+1].Series(0).GetYValues().GetValue(abs(m_nSelectedHT)-1);
			m_chartVel[chartIdx+1].GetTools().GetItems(0).GetAsTeeCursor().SetXVal(x);
			m_chartVel[chartIdx+1].GetTools().GetItems(0).GetAsTeeCursor().SetYVal(y);
		}

	}

}


void CDlgAnalyzeHeadThrust::selectHT2(int selectedHT)
{
	//기존 선택된 것을 원래 컬러로 
	if(m_nSelectedHT)
	{
		//기존 선택 series를 원래 색으로 바꾼다.
		int chartIdx = m_nSelectedHT > 0 ? 0 : 2;		//plus면 chart 0,1번 minum면 2,3번
		CListCtrl* pList = m_nSelectedHT > 0  ? &m_listHTUp : &m_listHTDown;
		//list에서 이전에 선택되었던 ROW 선택 해제
		pList->SetItemState(abs(m_nSelectedHT)-1, 0,  LVIS_FOCUSED | LVIS_SELECTED);
		
		// head vel-eye vel plot
		m_chartVel[chartIdx].GetTools().GetItems(0).SetActive(false);
		m_chartVel[chartIdx+1].GetTools().GetItems(0).SetActive(false);	
	}

	//선택된 것을 선택 컬러로
	this->m_nSelectedHT = selectedHT;
	if(m_nSelectedHT)
	{
		//기존 선택 series를 원래 색으로 바꾼다.
		int chartIdx = m_nSelectedHT > 0 ? 0 : 2;		//plus면 chart 0,1번 minum면 2,3번
		
		// head vel-eye vel plot
		double x,y;
		//left eye
		m_chartVel[chartIdx].GetTools().GetItems(0).SetActive(true);
		x = m_chartVel[chartIdx].Series(0).GetXValues().GetValue(abs(m_nSelectedHT)-1);
		y = m_chartVel[chartIdx].Series(0).GetYValues().GetValue(abs(m_nSelectedHT)-1);
		m_chartVel[chartIdx].GetTools().GetItems(0).GetAsTeeCursor().SetXVal(x);
		m_chartVel[chartIdx].GetTools().GetItems(0).GetAsTeeCursor().SetYVal(y);
		
		//right eye
		m_chartVel[chartIdx+1].GetTools().GetItems(0).SetActive(true);
		x = m_chartVel[chartIdx+1].Series(0).GetXValues().GetValue(abs(m_nSelectedHT)-1);
		y = m_chartVel[chartIdx+1].Series(0).GetYValues().GetValue(abs(m_nSelectedHT)-1);
		m_chartVel[chartIdx+1].GetTools().GetItems(0).GetAsTeeCursor().SetXVal(x);
		m_chartVel[chartIdx+1].GetTools().GetItems(0).GetAsTeeCursor().SetYVal(y);

	}
}

void CDlgAnalyzeHeadThrust::OnClickTchartHtVelocityChart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int chartIdx = -1;
	CPoint pt;
	::GetCursorPos(&pt);

	CRect rect;
	for(int i=0; i<4; i++)
	{
		m_chartVel[i].GetWindowRect(&rect);
		if(rect.PtInRect(pt))
			chartIdx = i;
	}

	// 0-based에서 1-based로 바꾸고 down 방향 차트에서 selected 되었다면 따라 -1을 곱해준다.
	long idx = m_chartVel[chartIdx].GetTools().GetItems(1).GetAsNearest().GetPoint();
	
	//int selected = (chartIdx<2 ? 1 : -1)*(idx+1);
	//selectHT(selected);

	//list를 선택해 준다.
	CListCtrl* pList = chartIdx<2 ? &m_listHTUp : &m_listHTDown;
	pList->SetFocus();
	pList->SetItemState(idx,  LVIS_FOCUSED | LVIS_SELECTED,  LVIS_FOCUSED | LVIS_SELECTED);
}

void CDlgAnalyzeHeadThrust::deleteHT2(int idxHT)
{
	//idxHT는 CCtrlList에 몇번째 열인지를 나타내고 있다.
	if(!idxHT)
		return;

	// delete 시 해야 할 일
	// 0. listctrl에서 list의 몇번째 item인지 확인
	// 1. list에서 valid를 false로 
	// 2. chart에서 제거
	// 3. listctrl에서 제거
	// 4. mean update (clistctrl)
	// 5. mean update (series)

	// 0. listctrl에서 list의 몇번째 item인지 확인하자.
	CListCtrl* pListCtrl = (idxHT>0) ? (&m_listHTUp) : (&m_listHTDown);
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	int nItem = pListCtrl->GetNextSelectedItem(pos);			//row를 구한다.
	if(nItem == -1) return;

	structHeadThrust* pHT = (structHeadThrust*)(pListCtrl->GetItemData(nItem));
	if(!pHT) return;
	int nListPos = this->m_analyzeHeadThrust[0].findHTLocation(pHT);

	if(nListPos == -1)
		return;

	// 1. list에서 item을 제거
	this->m_analyzeHeadThrust[0].deleteHT(nListPos);
	this->m_analyzeHeadThrust[1].deleteHT(nListPos);

	// 2. chart에서 제거
	int chartIdx = idxHT > 0 ? 0 : 2;		//plus면 chart 0,1번 minum면 2,3번
	//idxHT*2, idxHT*2+1번째 series를 제거한다.
	int seriesIdx = nItem*2;
	
	//left (eye, head)
	// seriesIdx+1 부터 제거해야 한다.
	m_chart[chartIdx].RemoveSeries(seriesIdx+1);
	m_chart[chartIdx].RemoveSeries(seriesIdx);		
	m_chartVel[chartIdx].Series(0).Delete(abs(idxHT)-1);
	
	m_chart[chartIdx+1].RemoveSeries(seriesIdx+1);
	m_chart[chartIdx+1].RemoveSeries(seriesIdx);
	m_chartVel[chartIdx+1].Series(0).Delete(abs(idxHT)-1);


	// 3. listctrl에서 제거
	pListCtrl->DeleteItem(nItem);

	// 4. mean update (clistctrl)
	//mean value부터 넣는다.
	int pnIdx = idxHT > 0 ? 0 : 1;		//positive가 0, negative가 1 index	
	double meanVal[] = {m_analyzeHeadThrust[0].m_structMeanHT[pnIdx].headPeakVel, 	//head peak acc
						m_analyzeHeadThrust[0].m_structMeanHT[pnIdx].eyePeakVel,		//left eye acc
						m_analyzeHeadThrust[1].m_structMeanHT[pnIdx].eyePeakVel,			//right eye acc
						m_analyzeHeadThrust[0].m_structMeanHT[pnIdx].eyePeakVel/m_analyzeHeadThrust[0].m_structMeanHT[pnIdx].headPeakVel, //left gain
						m_analyzeHeadThrust[1].m_structMeanHT[pnIdx].eyePeakVel/m_analyzeHeadThrust[0].m_structMeanHT[pnIdx].headPeakVel}; //right gain
	int itemIdx = pListCtrl->GetItemCount();
	corChangeListValue(pListCtrl, itemIdx-1, meanVal, 5);

	// 5. mean update (series)
	for(int LR=0; LR<2; LR++)
	{
		//마지막에서 두번째 것이 mean eye
		seriesIdx = m_chart[chartIdx + LR].GetSeriesCount()-2;	//
		m_chart[chartIdx+LR].Series(seriesIdx).Clear();
		int signIdx = idxHT>0 ? 0 : 1;
		for(int i=0; i<MS400; i++)
			m_chart[chartIdx + LR].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, m_analyzeHeadThrust[LR].m_ppMeanEyeVel[signIdx][i], NULL, COLOR_EYE_MEAN_VEL);
		//마지막 series가 mean head
		seriesIdx = m_chart[chartIdx + LR].GetSeriesCount()-1;
		m_chart[chartIdx+LR].Series(seriesIdx).Clear();
		for(int i=0; i<MS400; i++)
			m_chart[chartIdx+LR].Series(seriesIdx).AddXY(double(i)/FRAMERATE*1000, m_analyzeHeadThrust[LR].m_ppMeanHeadVel[signIdx][i], NULL, COLOR_HEAD_MEAN_VEL);
		
		
		//left head vel-eye vel point plot
		m_chartVel[chartIdx].Series(1).GetXValues().SetValue(0, m_analyzeHeadThrust[LR].m_structMeanHT[0].headPeakVel);
		m_chartVel[chartIdx].Series(1).GetYValues().SetValue(0, m_analyzeHeadThrust[LR].m_structMeanHT[0].eyePeakVel);
	}

}

void CDlgAnalyzeHeadThrust::deleteHT(int idxHT)
{
	
	if(!idxHT)
		return;

	int leftIdx, rightIdx;

	//양수라면 positive에서, 음수라면 negative에서 찾는다.
	int chartIdx = (idxHT>0) ? 0 : 2;
	
	
	double headVel, eyeVel;
	//left eye에서 찾는다.
	headVel = m_chartVel[chartIdx].Series(0).GetXValues().GetValue(abs(idxHT)-1);
	eyeVel = m_chartVel[chartIdx].Series(0).GetYValues().GetValue(abs(idxHT)-1);
	leftIdx = m_analyzeHeadThrust[0].findHT(headVel, eyeVel);

	//right eye에서 찾는다.
	headVel = m_chartVel[chartIdx+1].Series(0).GetXValues().GetValue(abs(idxHT)-1);
	eyeVel = m_chartVel[chartIdx+1].Series(0).GetYValues().GetValue(abs(idxHT)-1);
	rightIdx = m_analyzeHeadThrust[1].findHT(headVel, eyeVel);

	if(leftIdx == -1)
		leftIdx = rightIdx;

	if(rightIdx == -1)
		rightIdx = leftIdx;

	//두개가 같아야 한다.
	if(leftIdx != -1)
	{
		//list에서 없앤다.
		m_analyzeHeadThrust[0].deleteHT(leftIdx);
		m_analyzeHeadThrust[1].deleteHT(rightIdx);


		//다시 그린다.
		this->showResult2();

		int count = this->m_chart[0].GetSeriesCount();
		int w = m_chart[0].Series(count-1).GetPen().GetWidth();
		w = m_chart[0].Series(count-1).GetPen().GetWidth();
		w = 1;

	}

}

void CDlgAnalyzeHeadThrust::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->saveAnalyzeData();

	OnOK();
}



bool CDlgAnalyzeHeadThrust::saveAnalyzeData()
{
	bool bRtn = true;

	CString str;
	CFileStatus fs;
	//Left
	str = this->m_strTestFile;
	str.Replace(".emd", "L.ema");
	if(!this->m_analyzeHeadThrust[0].save(str))
		bRtn = false;

	//Right
	str = this->m_strTestFile;
	str.Replace(".emd", "R.ema");
	if(!m_analyzeHeadThrust[1].save(str))
		bRtn = false;

	return bRtn;

}

void CDlgAnalyzeHeadThrust::OnBnClickedButtonHeadThrustOption()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_dlgHeadThrustOption.DoModal();
	
}

void CDlgAnalyzeHeadThrust::OnBnClickedButtonHtPrint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strProgram;
	corGetProgramFolder(&strProgram);

	
	//form 파일명
	CString strFormFname = strProgram + "\\form\\analyze_head_thrust.rpt";

	//print에 필요한 파일들을 생성할 폴더
	CString strPrintTempFolder = strProgram + "\\printTemp";
	//없다면 만든다.
	if(!corFolderExist(strPrintTempFolder))
		corCreateDirectory(strPrintTempFolder);

	


	CRect rect;

	//eye movement chart export
	CString fname = strPrintTempFolder +"\\eye_velocity.bmp";
	exportTChart(&m_chartMain, fname);

	//time-velocity curve chart export
	CString strVels[4] = {"left_positive_velocity.bmp", "right_positive_velocity.bmp",
					"left_negative_velocity.bmp", "right_negative_velocity.bmp"};
	for(int i=0; i<4; i++)
	{
		fname = strPrintTempFolder +"\\" + strVels[i];
		exportTChart(&m_chart[i], fname);
	}

	//head_peak - eye peak chart export
	CString strPeaks[4] = {"left_positive_peak.bmp", "right_positive_peak.bmp",
							"left_negative_peak.bmp", "right_negative_peak.bmp"};

	//gain chart export
	for(int i=0; i<4; i++)
	{
		fname = strPrintTempFolder +"\\" + strPeaks[i];
		exportTChart(&m_chartVel[i], fname);
	}
	
	//load form
	CFile file;
	CString strForm;
	if(file.Open(strFormFname, CFile::modeRead))
	{
		unsigned char* pBuf = new unsigned char[size_t(file.GetLength())+1];
		pBuf[file.GetLength()] = NULL;
		file.Read(pBuf, UINT(file.GetLength()));
		strForm = CString(pBuf);
		delete []pBuf;
		file.Close();
	}
	else
		return;

	//insert subtitle
	CString strSubtitle(m_TI.m_chFileName);
	strSubtitle = strSubtitle.Left(strSubtitle.ReverseFind('.'));
	strSubtitle.Replace(",", "-");
	strForm.Replace("TEST_SUBTITLE", strSubtitle);

	//TEST DIRECTION
	CString strDirection = m_TI.m_analTarget == HORIZONTAL ? "Horizontal" : "Vertical";
	strForm.Replace("TEST_DIRECTION", strDirection);

	//insert subject info
	CString temp;
	strForm.Replace("SUBJECT_DOCTOR", m_patientInfo.m_chDoctor);
	strForm.Replace("SUBJECT_TESTER", m_patientInfo.m_chTester);
	strForm.Replace("SUBJECT_ID", m_patientInfo.m_chID);
	strForm.Replace("SUBJECT_NAME", m_patientInfo.m_chName);
	temp.Format("%s",  m_patientInfo.m_nSex ? "Male":"Femail");
	strForm.Replace("SUBJECT_SEX", temp);
	temp.Format("%d",  m_patientInfo.m_nAge);
	strForm.Replace("SUBJECT_AGE", temp);
	temp.Format("%d/%02d/%02d", m_patientInfo.m_timeTime.GetYear(), m_patientInfo.m_timeTime.GetMonth(), m_patientInfo.m_timeTime.GetDay());
	strForm.Replace("SUBJECT_DATE", temp);
	temp.Format("%02d-%02d-%02d", m_patientInfo.m_timeTime.GetHour(), m_patientInfo.m_timeTime.GetMinute(), m_patientInfo.m_timeTime.GetSecond());
	strForm.Replace("SUBJECT_TIME", temp);


	//insert test parameters
	//LP, LN, RP, RN 순서
	double headVel[4] = {m_analyzeHeadThrust[0].m_structMeanHT[0].headPeakVel,	//left positive
						m_analyzeHeadThrust[0].m_structMeanHT[1].headPeakVel,	//elft negative
						m_analyzeHeadThrust[1].m_structMeanHT[0].headPeakVel,	//right positive
						m_analyzeHeadThrust[1].m_structMeanHT[1].headPeakVel};	//left negative
	double eyeVel[4] = {m_analyzeHeadThrust[0].m_structMeanHT[0].eyePeakVel,	//left positive
						m_analyzeHeadThrust[0].m_structMeanHT[1].eyePeakVel,	//elft negative
						m_analyzeHeadThrust[1].m_structMeanHT[0].eyePeakVel,	//right positive
						m_analyzeHeadThrust[1].m_structMeanHT[1].eyePeakVel};	//left negative
	double gain[4] = {0, 0, 0, 0};
	for(int i=0; i<4; i++)
		gain[i] = headVel[i] ? eyeVel[i]/headVel[i] : 0;
	double latency[4] = {0, 0, 0, 0};

	CString idsHeadVel[] = {"HEAD_VEL_LP", "HEAD_VEL_LN", "HEAD_VEL_RP", "HEAD_VEL_RN"};
	for(int i=0; i<4; i++)
		insertString2Report(&strForm, headVel[i], idsHeadVel[i]);
	
	CString idsEyeVel[] = {"EYE_VEL_LP", "EYE_VEL_LN", "EYE_VEL_RP", "EYE_VEL_RN"};
	for(int i=0; i<4; i++)
		insertString2Report(&strForm, eyeVel[i], idsEyeVel[i]);


	CString idsGain[] = {"GAIN_LP", "GAIN_LN", "GAIN_RP", "GAIN_RN"};
	for(int i=0; i<4; i++)
		insertString2Report(&strForm, gain[i], idsGain[i]);


	CString idsLatency[] = {"LATENCY_LP", "LATENCY_LN", "LATENCY_RP", "LATENCY_RN"};
	for(int i=0; i<4; i++)
		insertString2Report(&strForm, latency[i], idsLatency[i]);




	/*
	// insert test parameters
	structPrintPursuit printPursuit[2];
	this->analyzeForPrint(printPursuit, false);

	double left[2] = {printPursuit[LEFT].gain[0], printPursuit[LEFT].gain[1]};
	double right[2] = {printPursuit[RIGHT].gain[0], printPursuit[RIGHT].gain[1]};

	double value[4] = {printPursuit[LEFT].gain[0], printPursuit[LEFT].gain[1],
		printPursuit[RIGHT].gain[0], printPursuit[RIGHT].gain[1]};

	CString ids[] = {"GAIN_LL", "GAIN_LR", "GAIN_RL", "GAIN_RR"};
	for(int i=0; i<4; i++)
		insertString2Report(&strForm, value[i], ids[i]);
	*/

	// make report file
	fname = strPrintTempFolder + "\\analyze_head_thrust.rpt";
	if(file.Open(fname, CFile::modeWrite | CFile::modeCreate))
	{
		file.Write(strForm, strForm.GetLength());
		file.Close();
	}
	else 
		return;


	
	

	//print
	CString strCurrentFolder = setCurrentWorkPath(strPrintTempFolder);
	CReportGenerator report;
	report.New();
	report.AddTemplate(fname, TEMPLATE_TYPE_SECTION);
	report.Print();
	report.Clear();
	setCurrentWorkPath(strCurrentFolder);
}

void CDlgAnalyzeHeadThrust::OnScrollTchartHeadThrustMain()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	m_chartMain.Repaint();
	EU_EventFile::adjustEventLocation(&m_chartMain, this->m_eventSeriesIdx);
	
}

void CDlgAnalyzeHeadThrust::OnZoomTchartHeadThrustMain()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	m_chartMain.Repaint();
	EU_EventFile::adjustEventLocation(&m_chartMain, this->m_eventSeriesIdx);
	
}

void CDlgAnalyzeHeadThrust::OnUndoZoomTchartHeadThrustMain()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	m_chartMain.Repaint();
	EU_EventFile::adjustEventLocation(&m_chartMain, this->m_eventSeriesIdx);
	
}

/*
void CDlgAnalyzeHeadThrust::OnBeforeDrawSeriesTchartHeadThrustMain()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EU_EventFile::adjustEventLocation(&m_chartMain, this->m_eventSeriesIdx);
}*/

void setListCheckbox(CListCtrl* pListCtrl, int itemIndex, BOOL bCheck)
{
	pListCtrl->SetItemState(itemIndex, UINT((int(bCheck) + 1) << 12), LVIS_STATEIMAGEMASK);
}
