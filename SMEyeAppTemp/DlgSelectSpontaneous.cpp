// DlgSelectSpontaneous.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgSelectSpontaneous.h"
#include ".\dlgselectspontaneous.h"
#include "Analyzenystagmus2.h"
#include "eu_testfilev2.h"


// CDlgSelectSpontaneous ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgSelectSpontaneous, CDialog)
CDlgSelectSpontaneous::CDlgSelectSpontaneous(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectSpontaneous::IDD, pParent)
{
	m_dSpontaneousValue[0] = m_dSpontaneousValue[1] = 0;
	this->m_bManualSelect = true;
}

CDlgSelectSpontaneous::~CDlgSelectSpontaneous()
{
	m_nEyeSelect = 0;
}

void CDlgSelectSpontaneous::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SELECT_COMPENSATION, m_treeSelectCompensation);
}


BEGIN_MESSAGE_MAP(CDlgSelectSpontaneous, CDialog)
	ON_BN_CLICKED(IDC_RADIO_SPON_MANUAL_SELECT, OnBnClickedRadioSponManualSelect)
	ON_BN_CLICKED(IDC_RADIO_SPON_TEST_SELECT, OnBnClickedRadioSponTestSelect)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SELECT_COMPENSATION, OnTvnSelchangedTreeSelectCompensation)
END_MESSAGE_MAP()


// CDlgSelectSpontaneous �޽��� ó�����Դϴ�.

void CDlgSelectSpontaneous::OnBnClickedRadioSponManualSelect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bManualSelect = true;

	GetDlgItem(IDC_EDIT_SPON_MANUAL_LEFT_VALUE)->EnableWindow(m_bManualSelect);
	GetDlgItem(IDC_EDIT_SPON_MANUAL_RIGHT_VALUE)->EnableWindow(m_bManualSelect);
	
	//select
	m_treeSelectCompensation.EnableWindow(!m_bManualSelect);
	GetDlgItem(IDC_EDIT_SPON_TEST_LEFT_VALUE)->EnableWindow(!m_bManualSelect);
	GetDlgItem(IDC_EDIT_SPON_TEST_RIGHT_VALUE)->EnableWindow(!m_bManualSelect);
}

void CDlgSelectSpontaneous::OnBnClickedRadioSponTestSelect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bManualSelect = false;
	
	GetDlgItem(IDC_EDIT_SPON_MANUAL_LEFT_VALUE)->EnableWindow(m_bManualSelect);
	GetDlgItem(IDC_EDIT_SPON_MANUAL_RIGHT_VALUE)->EnableWindow(m_bManualSelect);
	
	//select
	m_treeSelectCompensation.EnableWindow(!m_bManualSelect);
	GetDlgItem(IDC_EDIT_SPON_TEST_LEFT_VALUE)->EnableWindow(!m_bManualSelect);
	GetDlgItem(IDC_EDIT_SPON_TEST_RIGHT_VALUE)->EnableWindow(!m_bManualSelect);
}

BOOL CDlgSelectSpontaneous::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//manual
	((CButton*)GetDlgItem(IDC_RADIO_SPON_MANUAL_SELECT))->SetCheck(m_bManualSelect);
	GetDlgItem(IDC_EDIT_SPON_MANUAL_LEFT_VALUE)->EnableWindow(m_bManualSelect);
	GetDlgItem(IDC_EDIT_SPON_MANUAL_RIGHT_VALUE)->EnableWindow(m_bManualSelect);
	
	//select
	((CButton*)GetDlgItem(IDC_RADIO_SPON_TEST_SELECT))->SetCheck(!m_bManualSelect);
	GetDlgItem(IDC_EDIT_SPON_TEST_LEFT_VALUE)->EnableWindow(!m_bManualSelect);
	GetDlgItem(IDC_EDIT_SPON_TEST_RIGHT_VALUE)->EnableWindow(!m_bManualSelect);
	GetDlgItem(IDC_TREE_SELECT_COMPENSATION)->EnableWindow(!m_bManualSelect);
	

    CString str;

	//�ʱ�ȭ
	str = "0";
	GetDlgItem(IDC_EDIT_SPON_MANUAL_LEFT_VALUE)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_SPON_MANUAL_RIGHT_VALUE)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_SPON_TEST_LEFT_VALUE)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_SPON_TEST_RIGHT_VALUE)->SetWindowText(str);



	//���� ctrl�� ���´�.
	if(m_bManualSelect)
	{
		str.Format("%.1f", this->m_dSpontaneousValue[0]);
		GetDlgItem(IDC_EDIT_SPON_MANUAL_LEFT_VALUE)->SetWindowText(str);

		str.Format("%.1f", this->m_dSpontaneousValue[1]);
		GetDlgItem(IDC_EDIT_SPON_MANUAL_RIGHT_VALUE)->SetWindowText(str);

	}

	//tree�� load �Ѵ�.
	m_patientInfo.load(m_strSubjectFolder + PATIENT_INFO_FILENAME);
	m_treeSelectCompensation.LoadFromXML(this->m_strSubjectFolder + TEST_LIST_FILENAME, 
							this->m_strSubjectFolder + TEST_INFO_FILENAME);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgSelectSpontaneous::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	if(this->m_bManualSelect)
	{
        GetDlgItem(IDC_EDIT_SPON_MANUAL_LEFT_VALUE)->GetWindowText(str);
		this->m_dSpontaneousValue[0] = atof(str);
		GetDlgItem(IDC_EDIT_SPON_MANUAL_RIGHT_VALUE)->GetWindowText(str);
		this->m_dSpontaneousValue[1] = atof(str);
	}
	else
	{
        GetDlgItem(IDC_EDIT_SPON_TEST_LEFT_VALUE)->GetWindowText(str);
		this->m_dSpontaneousValue[0] = atof(str);
		GetDlgItem(IDC_EDIT_SPON_TEST_RIGHT_VALUE)->GetWindowText(str);
		this->m_dSpontaneousValue[1] = atof(str);
	}

	OnOK();
}


void CDlgSelectSpontaneous::OnTvnSelchangedTreeSelectCompensation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	ED_TestInfo* pTI = this->m_treeSelectCompensation.getTestInfoFromItem(hItem);

	if(pTI)
	{
		this->m_strSpontaneousFileName.Format("%s%s", this->m_strSubjectFolder,pTI->m_chFileName);
		getSpontaneousValue();
	}

	*pResult = 0;
}


void CDlgSelectSpontaneous::getSpontaneousValue()
{
	//�ڹ� ���� ���� ����Ѵ�.

	CFileStatus fs;
	CString str;
	for(int i=0; i<2; i++)
	{
		//nystagmus �� load�Ѵ�.
		//analyze ������ �ִٸ� load�ϰ� ���ٸ� analyze�Ѵ�.
		CAnalyzeNystagmus2 analyze;
		str = m_strSpontaneousFileName;
		str.Replace(".emd", i==0 ? "L.ema" : "R.ema");
		if(!CFile::GetStatus(str, fs))
		{
			//analyze file�� ���ٸ� eye movement data�� �о�� �м��Ѵ�.
			double* pEye = NULL;
			unsigned long count = 0;
			
			EU_TestFileV2 testFile;
			ED_TestInfo testInfo;
			if(testFile.open(str, NULL, false))
			{
				testFile.readTI(&testInfo);
				count = testInfo.m_lEyeDataCount;
				if(count)
				{
				
					pEye = new double[count];
					testFile.readTestData(true, (i==LEFT)?pEye:NULL, (i==RIGHT)?pEye:NULL, NULL, NULL);

					analyze.analyze(pEye, count);
					//this->m_analyzeNys[i].analyze(pEye, count);
					delete []pEye;
				}
			}

			
		}
		else	        //analyze ���� �ε��Ѵ�. 
			analyze.load(str);

		// nystagmus �� ��� �ӵ��� ����Ѵ�.
		this->m_dSpontaneousValue[i] = analyze.getMean(ALL);
	}

	
	//select
	//CString str;
	str.Format("%.1f", m_dSpontaneousValue[0]);
	GetDlgItem(IDC_EDIT_SPON_TEST_LEFT_VALUE)->SetWindowText(str);
	str.Format("%.1f", m_dSpontaneousValue[1]);
	GetDlgItem(IDC_EDIT_SPON_TEST_RIGHT_VALUE)->SetWindowText(str);

}