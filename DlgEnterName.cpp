// DlgEnterName.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgEnterName.h"
#include ".\dlgentername.h"


// CDlgEnterName ��ȭ �����Դϴ�.

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


// CDlgEnterName �޽��� ó�����Դϴ�.

void CDlgEnterName::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	GetDlgItem(IDC_EDIT_NEW_NAME)->GetWindowText(this->m_strNewName);
	OnOK();
}
