#if !defined(AFX_SPACING_H__D87E3B59_356D_421F_B355_93D0E65D1296__INCLUDED_)
#define AFX_SPACING_H__D87E3B59_356D_421F_B355_93D0E65D1296__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CSpacing wrapper class

class CSpacing : public COleDispatchDriver
{
public:
	CSpacing() {}		// Calls COleDispatchDriver default constructor
	CSpacing(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSpacing(const CSpacing& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetHorizontal();
	void SetHorizontal(long nNewValue);
	long GetVertical();
	void SetVertical(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPACING_H__D87E3B59_356D_421F_B355_93D0E65D1296__INCLUDED_)
