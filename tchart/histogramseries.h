#if !defined(AFX_HISTOGRAMSERIES_H__649A8879_BB99_4D89_A22F_0FE0AF7893B1__INCLUDED_)
#define AFX_HISTOGRAMSERIES_H__649A8879_BB99_4D89_A22F_0FE0AF7893B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CPen1;
class CBrush1;

/////////////////////////////////////////////////////////////////////////////
// CHistogramSeries wrapper class

class CHistogramSeries : public COleDispatchDriver
{
public:
	CHistogramSeries() {}		// Calls COleDispatchDriver default constructor
	CHistogramSeries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CHistogramSeries(const CHistogramSeries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CPen1 GetLinesPen();
	long GetTransparency();
	void SetTransparency(long nNewValue);
	CPen1 GetLinePen();
	CBrush1 GetBrush();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTOGRAMSERIES_H__649A8879_BB99_4D89_A22F_0FE0AF7893B1__INCLUDED_)
