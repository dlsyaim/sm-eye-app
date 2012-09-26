// DlgAnalCalibration.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgAnalCalibration.h"

#include "corwlutil.h"
#include "eu_testfilev2.h"
#include "ED_TestProtocole.h"

#include "corwlDefines.h"
#include ".\dlganalcalibration.h"

#include "includeTChart.h"
#include ".\tchart\toollist.h"
#include ".\tchart\colorlinetool.h"
#include ".\tchart\annotationtool.h"
#include "eu_eventfile.h"


// CDlgAnalCalibration 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAnalCalibration, CDialog)
CDlgAnalCalibration::CDlgAnalCalibration(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalCalibration::IDD, pParent)
{
	m_pdLH = m_pdLV = m_pdRH = m_pdRV = m_pdStiH = m_pdStiV = NULL;
	m_ulDataCount = 0;
	memset(m_ulSeperateTime, 0, sizeof(double)*5);
	m_bCustom = false;

	m_bCalibrationUsingGyro = false;

	m_eventSeriesIdx = -1;
}

CDlgAnalCalibration::~CDlgAnalCalibration()
{
	DEL(m_pdLH);
	DEL(m_pdLV);
	DEL(m_pdRH);
	DEL(m_pdRV);
	DEL(m_pdStiH);
	DEL(m_pdStiV);

	m_ulDataCount = 0;
}

void CDlgAnalCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_AC_LEFT_RAW2, m_chartLeftRaw);
	DDX_Control(pDX, IDC_TCHART_AC_LEFT_CALIB, m_chartLeftCalib);
	DDX_Control(pDX, IDC_TCHART_AC_RIGHT_RAW, m_chartRightRaw);
	DDX_Control(pDX, IDC_TCHART_AC_RIGHT_CALIB, m_chartRightCalib);
	DDX_Control(pDX, IDC_TCHART_AC_H, m_chartH);
	DDX_Control(pDX, IDC_TCHART_AC_V, m_chartV);
}



BEGIN_MESSAGE_MAP(CDlgAnalCalibration, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_AC_SAVE, OnBnClickedButtonAcSave)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_CALIBRATION, OnBnClickedCheckCustomCalibration)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_CALIBRATION_ANALYZE, OnBnClickedButtonCustomCalibrationAnalyze)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_CALIBRATION_SETTING, OnBnClickedButtonCustomCalibrationSetting)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAnalCalibration 메시지 처리기입니다.


void CDlgAnalCalibration::loadData()
{
	EU_TestFileV2 testFile;
	
	
	if(testFile.open(this->m_strTestFile, &m_testInfo, false))
	{
		testFile.readTI(&m_testInfo);
		m_ulDataCount = m_testInfo.m_lEyeDataCount;
		if(m_ulDataCount)
		{
			this->allocVariable();

			// 4-point calibration 
			if((this->m_testInfo.m_stiModeCommand == ED_TestProtocole::getStiModeCommand(STI_CALIBRATION)) && (m_testInfo.m_stiSubmode != ED_TestProtocole::getStiSubmodeNum(STI_CALIBRATION)-1) )
				testFile.readTestData(false, this->m_pdLH, m_pdRH,		//H
													m_pdLV, m_pdRV,		//V
													0, 0,				//t
													0, 0,				//R
													0, 0, 0,			//gyro
													0, 0,				//acc
													this->m_pdStiH, m_pdStiV);
			// gyro calibration
			else	
				testFile.readTestData(false, this->m_pdLH, m_pdRH,
													m_pdLV, m_pdRV,
													0, 0,
													0, 0,
													0, m_pdStiV, m_pdStiH,
													0, 0,
													0, 0);

		}
	}    
}

void CDlgAnalCalibration::allocVariable()
{
	if(m_ulDataCount)
	{
		m_pdLH = new double[m_ulDataCount];
		m_pdLV = new double[m_ulDataCount];
		m_pdRH = new double[m_ulDataCount];
		m_pdRV = new double[m_ulDataCount];
		m_pdStiH = new double[m_ulDataCount];
		m_pdStiV = new double[m_ulDataCount];
	}
}
BOOL CDlgAnalCalibration::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ShowWindow(SW_SHOWMAXIMIZED);
	layoutCtrls();
	
	this->loadData();


	analyzeThis();
	//loadAnalyzeData();

	showResult();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgAnalCalibration::analyzeThis()
{
	if(this->m_bCalibrationUsingGyro)
	{
		m_Calibration[0].analyzeUsingGyro(m_pdLH, m_pdLV, m_ulDataCount, m_pdStiH, m_pdStiV, m_ulDataCount);
		m_Calibration[1].analyzeUsingGyro(m_pdRH, m_pdRV, m_ulDataCount, m_pdStiH, m_pdStiV, m_ulDataCount);
	}
	else if(((CButton*)GetDlgItem(IDC_CHECK_CUSTOM_CALIBRATION))->GetCheck())
	{
		//check seperation time in onder
		for(int i=0; i<4; i++)
		{
			if(m_ulSeperateTime[i] >= m_ulSeperateTime[i+1])
			{
				::AfxMessageBox("Seperation strips must be in order");
				return;
			}
		}

		unsigned long seperateIdx[5];

		for(int i=0; i<5; i++)
		{
			seperateIdx[i] = this->m_ulSeperateTime[i]*FRAMERATE;
		}

		double targetH[6];
		double targetV[6];

		memset(targetH, 0, sizeof(double)*6);
		memset(targetV, 0, sizeof(double)*6);

		//seperateIdx

		//target position
		double lr = this->m_dlgCustomCalibrationSetting.m_dLeftRightDegree;
		double td = this->m_dlgCustomCalibrationSetting.m_dTopBottomDegree;

		targetH[1] = lr;
		targetH[2] = -1*lr;
		targetV[3] = td;
		targetV[4] = -1*td;

		//custom calibrate
		m_Calibration[0].analyzeCustom(m_pdLH, m_pdLV, m_ulDataCount, seperateIdx, 5, targetH, targetV, 6);
		m_Calibration[1].analyzeCustom(m_pdRH, m_pdRV, m_ulDataCount, seperateIdx, 5, targetH, targetV, 6);
	}
	else
	{
		
		m_Calibration[0].analyze(m_pdLH, m_pdLV, m_pdStiH, m_pdStiV, m_ulDataCount);
			
		m_Calibration[1].analyze(m_pdRH, m_pdRV, m_pdStiH, m_pdStiV, m_ulDataCount);
	}
		
}

void CDlgAnalCalibration::layoutCtrls()
{
	double gap = 0.1/10;
	double button_h = gap*4;
	double button_w = button_h*3;
	CRect rect;
	this->GetClientRect(&rect);
	
	//static
	corLocateWindow(GetDlgItem(IDC_STATIC_AC_MODEL_COEFF_H), gap, gap, 0.4+gap, 0.1, rect);
	//corLocateWindow(GetDlgItem(IDC_STATIC_AC_MODEL_COEFF_V), 0.4+18*gap, gap, 0.4+gap, 0.1, rect);
	corLocateWindow(GetDlgItem(IDC_STATIC_AC_MODEL_COEFF_V), 1-0.4-gap, gap, 0.4+gap, 0.1, rect);

	//custom ctrls
	corLocateWindow(GetDlgItem(IDC_CHECK_CUSTOM_CALIBRATION), 0.44, gap, 0.1, button_h, rect);
	corLocateWindow(GetDlgItem(IDC_BUTTON_CUSTOM_CALIBRATION_SETTING), 0.44, gap+button_h+gap, 0.1, button_h, rect);
	corLocateWindow(GetDlgItem(IDC_BUTTON_CUSTOM_CALIBRATION_ANALYZE), 0.44, gap+2*(button_h+gap), 0.1, button_h, rect);
	
	


	//xy plot
	corLocateWindow((CWnd*)(&m_chartLeftRaw), gap, 0.1+2*gap, 0.2, 0.2, rect);
	corLocateWindow((CWnd*)(&m_chartLeftCalib), 0.2 + 2*gap, 0.1+2*gap, 0.2, 0.2, rect);
	corLocateWindow((CWnd*)(&m_chartRightRaw), 2*0.2+18*gap, 0.1+2*gap, 0.2, 0.2, rect);
	corLocateWindow((CWnd*)(&m_chartRightCalib), 3*0.2+19*gap, 0.1+2*gap, 0.2, 0.2, rect);

	corLocateWindow((CWnd*)(&m_chartH), gap, 0.3+3*gap, 1-2*gap, 0.3, rect);
	corLocateWindow((CWnd*)(&m_chartV), gap, 0.6+4*gap, 1-2*gap, 0.3, rect);


	//button
	corLocateWindow(GetDlgItem(IDC_BUTTON_AC_SAVE), gap, 1-gap-button_h, button_w*2, button_h, rect);
	corLocateWindow(GetDlgItem(IDCANCEL),1-button_w*2-2*gap, 1-gap-button_h, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDOK),1-button_w-gap, 1-gap-button_h, button_w, button_h, rect);



}

void CDlgAnalCalibration::initTChart()
{
	CTChart* bufCharts[6];

	bufCharts[0] = &m_chartLeftRaw;
	bufCharts[1] = &m_chartLeftCalib;
	bufCharts[2] = &m_chartRightRaw;
	bufCharts[3] = &m_chartRightCalib;
	bufCharts[4] = &m_chartH;
	bufCharts[5] = &m_chartV;

	CString strTitles[6] = {"Left Raw", "Left Calibrated", "Right Raw", "Right Calibrated", 
							"Horizontal Position", "Vertical Position"};

	if(this->m_bCalibrationUsingGyro)
	{
		strTitles[4] = "Horizontal Velocity";
		strTitles[5] = "Vertical Velocity";
	}

	//m_chartLeftCalib.AddSeries(scPoint);

	for(int i=0; i<6; i++)
	{
		bufCharts[i]->ClearChart();
		bufCharts[i]->GetAspect().SetView3D(false);
		bufCharts[i]->GetZoom().SetEnable(true);
		bufCharts[i]->GetAspect().SetZoom(tzdBoth);
		bufCharts[i]->GetLegend().SetAlignment(laBottom);		

		bufCharts[i]->GetHeader().GetText().SetText(strTitles[i]);
		bufCharts[i]->SetAutoRepaint(false);
	}


	//xy plot
	COLORREF cl[4] = {clPlot0, clPlot1, clPlot0, clPlot1};
	for(int i=0; i<4; i++)
	{
		bufCharts[i]->AddSeries(scPoint);
		bufCharts[i]->Series(0).SetColor(cl[i]);
		bufCharts[i]->Series(0).GetAsPoint().SetTransparency(80);
		bufCharts[i]->Series(0).GetAsPoint().GetPointer().SetStyle(psCircle);
		bufCharts[i]->Series(0).GetAsPoint().GetPointer().SetHorizontalSize(2);
		bufCharts[i]->Series(0).GetAsPoint().GetPointer().SetVerticalSize(2);
		

		bufCharts[i]->GetLegend().SetVisible(false);
		
		if(i%2 == 0)	//raw
		{
			bufCharts[i]->GetAxis().GetBottom().SetMinMax(0,320);
			bufCharts[i]->GetAxis().GetBottom().GetTitle().SetCaption("H(Pixel)");
			bufCharts[i]->GetAxis().GetLeft().SetMinMax(0,240);
			bufCharts[i]->GetAxis().GetLeft().GetTitle().SetCaption("V(Pixel0)");

		}
		else	//calibrated
		{
			bufCharts[i]->GetAxis().GetBottom().SetMinMax(-30, 30);
			bufCharts[i]->GetAxis().GetBottom().GetTitle().SetCaption("H(Degree)");
			bufCharts[i]->GetAxis().GetLeft().SetMinMax(-30, 30);
			bufCharts[i]->GetAxis().GetLeft().GetTitle().SetCaption("V(Degree)");
		}
	}

	// chart with time
	double yAxisMinMax = m_bCalibrationUsingGyro ? 300 : 30;
	CString yAxisUnit = m_bCalibrationUsingGyro ? "Degree/Sec" : "Degree";
	for(int i=4; i<6; i++)
	{
		bufCharts[i]->GetLegend().SetVisible(true);
		bufCharts[i]->GetAxis().GetLeft().SetMinMax(-1*yAxisMinMax, yAxisMinMax);
		bufCharts[i]->GetAxis().GetLeft().GetTitle().SetCaption(yAxisUnit);

		

		bufCharts[i]->AddSeries(scLine);
		bufCharts[i]->AddSeries(scLine);
		bufCharts[i]->AddSeries(scLine);

		bufCharts[i]->Series(0).SetName("Left");
		bufCharts[i]->Series(1).SetName("Right");
		bufCharts[i]->Series(2).SetName("Target");

		bufCharts[i]->Series(0).SetColor(COLOR_LEFT);
		bufCharts[i]->Series(1).SetColor(COLOR_RIGHT);
		bufCharts[i]->Series(2).SetColor(COLOR_STI);

		//colorline과 annotation 추가
		for(int ax = 0; ax<5; ax++)
		{
			DWORD line = bufCharts[i]->GetTools().Add(tcColorLine);
			VARIANT varAxis;
			varAxis.vt=VT_DISPATCH;
			CAxis axBottom = bufCharts[i]->GetAxis().GetBottom();
			varAxis.pdispVal=axBottom;
			bufCharts[i]->GetTools().GetItems(line).GetAsColorLine().SetAxis(varAxis);
			bufCharts[i]->GetTools().GetItems(line).GetAsColorLine().SetDrawBehind(TRUE);
			bufCharts[i]->GetTools().GetItems(line).GetAsColorLine().SetValue(0);
			bufCharts[i]->GetTools().GetItems(line).GetAsColorLine().SetAllowDrag(TRUE);
			bufCharts[i]->GetTools().GetItems(line).SetActive(false);		
		}
		for(int ax = 0; ax<5; ax++)
		{
			DWORD ann = bufCharts[i]->GetTools().Add(tcAnnotate);
			CString str;
			str.Format("%d", ax);
			bufCharts[i]->GetTools().GetItems(ann).GetAsAnnotation().SetText(str);
			bufCharts[i]->GetTools().GetItems(ann).SetActive(false);
		}
	}	

	// init event series
	m_eventSeriesIdx = EU_EventFile::initEventSeries(&m_chartH);
	m_eventSeriesIdx = EU_EventFile::initEventSeries(&m_chartV);

	
}

void CDlgAnalCalibration::showResult()
{
	//칼리브레이션은 세가지 방법이 있다.
	// LED 바를 이용한 5 point 방법
	// custom position을 이용한 5 point 
	// gyro data를 이용하는 방법
	initTChart();

	double pixel[2];
	double degree[2];

	if(!m_bCalibrationUsingGyro)
	{	
		// xy plot
		double t;
		for(unsigned long i=0; i<this->m_ulDataCount; i++)
		{
			t = double(i)/FRAMERATE;
			////////////////////////////////////////
			//Left
			pixel[0] = m_pdLH[i]; pixel[1] = m_pdLV[i];
			m_Calibration[0].pixel2degree(pixel, degree);

			
			//XY plot
			m_chartLeftRaw.Series(0).AddXY(pixel[0], pixel[1], "", clPlot0);		
			m_chartLeftCalib.Series(0).AddXY(degree[0], degree[1], "", clPlot0);

			// time plot (left)
			m_chartH.Series(0).AddXY(t, degree[0], "", 0);
			m_chartV.Series(0).AddXY(t, degree[1], "", 0);
			


			///////////////////////////////////////////////
			//Right
			pixel[0] = m_pdRH[i]; pixel[1] = m_pdRV[i];
			m_Calibration[1].pixel2degree(pixel, degree);

		
			//XY plot
			m_chartRightRaw.Series(0).AddXY(pixel[0], pixel[1], "", clPlot1);		
			m_chartRightCalib.Series(0).AddXY(degree[0], degree[1], "", clPlot1);


			// time plot (right)
			m_chartH.Series(1).AddXY(t, degree[0], "", 0);		
			m_chartV.Series(1).AddXY(t, degree[1], "", 0);
			
			//stimulation plot
			m_chartH.Series(2).AddXY(t, this->m_pdStiH[i], "", 0);
			m_chartV.Series(2).AddXY(t, this->m_pdStiV[i], "", 0);
			
		}

		

			

		// custom이라면
		// Target Series를 새로 그리고
		// seperation line을 그려준다.
		if(m_bCustom)
		{
			
			//colorline과 annotation을 visible
			for(int i=0; i<5; i++)
			{
				m_chartH.GetTools().GetItems(i).SetActive(true);
				m_chartV.GetTools().GetItems(i).SetActive(true);

				m_chartH.GetTools().GetItems(i+5).SetActive(true);
				m_chartV.GetTools().GetItems(i+5).SetActive(true);

			}

		}
		//seperation 위치를 colorline으로 표시
		long left, top;
		for(int i=0; i<5; i++)
		{
			//H chart의 colorline의 위치
			this->m_chartH.GetTools().GetItems(i).GetAsColorLine().SetValue(m_ulSeperateTime[i]);
			//annotation 위치
			left = m_chartH.GetAxis().GetBottom().CalcXPosValue(m_ulSeperateTime[i]);
			top = m_chartH.GetAxis().GetLeft().CalcYPosValue(m_chartH.GetAxis().GetLeft().GetMaximum());
			CAnnotationTool tool = m_chartH.GetTools().GetItems(i+5).GetAsAnnotation();
			m_chartH.GetTools().GetItems(i+5).GetAsAnnotation().SetLeft(left);
			m_chartH.GetTools().GetItems(i+5).GetAsAnnotation().SetTop(top);

			//V chart의 colorline 위치
			this->m_chartV.GetTools().GetItems(i).GetAsColorLine().SetValue(m_ulSeperateTime[i]);
			left = m_chartV.GetAxis().GetBottom().CalcXPosValue(m_ulSeperateTime[i]);
			top = m_chartV.GetAxis().GetLeft().CalcYPosValue(m_chartV.GetAxis().GetLeft().GetMaximum());
			m_chartV.GetTools().GetItems(i+5).GetAsAnnotation().SetLeft(left);
			m_chartV.GetTools().GetItems(i+5).GetAsAnnotation().SetTop(top);
		}
	}
	else
	{
		//gyro를 이용한 칼리브레이션일 경우
		double t;
		double prevPosH[2];		//left right horizontal
		double prevPosV[2];		//left right vertical
		
		//get left eye position
		pixel[0] = m_pdLH[0]; pixel[1] = m_pdLV[0];
		m_Calibration[0].pixel2degree(pixel, degree);
		prevPosH[0] = degree[0]; prevPosV[0] = degree[1];

		//get right eye position
		pixel[0] = m_pdLH[1]; pixel[1] = m_pdLV[1];
		m_Calibration[1].pixel2degree(pixel, degree);
		prevPosH[1] = degree[0]; prevPosV[1] = degree[1];

		

		for(unsigned long i=1; i<this->m_ulDataCount; i++)
		{
			t = double(i)/FRAMERATE;
			////////////////////////////////////////
			//Left
			pixel[0] = m_pdLH[i]; pixel[1] = m_pdLV[i];
			m_Calibration[0].pixel2degree(pixel, degree);

			// time plot (left)
			m_chartH.Series(0).AddXY(t, (degree[0]-prevPosH[0])*FRAMERATE, "", 0);
			m_chartV.Series(0).AddXY(t, (degree[1]-prevPosV[0])*FRAMERATE, "", 0);
			
			//update prev value
			prevPosH[0] = degree[0];
			prevPosV[0] = degree[1];

			////////////////////////////////////////
			//Right
			pixel[0] = m_pdRH[i]; pixel[1] = m_pdRV[i];
			m_Calibration[1].pixel2degree(pixel, degree);

			// time plot (right)
			m_chartH.Series(1).AddXY(t, (degree[0]-prevPosH[1])*FRAMERATE, "", 0);
			m_chartV.Series(1).AddXY(t, (degree[1]-prevPosV[1])*FRAMERATE, "", 0);

			//update prev value
			prevPosH[1] = degree[0];
			prevPosV[1] = degree[1];


			////////////////////////////////////////
			//Target
			m_chartH.Series(2).AddXY(t, this->m_pdStiH[i], "", 0);
			m_chartV.Series(2).AddXY(t, this->m_pdStiV[i], "", 0);
		
		}

	}


	//event file 을 load한다.
	CString fname = m_strTestFile;
	fname.Replace(".emd", ".evt");
	EU_EventFile::loadEventFile(fname, &m_chartH, this->m_eventSeriesIdx, false);
	EU_EventFile::loadEventFile(fname, &m_chartV, this->m_eventSeriesIdx, false);



	//repaint
	m_chartLeftRaw.SetAutoRepaint(true);
	m_chartLeftCalib.SetAutoRepaint(true);
	m_chartRightRaw.SetAutoRepaint(true);
	m_chartRightCalib.SetAutoRepaint(true);
	m_chartH.SetAutoRepaint(true);
	m_chartV.SetAutoRepaint(true);

	m_chartLeftRaw.Repaint();
	m_chartLeftCalib.Repaint();
	m_chartRightRaw.Repaint();
	m_chartRightCalib.Repaint();
	m_chartH.Repaint();
	m_chartV.Repaint();


	//fill text
	CString str;
	str.Format("\r\n Left \r\n %s", this->m_Calibration[0].getModelStr());
	GetDlgItem(IDC_STATIC_AC_MODEL_COEFF_H)->SetWindowText(str);
	str.Format("\r\n Right \r\n %s", this->m_Calibration[1].getModelStr());
	GetDlgItem(IDC_STATIC_AC_MODEL_COEFF_V)->SetWindowText(str);
	
}


BEGIN_EVENTSINK_MAP(CDlgAnalCalibration, CDialog)
ON_EVENT(CDlgAnalCalibration, IDC_TCHART_AC_H, 27, OnZoomTchartAc, VTS_NONE)
ON_EVENT(CDlgAnalCalibration, IDC_TCHART_AC_H, 26, OnUndoZoomTchartAc, VTS_NONE)
ON_EVENT(CDlgAnalCalibration, IDC_TCHART_AC_H, 24, OnScrollTchartAc, VTS_NONE)

ON_EVENT(CDlgAnalCalibration, IDC_TCHART_AC_V, 27, OnZoomTchartAc, VTS_NONE)
ON_EVENT(CDlgAnalCalibration, IDC_TCHART_AC_V, 26, OnUndoZoomTchartAc, VTS_NONE)
ON_EVENT(CDlgAnalCalibration, IDC_TCHART_AC_V, 24, OnScrollTchartAc, VTS_NONE)


ON_EVENT(CDlgAnalCalibration, IDC_TCHART_AC_H, 222, OnColorLineToolEndDragLineTchartAcH, VTS_NONE)
ON_EVENT(CDlgAnalCalibration, IDC_TCHART_AC_V, 222, OnColorLineToolEndDragLineTchartAcV, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgAnalCalibration::OnZoomTchartAc()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	CTChart* pTChart[2];
	pTChart[0] = &m_chartH;
	pTChart[1] = &m_chartV;

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

void CDlgAnalCalibration::OnUndoZoomTchartAc()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	CTChart* pTChart[2];
	pTChart[0] = &m_chartH;
	pTChart[1] = &m_chartV;

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
				pTChart[i]->GetScroll().SetEnable(pmBoth);
			}
			
		}
	}
}

void CDlgAnalCalibration::OnScrollTchartAc()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	CTChart* pTChart[2];
	pTChart[0] = &m_chartH;
	pTChart[1] = &m_chartV;

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

			//pTChart[i]->Repaint();			
		}
	}
}

void CDlgAnalCalibration::OnBnClickedButtonAcSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString filter="Eye Movement Calibration file (*.emc)|*.emc";
	CFileDialog fileDlg(FALSE, "emc", "*.emc", NULL, filter, NULL);
	fileDlg.m_ofn.lpstrTitle="Input the Eye Movement Calibration data filename.";

	if(fileDlg.DoModal() == IDOK) 
	{

		CString filename = fileDlg.GetPathName();
		
		CFile file;
		if(file.Open(filename, CFile::modeCreate | CFile::modeWrite))
		{
			unsigned int ver = 1;
			
			file.Write(&ver, sizeof(ver));
			//left
			file.Write(this->m_Calibration[0].m_dHCoeff, sizeof(double)*6);
			file.Write(this->m_Calibration[0].m_dVCoeff, sizeof(double)*6);
			file.Write(&(m_Calibration[0].m_dRCoeff), sizeof(double));
			//right
			file.Write(this->m_Calibration[1].m_dHCoeff, sizeof(double)*6);
			file.Write(this->m_Calibration[1].m_dVCoeff, sizeof(double)*6);
			file.Write(&(m_Calibration[1].m_dRCoeff), sizeof(double));

			file.Close();


		}


	}

}

void CDlgAnalCalibration::OnBnClickedCheckCustomCalibration()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(((CButton*)GetDlgItem(IDC_CHECK_CUSTOM_CALIBRATION))->GetCheck())
	{
		GetDlgItem(IDC_BUTTON_CUSTOM_CALIBRATION_SETTING)->EnableWindow(true);
		m_bCustom = true;
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CUSTOM_CALIBRATION_SETTING)->EnableWindow(false);
		m_bCustom = false;
	}

	for(int i=0; i<5; i++)
	{
		//colorline
		this->m_chartH.GetTools().GetItems(i).SetActive(m_bCustom);
		this->m_chartV.GetTools().GetItems(i).SetActive(m_bCustom);
		//annotation
		this->m_chartH.GetTools().GetItems(i+5).SetActive(m_bCustom);
		this->m_chartV.GetTools().GetItems(i+5).SetActive(m_bCustom);

	}
	
}

void CDlgAnalCalibration::OnBnClickedButtonCustomCalibrationAnalyze()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	analyzeThis();
	showResult();
}

void CDlgAnalCalibration::OnBnClickedButtonCustomCalibrationSetting()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_dlgCustomCalibrationSetting.DoModal();
}

void CDlgAnalCalibration::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	saveAnalyzeData();

	OnOK();
}


bool CDlgAnalCalibration::loadAnalyzeData()
{
	bool bRtn = true;

	CString str;
	CFileStatus fs;
	//Left
	str = this->m_strTestFile;
	str.Replace(".emd", "L.ema");
	if(CFile::GetStatus(str, fs))
		this->m_Calibration[0].load(str);
	else
		bRtn = false;

	//Right
	str = this->m_strTestFile;
	str.Replace(".emd", "R.ema");
	if(CFile::GetStatus(str, fs))
		this->m_Calibration[1].load(str);
	else
		bRtn = false;

/*	if(m_Calibration[0].m_bCustom)
	{
		m_bCustom = true;

		//custom 체크 버튼을 체크하고 setting버튼 활성화
		((CButton*)GetDlgItem(IDC_CHECK_CUSTOM_CALIBRATION))->SetCheck(true);
		GetDlgItem(IDC_BUTTON_CUSTOM_CALIBRATION_SETTING)->EnableWindow(true);

		//Custom degree를 읽어온다.
		this->m_dlgCustomCalibrationSetting.m_dLeftRightDegree = m_Calibration[0].m_dCustomLRTB[0];
		this->m_dlgCustomCalibrationSetting.m_dTopBottomDegree = m_Calibration[0].m_dCustomLRTB[1];

		//seperation 구간을 읽어온다.
		memcpy(m_ulSeperateTime, m_Calibration[0].m_ulSeperateTime, sizeof(double)*5);
	}

	if(m_ulSeperateTime[0] == 0)
	{
		//seperate idx가 제대로 되어 있지 않다면
		for(int i=0; i<5; i++)
			m_ulSeperateTime[i] = this->m_ulDataCount/FRAMERATE/6*(i+1);
	}
*/
	


	return bRtn;
}


bool CDlgAnalCalibration::saveAnalyzeData()
{
	bool bRtn = true;

	CString str;
	CFileStatus fs;
	//Left
	str = this->m_strTestFile;
	str.Replace(".emd", "L.ema");
	if(!m_Calibration[0].save(str))
		bRtn = false;

	//Right
	str = this->m_strTestFile;
	str.Replace(".emd", "R.ema");
	if(!m_Calibration[1].save(str))
		bRtn = false;

	return bRtn;

}







void CDlgAnalCalibration::OnColorLineToolEndDragLineTchartAcH()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double x;
	long left;
	for(int i=0; i<5; i++)
	{
		//colorline의 위치를 찾아서
		x = this->m_chartH.GetTools().GetItems(i).GetAsColorLine().GetValue();
		left = m_chartH.GetAxis().GetBottom().CalcXPosValue(x);
		//annotation을 옮겨주고
		m_chartH.GetTools().GetItems(i+5).GetAsAnnotation().SetLeft(left);

        //Vertical chart도 옮긴다.
		this->m_chartV.GetTools().GetItems(i).GetAsColorLine().SetValue(x);
		this->m_chartV.GetTools().GetItems(i+5).GetAsAnnotation().SetLeft(left);

		this->m_ulSeperateTime[i] = x;


	}
}

void CDlgAnalCalibration::OnColorLineToolEndDragLineTchartAcV()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	double x;
	long left;
	for(int i=0; i<5; i++)
	{
		//colorline의 위치를 찾아서
		x = this->m_chartV.GetTools().GetItems(i).GetAsColorLine().GetValue();
		left = m_chartV.GetAxis().GetBottom().CalcXPosValue(x);
		//annotation을 옮겨주고
		m_chartV.GetTools().GetItems(i+5).GetAsAnnotation().SetLeft(left);

        //Vertical chart도 옮긴다.
		this->m_chartH.GetTools().GetItems(i).GetAsColorLine().SetValue(x);
		this->m_chartH.GetTools().GetItems(i+5).GetAsAnnotation().SetLeft(left);


		this->m_ulSeperateTime[i] = x;
	}
}



void CDlgAnalCalibration::setCalbrationUsingGyro(bool bGyro)
{
	this->m_bCalibrationUsingGyro = bGyro;

}
