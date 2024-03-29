#if !defined(AFX_SCROLLBARTHUMB_H__E5BA86F0_88C1_412F_99B9_52FD1948B05B__INCLUDED_)
#define AFX_SCROLLBARTHUMB_H__E5BA86F0_88C1_412F_99B9_52FD1948B05B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CBrush1;
class CGradient;
class CTeePicture;

/////////////////////////////////////////////////////////////////////////////
// CScrollBarThumb wrapper class

class CScrollBarThumb : public COleDispatchDriver
{
public:
	CScrollBarThumb() {}		// Calls COleDispatchDriver default constructor
	CScrollBarThumb(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CScrollBarThumb(const CScrollBarThumb& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CBrush1 GetBrush();
	CGradient GetGradient();
	CTeePicture GetImage();
	long GetSize();
	void SetSize(long nNewValue);
	long GetStyle();
	void SetStyle(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLBARTHUMB_H__E5BA86F0_88C1_412F_99B9_52FD1948B05B__INCLUDED_)
