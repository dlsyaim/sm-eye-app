#if !defined(AFX_WALL_H__042BECA8_DA6A_4F20_BB4F_D0BC1591B218__INCLUDED_)
#define AFX_WALL_H__042BECA8_DA6A_4F20_BB4F_D0BC1591B218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CBrush1;
class CPen1;
class CGradient;
class CBackImage;

/////////////////////////////////////////////////////////////////////////////
// CWall wrapper class

class CWall : public COleDispatchDriver
{
public:
	CWall() {}		// Calls COleDispatchDriver default constructor
	CWall(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CWall(const CWall& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CBrush1 GetBrush();
	unsigned long GetColor();
	void SetColor(unsigned long newValue);
	CPen1 GetPen();
	long GetSize();
	void SetSize(long nNewValue);
	BOOL GetTransparent();
	void SetTransparent(BOOL bNewValue);
	BOOL GetDark3D();
	void SetDark3D(BOOL bNewValue);
	CGradient GetGradient();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	long GetTransparency();
	void SetTransparency(long nNewValue);
	BOOL GetAutoHide();
	void SetAutoHide(BOOL bNewValue);
	CBackImage GetPicture();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WALL_H__042BECA8_DA6A_4F20_BB4F_D0BC1591B218__INCLUDED_)
