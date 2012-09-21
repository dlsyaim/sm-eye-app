#include "StdAfx.h"
#include ".\analyzenystagmus.h"
#include "math.h"
#include "corwlDefines.h"
#include "corwlUtil.h"

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


#define CHART_EYE_MOV	0
#define CHART_EYE_VEL	1
#define CHART_NYS_VEL	2



CAnalyzeNystagmus::CAnalyzeNystagmus(void)
{
	

}

CAnalyzeNystagmus::~CAnalyzeNystagmus(void)
{
	this->m_listNystag.RemoveAll();
	this->m_listNys[LEFT].RemoveAll();
	this->m_listNys[RIGHT].RemoveAll();
}

void CAnalyzeNystagmus::fillListCtrl()
{
	//반드시 dlg의 oninitdi() 에서 불려야 한다.
	if((!m_pListCtrlSummary) || (!m_pListCtrlSummary->GetSafeHwnd()))
		return;

	//list clear

	// list title
	CRect rect;
	this->m_pListCtrlSummary->GetClientRect(&rect);
	int w = rect.Width();
	char* list[4] = {"", "NEGA", "POSI", "Total"};
	double width[4] = {.25, .25,.25 ,.25};
	for(int i=0; i<4; i++)
		this->m_pListCtrlSummary->InsertColumn(i, list[i], LVCFMT_CENTER, int(w*width[i]));

	int nIdx;
	double value[3];
	CString str;
	//count
	nIdx = m_pListCtrlSummary->InsertItem(0, "count");
	value[0] = this->getCount(NEGA);
	value[1] = this->getCount(POSI);
	value[2] = this->getCount(ALL);
	
	for(int i=0; i<3; i++)
	{
		str.Format("%d", int(value[i]));
		m_pListCtrlSummary->SetItemText(nIdx, 1, str);
	}

	//mean
	nIdx = m_pListCtrlSummary->InsertItem(0, "mean");
	value[0] = this->getMean(NEGA);
	value[1] = this->getMean(POSI);
	value[2] = this->getMean(ALL);
	
	for(int i=0; i<3; i++)
	{
		str.Format("%d", int(value[i]));
		m_pListCtrlSummary->SetItemText(nIdx, 1, str);
	}

	//maximum
	nIdx = m_pListCtrlSummary->InsertItem(0, "maximum");
	value[1] = this->getMax(POSI);
	value[2] = this->getMax(ALL);
		
	for(int i=0; i<3; i++)
	{
		if(i==0)
			str = "x";
		str.Format("%d", int(value[i]));
		m_pListCtrlSummary->SetItemText(nIdx, 1, str);

	}

}



void CAnalyzeNystagmus::layoutTChart(CTChart** ppTChart, unsigned char chartNum, CRect rect)
{
	int width = rect.Width()-6;
	int height = (rect.Height()-2*3)/3;

	//3개만 필요하다.
	for(int i=0; i<CHART_ANALYZE_NUM; i++)
	{
		if(i<m_AvailableChartNum)
			ppTChart[i]->SetVisible(true);
		else
			ppTChart[i]->SetVisible(false);
	}


	for(int i=0; i<m_AvailableChartNum; i++)
	{
		ppTChart[i]->MoveWindow(3, rect.top + 3 + i*(height+3), width, height);
	}
}

void CAnalyzeNystagmus::initTChart(CTChart** ppTChart)
{	
	//좌,우에서 사용할 것을 모두 한번에 만든다.
	CString strTitle[] = {"Eye Position", "Eye Velocity", "Nystagmus Velocity"};
	for(int i=0; i<m_AvailableChartNum; i++)
	{
		ppTChart[i]->ClearChart();
		ppTChart[i]->GetAspect().SetView3D(false);
		ppTChart[i]->GetZoom().SetEnable(true);
		ppTChart[i]->GetAspect().SetZoom(tzdBoth);
		ppTChart[i]->GetLegend().SetAlignment(laBottom);
		ppTChart[i]->GetHeader().GetText().SetText(strTitle[i]);

		if(i>m_AvailableChartNum)
			ppTChart[i]->SetVisible(false);

	}
	
	//좌우 두개의 series를 더해준다.
	//movement
	ppTChart[CHART_EYE_MOV]->AddSeries(scLine);
	ppTChart[CHART_EYE_MOV]->AddSeries(scLine);
	//nystagmus 표시용
	ppTChart[CHART_EYE_MOV]->AddSeries(scLine);
	ppTChart[CHART_EYE_MOV]->AddSeries(scLine);

	ppTChart[CHART_EYE_MOV]->Series(0).SetColor(COLOR_LEFT);
	ppTChart[CHART_EYE_MOV]->Series(1).SetColor(COLOR_RIGHT);
	ppTChart[CHART_EYE_MOV]->Series(2).SetColor(RGB(0,0,0));
	ppTChart[CHART_EYE_MOV]->Series(3).SetColor(RGB(0,0,0));

	ppTChart[CHART_EYE_MOV]->Series(0).SetTitle("Left");
	ppTChart[CHART_EYE_MOV]->Series(1).SetTitle("Right");
	ppTChart[CHART_EYE_MOV]->Series(2).SetShowInLegend(false);
	ppTChart[CHART_EYE_MOV]->Series(3).SetShowInLegend(false);

	
	
	//velocity
	ppTChart[CHART_EYE_VEL]->AddSeries(scLine);
	ppTChart[CHART_EYE_VEL]->AddSeries(scLine);
	ppTChart[CHART_EYE_VEL]->AddSeries(scPoint);
	ppTChart[CHART_EYE_VEL]->AddSeries(scPoint);

	CString str[] = {"Left", "Right", "Left Fast Move", "Right Fast Move"};
	for(int i=0; i<4; i++)
	{
		ppTChart[CHART_EYE_VEL]->Series(i).SetTitle(str[i]);
	}

	//fast movement를 표시할 pointer series
	ppTChart[CHART_EYE_VEL]->Series(2).GetAsPoint().GetPointer().SetStyle(psCircle);
	ppTChart[CHART_EYE_VEL]->Series(2).GetAsPoint().GetPointer().SetHorizontalSize(2);
	ppTChart[CHART_EYE_VEL]->Series(2).GetAsPoint().GetPointer().SetVerticalSize(2);
	ppTChart[CHART_EYE_VEL]->Series(2).SetColor(ppTChart[CHART_EYE_VEL]->Series(0).GetColor());
	ppTChart[CHART_EYE_VEL]->Series(3).GetAsPoint().GetPointer().SetStyle(psCircle);
	ppTChart[CHART_EYE_VEL]->Series(3).GetAsPoint().GetPointer().SetHorizontalSize(2);
	ppTChart[CHART_EYE_VEL]->Series(3).GetAsPoint().GetPointer().SetVerticalSize(2);
	ppTChart[CHART_EYE_VEL]->Series(3).SetColor(ppTChart[CHART_EYE_VEL]->Series(1).GetColor());

	//color
	ppTChart[CHART_EYE_VEL]->Series(0).SetColor(COLOR_LEFT);
	ppTChart[CHART_EYE_VEL]->Series(1).SetColor(COLOR_RIGHT);
	ppTChart[CHART_EYE_VEL]->Series(2).SetColor(COLOR_LEFT);
	ppTChart[CHART_EYE_VEL]->Series(3).SetColor(COLOR_RIGHT);


	
	//nystagmus velocity
	ppTChart[CHART_NYS_VEL]->AddSeries(scPoint);
	ppTChart[CHART_NYS_VEL]->AddSeries(scPoint);
	ppTChart[CHART_NYS_VEL]->Series(0).GetAsPoint().GetPointer().SetStyle(psCircle);
	ppTChart[CHART_NYS_VEL]->Series(0).GetAsPoint().GetPointer().SetHorizontalSize(2);
	ppTChart[CHART_NYS_VEL]->Series(0).GetAsPoint().GetPointer().SetVerticalSize(2);

	ppTChart[CHART_NYS_VEL]->Series(1).GetAsPoint().GetPointer().SetStyle(psCircle);
	ppTChart[CHART_NYS_VEL]->Series(1).GetAsPoint().GetPointer().SetHorizontalSize(2);
	ppTChart[CHART_NYS_VEL]->Series(1).GetAsPoint().GetPointer().SetVerticalSize(2);

	

	ppTChart[CHART_NYS_VEL]->Series(0).SetTitle("Left");
	ppTChart[CHART_NYS_VEL]->Series(1).SetTitle("Right");
	

	


	


	
	
}

unsigned long findLargerIndex(double* pDat, unsigned long startIndex, unsigned long endIndex, double targetValue,  bool bFoward, bool bLarger);
void findMinMax(double* pDat, unsigned long startIndex, unsigned long endIndex, double* pMax, double* pMin);
double findMean(double* pDat, unsigned long startIndex, unsigned long endIndex);
void CAnalyzeNystagmus::analyze(double* pDat, unsigned long count, CTChart** ppTChart, unsigned char seriesNum)
{
	EA_Nystagmus analNys;
	analNys.init();
	COLORREF color = ((seriesNum%2)==0) ? COLOR_LEFT : COLOR_RIGHT;
	int nRtn;

	//list 초기화
	this->m_listNystag.RemoveAll();
	this->m_listNys[LEFT].RemoveAll();
	this->m_listNys[RIGHT].RemoveAll();


	for(unsigned long i=0; i<count; i++)
	{
		//eye movement를 그려준다.
		ppTChart[CHART_EYE_MOV]->Series(seriesNum).AddXY(double(i)/FRAMERATE, pDat[i], "", color);

		nRtn = analNys.putSignal(pDat[i]);

		//velocity를 그려준다.
		ppTChart[CHART_EYE_VEL]->Series(seriesNum).AddXY(double(i)/FRAMERATE, analNys.m_prevVel[1], "", color);

		if(nRtn)
		{
			//list에 더한다.
			this->m_listNystag.AddTail(analNys.m_structCurrentNystag);

			//chart에 표시해 준다.
			//movement에 nystagmus를 더한다.
			double x,y;
			double offset = 10;
			x = double(analNys.m_structCurrentNystag.startI)/FRAMERATE;
			y = pDat[analNys.m_structCurrentNystag.startI];
			ppTChart[CHART_EYE_MOV]->Series(2+seriesNum).AddXY(x,y+offset, "", 0);

			x = double(analNys.m_structCurrentNystag.endI)/FRAMERATE;
			y = y + offset + analNys.m_structCurrentNystag.vel*(analNys.m_structCurrentNystag.endI-analNys.m_structCurrentNystag.startI)/FRAMERATE;
			ppTChart[CHART_EYE_MOV]->Series(2+seriesNum).AddXY(x, y, "", 0);
			ppTChart[CHART_EYE_MOV]->Series(2+seriesNum).AddNullXY(x,y, "");
			
			//nystagmus velocity를 표시
			double x_ = double((analNys.m_structCurrentNystag.startI + analNys.m_structCurrentNystag.endI)/2)/FRAMERATE;
			ppTChart[CHART_NYS_VEL]->Series(seriesNum).AddXY(x_, analNys.m_structCurrentNystag.vel, "", color);

		}
	}

	for(int i=0; i<3; i++)
	{
		ppTChart[i]->SetAutoRepaint(true);
		ppTChart[i]->Repaint();
	}
}

void CAnalyzeNystagmus::analyze(double* pDat, unsigned long count, int LR)
{
	EA_Nystagmus analNys;
	analNys.init();
	
	int nRtn;

	//list 초기화
	this->m_listNystag.RemoveAll();
	this->m_listNys[LR].RemoveAll();
	


	for(unsigned long i=0; i<count; i++)
	{
		nRtn = analNys.putSignal(pDat[i]);

		if(nRtn)
		{
			//list에 더한다.
			this->m_listNys[LR].AddTail(analNys.m_structCurrentNystag);
		}
	}
}

int CAnalyzeNystagmus::getCount(int nysType)
{
	int count = 0;
	POSITION pos1, pos2;
	
	switch(nysType)
	{
	case ALL:
		count = int(this->m_listNystag.GetCount());
		break;
	case POSI:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			if(nys.vel > 0)
				count++;
		}
		break;
	case NEGA:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			if(nys.vel < 0)
				count++;
		}
		break;
	default:
		break;
	}
	return count;
}

double CAnalyzeNystagmus::getMean(int nysType)
{
	double rtn = 0;
	POSITION pos1, pos2;
	double s = 0;
	int n = 0;
	
	switch(nysType)
	{
	case ALL:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			s += nys.vel;
			n++;
		}			
		break;
	case POSI:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			if(nys.vel > 0)
			{
				s += nys.vel;
				n++;
			}


		}
		break;
	case NEGA:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			if(nys.vel < 0)
			{
				s += nys.vel;
				n++;
			}
		}
		break;
	default:
		break;
	}

	if(n)
		rtn = s/n;
	else
		rtn = 0;
	return rtn;
}
double CAnalyzeNystagmus::getMax(int nysType)
{
	double rtn = 0;
	POSITION pos1, pos2;
	
	double m = -100000;

	switch(nysType)
	{
	case ALL:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			if(m < nys.vel )
				m = nys.vel;
		}
		
		break;
	case POSI:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			if((nys.vel > 0) && (m < nys.vel))
				m = nys.vel;
		}
		break;
	case NEGA:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			if((nys.vel < 0) && (m < nys.vel))
				m = nys.vel;
		}
		break;
	default:
		break;
	}
	return m;
}
double CAnalyzeNystagmus::getMin(int nysType)
{
	double rtn = 0;
	POSITION pos1, pos2;
	
	double m = 100000;

	switch(nysType)
	{
	case ALL:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			if(m > nys.vel )
				m = nys.vel;
		}
		
		break;
	case POSI:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			if((nys.vel > 0) && (m > nys.vel))
				m = nys.vel;
		}
		break;
	case NEGA:
		for(pos1 = m_listNystag.GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNystag.GetNext(pos1);
			if((nys.vel < 0) && (m > nys.vel))
				m = nys.vel;
		}
		break;
	default:
		break;
	}
	return m;
}


unsigned long findLargerIndex(double* pDat, unsigned long startIndex, unsigned long endIndex, double targetValue,  bool bFoward, bool bLarger)
{
	//pDat[startIndex]로부터 시작하여 targetvalue보다 큰값의 index를 return한다.
	// bFoward가 false면 반대방향으로 search
	// bLarger가 true면 targetValue보다 작은 값이 나올때까지 search

	if(bFoward)
	{
		for(unsigned long i = startIndex; i<endIndex; i++)
		{
			if(bLarger)	//큰값이 나올때까지 찾는다.
			{
				if(pDat[i] > targetValue)
					return i;
			}
			else
				if(pDat[i] < targetValue)
					return i;
		}
	}
	//뒤쪽으로 찾는다.
	else
	{
		for(unsigned long i = startIndex; i>endIndex; i--)
		{
			if(bLarger)
			{
				if(pDat[i] >targetValue)
					return i;
			}
			else
				if(pDat[i] <targetValue)
					return i;
		}
	}

	return 0;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CAnalyzeNystagmus::getCount(int nysType, int LR)
{
	int count = 0;
	POSITION pos1, pos2;
	
	switch(nysType)
	{
	case ALL:
		count = int(this->m_listNys[LR].GetCount());
		break;
	case POSI:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			if(nys.vel > 0)
				count++;
		}
		break;
	case NEGA:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			if(nys.vel < 0)
				count++;
		}
		break;
	default:
		break;
	}
	return count;
}

double CAnalyzeNystagmus::getMean(int nysType, int LR)
{
	double rtn = 0;
	POSITION pos1, pos2;
	double s = 0;
	int n = 0;
	
	switch(nysType)
	{
	case ALL:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			s += nys.vel;
			n++;
		}			
		break;
	case POSI:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			if(nys.vel > 0)
			{
				s += nys.vel;
				n++;
			}


		}
		break;
	case NEGA:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			if(nys.vel < 0)
			{
				s += nys.vel;
				n++;
			}
		}
		break;
	default:
		break;
	}

	if(n)
		rtn = s/n;
	else
		rtn = 0;
	return rtn;
}
double CAnalyzeNystagmus::getMax(int nysType, int LR)
{
	double rtn = 0;
	POSITION pos1, pos2;
	
	double m = -100000;

	switch(nysType)
	{
	case ALL:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			if(m < nys.vel )
				m = nys.vel;
		}
		
		break;
	case POSI:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			if((nys.vel > 0) && (m < nys.vel))
				m = nys.vel;
		}
		break;
	case NEGA:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			if((nys.vel < 0) && (m < nys.vel))
				m = nys.vel;
		}
		break;
	default:
		break;
	}
	return m;
}
double CAnalyzeNystagmus::getMin(int nysType, int LR)
{
	double rtn = 0;
	POSITION pos1, pos2;
	
	double m = 100000;

	switch(nysType)
	{
	case ALL:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			if(m > nys.vel )
				m = nys.vel;
		}
		
		break;
	case POSI:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			if((nys.vel > 0) && (m > nys.vel))
				m = nys.vel;
		}
		break;
	case NEGA:
		for(pos1 = m_listNys[LR].GetHeadPosition(); (pos2 = pos1) != NULL; )
		{
			structNystag nys = m_listNys[LR].GetNext(pos1);
			if((nys.vel < 0) && (m > nys.vel))
				m = nys.vel;
		}
		break;
	default:
		break;
	}
	return m;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void findMinMax(double* pDat, unsigned long startIndex, unsigned long endIndex, double* pMax, double* pMin)
{
	*pMax = -10000000;
	*pMin = 10000000;

	for(unsigned long i=startIndex; i<=endIndex; i++)
	{
		if(pDat[i] > *pMax)
			*pMax = pDat[i];
		if(pDat[i] < *pMin)
			*pMin = pDat[i];
	}
}

double findMean(double* pDat, unsigned long startIndex, unsigned long endIndex)
{
	double s = 0;
	for(unsigned long i = startIndex; i<= endIndex; i++)
		s += pDat[i];
	return s/(endIndex-startIndex+1);
}