// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "axislabelsitem.h"

// Dispatch interfaces referenced by this interface
#include "TeeFont.h"
#include "gradient.h"
#include "teerect.h"
#include "TeeShadow.h"
#include "pen.h"
#include "backimage.h"


/////////////////////////////////////////////////////////////////////////////
// CAxisLabelsItem properties

/////////////////////////////////////////////////////////////////////////////
// CAxisLabelsItem operations

unsigned long CAxisLabelsItem::GetColor()
{
	unsigned long result;
	InvokeHelper(0x6c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CTeeFont CAxisLabelsItem::GetFont()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x68, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CTeeFont(pDispatch);
}

CGradient CAxisLabelsItem::GetGradient()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x65, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CGradient(pDispatch);
}

long CAxisLabelsItem::GetShadowSize()
{
	long result;
	InvokeHelper(0x3f4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetShadowSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3f4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

unsigned long CAxisLabelsItem::GetShadowColor()
{
	unsigned long result;
	InvokeHelper(0x66, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetShadowColor(unsigned long newValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x66, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL CAxisLabelsItem::GetTransparent()
{
	BOOL result;
	InvokeHelper(0x3f5, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetTransparent(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x3f5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long CAxisLabelsItem::GetShapeStyle()
{
	long result;
	InvokeHelper(0x67, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetShapeStyle(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x67, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CAxisLabelsItem::GetTransparency()
{
	long result;
	InvokeHelper(0x7e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetTransparency(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x7e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CTeeRect CAxisLabelsItem::GetShapeBounds()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x85, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CTeeRect(pDispatch);
}

long CAxisLabelsItem::GetBevel()
{
	long result;
	InvokeHelper(0x139, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetBevel(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x139, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long CAxisLabelsItem::GetBevelWidth()
{
	long result;
	InvokeHelper(0x13a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetBevelWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void CAxisLabelsItem::ShowEditorShape()
{
	InvokeHelper(0xc9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CTeeShadow CAxisLabelsItem::GetShadow()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CTeeShadow(pDispatch);
}

long CAxisLabelsItem::GetRoundSize()
{
	long result;
	InvokeHelper(0xcb, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetRoundSize(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xcb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CPen1 CAxisLabelsItem::GetPen()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xcc, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CPen1(pDispatch);
}

CBackImage CAxisLabelsItem::GetPicture()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CBackImage(pDispatch);
}

void CAxisLabelsItem::Show()
{
	InvokeHelper(0xce, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CAxisLabelsItem::Hide()
{
	InvokeHelper(0xcf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

double CAxisLabelsItem::GetValue()
{
	double result;
	InvokeHelper(0x12d, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetValue(double newValue)
{
	static BYTE parms[] =
		VTS_R8;
	InvokeHelper(0x12d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString CAxisLabelsItem::GetText()
{
	CString result;
	InvokeHelper(0x12e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x12e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL CAxisLabelsItem::GetVisible()
{
	BOOL result;
	InvokeHelper(0x12f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CAxisLabelsItem::SetVisible(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x12f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}
