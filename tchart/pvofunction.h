#if !defined(AFX_PVOFUNCTION_H__C0C23147_1423_450C_8135_D48A9552D780__INCLUDED_)
#define AFX_PVOFUNCTION_H__C0C23147_1423_450C_8135_D48A9552D780__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CPVOFunction wrapper class

class CPVOFunction : public COleDispatchDriver
{
public:
	CPVOFunction() {}		// Calls COleDispatchDriver default constructor
	CPVOFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPVOFunction(const CPVOFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetPercentage();
	void SetPercentage(BOOL bNewValue);
	long GetPeriod2();
	void SetPeriod2(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PVOFUNCTION_H__C0C23147_1423_450C_8135_D48A9552D780__INCLUDED_)
