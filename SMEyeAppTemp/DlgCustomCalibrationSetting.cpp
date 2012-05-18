// DlgCustomCalibrationSetting.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgCustomCalibrationSetting.h"
#include ".\dlgcustomcalibrationsetting.h"


// CDlgCustomCalibrationSetting ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgCustomCalibrationSetting, CDialog)
CDlgCustomCalibrationSetting::CDlgCustomCalibrationSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCustomCalibrationSetting::IDD, pParent)
{
	m_dLeftRightDegree = 10;
	m_dTopBottomDegree = 10;

}

CDlgCustomCalibrationSetting::~CDlgCustomCalibrationSetting()
{
}

void CDlgCustomCalibrationSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCustomCalibrationSetting, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCustomCalibrationSetting �޽��� ó�����Դϴ�.

void CDlgCustomCalibrationSetting::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	GetDlgItem(IDC_EDIT_CUSTOM_LR)->GetWindowText(str);
	this->m_dLeftRightDegree = atof(str);
	GetDlgItem(IDC_EDIT_CUSTOM_TB)->GetWindowText(str);
	this->m_dTopBottomDegree = atof(str);

	OnOK();
}

BOOL CDlgCustomCalibrationSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CString str;
	str.Format("%d", int(m_dLeftRightDegree+.5));
	GetDlgItem(IDC_EDIT_CUSTOM_LR)->SetWindowText(str);
	str.Format("%d", int(this->m_dTopBottomDegree+.5));
	GetDlgItem(IDC_EDIT_CUSTOM_TB)->SetWindowText(str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
