// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "pointfigureseries.h"

// Dispatch interfaces referenced by this interface
#include "ValueList.h"
#include "Pointer.h"


/////////////////////////////////////////////////////////////////////////////
// CPointFigureSeries properties

/////////////////////////////////////////////////////////////////////////////
// CPointFigureSeries operations

CValueList CPointFigureSeries::GetCloseValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

CValueList CPointFigureSeries::GetDateValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

CValueList CPointFigureSeries::GetHighValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

CValueList CPointFigureSeries::GetLowValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xcc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

CValueList CPointFigureSeries::GetOpenValues()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CValueList(pDispatch);
}

long CPointFigureSeries::AddOHLC(double DateValue, double OpenValue, double HighValue, double LowValue, double CloseValue)
{
	long result;
	static BYTE parms[] =
		VTS_R8 VTS_R8 VTS_R8 VTS_R8 VTS_R8;
	InvokeHelper(0xce, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		DateValue, OpenValue, HighValue, LowValue, CloseValue);
	return result;
}

double CPointFigureSeries::GetBoxSize()
{
	double result;
	InvokeHelper(0xcf, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CPointFigureSeries::SetBoxSize(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xcf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

double CPointFigureSeries::GetReversalAmount()
{
	double result;
	InvokeHelper(0xd0, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CPointFigureSeries::SetReversalAmount(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xd0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CPointer CPointFigureSeries::GetDownSymbol()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xd1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPointer(pDispatch);
}

CPointer CPointFigureSeries::GetUpSymbol()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xd2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPointer(pDispatch);
}

long CPointFigureSeries::GetInversionColumn(long Index)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xd3, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Index);
	return result;
}
