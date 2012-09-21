// DlgCustomCalibrationSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgCustomCalibrationSetting.h"
#include ".\dlgcustomcalibrationsetting.h"


// CDlgCustomCalibrationSetting 대화 상자입니다.

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


// CDlgCustomCalibrationSetting 메시지 처리기입니다.

void CDlgCustomCalibrationSetting::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString str;
	str.Format("%d", int(m_dLeftRightDegree+.5));
	GetDlgItem(IDC_EDIT_CUSTOM_LR)->SetWindowText(str);
	str.Format("%d", int(this->m_dTopBottomDegree+.5));
	GetDlgItem(IDC_EDIT_CUSTOM_TB)->SetWindowText(str);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
