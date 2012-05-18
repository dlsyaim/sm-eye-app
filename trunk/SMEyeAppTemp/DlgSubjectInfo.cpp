// DlgSubjectInfo.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgSubjectInfo.h"
#include ".\dlgsubjectinfo.h"


// CDlgSubjectInfo ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgSubjectInfo, CDialog)
CDlgSubjectInfo::CDlgSubjectInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubjectInfo::IDD, pParent)
{
}

CDlgSubjectInfo::~CDlgSubjectInfo()
{
}

void CDlgSubjectInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSubjectInfo, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSubjectInfo �޽��� ó�����Դϴ�.

void CDlgSubjectInfo::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strName, strID, strMemo, strAge;
	int age, sex;


	GetDlgItem(IDC_EDIT_PATIENT_NAME)->GetWindowText(strName);
	GetDlgItem(IDC_EDIT_PATIENT_ID)->GetWindowText(strID);
	GetDlgItem(IDC_EDIT_PATIENT_MEMO)->GetWindowText(strMemo);

	sex = ((CButton*)GetDlgItem(IDC_RADIO_FEMAIL))->GetCheck() ? 0 : 1;
	GetDlgItem(IDC_EDIT_PATIENT_AGE)->GetWindowText(strAge);
	age = atoi(strAge);

	if(strName.GetLength() == 0)
	{
		::AfxMessageBox("A name is a must");
		return;
	}
	else if(strID.GetLength() == 0)
	{
		::AfxMessageBox("A ID is a must");
		return;
	}

	::ZeroMemory(m_PI.m_chName, MAX_PI_NAME);
	::ZeroMemory(m_PI.m_chID, MAX_PI_ID);
	::ZeroMemory(m_PI.m_chMemo, MAX_PI_MEMO);
	memcpy(m_PI.m_chName, strName.GetBuffer(), min(strName.GetLength(), MAX_PI_NAME-1));
	memcpy(m_PI.m_chID, strID.GetBuffer(), min(strID.GetLength(), MAX_PI_ID-1));
	memcpy(m_PI.m_chMemo, strMemo.GetBuffer(), min(strMemo.GetLength(), MAX_PI_MEMO-1));

	m_PI.m_nAge = age;
	m_PI.m_nSex = sex;

	m_PI.m_timeTime = this->m_time;

	OnOK();
	
}

BOOL CDlgSubjectInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_time = CTime::GetCurrentTime();

	CString str;
	str.Format("%d-%d-%d  %d:%d:%d", 
		m_time.GetYear(), m_time.GetMonth(), m_time.GetDay(),
		m_time.GetHour(), m_time.GetMinute(), m_time.GetSecond());
	GetDlgItem(IDC_STATIC_PATIENT_DATE)->SetWindowText(str);
	
	GetDlgItem(IDC_EDIT_PATIENT_NAME)->SetWindowText(m_PI.m_chName);
	GetDlgItem(IDC_EDIT_PATIENT_ID)->SetWindowText(m_PI.m_chID);
	str.Format("%d", m_PI.m_nAge);
	GetDlgItem(IDC_EDIT_PATIENT_AGE)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_PATIENT_MEMO)->SetWindowText(m_PI.m_chMemo);

	((CButton*)GetDlgItem(m_PI.m_nSex ? IDC_RADIO_MALE : IDC_RADIO_FEMALE))->SetCheck(true);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

