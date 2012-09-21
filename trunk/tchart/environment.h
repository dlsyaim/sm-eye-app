#if !defined(AFX_ENVIRONMENT_H__0C37C5E1_0FB7_4C5A_81EA_BE6686463857__INCLUDED_)
#define AFX_ENVIRONMENT_H__0C37C5E1_0FB7_4C5A_81EA_BE6686463857__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CEnvironment wrapper class

class CEnvironment : public COleDispatchDriver
{
public:
	CEnvironment() {}		// Calls COleDispatchDriver default constructor
	CEnvironment(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CEnvironment(const CEnvironment& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetNoPromptDBConnect();
	void SetNoPromptDBConnect(BOOL bNewValue);
	BOOL GetIEPrintWithPage();
	void SetIEPrintWithPage(BOOL bNewValue);
	void InternalRepaint();
	BOOL GetMouseWheelScroll();
	void SetMouseWheelScroll(BOOL bNewValue);
	CString GetDecimalSeparator();
	BOOL GetTeeClipWhenPrinting();
	void SetTeeClipWhenPrinting(BOOL bNewValue);
	BOOL GetTeeClipWhenMetafiling();
	void SetTeeClipWhenMetafiling(BOOL bNewValue);
	CString DecodeUTF8String(LPCTSTR aString);
	CString CodeUTF8String(LPCTSTR aString, BOOL addPrefix);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENVIRONMENT_H__0C37C5E1_0FB7_4C5A_81EA_BE6686463857__INCLUDED_)
