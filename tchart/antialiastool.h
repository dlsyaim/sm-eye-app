#if !defined(AFX_ANTIALIASTOOL_H__BFE85DF6_00A2_46F7_B301_292EBF828891__INCLUDED_)
#define AFX_ANTIALIASTOOL_H__BFE85DF6_00A2_46F7_B301_292EBF828891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CFilterItems;

/////////////////////////////////////////////////////////////////////////////
// CAntiAliasTool wrapper class

class CAntiAliasTool : public COleDispatchDriver
{
public:
	CAntiAliasTool() {}		// Calls COleDispatchDriver default constructor
	CAntiAliasTool(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAntiAliasTool(const CAntiAliasTool& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPUNKNOWN GetBitmap();
	CFilterItems GetFilters();
	BOOL GetAntialias();
	void SetAntialias(BOOL bNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANTIALIASTOOL_H__BFE85DF6_00A2_46F7_B301_292EBF828891__INCLUDED_)
