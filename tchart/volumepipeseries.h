#if !defined(AFX_VOLUMEPIPESERIES_H__83CB8B4E_5251_44A5_8EDE_0486ABE2388B__INCLUDED_)
#define AFX_VOLUMEPIPESERIES_H__83CB8B4E_5251_44A5_8EDE_0486ABE2388B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CGradient;
class CPen1;

/////////////////////////////////////////////////////////////////////////////
// CVolumePipeSeries wrapper class

class CVolumePipeSeries : public COleDispatchDriver
{
public:
	CVolumePipeSeries() {}		// Calls COleDispatchDriver default constructor
	CVolumePipeSeries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CVolumePipeSeries(const CVolumePipeSeries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetConePercent();
	void SetConePercent(long nNewValue);
	CGradient GetGradient();
	CPen1 GetLinesPen();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMEPIPESERIES_H__83CB8B4E_5251_44A5_8EDE_0486ABE2388B__INCLUDED_)
