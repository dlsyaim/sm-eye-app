// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "environment.h"


/////////////////////////////////////////////////////////////////////////////
// CEnvironment properties

/////////////////////////////////////////////////////////////////////////////
// CEnvironment operations

BOOL CEnvironment::GetNoPromptDBConnect()
{
	BOOL result;
	InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CEnvironment::SetNoPromptDBConnect(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CEnvironment::GetIEPrintWithPage()
{
	BOOL result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CEnvironment::SetIEPrintWithPage(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void CEnvironment::InternalRepaint()
{
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL CEnvironment::GetMouseWheelScroll()
{
	BOOL result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CEnvironment::SetMouseWheelScroll(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString CEnvironment::GetDecimalSeparator()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL CEnvironment::GetTeeClipWhenPrinting()
{
	BOOL result;
	InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CEnvironment::SetTeeClipWhenPrinting(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL CEnvironment::GetTeeClipWhenMetafiling()
{
	BOOL result;
	InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CEnvironment::SetTeeClipWhenMetafiling(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xca, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString CEnvironment::DecodeUTF8String(LPCTSTR aString)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xcb, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		aString);
	return result;
}

CString CEnvironment::CodeUTF8String(LPCTSTR aString, BOOL addPrefix)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0xcc, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		aString, addPrefix);
	return result;
}
