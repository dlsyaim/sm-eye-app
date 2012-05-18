// DlgOptionHeadThrust.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgOptionHeadThrust.h"
#include ".\dlgoptionheadthrust.h"


// CDlgOptionHeadThrust ��ȭ �����Դϴ�.

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


// CDlgOptionHeadThrust �޽��� ó�����Դϴ�.

BOOL CDlgOptionHeadThrust::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgOptionHeadThrust::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->m_bUseMaxHeadSpeedLimit = ((CButton*)GetDlgItem(IDC_CHECK_LIMIT_MAX_HEAD_SPEED))->GetCheck() == BST_CHECKED;
	GetDlgItem(IDC_EDIT_LIMIT_MAX_HEAD_SPEED)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	GetDlgItem(IDC_STATIC_MAX_HEAD_SPEAD)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	GetDlgItem(IDC_EDIT_LIMIT_MIN_HEAD_SPEED)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	GetDlgItem(IDC_STATIC_MIN_HEAD_SPEAD)->EnableWindow(this->m_bUseMaxHeadSpeedLimit);
	
}
