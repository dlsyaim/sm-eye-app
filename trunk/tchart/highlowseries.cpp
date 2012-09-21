// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "highlowseries.h"

// Dispatch interfaces referenced by this interface
#include "valuelist.h"
#include "brush.h"
#include "pen.h"


/////////////////////////////////////////////////////////////////////////////
// CHighLowSeries properties

/////////////////////////////////////////////////////////////////////////////
// CHighLowSeries operations

long CHighLowSeries::AddHighLow(double AX, double AHigh, double ALow, LPCTSTR AXLabel, unsigned long AColor)
{
	long result;
	static BYTE parms[] =
		VTS_R8 VTS_R8 VTS_R8 VTS_BSTR VTS_I4;
	InvokeHelper(0xc9, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		AX, AHigh, ALow, AXLabel, AColor);
	return result;
}

CValueList CHighLowSeries::GetHighValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

CValueList CHighLowSeries::GetLowValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

double CHighLowSeries::MaxYValue()
{
	double result;
	InvokeHelper(0xcc, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
	return result;
}

double CHighLowSeries::MinYValue()
{
	double result;
	InvokeHelper(0xcd, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
	return result;
}

CBrush1 CHighLowSeries::GetHighBrush()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CBrush1(pDispatch);
}

CPen1 CHighLowSeries::GetHighPen()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPen1(pDispatch);
}

CBrush1 CHighLowSeries::GetLowBrush()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CBrush1(pDispatch);
}

CPen1 CHighLowSeries::GetLowPen()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPen1(pDispatch);
}

CPen1 CHighLowSeries::GetPen()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPen1(pDispatch);
}

long CHighLowSeries::GetTransparency()
{
	long result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CHighLowSeries::SetTransparency(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}
