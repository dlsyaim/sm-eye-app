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
	DDX_Control(pDX, IDC_LIST_EVENT, m_listEventString);
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
	
	

	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_EVENT, OnLvnEndlabeleditListEvent)
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



	// add event string
	CRect rect;
	this->m_listEventString.GetClientRect(&rect);
	int w = rect.Width();
	m_listEventString.InsertColumn(0, "Event Text", LVCFMT_CENTER, 0.8*w);
	m_listEventString.InsertColumn(1, "Short Key", LVCFMT_CENTER, 0.2*w);
	CString strF;
	int nIdx;
	for(int i=8; i>=0; i--)
	{
		strF.Format("F%d", i+1);
		nIdx = m_listEventString.InsertItem(0, m_pTestInfo->getEventString(i));
		m_listEventString.SetItemText(nIdx, 1, strF);
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}



void CDlgTestOption::OnLvnEndlabeleditListEvent(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/*CString strText = "";
	CEdit *pEdit = this->m_listEventString.GetEditControl();
	pEdit->GetWindowText(strText);
	m_listEventString.SetItemText(pDispInfo->item.iItem, 0, strText);
	*/

	CString text(pDispInfo->item.pszText);
	if(text.GetLength() >= EVENT_STRING_SIZE)
	{
		::AfxMessageBox("Maximum length of event string is 14. String is truncated."); 
		text = text.Left(EVENT_STRING_SIZE-1);
	}

	m_listEventString.SetItemText(pDispInfo->item.iItem, 0, text);
	
	*pResult = 0;
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


		//set test string
		CString strEvent[EVENT_STRING_COUNT];
		char buf[EVENT_STRING_SIZE];
		for(int i=0; i<EVENT_STRING_COUNT; i++)
		{
			memset(buf, 0, EVENT_STRING_SIZE);
			this->m_listEventString.GetItemText(i, 0, buf, EVENT_STRING_SIZE-1);
			strEvent[i].Format("%s", buf);
		}

		m_pTestInfo->setEventString(strEvent);

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

