// DlgEnterName.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgEnterName.h"
#include ".\dlgentername.h"


// CDlgEnterName 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgEnterName, CDialog)
CDlgEnterName::CDlgEnterName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnterName::IDD, pParent)
{
}

CDlgEnterName::~CDlgEnterName()
{
}

void CDlgEnterName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEnterName, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgEnterName 메시지 처리기입니다.

void CDlgEnterName::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_EDIT_NEW_NAME)->GetWindowText(this->m_strNewName);
	OnOK();
}
