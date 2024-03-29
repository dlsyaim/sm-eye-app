// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "darvasseries.h"

// Dispatch interfaces referenced by this interface
#include "Pointer.h"
#include "ValueList.h"
#include "pen.h"
#include "gradient.h"
#include "brush.h"
#include "teerect.h"


/////////////////////////////////////////////////////////////////////////////
// CDarvasSeries properties

/////////////////////////////////////////////////////////////////////////////
// CDarvasSeries operations

CPointer CDarvasSeries::GetPointer()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPointer(pDispatch);
}

BOOL CDarvasSeries::GetDark3D()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetDark3D(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CDarvasSeries::GetTransparency()
{
	long result;
	InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetTransparency(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL CDarvasSeries::GetFastPoint()
{
	BOOL result;
	InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetFastPoint(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xca, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CDarvasSeries::GetTreatNulls()
{
	long result;
	InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetTreatNulls(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xcb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CDarvasSeries::GetCandleStyle()
{
	long result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetCandleStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CDarvasSeries::GetCandleWidth()
{
	long result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetCandleWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x24, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CValueList CDarvasSeries::GetCloseValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

CValueList CDarvasSeries::GetDateValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

unsigned long CDarvasSeries::GetDownCloseColor()
{
	unsigned long result;
	InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetDownCloseColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x25, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CValueList CDarvasSeries::GetHighValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

CValueList CDarvasSeries::GetLowValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

CValueList CDarvasSeries::GetOpenValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

BOOL CDarvasSeries::GetShowCloseTick()
{
	BOOL result;
	InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetShowCloseTick(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CDarvasSeries::GetShowOpenTick()
{
	BOOL result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetShowOpenTick(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

unsigned long CDarvasSeries::GetUpCloseColor()
{
	unsigned long result;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetUpCloseColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long CDarvasSeries::AddCandle(double DateValue, double OpenValue, double HighValue, double LowValue, double CloseValue)
{
	long result;
	static BYTE parms[] =
		VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8;
	InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		DateValue, OpenValue, HighValue, LowValue, CloseValue);
	return result;
}

CPen1 CDarvasSeries::GetHighLowPen()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x12d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPen1(pDispatch);
}

long CDarvasSeries::GetColorStyle()
{
	long result;
	InvokeHelper(0x12e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CDarvasSeries::SetColorStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x12e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CGradient CDarvasSeries::GetUpCloseGradient()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x12f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CGradient(pDispatch);
}

CGradient CDarvasSeries::GetDownCloseGradient()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x130, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CGradient(pDispatch);
}

CBrush1 CDarvasSeries::GetBoxBrush()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x191, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CBrush1(pDispatch);
}

CPen1 CDarvasSeries::GetBoxPen()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x192, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPen1(pDispatch);
}

long CDarvasSeries::GetNumBoxes()
{
	long result;
	InvokeHelper(0x193, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long CDarvasSeries::ClickedBox(long X, long Y)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x195, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		X, Y);
	return result;
}

CTeeRect CDarvasSeries::GetBoxesRect(long Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x194, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		Index);
	return CTeeRect(pDispatch);
}
