#if !defined(AFX_TEECOMMANDER_H__072D26CC_6B10_4344_B24D_6B6EC490A502__INCLUDED_)
#define AFX_TEECOMMANDER_H__072D26CC_6B10_4344_B24D_6B6EC490A502__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CTeeCommanderButton;
class CGradient;

/////////////////////////////////////////////////////////////////////////////
// CTeeCommander wrapper class

class CTeeCommander : public CWnd
{
protected:
	DECLARE_DYNCREATE(CTeeCommander)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xdcab77d9, 0x709f, 0x4bb0, { 0x92, 0xb9, 0x5c, 0xfe, 0x7a, 0x51, 0x70, 0xeb } };
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
	BOOL GetLabelValues();
	void SetLabelValues(BOOL bNewValue);
	unsigned long GetColor();
	void SetColor(unsigned long newValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	void SetEditor(LPDISPATCH newValue);
	void SetPreviewer(LPDISPATCH newValue);
	void SetEditorLink(long nNewValue);
	void SetPreviewerLink(long nNewValue);
	BOOL GetVertical();
	void SetVertical(BOOL bNewValue);
	VARIANT GetControls();
	void SetControls(const VARIANT& newValue);
	VARIANT GetButtons();
	void SetButtons(const VARIANT& newValue);
	long GetBevelInner();
	void SetBevelInner(long nNewValue);
	long GetBevelOuter();
	void SetBevelOuter(long nNewValue);
	long GetBevelWidth();
	void SetBevelWidth(long nNewValue);
	long GetBevelOffset();
	void SetBevelOffset(long nNewValue);
	long GetBorderStyle();
	void SetBorderStyle(long nNewValue);
	long GetDefaultButton();
	void SetDefaultButton(long nNewValue);
	CTeeCommanderButton GetButton3D();
	CTeeCommanderButton GetButtonCopy();
	CTeeCommanderButton GetButtonDepth();
	CTeeCommanderButton GetButtonEdit();
	CTeeCommanderButton GetButtonMove();
	CTeeCommanderButton GetButtonNormal();
	CTeeCommanderButton GetButtonPrint();
	CTeeCommanderButton GetButtonRotate();
	CTeeCommanderButton GetButtonSave();
	CTeeCommanderButton GetButtonZoom();
	void RepositionControls();
	CGradient GetGradient();
	BOOL GetFullRotation();
	void SetFullRotation(BOOL bNewValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEECOMMANDER_H__072D26CC_6B10_4344_B24D_6B6EC490A502__INCLUDED_)
