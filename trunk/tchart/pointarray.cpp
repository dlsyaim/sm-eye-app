// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "pointarray.h"

// Dispatch interfaces referenced by this interface
#include "TeePoint2D.h"


/////////////////////////////////////////////////////////////////////////////
// CPointArray properties

/////////////////////////////////////////////////////////////////////////////
// CPointArray operations

long CPointArray::GetCount()
{
	long result;
	InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

CTeePoint2D CPointArray::GetItem(long Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		Index);
	return CTeePoint2D(pDispatch);
}