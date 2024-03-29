#if !defined(AFX_CUSTOMFUNCTION_H__277E3125_45FB_4C86_AC08_A63029F14854__INCLUDED_)
#define AFX_CUSTOMFUNCTION_H__277E3125_45FB_4C86_AC08_A63029F14854__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CCustomFunction wrapper class

class CCustomFunction : public COleDispatchDriver
{
public:
	CCustomFunction() {}		// Calls COleDispatchDriver default constructor
	CCustomFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCustomFunction(const CCustomFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetNumPoints();
	void SetNumPoints(long nNewValue);
	double GetStartX();
	void SetStartX(double newValue);
	double GetX();
	void SetX(double newValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMFUNCTION_H__277E3125_45FB_4C86_AC08_A63029F14854__INCLUDED_)
