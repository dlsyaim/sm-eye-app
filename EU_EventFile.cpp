#include "StdAfx.h"
#include ".\eu_eventfile.h"
#include ".\tchart\series.h"
#include ".\tchart\valuelist.h"
#include ".\tchart\teechartdefines.h"
#include ".\tchart\series.h"
#include ".\tchart\pointseries.h"
#include ".\tchart\pointer.h"
#include ".\tchart\brush.h"
#include ".\tchart\marks.h"
#include ".\tchart\axes.h"
#include ".\tchart\axis.h"
#include ".\tchart\axislabels.h"
#include ".\tchart\pen.h"

#include "corwlDefines.h"

EU_EventFile::EU_EventFile(void)
{
}

EU_EventFile::~EU_EventFile(void)
{
}

int EU_EventFile::initEventSeries(CTChart* pTChart)
{
	int idx = -1;
	idx = pTChart->AddSeries(scPoint);		//5

	pTChart->Series(idx).GetAsPoint().GetPointer().SetStyle(psDiamond);
	pTChart->Series(idx).GetAsPoint().GetPointer().GetBrush().SetColor(0x0000ff);
	pTChart->Series(idx).GetMarks().SetVisible(true);
	pTChart->Series(idx).GetMarks().SetArrowLength(20);

	pTChart->GetAxis().GetBottom().GetLabels().SetStyle(talValue);

	return idx;
}

void EU_EventFile::saveEventFile(CString fname, CTChart* pTChart, int seriesIdx)
{
	//[Header]
	//count = 2;

	//[Event 0]
	//index = 12345
	//string = Event 1

	//[Event 1]
	//index = 324234
	//string = Event 2

	//fname이 존재한다면 삭제한다.
	CFileStatus fs;
	if(CFile::GetStatus(fname, fs))
		CFile::Remove(fname);


	//get event count;
	int count = pTChart->Series(seriesIdx).GetCount();

	CString str;
	if(count)
	{
	// make header
		str.Format("%d", count);
		WritePrivateProfileString("Header", "count", str, fname);
	// add events
		int idx;
		CString strSection;
		for(int i=0; i<count; i++)
		{
			strSection.Format("Event %d", i);

			// add index
			idx = pTChart->Series(seriesIdx).GetXValues().GetValue(i)*FRAMERATE;
			str.Format("%d", idx);
			WritePrivateProfileString(strSection, "index", str, fname);

			// add string
			str = pTChart->Series(seriesIdx).GetValueMarkText(i);
			WritePrivateProfileString(strSection, "string", str, fname);

		}

	}
}



void EU_EventFile::loadEventFile(CString fname, CTChart* pTChart, int seriesIdx,  bool bRemoveOffset)
{
	//[Header]
	//count = 2;

	//[Event 0]
	//index = 12345
	//string = Event 1

	//[Event 1]
	//index = 324234
	//string = Event 2


	//get event count;
	int count = GetPrivateProfileInt("Header", "count", 0, fname);

	int idxEvent;
	CString strEvent;
	char bufEvent[256];
	CString strSection;
	double x,y;
	double offset = 0;

	//set offset
	if(bRemoveOffset)
		offset = pTChart->Series(0).GetYValues().GetValue(0);

	/*double minY = pTChart->GetAxis().GetLeft().GetMinimum();
	double maxY = pTChart->GetAxis().GetLeft().GetMaximum();

	long len = pTChart->GetAxis().GetLeft().CalcYPosValue(minY)-pTChart->GetAxis().GetLeft().CalcYPosValue(maxY)-10;
*/
	for(int i=0; i<count; i++)
	{
		//set section name
		strSection.Format("Event %d", i);

		// get idx and x,y
		idxEvent = GetPrivateProfileInt(strSection, "index", -1, fname);
		x = double(idxEvent)/FRAMERATE;
		y = pTChart->Series(0).GetYValues().GetValue(idxEvent) - offset;
		//y = minY;

		// get event string
		memset(bufEvent, 0, 256);
		GetPrivateProfileString(strSection, "string", "", bufEvent, 255, fname);
		strEvent.Format("%s", bufEvent);


		//add to chart
		pTChart->Series(seriesIdx).AddXY(x,y,strEvent, 0);
	}

    /*pTChart->Series(seriesIdx).GetMarks().GetArrow().SetColor(RGB(255,0,0));
	pTChart->Series(seriesIdx).GetMarks().GetArrow().SetWidth(2);
	pTChart->Series(seriesIdx).GetMarks().SetArrowLength(len);*/

	adjustEventLocation(pTChart, seriesIdx);

}

void EU_EventFile::adjustEventLocation(CTChart* pTChart, int seriesIdx)
{
	int count = pTChart->Series(seriesIdx).GetCount();

	double minY = pTChart->GetAxis().GetLeft().GetMinimum();
	double maxY = pTChart->GetAxis().GetLeft().GetMaximum();

	long len = pTChart->GetAxis().GetLeft().CalcYPosValue(minY)-pTChart->GetAxis().GetLeft().CalcYPosValue(maxY)-10;


		pTChart->Series(seriesIdx).GetMarks().GetArrow().SetColor(RGB(0,0,0));
	pTChart->Series(seriesIdx).GetMarks().GetArrow().SetWidth(2);
	pTChart->Series(seriesIdx).GetMarks().SetArrowLength(len);


	for(int i=0; i<count; i++)
		pTChart->Series(seriesIdx).GetYValues().SetValue(i, minY);
	
	//redraw
	bool bAuto = pTChart->GetAutoRepaint();
	pTChart->SetAutoRepaint(true);
	pTChart->Repaint();
	pTChart->SetAutoRepaint(bAuto);
}