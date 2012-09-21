// DlgSelectSpontaneous.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgSelectSpontaneous.h"
#include ".\dlgselectspontaneous.h"
#include "Analyzenystagmus2.h"
#include "eu_testfilev2.h"


// CDlgSelectSpontaneous 대화 상자입니다.

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


// CDlgSelectSpontaneous 메시지 처리기입니다.

void CDlgSelectSpontaneous::OnBnClickedRadioSponManualSelect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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

	//초기화
	str = "0";
	GetDlgItem(IDC_EDIT_SPON_MANUAL_LEFT_VALUE)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_SPON_MANUAL_RIGHT_VALUE)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_SPON_TEST_LEFT_VALUE)->SetWindowText(str);
	GetDlgItem(IDC_EDIT_SPON_TEST_RIGHT_VALUE)->SetWindowText(str);



	//값을 ctrl에 적는다.
	if(m_bManualSelect)
	{
		str.Format("%.1f", this->m_dSpontaneousValue[0]);
		GetDlgItem(IDC_EDIT_SPON_MANUAL_LEFT_VALUE)->SetWindowText(str);

		str.Format("%.1f", this->m_dSpontaneousValue[1]);
		GetDlgItem(IDC_EDIT_SPON_MANUAL_RIGHT_VALUE)->SetWindowText(str);

	}

	//tree를 load 한다.
	m_patientInfo.load(m_strSubjectFolder + PATIENT_INFO_FILENAME);
	m_treeSelectCompensation.LoadFromXML(this->m_strSubjectFolder + TEST_LIST_FILENAME, 
							this->m_strSubjectFolder + TEST_INFO_FILENAME);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgSelectSpontaneous::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
	//자발 안진 값을 계산한다.

	CFileStatus fs;
	CString str;
	for(int i=0; i<2; i++)
	{
		//nystagmus 를 load한다.
		//analyze 파일이 있다면 load하고 없다면 analyze한다.
		CAnalyzeNystagmus2 analyze;
		str = m_strSpontaneousFileName;
		str.Replace(".emd", i==0 ? "L.ema" : "R.ema");
		if(!CFile::GetStatus(str, fs))
		{
			//analyze file이 없다면 eye movement data를 읽어와 분석한다.
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
		else	        //analyze 파일 로드한다. 
			analyze.load(str);

		// nystagmus 의 평균 속도를 계산한다.
		this->m_dSpontaneousValue[i] = analyze.getMean(ALL);
	}

	
	//select
	//CString str;
	str.Format("%.1f", m_dSpontaneousValue[0]);
	GetDlgItem(IDC_EDIT_SPON_TEST_LEFT_VALUE)->SetWindowText(str);
	str.Format("%.1f", m_dSpontaneousValue[1]);
	GetDlgItem(IDC_EDIT_SPON_TEST_RIGHT_VALUE)->SetWindowText(str);

}