#if !defined(AFX_BANNERTOOL_H__865DAF1D_3C06_4870_BFA1_940D3A2E356C__INCLUDED_)
#define AFX_BANNERTOOL_H__865DAF1D_3C06_4870_BFA1_940D3A2E356C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CTeeShapePanel;
class CAnnotationCallout;
class CBrush1;

/////////////////////////////////////////////////////////////////////////////
// CBannerTool wrapper class

class CBannerTool : public COleDispatchDriver
{
public:
	CBannerTool() {}		// Calls COleDispatchDriver default constructor
	CBannerTool(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CBannerTool(const CBannerTool& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetPosition();
	void SetPosition(long nNewValue);
	CTeeShapePanel GetShape();
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	CAnnotationCallout GetCallout();
	long GetTextAlignment();
	void SetTextAlignment(long nNewValue);
	BOOL Clicked(long X, long Y);
	long GetWidth();
	void SetWidth(long nNewValue);
	long GetHeight();
	void SetHeight(long nNewValue);
	short GetCursor();
	void SetCursor(short nNewValue);
	long GetLeft();
	void SetLeft(long nNewValue);
	long GetTop();
	void SetTop(long nNewValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CBrush1 GetBrush();
	// method 'GetBounds' not emitted because of invalid return type or parameter type
	void SetBounds(long Left, long Top, long Right, long Bottom);
	BOOL GetBlink();
	void SetBlink(BOOL bNewValue);
	long GetBlinkDelayOff();
	void SetBlinkDelayOff(long nNewValue);
	long GetBlinkDelayOn();
	void SetBlinkDelayOn(long nNewValue);
	long GetScrollDelay();
	void SetScrollDelay(long nNewValue);
	long GetScrollDirection();
	void SetScrollDirection(long nNewValue);
	BOOL GetScroll();
	void SetScroll(BOOL bNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANNERTOOL_H__865DAF1D_3C06_4870_BFA1_940D3A2E356C__INCLUDED_)
