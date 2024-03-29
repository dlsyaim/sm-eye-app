#if !defined(AFX_TEEEDITOROPTIONS_H__404B3F00_749D_43A9_8EBB_CF28F9AD3A0D__INCLUDED_)
#define AFX_TEEEDITOROPTIONS_H__404B3F00_749D_43A9_8EBB_CF28F9AD3A0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CTeeEditorOptions wrapper class

class CTeeEditorOptions : public COleDispatchDriver
{
public:
	CTeeEditorOptions() {}		// Calls COleDispatchDriver default constructor
	CTeeEditorOptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CTeeEditorOptions(const CTeeEditorOptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetAdd();
	void SetAdd(BOOL bNewValue);
	BOOL GetDelete();
	void SetDelete(BOOL bNewValue);
	BOOL GetChange();
	void SetChange(BOOL bNewValue);
	BOOL GetClone();
	void SetClone(BOOL bNewValue);
	BOOL GetDataSource();
	void SetDataSource(BOOL bNewValue);
	BOOL GetTitle();
	void SetTitle(BOOL bNewValue);
	BOOL GetHelp();
	void SetHelp(BOOL bNewValue);
	BOOL GetGroups();
	void SetGroups(BOOL bNewValue);
	BOOL GetAllGroups();
	void SetAllGroups(BOOL bNewValue);
	BOOL GetOrderSeries();
	void SetOrderSeries(BOOL bNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEEEDITOROPTIONS_H__404B3F00_749D_43A9_8EBB_CF28F9AD3A0D__INCLUDED_)
