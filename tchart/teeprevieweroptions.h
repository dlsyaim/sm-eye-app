#if !defined(AFX_TEEPREVIEWEROPTIONS_H__D5A02291_1D8C_453A_8D54_982B302C9076__INCLUDED_)
#define AFX_TEEPREVIEWEROPTIONS_H__D5A02291_1D8C_453A_8D54_982B302C9076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CTeePreviewerOptions wrapper class

class CTeePreviewerOptions : public COleDispatchDriver
{
public:
	CTeePreviewerOptions() {}		// Calls COleDispatchDriver default constructor
	CTeePreviewerOptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTeePreviewerOptions(const CTeePreviewerOptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetChangePrinter();
	void SetChangePrinter(BOOL bNewValue);
	BOOL GetSetupPrinter();
	void SetSetupPrinter(BOOL bNewValue);
	BOOL GetResizeImage();
	void SetResizeImage(BOOL bNewValue);
	BOOL GetMoveImage();
	void SetMoveImage(BOOL bNewValue);
	BOOL GetChangeDetail();
	void SetChangeDetail(BOOL bNewValue);
	BOOL GetChangePaperOrientation();
	void SetChangePaperOrientation(BOOL bNewValue);
	BOOL GetChangeMargins();
	void SetChangeMargins(BOOL bNewValue);
	BOOL GetDragImage();
	void SetDragImage(BOOL bNewValue);
	BOOL GetPrintPanel();
	void SetPrintPanel(BOOL bNewValue);
	BOOL GetAsBitmap();
	void SetAsBitmap(BOOL bNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEEPREVIEWEROPTIONS_H__D5A02291_1D8C_453A_8D54_982B302C9076__INCLUDED_)
