#if !defined(AFX_JPEGEXPORT_H__E6D93322_9AFB_4EEE_BAA8_6B2851DBF7E8__INCLUDED_)
#define AFX_JPEGEXPORT_H__E6D93322_9AFB_4EEE_BAA8_6B2851DBF7E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CJPEGExport wrapper class

class CJPEGExport : public COleDispatchDriver
{
public:
	CJPEGExport() {}		// Calls COleDispatchDriver default constructor
	CJPEGExport(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CJPEGExport(const CJPEGExport& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

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
	BOOL GetGrayScale();
	void SetGrayScale(BOOL bNewValue);
	long GetCompressionQuality();
	void SetCompressionQuality(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JPEGEXPORT_H__E6D93322_9AFB_4EEE_BAA8_6B2851DBF7E8__INCLUDED_)
