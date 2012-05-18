// DlgPrintSetup.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgPrintSetup.h"
#include ".\dlgprintsetup.h"
#include "corwlDefines.h"

#define WM_ITEMCHECKED	WM_USER+5

// CDlgPrintSetup ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgPrintSetup, CDialog)
CDlgPrintSetup::CDlgPrintSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrintSetup::IDD, pParent)
{
	m_pTestInfo = NULL;
}

CDlgPrintSetup::~CDlgPrintSetup()
{
}

void CDlgPrintSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PRINT, m_treePrint);
}


BEGIN_MESSAGE_MAP(CDlgPrintSetup, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PRINT, OnNMClickTreePrint)
	ON_MESSAGE(WM_ITEMCHECKED, OnTreeChecked)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnBnClickedButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_NONE, OnBnClickedButtonSelectNone)
END_MESSAGE_MAP()


// CDlgPrintSetup �޽��� ó�����Դϴ�.

BOOL CDlgPrintSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_patientInfo.load(m_strSubjectFolder + PATIENT_INFO_FILENAME);

	m_treePrint.LoadFromXML(this->m_strSubjectFolder + TEST_LIST_FILENAME, 
							this->m_strSubjectFolder + TEST_INFO_FILENAME);
	
	//m_treePrint.deletePlainItem();	//data�� �ִ� �༮�� ���� ���´�.
	m_treePrint.ModifyStyle( TVS_CHECKBOXES, 0 );
	m_treePrint.ModifyStyle( 0, TVS_CHECKBOXES );
	m_treePrint.setPrintCheck();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgPrintSetup::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CDlgPrintSetup::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->m_treePrint.SaveToXML(this->m_strXMLName, this->m_strTIName);
	OnOK();
}

void CDlgPrintSetup::OnNMClickTreePrint(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	//hit tet
	UINT uFlag = 0;

	CPoint point;
	GetCursorPos(&point);
	this->m_treePrint.ScreenToClient(&point);

	//���õ� item�� ��´�.
	HTREEITEM hItem = m_treePrint.HitTest(point, &uFlag);


	m_pTestInfo = m_treePrint.getTestInfoFromItem(hItem);

    //item�� Ŭ���ϰ�
	if((hItem != NULL) && (TVHT_ONITEM & uFlag))
	{
		// ti�� ������
		if(m_pTestInfo)
		{
			//since the check state does not change until this function returns
			PostMessage(WM_ITEMCHECKED,(WPARAM)hItem, 0);
			
		}
	}

	*pResult = 0;
}

afx_msg LRESULT CDlgPrintSetup::OnTreeChecked(WPARAM wParam, LPARAM lParam)
{
	if(m_pTestInfo)
	{
		HTREEITEM hItem = (HTREEITEM)wParam;
		m_pTestInfo->m_ucPrint = m_treePrint.GetCheck(hItem);
	}

	return 1;
}


void CDlgPrintSetup::OnBnClickedButtonSelectAll()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->m_treePrint.setPrintCheck(1);
}


void CDlgPrintSetup::OnBnClickedButtonSelectNone()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->m_treePrint.setPrintCheck(-1);
}
