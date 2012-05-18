// DlgTestOption.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgTestOption.h"
#include ".\dlgtestoption.h"


// CDlgTestOption ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgTestOption, CDialog)
CDlgTestOption::CDlgTestOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestOption::IDD, pParent)
{

	this->m_pTestInfo = NULL;

	m_stiRadioID[0] = IDC_RADIO_STI_CALIBRATION;
	m_stiRadioID[1] = IDC_RADIO_STI_GAZE_A;
	m_stiRadioID[2] = IDC_RADIO_STI_GAZE_B;
	m_stiRadioID[3] = IDC_RADIO_STI_PURSUIT;
	m_stiRadioID[4] = IDC_RADIO_STI_SACCADE_A;
	m_stiRadioID[5] = IDC_RADIO_STI_SACCADE_B;
	m_stiRadioID[6] = IDC_RADIO_STI_SACCADE_RANDOM;
	m_stiRadioID[7] = IDC_RADIO_STI_OKN;
	m_stiRadioID[8] = IDC_RADIO_STI_LED;
	m_stiRadioID[9] = IDC_RADIO_STI_CALORIC;
}

CDlgTestOption::~CDlgTestOption()
{
}

void CDlgTestOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ANAL_METHOD, m_comboAnalMethod);
	DDX_Control(pDX, IDC_COMBO_ANAL_COMPONENT, m_comboAnalTarget);
	DDX_Control(pDX, IDC_COMBO_STIMULATION_LIST, m_comboStiSubmode);
}


BEGIN_MESSAGE_MAP(CDlgTestOption, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ANAL_METHOD, OnCbnSelchangeComboAnalMethod)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_STI_CALIBRATION, OnBnClickedRadioStimulation)
	ON_BN_CLICKED(IDC_RADIO_STI_GAZE_A, OnBnClickedRadioStimulation)
	ON_BN_CLICKED(IDC_RADIO_STI_GAZE_B, OnBnClickedRadioStimulation)
	ON_BN_CLICKED(IDC_RADIO_STI_PURSUIT, OnBnClickedRadioStimulation)
	ON_BN_CLICKED(IDC_RADIO_STI_SACCADE_A, OnBnClickedRadioStimulation)
	ON_BN_CLICKED(IDC_RADIO_STI_SACCADE_B, OnBnClickedRadioStimulation)
	ON_BN_CLICKED(IDC_RADIO_STI_SACCADE_RANDOM, OnBnClickedRadioStimulation)
	ON_BN_CLICKED(IDC_RADIO_STI_OKN, OnBnClickedRadioStimulation)
	ON_BN_CLICKED(IDC_RADIO_STI_LED, OnBnClickedRadioStimulation)
	ON_BN_CLICKED(IDC_RADIO_STI_CALORIC, OnBnClickedRadioStimulation)
	
	

END_MESSAGE_MAP()


// CDlgTestOption �޽��� ó�����Դϴ�.

BOOL CDlgTestOption::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if(!m_pTestInfo)
	{
		::AfxMessageBox("No test info");
		return TRUE;
	}

	//test name
	GetDlgItem(IDC_EDIT_TEST_NAME)->SetWindowText(this->m_strTestName);
	
	
	//analysis method
	for(int i=0; i<ANAL_METHOD_NUM; i++)
		this->m_comboAnalMethod.AddString(ED_TestProtocole::getAnalMethodName(i));
	m_comboAnalMethod.SetCurSel(this->m_pTestInfo->m_analMethod);

	//analysis src
	for(int i=0; i<ANAL_TARGET_NUM; i++)
		this->m_comboAnalTarget.AddString(ED_TestProtocole::getAnalTargetName(i));
	m_comboAnalTarget.SetCurSel(m_pTestInfo->m_analTarget);

	unsigned char chartVisBit[CHART_NUM] = {HORIZONTAL, VERTICAL, TORSIONAL, RADIUS, ACC, RT_PROC};

	//chart visibility
	for(int i=0; i<CHART_NUM-1;	i++)	//analysis�� ���α׷����� �ڵ����� �������� �����Ѵ�.
	{
		if(this->m_pTestInfo->m_showChart & (1<<chartVisBit[i]))
			((CButton*)GetDlgItem(IDC_CHECK_CHART_H+i))->SetCheck(true);
	}

	//CHECK REAL TIME PROCESSING
	if(this->m_pTestInfo->m_showChart & (1<<RT_PROC) )
		((CButton*)GetDlgItem(IDC_CHECK_REAL_TIME_ANAL))->SetCheck(true);
	else
		((CButton*)GetDlgItem(IDC_CHECK_REAL_TIME_ANAL))->SetCheck(false);


	//stimulation mode
	unsigned char stiModeCommand = m_pTestInfo->m_stiModeCommand;
	unsigned char mode = 0;
	for(int i=0; i<STI_NUM; i++)
	{
		if(ED_TestProtocole::getStiModeCommand(i) == m_pTestInfo->m_stiModeCommand)
		{
			mode = i;
			break;
		}
	}

	((CButton*)GetDlgItem(m_stiRadioID[mode]))->SetCheck(true);


	//add submode
	int submodeCount = ED_TestProtocole::getStiSubmodeNum(mode);
	for(int i=0; i<submodeCount; i++)
		this->m_comboStiSubmode.AddString(ED_TestProtocole::getStiSubmodeName(mode, i));
	unsigned char stiSubmode = m_pTestInfo->m_stiSubmode;
	m_comboStiSubmode.SetCurSel(stiSubmode);
	GetDlgItem(IDC_STATIC_STI_DESCRIPTION)->SetWindowText(ED_TestProtocole::getStiSubmodeName(mode, stiSubmode));



	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgTestOption::OnCbnSelchangeComboAnalMethod()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	

	//
}

void CDlgTestOption::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_pTestInfo)
	{
		CString str;
		//test name
		GetDlgItem(IDC_EDIT_TEST_NAME)->GetWindowText(str);

		this->m_strTestName = str;

		//analysis method
		m_pTestInfo->m_analMethod = m_comboAnalMethod.GetCurSel();

		//analysis src
		m_pTestInfo->m_analTarget = m_comboAnalTarget.GetCurSel();

		//chart visibility
		unsigned char chartVis = 0;
		for(int i=0; i<CHART_NUM-1;	i++)	//analysis�� ���α׷����� �ڵ����� �������� �����Ѵ�.
		{
			if(((CButton*)GetDlgItem(IDC_CHECK_CHART_H+i))->GetCheck())
			{
				chartVis +=  1<<i;
			}
		}

		//REAL TIME processing�� �ؾ� �ϸ� �ش� íƮ�� �����ش�.
		//if(((CButton*)GetDlgItem(IDC_CHECK_REAL_TIME_ANAL))->GetCheck() == BST_CHECKED)
          //  chartVis += 1<<RT_PROC;
		


		m_pTestInfo->m_showChart = chartVis;

		//stimulation mode
		for(int i=0; i< STI_NUM; i++)
		{
			if(((CButton*)GetDlgItem(this->m_stiRadioID[i]))->GetCheck())
			{
				m_pTestInfo->m_stiModeCommand = ED_TestProtocole::getStiModeCommand(i);
				break;
			}
		}
		
		

		//stimulation submode
		m_pTestInfo->m_stiSubmode = m_comboStiSubmode.GetCurSel();

	}
	

	OnOK();
}

void CDlgTestOption::OnBnClickedRadioStimulation()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// get stimulation type

	unsigned char mode = 0;
	for(int i=0; i<STI_NUM; i++)
	{
		if( ((CButton*)GetDlgItem(m_stiRadioID[i]))->GetCheck() )
		{
			mode = i;
			m_pTestInfo->m_stiModeCommand = ED_TestProtocole::getStiModeCommand(i);
			break;
		}
	}

	//clear prev submode
	unsigned int count = m_comboStiSubmode.GetCount();
	for(int i=count-1; i>=0; i--)
		m_comboStiSubmode.DeleteString(i);

    //insert submode
	int submodeCount = ED_TestProtocole::getStiSubmodeNum(mode);
	for(int i=0; i<submodeCount; i++)
		this->m_comboStiSubmode.AddString(ED_TestProtocole::getStiSubmodeName(mode, i));
	unsigned char stiSubmode = m_pTestInfo->m_stiSubmode = 0;
	m_comboStiSubmode.SetCurSel(stiSubmode);
	GetDlgItem(IDC_STATIC_STI_DESCRIPTION)->SetWindowText(ED_TestProtocole::getStiSubmodeName(mode, stiSubmode));


	//���õ� radio ��ư�� �´� analysis 
	//select analysis method
	CString strAnal;
	switch(mode)
	{
	case 0:
		strAnal = "Calibration";
		break;
	case 1:
		strAnal = "Gaze";	//radio gaze A
		break;
	case 2:
		strAnal = "Gaze";	//radio gaze B
		break;
	case 3:
		strAnal = "Pursuit";
		break;
	case 4:
		strAnal = "Saccade";
		break;
	case 5:
		strAnal = "Saccade";
		break;
	case 6:
		strAnal = "Saccade";
		break;
	case 7:
		strAnal = "Nystagmus";	//OKN
		break;
	case 8:
		strAnal = "Nystagmus";	//LED off
		break;
	case 9:
		strAnal = "Nystagmus";	//caloric
		break;
	default:
		break;
	}
	unsigned char analIdx = 0;
	for (int i=0; i<ANAL_METHOD_NUM; i++)
	{
		if(strAnal	== ED_TestProtocole::getAnalMethodName(i))
			analIdx = i;
	}
	m_comboAnalMethod.SetCurSel(analIdx);



	
}

