#if !defined(AFX_GRIDBANDTOOL_H__16FD69A0_D49A_41A3_987E_6B7285D7C758__INCLUDED_)
#define AFX_GRIDBANDTOOL_H__16FD69A0_D49A_41A3_987E_6B7285D7C758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CGridBandBrush;

/////////////////////////////////////////////////////////////////////////////
// CGridBandTool wrapper class

class CGridBandTool : public COleDispatchDriver
{
public:
	CGridBandTool() {}		// Calls COleDispatchDriver default constructor
	CGridBandTool(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CGridBandTool(const CGridBandTool& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	VARIANT GetAxis();
	void SetAxis(const VARIANT& newValue);
	CGridBandBrush GetBand1();
	CGridBandBrush GetBand2();
	unsigned long GetBandBackColor(long ABand);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDBANDTOOL_H__16FD69A0_D49A_41A3_987E_6B7285D7C758__INCLUDED_)
