#if !defined(AFX_ROTATEFILTER_H__859FBAA1_AB3D_4815_B3DC_07A73D146D47__INCLUDED_)
#define AFX_ROTATEFILTER_H__859FBAA1_AB3D_4815_B3DC_07A73D146D47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CRotateFilter wrapper class

class CRotateFilter : public COleDispatchDriver
{
public:
	CRotateFilter() {}		// Calls COleDispatchDriver default constructor
	CRotateFilter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CRotateFilter(const CRotateFilter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	double GetAngle();
	void SetAngle(double newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	unsigned long GetBackColor();
	void SetBackColor(unsigned long newValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATEFILTER_H__859FBAA1_AB3D_4815_B3DC_07A73D146D47__INCLUDED_)