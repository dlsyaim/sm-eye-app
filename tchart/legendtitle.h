#if !defined(AFX_LEGENDTITLE_H__E2AF2E61_37FA_424E_B3B9_4ADB0B9F7A7F__INCLUDED_)
#define AFX_LEGENDTITLE_H__E2AF2E61_37FA_424E_B3B9_4ADB0B9F7A7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CGradient;
class CChartFont;
class CTeeShadow;
class CStrings;

/////////////////////////////////////////////////////////////////////////////
// CLegendTitle wrapper class

class CLegendTitle : public COleDispatchDriver
{
public:
	CLegendTitle() {}		// Calls COleDispatchDriver default constructor
	CLegendTitle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CLegendTitle(const CLegendTitle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetTransparency();
	void SetTransparency(long nNewValue);
	long GetBevel();
	void SetBevel(long nNewValue);
	long GetBevelWidth();
	void SetBevelWidth(long nNewValue);
	unsigned long GetColor();
	void SetColor(unsigned long newValue);
	CGradient GetGradient();
	CChartFont GetFont();
	CTeeShadow GetShadow();
	long GetShapeStyle();
	void SetShapeStyle(long nNewValue);
	CStrings GetText();
	long GetTextAlignment();
	void SetTextAlignment(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	BOOL GetTransparent();
	void SetTransparent(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEGENDTITLE_H__E2AF2E61_37FA_424E_B3B9_4ADB0B9F7A7F__INCLUDED_)
