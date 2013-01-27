// DlgAnalSaccade.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "corwldefines.h"
#include "DlgAnalSaccade.h"
#include ".\dlganalsaccade.h"
#include "eu_testfilev2.h"
#include "ed_testinfo.h"
#include "corwlUtil.h"
#include "includeTChart.h"
#include ".\reportGenerator\reportGenerator.h"
#include ".\eu_eventfile.h"


#include "math.h"

// CDlgAnalSaccade 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAnalSaccade, CDialog)
CDlgAnalSaccade::CDlgAnalSaccade(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalSaccade::IDD, pParent)
{
	m_pdLeft = m_pdRight = m_pdTarget = NULL;
	m_ulDataCount = 0;

	m_bEditing = false;
	m_pSaccadeSelected = NULL;

	int m_eventSeriesIdx = -1;
}

CDlgAnalSaccade::~CDlgAnalSaccade()
{
	DEL(m_pdLeft);
	DEL(m_pdRight);
	DEL(m_pdTarget);
}

void CDlgAnalSaccade::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_AS_ZOOM, m_chartZoom);
	DDX_Control(pDX, IDC_TCHART_AS_PEAK_VEL, m_chartPeakVel);
	DDX_Control(pDX, IDC_TCHART_AS_ACCURACY, m_chartAccuracy);
	DDX_Control(pDX, IDC_TCHART_AS_LATENCY, m_chartLatency);
	DDX_Control(pDX, IDC_TCHART_AS_DATA, m_chartRaw);
	DDX_Control(pDX, IDC_LIST_AS_RESULT_SUMMARY, m_listResultSummary);
	DDX_Control(pDX, IDC_LIST_AS_RESULT_DETAIL, m_listResultDetail);
}


BEGIN_MESSAGE_MAP(CDlgAnalSaccade, CDialog)
	ON_BN_CLICKED(IDC_RADIO_AS_ALL, OnBnClickedRadioEyeSelect)
	ON_BN_CLICKED(IDC_RADIO_AS_LEFT, OnBnClickedRadioEyeSelect)
	ON_BN_CLICKED(IDC_RADIO_AS_RIGHT, OnBnClickedRadioEyeSelect)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_AS_RESULT_DETAIL, OnLvnItemchangedListAsResultDetail)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AS_RESULT_DETAIL, OnNMDblclkListAsResultDetail)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_AS_REFRESH, OnBnClickedButtonAsRefresh)
	ON_BN_CLICKED(IDC_BUTTON_SACCADE_PRINT, OnBnClickedButtonSaccadePrint)
	ON_BN_CLICKED(IDC_CHECK_REMOVE_OFFSET, OnBnClickedCheckRemoveOffset)
END_MESSAGE_MAP()


// CDlgAnalSaccade 메시지 처리기입니다.


BOOL CDlgAnalSaccade::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ShowWindow(SW_SHOWMAXIMIZED);
	layoutCtrls();

	
	this->initTChart();
	
	
	this->loadData();

	//먼저 ANALYZE하고 ANALYZE FILE이 있으면 결과만 덮어쓴다.
	analyzeThis(BOTH);
	loadAnalyzeData();

	this->initListCtrls();
	showResult();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CDlgAnalSaccade::loadData()
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

bool CDlgAnalSaccade::loadAnalyzeData()
{
	bool bRtn = true;

	CString str;
	CFileStatus fs;
	//Left
	str = this->m_strTestFile;
	str.Replace(".emd", "L.ema");
	if(CFile::GetStatus(str, fs))
		this->m_analyzeSaccade[0].load(str);
	else
		bRtn = false;

	//Right
	str = this->m_strTestFile;
	str.Replace(".emd", "R.ema");
	if(CFile::GetStatus(str, fs))
		this->m_analyzeSaccade[1].load(str);
	else
		bRtn = false;

	return bRtn;
}

bool CDlgAnalSaccade::saveAnalyzeData()
{
	bool bRtn = true;

	CString str;
	CFileStatus fs;
	//Left
	str = this->m_strTestFile;
	str.Replace(".emd", "L.ema");
	if(!m_analyzeSaccade[0].save(str))
		bRtn = false;

	//Right
	str = this->m_strTestFile;
	str.Replace(".emd", "R.ema");
	if(!m_analyzeSaccade[1].save(str))
		bRtn = false;

	return bRtn;

}


void CDlgAnalSaccade::allocVariable()
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

void CDlgAnalSaccade::initListCtrls()
{
	//setup list summary
	CRect rect;
	this->m_listResultSummary.GetClientRect(&rect);
	int w = rect.Width();

	char* list[7] = {"", "Peak Vel L(°/s) ", "Peak Vel R(°/s) ", 
						"Latency L(ms)", "Latency R(ms)",
						"Accuracy L(%)", "Accuracy R(%)"};
	if(m_TI.m_analTarget == VERTICAL)
	{
		 list[1] = "Peak Vel U(°/s) ";
		 list[2] = "Peak Vel D(°/s) ";
		 list[3] = "Latency U(ms)";
		 list[4] = "Latency D(ms)";
		 list[5] = "Accuracy U(%)";
		 list[6] = "Accuracy D(%)";
	}
	double width[7] = {.1, .15, .15, .15, .15, .15, .15, };
	for(int i=0; i<7; i++)
		m_listResultSummary.InsertColumn(i, list[i], LVCFMT_CENTER, int(w*width[i]));

	//setup list detail
	m_listResultDetail.GetClientRect(&rect);
	w = rect.Width();
	DWORD dwStyle = m_listResultDetail.GetExStyle();
	dwStyle = (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listResultDetail.SetExtendedStyle(dwStyle);

	char* listD[6] = {"No", "Peak Velocity(°/s)", "Accuracy(%)", "Latency(ms)", "Target start(s)", "Saccade End(s)"};
	double widthD[6] = {.1, .3, .3, .3, 0, 0};
	for(int i=0; i<6; i++)
		m_listResultDetail.InsertColumn(i, listD[i], LVCFMT_CENTER, int(widthD[i]*w));
}

void CDlgAnalSaccade::initTChart()
{
	m_selectedSacIdx = -1;

	//raw
	CTChart* bufTChart[5];
	bufTChart[0] = &m_chartZoom;
	bufTChart[1] = &m_chartPeakVel;
	bufTChart[2] = &m_chartAccuracy;
	bufTChart[3] = &m_chartLatency;
	bufTChart[4] = &m_chartRaw;


	CString strTitles[5] = {"Zoom", "Peak Velocity", "Accuracy", "Latency", "Data"};

	for(int i=0; i<5; i++)
	{
		bufTChart[i]->ClearChart();
		bufTChart[i]->GetAspect().SetView3D(false);
		bufTChart[i]->GetZoom().SetEnable(true);
		bufTChart[i]->GetAspect().SetZoom(tzdBoth);
		if(i!=4)
			bufTChart[i]->GetLegend().SetVisible(false);
		bufTChart[i]->GetLegend().SetAlignment(laBottom);		

		bufTChart[i]->GetHeader().GetText().SetText(strTitles[i]);
		bufTChart[i]->SetAutoRepaint(false);
	}

	
	

	//chart zoom
	double axisMin[4] = {-40, 0, 150, 0};
	double axisMax[4] = {40, 750, 0, 750};
	CString axisTitle[4] = {"Degree", "°/sec", "%", "ms"};
	for(int i=0; i<4; i++)
	{
		bufTChart[i]->AddSeries(scPoint);
		bufTChart[i]->AddSeries(scPoint);		
		
		bufTChart[i]->Series(0).GetAsPoint().GetPointer().SetStyle(psCircle);
		bufTChart[i]->Series(0).GetAsPoint().GetPointer().SetHorizontalSize(3);
		bufTChart[i]->Series(0).GetAsPoint().GetPointer().SetVerticalSize(3);		
		bufTChart[i]->Series(1).GetAsPoint().GetPointer().SetStyle(psCircle);
		bufTChart[i]->Series(1).GetAsPoint().GetPointer().SetHorizontalSize(3);
		bufTChart[i]->Series(1).GetAsPoint().GetPointer().SetVerticalSize(3);

		bufTChart[i]->Series(0).GetXValues().SetOrder(loNone);
		bufTChart[i]->Series(1).GetXValues().SetOrder(loNone);
		

		bufTChart[i]->GetAxis().GetLeft().SetMinMax(axisMin[i], axisMax[i]);
		bufTChart[i]->GetAxis().GetLeft().GetTitle().SetCaption(axisTitle[i]);
		bufTChart[i]->GetAxis().GetBottom().SetMinMax(-40, 40);	

		
		if(i!=0)
		{
			//cursor tool
			bufTChart[i]->GetTools().Add(tcCursor);
			bufTChart[i]->GetTools().GetItems(0).SetActive(false);			
			bufTChart[i]->GetTools().GetItems(0).GetAsTeeCursor().SetFollowMouse(false);
			/*VARIANT varSeries; varSeries.vt=VT_DISPATCH;
			CSeries series = bufTChart[i]->Series(0);
			varSeries.pdispVal = series;
			bufTChart[i]->GetTools().GetItems(0).GetAsTeeCursor().SetSeries(varSeries);			*/
			bufTChart[i]->GetTools().GetItems(0).GetAsTeeCursor().SetStyle(cssBoth);
			bufTChart[i]->GetTools().GetItems(0).GetAsTeeCursor().SetXVal(10);
			bufTChart[i]->GetTools().GetItems(0).GetAsTeeCursor().SetYVal(110);
		

			//nearest tool
			bufTChart[i]->GetTools().Add(tcNearest);
			bufTChart[i]->GetTools().GetItems(1).GetAsNearest().SetDrawLine(false);
			bufTChart[i]->GetTools().SetActive(false);
		}

		

	}

	//data chart with time
	m_chartRaw.GetLegend().SetVisible(true);
	m_chartRaw.GetAxis().GetLeft().SetMinMax(-40, 40);
	m_chartRaw.GetAxis().GetLeft().GetTitle().SetCaption("Degree");

	m_chartRaw.AddSeries(scLine);	//Left
	m_chartRaw.AddSeries(scLine);	//Right
	m_chartRaw.AddSeries(scLine);	//Target
	m_chartRaw.AddSeries(scPoint);	//Left saccade result(3)
	m_chartRaw.AddSeries(scPoint);	//Right saccade result(4)

	m_chartRaw.Series(0).SetTitle("Left");
	m_chartRaw.Series(1).SetTitle("Right");
	m_chartRaw.Series(2).SetTitle("Target");
	m_chartRaw.Series(0).SetColor(COLOR_LEFT);
	m_chartRaw.Series(1).SetColor(COLOR_RIGHT);
	m_chartRaw.Series(2).SetColor(COLOR_TARGET);
	m_chartRaw.Series(3).GetAsPoint().GetPointer().SetStyle(psCircle);
	m_chartRaw.Series(3).GetAsPoint().GetPointer().SetHorizontalSize(3);
	m_chartRaw.Series(3).GetAsPoint().GetPointer().SetVerticalSize(3);		
	m_chartRaw.Series(3).SetShowInLegend(false);
	m_chartRaw.Series(4).GetAsPoint().GetPointer().SetStyle(psCircle);
	m_chartRaw.Series(4).GetAsPoint().GetPointer().SetHorizontalSize(3);
	m_chartRaw.Series(4).GetAsPoint().GetPointer().SetVerticalSize(3);		
	m_chartRaw.Series(4).SetShowInLegend(false);

	//data chart에 colorband를 그려준다.
	m_chartRaw.GetTools().Add(tcColorband);
	m_chartRaw.GetTools().GetItems(0).GetAsColorband().SetColor(COLOR_SELECTED);
	m_chartRaw.GetTools().GetItems(0).GetAsColorband().SetTransparency(50);
	m_chartRaw.GetTools().GetItems(0).GetAsColorband().GetPen().SetVisible(false);
	VARIANT varAxis; varAxis.vt=VT_DISPATCH;
	CAxis ax = m_chartRaw.GetAxis().GetBottom();
	varAxis.pdispVal = ax;
	m_chartRaw.GetTools().GetItems(0).GetAsColorband().SetAxis(varAxis);
	m_chartRaw.GetTools().GetItems(0).GetAsColorband().SetDrawBehind(true);
	m_chartRaw.GetTools().GetItems(0).SetActive(false);

	

	// init event series
	m_eventSeriesIdx = EU_EventFile::initEventSeries(&m_chartRaw);

	//다시 그려준다.
	for(int i=0; i<5; i++)
	{
		bufTChart[i]->SetAutoRepaint(true);
		bufTChart[i]->Repaint();
	}
	


}
void CDlgAnalSaccade::analyzeThis(int LR)
{
	if(this->m_pdLeft)
	{
		if(LR == BOTH)
		{
			this->m_analyzeSaccade[0].analyze(m_pdLeft, m_pdTarget, m_ulDataCount);
			this->m_analyzeSaccade[1].analyze(m_pdRight, m_pdTarget, m_ulDataCount);
		}
		else
			this->m_analyzeSaccade[LR].analyze((LR==0) ? m_pdLeft : m_pdRight, m_pdTarget, m_ulDataCount);
	}
}

void CDlgAnalSaccade::layoutCtrls()
{
	//all을 선택
	((CButton*)GetDlgItem(IDC_RADIO_AS_ALL))->SetCheck(true);

	double gap = 0.01;
	double button_h = gap*4;
	double button_w = button_h*3;
	CRect rect;
	this->GetClientRect(&rect);

	//radio
	corLocateWindow(GetDlgItem(IDC_RADIO_AS_ALL), gap, gap, button_w, 0.1, rect);
	corLocateWindow(GetDlgItem(IDC_RADIO_AS_LEFT), gap*2+button_w, gap, button_w, 0.1, rect);
	corLocateWindow(GetDlgItem(IDC_RADIO_AS_RIGHT), gap*3+button_w*2, gap, button_w, 0.1, rect);

	double chartW = 0.95/2;
	//list ctrl
	corLocateWindow(GetDlgItem(IDC_LIST_AS_RESULT_SUMMARY), gap, 0.1+2*gap, 0.6, 0.2, rect);
	corLocateWindow(GetDlgItem(IDC_LIST_AS_RESULT_DETAIL), gap*2+0.6, 0.1+2*gap, 0.4-gap*3, 0.2, rect);

	

	//charts
	chartW = 0.96/3;
	//corLocateWindow(&m_chartZoom, gap, 0.3+3*gap, chartW, 0.3, rect);
	corLocateWindow(&m_chartPeakVel, gap, 0.3+3*gap, chartW, 0.3, rect);
	corLocateWindow(&m_chartAccuracy, gap*2+chartW, 0.3+3*gap, chartW, 0.3, rect);
	corLocateWindow(&m_chartLatency, gap*3+2*chartW, 0.3+3*gap, chartW, 0.3, rect);

	//remove offset
	corLocateWindow(GetDlgItem(IDC_CHECK_REMOVE_OFFSET), gap, 0.6+3*gap + 0.5*gap, 10*gap, 2*gap, rect);

	//chart with time
	corLocateWindow(&m_chartRaw, gap, 0.6+5*gap, 1-2*gap, 0.3, rect);

	//ok cancel
	//button
	corLocateWindow(GetDlgItem(IDC_BUTTON_AS_REFRESH), gap, 1-gap-button_h, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDC_BUTTON_SACCADE_PRINT), 2*gap + button_w, 1-gap-button_h, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDOK),1-button_w*2-2*gap, 1-gap-button_h, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDCANCEL),1-button_w-gap, 1-gap-button_h, button_w, button_h, rect);



	
}
void CDlgAnalSaccade::showResult()
{
	//list ctrl
	this->fillListSummary();
	this->fillListDetail();

	this->initTChart();

	CTChart* bufTChart[5];
	bufTChart[0] = &m_chartZoom;
	bufTChart[1] = &m_chartPeakVel;
	bufTChart[2] = &m_chartAccuracy;
	bufTChart[3] = &m_chartLatency;
	bufTChart[4] = &m_chartRaw;

	for(int i=0; i<5; i++)
		bufTChart[i]->SetAutoRepaint(false);


	// chart with time
	double t = 0;
	for(unsigned long i=0; i<this->m_ulDataCount; i++)
	{
		t = double(i)/FRAMERATE;
		this->m_chartRaw.Series(0).AddXY(t, this->m_pdLeft[i], NULL, 0);
		this->m_chartRaw.Series(1).AddXY(t, this->m_pdRight[i], NULL, 0);
		this->m_chartRaw.Series(2).AddXY(t, this->m_pdTarget[i], NULL, 0);
	}

	
	this->showAnalyze();


	//event file 을 load한다.
	CString fname = m_strTestFile;
	fname.Replace(".emd", ".evt");
	EU_EventFile::loadEventFile(fname, &m_chartRaw, this->m_eventSeriesIdx, false);
	

	

	for(int i=0; i<5; i++)
	{
		bufTChart[i]->SetAutoRepaint(true);
		bufTChart[i]->Repaint();
	}

}

void CDlgAnalSaccade::showAnalyze()
{
	m_chartPeakVel.Series(LEFT).Clear();
	m_chartPeakVel.Series(RIGHT).Clear();
	m_chartAccuracy.Series(LEFT).Clear();
	m_chartAccuracy.Series(RIGHT).Clear();
	m_chartLatency.Series(LEFT).Clear();
	m_chartLatency.Series(RIGHT).Clear();

	m_chartRaw.Series(LEFT+3).Clear();
	m_chartRaw.Series(RIGHT+3).Clear();
	


	//분석결과
	unsigned int count = 0;
	count = this->m_analyzeSaccade[0].getCount();
	POSITION pos1, pos2;
	double x;
	// left	

	double maxSaccade = 0;

	for(pos1 = m_analyzeSaccade[0].m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; )
	{
		structSaccade* pSac = m_analyzeSaccade[0].m_listSaccade.GetNext(pos1);
		x = pSac->targetLeap;
		
		m_chartPeakVel.Series(LEFT).AddXY(x, fabs(pSac->peakVel), "", COLOR_LEFT);	//peak vel
		m_chartAccuracy.Series(LEFT).AddXY(x, pSac->accuracy, "", COLOR_LEFT);	//accuracy
		m_chartLatency.Series(LEFT).AddXY(x, pSac->latency*(1000./FRAMERATE), "", COLOR_LEFT);		//latency

		//x축이 범위에서 벗어나지 않게 확인한다.
		if(fabs(pSac->targetLeap) > maxSaccade)
			maxSaccade = fabs(pSac->targetLeap);
		

	}
	//right
	count = this->m_analyzeSaccade[1].getCount();
	for(pos1 = m_analyzeSaccade[1].m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; )
	{
		structSaccade* pSac = m_analyzeSaccade[1].m_listSaccade.GetNext(pos1);
		x = pSac->targetLeap;
		
		m_chartPeakVel.Series(RIGHT).AddXY(x, fabs(pSac->peakVel), "", COLOR_RIGHT);	//peak vel
		m_chartAccuracy.Series(RIGHT).AddXY(x, pSac->accuracy, "", COLOR_RIGHT);	//accuracy
		m_chartLatency.Series(RIGHT).AddXY(x, pSac->latency*(1000.0/FRAMERATE), "", COLOR_RIGHT);		//latency

		if(fabs(pSac->targetLeap) > maxSaccade)
			maxSaccade = fabs(pSac->targetLeap);
		
	}

	//x축이 범위에서 벗어나면 넓혀준다.
	if(maxSaccade > m_chartPeakVel.GetAxis().GetBottom().GetMaximum())
	{
		maxSaccade = 10*(int(maxSaccade/10)+1);

		m_chartPeakVel.GetAxis().GetBottom().SetMinMax(-1*maxSaccade, maxSaccade);
		m_chartAccuracy.GetAxis().GetBottom().SetMinMax(-1*maxSaccade, maxSaccade);
		m_chartLatency.GetAxis().GetBottom().SetMinMax(-1*maxSaccade, maxSaccade);
	}


	//각 timing을 그려준다.
	for(int LR=LEFT; LR<=RIGHT; LR++)
	{
		CValueList valueTarget = m_chartRaw.Series(2).GetYValues();
		CValueList valueEye = m_chartRaw.Series(LR).GetYValues();
		m_chartRaw.Series(LR+3).Clear();
		
		for(pos1 = m_analyzeSaccade[LR].m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structSaccade* pSac = m_analyzeSaccade[LR].m_listSaccade.GetNext(pos1);
			unsigned long idx = pSac->startIdx;
			m_chartRaw.Series(LR+3).AddXY(double(idx)/FRAMERATE, valueTarget.GetValue(idx), NULL, 0x00f00000);
			idx = pSac->startIdx+ pSac->latency;
			m_chartRaw.Series(LR+3).AddXY(double(idx)/FRAMERATE, valueEye.GetValue(idx), NULL, 0x0000ff00);
			idx = pSac->endIdx;
			m_chartRaw.Series(LR+3).AddXY(double(idx)/FRAMERATE, valueEye.GetValue(idx), NULL, 0x000000ff);
		}
	}
}

void CDlgAnalSaccade::fillListSummary()
{
	int LR = this->getRadioLR();
	m_listResultSummary.DeleteAllItems();	

	
	//if(LR!=BOTH)
	{
		structPrintSaccade printSaccade[2];

		this->analyzeForPrint(printSaccade, false);
		double left[6] = {printSaccade[0].peakVel[0], printSaccade[0].peakVel[1],
						printSaccade[0].latency[0], printSaccade[0].latency[1],
						printSaccade[0].accuracy[0], printSaccade[0].accuracy[1]};
		double right[6] = {printSaccade[1].peakVel[0], printSaccade[1].peakVel[1],
						printSaccade[1].latency[0], printSaccade[1].latency[1],
						printSaccade[1].accuracy[0], printSaccade[1].accuracy[1]};
		corAdd2List(&m_listResultSummary, "Left Eye", left, 6);
		corAdd2List(&m_listResultSummary, "Right Eye", right, 6);
	}


/*
		

		double minValue, maxValue, meanValue;
		double val[3];

		
		//Latency
		this->m_analyzeSaccade[LR].getLatency(minValue, maxValue, meanValue);
		val[0] = minValue;
		val[1] = maxValue;
		val[2] = meanValue;
		corAdd2List(&m_listResultSummary, "Latency(ms)", val, 3);

		//accuracy
		this->m_analyzeSaccade[LR].getAccuracy(minValue, maxValue, meanValue);
		val[0] = minValue;
		val[1] = maxValue;
		val[2] = meanValue;
		corAdd2List(&m_listResultSummary, "Accuracy(%)", val, 3);

		//peak value
		this->m_analyzeSaccade[LR].getPeakVelocity(minValue, maxValue, meanValue);
		val[0] = minValue;
		val[1] = maxValue;
		val[2] = meanValue;
		corAdd2List(&m_listResultSummary, "Peak Vel(°/sec)", val, 3);


		//count 
		unsigned int count = this->m_analyzeSaccade[LR].getCount();
		val[0] = val[1] = val[2] = count;
		corAdd2List(&this->m_listResultSummary, "count", val, 3);
	}*/
}
void CDlgAnalSaccade::fillListDetail()
{
	int LR = getRadioLR();
	m_listResultDetail.DeleteAllItems();	


	POSITION pos1, pos2;
	
	
	if(LR == BOTH)
		return;

	CString strCount;
	int count = this->m_analyzeSaccade[LR].getCount();
	for(pos1 = this->m_analyzeSaccade[LR].m_listSaccade.GetTailPosition(); (pos2 = pos1) != NULL; )
	{
		//velocity chart에 표시
		structSaccade* pSac = m_analyzeSaccade[LR].m_listSaccade.GetPrev(pos1);

		strCount.Format("%d", count--);
		double val[5] = {pSac->peakVel, pSac->accuracy, pSac->latency*(1000.0/FRAMERATE), double(pSac->startIdx)/FRAMERATE, double(pSac->endIdx)/FRAMERATE};
		corAdd2List(&m_listResultDetail, strCount, val, 5);		
	}
}



int CDlgAnalSaccade::getRadioLR()
{
	//radio button이 어느눈을 선택하고 있는지 리턴한다.
	int LR;
	//determin eye
	if(((CButton*)GetDlgItem(IDC_RADIO_AS_LEFT))->GetCheck())
		LR = LEFT;
	else if(((CButton*)GetDlgItem(IDC_RADIO_AS_RIGHT))->GetCheck())
		LR = RIGHT;
	else
		LR = BOTH;

	return LR;
}

void CDlgAnalSaccade::OnBnClickedRadioEyeSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	selectSaccadeOnChart(-1);
		
}


void CDlgAnalSaccade::setLRVisible(bool leftVisible, bool rightVisible)
{
	CTChart* bufTChart[5];
	bufTChart[0] = &m_chartZoom;
	bufTChart[1] = &m_chartPeakVel;
	bufTChart[2] = &m_chartAccuracy;
	bufTChart[3] = &m_chartLatency;
	bufTChart[4] = &m_chartRaw;

	for(int i=0; i<5; i++)
	{
		bufTChart[i]->Series(LEFT).SetActive(leftVisible);
		bufTChart[i]->Series(RIGHT).SetActive(rightVisible);
	}

	// analysis marks
	m_chartRaw.Series(LEFT+3).SetActive(leftVisible);
	m_chartRaw.Series(RIGHT+3).SetActive(rightVisible);


	//nearest tool
	int LR = getRadioLR();
	for(int i=1; i<4; i++)
	{
		if(LR == BOTH)
		{
			bufTChart[i]->GetTools().SetActive(false);
		}
		else
		{
			bufTChart[i]->GetTools().SetActive(true);
			VARIANT varSeries; varSeries.vt=VT_DISPATCH;
			CSeries series = bufTChart[i]->Series(LR);
			varSeries.pdispVal = series;
			bufTChart[i]->GetTools().GetItems(1).GetAsNearest().SetSeries(varSeries);
		}
	}
}


void CDlgAnalSaccade::selectSaccadeOnChart(int count)
{
	//if(this->m_bEditing)
	//	this->enterEditMode(false, 0);	//cancel

	m_selectedSacIdx = count;
	// colorband를 해당 nystagmus위에 올려 놓는다.
	// -1일경우 invisible 시킨다.
	int LR = getRadioLR();
	if(LR == BOTH)
		return;

	if(count == -1)
	{
		m_chartRaw.GetTools().GetItems(0).SetActive(false);
		m_chartPeakVel.GetTools().GetItems(0).SetActive(false);
		m_chartAccuracy.GetTools().GetItems(0).SetActive(false);
		m_chartLatency.GetTools().GetItems(0).SetActive(false);

		this->m_pSaccadeSelected = NULL;
		if(this->m_bEditing)
			this->m_saccadeSaved = *m_pSaccadeSelected;
	}
	else
	{
		m_chartRaw.GetTools().GetItems(0).SetActive(true);
		m_chartPeakVel.GetTools().GetItems(0).SetActive(true);
		m_chartAccuracy.GetTools().GetItems(0).SetActive(true);
		m_chartLatency.GetTools().GetItems(0).SetActive(true);
		        
		POSITION pos1, pos2;
		int n = 0;
		
		for(pos1 = m_analyzeSaccade[LR].m_listSaccade.GetHeadPosition(); (pos2 = pos1) != NULL; n++)
		{
			m_pSaccadeSelected = m_analyzeSaccade[LR].m_listSaccade.GetNext(pos1);
			if(n == count)
				break;
		}
		if(this->m_bEditing)
			this->m_saccadeSaved = *m_pSaccadeSelected;

		//colorban의 위치 조정
		m_chartRaw.GetTools().GetItems(0).GetAsColorband().SetStartValue(double(m_pSaccadeSelected->startIdx)/FRAMERATE-0.1);
		m_chartRaw.GetTools().GetItems(0).GetAsColorband().SetEndValue(double(m_pSaccadeSelected->endIdx)/FRAMERATE+0.1);

		//cursor 위치 조정
		m_chartPeakVel.GetTools().GetItems(0).GetAsTeeCursor().SetXVal(m_pSaccadeSelected->targetLeap);
		m_chartPeakVel.GetTools().GetItems(0).GetAsTeeCursor().SetYVal(fabs(m_pSaccadeSelected->peakVel));
		m_chartAccuracy.GetTools().GetItems(0).GetAsTeeCursor().SetXVal(m_pSaccadeSelected->targetLeap);
		m_chartAccuracy.GetTools().GetItems(0).GetAsTeeCursor().SetYVal(m_pSaccadeSelected->accuracy);
		m_chartLatency.GetTools().GetItems(0).GetAsTeeCursor().SetXVal(m_pSaccadeSelected->targetLeap);
		m_chartLatency.GetTools().GetItems(0).GetAsTeeCursor().SetYVal(m_pSaccadeSelected->latency*(1000.0/FRAMERATE));	

	}
}
void CDlgAnalSaccade::OnLvnItemchangedListAsResultDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
    long lRow = 0;                                  // 로우
    int nCol = 0;                                   // 컬럼

	//새롭게 선택된것
	if(temp->uNewState == 3)
	{

		lRow = temp->iItem;                             // 로우값(클릭한 로우리턴)
		nCol = temp->iSubItem;                          // 컬럼값(클릭한 컬럼리턴 : 님 경우 별 쓸모없겠지만..)

		

		this->selectSaccadeOnChart(lRow);
	}
}

BOOL CDlgAnalSaccade::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		

		if(this->m_bEditing)
		{
			// left right는 onset 을 조정
			if(pMsg->wParam == 37)	//left
			{
				moveSelectedSaccade(-1, 0);
				return true;
			}
			else if(pMsg->wParam == 39)	//right
			{
				moveSelectedSaccade(1, 0);
				return true;
			}
			//up down은 offset을 저정
			else if(pMsg->wParam == 38) // up
			{
				moveSelectedSaccade(0, 1);
				return true;
			}
			else if(pMsg->wParam == 40)	//down
			{
				moveSelectedSaccade(0, -1);
				return true;
			}
		}
		else
		{
			if(pMsg->wParam == 46) //DEL
			{
				int LR = this->getRadioLR();
				if(( (LR == LEFT) || (LR == RIGHT) )  && (this->m_selectedSacIdx != -1))
					deleteSelectedSaccade(LR, m_selectedSacIdx);				
				
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

		// ctrl + E : export to csv9
		if(pMsg->wParam == 69)
		{
			//bool bShift = ::GetAsyncKeyState(VK_LCONTROL);
			bool bCtrl = IsCTRLpressed();
			if(bCtrl)
			{
				exportToCSV();
			}
		}


	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgAnalSaccade::deleteSelectedSaccade(int LR, int sacIdx)
{
	
	// 1. list에서 삭제
	// 2. 결과 update
	// 3. chart update
	// 4. list control에서 삭제
	// 5. 다음 item 선택

	int count = int(this->m_analyzeSaccade[LR].m_listSaccade.GetCount());

	//validation
	if( ((LR != LEFT) && (LR != RIGHT)) || (sacIdx >= count))
		return;

	// 1. list에서 삭제
	POSITION pos = m_analyzeSaccade[LR].m_listSaccade.FindIndex(sacIdx);
	structSaccade* pSac = m_analyzeSaccade[LR].m_listSaccade.GetAt(pos);
	delete pSac;
	m_analyzeSaccade[LR].m_listSaccade.RemoveAt(pos);


	count = this->m_analyzeSaccade[1].getCount();

	// 2. 3. chart 및 list update
	this->showAnalyze();
	count = this->m_analyzeSaccade[1].getCount();
	this->fillListSummary();
	count = this->m_analyzeSaccade[1].getCount();
	this->fillListDetail();
	count = this->m_analyzeSaccade[1].getCount();
	
	

	// 5. 다음 아이템 선택
	// 마지막 item이 아니라면 다음 nystagmus를 선택하고 마지막이라면 이전 것을 선택한다.
	if(sacIdx == count-1)
		sacIdx--;
	if(sacIdx >= 0)
	{
		m_listResultDetail.SetSelectionMark(sacIdx);
		m_listResultDetail.SetItemState(sacIdx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_listResultDetail.SetFocus();

		//chat의 colorband는 selection을 바꿈으로써 OnLvnItemChagedList()에서 이동한다.
	}
	else
	{
		// 더이상 nystagmus가 없다면
		// colorBand를 inactive 시킨다.
		selectSaccadeOnChart(-1);

	}

	
	


	
}
BEGIN_EVENTSINK_MAP(CDlgAnalSaccade, CDialog)
	
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_DATA, 3, OnClickTchart, VTS_NONE)
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_DATA, 8, OnDblClickTchart, VTS_NONE)
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_ACCURACY, 3, OnClickTchartSmall, VTS_NONE)
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_ACCURACY, 8, OnDblClickTchartSmall, VTS_NONE)
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_PEAK_VEL, 3, OnClickTchartSmall, VTS_NONE)
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_PEAK_VEL, 8, OnDblClickTchartSmall, VTS_NONE)
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_LATENCY, 3, OnClickTchartSmall, VTS_NONE)
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_LATENCY, 8, OnDblClickTchartSmall, VTS_NONE)
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_DATA, 24, OnScrollTchartAsData, VTS_NONE)
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_DATA, 27, OnZoomTchartAsData, VTS_NONE)
	ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_DATA, 26, OnUndoZoomTchartAsData, VTS_NONE)
	//ON_EVENT(CDlgAnalSaccade, IDC_TCHART_AS_DATA, 29, OnBeforeDrawSeriesTchartAsData, VTS_NONE)
END_EVENTSINK_MAP()



void CDlgAnalSaccade::enterEditMode(bool bEdit, unsigned long saccadeIdx)
{
	int LR = this->getRadioLR();

	
	if(bEdit)
	{
		//edit mode로 들어간다.
		
		m_bEditing = true;
		this->m_chartRaw.GetTools().GetItems(0).GetAsColorband().SetColor(COLOR_EDIT);

		unsigned int count = int(this->m_analyzeSaccade[LR].m_listSaccade.GetCount());

		//validation
		if( ((LR != LEFT) && (LR != RIGHT)) || (saccadeIdx >= count))
			return;

		// 선택된 pursuit를 찾고 저장한다.
		if(m_pSaccadeSelected)
			this->m_saccadeSaved = (*m_pSaccadeSelected);
	}
	else
	{
		//edit mode에서 나간다.
		m_bEditing = false;
		m_chartRaw.GetTools().GetItems(0).GetAsColorband().SetColor(COLOR_SELECTED);

		if(saccadeIdx == 0)	//edit cancel
		{
			// 저장해 놓았던 것을 load한다.
			if(m_pSaccadeSelected)
				(*m_pSaccadeSelected) = this->m_saccadeSaved;
			
			this->showAnalyze();
			this->fillListSummary();
			
			double val[5] = {m_pSaccadeSelected->peakVel, 
						m_pSaccadeSelected->accuracy,
						m_pSaccadeSelected->latency*(1000.0/FRAMERATE),
						double(m_pSaccadeSelected->startIdx)/FRAMERATE, 
						double(m_pSaccadeSelected->endIdx)/FRAMERATE};
			corChangeListValue(&m_listResultDetail, m_selectedSacIdx, val, 5);
		}

	}
}
void CDlgAnalSaccade::moveSelectedSaccade(int latencyOffset, int endIdxOffset)
{
	if(this->m_pSaccadeSelected)
	{
		int LR = this->getRadioLR();

		m_pSaccadeSelected->latency += latencyOffset;
		m_pSaccadeSelected->endIdx += endIdxOffset;
		//latency 
		double* pEye = (LR==0) ? this->m_pdLeft : m_pdRight;
		double eyeLeap = pEye[m_pSaccadeSelected->endIdx] - pEye[m_pSaccadeSelected->startIdx+m_pSaccadeSelected->latency-1];
		m_pSaccadeSelected->accuracy = eyeLeap/m_pSaccadeSelected->targetLeap*100;		

		//peak velocity
		unsigned long startIdx = m_pSaccadeSelected->startIdx+m_pSaccadeSelected->latency-1;
		unsigned long endIdx = m_pSaccadeSelected->endIdx;
		unsigned long peakIdx = 0;
		double maxDiff =  getMaxAbsDiff((LR==0)?m_pdLeft:m_pdRight, startIdx, endIdx, &peakIdx, 3);
		m_pSaccadeSelected->peakVel =maxDiff/(double(3)/FRAMERATE);	//degree/sec

		this->showAnalyze();
		this->fillListSummary();
		
		
		double val[5] = {m_pSaccadeSelected->peakVel, 
						m_pSaccadeSelected->accuracy,
						m_pSaccadeSelected->latency*(1000.0/FRAMERATE),
						double(m_pSaccadeSelected->startIdx)/FRAMERATE, 
						double(m_pSaccadeSelected->endIdx)/FRAMERATE};
		corChangeListValue(&m_listResultDetail, m_selectedSacIdx, val, 5);
		//this->fillListDetail();
	}


}
void CDlgAnalSaccade::OnNMDblclkListAsResultDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
    long lRow = 0;                                  // 로우
    int nCol = 0;                                   // 컬럼

	//새롭게 선택된것
	//if(temp->uNewState == 3)
	{

		lRow = temp->iItem;                             // 로우값(클릭한 로우리턴)
		nCol = temp->iSubItem;                          // 컬럼값(클릭한 컬럼리턴 : 님 경우 별 쓸모없겠지만..)

		this->enterEditMode(true, lRow);
	}
}

void CDlgAnalSaccade::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	this->saveAnalyzeData();

	OnOK();
}

void CDlgAnalSaccade::OnBnClickedButtonAsRefresh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int LR = this->getRadioLR();

	this->analyzeThis(LR);
	this->showAnalyze();
	this->fillListSummary();
	this->fillListDetail();
}

void CDlgAnalSaccade::OnClickTchart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	//bool bShift = ::GetAsyncKeyState(VK_LSHIFT);
	bool bShift = IsSHIFTpressed();
	if(bShift)
	{
		//pursuit 추가
		int LR = this->getRadioLR();

		if(LR != BOTH)
		{
			// 1. mouse pos를 받는다.
			CRect rect;
			CPoint p;
			::GetCursorPos(&p);
			
		
			CTeePoint2D point = m_chartRaw.GetMousePosition();
			double x = m_chartRaw.GetAxis().GetBottom().CalcPosPoint(long(point.GetX()));
		
			// 2. add nystagmus
			double* pEye[2] = {m_pdLeft, m_pdRight};
			unsigned long idx = this->m_analyzeSaccade[LR].addSaccade((unsigned long)(x*FRAMERATE), pEye[LR], m_pdTarget, m_ulDataCount);		
			
			
			if(idx != -1)
			{
				// 3. List 및 chart update
				showAnalyze();
				fillListSummary();
				fillListDetail();

				// 4. add 되었으면 edit 모드로
				selectListDetail(idx);
				this->selectSaccadeOnChart(idx);	
			}
		
		}
	}
	else
	{
		unsigned long idx = getClickedSaccade();
		if(idx != -1)
		{
			selectListDetail(idx);
			selectSaccadeOnChart(idx);	
		}
	}
}

void CDlgAnalSaccade::OnDblClickTchart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	unsigned long idx = getClickedSaccade();
	if(idx != -1)
	{
		selectListDetail(idx);
		selectSaccadeOnChart(idx);
		this->enterEditMode(true, idx);
	}
	
}


unsigned long CDlgAnalSaccade::getClickedSaccade()
{
	// mouse 위치에서 가장 가까운 pursuit를 찾아 index를 리턴한다.
	unsigned long idx = -1;
	int LR = this->getRadioLR();

	if(LR != BOTH)
	{
		CTChart* pCharts[] = {&m_chartRaw};

		CRect rect;
		CPoint p;
		::GetCursorPos(&p);
		
		CTChart* pTChart = NULL;
		for(int i=0; i<1; i++)
		{
			pCharts[i]->GetWindowRect(&rect);
			if(rect.PtInRect(p))
				pTChart = pCharts[i];
		}
		
		if(pTChart)
		{
			CTeePoint2D point = pTChart->GetMousePosition();
			double x = pTChart->GetAxis().GetBottom().CalcPosPoint(long(point.GetX()));

			
			idx = this->m_analyzeSaccade[LR].getClosestSaccade(x);
		}
		
	}
	return idx;
}

void CDlgAnalSaccade::selectListDetail(unsigned long idx)
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


void CDlgAnalSaccade::OnClickTchartSmall()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	unsigned long idx = getClickedSaccadeSmall();
	if(idx != -1)
	{
		selectListDetail(idx);
		selectSaccadeOnChart(idx);	
	}

	
}

void CDlgAnalSaccade::OnDblClickTchartSmall()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	unsigned long idx = getClickedSaccadeSmall();
	if(idx != -1)
	{
		selectListDetail(idx);
		selectSaccadeOnChart(idx);
		this->enterEditMode(true, idx);
	}
	
}

unsigned long CDlgAnalSaccade::getClickedSaccadeSmall()
{
	unsigned long idx = -1;
	int LR = this->getRadioLR();

	if(LR != BOTH)
	{
		CTChart* pCharts[] = {&m_chartPeakVel, &m_chartAccuracy, &m_chartLatency};
		CRect rect;
		CPoint p;
		::GetCursorPos(&p);
		
		CTChart* pTChart = NULL;
		for(int i=0; i<3; i++)
		{
			pCharts[i]->GetWindowRect(&rect);
			if(rect.PtInRect(p))
				pTChart = pCharts[i];
		}
		
		if(pTChart)
		{
			idx = pTChart->GetTools().GetItems(1).GetAsNearest().GetPoint();
		}
	}
	return idx;
}

void CDlgAnalSaccade::analyzeForPrint(structPrintSaccade* pPrintSaccade, bool forPrint)
{
	if(forPrint)
	{
		//print 용이 아닐 경우 새로 load하지 말아야 한다.
		this->loadData();

		//먼저 ANALYZE하고 ANALYZE FILE이 있으면 결과만 덮어쓴다.
		analyzeThis(BOTH);
		loadAnalyzeData();
	}


	
	
	memset(pPrintSaccade, 0, sizeof(structPrintSaccade)*2);
	
	
	double latency[2], accuracy[2], peakVel[2];
	int numSaccade[2];
	latency[0] = accuracy[0] = peakVel[0] =  latency[1] = accuracy[1] = peakVel[1] = 0;
	numSaccade[0] = numSaccade[1] = 0;

	int bi = 0;
	POSITION pos;
	int count = 0;

	//LEFT EYE
	bi = LEFT;
	pos = this->m_analyzeSaccade[bi].m_listSaccade.GetHeadPosition();
	count = m_analyzeSaccade[bi].m_listSaccade.GetCount();

	structSaccade* pSaccade = NULL;
	for(int i=0; i<count; i++)
	{
		pSaccade = m_analyzeSaccade[bi].m_listSaccade.GetNext(pos);
		if((pSaccade) && (pSaccade->targetLeap > 0))	//LEFT
		{
			pPrintSaccade[bi].latency[LEFT] += pSaccade->latency*(1000.0/FRAMERATE);
			pPrintSaccade[bi].accuracy[LEFT] += pSaccade->accuracy;
			pPrintSaccade[bi].peakVel[LEFT] += pSaccade->peakVel;
			numSaccade[LEFT] += 1;
		}
		else
		{
			pPrintSaccade[bi].latency[RIGHT] += pSaccade->latency*(1000.0/FRAMERATE);
			pPrintSaccade[bi].accuracy[RIGHT] += pSaccade->accuracy;
			pPrintSaccade[bi].peakVel[RIGHT] += pSaccade->peakVel;
			numSaccade[RIGHT] += 1;
		}
	}
	if(numSaccade[LEFT])
	{
		pPrintSaccade[bi].latency[LEFT] /= numSaccade[LEFT];
		pPrintSaccade[bi].accuracy[LEFT] /= numSaccade[LEFT];
		pPrintSaccade[bi].peakVel[LEFT] /= numSaccade[LEFT];
	}
	if(numSaccade[RIGHT])
	{
		pPrintSaccade[bi].latency[RIGHT] /= numSaccade[RIGHT];
		pPrintSaccade[bi].accuracy[RIGHT] /= numSaccade[RIGHT];
		pPrintSaccade[bi].peakVel[RIGHT] /= numSaccade[RIGHT];
	
	}


	//RIGHT EYE
	numSaccade[0] = numSaccade[1] = 0;
	bi = RIGHT;
	pos = this->m_analyzeSaccade[bi].m_listSaccade.GetHeadPosition();
	count = m_analyzeSaccade[bi].m_listSaccade.GetCount();

	pSaccade = NULL;
	for(int i=0; i<count; i++)
	{
		pSaccade = m_analyzeSaccade[bi].m_listSaccade.GetNext(pos);
		if((pSaccade) && (pSaccade->targetLeap > 0))	//LEFT
		{
			pPrintSaccade[bi].latency[LEFT] += pSaccade->latency*(1000.0/FRAMERATE);
			pPrintSaccade[bi].accuracy[LEFT] += pSaccade->accuracy;
			pPrintSaccade[bi].peakVel[LEFT] += pSaccade->peakVel;
			numSaccade[LEFT] += 1;
		}
		else
		{
			pPrintSaccade[bi].latency[RIGHT] += pSaccade->latency*(1000.0/FRAMERATE);
			pPrintSaccade[bi].accuracy[RIGHT] += pSaccade->accuracy;
			pPrintSaccade[bi].peakVel[RIGHT] += pSaccade->peakVel;
			numSaccade[RIGHT] += 1;
		}
	}
	if(numSaccade[LEFT])
	{
		pPrintSaccade[bi].latency[LEFT] /= numSaccade[LEFT];
		pPrintSaccade[bi].accuracy[LEFT] /= numSaccade[LEFT];
		pPrintSaccade[bi].peakVel[LEFT] /= numSaccade[LEFT];
	}
	if(numSaccade[RIGHT])
	{
		pPrintSaccade[bi].latency[RIGHT] /= numSaccade[RIGHT];
		pPrintSaccade[bi].accuracy[RIGHT] /= numSaccade[RIGHT];
		pPrintSaccade[bi].peakVel[RIGHT] /= numSaccade[RIGHT];
	
	}



}


void CDlgAnalSaccade::exportToCSV()
{
	CString datafile;
	CString filter="CSV data file (*.csv)|*.csv||";
	CFileDialog fileDlg(FALSE, "csv", "*.csv", NULL, filter, NULL);
	fileDlg.m_ofn.lpstrTitle="Input the CSV data filename.";

	if(fileDlg.DoModal() == IDOK) 
	{

		CWaitCursor sandwatch;


		datafile = fileDlg.GetPathName();
		CString strFolder = datafile.Left(datafile.ReverseFind('\\'));;
		CString strFileName = fileDlg.GetFileTitle();
		CString strExtName = fileDlg.GetFileExt();


		//left
        CString fname = strFolder + "\\" +strFileName + "Left." + strExtName ;
		exportToCSV(fname, &(m_analyzeSaccade[0].m_listSaccade));
		
		//right
		fname = strFolder + "\\" +strFileName + "Right." + strExtName ;
		exportToCSV(fname, &(m_analyzeSaccade[1].m_listSaccade));
		
	}
}

bool CDlgAnalSaccade::exportToCSV(CString fname, CList<structSaccade*, structSaccade*> *pList)
{
	CFile file;
	CString str;
	if(file.Open(fname, CFile::modeCreate | CFile::modeWrite))
	{
		str = "No, Target_Displacement(°), Eye_Peak_Velocity(°/sec),  Accuracy(%), Latency(ms),  Saccade_Start(sec), Saccade_End(sec)   \n";
		file.Write(str.GetBuffer(), str.GetLength());

		int count = pList->GetCount();
		POSITION pos = pList->GetHeadPosition();;
		structSaccade* pStruct;
		for(int i=0; i<count; i++)
		{
			pStruct = pList->GetNext(pos);
			if(pStruct)
			{
				str.Format("%d, %f,  %f,  %f,  %f,  %f,  %f  \n",
					i,
					double(pStruct->targetLeap),
					double(pStruct->peakVel),
					double(pStruct->accuracy),
					double(pStruct->latency)/FRAMERATE*1000,
					double(pStruct->startIdx)/FRAMERATE,
					double(pStruct->endIdx)/FRAMERATE);
				file.Write(str.GetBuffer(), str.GetLength());
			}

		}

		file.Close();

		return true;
	}
	return false;
}
void CDlgAnalSaccade::OnBnClickedButtonSaccadePrint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strProgram;
	corGetProgramFolder(&strProgram);

	
	//form 파일명
	CString strFormFname = strProgram + "\\form\\analyze_saccade.rpt";

	//print에 필요한 파일들을 생성할 폴더
	CString strPrintTempFolder = strProgram + "\\printTemp";
	//없다면 만든다.
	if(!corFolderExist(strPrintTempFolder))
		corCreateDirectory(strPrintTempFolder);

	CRect rect;

	//peak vel chart export
	CString fname = strPrintTempFolder +"\\peak_velocity.bmp";
	unsigned long color = m_chartPeakVel.GetPanel().GetColor();
	m_chartPeakVel.GetWindowRect(&rect);
	m_chartPeakVel.GetPanel().SetColor(RGB(255, 255, 255));
	m_chartPeakVel.GetExport().GetAsBMP().SetWidth(rect.Width());
	m_chartPeakVel.GetExport().GetAsBMP().SetHeight(rect.Height());
	m_chartPeakVel.GetExport().GetAsBMP().SaveToFile(fname);
	m_chartPeakVel.GetPanel().SetColor(color);

	//latency chart export
	fname = strPrintTempFolder +"\\latency.bmp";
	color = m_chartLatency.GetPanel().GetColor();
	m_chartLatency.GetPanel().SetColor(RGB(255, 255, 255));
	m_chartLatency.GetWindowRect(&rect);
	this->m_chartLatency.GetExport().GetAsBMP().SetWidth(rect.Width());
	this->m_chartLatency.GetExport().GetAsBMP().SetHeight(rect.Height());
	this->m_chartLatency.GetExport().GetAsBMP().SaveToFile(fname);
	m_chartLatency.GetPanel().SetColor(color);

	//accuracy chart export
	fname = strPrintTempFolder +"\\accuracy.bmp";
	color = m_chartAccuracy.GetPanel().GetColor();
	m_chartAccuracy.GetPanel().SetColor(RGB(255, 255, 255));
	m_chartAccuracy.GetWindowRect(&rect);
	this->m_chartAccuracy.GetExport().GetAsBMP().SetWidth(rect.Width());
	this->m_chartAccuracy.GetExport().GetAsBMP().SetHeight(rect.Height());
	this->m_chartAccuracy.GetExport().GetAsBMP().SaveToFile(fname);
	m_chartAccuracy.GetPanel().SetColor(color);

	//eye movement chart export
	fname = strPrintTempFolder +"\\eye_movement.bmp";
	color = m_chartRaw.GetPanel().GetColor();
	m_chartRaw.GetPanel().SetColor(RGB(255, 255, 255));
	m_chartRaw.GetWindowRect(&rect);
	this->m_chartRaw.GetExport().GetAsBMP().SetWidth(rect.Width());
	this->m_chartRaw.GetExport().GetAsBMP().SetHeight(rect.Height());
	this->m_chartRaw.GetExport().GetAsBMP().SaveToFile(fname);
	m_chartRaw.GetPanel().SetColor(color);

	


	//get test parameters
	
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


	// insert value
	structPrintSaccade printSaccade[2];

	this->analyzeForPrint(printSaccade, false);
	double left[6] = {printSaccade[0].peakVel[0], printSaccade[0].peakVel[1],
					printSaccade[0].latency[0], printSaccade[0].latency[1],
					printSaccade[0].accuracy[0], printSaccade[0].accuracy[1]};
	double right[6] = {printSaccade[1].peakVel[0], printSaccade[1].peakVel[1],
					printSaccade[1].latency[0], printSaccade[1].latency[1],
					printSaccade[1].accuracy[0], printSaccade[1].accuracy[1]};
		
	double value[3];
	CString LL_ids[] = {"PEAK_VEL_LL", "LATENCY_LL", "ACCURACY_LL"};
	double LL_value[] = {printSaccade[0].peakVel[0], printSaccade[0].latency[0], printSaccade[0].accuracy[0]};
	for(int i=0; i<3; i++)
		insertString2Report(&strForm, LL_value[i], LL_ids[i]);
	
	CString LR_ids[] = {"PEAK_VEL_LR", "LATENCY_LR", "ACCURACY_LR"};
	double LR_value[] = {printSaccade[0].peakVel[1], printSaccade[0].latency[1], printSaccade[0].accuracy[1]};
	for(int i=0; i<3; i++)
		insertString2Report(&strForm, LR_value[i], LR_ids[i]);
	
	CString RL_ids[] = {"PEAK_VEL_RL", "LATENCY_RL", "ACCURACY_RL"};
	double RL_value[] = {printSaccade[1].peakVel[0], printSaccade[1].latency[0], printSaccade[1].accuracy[0]};
	for(int i=0; i<3; i++)
		insertString2Report(&strForm, RL_value[i], RL_ids[i]);
	
	CString RR_ids[] = {"PEAK_VEL_RR", "LATENCY_RR", "ACCURACY_RR"};
	double RR_value[] = {printSaccade[1].peakVel[1], printSaccade[1].latency[1], printSaccade[1].accuracy[1]};
	for(int i=0; i<3; i++)
		insertString2Report(&strForm, RR_value[i], RR_ids[i]);
	


	// make report file
	fname = strPrintTempFolder + "\\analyze_nystagmus.rpt";
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

void CDlgAnalSaccade::OnScrollTchartAsData()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//adjust event location
	
	m_chartRaw.Repaint();
	EU_EventFile::adjustEventLocation(&m_chartRaw, this->m_eventSeriesIdx);
}

void CDlgAnalSaccade::OnZoomTchartAsData()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//adjust event location
	
	m_chartRaw.Repaint();
	EU_EventFile::adjustEventLocation(&m_chartRaw, this->m_eventSeriesIdx);
}

void CDlgAnalSaccade::OnUndoZoomTchartAsData()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//adjust event location
	
	m_chartRaw.Repaint();
	EU_EventFile::adjustEventLocation(&m_chartRaw, this->m_eventSeriesIdx);
}

void CDlgAnalSaccade::OnBnClickedCheckRemoveOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double offset[2] = {m_pdLeft[0], m_pdRight[0]};
	if( ((CButton*)GetDlgItem(IDC_CHECK_REMOVE_OFFSET))->GetCheck() == BST_CHECKED)
	{
		offset[0] = -1*m_pdLeft[0];
		offset[1] = -1*m_pdRight[0];
	}

	//eye movement를 옮겨준다..
	unsigned long count = m_chartRaw.Series(LEFT).GetCount();
	double value = 0;
	CValueList leftEye = m_chartRaw.Series(LEFT).GetYValues();
	CValueList rightEye = m_chartRaw.Series(RIGHT).GetYValues();
	for(unsigned long i=0; i<count; i++)
	{
		value = leftEye.GetValue(i);
		leftEye.SetValue(i,  value + offset[0]);
		value = rightEye.GetValue(i);
		rightEye.SetValue(i, value + offset[1]);
	}

	//saccade result도 옮겨준다.
	// left 
	count = m_chartRaw.Series(LEFT+3).GetCount();
	CValueList leftSaccade = m_chartRaw.Series(LEFT+3).GetYValues();
	for(unsigned long i=0; i<count; i++)
	{
		//target은 움직이지 않는다.
		if( i%3 != 0)
		{
			value = leftSaccade.GetValue(i);
			leftSaccade.SetValue(i, value + offset[0]);
		}
	}
	//right
	count = m_chartRaw.Series(RIGHT+3).GetCount();
	CValueList rightSaccade = m_chartRaw.Series(RIGHT+3).GetYValues();
	for(unsigned long i=0; i<count; i++)
	{
		if( i%3 != 0)
		{
			value = rightSaccade.GetValue(i);
			rightSaccade.SetValue(i, value + offset[1]);
		}
	}

}

/*
void CDlgAnalSaccade::OnBeforeDrawSeriesTchartAsData()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EU_EventFile::adjustEventLocation(&m_chartRaw, this->m_eventSeriesIdx);
}*/
