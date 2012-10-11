// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "contourlevels.h"

// Dispatch interfaces referenced by this interface
#include "ContourLevel.h"


/////////////////////////////////////////////////////////////////////////////
// CContourLevels properties

/////////////////////////////////////////////////////////////////////////////
// CContourLevels operations

CContourLevel CContourLevels::GetItems(long Index)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		Index);
	return CContourLevel(pDispatch);
}

long CContourLevels::Clicked(long XCoord, long YCoord, long* SegmentIndex, long* PointIndex)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_PI4 VTS_PI4;
	InvokeHelper(0xc9, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		XCoord, YCoord, SegmentIndex, PointIndex);
	return result;
}

long CContourLevels::AddLevel(double LevelValue, unsigned long Color)
{
	long result;
	static BYTE parms[] =
		VTS_R8 VTS_I4;
	InvokeHelper(0xca, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		LevelValue, Color);
	return result;
}