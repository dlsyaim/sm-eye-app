#if !defined(AFX_DATATABLETOOL_H__86E2AFF9_54A9_45C3_BF7A_5B7D7819E0F1__INCLUDED_)
#define AFX_DATATABLETOOL_H__86E2AFF9_54A9_45C3_BF7A_5B7D7819E0F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CBrush1;
class CPen1;
class CChartFont;
class CTableLegend;

/////////////////////////////////////////////////////////////////////////////
// CDataTableTool wrapper class

class CDataTableTool : public COleDispatchDriver
{
public:
	CDataTableTool() {}		// Calls COleDispatchDriver default constructor
	CDataTableTool(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDataTableTool(const CDataTableTool& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetAutoPosition();
	void SetAutoPosition(BOOL bNewValue);
	CBrush1 GetBrush();
	BOOL GetClipText();
	void SetClipText(BOOL bNewValue);
	CPen1 GetColumnPen();
	CChartFont GetFont();
	BOOL GetInverted();
	void SetInverted(BOOL bNewValue);
	long GetLeft();
	void SetLeft(long nNewValue);
	CTableLegend GetLegend();
	CPen1 GetRowPen();
	long GetTop();
	void SetTop(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATATABLETOOL_H__86E2AFF9_54A9_45C3_BF7A_5B7D7819E0F1__INCLUDED_)
