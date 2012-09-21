// DlgMakeTChartImage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgMakeTChartImage.h"
#include ".\dlgmaketchartimage.h"
#include "eu_testfilev2.h"
#include "corwldefines.h"
#include "math.h"
#include "eu_eventfile.h"

#include ".\tchart\series.h"
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
#include ".\tchart\panel.h"
#include ".\tchart\chartaxispen.h"
#include ".\tchart\export.h"
#include ".\tchart\bmpexport.h"
#include ".\tchart\brush.h"

// CDlgMakeTChartImage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMakeTChartImage, CDialog)
CDlgMakeTChartImage::CDlgMakeTChartImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMakeTChartImage::IDD, pParent)
{
	m_ulDataCount = 0;
	m_pdLH = m_pdRH = m_pdLV = m_pdRV = m_pdStiH = m_pdStiV = NULL;
}

CDlgMakeTChartImage::~CDlgMakeTChartImage()
{
	DEL(m_pdLH);
	DEL(m_pdRH);
	DEL(m_pdLV);
	DEL(m_pdRV);
	DEL(m_pdStiH);
	DEL(m_pdStiV);
}

void CDlgMakeTChartImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_IMAGE, m_chart);
}


BEGIN_MESSAGE_MAP(CDlgMakeTChartImage, CDialog)
END_MESSAGE_MAP()


// CDlgMakeTChartImage 메시지 처리기입니다.

BOOL CDlgMakeTChartImage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	layoutCtrls();

	CString fname;
	fname.Format("%s%s", this->m_strSubjectFolder, this->m_pTI->m_chFileName);
	loadData(fname);

	makeImages();


	


	OnOK();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgMakeTChartImage::layoutCtrls()
{
	this->m_chart.MoveWindow(0,0, 100, 200);
}
void CDlgMakeTChartImage::loadData(CString strFileName)
{
	EU_TestFileV2 testFile;
	ED_TestInfo testInfo;
	
	if(testFile.open(strFileName, &testInfo, false))
	{
		testFile.readTI(&testInfo);
		m_ulDataCount = testInfo.m_lEyeDataCount;
		if(m_ulDataCount)
		{
			this->allocVariable();

			testFile.readTestData(true, this->m_pdLH, m_pdRH, m_pdLV, m_pdRV, 0, 0, 0, 0, 0, 0, 0, 0, 0, this->m_pdStiH, m_pdStiV);

			
		}

	}
	

}

void CDlgMakeTChartImage::allocVariable()
{
	DEL(m_pdLH);
	DEL(m_pdRH);
	DEL(m_pdLV);
	DEL(m_pdRV);
	DEL(m_pdStiH);
	DEL(m_pdStiV);

	if(m_ulDataCount)
	{
		m_pdLH = new double[m_ulDataCount];
		m_pdRH = new double[m_ulDataCount];
		m_pdLV = new double[m_ulDataCount];
		m_pdRV = new double[m_ulDataCount];
		m_pdStiH = new double[m_ulDataCount];
		m_pdStiV = new double[m_ulDataCount];

		memset(m_pdLH, 0, sizeof(double)*m_ulDataCount);
		memset(m_pdRH, 0, sizeof(double)*m_ulDataCount);
		memset(m_pdLV, 0, sizeof(double)*m_ulDataCount);
		memset(m_pdRV, 0, sizeof(double)*m_ulDataCount);
		memset(m_pdStiH, 0, sizeof(double)*m_ulDataCount);
		memset(m_pdStiV, 0, sizeof(double)*m_ulDataCount);
	}
}

void CDlgMakeTChartImage::makeImages()
{
	//horizontal
	initTChart();
	CString text;

	

	if(drawData(HORIZONTAL))		//그림이 잘 그려졌다면
	{
		if((m_bottomAxisDuration == 0))	//axis duration이 정해지지 않았거나 
		{
			//전체를 그려준다.
			m_bottomAxisDuration = m_chart.Series(0).GetXValues().GetMaximum();
			int ccount = m_chart.Series(0).GetXValues().GetCount();
		}
		
		m_chart.GetAxis().GetBottom().SetAutomatic(false);
		m_chart.GetAxis().GetBottom().SetMinMax(0, 0);

		int count = m_chartNum;
		while(m_chart.Series(0).GetXValues().GetMaximum() > m_chart.GetAxis().GetBottom().GetMaximum())
		{
			//bottom axis를 다음 범위로 이동
			double m = m_chart.GetAxis().GetBottom().GetMaximum();
			m_chart.GetAxis().GetBottom().SetMinMax(m, m+m_bottomAxisDuration);

			printImgParam param = export();			//파일로 만든다.
			text = "";
			if(count == m_chartNum)
			{
				//이 text는 맨 처음 한번만 써 준다.
				text.Format("%s horizontal eye position", this->m_pTI->m_chFileName);		//그림 title로 쓰여질 text
				text.Replace(".emd", "");	//파일명의 확장자는 빼주자
				param.text = text;
			}
			this->m_listImgParam.AddTail(param);		//리스트에 추가하고 chartNum을 증가
			m_chartNum++;

		}
	}


	//vertical
	initTChart();
	
	if(drawData(VERTICAL))
	{
		if(m_bottomAxisDuration == 0)
		{
			m_bottomAxisDuration = m_chart.Series(0).GetXValues().GetMaximum();
			int ccount = m_chart.Series(0).GetXValues().GetCount();
		}

		m_chart.GetAxis().GetBottom().SetAutomatic(false);
		m_chart.GetAxis().GetBottom().SetMinMax(0, 0);


		int count = m_chartNum;
		while(m_chart.Series(0).GetXValues().GetMaximum() > m_chart.GetAxis().GetBottom().GetMaximum())
		{
			//bottom axis를 다음 범위로 이동
			double m = m_chart.GetAxis().GetBottom().GetMaximum();
			m_chart.GetAxis().GetBottom().SetMinMax(m, m+m_bottomAxisDuration);

			printImgParam param = export();
			text = "";
			if(count == m_chartNum)
			{
				text.Format("%s vertical eye position", this->m_pTI->m_chFileName);
				text.Replace(".emd", "");
				param.text = text;
			}

			this->m_listImgParam.AddTail(param);
			m_chartNum++;
			
		}
	}
}

bool CDlgMakeTChartImage::drawData(int HV)
{
	double *pLeft, *pRight, *pTarget;
	if(HV == HORIZONTAL)
	{
		pLeft = m_pdLH;
		pRight = m_pdRH;
		pTarget = this->m_pdStiH;
	}
	else if(HV == VERTICAL)
	{
		pLeft = m_pdLV;
		pRight = m_pdRV;
		pTarget = m_pdStiV;
	}
	for(int i=0; i<m_ulDataCount; i++)
	{
		double t = double(i)/m_pTI->m_nEyeFreq;
		m_chart.Series(0).AddXY(t, pLeft[i], NULL, 0);
		m_chart.Series(1).AddXY(t, pRight[i], NULL, 0);
		m_chart.Series(2).AddXY(t, pTarget[i], NULL, 0);
	}

	//data의 범위를 보고 axis의 볌위를 조절한다.
	double mm = 100000000000000;
	double MM = -100000000000000;
	for(int i=0; i<3; i++)
	{
		double m = m_chart.Series(i).GetYValues().GetMinimum();
		double M = m_chart.Series(i).GetYValues().GetMaximum();
		if(m < mm) mm = m;
		if(M > MM) MM = M;
	}
	//axis의 범위를 벗어나는 데이터가 있는지 확인한다.
	if((m_chart.GetAxis().GetLeft().GetMaximum() < MM) || (m_chart.GetAxis().GetLeft().GetMinimum() > mm))
	{
		double MMM = max(fabs(MM), fabs(mm));
		m_chart.GetAxis().GetLeft().SetMinMax(-1*MMM, MMM);

	}

	m_chart.SetAutoRepaint(true);
	m_chart.Repaint();

	//event file을 load한다.
	CString fname;
	fname.Format("%s%s", this->m_strSubjectFolder, this->m_pTI->m_chFileName);
	CString strEventFname = fname;
	strEventFname.Replace(".emd", ".evt");

	EU_EventFile::loadEventFile(strEventFname, &(this->m_chart), 3, true);
	m_chart.Series(3).GetMarks().GetFont().SetHeight(20);

	if(m_ulDataCount)
		return true;
	else
		return false;
}

void CDlgMakeTChartImage::initTChart()
{

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// chart ECG
	m_chart.ClearChart();
	 
	 
	m_chart.SetAutoRepaint(false);
	m_chart.GetLegend().SetVisible(true);
	m_chart.GetLegend().SetAlignment(laBottom);
	m_chart.GetLegend().GetFont().SetSize(20);
	//m_chart.
	//back color
	m_chart.GetPanel().SetColor(RGB(255,255,255));	//white
	m_chart.GetPanel().SetMarginTop(3);
	m_chart.GetPanel().SetMarginRight(3);

	m_chart.GetAspect().SetView3D(false);
	m_chart.GetAxis().GetLeft().SetAutomatic(false);

	//left labels
	if((this->m_pTI->m_stiModeCommand == COMMAND_STI_SACCADE_A) ||
		(m_pTI->m_stiModeCommand == COMMAND_STI_SACCADE_B) ||
		(m_pTI->m_stiModeCommand == COMMAND_STI_SACCADE_RANDOM))
		m_chart.GetAxis().GetLeft().SetMinMax(-40, 40);
	else
        m_chart.GetAxis().GetLeft().SetMinMax(-30, 30);
	 
	  
	//axis color
	//m_chart.GetAxis().GetTop().SetVisible(false);
	//m_chart.GetAxis().GetRight().SetVisible(false);

	m_chart.GetAxis().GetLeft().GetAxisPen().SetColor(RGB(128, 128, 128));
	m_chart.GetAxis().GetLeft().SetIncrement(10);
	m_chart.GetAxis().GetLeft().GetLabels().GetFont().SetSize(20);
	m_chart.GetAxis().GetBottom().GetAxisPen().SetColor(RGB(128, 128, 128));
	m_chart.GetAxis().GetBottom().SetIncrement(5);
	m_chart.GetAxis().GetBottom().GetLabels().GetFont().SetSize(20);
		 
	CString str[4] = {"Left", "Right", "Target", "Event"};
	DWORD color[4] = {COLOR_LEFT, COLOR_RIGHT, COLOR_TARGET, RGB(255, 0, 0)};
	for(int i=0; i<3; i++)
	{
		int idx = m_chart.AddSeries(scLine);
		m_chart.Series(idx).SetName(str[i]);
		m_chart.Series(idx).SetColor(color[i]);
		m_chart.Series(idx).GetPen().SetWidth(2);
	}

	//series for event
	m_chart.GetAxis().GetBottom().GetLabels().SetStyle(talValue);

	int idx = m_chart.AddSeries(scPoint);			//7
	m_chart.Series(idx).SetName(str[3]);
	m_chart.Series(idx).GetAsPoint().GetPointer().SetStyle(psDiamond);
	m_chart.Series(idx).GetAsPoint().GetPointer().GetBrush().SetColor(0x0000ff);
	m_chart.Series(idx).GetMarks().SetVisible(true);
	m_chart.Series(idx).GetMarks().SetArrowLength(20);


	 
	m_chart.SetAutoRepaint(true);


}



printImgParam CDlgMakeTChartImage::export()
{
	CString fname;
	fname.Format("%s\\IMAGE%02d.bmp", this->m_strPrintTempFolder, this->m_chartNum);
	m_chart.GetExport().GetAsBMP().SetHeight(285*2);
	m_chart.GetExport().GetAsBMP().SetWidth(1080*2);
	m_chart.GetExport().GetAsBMP().SaveToFile(fname);

	printImgParam param;
	param.fname.Format("IMAGE%02d.bmp", m_chartNum);
	return param;
	//this->m_listFname.AddTail(param);

}BEGIN_EVENTSINK_MAP(CDlgMakeTChartImage, CDialog)
ON_EVENT(CDlgMakeTChartImage, IDC_TCHART_IMAGE, 29, OnBeforeDrawSeriesTchartImage, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgMakeTChartImage::OnBeforeDrawSeriesTchartImage()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	EU_EventFile::adjustEventLocation(&(this->m_chart), 3);
}
