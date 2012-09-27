#if !defined(AFX_PAGE_H__696F9BF5_C60B_47B3_B185_21AB7C84DC84__INCLUDED_)
#define AFX_PAGE_H__696F9BF5_C60B_47B3_B185_21AB7C84DC84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CPage wrapper class

class CPage : public COleDispatchDriver
{
public:
	CPage() {}		// Calls COleDispatchDriver default constructor
	CPage(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPage(const CPage& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Next();
	void Previous();
	long GetMaxPointsPerPage();
	void SetMaxPointsPerPage(long nNewValue);
	long GetCurrent();
	void SetCurrent(long nNewValue);
	BOOL GetScaleLastPage();
	void SetScaleLastPage(BOOL bNewValue);
	long GetCount();
	long GetFirstValueIndex();
	BOOL GetAutoScale();
	void SetAutoScale(BOOL bNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE_H__696F9BF5_C60B_47B3_B185_21AB7C84DC84__INCLUDED_)