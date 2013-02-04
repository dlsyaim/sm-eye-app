#if !defined(AFX_MARKSSYMBOL_H__B0EE614B_265C_4A76_A311_45D819761D3C__INCLUDED_)
#define AFX_MARKSSYMBOL_H__B0EE614B_265C_4A76_A311_45D819761D3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CPen1;
class CGradient;
class CTeeShadow;
class CBrush1;

/////////////////////////////////////////////////////////////////////////////
// CMarksSymbol wrapper class

class CMarksSymbol : public COleDispatchDriver
{
public:
	CMarksSymbol() {}		// Calls COleDispatchDriver default constructor
	CMarksSymbol(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CMarksSymbol(const CMarksSymbol& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetBevel();
	void SetBevel(long nNewValue);
	long GetBevelWidth();
	void SetBevelWidth(long nNewValue);
	CPen1 GetFrame();
	CGradient GetGradient();
	CTeeShadow GetShadow();
	long GetShapeStyle();
	void SetShapeStyle(long nNewValue);
	long GetTransparency();
	void SetTransparency(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	CPen1 GetPen();
	CBrush1 GetBrush();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKSSYMBOL_H__B0EE614B_265C_4A76_A311_45D819761D3C__INCLUDED_)