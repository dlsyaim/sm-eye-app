#if !defined(AFX_FUNNELSERIES_H__11530DF6_81E0_4829_8103_985720B382AC__INCLUDED_)
#define AFX_FUNNELSERIES_H__11530DF6_81E0_4829_8103_985720B382AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CBrush1;
class CPen1;
class CValueList;

/////////////////////////////////////////////////////////////////////////////
// CFunnelSeries wrapper class

class CFunnelSeries : public COleDispatchDriver
{
public:
	CFunnelSeries() {}		// Calls COleDispatchDriver default constructor
	CFunnelSeries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CFunnelSeries(const CFunnelSeries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long AddSegment(double AQuote, double AOpportunity, LPCTSTR ALabel, unsigned long AColor);
	void Recalc();
	CBrush1 GetBrush();
	CPen1 GetPen();
	CPen1 GetLinesPen();
	BOOL GetAutoUpdate();
	void SetAutoUpdate(BOOL bNewValue);
	BOOL GetQuotesSorted();
	void SetQuotesSorted(BOOL bNewValue);
	double GetDifferenceLimit();
	void SetDifferenceLimit(double newValue);
	unsigned long GetAboveColor();
	void SetAboveColor(unsigned long newValue);
	unsigned long GetWithinColor();
	void SetWithinColor(unsigned long newValue);
	unsigned long GetBelowColor();
	void SetBelowColor(unsigned long newValue);
	CValueList GetOpportunityValues();
	CValueList GetQuoteValues();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNNELSERIES_H__11530DF6_81E0_4829_8103_985720B382AC__INCLUDED_)
