// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "mappolygonlist.h"

// Dispatch interfaces referenced by this interface
#include "MapPolygon.h"


/////////////////////////////////////////////////////////////////////////////
// CMapPolygonList properties

/////////////////////////////////////////////////////////////////////////////
// CMapPolygonList operations

long CMapPolygonList::Add()
{
	long result;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

CMapPolygon CMapPolygonList::GetPolygon(long Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		Index);
	return CMapPolygon(pDispatch);
}

long CMapPolygonList::GetCount()
{
	long result;
	InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}
