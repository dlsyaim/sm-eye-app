// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "adxfunction.h"

// Dispatch interfaces referenced by this interface
#include "series.h"


/////////////////////////////////////////////////////////////////////////////
// CADXFunction properties

/////////////////////////////////////////////////////////////////////////////
// CADXFunction operations

CSeries CADXFunction::GetDMDown()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CSeries(pDispatch);
}

CSeries CADXFunction::GetDMUp()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CSeries(pDispatch);
}
