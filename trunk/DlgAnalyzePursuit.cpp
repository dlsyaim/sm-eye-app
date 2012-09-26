// DlgAnalyzePursuit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgAnalyzePursuit.h"
#include "corwldefines.h"
#include "eu_testfilev2.h"
#include "ed_testinfo.h"
#include "corwlUtil.h"
#include "includeTChart.h"
#include ".\dlganalyzepursuit.h"
#include "math.h"
#include ".\reportGenerator\reportGenerator.h"
#include "eu_eventfile.h"

// CDlgAnalyzePursuit 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAnalyzePursuit, CDialog)
CDlgAnalyzePursuit::CDlgAnalyzePursuit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAnalyzePursuit::IDD, pParent)
{
	m_pdLeft = m_pdRight = m_pdTarget = NULL;
	m_ulDataCount = 0;

	m_bEditing = false;
	m_pPursuitSelected = NULL;

	m_eventSeriesIdx = -1;
}

CDlgAnalyzePursuit::~CDlgAnalyzePursuit()
{
	DEL(m_pdLeft);
	DEL(m_pdRight);
	DEL(m_pdTarget);
}

void CDlgAnalyzePursuit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TCHART_AP_POSITION, m_chartPosition);
	DDX_Control(pDX, IDC_TCHART_AP_VELOCITY, m_chartVelocity);
	DDX_Control(pDX, IDC_TCHART_AP_GAIN, m_chartGain);
	DDX_Control(pDX, IDC_LIST_AP_RESULT_SUMMARY, m_listResultSummary);
	DDX_Control(pDX, IDC_LIST_AP_RESULT_DETAIL, m_listResultDetail);
}


BEGIN_MESSAGE_MAP(CDlgAnalyzePursuit, CDialog)
	ON_BN_CLICKED(IDC_RADIO_AP_ALL, OnBnClickedRadioEyeSelect)
	ON_BN_CLICKED(IDC_RADIO_AP_LEFT, OnBnClickedRadioEyeSelect)
	ON_BN_CLICKED(IDC_RADIO_AP_RIGHT, OnBnClickedRadioEyeSelect)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_AP_RESULT_DETAIL, OnLvnItemchangedListAsResultDetail)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AP_RESULT_DETAIL, OnNMDblclkListApResultDetail)
	ON_BN_CLICKED(IDC_BUTTON_AP_REFRESH, OnBnClickedButtonApRefresh)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PURSUIT_PRINT, OnBnClickedButtonPursuitPrint)
	ON_BN_CLICKED(IDC_CHECK_REMOVE_OFFSET, OnBnClickedCheckRemoveOffset)
END_MESSAGE_MAP()


// CDlgAnalyzePursuit 메시지 처리기입니다.


BOOL CDlgAnalyzePursuit::OnInitDialog()
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



void CDlgAnalyzePursuit::loadData()
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

bool CDlgAnalyzePursuit::loadAnalyzeData()
{
	bool bRtn = true;

	CString str;
	CFileStatus fs;
	//Left
	str = this->m_strTestFile;
	str.Replace(".emd", "L.ema");
	if(CFile::GetStatus(str, fs))
		this->m_analyzePursuit[0].load(str);
	else
		bRtn = false;

	//Right
	str = this->m_strTestFile;
	str.Replace(".emd", "R.ema");
	if(CFile::GetStatus(str, fs))
		this->m_analyzePursuit[1].load(str);
	else
		bRtn = false;

	return bRtn;
}

bool CDlgAnalyzePursuit::saveAnalyzeData()
{
	bool bRtn = true;

	CString str;
	CFileStatus fs;
	//Left
	str = this->m_strTestFile;
	str.Replace(".emd", "L.ema");
	if(!m_analyzePursuit[0].save(str))
		bRtn = false;

	//Right
	str = this->m_strTestFile;
	str.Replace(".emd", "R.ema");
	if(m_analyzePursuit[1].save(str))
		bRtn = false;

	return bRtn;

}

void CDlgAnalyzePursuit::allocVariable()
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

void CDlgAnalyzePursuit::initListCtrls()
{
	//setup list summary
	CRect rect;
	this->m_listResultSummary.GetClientRect(&rect);
	int w = rect.Width();

	char* list[3] = {"", "Left Gain", "Right Gain"};
	if(m_TI.m_analTarget == VERTICAL)
	{
		 list[1] = "Up Gain";
		 list[2] = "Down Gain";
	}

	double width[3] = {.34, .33, .33};
	for(int i=0; i<3; i++)
		m_listResultSummary.InsertColumn(i, list[i], LVCFMT_CENTER, int(w*width[i]));

	//setup list detail
	m_listResultDetail.GetClientRect(&rect);
	w = rect.Width();
	DWORD dwStyle = m_listResultDetail.GetExStyle();
	dwStyle = (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listResultDetail.SetExtendedStyle(dwStyle);

	char* listD[4] = {"No", "Eye Peak Velocity(°/s)", "Target Peak Velocity(°/s)", "Gain (Eye/Target)"};
	double widthD[4] = {.1, .3, .3, .4};
	for(int i=0; i<4; i++)
		m_listResultDetail.InsertColumn(i, listD[i], LVCFMT_CENTER, int(widthD[i]*w));
		
}

void CDlgAnalyzePursuit::initTChart()
{
	m_selectedPursuitIdx = -1;

	//raw
	CTChart* bufTChart[3];
	bufTChart[0] = &m_chartPosition;
	bufTChart[1] = &m_chartVelocity;
	bufTChart[2] = &m_chartGain;
	

	CString strTitles[3] = {"Position", "Velocity", "Gain"};

	for(int i=0; i<3; i++)
	{
		bufTChart[i]->ClearChart();
		bufTChart[i]->GetAspect().SetView3D(false);
		bufTChart[i]->GetZoom().SetEnable(true);
		bufTChart[i]->GetAspect().SetZoom(tzdBoth);
		//bufTChart[i]->GetLegend().SetAlignment(laBottom);		
		bufTChart[i]->GetLegend().SetVisible(false);

		bufTChart[i]->GetHeader().GetText().SetText(strTitles[i]);
		bufTChart[i]->SetAutoRepaint(false);
	}

    	

	//chart zoom
	double axisMin[3] = {-40, -40, 0};
	double axisMax[3] = {40, 40, 2};
	CString axisTitle[3] = {"Degree", "°/sec", ""};
	for(int i=0; i<3; i++)
	{
		
		if(i == 0)	//position, 
		{
			bufTChart[i]->AddSeries(scLine);	//left
			bufTChart[i]->AddSeries(scLine);	//right
			bufTChart[i]->AddSeries(scLine);	//target
		}
		else if(i == 1)	//velocity, 
		{
			bufTChart[i]->AddSeries(scLine);	//left
			bufTChart[i]->AddSeries(scLine);	//right
			bufTChart[i]->AddSeries(scLine);	//target

			bufTChart[i]->AddSeries(scLine);	// left max velocity 위치 표시 (3)
			bufTChart[i]->AddSeries(scLine);	// right max velocity 취치 표시 (4)
			bufTChart[i]->Series(3).SetColor(0);
			bufTChart[i]->Series(4).SetColor(0);
		}
		else	//Gain
		{
			bufTChart[i]->AddSeries(scPoint);
			bufTChart[i]->AddSeries(scPoint);
			bufTChart[i]->Series(0).GetAsPoint().GetPointer().SetStyle(psCircle);
			bufTChart[i]->Series(0).GetAsPoint().GetPointer().SetHorizontalSize(3);
			bufTChart[i]->Series(0).GetAsPoint().GetPointer().SetVerticalSize(3);		
			bufTChart[i]->Series(1).GetAsPoint().GetPointer().SetStyle(psCircle);
			bufTChart[i]->Series(1).GetAsPoint().GetPointer().SetHorizontalSize(3);
			bufTChart[i]->Series(1).GetAsPoint().GetPointer().SetVerticalSize(3);
		}

		bufTChart[i]->Series(0).SetTitle("Left");
		bufTChart[i]->Series(1).SetTitle("Right");
		bufTChart[i]->Series(0).SetColor(COLOR_LEFT);
		bufTChart[i]->Series(1).SetColor(COLOR_RIGHT);
		if (i != 2)	//gain 이 아니면
		{
            bufTChart[i]->Series(2).SetTitle("Target");
			bufTChart[i]->Series(2).SetColor(COLOR_TARGET);
		}

		

				
		bufTChart[i]->GetAxis().GetLeft().SetMinMax(axisMin[i], axisMax[i]);
		bufTChart[i]->GetAxis().GetLeft().GetTitle().SetCaption(axisTitle[i]);
		
		
		//data chart에 colorband를 그려준다.
		bufTChart[i]->GetTools().Add(tcColorband);
		bufTChart[i]->GetTools().GetItems(0).GetAsColorband().SetColor(COLOR_SELECTED);
		bufTChart[i]->GetTools().GetItems(0).GetAsColorband().SetTransparency(50);
		bufTChart[i]->GetTools().GetItems(0).GetAsColorband().GetPen().SetVisible(false);
		VARIANT varAxis; varAxis.vt=VT_DISPATCH;
		CAxis ax = bufTChart[i]->GetAxis().GetBottom();
		varAxis.pdispVal = ax;
		bufTChart[i]->GetTools().GetItems(0).GetAsColorband().SetAxis(varAxis);
		bufTChart[i]->GetTools().GetItems(0).GetAsColorband().SetDrawBehind(true);
		bufTChart[i]->GetTools().GetItems(0).SetActive(false);

	}

	m_chartGain.GetAxis().GetLeft().SetIncrement(0.5);


	// init event series
	m_eventSeriesIdx = EU_EventFile::initEventSeries(&m_chartPosition);


	//다시 그려준다.
	for(int i=0; i<3; i++)
	{
		bufTChart[i]->SetAutoRepaint(true);
		bufTChart[i]->Repaint();
	}
	


}
void CDlgAnalyzePursuit::analyzeThis(int LR)
{
	if(this->m_pdLeft)
	{
		if(LR == BOTH)
		{
			this->m_analyzePursuit[0].analyze(m_pdLeft, m_pdTarget, m_ulDataCount);
			this->m_analyzePursuit[1].analyze(m_pdRight, m_pdTarget, m_ulDataCount);
		}
		else
			this->m_analyzePursuit[LR].analyze((LR==0) ? m_pdLeft : m_pdRight, m_pdTarget, m_ulDataCount);

	}
}

void CDlgAnalyzePursuit::layoutCtrls()
{
	//all을 선택
	((CButton*)GetDlgItem(IDC_RADIO_AP_ALL))->SetCheck(true);

	double gap = 0.01;
	double button_h = gap*4;
	double button_w = button_h*4;
	CRect rect;
	this->GetClientRect(&rect);

	//radio
	corLocateWindow(GetDlgItem(IDC_RADIO_AP_ALL), gap, gap, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDC_RADIO_AP_LEFT), gap*2+button_w, gap, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDC_RADIO_AP_RIGHT), gap*3+button_w*2, gap, button_w, button_h, rect);

	double chartW = 0.95/4;
	//list ctrl
	corLocateWindow(GetDlgItem(IDC_LIST_AP_RESULT_SUMMARY), gap, 0.04+2*gap, chartW, 0.19, rect);
	corLocateWindow(GetDlgItem(IDC_LIST_AP_RESULT_DETAIL), gap*2+chartW, 0.04+2*gap, chartW*1.5, 0.19, rect);

	//remove offset
	corLocateWindow(GetDlgItem(IDC_CHECK_REMOVE_OFFSET), gap, 0.19+0.04+2*gap +0.5*gap, 10*gap, 2*gap, rect);

	//charts
	chartW = 0.98;
	corLocateWindow(&m_chartPosition, gap, 0.19+0.04+4*gap, 1-2*gap, 0.22, rect);
	corLocateWindow(&m_chartVelocity, gap, 0.19+0.04+4*gap+0.22, 1-2*gap, 0.22, rect);
	corLocateWindow(&m_chartGain, gap, 0.19+0.04+5*gap+2*0.22, 1-2*gap, 0.22, rect);

	//ok cancel
	//button
	corLocateWindow(GetDlgItem(IDC_BUTTON_AP_REFRESH), gap, 1-gap-button_h, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDC_BUTTON_PURSUIT_PRINT), 2*gap + button_w, 1-gap-button_h, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDCANCEL),1-button_w*2-2*gap, 1-gap-button_h, button_w, button_h, rect);
	corLocateWindow(GetDlgItem(IDOK),1-button_w-gap, 1-gap-button_h, button_w, button_h, rect);



	
}
void CDlgAnalyzePursuit::showResult()
{
	//list ctrl
	this->fillListSummary();
	this->fillListDetail();

	this->initTChart();


	this->m_chartPosition.SetAutoRepaint(false);
	this->m_chartVelocity.SetAutoRepaint(false);
	this->m_chartGain.SetAutoRepaint(false);

	// chart with time
	double t = 0;
	for(unsigned long i=0; i<this->m_ulDataCount; i++)
	{
		t = double(i)/FRAMERATE;
		this->m_chartPosition.Series(0).AddXY(t, this->m_pdLeft[i], NULL, 0);
		this->m_chartPosition.Series(1).AddXY(t, this->m_pdRight[i], NULL, 0);
		this->m_chartPosition.Series(2).AddXY(t, this->m_pdTarget[i], NULL, 0);

		this->m_chartVelocity.Series(0).AddXY(t, this->m_analyzePursuit[0].m_pdEyeVel[i], NULL, 0);
		this->m_chartVelocity.Series(1).AddXY(t, this->m_analyzePursuit[1].m_pdEyeVel[i], NULL, 0);
		this->m_chartVelocity.Series(2).AddXY(t, this->m_analyzePursuit[0].m_pdTargetVel[i], NULL, 0);

	
	}

	showAnalyze();

	//event file 을 load한다.
	CString fname = m_strTestFile;
	fname.Replace(".emd", ".evt");
	EU_EventFile::loadEventFile(fname, &m_chartPosition, this->m_eventSeriesIdx, false);


	this->m_chartPosition.SetAutoRepaint(true);
	this->m_chartVelocity.SetAutoRepaint(true);
	this->m_chartGain.SetAutoRepaint(true);
	this->m_chartPosition.Repaint();
	this->m_chartVelocity.Repaint();
	this->m_chartGain.Repaint();


}

void CDlgAnalyzePursuit::showAnalyze()
{


	//gain
	POSITION pos1, pos2;
	COLORREF color[2] = {COLOR_LEFT, COLOR_RIGHT};
	for(int LR = 0; LR<2; LR++)
	{
		
		this->m_chartGain.Series(LR).Clear();
		this->m_chartVelocity.Series(LR+3).Clear();

		int count = this->m_analyzePursuit[LR].getCount();
		
		
		//pusruit 의 가장짧은 간격을 찾는다.
		double tmin = 100;
		double prevT = 0;
		for(pos1 = this->m_analyzePursuit[LR].m_listPursuit.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structPursuit* pPursuit = m_analyzePursuit[LR].m_listPursuit.GetNext(pos1);			
			double ct = double(pPursuit->peakVelEyeIdx)/FRAMERATE;
			

			if(ct-prevT<tmin)
				tmin = ct-prevT;
			prevT = ct;
		}
		//max vel을 표시할 바의 길이는 tmin/2
		double len = min(tmin/3,1);
		len = max(len, 0.25);


		for(pos1 = this->m_analyzePursuit[LR].m_listPursuit.GetTailPosition(); (pos2 = pos1) != NULL; )
		{
			
			structPursuit* pPursuit = m_analyzePursuit[LR].m_listPursuit.GetPrev(pos1);			
			// gain chart
			double t = double(pPursuit->peakVelEyeIdx)/FRAMERATE;
			m_chartGain.Series(LR).AddXY(t, fabs(pPursuit->peakVelEye/pPursuit->peakVelTarget), NULL, color[LR]);

			// velocity chart의 max velocity 표시
			m_chartVelocity.Series(LR+3).AddXY(t-len, pPursuit->peakVelEye, NULL, 0);
			m_chartVelocity.Series(LR+3).AddXY(t+len, pPursuit->peakVelEye, NULL, 0);
			m_chartVelocity.Series(LR+3).AddNullXY(t+len, pPursuit->peakVelEye, NULL);

		}
	}
}

void CDlgAnalyzePursuit::fillListSummary()
{
	int LR = this->getRadioLR();
	m_listResultSummary.DeleteAllItems();	

	
	//if(LR!=BOTH)
	{

		structPrintPursuit printPursuit[2];
		this->analyzeForPrint(printPursuit, false);

		double left[2] = {printPursuit[LEFT].gain[0], printPursuit[LEFT].gain[1]};
		double right[2] = {printPursuit[RIGHT].gain[0], printPursuit[RIGHT].gain[1]};

		corAdd2List(&m_listResultSummary, "Left Eye", left,2 );
		corAdd2List(&m_listResultSummary, "Right Eye", right, 2);

	}

		/*
		double minValue, maxValue, meanValue;
		double val[3];

		
		//Latency
		this->m_analyzePursuit[LR].getGain(minValue, maxValue, meanValue);
		val[0] = minValue;
		val[1] = maxValue;
		val[2] = meanValue;
		corAdd2List(&m_listResultSummary, "Gain", val, 3);

		
		//count 
		unsigned int count = this->m_analyzePursuit[LR].getCount();
		val[0] = val[1] = val[2] = count;
		corAdd2List(&this->m_listResultSummary, "count", val, 3);
	}*/
}
void CDlgAnalyzePursuit::fillListDetail()
{
	int LR = getRadioLR();
	m_listResultDetail.DeleteAllItems();	


	POSITION pos1, pos2;
	
	
	if(LR == BOTH)
		return;

	CString strCount;
	int count = this->m_analyzePursuit[LR].getCount();
	for(pos1 = this->m_analyzePursuit[LR].m_listPursuit.GetTailPosition(); (pos2 = pos1) != NULL; )
	{
		//velocity chart에 표시
		structPursuit* pPursuit = m_analyzePursuit[LR].m_listPursuit.GetPrev(pos1);

		strCount.Format("%d", count--);
		double val[3] = {pPursuit->peakVelEye, pPursuit->peakVelTarget, pPursuit->peakVelTarget ? pPursuit->peakVelEye/pPursuit->peakVelTarget : 0};
		corAdd2List(&m_listResultDetail, strCount, val, 3);		
	}
}



int CDlgAnalyzePursuit::getRadioLR()
{
	//radio button이 어느눈을 선택하고 있는지 리턴한다.
	int LR;
	//determin eye
	if(((CButton*)GetDlgItem(IDC_RADIO_AP_LEFT))->GetCheck())
		LR = LEFT;
	else if(((CButton*)GetDlgItem(IDC_RADIO_AP_RIGHT))->GetCheck())
		LR = RIGHT;
	else
		LR = BOTH;

	return LR;
}

void CDlgAnalyzePursuit::OnBnClickedRadioEyeSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int LR = this->getRadioLR();
	
	if(LR == LEFT)
	{
		this->setLRVisible(true, false);
	}
	//else if(((CButton*)GetDlgItem(IDC_RADIO_AN_RIGHT))->GetCheck())
	else if(LR == RIGHT)
	{
		this->setLRVisible(false, true);
	}
	else if (LR == BOTH)
	{
        this->setLRVisible(true, true);		
	}

	fillListSummary();
	fillListDetail();
	//선택된 nystagmus unselect
	selectPursuitOnChart(-1);
		
}


void CDlgAnalyzePursuit::setLRVisible(bool leftVisible, bool rightVisible)
{
	CTChart* bufTChart[3];
	bufTChart[0] = &m_chartPosition;
	bufTChart[1] = &m_chartVelocity;
	bufTChart[2] = &m_chartGain;
	
	for(int i=0; i<3; i++)
	{
		bufTChart[i]->Series(LEFT).SetActive(leftVisible);
		bufTChart[i]->Series(RIGHT).SetActive(rightVisible);
	}

	m_chartVelocity.Series(LEFT+3).SetActive(leftVisible);
	m_chartVelocity.Series(RIGHT+3).SetActive(rightVisible);
}


void CDlgAnalyzePursuit::selectPursuitOnChart(int count)
{
	//if(this->m_bEditing)
	//	this->enterEditMode(false, 0);	//cancel

	m_selectedPursuitIdx = count;
	// colorband를 해당 nystagmus위에 올려 놓는다.
	// -1일경우 invisible 시킨다.
	int LR = getRadioLR();
	if(LR == BOTH)
		return;

	if(count == -1)
	{
		m_chartPosition.GetTools().GetItems(0).SetActive(false);
		m_chartVelocity.GetTools().GetItems(0).SetActive(false);
		m_chartGain.GetTools().GetItems(0).SetActive(false);

		m_pPursuitSelected = NULL;

		if(this->m_bEditing)
			m_pursuitSaved = *m_pPursuitSelected;
	}
	else
	{
		m_chartPosition.GetTools().GetItems(0).SetActive(true);
		m_chartVelocity.GetTools().GetItems(0).SetActive(true);
		m_chartGain.GetTools().GetItems(0).SetActive(true);

		    
		//count 위치의 pursuit을 받아온다.
		POSITION pos1, pos2;
		int n = 0;
		for(pos1 = m_analyzePursuit[LR].m_listPursuit.GetHeadPosition(); (pos2 = pos1) != NULL; n++)
		{
			this->m_pPursuitSelected = m_analyzePursuit[LR].m_listPursuit.GetNext(pos1);
			if(n == count)
				break;
		}

		if(this->m_bEditing)
			m_pursuitSaved = *m_pPursuitSelected;

		//colorban의 위치 조정
		m_chartPosition.GetTools().GetItems(0).GetAsColorband().SetStartValue(double(m_pPursuitSelected->startIdx)/FRAMERATE-0.1);
		m_chartPosition.GetTools().GetItems(0).GetAsColorband().SetEndValue(double(m_pPursuitSelected->endIdx)/FRAMERATE+0.1);
		m_chartVelocity.GetTools().GetItems(0).GetAsColorband().SetStartValue(double(m_pPursuitSelected->startIdx)/FRAMERATE-0.1);
		m_chartVelocity.GetTools().GetItems(0).GetAsColorband().SetEndValue(double(m_pPursuitSelected->endIdx)/FRAMERATE+0.1);
		m_chartGain.GetTools().GetItems(0).GetAsColorband().SetStartValue(double(m_pPursuitSelected->startIdx)/FRAMERATE-0.1);
		m_chartGain.GetTools().GetItems(0).GetAsColorband().SetEndValue(double(m_pPursuitSelected->endIdx)/FRAMERATE+0.1);

	}
	
}
void CDlgAnalyzePursuit::OnLvnItemchangedListAsResultDetail(NMHDR *pNMHDR, LRESULT *pResult)
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

		this->selectPursuitOnChart(lRow);
	}
	
}

BOOL CDlgAnalyzePursuit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(this->m_bEditing)
		{
			if(pMsg->wParam == 37)	//left
			{
				moveSelectedPursuit(-1, 0);
				return true;
			}
			else if(pMsg->wParam == 39)	//right
			{
				moveSelectedPursuit(1, 0);
				return true;
			}
			else if(pMsg->wParam == 38) // up
			{
				moveSelectedPursuit(0, 0.1);
				return true;
			}
			else if(pMsg->wParam == 40)	//down
			{
				moveSelectedPursuit(0, -0.1);
				return true;
			}
		}
		else	//delete는 edit시에는 안된다.
		{
			if(pMsg->wParam == 46) //DEL
			{
				int LR = this->getRadioLR();
				if(( (LR == LEFT) || (LR == RIGHT) )  && (this->m_selectedPursuitIdx != -1))
					deleteSelectedPursuit(LR, m_selectedPursuitIdx);				
				
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

	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgAnalyzePursuit::deleteSelectedPursuit(int LR, int pursuitIdx)
{
	
	// 1. list에서 삭제
	// 2. 결과 update
	// 3. chart update
	// 4. list control에서 삭제
	// 5. 다음 item 선택

	int count = int(this->m_analyzePursuit[LR].m_listPursuit.GetCount());

	//validation
	if( ((LR != LEFT) && (LR != RIGHT)) || (pursuitIdx >= count))
		return;

	// 1. list에서 삭제
	POSITION pos = m_analyzePursuit[LR].m_listPursuit.FindIndex(pursuitIdx);
	structPursuit* pPursuit = m_analyzePursuit[LR].m_listPursuit.GetAt(pos);
	delete pPursuit;
	m_analyzePursuit[LR].m_listPursuit.RemoveAt(pos);


	// 2. 3. chart 및 list update
	//this->showResult();
	//this->OnBnClickedRadioEyeSelect();	//initTChart를 해서 다 보인다. 선택한 것만 보이게.
	this->showAnalyze();
	this->fillListSummary();
	this->fillListDetail();

	
	// 5. 다음 아이템 선택
	// 마지막 item이 아니라면 다음 nystagmus를 선택하고 마지막이라면 이전 것을 선택한다.
	if(pursuitIdx == count-1)
		pursuitIdx--;
	if(pursuitIdx >= 0)
	{
		m_listResultDetail.SetSelectionMark(pursuitIdx);
		m_listResultDetail.SetItemState(pursuitIdx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_listResultDetail.SetFocus();

		//chat의 colorband는 selection을 바꿈으로써 OnLvnItemChagedList()에서 이동한다.
	}
	else
	{
		// 더이상 nystagmus가 없다면
		// colorBand를 inactive 시킨다.
		selectPursuitOnChart(-1);

	}

	


	
}

BEGIN_EVENTSINK_MAP(CDlgAnalyzePursuit, CDialog)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_POSITION, 27, OnZoomTchartAp, VTS_NONE)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_POSITION, 26, OnUndoZoomTchartAp, VTS_NONE)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_POSITION, 24, OnScrollTchartAp, VTS_NONE)

	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_VELOCITY, 27, OnZoomTchartAp, VTS_NONE)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_VELOCITY, 26, OnUndoZoomTchartAp, VTS_NONE)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_VELOCITY, 24, OnScrollTchartAp, VTS_NONE)

	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_GAIN, 27, OnZoomTchartAp, VTS_NONE)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_GAIN, 26, OnUndoZoomTchartAp, VTS_NONE)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_GAIN, 24, OnScrollTchartAp, VTS_NONE)

	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_POSITION, 1, OnAfterDrawTchartApPosition, VTS_NONE)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_VELOCITY, 8, OnDblClickTchart, VTS_NONE)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_GAIN, 8, OnDblClickTchart, VTS_NONE)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_VELOCITY, 3, OnClickTchart, VTS_NONE)
	ON_EVENT(CDlgAnalyzePursuit, IDC_TCHART_AP_GAIN, 3, OnClickTchart, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgAnalyzePursuit::OnZoomTchartAp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	CTChart* pTChart[3];
	pTChart[0] = &m_chartPosition;
	pTChart[1] = &m_chartVelocity;
	pTChart[2] = &m_chartGain;

	//마우스가 위치한 chart를 찾는다.
	for(int i=0; i<3; i++)
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

		for(int i=0; i<3; i++)
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

	m_chartPosition.Repaint();
	EU_EventFile::adjustEventLocation(&m_chartPosition, this->m_eventSeriesIdx);
}

void CDlgAnalyzePursuit::OnUndoZoomTchartAp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	CTChart* pTChart[3];
	pTChart[0] = &m_chartPosition;
	pTChart[1] = &m_chartVelocity;
	pTChart[2] = &m_chartGain;

	//마우스가 위치한 chart를 찾는다.
	for(int i=0; i<3; i++)
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

		for(int i=0; i<3; i++)
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

	m_chartPosition.Repaint();
	EU_EventFile::adjustEventLocation(&m_chartPosition, this->m_eventSeriesIdx);
}

void CDlgAnalyzePursuit::OnScrollTchartAp()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	CTChart* pTChart[3];
	pTChart[0] = &m_chartPosition;
	pTChart[1] = &m_chartVelocity;
	pTChart[2] = &m_chartGain;

	//마우스가 위치한 chart를 찾는다.
	for(int i=0; i<3; i++)
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

		for(int i=0; i<3; i++)
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

	m_chartPosition.Repaint();
	EU_EventFile::adjustEventLocation(&m_chartPosition, this->m_eventSeriesIdx);
}

void CDlgAnalyzePursuit::OnAfterDrawTchartApPosition()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//position bottom axis의 범위에 gain axis을 맞춘다.
	double m = this->m_chartPosition.GetAxis().GetBottom().GetMinimum();
	double M = this->m_chartPosition.GetAxis().GetBottom().GetMaximum();

	this->m_chartGain.GetAxis().GetBottom().SetMinMax(m, M);
}

void CDlgAnalyzePursuit::OnNMDblclkListApResultDetail(NMHDR *pNMHDR, LRESULT *pResult)
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


void CDlgAnalyzePursuit::enterEditMode(bool bEdit, unsigned long pursuitIdx)
{
	int LR = this->getRadioLR();

	
	if(bEdit)
	{
		//edit mode로 들어간다.
		
		m_bEditing = true;
		m_chartVelocity.GetTools().GetItems(0).GetAsColorband().SetColor(COLOR_EDIT);

		unsigned int count = int(this->m_analyzePursuit[LR].m_listPursuit.GetCount());

		//validation
		if( ((LR != LEFT) && (LR != RIGHT)) || (pursuitIdx >= count))
			return;

		// 선택된 pursuit를 찾고 저장한다.
		if(m_pPursuitSelected)
			this->m_pursuitSaved = *m_pPursuitSelected;
	}
	else
	{
		//edit mode에서 나간다.
		m_bEditing = false;
		m_chartVelocity.GetTools().GetItems(0).GetAsColorband().SetColor(COLOR_SELECTED);

		if(pursuitIdx == 0)	//edit cancel
		{
			// 저장해 놓았던 것을 load한다.
			if(m_pPursuitSelected)
				*m_pPursuitSelected = this->m_pursuitSaved;
			this->showAnalyze();
			this->fillListSummary();
			double val[3] = {m_pPursuitSelected->peakVelEye, 
						m_pPursuitSelected->peakVelTarget, 
						m_pPursuitSelected->peakVelTarget ? m_pPursuitSelected->peakVelEye/m_pPursuitSelected->peakVelTarget : 0};		
			corChangeListValue(&m_listResultDetail, this->m_selectedPursuitIdx, val, 3);
		}

	}
}

void CDlgAnalyzePursuit::moveSelectedPursuit(double x, double y)
{
	if(m_pPursuitSelected)
	{
		int LR = this->getRadioLR();
		double* pEye = (LR==0) ? this->m_pdLeft : m_pdRight;

		if(x)
		{
			//좌우로 움직이면 velocity line을 따라간다.
			m_pPursuitSelected->peakVelEyeIdx += (unsigned long)x;
			if(m_pPursuitSelected->peakVelEyeIdx<0)
				m_pPursuitSelected->peakVelEyeIdx = 0;
			else if(m_pPursuitSelected->peakVelEyeIdx >= this->m_ulDataCount)
				m_pPursuitSelected->peakVelEyeIdx = m_ulDataCount-1;

			m_pPursuitSelected->peakVelEye = this->m_analyzePursuit[LR].m_pdEyeVel[m_pPursuitSelected->peakVelEyeIdx];

		}
		else if(y)
		{
			m_pPursuitSelected->peakVelEye += y;
		}

		this->showAnalyze();
		this->fillListSummary();
		//this->fillListDetail();

		double val[3] = {m_pPursuitSelected->peakVelEye, 
						m_pPursuitSelected->peakVelTarget, 
						m_pPursuitSelected->peakVelTarget ? m_pPursuitSelected->peakVelEye/m_pPursuitSelected->peakVelTarget : 0};		
		corChangeListValue(&m_listResultDetail, this->m_selectedPursuitIdx, val, 3);
	}
}
void CDlgAnalyzePursuit::OnBnClickedButtonApRefresh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int LR = this->getRadioLR();

	this->analyzeThis(LR);
	this->showAnalyze();
	this->fillListSummary();
	this->fillListDetail();


}

void CDlgAnalyzePursuit::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	this->saveAnalyzeData();

	OnOK();
}


void CDlgAnalyzePursuit::OnClickTchart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//bool bShift = bool(::GetAsyncKeyState(VK_LSHIFT));
	bool bShift = IsSHIFTpressed();
	if(bShift)
	{
		//pursuit 추가
		int LR = this->getRadioLR();

		if(LR != BOTH)
		{
			// 1. mouse pos를 받는다.
			CTChart* pCharts[] = {&(m_chartPosition), &(this->m_chartVelocity)};

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
				unsigned long idx = this->m_analyzePursuit[LR].addPursuit((unsigned long)(x*FRAMERATE), pEye[LR], this->m_ulDataCount);		
				
				
				if(idx != -1)
				{
					// 3. List 및 chart update
					showAnalyze();
					fillListSummary();
					fillListDetail();

					// 4. add 되었으면 edit 모드로
					selectListDetail(idx);
					this->selectPursuitOnChart(idx);	
				}
			}
		}
	}
	else
	{
		//pursuit 선택
		unsigned long idx = getClickedPursuit();
		if(idx != -1)
		{
			selectListDetail(idx);
			selectPursuitOnChart(idx);	
		}
	}
}


void CDlgAnalyzePursuit::OnDblClickTchart()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	unsigned long idx = getClickedPursuit();
	if(idx != -1)
	{
		selectListDetail(idx);
		selectPursuitOnChart(idx);
		this->enterEditMode(true, idx);
	}
	
}

unsigned long CDlgAnalyzePursuit::getClickedPursuit()
{
	// mouse 위치에서 가장 가까운 pursuit를 찾아 index를 리턴한다.
	unsigned long idx = -1;
	int LR = this->getRadioLR();

	if(LR != BOTH)
	{
		CTChart* pCharts[] = {&m_chartVelocity, &m_chartGain};

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

			
			idx = m_analyzePursuit[LR].getClosestPursuit(x);
		}
		
	}
	return idx;
}

void CDlgAnalyzePursuit::selectListDetail(unsigned long idx)
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


void CDlgAnalyzePursuit::analyzeForPrint(structPrintPursuit* pPrintPursuit, bool forPrint)
{
	memset(pPrintPursuit, 0, sizeof(structPrintPursuit)*2);

	if(forPrint)
	{
		this->loadData();

		//먼저 ANALYZE하고 ANALYZE FILE이 있으면 결과만 덮어쓴다.
		analyzeThis(BOTH);
		loadAnalyzeData();
	}


	
	
	memset(pPrintPursuit, 0, sizeof(structPrintPursuit)*2);
	
	

	int numPursuit[2];
	numPursuit[0] = numPursuit[1] = 0;

	int bi = 0;
	POSITION pos;
	int count = 0;

	//LEFT EYE
	bi = LEFT;
	pos = this->m_analyzePursuit[bi].m_listPursuit.GetHeadPosition();
	count = m_analyzePursuit[bi].m_listPursuit.GetCount();

	structPursuit* pPursuit  = NULL;
	for(int i=0; i<count; i++)
	{
		pPursuit  = m_analyzePursuit[bi].m_listPursuit.GetNext(pos);
		if((pPursuit) && (pPursuit->peakVelTarget > 0))	//LEFT
		{
			pPrintPursuit[bi].gain[LEFT] += fabs(pPursuit->peakVelEye/pPursuit->peakVelTarget);
			numPursuit[LEFT] += 1;
		}
		else
		{
			pPrintPursuit[bi].gain[RIGHT] += fabs(pPursuit->peakVelEye/pPursuit->peakVelTarget);
			numPursuit[RIGHT] += 1;
		}
	}
	if(numPursuit[LEFT])
	{
		pPrintPursuit[bi].gain[LEFT] /= numPursuit[LEFT];
	}
	if(numPursuit[RIGHT])
	{
		pPrintPursuit[bi].gain[RIGHT] /= numPursuit[RIGHT];	
	}

	//RIGHT EYE
	bi = RIGHT;
	numPursuit[LEFT] = numPursuit[RIGHT] = 0;
	pos = this->m_analyzePursuit[bi].m_listPursuit.GetHeadPosition();
	count = m_analyzePursuit[bi].m_listPursuit.GetCount();

	pPursuit  = NULL;
	for(int i=0; i<count; i++)
	{
		pPursuit  = m_analyzePursuit[bi].m_listPursuit.GetNext(pos);
		if((pPursuit) && (pPursuit->peakVelTarget > 0))	//LEFT
		{
			pPrintPursuit[bi].gain[LEFT] += fabs(pPursuit->peakVelEye/pPursuit->peakVelTarget);
			numPursuit[LEFT] += 1;
		}
		else
		{
			pPrintPursuit[bi].gain[RIGHT] += fabs(pPursuit->peakVelEye/pPursuit->peakVelTarget);
			numPursuit[RIGHT] += 1;
		}
	}
	if(numPursuit[LEFT])
	{
		pPrintPursuit[bi].gain[LEFT] /= numPursuit[LEFT];
	}
	if(numPursuit[RIGHT])
	{
		pPrintPursuit[bi].gain[RIGHT] /= numPursuit[RIGHT];	
	}

}
void CDlgAnalyzePursuit::OnBnClickedButtonPursuitPrint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strProgram;
	corGetProgramFolder(&strProgram);

	
	//form 파일명
	CString strFormFname = strProgram + "\\form\\analyze_pursuit.rpt";

	//print에 필요한 파일들을 생성할 폴더
	CString strPrintTempFolder = strProgram + "\\printTemp";
	//없다면 만든다.
	if(!corFolderExist(strPrintTempFolder))
		corCreateDirectory(strPrintTempFolder);

	CRect rect;

	//eye movement chart export
	CString fname = strPrintTempFolder +"\\eye_movement.bmp";
	exportTChart(&m_chartPosition, fname);

	//eye velocity chart export
	fname = strPrintTempFolder +"\\eye_velocity.bmp";
	exportTChart(&m_chartVelocity, fname);

	//gain chart export
	fname = strPrintTempFolder +"\\pursuit_gain.bmp";
	exportTChart(&m_chartGain, fname);
	
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

void CDlgAnalyzePursuit::OnBnClickedCheckRemoveOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double offset[2] = {m_pdLeft[0], m_pdRight[0]};
	if( ((CButton*)GetDlgItem(IDC_CHECK_REMOVE_OFFSET))->GetCheck() == BST_CHECKED)
	{
		offset[0] = -1*m_pdLeft[0];
		offset[1] = -1*m_pdRight[0];
	}

	//eye movement를 옮겨준다..
	unsigned long count = m_chartPosition.Series(LEFT).GetCount();
	double value = 0;
	CValueList leftEye = m_chartPosition.Series(LEFT).GetYValues();
	CValueList rightEye = m_chartPosition.Series(RIGHT).GetYValues();
	for(unsigned long i=0; i<count; i++)
	{
		value = leftEye.GetValue(i);
		leftEye.SetValue(i,  value + offset[0]);
		value = rightEye.GetValue(i);
		rightEye.SetValue(i, value + offset[1]);
	}
}
