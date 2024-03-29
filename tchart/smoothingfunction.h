#if !defined(AFX_SMOOTHINGFUNCTION_H__ED30CEF1_61F1_4864_BC09_93BB32B8BCFB__INCLUDED_)
#define AFX_SMOOTHINGFUNCTION_H__ED30CEF1_61F1_4864_BC09_93BB32B8BCFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CSmoothingFunction wrapper class

class CSmoothingFunction : public COleDispatchDriver
{
public:
	CSmoothingFunction() {}		// Calls COleDispatchDriver default constructor
	CSmoothingFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSmoothingFunction(const CSmoothingFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetInterpolate();
	void SetInterpolate(BOOL bNewValue);
	long GetFactor();
	void SetFactor(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMOOTHINGFUNCTION_H__ED30CEF1_61F1_4864_BC09_93BB32B8BCFB__INCLUDED_)
