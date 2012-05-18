// HeaderCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "HeaderCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx

CHeaderCtrlEx::CHeaderCtrlEx()
{
}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlEx, CHeaderCtrl)
//{{AFX_MSG_MAP(CHeaderCtrlEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx message handlers



void CHeaderCtrlEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	
	ASSERT(lpDrawItemStruct->CtlType == ODT_HEADER);
	
	HDITEM hdi;
	TCHAR  lpBuffer[256];
	
	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;
	
	GetItem(lpDrawItemStruct->itemID, &hdi);
   	
	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	//lpDrawItemStruct->rcItem.bottom = lpDrawItemStruct->rcItem.bottom*2;
	//THIS FONT IS ONLY FOR DRAWING AS LONG AS WE DON'T DO A SetFont(...)
	LOGFONT logfont;
	CFont bgFont,*pOldFont;
	logfont.lfHeight=m_lHeight;
	logfont.lfWidth=0;
	logfont.lfEscapement=0;
	logfont.lfOrientation=0;
	logfont.lfWeight=FW_BOLD;
	logfont.lfItalic=FALSE;
	logfont.lfUnderline=FALSE;
	logfont.lfStrikeOut=FALSE;
	logfont.lfCharSet=DEFAULT_CHARSET;
	logfont.lfOutPrecision=OUT_CHARACTER_PRECIS;
	logfont.lfClipPrecision=CLIP_CHARACTER_PRECIS;
	logfont.lfQuality=DEFAULT_QUALITY;
	logfont.lfPitchAndFamily=DEFAULT_PITCH;//|FF_DONTCARE;
	strcpy(logfont.lfFaceName, m_strFaceName.GetBuffer(m_strFaceName.GetLength()));
	bgFont.CreateFontIndirect(&logfont);
	pOldFont=(CFont*)pDC->SelectObject(&bgFont);

	//pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
	// Draw the button frame.
	::DrawFrameControl(lpDrawItemStruct->hDC, 
		&lpDrawItemStruct->rcItem, DFC_BUTTON, DFCS_BUTTONPUSH);
	
	UINT uFormat = DT_CENTER;
	
	//DRAW THE TEXT
	//::DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer), &lpDrawItemStruct->rcItem, uFormat);
	//pDC->SelectStockObject(SYSTEM_FONT);
	DrawTextEx(pDC, lpBuffer, &lpDrawItemStruct->rcItem, uFormat);
	pDC->SelectObject(pOldFont);
}


int CHeaderCtrlEx::DrawTextEx(CDC *pDC, const CString &str, CRect rect, UINT nFormat)
{
	int nLineCount = 0;
	
	if(str.GetLength())
	{
		nLineCount = 1;
		int nPos = -1;
		
		while((nPos = str.Find('\n', nPos+1)) > 0)
			nLineCount++;
	}
	
	CSize szText = pDC->GetTextExtent(str);
	szText.cy = szText.cy * nLineCount;
	
	// 프린트시 위아래(음수값)가 바뀔수 있음..
	int nHeight = rect.Height();
	if(nHeight < 0)
		nHeight = nHeight + szText.cy;
	else
		nHeight = nHeight - szText.cy;
	
	rect.top = rect.top + (nHeight / 2);
	
	return pDC->DrawText(str, rect, nFormat);
}
