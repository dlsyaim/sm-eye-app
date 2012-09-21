// DlgOptionHeadThrust.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgOptionHeadThrust.h"
#include ".\dlgoptionheadthrust.h"


// CDlgOptionHeadThrust 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgOptionHeadThrust, CDialog)
CDlgOptionHeadThrust::CDlgOptionHeadThrust(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOptionHeadThrust::IDD, pParent)
{
	loadParameters();
}

CDlgOptionHeadThrust::~CDlgOptionHeadThrust()
{
}

void CDlgOptionHeadThrust::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgOptionHeadThrust, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_LIMIT_MAX_HEAD_SPEED, OnBnClickedCheckLimitMaxHeadSpeed)
END_MESSAGE_MAP()


// CDlgOptionHeadThrust 메시지 처리기입니다.

BOOL CDlgOptionHeadThrust::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString str;
	

	//set max value
	str.Format("%d", int(this->m_dMaxHeadSpeedLimit+.5));
	GetDlgItem(IDC_EDIT_LIMIT_MAX_HEAD_SPEED)->SetWindowText(str);


	//set min value
	str.Format("%d", int(this->m_dMinHeadSpeedLimit+.5));
	GetDlgItem(IDC_EDIT_LIMIT_MIN_HEAD_SPEED)->SetWindowText(str);


	//check and enable
	((CButton*)GetDlgItem(IDC_CHECK_LIMIT_MAX_HEAD_SPEED))->SetCheck(m_bUseMaxHeadSpeedLimit ? BST_CHECKED : BST_UNCHECKED);
	GetDlgItem(IDC_EDIT_LIMIT_MAX_HEAD_SPEED)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	GetDlgItem(IDC_STATIC_MAX_HEAD_SPEAD)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	GetDlgItem(IDC_EDIT_LIMIT_MIN_HEAD_SPEED)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	GetDlgItem(IDC_STATIC_MIN_HEAD_SPEAD)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgOptionHeadThrust::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	//max
	GetDlgItem(IDC_EDIT_LIMIT_MAX_HEAD_SPEED)->GetWindowText(str);
	this->m_dMaxHeadSpeedLimit = atoi(str);
	//min
	GetDlgItem(IDC_EDIT_LIMIT_MIN_HEAD_SPEED)->GetWindowText(str);
	this->m_dMinHeadSpeedLimit = atoi(str);



	saveParameters();

	OnOK();
}

void CDlgOptionHeadThrust::loadParameters()
{
	this->m_bUseMaxHeadSpeedLimit = ::AfxGetApp()->GetProfileInt("EvalBVA", "HEADTHRUST_USE_MAX_HEAD_SPEED_LIMIT", 0);
	this->m_dMaxHeadSpeedLimit = ::AfxGetApp()->GetProfileInt("EvalBVA", "HEADTHRUST_MAX_HEAD_SPEED_LIMIT", 300);
	this->m_dMinHeadSpeedLimit = ::AfxGetApp()->GetProfileInt("EvalBVA", "HEADTHRUST_MIN_HEAD_SPEED_LIMIT", 100);
}

void CDlgOptionHeadThrust::saveParameters()
{
	::AfxGetApp()->WriteProfileInt("EvalBVA", "HEADTHRUST_USE_MAX_HEAD_SPEED_LIMIT", m_bUseMaxHeadSpeedLimit ? 1 : 0);
	::AfxGetApp()->WriteProfileInt("EvalBVA", "HEADTHRUST_MAX_HEAD_SPEED_LIMIT", m_dMaxHeadSpeedLimit);
	::AfxGetApp()->WriteProfileInt("EvalBVA", "HEADTHRUST_MIN_HEAD_SPEED_LIMIT", m_dMinHeadSpeedLimit);
}
void CDlgOptionHeadThrust::OnBnClickedCheckLimitMaxHeadSpeed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	this->m_bUseMaxHeadSpeedLimit = ((CButton*)GetDlgItem(IDC_CHECK_LIMIT_MAX_HEAD_SPEED))->GetCheck() == BST_CHECKED;
	GetDlgItem(IDC_EDIT_LIMIT_MAX_HEAD_SPEED)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	GetDlgItem(IDC_STATIC_MAX_HEAD_SPEAD)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	GetDlgItem(IDC_EDIT_LIMIT_MIN_HEAD_SPEED)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	GetDlgItem(IDC_STATIC_MIN_HEAD_SPEAD)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	
}
