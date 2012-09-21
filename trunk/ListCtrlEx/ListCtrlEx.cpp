// ListCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "ListCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

CListCtrlEx::CListCtrlEx()
{
	m_colRow1 = RGB(240,247,249);
	m_colRow2 = RGB(229,232,239);
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	//{{AFX_MSG_MAP(CListCtrlEx)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//ON_WM_MEASUREITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers
/*
void CListCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	TCHAR  lpBuffer[256];
	
	LV_ITEM lvi;
	
	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
	lvi.iItem = lpDrawItemStruct->itemID ; 	
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer ;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));
	
	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;
	
	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		if ( nCol > 0 ) 
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol-1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left ;	
		}
		
		// Get the text 
		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
		
		CDC* pDC;
		pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		
		if ( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT)) ; 
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)) ;
		}
		else
		{
			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW)) ;
			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT)) ; 
		}
		
		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
		
		UINT		uFormat    = DT_LEFT ;
		
		::DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer), 
			&lpDrawItemStruct->rcItem, uFormat) ;
		
		pDC->SelectStockObject(SYSTEM_FONT) ;
	}
}*/

BOOL CListCtrlEx::Initializing(int nPointSize, LPCTSTR lpszFaceName, CDC *pDC)
{
	// 리스트 콘트롤 멀티라인 헤더
	m_NewHeaderFont.CreatePointFont(nPointSize, lpszFaceName);
	m_HeaderCtrlEx.SetHeaderFont(nPointSize/20, lpszFaceName);
	//m_NewDataFont.CreateFont(nPointSize/20-)
	
	CHeaderCtrl* pHeader = NULL;
	pHeader = GetHeaderCtrl();
	
	if(pHeader==NULL)
		return FALSE;
	
	VERIFY(m_HeaderCtrlEx.SubclassWindow(pHeader->m_hWnd));	
	
	// 위에서 생성한 폰트가 클수록 콘트롤이 더 커진다.
	m_HeaderCtrlEx.SetFont(&m_NewHeaderFont);
	
	HDITEM hdItem;
	
	hdItem.mask = HDI_FORMAT;
	
	for(int i=0; i<m_HeaderCtrlEx.GetItemCount(); i++)
	{
		m_HeaderCtrlEx.GetItem(i,&hdItem);
		
		if(i==m_HeaderCtrlEx.GetItemCount()-1)
			hdItem.fmt|= HDF_LEFT|HDF_OWNERDRAW;
		else
			hdItem.fmt|= HDF_CENTER|HDF_OWNERDRAW;
		
		m_HeaderCtrlEx.SetItem(i,&hdItem);
	}

	return TRUE;
}

BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	CListCtrlEx::GetClientRect(rect);
	
	POINT mypoint;  
	
	CBrush brush0(m_colRow1);
	CBrush brush1(m_colRow2);
	
	int chunk_height=GetCountPerPage();
	pDC->FillRect(&rect,&brush1);
	
	for (int i=0;i<=chunk_height;i++)
	{
		GetItemPosition(i,&mypoint);
		rect.top=mypoint.y ;
		GetItemPosition(i+1,&mypoint);
		rect.bottom =mypoint.y;
		pDC->FillRect(&rect,i %2 ? &brush1 : &brush0);
		
	}
	brush0.DeleteObject();
	brush1.DeleteObject();
	
	//return FALSE;
	return CListCtrl::OnEraseBkgnd(pDC);
}

void CListCtrlEx::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	int iRow = lplvcd->nmcd.dwItemSpec;
	
	switch(lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT :
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			return;
		}
		
		// Modify item text and or background
	case CDDS_ITEMPREPAINT:
		{
			lplvcd->clrText = RGB(0,0,0);
			// If you want the sub items the same as the item,
			// set *pResult to CDRF_NEWFONT
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			return;
		}
		
		// Modify sub item text and/or background
	case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
		{
			if(iRow %2){
				lplvcd->clrTextBk = m_colRow2;
			}
			else{
				lplvcd->clrTextBk = m_colRow1;
			}
			*pResult = CDRF_DODEFAULT;
			return;
		}
	}	
}
