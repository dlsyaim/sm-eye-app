#if !defined(AFX_GAMMACORRECTIONFILTER_H__54C641F9_39CD_4628_AD60_2AEB8B5C7DCC__INCLUDED_)
#define AFX_GAMMACORRECTIONFILTER_H__54C641F9_39CD_4628_AD60_2AEB8B5C7DCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CGammaCorrectionFilter wrapper class

class CGammaCorrectionFilter : public COleDispatchDriver
{
public:
	CGammaCorrectionFilter() {}		// Calls COleDispatchDriver default constructor
	CGammaCorrectionFilter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGammaCorrectionFilter(const CGammaCorrectionFilter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetPercent();
	void SetPercent(BOOL bNewValue);
	long GetAmount();
	void SetAmount(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMMACORRECTIONFILTER_H__54C641F9_39CD_4628_AD60_2AEB8B5C7DCC__INCLUDED_)
