#if !defined(AFX_XAMLEXPORT_H__72F3F7C7_6985_4CF1_B2B1_CAC917E1E763__INCLUDED_)
#define AFX_XAMLEXPORT_H__72F3F7C7_6985_4CF1_B2B1_CAC917E1E763__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CXAMLExport wrapper class

class CXAMLExport : public COleDispatchDriver
{
public:
	CXAMLExport() {}		// Calls COleDispatchDriver default constructor
	CXAMLExport(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CXAMLExport(const CXAMLExport& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetWidth();
	void SetWidth(long nNewValue);
	long GetHeight();
	void SetHeight(long nNewValue);
	void SaveToFile(LPCTSTR FileName);
	VARIANT SaveToStream();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XAMLEXPORT_H__72F3F7C7_6985_4CF1_B2B1_CAC917E1E763__INCLUDED_)
