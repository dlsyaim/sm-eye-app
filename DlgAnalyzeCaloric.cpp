// DlgAnalyzeCaloric.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgAnalyzeCaloric.h"
#include ".\dlganalyzecaloric.h"
#include "eu_testfilev2.h"
#include "DlgSelectSpontaneous.h"
#include "math.h"

#include ".\reportGenerator\reportGenerator.h"
#include "includeTChart.h"
#include "corwlUtil.h"


#define COLOR_COOL RGB(13, 10, 233)
#define COLOR_WARM RGB(233, 10, 13)

// CDlgAnalyzeCaloric 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAnalyzeCaloric, CDialog)
CDlgAnalyzeCaloric::CDlgAnalyzeCaloric(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalyzeCaloric::IDD, pParent)
{
	this->m_idxToolFixationBlock = 0;
	this->m_bExportForPrint = false;
	this->m_nExportImageNum = 0;

}

CDlgAnalyzeCaloric::~CDlgAnalyzeCaloric()
{
}

void CDlgAnalyzeCaloric::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_CALORIC_SLOW_MOVE, m_chartCaloric);
	DDX_Control(pDX, IDC_TCHART_CALORIC_BUTTERFLY, m_chartButterfly);
	DDX_Control(pDX, IDC_LIST_TESTS_RESULT, m_listTestResult);
	DDX_Control(pDX, IDC_LIST_TESTS_SUMMARY, m_listTestSummary);
}


BEGIN_MESSAGE_MAP(CDlgAnalyzeCaloric, CDialog)

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	//ON_BN_CLICKED(IDC_CHECK_USE_FIX_BLOCK, OnBnClickedCheckUseFixBlock)
	ON_BN_CLICKED(IDC_RADIO_CALORIC_LEFT_EYE, OnBnClickedRadioCaloricEye)
	ON_BN_CLICKED(IDC_RADIO_CALORIC_RIGHT_EYE, OnBnClickedRadioCaloricEye)
	ON_BN_CLICKED(IDC_CHECK_USE_FIX_BLOCK_LW, OnBnClickedCheckUseFixBlock)
	ON_BN_CLICKED(IDC_CHECK_USE_FIX_BLOCK_RC, OnBnClickedCheckUseFixBlock)
	ON_BN_CLICKED(IDC_CHECK_USE_FIX_BLOCK_LC, OnBnClickedCheckUseFixBlock)
	ON_BN_CLICKED(IDC_CHECK_USE_FIX_BLOCK_RW, OnBnClickedCheckUseFixBlock)
	
	ON_BN_CLICKED(IDC_CHECK_SPONTANEOUS_NYSTAG, OnBnClickedCheckSpontaneousNystag)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_SPONTANEOUS_TEST, OnBnClickedButtonSelectSpontaneousTest)
	ON_BN_CLICKED(IDC_BUTTON_RESET_PEAK, OnBnClickedButtonResetPeak)
	ON_BN_CLICKED(IDC_RADIO_MANUAL_PEAK_NONE, OnBnClickedRadioManualPeak)
	ON_BN_CLICKED(IDC_RADIO_MANUAL_PEAK_X, OnBnClickedRadioManualPeak)
	ON_BN_CLICKED(IDC_RADIO_MANUAL_PEAK_XY, OnBnClickedRadioManualPeak)
	ON_BN_CLICKED(IDC_BUTTON_CALORIC_PRINT, OnBnClickedButtonCaloricPrint)
END_MESSAGE_MAP()


// CDlgAnalyzeCaloric 메시지 처리기입니다.


void CDlgAnalyzeCaloric::setTestFiles(CString *pstrFname)
{
	if(!pstrFname)
		return;
	for(unsigned int i=0; i<4; i++)
	{
		this->m_analyzeCaloric.m_caloricData[i].m_strTestFname = pstrFname[i];
	}
}
BOOL CDlgAnalyzeCaloric::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ShowWindow(SW_SHOWMAXIMIZED);
	
	layoutCtrls();


	initTChart();

	//analyze 분석을 위한 parameter 데이터를 load한다	
	this->loadAnalyzeData();

	//nystagmus data를 load한다.
	loadNystagmusData(m_analyzeCaloric.m_nEyeSelect);

    

	
	//분석한다.
	analyzeThis();
			

	initListCtrls();
	
	
	//보상값
	double spon = 0;
	if(this->m_analyzeCaloric.m_bCompensateSpontaneous)
		spon = this->m_analyzeCaloric.m_dSpontaneousValue[this->m_analyzeCaloric.m_nEyeSelect];

	//nystagmus 를 그린다.
	this->drawNystagmus(spon);

	showResult();


	// print를 위한 export 라면 image 생성 후 바로 종료시킨다.
	if(this->m_bExportForPrint)
	{
		this->exportForPrint();
		OnOK();
	}

	return TRUE;  // return TRUE unless you set the focus to a control



	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgAnalyzeCaloric::layoutCtrls()
{
	//manual peak 라디오 버튼 none으로 체크
	((CButton*)GetDlgItem(IDC_RADIO_MANUAL_PEAK_NONE))->SetCheck(BST_CHECKED);
	this->m_nManualPeakSelect = 0;//None으로


	double gap = 0.01/2;
	double button_h = gap*8;
	double button_w = button_h*3;
	CRect rect;
	this->GetClientRect(&rect);

	double chartWidth = (1.-gap*2)/2;
	double chartHeight = (1.-gap*4 - button_h)/3;

	chartWidth = (1.-gap*2)/3;
	corLocateWindow(GetDlgItem(IDC_TCHART_CALORIC_BUTTERFLY), 1-chartWidth-gap, gap, chartWidth, chartHeight, rect); 

	chartWidth = (1.-gap*2)/2;
	//left
	corLocateWindow(GetDlgItem(IDC_TCHART_CALORIC_SLOW_MOVE), gap, gap*2+chartHeight, chartWidth*2, chartHeight*2,rect);


	//buttons
	corLocateWindow(GetDlgItem(IDCANCEL), 1-gap-button_w, 1-gap-button_h, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDOK), 1-2*gap-2*button_w, 1-gap-button_h, button_w, button_h, rect);
	//corLocateWindow(GetDlgItem(IDC_BUTTON_RESET_PEAK), gap, 1-gap-button_h, button_w, button_h, rect);

	corLocateWindow(GetDlgItem(IDC_BUTTON_CALORIC_PRINT), 0.5-button_w/2, 1-gap-button_h, button_w, button_h, rect);

}
void CDlgAnalyzeCaloric::initTChart()
{
	int idx = 0;

	//butterfly
	m_chartButterfly.ClearChart();
	m_chartButterfly.GetHeader().GetText().SetText("Butterfly");
	m_chartButterfly.GetHeader().GetFont().SetSize(15);
	m_chartButterfly.GetHeader().GetFont().SetBold(true);
	m_chartButterfly.GetAspect().SetView3D(false);
	m_chartButterfly.GetZoom().SetEnable(false);
	m_chartButterfly.GetLegend().SetColorWidth(30);;
	m_chartButterfly.GetLegend().SetAlignment(laBottom);
	//m_chartButterfly.GetLegend().SetVisible(false);
	//warm
	idx = m_chartButterfly.AddSeries(scLine);
	m_chartButterfly.Series(idx).SetVerticalAxis(atLeft);
	m_chartButterfly.Series(idx).SetColor(COLOR_WARM);
	m_chartButterfly.Series(idx).SetTitle(_T("WARM"));
	m_chartButterfly.Series(idx).GetPen().SetWidth(2);
	//cool
	idx = m_chartButterfly.AddSeries(scLine);
	m_chartButterfly.Series(idx).SetVerticalAxis(atRight);
	m_chartButterfly.Series(idx).SetColor(COLOR_COOL);
	m_chartButterfly.Series(idx).SetTitle(_T("COOL"));
	m_chartButterfly.Series(idx).GetPen().SetWidth(2);


	//butterfly axis
	m_chartButterfly.GetAxis().GetLeft().SetAutomatic(false);
	m_chartButterfly.GetAxis().GetLeft().SetMinMax(-80, 80);
	m_chartButterfly.GetAxis().GetRight().SetAutomatic(false);
	m_chartButterfly.GetAxis().GetRight().SetMinMax(-80, 80);
	m_chartButterfly.GetAxis().GetBottom().SetAutomatic(false);
	m_chartButterfly.GetAxis().GetBottom().SetMinMax(0,1);
	//m_chartButterfly.GetAxis().GetBottom().GetLabels().SetVisible(false);

	//set h axis labels
	m_chartButterfly.GetAxis().GetBottom().GetLabels().Add(0, _T("LEFT"));
	m_chartButterfly.GetAxis().GetBottom().GetLabels().Add(1, _T("RIGHT"));

	/*m_chartButterfly.Series(0).AddXY(0, -60, _T("LW"), 0);
	m_chartButterfly.Series(0).AddXY(1, 60, NULL, 0);
	m_chartButterfly.Series(1).AddXY(0, 60, NULL, 0);
	m_chartButterfly.Series(1).AddXY(1, -60, NULL, 0);*/
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// nystagmus chart
	CString titles[4] = {"LEFT WARM", "RIGHT COOL", "LEFT COOL", "RIGHT WARM"}; 

	m_chartCaloric.ClearChart();

	//m_chartCaloric.GetHeader().GetText().SetText(titles[i]);

	m_chartCaloric.GetAspect().SetView3D(false);
	m_chartCaloric.GetZoom().SetEnable(false);
	m_chartCaloric.GetScroll().SetEnable(false);
	m_chartCaloric.GetLegend().SetVisible(false);

	//set axis
	m_chartCaloric.GetAxis().GetLeft().SetAutomatic(false);
	m_chartCaloric.GetAxis().GetLeft().SetMinMax(-80, 80);
	m_chartCaloric.GetAxis().GetLeft().SetPositionPercent(50);
	m_chartCaloric.GetAxis().GetBottom().SetPositionPercent(50);
	m_chartCaloric.GetAxis().GetBottom().SetAutomatic(false);
	m_chartCaloric.GetAxis().GetBottom().SetMinMax(-100, 100);
	
	//set grid
	//m_chartCaloric.GetAxis().GetLeft().GetGridPen().SetVisible(false);
	m_chartCaloric.GetAxis().GetLeft().SetIncrement(10);
	m_chartCaloric.GetAxis().GetBottom().GetGridPen().SetVisible(false);
	
	
	m_chartCaloric.Repaint();
	//add series for nystagmus
	for(int i=0; i<4; i++)
	{
		m_chartCaloric.AddSeries(scPoint);
		m_chartCaloric.Series(i).GetAsPoint().GetPointer().SetHorizontalSize(3);
		m_chartCaloric.Series(i).GetAsPoint().GetPointer().SetVerticalSize(3);
		m_chartCaloric.Series(i).GetAsPoint().GetPointer().SetStyle((i%2)? psCross : psDiagCross);
		m_chartCaloric.Series(i).GetAsPoint().SetTransparency(20);
		m_chartCaloric.Series(i).GetAsPoint().GetPointer().GetPen().SetColor((i==0)||(i==3)?COLOR_WARM:COLOR_COOL);
	//	m_chartCaloric.Series(i).GetAsPoint().GetPointer().GetPen().SetWidth(2);
	}

	


	


	
	//ADD annotation
	//축의 양 끝단의 coordinate를 계산한다.
	double left, top, right, bottom;
//	int t = m_chartCaloric.Series(0).CalcXPos(0);
	left = m_chartCaloric.GetAxis().GetLeft().CalcXPosValue(m_chartCaloric.GetAxis().GetLeft().GetMinimum());
	right = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(m_chartCaloric.GetAxis().GetBottom().GetMaximum());
	top = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(m_chartCaloric.GetAxis().GetLeft().GetMaximum());
	bottom = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(m_chartCaloric.GetAxis().GetLeft().GetMinimum());
	CRect chartRect;
	m_chartCaloric.GetWindowRect(chartRect);
	for(int i=0; i<4; i++)
	{
		idx = m_chartCaloric.GetTools().Add(tcAnnotate);
		CAnnotationTool ann = m_chartCaloric.GetTools().GetItems(idx).GetAsAnnotation();
		ann.SetText(titles[i]);
		ann.GetShape().GetShadow().SetVisible(false);	//그림자 제거
		ann.GetShape().GetFont().SetSize(chartRect.Height()/40);
		ann.GetShape().GetFont().SetBold(true);
		ann.GetShape().GetFont().SetColor(((i==1)||(i==2)) ? COLOR_COOL:COLOR_WARM); //1,2는 COOL, 0,4는 WARM

		//set annotation position
		m_chartCaloric.Repaint();	//annotation의 크기를 알기 위해 그려준다.
		//axis 끝에 위치하게 한다.
		int w = ann.GetWidth();
		int h = ann.GetHeight();
		ann.SetLeft(long(i%2 ? right-w : left+w*.5));
		ann.SetTop(long(i<2 ? top : bottom-h));
	}


	//add rectangle tool for stop region
	for(int i=0; i<4; i++)
	{
		idx = m_chartCaloric.GetTools().Add(tcRectangle);
		if(i==0)
			this->m_idxToolFixationBlock = idx;

		m_chartCaloric.GetTools().GetItems(idx).GetAsRectangle().SetAllowResize(true);
		m_chartCaloric.GetTools().GetItems(idx).GetAsRectangle().SetAllowDrag(false);

		m_chartCaloric.GetTools().GetItems(idx).SetActive(false);
	}

	//add rectangle for peak 
	for(int i=0; i<4; i++)
	{
		idx = m_chartCaloric.GetTools().Add(tcRectangle);
		if(i==0)
			this->m_idxToolPeakAmp = idx;
		m_chartCaloric.GetTools().GetItems(idx).GetAsRectangle().SetAllowResize(false);
		m_chartCaloric.GetTools().GetItems(idx).GetAsRectangle().SetAllowDrag(this->m_nManualPeakSelect);
		m_chartCaloric.GetTools().GetItems(idx).GetAsRectangle().GetShape().SetColor((i==0)||(i==3) ? COLOR_WARM : COLOR_COOL);
		m_chartCaloric.GetTools().GetItems(idx).GetAsRectangle().GetShape().GetPen().SetColor(0);
		m_chartCaloric.GetTools().GetItems(idx).GetAsRectangle().GetShape().GetPen().SetWidth(3);
		//m_chartCaloric.GetTools().GetItems(idx).GetAsRectangle().GetShape().SetTransparent(false);
		m_chartCaloric.GetTools().GetItems(idx).GetAsRectangle().GetShape().SetTransparency(75);

		m_chartCaloric.GetTools().GetItems(idx).GetAsRectangle().GetBrush().SetStyle(bsCross);
		
		
		//this->setPeakRect(i, ltMax/2, (i<2?1:-1)*btMax/2);
	}

	//add spontaneous compensation line
	idx = m_chartCaloric.GetTools().Add(tcColorLine);
	this->m_idxToolCompensation = idx;
	VARIANT varAxis; varAxis.vt=VT_DISPATCH;
	CAxis ax = m_chartCaloric.GetAxis().GetLeft();
	varAxis.pdispVal = ax;
	m_chartCaloric.GetTools().GetItems(idx).GetAsColorLine().SetAxis(varAxis);
	m_chartCaloric.GetTools().GetItems(idx).GetAsColorLine().SetValue(0);
	m_chartCaloric.GetTools().GetItems(idx).GetAsColorLine().GetPen().SetColor(RGB(34, 233, 234));
	m_chartCaloric.GetTools().GetItems(idx).GetAsColorLine().GetPen().SetWidth(2);
	this->m_chartCaloric.GetTools().GetItems(idx).GetAsColorLine().SetAllowDrag(false);
	m_chartCaloric.GetTools().GetItems(idx).SetActive(false);


	
}

void CDlgAnalyzeCaloric::setFixationBlock(int test, double t0, double t1)
{
	
	



	double left, right, top, bottom;

	//add rectangle tool for stop region
	double btMax = m_chartCaloric.GetAxis().GetBottom().GetMaximum();
	double ltMax = m_chartCaloric.GetAxis().GetLeft().GetMaximum();
	

	double xdir = (test%2) ? 1 : -1;
	double ydir = (test<2) ? 1 : -1;

	left = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(test%2 ? t0 : -1*t1);
	right = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(test%2 ? t1 : -1*t0);
	top = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(test<2 ? ltMax : 0);
	bottom = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(test<2 ? 0 : -1*ltMax);

	m_chartCaloric.GetTools().GetItems(m_idxToolFixationBlock+test).GetAsRectangle().SetLeft(long(left));
	m_chartCaloric.GetTools().GetItems(m_idxToolFixationBlock+test).GetAsRectangle().SetTop(long(top));
	m_chartCaloric.GetTools().GetItems(m_idxToolFixationBlock+test).GetAsRectangle().SetWidth(long(right-left));
	m_chartCaloric.GetTools().GetItems(m_idxToolFixationBlock+test).GetAsRectangle().SetHeight(long(bottom-top));
	
	
}

void CDlgAnalyzeCaloric::setPeakRect(int test, double x, double y)
{
	//peak rectangle을 표시된 곳에 위치 시킨다.
	double width = this->m_analyzeCaloric.m_caloricData[test].m_peakRegion[1]-m_analyzeCaloric.m_caloricData[test].m_peakRegion[0];
	if(!width)
		width = 10;
	long left, top, right, bottom;
	if(test%2 == 0)	//left일 경우 x 축이 음수이다.
		x = -1*x;

	left = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(x-width/2);
	right = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(x+width/2);
	top = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(y)-10;
	bottom = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(y)+10;


	CRectangleTool rect = m_chartCaloric.GetTools().GetItems(this->m_idxToolPeakAmp+test).GetAsRectangle();
	rect.SetLeft(left);
	rect.SetWidth(right-left);
	
	rect.SetTop(top);
	rect.SetHeight(bottom-top);
}



void CDlgAnalyzeCaloric::loadNystagmusData(int LR)
{
		
	//nystagmus 데이터를 load한다.
	CString str;
	CFileStatus fs;
	for(int i=0; i<4; i++)
	{
		str = this->m_analyzeCaloric.m_caloricData[i].m_strTestFname;
		str.Replace(".emd", LR == LEFT ? "L.ema" : "R.ema");
		if(!CFile::GetStatus(str, fs))
		{
			//analyze file이 없다면 eye movement data를 읽어와 분석한다.
			double* pEye = NULL;
			unsigned long count = 0;
			
			EU_TestFileV2 testFile;
			ED_TestInfo testInfo;
			if(testFile.open(this->m_analyzeCaloric.m_caloricData[i].m_strTestFname, NULL, false))
			{
				testFile.readTI(&testInfo);
				count = testInfo.m_lEyeDataCount;
				if(count)
				{
				
					pEye = new double[count];
					testFile.readTestData(true, (LR==LEFT)?pEye:NULL, (LR==RIGHT)?pEye:NULL, NULL, NULL);

					this->m_analyzeCaloric.m_caloricData[i].m_analyzeNys.analyze(pEye, count);
					//this->m_analyzeNys[i].analyze(pEye, count);
					delete []pEye;
					//분석한 내용을 파일로 저장한다.
					m_analyzeCaloric.m_caloricData[i].m_analyzeNys.save(str);
				}
			}

			
		}
		else	        //analyze 파일 로드한다. 
			m_analyzeCaloric.m_caloricData[i].m_analyzeNys.load(str);

	}
	
}

void CDlgAnalyzeCaloric::analyzeThis()
{
	this->m_analyzeCaloric.analyze();


}
bool CDlgAnalyzeCaloric::loadAnalyzeData()
{
	bool bRtn;
	// analyze file을 load한다.
	bRtn = m_analyzeCaloric.load(this->m_strAnalyzeFileName);

	// eye select
	if(m_analyzeCaloric.m_nEyeSelect == LEFT)
		((CButton*)GetDlgItem(IDC_RADIO_CALORIC_LEFT_EYE))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_RADIO_CALORIC_RIGHT_EYE))->SetCheck(true);


	//SPONTANEOUS NYSTAGMUS를 COMPENSATE하는지 확인
	((CButton*)GetDlgItem(IDC_CHECK_SPONTANEOUS_NYSTAG))->SetCheck(m_analyzeCaloric.m_bCompensateSpontaneous);
	
	//spontaneous 값
	CString str;
	str.Format("%.1f °/sec", this->m_analyzeCaloric.m_dSpontaneousValue);
	GetDlgItem(IDC_STATIC_SPONTANEOUS_NYSTAG_VALUE)->SetWindowText(str);

	//사용한다면 관련 ctrls들 active 시킨다.
	GetDlgItem(IDC_BUTTON_SELECT_SPONTANEOUS_TEST)->EnableWindow(m_analyzeCaloric.m_bCompensateSpontaneous);
	GetDlgItem(IDC_STATIC_SPONTANEOUS_NYSTAG_VALUE)->EnableWindow(m_analyzeCaloric.m_bCompensateSpontaneous);
	

	//fixation region을 사용하는지
	double left, top, right, bottom;
	double xdirection = 1;
	double btMax = m_chartCaloric.GetAxis().GetBottom().GetMaximum();
	double ltMax = m_chartCaloric.GetAxis().GetLeft().GetMaximum();

	DWORD ids[] = {IDC_CHECK_USE_FIX_BLOCK_LW, IDC_CHECK_USE_FIX_BLOCK_RC, IDC_CHECK_USE_FIX_BLOCK_LC, IDC_CHECK_USE_FIX_BLOCK_RW};
	//fixation region 설정
	for(int i=0; i<4; i++)
	{
		//fixation block을 사용한다면, check 설정하고, fix region을 보여준다.
		((CButton*)GetDlgItem(ids[i]))->SetCheck(m_analyzeCaloric.m_caloricData[i].m_bUseFixationBlock);
		this->m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationBlock+i).SetActive(m_analyzeCaloric.m_caloricData[i].m_bUseFixationBlock);
		
		//fixation region 위치를 설정한다.
		if(this->m_analyzeCaloric.m_caloricData[i].m_fixationBlock[0] || m_analyzeCaloric.m_caloricData[i].m_fixationBlock[0])
		{
			//left의 경우 음수로 그려줘야한다.
			xdirection = i%2 ? 1 : -1;			

			//둘다 0이 아니라면 fixation block의 위치를 조절한다.
			if(i%2)	//1,4분면
			{
				left = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(xdirection*m_analyzeCaloric.m_caloricData[i].m_fixationBlock[0]);
				right = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(xdirection*m_analyzeCaloric.m_caloricData[i].m_fixationBlock[1]);
			}
			else
			{
				//2,3분면. 음수영역이므로 left, right가 바뀌어야한다.
				right = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(xdirection*m_analyzeCaloric.m_caloricData[i].m_fixationBlock[0]);
				left = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(xdirection*m_analyzeCaloric.m_caloricData[i].m_fixationBlock[1]);

			}
			//상하 범위는 axis 크기와 맞춰준다.
			top = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(i<2 ? ltMax : 0);
			bottom = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(i<2 ? 0 : -1*ltMax);

			m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationBlock+i).GetAsRectangle().SetLeft(long(left));
			m_chartCaloric.GetTools().GetItems(m_idxToolFixationBlock+i).GetAsRectangle().SetTop(long(top));
			m_chartCaloric.GetTools().GetItems(m_idxToolFixationBlock+i).GetAsRectangle().SetWidth(long(right-left));
			m_chartCaloric.GetTools().GetItems(m_idxToolFixationBlock+i).GetAsRectangle().SetHeight(long(bottom-top));
		}

	

	}
	//((CButton*)GetDlgItem(IDC_CHECK_USE_FIX_BLOCK))->SetCheck(m_analyzeCaloric.m_
	return bRtn;
}

void CDlgAnalyzeCaloric::initListCtrls()
{
	
	//set list control
	//insert column
	CRect rect;
	this->m_listTestResult.GetClientRect(&rect);
	int w = rect.Width();

	//setup list test result
	CString list[5] = {"", "LW", "RC", "LC", "RW"};
	
	
	double widths[5] = {.4, .15, .15, .15, .15};
	for(int i=0; i<5; i++)
		m_listTestResult.InsertColumn(i, list[i], LVCFMT_CENTER, int(w*widths[i]));
		

	//setup list test summary
	char* listD[2] = {"", "value"};
	double widthD[2] = {.5, .5};
	for(int i=0; i<2; i++)
		this->m_listTestSummary.InsertColumn(i, listD[i], LVCFMT_CENTER, int(widthD[i]*w));

}
void CDlgAnalyzeCaloric::showResult()
{
	//보상값
	double spon = 0;
	CString str;
	if(this->m_analyzeCaloric.m_bCompensateSpontaneous)
		spon = this->m_analyzeCaloric.m_dSpontaneousValue[this->m_analyzeCaloric.m_nEyeSelect];
	str.Format("%.1f °/sec", m_analyzeCaloric.m_dSpontaneousValue[m_analyzeCaloric.m_nEyeSelect]);
	GetDlgItem(IDC_STATIC_SPONTANEOUS_NYSTAG_VALUE)->SetWindowText(str);
	
	

	//manual peak
/*	if(this->m_analyzeCaloric.m_nManualPeakSelect == 0)
		((CButton*)GetDlgItem(IDC_RADIO_MANUAL_PEAK_NONE))->SetCheck(true);
	else if(m_analyzeCaloric.m_nManualPeakSelect == 1)
		((CButton*)GetDlgItem(IDC_RADIO_MANUAL_PEAK_X))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_RADIO_MANUAL_PEAK_XY))->SetCheck(true);*/
	// allow drag
/*	for(int i=0; i<4; i++)
	{
		this->m_chartCaloric.GetTools().GetItems(m_idxToolPeakAmp+i).GetAsRectangle().SetAllowDrag(
						m_analyzeCaloric.m_nManualPeakSelect == 0 ? false : true);
	}
*/

	//peak region을 그린다.
	for(int i=0; i<4; i++)
		this->setPeakRect(i, (m_analyzeCaloric.m_caloricData[i].m_peakRegion[0] + m_analyzeCaloric.m_caloricData[i].m_peakRegion[1])/2,
					m_analyzeCaloric.getPeakVlue(i));

	// butterfly를 그린다.
	this->m_chartButterfly.Series(0).AddXY(0, this->m_analyzeCaloric.getPeakVlue(LW), "", 0);
	this->m_chartButterfly.Series(0).AddXY(1, this->m_analyzeCaloric.getPeakVlue(RW), "", 0);
	this->m_chartButterfly.Series(1).AddXY(0, this->m_analyzeCaloric.getPeakVlue(LC), "", 0);
	this->m_chartButterfly.Series(1).AddXY(1, this->m_analyzeCaloric.getPeakVlue(RC), "", 0);

	// spontaneous 보상값을 그린다.
	this->m_chartCaloric.GetTools().GetItems(this->m_idxToolCompensation).GetAsColorLine().SetValue(this->m_analyzeCaloric.m_dSpontaneousValue[m_analyzeCaloric.m_nEyeSelect]);
	m_chartCaloric.GetTools().GetItems(this->m_idxToolCompensation).SetActive(m_analyzeCaloric.m_bCompensateSpontaneous);
		

	// fixation 구간을 그려준다.
	for(int i=0; i<4; i++)
	{
		setFixationBlock(i, m_analyzeCaloric.m_caloricData[i].m_fixationBlock[0],  m_analyzeCaloric.m_caloricData[i].m_fixationBlock[1]);
		//fixation block을 active 시킬지
		m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationBlock+i).SetActive(m_analyzeCaloric.m_caloricData[i].m_bUseFixationBlock);
	}
	

	// 결과 값을 보여준다.
	this->fillListResult();
}

void CDlgAnalyzeCaloric::fillListResult()
{
	m_listTestResult.DeleteAllItems();	
	m_listTestSummary.DeleteAllItems();

	double peakVel[4];
	double fi[4];

	double value;


	//fill test result
	for(int i=0; i<4; i++)
	{
		peakVel[i] = this->m_analyzeCaloric.getPeakVlue(i);
		if(this->m_analyzeCaloric.m_caloricData[i].m_bUseFixationBlock)
			fi[i] = this->m_analyzeCaloric.m_caloricData[i].m_fixationIdx*100;
		else
			fi[i] = -1.1111111;
	}
	::corAdd2List(&m_listTestResult, "fixation index (%)", fi, 4);
	::corAdd2List(&m_listTestResult, "peak velocity (°/sec)", peakVel, 4);
	

	// fill test summary
	value = this->m_analyzeCaloric.m_caloricTemperatureEffect*100;
	::corAdd2List(&m_listTestSummary, "Temperature Effect (%)", &value, 1);
	
	value = this->m_analyzeCaloric.m_caloricDirectionalpreponderance*100;
	::corAdd2List(&m_listTestSummary, "Directional Preponderance (%)", &value, 1);

	value = this->m_analyzeCaloric.m_caloricVestibularParesis*100;
	::corAdd2List(&m_listTestSummary, "Vestibular Paresis (%)", &value, 1);
	


}

void CDlgAnalyzeCaloric::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->m_analyzeCaloric.save(this->m_strAnalyzeFileName);

	OnOK();
}


void CDlgAnalyzeCaloric::drawNystagmus(double spon)
{
	POSITION pos;
	DWORD color;

	double maxX = 0;
	double maxY = 0;
	double x,y;

	

	for(int i=0; i<4; i++)
	{
		double xdir = (i%2)? 1: -1;	//0,2는 x좌표가 음수
		double ydir = (i<2)? 1: -1;

		pos = m_analyzeCaloric.m_caloricData[i].m_analyzeNys.m_listNystagmus.GetHeadPosition();
		int count = int(this->m_analyzeCaloric.m_caloricData[i].m_analyzeNys.m_listNystagmus.GetCount());
		structNystag* pNys = NULL;

		color = (i==0)||(i==3) ? COLOR_WARM : COLOR_COOL;
	

		for(int n = 0; n<count; n++)
		{
			pNys = m_analyzeCaloric.m_caloricData[i].m_analyzeNys.m_listNystagmus.GetNext(pos);
			//left는 음수로 그려줘야한다.
			x = xdir*(pNys->endI+pNys->startI)/2./FRAMERATE;
			y = pNys->vel + spon;

			//방향이 맞는 것만 그린다.
			//if(ydir*(pNys->vel+spon) > 0)
				this->m_chartCaloric.Series(i).AddXY(x, pNys->vel + spon, "", color);

			if(maxX < fabs(x))
				maxX = fabs(x);
			if(maxY < fabs(y))
				maxY = fabs(y);
		}
	}

	//챠트 밖에 그려지는 것이 있다면 조절해 준다.
	if(this->m_chartCaloric.GetAxis().GetBottom().GetMaximum() < maxX)
		m_chartCaloric.GetAxis().GetBottom().SetMinMax(-1*(maxX+1), maxX + 1);
	if(this->m_chartCaloric.GetAxis().GetLeft().GetMaximum() < maxY)
		m_chartCaloric.GetAxis().GetLeft().SetMinMax(-1*(maxY+10), maxY+10);

	m_chartCaloric.Repaint();
}

void CDlgAnalyzeCaloric::OnBnClickedCheckUseFixBlock()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DWORD ids[] = {IDC_CHECK_USE_FIX_BLOCK_LW, IDC_CHECK_USE_FIX_BLOCK_RC, IDC_CHECK_USE_FIX_BLOCK_LC, IDC_CHECK_USE_FIX_BLOCK_RW};

	CRect rect;
	CPoint point;
	GetCursorPos(&point);
	bool bFixation;
	
	for(int i=0; i<4; i++)
	{
		GetDlgItem(ids[i])->GetWindowRect(&rect);
		if(rect.PtInRect(point))
		{
			bFixation = ((CButton*)GetDlgItem(ids[i]))->GetCheck() == BST_CHECKED;
    		this->m_analyzeCaloric.m_caloricData[i].m_bUseFixationBlock = bFixation;
			m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationBlock+i).SetActive(bFixation);
			
		}
	}

	this->fillListResult();
}

BEGIN_EVENTSINK_MAP(CDlgAnalyzeCaloric, CDialog)
	ON_EVENT(CDlgAnalyzeCaloric, IDC_TCHART_CALORIC_SLOW_MOVE, 8, OnDblClickTchartCaloricSlowMove, VTS_NONE)
	ON_EVENT(CDlgAnalyzeCaloric, IDC_TCHART_CALORIC_SLOW_MOVE, 246, OnRectangleToolResizedTchartCaloricSlowMove, VTS_NONE)
	ON_EVENT(CDlgAnalyzeCaloric, IDC_TCHART_CALORIC_SLOW_MOVE, 232, OnRectangleToolResizingTchartCaloricSlowMove, VTS_NONE)
	ON_EVENT(CDlgAnalyzeCaloric, IDC_TCHART_CALORIC_SLOW_MOVE, 245, OnRectangleToolDraggedTchartCaloricSlowMove, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgAnalyzeCaloric::OnDblClickTchartCaloricSlowMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	/*CPoint point;
	GetCursorPos(&point);

	double x0, y0, x1, y1;
	int X0, Y0, X1, Y1;
	CRect rect;

	m_chartCaloric.GetWindowRect(&rect);
	point.x -= rect.left;
	point.y -= rect.top;


	if(!m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationColorBand).GetActive())
	{
		// colorband가 보이지 않는다면
		// fixation block의 위치를 edit 할 수 있게 해야 한다.
		// DBClick 위치가 fixation block 내인지 확인하고
		// 맞다면 colorband를 block에 맞게 보이고
		// fixation block의 색을 바꾼다.

		for(int i=0; i<4; i++)
		{
			if(this->m_chartCaloric.Series(m_idxSeriesFixation+i).GetActive())
			{
				x0 = this->m_chartCaloric.Series(m_idxSeriesFixation+i).GetAsShape().GetX0();
				y0 = this->m_chartCaloric.Series(m_idxSeriesFixation+i).GetAsShape().GetY0();
				x1 = this->m_chartCaloric.Series(m_idxSeriesFixation+i).GetAsShape().GetX1();
				y1 = this->m_chartCaloric.Series(m_idxSeriesFixation+i).GetAsShape().GetY1();

				X0 = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(x0);
				X1 = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(x1);
				Y0 = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(y0);
				Y1 = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(y1);

				rect.top = min(Y0, Y1);
				rect.bottom = max(Y0, Y1);
				rect.left = min(X0, X1);
				rect.right = max(X0, X1);

				if(rect.PtInRect(point))
				{
					m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationColorBand).GetAsColorband().SetStartValue(x0);
					m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationColorBand).GetAsColorband().SetEndValue(x1);
					m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationColorBand).SetActive(true);

					m_chartCaloric.Series(m_idxSeriesFixation+i).SetColor(RGB(255, 0, 0));//.GetAsShape().GetBrush().SetBackColor(RGB(255, 0, 0));//.SetColor(RGB(255, 0, 0));
					m_idxSelectedFixationBlock = i;
				}
			}

		}
	}
	else
	{
		//만일 colorband가 보이고 있는 상태라면
		// dbclick 위치를 파악하고 
		// 만일 colorband 안에서 일어났다면 fixation block을 이동시킨다.
		// 밖에서 일어났다면 무시한다.
		// colorband를 inactive 시키고,
		// fixation block 색을 원래데로 돌이킨다.

		x0 = this->m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationColorBand).GetAsColorband().GetStartValue();
		x1 = this->m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationColorBand).GetAsColorband().GetEndValue();
		y0 = this->m_chartCaloric.GetAxis().GetLeft().GetMinimum();
		y1 = this->m_chartCaloric.GetAxis().GetLeft().GetMaximum();

		X0 = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(x0);
		X1 = m_chartCaloric.GetAxis().GetBottom().CalcXPosValue(x1);
		Y0 = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(y0);
		Y1 = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(y1);

		rect.top = min(Y0, Y1);
		rect.bottom = max(Y0, Y1);
		rect.left = min(X0, X1);
		rect.right = max(X0, X1);

		// colorband 내라면
		if(rect.PtInRect(point))
		{
			if(m_idxSelectedFixationBlock != -1)
			{
				//fixation block을 colorband 위치로 옮긴다.
				m_chartCaloric.Series(this->m_idxSeriesFixation+m_idxSelectedFixationBlock).GetAsShape().SetX0(x0);
				m_chartCaloric.Series(this->m_idxSeriesFixation+m_idxSelectedFixationBlock).GetAsShape().SetX1(x1);
			}
		}
		//원래대로
		m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationColorBand).SetActive(false);

		m_chartCaloric.Series(this->m_idxSeriesFixation + m_idxSelectedFixationBlock).SetColor(RGB(255, 255, 255));
	}*/
}

void CDlgAnalyzeCaloric::OnRectangleToolResizedTchartCaloricSlowMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//rectangle tool의 크기를 바꿨을 때

	double direction = 1;
	double ltMax = m_chartCaloric.GetAxis().GetLeft().GetMaximum();
	double top, bottom;

	for(int i=0; i<4; i++)
	{
		//y축의 최대 최소값의 pixel값
		top = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(i<2 ? ltMax : 0);
		bottom = m_chartCaloric.GetAxis().GetLeft().CalcYPosValue(i<2 ? 0 : -1*ltMax);

		//x축 부호. left는 음수
		direction = i%2 ? 1:-1;
		
		//fixation region tool을 얻어온다.
		CRectangleTool rect = m_chartCaloric.GetTools().GetItems(this->m_idxToolFixationBlock + i).GetAsRectangle();
		
		//fixation region의 좌우 값을 얻어온다. 
		if(i%2)	// right일 경우1,4분면
		{
			m_analyzeCaloric.m_caloricData[i].m_fixationBlock[0] = m_chartCaloric.GetAxis().GetBottom().CalcPosPoint(rect.GetLeft());
			m_analyzeCaloric.m_caloricData[i].m_fixationBlock[1] = m_chartCaloric.GetAxis().GetBottom().CalcPosPoint(rect.GetLeft() + rect.GetWidth());
			
		}
		else
		{
			//2,4분면일 경우 left가 maximum, right가 minimum이다.
			// - 값이 구해지므로 +로 바꾸어야한다.
			m_analyzeCaloric.m_caloricData[i].m_fixationBlock[0] = -1*m_chartCaloric.GetAxis().GetBottom().CalcPosPoint(rect.GetLeft() + rect.GetWidth());
			m_analyzeCaloric.m_caloricData[i].m_fixationBlock[1] = -1*m_chartCaloric.GetAxis().GetBottom().CalcPosPoint(rect.GetLeft());
			
			
		}

		

		//혹시 상하 값이 바뀌었을 수도 있으니 y축 크기로 맞추어 준다.
		rect.SetTop(long(top));
		rect.SetHeight(long(bottom-top));

    }
	this->analyzeThis();
	this->showResult();
}

void CDlgAnalyzeCaloric::OnRectangleToolResizingTchartCaloricSlowMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDlgAnalyzeCaloric::OnBnClickedCheckSpontaneousNystag()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	bool bActive = ((CButton*)GetDlgItem(IDC_CHECK_SPONTANEOUS_NYSTAG))->GetCheck() == BST_CHECKED;
	this->m_analyzeCaloric.m_bCompensateSpontaneous = bActive;

	GetDlgItem(IDC_BUTTON_SELECT_SPONTANEOUS_TEST)->EnableWindow(bActive);
	GetDlgItem(IDC_STATIC_SPONTANEOUS_NYSTAG_VALUE)->EnableWindow(bActive);

	initTChart();	
	//분석한다.
	analyzeThis();		

	//보상값
	double spon = 0;
	if(this->m_analyzeCaloric.m_bCompensateSpontaneous)
		spon = this->m_analyzeCaloric.m_dSpontaneousValue[this->m_analyzeCaloric.m_nEyeSelect];

	//nystagmus 를 그린다.
	this->drawNystagmus(spon);

	showResult();


	

}

void CDlgAnalyzeCaloric::OnRectangleToolDraggedTchartCaloricSlowMove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//마우스 위치를 찾는다.
	CPoint point;
	GetCursorPos(&point);

	CRect rect;
	m_chartCaloric.GetWindowRect(&rect);

	//chart 상의 위치로 바꾼다.
	point.x -= rect.left;
	point.y -= rect.top;


	for(int i=0; i<4; i++)
	{
		CRectangleTool recTool = m_chartCaloric.GetTools().GetItems(this->m_idxToolPeakAmp+i).GetAsRectangle();
		rect.left = recTool.GetLeft();
		rect.right = rect.left + recTool.GetWidth();
		rect.top = recTool.GetTop();
		rect.bottom = rect.top + recTool.GetHeight();

		if(rect.PtInRect(point))
		{
			this->m_analyzeCaloric.m_caloricData[i].m_nManualPeakSelect = this->m_nManualPeakSelect;


			//해당 peak region을 update한다.
			if(i%2)
			{
				//1,4분면.
				m_analyzeCaloric.m_caloricData[i].m_peakRegion[0] = m_chartCaloric.GetAxis().GetBottom().CalcPosPoint(rect.left);
				m_analyzeCaloric.m_caloricData[i].m_peakRegion[1] = m_chartCaloric.GetAxis().GetBottom().CalcPosPoint(rect.right);;
			}
			else
			{
				//2,3분면. x 좌표가 음수이다.
				m_analyzeCaloric.m_caloricData[i].m_peakRegion[0] = -1*m_chartCaloric.GetAxis().GetBottom().CalcPosPoint(rect.right);
				m_analyzeCaloric.m_caloricData[i].m_peakRegion[1] = -1*m_chartCaloric.GetAxis().GetBottom().CalcPosPoint(rect.left);;
			}
			m_analyzeCaloric.m_caloricData[i].m_nManualPeakSelect = this->m_nManualPeakSelect;
			
			//음수로 갔는지 확인한다.
			if(m_analyzeCaloric.m_caloricData[i].m_peakRegion[0]<0)
			{
				double width = m_analyzeCaloric.m_caloricData[i].m_peakRegion[1] - m_analyzeCaloric.m_caloricData[i].m_peakRegion[0];
				m_analyzeCaloric.m_caloricData[i].m_peakRegion[0] = 0;
				m_analyzeCaloric.m_caloricData[i].m_peakRegion[1] = m_analyzeCaloric.m_caloricData[i].m_peakRegion[0]+width;
			}		


			//만일 manual peak x-y 모드라면 rectangle의 위치로 peak value를 update해 줘야 한다.
			if(this->m_nManualPeakSelect == 2)
			{
				double y0 = m_chartCaloric.GetAxis().GetLeft().CalcPosPoint(rect.top);
				double y1 = m_chartCaloric.GetAxis().GetLeft().CalcPosPoint(rect.bottom);
				m_analyzeCaloric.setPeakValue(i, (y0+y1)/2);
			}
		}
		//recTool.GetShape().SetColor(255, 0, 0)		;
	}

	
	//구간의 velocity를 계산한다.
	this->analyzeThis();
	//peak 구간을 다시 그려준다.
	showResult();
}

void CDlgAnalyzeCaloric::OnBnClickedButtonSelectSpontaneousTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlgSelectSpontaneous dlg;

	dlg.m_nEyeSelect = m_analyzeCaloric.m_nEyeSelect;
	dlg.m_bManualSelect = this->m_analyzeCaloric.m_bSpontaneousFromUser;
	dlg.m_dSpontaneousValue[0] = this->m_analyzeCaloric.m_dSpontaneousValue[0];
	dlg.m_dSpontaneousValue[1] = this->m_analyzeCaloric.m_dSpontaneousValue[1];
	dlg.m_strSpontaneousFileName = this->m_analyzeCaloric.m_strSpontaneousFileName;

	//for tree
	dlg.m_patientInfo = this->m_patientInfo;
	dlg.m_strSubjectFolder = this->m_strSubjectFolder;

	if(dlg.DoModal() == IDOK)
	{		
		m_analyzeCaloric.m_bSpontaneousFromUser = dlg.m_bManualSelect;
		m_analyzeCaloric.m_dSpontaneousValue[0] = dlg.m_dSpontaneousValue[0];
		m_analyzeCaloric.m_dSpontaneousValue[1] = dlg.m_dSpontaneousValue[1];
		if(!m_analyzeCaloric.m_bSpontaneousFromUser)	// user가 직접 입력하는 것이 아니라면
			m_analyzeCaloric.m_strSpontaneousFileName = dlg.m_strSpontaneousFileName;

		initTChart();
	

	
		//분석한다.
		analyzeThis();
				

		//보상값
		double spon = 0;
		if(this->m_analyzeCaloric.m_bCompensateSpontaneous)
			spon = this->m_analyzeCaloric.m_dSpontaneousValue[this->m_analyzeCaloric.m_nEyeSelect];

		//nystagmus 를 그린다.
		this->drawNystagmus(spon);

		showResult();

	}
}

void CDlgAnalyzeCaloric::OnBnClickedButtonResetPeak()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//전체 구간에서 찾아야 하므로 manual select를 None으로 놓는다.
	for(int i=0; i<4; i++)
		this->m_analyzeCaloric.m_caloricData[i].m_nManualPeakSelect = 0;
	
	analyzeThis();
	showResult();
}


void CDlgAnalyzeCaloric::OnBnClickedRadioCaloricEye()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//fixation block을 사용할지 여부

	CRect rect;
	CPoint point;
	GetCursorPos(&point);

	GetDlgItem(IDC_RADIO_CALORIC_LEFT_EYE)->GetWindowRect(&rect);
	if(rect.PtInRect(point))
		this->m_analyzeCaloric.m_nEyeSelect = LEFT;
	else
		this->m_analyzeCaloric.m_nEyeSelect = RIGHT;

	///////////////////////////////////////////////////////////////////
	// data를 새로 load하고 분석한다.

	initTChart();

	//nystagmus data를 load한다.
	loadNystagmusData(m_analyzeCaloric.m_nEyeSelect);

    
	//분석한다.
	analyzeThis();
			
	
	//보상값
	double spon = 0;
	if(this->m_analyzeCaloric.m_bCompensateSpontaneous)
		spon = this->m_analyzeCaloric.m_dSpontaneousValue[this->m_analyzeCaloric.m_nEyeSelect];

	//nystagmus 를 그린다.
	this->drawNystagmus(spon);

	showResult();
	
}




void CDlgAnalyzeCaloric::OnBnClickedRadioManualPeak()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CRect rect;
	CPoint point;
	GetCursorPos(&point);

	DWORD ids[] = {IDC_RADIO_MANUAL_PEAK_NONE, IDC_RADIO_MANUAL_PEAK_X, IDC_RADIO_MANUAL_PEAK_XY};

	int idx = -1;
	for(int i=0; i<3; i++)
	{
		GetDlgItem(ids[i])->GetWindowRect(rect);
		if(rect.PtInRect(point))
			this->m_nManualPeakSelect = i;
	}

	//None일 경우 peak rectangle의 drag가 안되게 한다.
	for(int i=0; i<4; i++)
		this->m_chartCaloric.GetTools().GetItems(this->m_idxToolPeakAmp + i).GetAsRectangle().SetAllowDrag(m_nManualPeakSelect==0 ? false : true);
	
		

}

void CDlgAnalyzeCaloric::exportForPrint()
{
	//butterfly
	CString fname;
	fname.Format("%s\\IMAGE%02d.bmp", this->m_strExportFolderName , this->m_nExportImageNum++);
	this->m_chartButterfly.GetExport().GetAsBMP().SetHeight(500);
	this->m_chartButterfly.GetExport().GetAsBMP().SetWidth(500);
	m_chartButterfly.GetExport().GetAsBMP().SaveToFile(fname);

    //nystagmus
	fname.Format("%s\\IMAGE%02d.bmp", this->m_strExportFolderName , this->m_nExportImageNum++);
	this->m_chartCaloric.GetExport().GetAsBMP().SetHeight(500);
	this->m_chartCaloric.GetExport().GetAsBMP().SetWidth(500);
	m_chartCaloric.GetExport().GetAsBMP().SaveToFile(fname);

}


void CDlgAnalyzeCaloric::OnBnClickedButtonCaloricPrint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strProgram;
	corGetProgramFolder(&strProgram);

	
	//form 파일명
	CString strFormFname = strProgram + "\\form\\caloric.rpt";

	//print에 필요한 파일들을 생성할 폴더
	CString strDataFolder = m_strAnalyzeFileName.Left(m_strAnalyzeFileName.ReverseFind('\\'));
	CString strPrintTempFolder = strDataFolder + "\\printTemp";
	//없다면 만든다.
	if(!corFolderExist(strPrintTempFolder))
		corCreateDirectory(strPrintTempFolder);

	CRect rect;

	//butterfly chart export
	CString fname = strPrintTempFolder +"\\butterfly.bmp";
	unsigned long color = m_chartButterfly.GetPanel().GetColor();
	m_chartButterfly.GetWindowRect(&rect);
	m_chartButterfly.GetPanel().SetColor(RGB(255, 255, 255));
	this->m_chartButterfly.GetExport().GetAsBMP().SetWidth(rect.Width());
	this->m_chartButterfly.GetExport().GetAsBMP().SetHeight(rect.Height());
	this->m_chartButterfly.GetExport().GetAsBMP().SaveToFile(fname);
	m_chartButterfly.GetPanel().SetColor(color);

	//caloric chart export
	fname = strPrintTempFolder +"\\caloric.bmp";
	color = m_chartCaloric.GetPanel().GetColor();
	m_chartCaloric.GetPanel().SetColor(RGB(255, 255, 255));
	m_chartCaloric.GetWindowRect(&rect);
	this->m_chartCaloric.GetExport().GetAsBMP().SetWidth(rect.Width());
	this->m_chartCaloric.GetExport().GetAsBMP().SetHeight(rect.Height());
	this->m_chartCaloric.GetExport().GetAsBMP().SaveToFile(fname);
	m_chartCaloric.GetPanel().SetColor(color);


	//get test parameters
	double value[7];

	value[0] = m_analyzeCaloric.getPeakVlue(LC);
	value[1] = m_analyzeCaloric.getPeakVlue(LW);
	value[2] = m_analyzeCaloric.getPeakVlue(RC);
	value[3] = m_analyzeCaloric.getPeakVlue(RW);

	value[4] = m_analyzeCaloric.m_caloricTemperatureEffect*100;
	value[5] = m_analyzeCaloric.m_caloricDirectionalpreponderance*100;
	value[6] = m_analyzeCaloric.m_caloricVestibularParesis*100;

	



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
	CString ids[] = {"LEFT_COOL_VALUE", "LEFT_WARM_VALUE", "RIGHT_COOL_VALUE", "RIGHT_WARM_VALUE",
		"TEMP_EFFECT_VALUE", "DIRECTIONAL_PREP_VALUE", "VESTIBULAR_PARESIS_VALUE"};
			
	for(int i=0; i<7; i++)
		insertString2Report(&strForm, value[i], ids[i]);
	


	// make report file
	fname = strPrintTempFolder + "caloric.rpt";
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
