#if !defined(AFX_CHARTEDITORPANEL_H__E4464794_6F6E_40E7_9D0C_A05F537B2FC8__INCLUDED_)
#define AFX_CHARTEDITORPANEL_H__E4464794_6F6E_40E7_9D0C_A05F537B2FC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CChartEditorPanel wrapper class

class CChartEditorPanel : public CWnd
{
protected:
	DECLARE_DYNCREATE(CChartEditorPanel)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xbfbfcf89, 0xfe34, 0x422f, { 0x98, 0x83, 0x25, 0xf4, 0x43, 0x87, 0xb1, 0x38 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	void SetChart(LPDISPATCH newValue);
	void SetChartLink(long nNewValue);
	void AboutBox();
	void SelectUnderMouse();
	long GetBevelOuter();
	void SetBevelOuter(long nNewValue);
	void SetEditor(LPDISPATCH newValue);
	void SetEditorLink(long nNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTEDITORPANEL_H__E4464794_6F6E_40E7_9D0C_A05F537B2FC8__INCLUDED_)
