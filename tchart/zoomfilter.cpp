// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "zoomfilter.h"


/////////////////////////////////////////////////////////////////////////////
// CZoomFilter properties

/////////////////////////////////////////////////////////////////////////////
// CZoomFilter operations

double CZoomFilter::GetPercent()
{
	double result;
	InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CZoomFilter::SetPercent(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}
