#if !defined(AFX_COLORFILTER_H__CD5CAC71_6C66_4E84_8E2D_3E79012DCFE0__INCLUDED_)
#define AFX_COLORFILTER_H__CD5CAC71_6C66_4E84_8E2D_3E79012DCFE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CColorFilter wrapper class

class CColorFilter : public COleDispatchDriver
{
public:
	CColorFilter() {}		// Calls COleDispatchDriver default constructor
	CColorFilter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CColorFilter(const CColorFilter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetRed();
	void SetRed(long nNewValue);
	long GetGreen();
	void SetGreen(long nNewValue);
	long GetBlue();
	void SetBlue(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORFILTER_H__CD5CAC71_6C66_4E84_8E2D_3E79012DCFE0__INCLUDED_)
