#if !defined(AFX_LISTCTRLEX_H__3E6294FD_2953_4C48_A347_6D300ED5B606__INCLUDED_)
#define AFX_LISTCTRLEX_H__3E6294FD_2953_4C48_A347_6D300ED5B606__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListCtrlEx.h : header file
//

#include "HeaderCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx window

class CListCtrlEx : public CListCtrl
{
// Construction
public:
	CListCtrlEx();

// Attributes
public:
	COLORREF m_colRow1;
	COLORREF m_colRow2;

	CFont			m_NewHeaderFont, m_NewDataFont;
	CHeaderCtrlEx	m_HeaderCtrlEx;

	BOOL Initializing(int nPointSize, LPCTSTR lpszFaceName, CDC *pDC=NULL);
	//afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void CListCtrlEx::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCTRLEX_H__3E6294FD_2953_4C48_A347_6D300ED5B606__INCLUDED_)
