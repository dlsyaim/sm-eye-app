// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "tchart.h"

// Dispatch interfaces referenced by this interface
#include "aspect.h"
#include "Axes.h"
#include "canvas.h"
#include "export.h"
#include "Titles.h"
#include "pen.h"
#include "Import.h"
#include "Legend.h"
#include "Page.h"
#include "Panel.h"
#include "Printer.h"
#include "Scroll.h"
#include "Series.h"
#include "walls.h"
#include "Zoom.h"
#include "Environment.h"
#include "TeePoint2D.h"
#include "ToolList.h"
#include "serieslist.h"
#include "teerect.h"

/////////////////////////////////////////////////////////////////////////////
// CTChart

IMPLEMENT_DYNCREATE(CTChart, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CTChart properties

/////////////////////////////////////////////////////////////////////////////
// CTChart operations

CAspect CTChart::GetAspect()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CAspect(pDispatch);
}

CAxes CTChart::GetAxis()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CAxes(pDispatch);
}

CCanvas CTChart::GetCanvas()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CCanvas(pDispatch);
}

BOOL CTChart::GetClipPoints()
{
	BOOL result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CTChart::SetClipPoints(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CTChart::GetCursor()
{
	short result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CTChart::SetCursor(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

short CTChart::GetDragCursor()
{
	short result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CTChart::SetDragCursor(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CTChart::GetDragMode()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CTChart::SetDragMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CTChart::GetEnabled()
{
	BOOL result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CTChart::SetEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CExport CTChart::GetExport()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CExport(pDispatch);
}

CTitles CTChart::GetFooter()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CTitles(pDispatch);
}

CPen1 CTChart::GetFrame()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPen1(pDispatch);
}

CTitles CTChart::GetHeader()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CTitles(pDispatch);
}

long CTChart::GetHeight()
{
	long result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CTChart::SetHeight(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CImport CTChart::GetImport()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CImport(pDispatch);
}

long CTChart::GetLeft()
{
	long result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CTChart::SetLeft(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CLegend CTChart::GetLegend()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CLegend(pDispatch);
}

CPage CTChart::GetPage()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPage(pDispatch);
}

CPanel CTChart::GetPanel()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPanel(pDispatch);
}

CPrinter CTChart::GetPrinter()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPrinter(pDispatch);
}

CScroll CTChart::GetScroll()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CScroll(pDispatch);
}

long CTChart::GetSeriesCount()
{
	long result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CTChart::GetTop()
{
	long result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CTChart::SetTop(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CTChart::GetVisible()
{
	BOOL result;
	InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CTChart::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CWalls CTChart::GetWalls()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CWalls(pDispatch);
}

long CTChart::GetWidth()
{
	long result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CTChart::SetWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CZoom CTChart::GetZoom()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CZoom(pDispatch);
}

void CTChart::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long CTChart::AddSeries(long SeriesClass)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		SeriesClass);
	return result;
}

void CTChart::ChangeSeriesType(long SeriesIndex, long NewSeriesType)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SeriesIndex, NewSeriesType);
}

long CTChart::CloneSeries(long SourceSeriesIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		SourceSeriesIndex);
	return result;
}

void CTChart::Draw(long DC, long Left, long Top, long Right, long Bottom)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 DC, Left, Top, Right, Bottom);
}

void CTChart::ExchangeSeries(long Series1, long Series2)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Series1, Series2);
}

double CTChart::GetDateTimeStep(long DateTimeStep)
{
	double result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_R8, (void*)&result, parms,
		DateTimeStep);
	return result;
}

unsigned long CTChart::GetFreeSeriesColor(BOOL CheckBackgroundColor)
{
	unsigned long result;
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		CheckBackgroundColor);
	return result;
}

CSeries CTChart::GetLabelsSeries(long DestAxis)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		DestAxis);
	return CSeries(pDispatch);
}

BOOL CTChart::IsFreeSeriesColor(unsigned long AColor, BOOL CheckBackgroundColor)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		AColor, CheckBackgroundColor);
	return result;
}

void CTChart::RemoveAllSeries()
{
	InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CTChart::RemoveSeries(long SeriesIndex)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SeriesIndex);
}

void CTChart::Repaint()
{
	InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString CTChart::SeriesTitleLegend(long ASeriesIndex)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		ASeriesIndex);
	return result;
}

void CTChart::ShowEditor(long SeriesIndex)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SeriesIndex);
}

void CTChart::StopMouse()
{
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL CTChart::GetTimerEnabled()
{
	BOOL result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CTChart::SetTimerEnabled(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CTChart::GetTimerInterval()
{
	long result;
	InvokeHelper(0x2c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CTChart::SetTimerInterval(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CTChart::GetAutoRepaint()
{
	BOOL result;
	InvokeHelper(0x2d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CTChart::SetAutoRepaint(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CTChart::EditOneSeries(long SeriesIndex)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SeriesIndex);
}

CEnvironment CTChart::GetEnvironment()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CEnvironment(pDispatch);
}

long CTChart::GetChartLink()
{
	long result;
	InvokeHelper(0x30, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CTeePoint2D CTChart::GetMousePosition()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x31, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CTeePoint2D(pDispatch);
}

void CTChart::ChartRect(long Left, long Top, long Right, long Bottom)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x32, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top, Right, Bottom);
}

CToolList CTChart::GetTools()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CToolList(pDispatch);
}

CTitles CTChart::GetSubHeader()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CTitles(pDispatch);
}

CTitles CTChart::GetSubFooter()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CTitles(pDispatch);
}

void CTChart::ClearChart()
{
	InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CTChart::RefreshData()
{
	InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CSeries CTChart::Series(long SeriesIndex)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x38, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		SeriesIndex);
	return CSeries(pDispatch);
}

void CTChart::ShowThemesEditor()
{
	InvokeHelper(0xc9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CTChart::SetTheme(long aTheme, long aPalette)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xca, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 aTheme, aPalette);
}

void CTChart::ApplyPalette(long paletteIndex)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xcb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 paletteIndex);
}

void CTChart::ApplyCustomPalette(const VARIANT& colorArray)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xcc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &colorArray);
}

CSeriesList CTChart::GetSeriesList()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CSeriesList(pDispatch);
}

CString CTChart::GetHint()
{
	CString result;
	InvokeHelper(0xce, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CTChart::SetHint(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xce, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL CTChart::GetShowHint()
{
	BOOL result;
	InvokeHelper(0xcf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CTChart::SetShowHint(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xcf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString CTChart::GetVersion()
{
	CString result;
	InvokeHelper(0xd0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL CTChart::GetCustomChartRect()
{
	BOOL result;
	InvokeHelper(0xd1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CTChart::SetCustomChartRect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xd1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CTChart::GetMonochrome()
{
	BOOL result;
	InvokeHelper(0xd2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CTChart::SetMonochrome(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xd2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

short CTChart::GetOriginalCursor()
{
	short result;
	InvokeHelper(0xd3, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

void CTChart::SetOriginalCursor(short nNewValue)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0xd3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CTChart::GetPrinting()
{
	BOOL result;
	InvokeHelper(0xd4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long CTChart::GetChartHeight()
{
	long result;
	InvokeHelper(0xd7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CTChart::GetChartWidth()
{
	long result;
	InvokeHelper(0xd8, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CTeeRect CTChart::GetChartBounds()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xdb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CTeeRect(pDispatch);
}

BOOL CTChart::GetBufferedDisplay()
{
	BOOL result;
	InvokeHelper(0xdc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CTChart::SetBufferedDisplay(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xdc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CTChart::GetCanClip()
{
	BOOL result;
	InvokeHelper(0xd5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CTeeRect CTChart::GetGetChartRect()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xd6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CTeeRect(pDispatch);
}
