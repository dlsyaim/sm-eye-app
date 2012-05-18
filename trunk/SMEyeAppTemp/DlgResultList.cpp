// DlgResultList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgResultList.h"
#include ".\dlgresultlist.h"
#include "corwlUtil.h"
#include "corwlDefines.h"


// CDlgResultList ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgResultList, CDialog)
CDlgResultList::CDlgResultList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgResultList::IDD, pParent)
{
}

CDlgResultList::~CDlgResultList()
{
}

void CDlgResultList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TEST_LIST, m_ListTest);
}


BEGIN_MESSAGE_MAP(CDlgResultList, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TEST_LIST, OnNMClickListTestList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TEST_LIST, OnNMDblclkListTestList)
END_MESSAGE_MAP()


// CDlgResultList �޽��� ó�����Դϴ�.

BOOL CDlgResultList::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if(corFolderExist(this->m_strDataFolder))
	{
		initTestList();
		fillTestList(m_strDataFolder);
	}
	else
	{
		CString strMessage;
		strMessage.Format("Save Folder is not exist.(%s)", this->m_strDataFolder);
		::AfxMessageBox(strMessage);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}




void CDlgResultList::initTestList()
{
	int n = 0;
	
	DWORD dwStyle = m_ListTest.GetExStyle();
	dwStyle = (LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListTest.SetExtendedStyle(dwStyle);

	CRect rect;
	m_ListTest.GetClientRect(&rect);
	int w = rect.Width();

	char* list[8] = {"", "Name", "ID", "Age", "Date", "Time", "Sex", "folder"};
	double width[8] = {.1, .8/5,.8/5 ,.8/5,.8/5, .8/5, .1, 0};

	for(int i=0; i<8; i++)
		m_ListTest.InsertColumn(i, list[i], LVCFMT_CENTER, int(w*width[i]));
}

void CDlgResultList::fillTestList(CString strFolderName)
{
	//���� '\' Ȯ��
	if(strFolderName.GetAt(strFolderName.GetLength()-1) != CString("\\"))
		strFolderName += "\\";

    CFileFind FileFinder;
    strFolderName = strFolderName + _T("*.*");
    BOOL bWorking = FileFinder.FindFile(strFolderName); 

	ED_PatientInfo patientInfo;
	CString folderName, fname;

	int idx = 0;
    while ( (bWorking) ) 
    {
        bWorking = FileFinder.FindNextFile(); 

        if ( FileFinder.IsDots() ) 
			continue; 
        if ( FileFinder.IsDirectory() )
		{
			//���� name�� ��� ����.
            folderName = FileFinder.GetFilePath();
			fname = folderName + "\\" + PATIENT_INFO_FILENAME;
			if(patientInfo.load(fname))
			{
				//patient info�� �д´�.
                add2TestList(&patientInfo, folderName, idx+1);
				idx++;
			}
		}
    }
}
void CDlgResultList::add2TestList(ED_PatientInfo* pPI, CString folderName, int idx)
{
	CString str;
	str.Format("%d" ,idx);
	int nIdx = m_ListTest.InsertItem(0, str);
	

	str.Format("%s" , pPI->m_chName);
	m_ListTest.SetItemText(nIdx, 1, str);

	str.Format("%s" , pPI->m_chID);
	m_ListTest.SetItemText(nIdx, 2, str);

	str.Format("%d", pPI->m_nAge);
	m_ListTest.SetItemText(nIdx, 3, str);


	str.Format("%04d/%02d/%02d", pPI->m_timeTime.GetYear(), pPI->m_timeTime.GetMonth(), pPI->m_timeTime.GetDay());
	m_ListTest.SetItemText(nIdx, 4, str);

	str.Format("%02d:%02d:%02d", pPI->m_timeTime.GetHour(), pPI->m_timeTime.GetMinute(), pPI->m_timeTime.GetSecond());
	m_ListTest.SetItemText(nIdx, 5, str);

	str = pPI->m_nSex ? "M":"F";
	m_ListTest.SetItemText(nIdx, 6, str);

	m_ListTest.SetItemText(nIdx, 7, folderName);
}

void CDlgResultList::OnNMClickListTestList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
    long lRow = 0;                                  // �ο�
    int nCol = 0;                                   // �÷�

    lRow = temp->iItem;                             // �ο찪(����Ŭ���� �ο츮��)
    nCol = temp->iSubItem;                          // �÷���(����Ŭ���� �÷����� : �� ��� �� �����������..)

	//longSelectedRow = lRow;
	m_strSelectedFolder = this->m_ListTest.GetItemText(lRow, 7);

	//Show Memo
	showMemo(m_strSelectedFolder + "\\" + PATIENT_INFO_FILENAME);
}

void CDlgResultList::OnNMDblclkListTestList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE) pNMHDR;
    long lRow = 0;                                  // �ο�
    int nCol = 0;                                   // �÷�

    lRow = temp->iItem;                             // �ο찪(����Ŭ���� �ο츮��)
    nCol = temp->iSubItem;                          // �÷���(����Ŭ���� �÷����� : �� ��� �� �����������..)

	//longSelectedRow = lRow;
	m_strSelectedFolder = this->m_ListTest.GetItemText(lRow, 7);

	if(m_strSelectedFolder != "")
		OnOK();
}


void CDlgResultList::showMemo(CString fname)
{
	ED_PatientInfo patientInfo;
	
	if(patientInfo.load(fname))
		GetDlgItem(IDC_STATIC_LIST_MEMO)->SetWindowText(patientInfo.m_chMemo);
	
}

CString CDlgResultList::getSelectedFolder()
{ 
	//���� "\" Ȯ��
	if(m_strSelectedFolder != "")
	{
		if(m_strSelectedFolder.GetAt(m_strSelectedFolder.GetLength()-1) != CString("\\"))
			m_strSelectedFolder += "\\";
	}
	return this->m_strSelectedFolder;
};