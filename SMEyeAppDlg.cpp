// SMEyeAppDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "SMEyeAppDlg.h"

#include "corwlUtil.h"

#include "analyzebase.h"
#include "dlganalyzebase.h"
#include "DlgAnalNystag.h"
#include "dlganalcalibration.h"
#include "dlganalSaccade.h"
#include "dlganalyzePursuit.h"
#include "dlganalyzecaloric.h"
#include "dlganalyzeheadThrust.h"

#include "ed_testinfo.h"
#include "dlgtestoption.h"
#include "ed_testprotocole.h"
#include "eu_eventfile.h"

#include ".\tchart\teechartdefines.h"
#include ".\tchart\axes.h"
#include ".\tchart\axis.h"
#include ".\tchart\fastlineseries.h"
#include ".\tchart\aspect.h"
#include ".\tchart\axisgridpen.h"
#include ".\tchart\axislabels.h"
#include ".\tchart\valuelist.h"
#include ".\tchart\legend.h"
#include ".\tchart\axistitle.h"
#include ".\tchart\teefont.h"
#include ".\tchart\toollist.h"
#include ".\tchart\colorlinetool.h"
#include ".\tchart\tools.h"
#include ".\tchart\pointseries.h"
#include ".\tchart\pointer.h"
#include ".\tchart\marks.h"
#include ".\tchart\zoom.h"
#include ".\tchart\scroll.h"
#include ".\tchart\annotationtool.h"
#include ".\tchart\brush.h"
#include ".\tchart\pen.h"

#include ".\reportGenerator\reportGenerator.h"


#include ".\smeyeappdlg.h"
#include "dlgsubjectinfo.h"
#include "dlgresultlist.h"
#include "dlgEntername.h"
#include "DlgAppOption.h"

#include "dlgPrintSetup.h"
#include "ED_rptMaker.h"

#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif






// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	
END_MESSAGE_MAP()


// CSMEyeAppDlg ��ȭ ����
HWND hCommWnd;		// for CCommThread, 


CSMEyeAppDlg::CSMEyeAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSMEyeAppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bUnderTest = false;
	m_displaySource = DISPLAY_SOURCE_CAM;
	
	m_pCurrentTI = NULL;
	m_hItemforContextMenu = NULL;
	m_hItemSelected = NULL;

	for(int i=0; i<CHART_NUM; i++)
		m_bChartVisible[i] = true;

	m_bDeleteFromContextMenu = false;

	m_cClickedChart = -1;
	m_cClickedSeries = -1;

	
}

void CSMEyeAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART_HORIZONTAL, m_ChartCtrl[0]);
	DDX_Control(pDX, IDC_TCHART_VERTICAL, m_ChartCtrl[1]);
	DDX_Control(pDX, IDC_TCHART_TORSIONAL, m_ChartCtrl[2]);
	DDX_Control(pDX, IDC_TCHART_RADIUS, m_ChartCtrl[3]);
	DDX_Control(pDX, IDC_TCHART_ACC, m_ChartCtrl[4]);
	DDX_Control(pDX, IDC_TCHART_ANALYZE_0, m_ChartCtrl[5]);
	DDX_Control(pDX, IDC_TREE_TEST, m_treeTest);
}

BEGIN_MESSAGE_MAP(CSMEyeAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_CAM_START, OnBnClickedCheckCamStart)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_TREE_TEST, OnNMClickTreeTest)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnBnClickedButtonOpen)

	ON_MESSAGE(WM_DRAW_CHART, OnDrawChart)		// Communication Message Handler
	ON_BN_CLICKED(IDC_BUTTON_ANALYZE, OnBnClickedButtonAnalyze)
	ON_BN_CLICKED(IDC_CHECK_EYE_LEFT, OnBnClickedCheckChartSeries)
	ON_BN_CLICKED(IDC_CHECK_EYE_LEFT+1, OnBnClickedCheckChartSeries)
	ON_BN_CLICKED(IDC_CHECK_EYE_LEFT+2, OnBnClickedCheckChartSeries)
	ON_BN_CLICKED(IDC_CHECK_EYE_LEFT+3, OnBnClickedCheckChartSeries)
	ON_BN_CLICKED(IDC_CHECK_EYE_LEFT+4, OnBnClickedCheckChartSeries)
	ON_BN_CLICKED(IDC_CHECK_EYE_LEFT+5, OnBnClickedCheckChartSeries)
	ON_BN_CLICKED(IDC_CHECK_EYE_LEFT+6, OnBnClickedCheckChartSeries)
	
	ON_BN_CLICKED(IDC_CHECK_CHART_H, OnBnClickedCheckChart)
	ON_BN_CLICKED(IDC_CHECK_CHART_H+1, OnBnClickedCheckChart)
	ON_BN_CLICKED(IDC_CHECK_CHART_H+2, OnBnClickedCheckChart)
	ON_BN_CLICKED(IDC_CHECK_CHART_H+3, OnBnClickedCheckChart)
	ON_BN_CLICKED(IDC_CHECK_CHART_H+4, OnBnClickedCheckChart)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_TEST, OnTvnSelchangedTreeTest)
	ON_COMMAND(ID_EDIT_TEST_INFO, OnContextChildEditTestInfo)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_TEST, OnNMRclickTreeTest)
	ON_COMMAND(ID_SETASDEFAULT_TEST_INFO, OnContextBlankSetasdefaultTestInfo)
	ON_COMMAND(ID_BLANK_ADDNEWGROUP, OnContextBlankAddnewgroup)
	ON_COMMAND(ID_CONTEXT_GROUP_DELETE, OnContextGroupDelete)
	ON_COMMAND(ID_CONTEXT_GROUP_RENAME, OnContextGroupRename)
	ON_COMMAND(ID_CONTEXT_BLANK_CLEAR, OnContextBlankClear)
	ON_COMMAND(ID_CONTEXT_BLANK_LOAD, OnContextBlankLoad)
	ON_COMMAND(ID_CONTEXT_BLANK_SAVE, OnContextBlankSave)
	ON_COMMAND(ID_CONTEXT_GROUP_ADD_TEST, OnContextGroupAddTest)
	ON_COMMAND(ID_EDITTEST_DELETE, OnContextChildDeleteTestInfo)
	ON_COMMAND(ID_CHILD_CLEARDATA, OnContextChildClearData)
	ON_NOTIFY(TVN_DELETEITEM, IDC_TREE_TEST, OnTvnDeleteitemTreeTest)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_SUBJECT_INFO, OnBnClickedButtonEditSubjectInfo)
	ON_BN_CLICKED(IDC_BUTTON_NEW_SUBJECT_INFO, OnBnClickedButtonNewSubjectInfo)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, OnBnClickedButtonOption)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_CONTEXT_IMAGE_RESET_TORSION, OnContextImageResetTorsion)
	
	
	ON_COMMAND(ID_CHARTPOS_REMOVEOFFSET, OnContextChartposRemoveoffset)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_CHECK_REMOVE_OFFSET, OnBnClickedCheckRemoveOffset)
	ON_BN_CLICKED(IDC_BUTTON_EVENT1, OnBnClickedButtonEvent)
	ON_BN_CLICKED(IDC_BUTTON_EVENT2, OnBnClickedButtonEvent)
	ON_BN_CLICKED(IDC_BUTTON_EVENT3, OnBnClickedButtonEvent)
	ON_BN_CLICKED(IDC_BUTTON_EVENT4, OnBnClickedButtonEvent)
	ON_BN_CLICKED(IDC_BUTTON_EVENT5, OnBnClickedButtonEvent)
	ON_BN_CLICKED(IDC_BUTTON_EVENT6, OnBnClickedButtonEvent)
	ON_BN_CLICKED(IDC_BUTTON_EVENT7, OnBnClickedButtonEvent)
	ON_BN_CLICKED(IDC_BUTTON_EVENT8, OnBnClickedButtonEvent)
	ON_BN_CLICKED(IDC_BUTTON_EVENT9, OnBnClickedButtonEvent)
END_MESSAGE_MAP()


// CSMEyeAppDlg �޽��� ó����

BOOL CSMEyeAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if(sizeof(ED_TestInfo) > TEST_INFO_SIZE)
		::AfxMessageBox("Test info size is larger than TEST_INFO_SIZE!!");
	


	ED_TestInfo ti;

	
	m_appOption.loadFromRegistry();
	this->m_stereoCam.m_eyeCalibration[0] = m_appOption.m_calibration[0];
	this->m_stereoCam.m_eyeCalibration[1] = m_appOption.m_calibration[1];
	this->m_stereoCam.m_aviGen.m_bSelectCodecByUser = m_appOption.m_bSelectCodecByUser;

	
	ShowWindow(SW_SHOWMAXIMIZED);
	layoutCtrls();
	initTChart(0xff);

	
	//subject information�� �޴´�.
	ShowWindow(SW_HIDE);

	enrollSubjectInfo();
	ShowWindow(SW_SHOW);

	//display class�� ctrol�� �����Ų��.
	this->m_ImageDisplay.SubclassDlgItem(IDC_STATIC_EYE_IMAGE_DISPLAY, this);
	//�׸��� �׷��� stereoCam�� �����Ų��.
	this->m_stereoCam.setImageDisplay(&m_ImageDisplay);

	//app option�� steroeCam�� ���
	this->m_stereoCam.setAppOption(&this->m_appOption);

	
	

	//THRESHOLD ������ �����ش�.
	this->updateParams(false);
	
	
		
	//tree ���� �о�´�.
	//this->m_treeTest.LoadFromXML(m_strAppPath + "data\\" + TEST_LIST_FILENAME, m_strAppPath + "data\\" + TEST_INFO_FILENAME);
	this->m_treeTest.LoadFromXML(m_appOption.m_strAppFolder + "data\\" + TEST_LIST_FILENAME, m_appOption.m_strAppFolder + "data\\" + TEST_INFO_FILENAME);

	//���ӵ� ����
	/*hCommWnd = this->m_hWnd;
	if(!this->m_CommACC.OpenPort(m_appOption.m_strComportAcc))
		;//::AfxMessageBox("Port open failure!");
	this->m_CommACC.startACC(false);
	this->m_CommACC.startACC(true);
	this->m_CommLED.OpenPort(m_appOption.m_strComportTarget);
	m_CommLED.setCommandStop();*/
	this->m_stereoCam.setCommDrive(&m_CommACC, &m_CommLED);

	//stimulation


	//avi manager
	this->m_aviLoaderManager.setData(this->m_ChartCtrl, &m_ImageDisplay);


	m_seriesBrush[0].CreateSolidBrush(COLOR_LEFT);
	m_seriesBrush[1].CreateSolidBrush(COLOR_RIGHT);
	m_seriesBrush[2].CreateSolidBrush(clPlot0);
	m_seriesBrush[3].CreateSolidBrush(clPlot1);
	m_seriesBrush[4].CreateSolidBrush(clPlot2);
	m_seriesBrush[5].CreateSolidBrush(clPlot3);
	m_seriesBrush[6].CreateSolidBrush(clPlot4);
	

	//gyro acc open port
	m_CommACC.OpenPort(m_appOption.m_strComportAcc);
	
	//ȭ������
	
	//CString strTemp =  "e:\\private\\SLMed\\Vog\\SMEyeApp\\20091221\\Debug\\data\\20091217_062841_calibration_Calibration\\Calibration_Calibration 1.emd";
	//CDlgAnalCalibration dlg;
	/*CString strTemp = "f:\\private\\SLMed\\Vog\\SMEyeApp\\20091221\\Debug\\data\\20091229_135343_saccade_saccade\\Saccade_Saccade 10 center.emd";
	CDlgAnalSaccade dlg;
	dlg.setTestFile(strTemp);
	dlg.DoModal();*/

		
	//cam ����
	m_stereoCam.setParentHWnd(this->m_hWnd);
	this->m_stereoCam.start();

	this->SetTimer(0, 1000, 0);

	for(int i=0; i<7; i++)
		GetDlgItem(IDC_STATIC_SERIES_EYE_LEFT_COLOR+i)->Invalidate();		
	

	this->selectNextTest(m_treeTest.GetRootItem());

	//set event strings
	this->setEventStrings(this->m_pCurrentTI);

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}


bool CSMEyeAppDlg::enrollSubjectInfo()
{
	bool bRtn;

	//subject info
	CDlgSubjectInfo dlgSinfo;
	if(dlgSinfo.DoModal() == IDOK)
	{
		//m_strSubjectFolder = m_strAppPath + "data\\" + dlgSinfo.m_PI.makeSubjectFolderName() + "\\";
		m_strSubjectFolder = this->m_appOption.m_strDataFolder + dlgSinfo.m_PI.makeSubjectFolderName() + "\\";
		this->m_patientInfo = dlgSinfo.m_PI;
		GetDlgItem(IDC_STATIC_SUBJECT_INFO)->SetWindowText(m_patientInfo.makePIString());
		bRtn = true;

		GetDlgItem(IDC_BUTTON_EDIT_SUBJECT_INFO)->EnableWindow(TRUE);
	}		
	else
	{
		// test data�� �ӽ÷� �����ϱ� ���� ������ �ִ��� Ȯ��.
		// ���� ���������� �� temp directory�� �����Ѵ�
		// ������ ���� ������ ��� �����
		// ������ �����.
		//m_strSubjectFolder = m_strAppPath + "temp\\";
		m_strSubjectFolder = m_appOption.m_strAppFolder + "temp\\";
		bRtn  = false;
		GetDlgItem(IDC_BUTTON_EDIT_SUBJECT_INFO)->EnableWindow(FALSE);
	}
	if(corFolderExist(m_strSubjectFolder))
		::corDeleteAllFilesInFolder(m_strSubjectFolder);
	else
		::corCreateDirectory(m_strSubjectFolder);


	//subject info�� ������ �����Ѵ�.
	if(bRtn)
		this->m_patientInfo.save(m_strSubjectFolder + PATIENT_INFO_FILENAME);

	return bRtn;


}

void CSMEyeAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSMEyeAppDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CSMEyeAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



////////////////////////////////////////////////////////////////////////////////////////////
//

void CSMEyeAppDlg::layoutCtrls()
{
	int sideMargin = 3;

	
	CRect clientRect, rect;
	this->GetClientRect(&clientRect);

	int x = sideMargin*4;
    int y = sideMargin;


	//top buttons
	// open, edit, new, | optin | analyze
	//cam start
	int button_h = clientRect.Height()/30;
	int button_w = clientRect.Width()/15;;


	GetDlgItem(IDC_BUTTON_OPEN)->MoveWindow(x, y, button_w, button_h);
	GetDlgItem(IDC_BUTTON_EDIT_SUBJECT_INFO)->MoveWindow(x + button_w, y, button_w, button_h);
	GetDlgItem(IDC_BUTTON_NEW_SUBJECT_INFO)->MoveWindow(x + 2*button_w, y, button_w, button_h);
	GetDlgItem(IDC_BUTTON_OPTION)->MoveWindow(x + 4*button_w, y, button_w, button_h);
	GetDlgItem(IDC_BUTTON_ANALYZE)->MoveWindow(x + 6*button_w, y, button_w, button_h);


	/*GetDlgItem(IDC_CHECK_CAM_START)->MoveWindow(x, y,  button_w, button_h);

		
	
	

	GetDlgItem(IDC_BUTTON_ANALYZE)->MoveWindow(clientRect.Width()/2, y, button_w, button_h);
	GetDlgItem(IDC_BUTTON_OPTION)->MoveWindow(clientRect.Width()/2 + button_w, y, button_w, button_h);
*/

	//ok

	GetDlgItem(IDC_BUTTON_PRINT)->MoveWindow(clientRect.Width()-2*button_w-5*sideMargin, y, button_w, button_h);
	GetDlgItem(IDOK)->MoveWindow(clientRect.Width()-button_w-4*sideMargin, y,  button_w, button_h);

	
	
	

	
		
	
    //Cam
	y += button_h + sideMargin;
	GetDlgItem(IDC_STATIC_EYE_IMAGE_DISPLAY)->MoveWindow(x, y, 320*2+7, 240+7);

	x += 320*2+7;
	int group_width = (clientRect.Width() - x - sideMargin*7)/3;
	int group_height = 240+7;

	//eye info
	GetDlgItem(IDC_STATIC_GROUP_EYE_INFO)->MoveWindow(x + sideMargin, y, group_width, (group_height - sideMargin)/2);
	GetDlgItem(IDC_STATIC_EYE_INFO)->MoveWindow( x + sideMargin+10, y+30, group_width-20, (group_height - sideMargin)/2-40);	

	//cam info
	GetDlgItem(IDC_STATIC_GROUP_CAM_INFO)->MoveWindow(x + sideMargin, y + (group_height + sideMargin)/2, group_width, (group_height - sideMargin)/2);
	GetDlgItem(IDC_STATIC_CAM_INFO)->MoveWindow( x + sideMargin+10, y + (group_height + sideMargin)/2+30, group_width-20, (group_height - sideMargin)/2-40);

	x += sideMargin + group_width;


	//subject info group
	GetDlgItem(IDC_STATIC_GROUP_SI)->MoveWindow(x + sideMargin, y, group_width, group_height);
	GetDlgItem(IDC_STATIC_SUBJECT_INFO)->MoveWindow(x + sideMargin+10, y+sideMargin+20, group_width-20, group_height-40);
	//GetDlgItem(IDC_BUTTON_EDIT_SUBJECT_INFO)->MoveWindow(x + 2*sideMargin, y+group_height-sideMargin-2*button_h, group_width-2*sideMargin, button_h);
	//GetDlgItem(IDC_BUTTON_NEW_SUBJECT_INFO)->MoveWindow(x + 2*sideMargin, y+group_height-(sideMargin+button_h), group_width-2*sideMargin, button_h);
	x += sideMargin + group_width;

	//test info group
	GetDlgItem(IDC_STATIC_GROUP_TI)->MoveWindow(x + sideMargin, y, group_width, group_height);
	GetDlgItem(IDC_STATIC_TEST_INFO)->MoveWindow(x + sideMargin+10, y+30, group_width-20, group_height*2/3);
	GetDlgItem(IDC_BUTTON_EDIT_TEST_INFO)->MoveWindow(x + sideMargin+20, y+30 + group_height*2/3 +sideMargin, group_width-40, button_h);

	y += 247 + sideMargin;

	//start button
	GetDlgItem(IDC_CHECK_CAM_START)->MoveWindow(sideMargin*4, y, button_w, button_h);

	//TREE
	int treeHeight = int((clientRect.Height() - y - 4*sideMargin))-button_h;;
	int treeWidth = int((clientRect.Width()-9*sideMargin)/4);
	GetDlgItem(IDC_TREE_TEST)->MoveWindow(sideMargin*4, y + button_h + sideMargin, treeWidth, treeHeight);



	// chart check box
	for(int i=0; i<8; i++)
	{
		GetDlgItem(IDC_CHECK_CHART_H+i)->MoveWindow(treeWidth + 5*sideMargin + i*(button_w+sideMargin), y, button_w, button_h/2);
		((CButton*)GetDlgItem(IDC_CHECK_CHART_H+i))->SetCheck(1);
	}

	//
	int ww = int(double(clientRect.Width()-treeWidth-9*sideMargin)/7+.5);
	int check_w = int(ww*.7+.5);
	int color_w = int(ww*.3+.5);
	for(int i=0; i<7; i++)
	{
		//color
		GetDlgItem(IDC_STATIC_SERIES_EYE_LEFT_COLOR+i)->MoveWindow(treeWidth+2*sideMargin + i*(sideMargin + ww), y + int(button_h*3/4)-2, color_w-5, 2);
		//GetDlgItem(IDC_STATIC_SERIES_EYE_LEFT_COLOR+i)->MoveWindow(treeWidth + 5*sideMargin + i*(check_w+color_w+sideMargin) , y + int(button_h*3/4)-2, int(color_w*0.8), 2);
		GetDlgItem(IDC_STATIC_SERIES_EYE_LEFT_COLOR+i)->SetWindowText("");
		
		//check
		GetDlgItem(IDC_CHECK_EYE_LEFT+i)->MoveWindow(treeWidth+2*sideMargin + i*(sideMargin + ww)+color_w, y + button_h/2, check_w, button_h/2);
		//GetDlgItem(IDC_CHECK_EYE_LEFT+i)->MoveWindow(treeWidth + 5*sideMargin + i*(check_w+color_w+sideMargin) + color_w, y + button_h/2, check_w/2, button_h/2);
		((CButton*)GetDlgItem(IDC_CHECK_EYE_LEFT+i))->SetCheck(1);
		
	}

	
		




	int tabHeight = int((clientRect.Height() - y - button_h - 3*sideMargin));
	int tabWidth =  int((clientRect.Width()-9*sideMargin)*3/4);

	//plot
	//chart�� �׷��� ���� ����
	m_rectChartRegion.left = treeWidth + 5*sideMargin;
	m_rectChartRegion.top = y+ button_h+sideMargin;
	m_rectChartRegion.right = m_rectChartRegion.left + tabWidth;
	m_rectChartRegion.bottom = m_rectChartRegion.top + tabHeight;

	unsigned int vis = VISIBLE_BIT_H | VISIBLE_BIT_V | VISIBLE_BIT_T | VISIBLE_BIT_R | VISIBLE_BIT_ACC;
	this->setChartVisibility(vis);
	

	// remove offset, and event buttons
	DWORD ids[10] = {IDC_CHECK_REMOVE_OFFSET, IDC_BUTTON_EVENT1, IDC_BUTTON_EVENT2, IDC_BUTTON_EVENT3, IDC_BUTTON_EVENT4,
					IDC_BUTTON_EVENT5, IDC_BUTTON_EVENT6, IDC_BUTTON_EVENT7, IDC_BUTTON_EVENT8, IDC_BUTTON_EVENT9};
	double width = (m_rectChartRegion.right-m_rectChartRegion.left)/10;

	for(int i=0; i<10; i++)
	{
		GetDlgItem(ids[i])->MoveWindow(m_rectChartRegion.left + i*width, y, width-1, button_h);
	}

	//remove offset checkbox
	//GetDlgItem(IDC_CHECK_REMOVE_OFFSET)->MoveWindow(treeWidth + 5*sideMargin, y, button_w, button_h/2);
	
	
}


void CSMEyeAppDlg::initTChart(unsigned int showChart)
{
	//CLEAR
	for(int i=0; i<CHART_NUM; i++)
		m_ChartCtrl[i].ClearChart();

	
	//analyze�� visible�� �� ���� ����
	//hit tet
	CPoint point;
	GetCursorPos(&point);
	this->m_treeTest.ScreenToClient(&point);
	//���õ� item�� ��´�.
	UINT uFlag = 0;
	HTREEITEM hItem = m_treeTest.HitTest(point, &uFlag);

	
	if(m_pCurrentTI)
		this->setChartVisibility(m_pCurrentTI->m_showChart);
	else
		this->setChartVisibility(VISIBLE_BIT_H + VISIBLE_BIT_V + VISIBLE_BIT_T);
	

	
		
	//check ��ư�� disable��Ų��.
	for(int i=0; i<CHART_ACC; i++)
		((CButton*)GetDlgItem(IDC_CHECK_CHART_H+i))->EnableWindow(false);
	
	for(int i=0; i<7; i++)
		((CButton*)GetDlgItem(IDC_CHECK_EYE_LEFT+i))->SetCheck(true);

	


	double axisMin[] = {-40, -40, -15, 0, -600, -100};
	double axisMax[] = {40, 40, 15, 12, 600, 100}; 
	double axishInc[] = {15, 15, 7.5, 3, 100, 25};

	CString titles[] = {"Horizontal", "Vertical", "Torsional", "Radius", "Gyro", "Eye Velocity"};


	for(int i=0; i<CHART_NUM; i++)
	{
		m_ChartCtrl[i].SetAutoRepaint(false);
		m_ChartCtrl[i].GetAspect().SetView3D(false);

		m_ChartCtrl[i].GetAxis().GetBottom().GetLabels().SetStyle(talValue);
		
		m_ChartCtrl[i].AddSeries(scFastLine);	//left
		m_ChartCtrl[i].AddSeries(scFastLine);	//right

		//series		
		if(i != CHART_ACC)
		{
			m_ChartCtrl[i].Series(SERIES_LEFT_EYE).SetName("Left");
			m_ChartCtrl[i].Series(SERIES_RIGHT_EYE).SetName("Right");
			m_ChartCtrl[i].Series(SERIES_LEFT_EYE).SetColor(COLOR_LEFT);
			m_ChartCtrl[i].Series(SERIES_RIGHT_EYE).SetColor(COLOR_RIGHT);
		}


		//acc chart
		if(i == CHART_ACC)
		{
			//acc chart���� �� 3���� series�� �ʿ��ϴ�. 1�� �߰��Ѵ�.
			m_ChartCtrl[i].AddSeries(scFastLine);	
			
			CString strAccName[] = {"GyroX", "GyroY", "GyroZ"};
			for(int j=0; j<3; j++)
				m_ChartCtrl[i].Series(j).SetName(strAccName[j]);

			COLORREF cols[] = {clPlot0, clPlot1, clPlot2, clPlot3, clPlot4}; 
		
			for(int series = 0; series<3; series++)
				m_ChartCtrl[i].Series(series).SetColor(cols[series]);
			//m_ChartCtrl[i].Series(4).SetColor(RGB(74,126,187));
		}
		else if((i==CHART_HORIZONTAL) || (i==CHART_VERTICAL))
		{
			//series for stimulation
			long idx = m_ChartCtrl[i].AddSeries(scFastLine);	//2
			m_ChartCtrl[i].Series(idx).SetColor(COLOR_STI);
			
			//TI�� ���ų� no stimulation�̸� series�� invisible��
			if(!m_pCurrentTI || ((m_pCurrentTI->m_stiModeCommand==0xfe)&&(m_pCurrentTI->m_stiSubmode==0x00)))
			{
				m_ChartCtrl[i].Series(idx).SetActive(false);
				m_bShowStimulation = false;
			}
			else
				m_bShowStimulation = true;

			//series for real time nystagmus
			m_ChartCtrl[i].AddSeries(scLine);			//3
			m_ChartCtrl[i].AddSeries(scLine);			//4
			m_ChartCtrl[i].Series(3).SetColor(0);
			m_ChartCtrl[i].Series(4).SetColor(0);
			m_ChartCtrl[i].Series(3).SetActive(false);
			m_ChartCtrl[i].Series(4).SetActive(false);

			//series for eog
			m_ChartCtrl[i].AddSeries(scFastLine);		//5
			m_ChartCtrl[i].AddSeries(scFastLine);		//6

			//series for event
			double minY = m_ChartCtrl[i].GetAxis().GetLeft().GetMinimum();
			double maxY = m_ChartCtrl[i].GetAxis().GetLeft().GetMaximum();
			long len = m_ChartCtrl[i].GetAxis().GetLeft().CalcYPosValue(minY)-m_ChartCtrl[i].GetAxis().GetLeft().CalcYPosValue(maxY)-10;
			idx = m_ChartCtrl[i].AddSeries(scPoint);			//7
			if(idx != SERIES_EVENT)								// check index
				::AfxMessageBox("SERIES_EVENT is not equal to series index");
			m_ChartCtrl[i].Series(SERIES_EVENT).GetAsPoint().GetPointer().SetStyle(psDiamond);
			m_ChartCtrl[i].Series(SERIES_EVENT).GetAsPoint().GetPointer().GetBrush().SetColor(0x0000ff);
			m_ChartCtrl[i].Series(SERIES_EVENT).GetMarks().SetVisible(true);
			m_ChartCtrl[i].Series(SERIES_EVENT).GetMarks().GetArrow().SetColor(RGB(0,0,0));
			m_ChartCtrl[i].Series(SERIES_EVENT).GetMarks().GetArrow().SetWidth(2);
			m_ChartCtrl[i].Series(SERIES_EVENT).GetMarks().SetArrowLength(len);




		}
		else if(i==CHART_ANALYZE_0)
		{
			//nystagmus�� ǥ���ϱ� ���� point series
			m_ChartCtrl[i].AddSeries(scPoint);	
			m_ChartCtrl[i].AddSeries(scPoint);	

			m_ChartCtrl[i].Series(2).GetAsPoint().GetPointer().SetStyle(psCircle);
			m_ChartCtrl[i].Series(2).GetAsPoint().GetPointer().SetHorizontalSize(2);
			m_ChartCtrl[i].Series(2).GetAsPoint().GetPointer().SetVerticalSize(2);
			m_ChartCtrl[i].Series(2).SetColor(COLOR_LEFT);
			m_ChartCtrl[i].Series(3).GetAsPoint().GetPointer().SetStyle(psCircle);
			m_ChartCtrl[i].Series(3).GetAsPoint().GetPointer().SetHorizontalSize(2);
			m_ChartCtrl[i].Series(3).GetAsPoint().GetPointer().SetVerticalSize(2);
			m_ChartCtrl[i].Series(3).SetColor(COLOR_RIGHT);

			m_ChartCtrl[i].SetAutoRepaint(true);
			
		}

		int n = m_ChartCtrl[0].GetSeriesCount();

		//axis
		m_ChartCtrl[i].GetAxis().GetLeft().SetAutomatic(false);
		m_ChartCtrl[i].GetAxis().GetLeft().SetMinMax(axisMin[i], axisMax[i]);
		m_ChartCtrl[i].GetAxis().GetLeft().SetIncrement(axishInc[i]);
		m_ChartCtrl[i].GetAxis().GetLeft().GetTitle().SetCaption(titles[i]);
		m_ChartCtrl[i].GetAxis().GetLeft().GetTitle().GetFont().SetSize(12);
		m_ChartCtrl[i].GetAxis().GetLeft().GetTitle().GetFont().SetBold(true);
		//m_ChartCtrl[i].GetAxis().GetLeft().SetGridCentered(true);
		

		
		
		m_ChartCtrl[i].GetAxis().GetBottom().SetAutomatic(false);
		m_ChartCtrl[i].GetAxis().GetBottom().SetMinMax(0, 20);
		
		m_ChartCtrl[i].GetAxis().GetBottom().SetIncrement(1);
		
		m_ChartCtrl[i].GetLegend().SetVisible(false);
		
		if(i==CHART_ANALYZE_0)
		{
			m_ChartCtrl[i].GetAxis().GetLeft().SetAutomatic(true);
			m_ChartCtrl[i].GetAxis().GetBottom().SetAutomatic(true);
			
			
		}

		//color line tool
		int idx = m_ChartCtrl[i].GetTools().Add(tcColorLine);
		VARIANT varAxis;
		varAxis.vt=VT_DISPATCH;
		CAxis axBottom = m_ChartCtrl[i].GetAxis().GetBottom();
		varAxis.pdispVal=axBottom;
		m_ChartCtrl[i].GetTools().GetItems(0).GetAsColorLine().SetAxis(varAxis);
		m_ChartCtrl[i].GetTools().GetItems(0).GetAsColorLine().SetDrawBehind(TRUE);
		m_ChartCtrl[i].GetTools().GetItems(0).GetAsColorLine().SetValue(0);
		m_ChartCtrl[i].GetTools().GetItems(0).GetAsColorLine().SetAllowDrag(TRUE);
		m_ChartCtrl[i].GetTools().GetItems(0).SetActive(false);
		
		

		
		
		m_ChartCtrl[i].SetAutoRepaint(true);
		m_ChartCtrl[i].Repaint();
	}

	
	
	

	//analyze
	if(m_pCurrentTI && (m_pCurrentTI->m_analMethod == ANAL_METHOD_NYSTAGMUS))
		this->m_stereoCam.m_bAnalyze = true;
	else
		this->m_stereoCam.m_bAnalyze = false;


	this->m_stereoCam.m_analManager.init();
	this->m_stereoCam.m_analManager.setResultSeries(&(m_ChartCtrl[CHART_ANALYZE_0]), &(m_ChartCtrl[CHART_HORIZONTAL]));

}

//bSave�� ������ ������ m_parameters���� �����ϰ� ������Ʈ���� �����Ѵ�.
//!bSave�� m_parameters���� ���� �������� ����Ѵ�.
void CSMEyeAppDlg::updateParams(bool bSave)
{
	
	if(bSave)
	{
		m_appOption.m_ucThresLeft= (unsigned char)smGetParameter(SM_PUPIL_THRESHOLD	, 0);
		m_appOption.m_ucThresRight = (unsigned char)smGetParameter(SM_PUPIL_THRESHOLD	, 1);
		m_appOption.saveToRegistry();
	}
	else
	{
		m_appOption.loadFromRegistry();
		
		smSetParameter(SM_PUPIL_THRESHOLD, m_appOption.m_ucThresLeft, 0);
		smSetParameter(SM_PUPIL_THRESHOLD, m_appOption.m_ucThresRight, 1);
	}
	
}

int k = 0;
void CSMEyeAppDlg::addData2Chart(double t, ED_EyeInfo* pEyeInfos, double* pGyro, double* pStiValue)
{
	//return; 
	if( !(pEyeInfos && pGyro && pStiValue) )
		return;

	//EOG ��ȣ�� ���´�. 
	double* pEOG = NULL;
	if(this->m_appOption.m_bUseEOG)
	{
		pEOG = this->m_CommEOG.getEOG();
		//TRACE("EOG Value is %.2f, %.2f %.2f %.2f \r\n", pEOG[0], pEOG[1], pEOG[2], pEOG[3]);
	}

	
	if(this->m_bChartVisible[CHART_HORIZONTAL])
	{
		m_ChartCtrl[CHART_HORIZONTAL].Series(SERIES_LEFT_EYE).GetAsFastLine().AddRealTime(t, pEyeInfos[0].m_fCx, NULL, 0);
		m_ChartCtrl[CHART_HORIZONTAL].Series(SERIES_RIGHT_EYE).GetAsFastLine().AddRealTime(t, pEyeInfos[1].m_fCx, NULL, 0);

		if(this->m_bShowStimulation)
			m_ChartCtrl[CHART_HORIZONTAL].Series(SERIES_STIMULATION).GetAsFastLine().AddRealTime(t, pStiValue[0], NULL, 0);

		if(pEOG)
		{
			m_ChartCtrl[CHART_HORIZONTAL].Series(SERIES_LEFT_EOG).GetAsFastLine().AddRealTime(t, pEOG[0], NULL, 0);
			m_ChartCtrl[CHART_HORIZONTAL].Series(SERIES_RIGHT_EOG).GetAsFastLine().AddRealTime(t, pEOG[1], NULL, 0);
		}
	}

	if(this->m_bChartVisible[CHART_VERTICAL])
	{
		m_ChartCtrl[CHART_VERTICAL].Series(SERIES_LEFT_EYE).GetAsFastLine().AddRealTime(t, pEyeInfos[0].m_fCy, NULL, 0);
		m_ChartCtrl[CHART_VERTICAL].Series(SERIES_RIGHT_EYE).GetAsFastLine().AddRealTime(t, pEyeInfos[1].m_fCy, NULL, 0);

		if(this->m_bShowStimulation)
			m_ChartCtrl[CHART_VERTICAL].Series(SERIES_STIMULATION).GetAsFastLine().AddRealTime(t, pStiValue[1], NULL, 0);

		if(pEOG)
		{
			m_ChartCtrl[CHART_VERTICAL].Series(SERIES_LEFT_EOG).GetAsFastLine().AddRealTime(t, pEOG[2], NULL, 0);
			m_ChartCtrl[CHART_VERTICAL].Series(SERIES_RIGHT_EOG).GetAsFastLine().AddRealTime(t, pEOG[3], NULL, 0);
		}
	}

	if(this->m_bChartVisible[CHART_TORSIONAL])
	{
		m_ChartCtrl[CHART_TORSIONAL].Series(SERIES_LEFT_EYE).GetAsFastLine().AddRealTime(t, pEyeInfos[0].m_fTorsion, NULL, 0);
		m_ChartCtrl[CHART_TORSIONAL].Series(SERIES_RIGHT_EYE).GetAsFastLine().AddRealTime(t, pEyeInfos[1].m_fTorsion, NULL, 0);
	}

	if(this->m_bChartVisible[CHART_RADIUS])
	{
		m_ChartCtrl[CHART_RADIUS].Series(SERIES_LEFT_EYE).GetAsFastLine().AddRealTime(t, pEyeInfos[0].m_fRadius, NULL, 0);
		m_ChartCtrl[CHART_RADIUS].Series(SERIES_RIGHT_EYE).GetAsFastLine().AddRealTime(t, pEyeInfos[1].m_fRadius, NULL, 0);
	}

	//ACC
	//���ӵ� ��ȣ�� ���´�.    
	if(this->m_bChartVisible[CHART_ACC])
	{
		m_ChartCtrl[CHART_ACC].Series(SERIES_GYRO_X).GetAsFastLine().AddRealTime(t, pGyro[0], NULL, 0);
		m_ChartCtrl[CHART_ACC].Series(SERIES_GYRO_Y).GetAsFastLine().AddRealTime(t, pGyro[1], NULL, 0);
		m_ChartCtrl[CHART_ACC].Series(SERIES_GYRO_Z).GetAsFastLine().AddRealTime(t, pGyro[2], NULL, 0);
		//m_ChartCtrl[CHART_ACC].Series(SERIES_ZYRO_X).GetAsFastLine().AddRealTime(t, pAcc[3]/360., NULL, 0);	//Hz�� ��Ÿ����.
		//m_ChartCtrl[CHART_ACC].Series(SERIES_ZYRO_Y).GetAsFastLine().AddRealTime(t, pAcc[4]/360., NULL, 0);
	}


	
	
	if(k++%6 == 0) //5Hz
	{
		
		CString str;
		str.Format("Left eye: (%.1f��, %.1f��), R:%.1f, T:%.1f�� \nRight eye: (%.1f��, %.1f��), R:%.1f, T:%.1f�� \nGyro: %.1f, %.1f, %.1f (��/s)",
			pEyeInfos[0].m_fCx, pEyeInfos[0].m_fCy, pEyeInfos[0].m_fRadius, pEyeInfos[0].m_fTorsion,
			pEyeInfos[1].m_fCx, pEyeInfos[1].m_fCy, pEyeInfos[1].m_fRadius, pEyeInfos[1].m_fTorsion,
			pGyro[0], pGyro[1], pGyro[2]);
		if(GetDlgItem(IDC_STATIC_EYE_INFO)->GetSafeHwnd())
			GetDlgItem(IDC_STATIC_EYE_INFO)->SetWindowText(str);
		
	}

	// LED comm�� ����Ǿ� �ְ�
	// LED bar�� stop��ȣ�� ���´��� Ȯ���ϰ� �������� test�� ������.
	if(this->m_CommLED.m_bConnected && this->m_CommLED.m_bStop)
	{
		((CButton*)GetDlgItem(IDC_CHECK_CAM_START))->SetCheck(false);	//stop
		OnBnClickedCheckCamStart();
	}


}




// TI������ �α����� �ߺ��Ǽ� ����ȴ�.
// test info ���Ͽ� tree�� ����Ǽ� ����Ǹ�
// ������ data���Ͽ��� �Բ� ����Ǿ�����. ���Ǹ� ���� �ߺ� �����Ѵ�.
// ������ testInfo.dat �� �⺻���� ����Ѵ�.

bool CSMEyeAppDlg::loadCurrentTestData()
{
	bool bRtn = true;
	bool bAviFile = false;


	if(m_pCurrentTI )
	{
		//test info�� ȭ��� �����ش�.
//		this->updateTestInfo();

		this->m_stereoCam.pause(true);

		//HCURSOR c = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
		::AfxGetApp()->BeginWaitCursor();
		

		
		//chart �� �����.
		int n = m_pCurrentTI->getVisibleChartNum();
		if (n==0)
			m_pCurrentTI->m_showChart = 0xff;
		initTChart(m_pCurrentTI->m_showChart);
		//initTChart(0);

		*m_testDataBinocular.getTestInfo() = *m_pCurrentTI;
		//������ load�Ѵ�.
		CString fname(m_pCurrentTI->m_chFileName);
		if(fname.GetLength())
		{
			//eu_testfile�� version�� �д´�.
			int ver = 0;            
			EU_TestFile testFile;
			if(testFile.open(this->m_strSubjectFolder + fname, false))
			{
				ED_TestInfo ti;
				testFile.readTI(&ti);
				testFile.close();
				ver = ti.m_nVer;
			}

			if(ver == 0)
			{
				if(testFile.open(this->m_strSubjectFolder + fname, false))
				{

					if(testFile.getTestDataCount() == (m_pCurrentTI->m_lEyeDataCount*8 + m_pCurrentTI->m_lAccDataCount*5))
						testFile.readTestData(this->m_ChartCtrl, m_pCurrentTI->m_lEyeDataCount, m_pCurrentTI->m_lAccDataCount);
					else
						::AfxMessageBox("Error : Test Info�� data�� ������ ���� �ʽ��ϴ�.");
					testFile.close();

					
				}
			}
			else if(ver >= 2)
			{
				EU_TestFileV2 testFile2;
				if(testFile2.open(this->m_strSubjectFolder + fname, NULL, false))
				{
					if(testFile2.getTestDataCount() != (m_pCurrentTI->m_lEyeDataCount*10 + m_pCurrentTI->m_lAccDataCount*5))
					{
						::AfxMessageBox("Error : Test Info�� data�� ������ ���� �ʽ��ϴ�.");
						m_pCurrentTI->m_lEyeDataCount = m_pCurrentTI->m_lAccDataCount = testFile2.getTestDataCount()/15;
					}
					int bCheck = ((CButton*)GetDlgItem(IDC_CHECK_REMOVE_OFFSET))->GetCheck();
					testFile2.readTestData(this->m_ChartCtrl, bCheck == BST_CHECKED);
					testFile2.close();

					//event file�� load�Ѵ�.
					CString strEventFname = fname;
					strEventFname.Replace(".emd", ".evt");
					strEventFname = m_strSubjectFolder + strEventFname;

					EU_EventFile::loadEventFile(strEventFname, &(m_ChartCtrl[0]), SERIES_EVENT, bCheck == BST_CHECKED);
					EU_EventFile::loadEventFile(strEventFname, &(m_ChartCtrl[1]), SERIES_EVENT, bCheck == BST_CHECKED);

				}

				/*//ACC ������ �ִ��� Ȯ���Ѵ�.
				CFile file;
				CString strAccFname = m_strSubjectFolder + fname;
				strAccFname.Replace(".emd", ".gyr");
				if(file.Open(strAccFname, CFile::modeRead))
				{
					this->m_ChartCtrl[4].SetAutoRepaint(false);
					int count = file.GetLength()/sizeof(double)/6;

//					double ratio = double(m_pCurrentTI->m_lEyeDataCount)/count;

 					double t = 0;
					double buf[6];
					for(int i=0; i<count; i++)
					{
						file.Read(buf, sizeof(double)*6);
						t = double(i)/FRAMERATE;
						//t = double(i)/FRAMERATE*ratio;
						//m_ChartCtrl[4].Series(0).AddXY(t, buf[0], NULL, 0); 
						//m_ChartCtrl[4].Series(1).AddXY(t, buf[1], NULL, 0);
						m_ChartCtrl[4].Series(2).AddXY(t, buf[2], NULL, 0);
												
					}

					this->m_ChartCtrl[4].SetAutoRepaint(true);
					this->m_ChartCtrl[4].Repaint();
					
					file.Close();

				}*/

			}

			

			//analyze
			if((m_pCurrentTI->m_analMethod == ANAL_METHOD_NYSTAGMUS))
			{
				this->m_stereoCam.m_analManager.init();
				this->m_stereoCam.m_analManager.setResultSeries(&(m_ChartCtrl[CHART_ANALYZE_0]), &(m_ChartCtrl[CHART_HORIZONTAL]));
				CValueList yl = m_ChartCtrl[CHART_HORIZONTAL].Series(0).GetYValues();
				CValueList yr = m_ChartCtrl[CHART_HORIZONTAL].Series(1).GetYValues();
				unsigned long count = yl.GetCount();
				
				
				for(unsigned long i=0; i<count; i++)
					this->m_stereoCam.m_analManager.putSignal(yl.GetValue(i), yr.GetValue(i));					
			}
			this->m_stereoCam.m_analManager.m_Nystagmus[0].m_pTChartResult->GetAxis().GetLeft().SetAutomatic(true);
			this->m_stereoCam.m_analManager.m_Nystagmus[0].m_pTChartResult->GetAxis().GetBottom().SetAutomatic(true);
			this->m_stereoCam.m_analManager.m_Nystagmus[0].m_pTChartResult->Repaint();


			//avi ������ �ִ��� Ȯ���Ѵ�.
			CString strAviFname = fname;
			strAviFname.Replace(".emd", ".avi");


			bAviFile = m_aviLoaderManager.open(m_strSubjectFolder + strAviFname, m_strSubjectFolder + fname);
			

			
			

			
		}
		else
		{
			bRtn = false;
		}
		
		
		::AfxGetApp()->EndWaitCursor();

	}
	else
	{
		initTChart(0xff);
		GetDlgItem(IDC_CHECK_CAM_START)->EnableWindow(false);
		bRtn = false;
	}

	return bRtn;
}


void CSMEyeAppDlg::activateTestCtrls(bool bActive)
{
	//test �� ctrl���� active inactive ��Ų��.

	//buttons
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(bActive);
	//subject info�� ��������
	if(m_patientInfo.m_chID[0])
		GetDlgItem(IDC_BUTTON_EDIT_SUBJECT_INFO)->EnableWindow(bActive);
	GetDlgItem(IDC_BUTTON_NEW_SUBJECT_INFO)->EnableWindow(bActive);
	GetDlgItem(IDC_BUTTON_OPTION)->EnableWindow(bActive);
	GetDlgItem(IDC_BUTTON_ANALYZE)->EnableWindow(bActive);
	GetDlgItem(IDOK)->EnableWindow(bActive);

	

	//series check
	for(int i=0; i<7; i++)
		GetDlgItem(IDC_CHECK_EYE_LEFT+i)->EnableWindow(bActive);

	//offset remove check
	GetDlgItem(IDC_CHECK_REMOVE_OFFSET)->EnableWindow(bActive);

	//tree
	GetDlgItem(IDC_TREE_TEST)->EnableWindow(bActive);
}



LRESULT CSMEyeAppDlg::OnDrawChart(WPARAM wParam, LPARAM lParam)
{
	getDataFromStereoCam();
	
	return 1;
}
void CSMEyeAppDlg::getDataFromStereoCam()
{
	ED_EyeInfo eyeInfos[2];
	double t = 0;
	int count = 0;

	count = m_stereoCam.m_QueEyeInfos.getCount();
	//double* pData = new double[15*count];
	//::ZeroMemory(pData, sizeof(double)*15*count);
	if(count)
	{
		
		for(int i=0; i<count; i++)
		{
            t = double(m_stereoCam.getTestFrameNum())/FRAMERATE;
			m_stereoCam.m_QueEyeInfos.pop(eyeInfos);

			

			//ù��°�� �׸���.
			if(i==0)
			{
				addData2Chart(t, eyeInfos, m_CommACC.getCurrentValue(), this->m_CommLED.getStiValue());
			}

		}
	}

	//delete []pData;
}
BOOL CSMEyeAppDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(!m_bUnderTest)
	{
		if(pMsg->message == WM_MOUSEWHEEL)
		{
			

			CRect rect;
			CPoint point;

			::GetCursorPos(&point);
			HWND hwnd = ::WindowFromPoint(point);
			int zDelta = GET_WHEEL_DELTA_WPARAM(pMsg->wParam);


			GetDlgItem(IDC_STATIC_EYE_IMAGE_DISPLAY)->GetWindowRect(rect);
			if(rect.PtInRect(point))
			{
				unsigned char threshold;
				
				rect.right = rect.left + rect.Width()/2;
				int camIndex = 0;
				if(rect.PtInRect(point))	//right image
					camIndex = 1;
				else						//right image
					camIndex = 0;
					
					
				threshold = (unsigned char)smGetParameter(SM_PUPIL_THRESHOLD , camIndex);
				if(zDelta>0)
					::smSetParameter(SM_PUPIL_THRESHOLD	, threshold-1, camIndex);
				else
					::smSetParameter(SM_PUPIL_THRESHOLD	, threshold+1, camIndex);

				updateParams(true);
				this->updateCamInfo();		
			}
			else 
			{
				int idx = -1;
				for(int i=0; i<CHART_NUM; i++)
				{
					//chart ������ wheel�� �����̸� y������� �޸� ���ش�.
					m_ChartCtrl[i].GetWindowRect(rect);
					if(m_ChartCtrl[i].GetVisible() && rect.PtInRect(point))
						idx = i;
				}
				//��Ʈ ������ �������ٸ�
				if(idx != -1)
				{
					double m = m_ChartCtrl[idx].GetAxis().GetLeft().GetMinimum();
					double M = m_ChartCtrl[idx].GetAxis().GetLeft().GetMaximum();

					if(zDelta>0)	//���
						m_ChartCtrl[idx].GetAxis().GetLeft().SetMinMax((m+M)/2 - (M-m)/4, (m+M)/2 + (M-m)/4);
					else	//Ȯ��
						m_ChartCtrl[idx].GetAxis().GetLeft().SetMinMax((m+M)/2 - (M-m), (m+M)/2 + (M-m));
				}
			}



		}
		if(pMsg->message == WM_LBUTTONDBLCLK)	//���콺 L ��ư ����Ŭ��
		{
			CRect rect;
			CPoint point;

			::GetCursorPos(&point);
			HWND hwnd = ::WindowFromPoint(point);

			//mouse�� tchart ������ ���ȴ��� Ȯ��
			bool bOnChart = false;
			for(int i=0; i<CHART_NUM; i++)
			{
				this->m_ChartCtrl[i].GetWindowRect(rect);
				if(rect.PtInRect(point))
					bOnChart = true;
			}
			if(bOnChart)	//chart ���� ���ȴٸ�
			{
				if( abs(rect.left-point.x) < 20)
					tchartLastPage(-1);		//��������
				else if(abs(rect.right-point.x) <20)
					tchartLastPage(1);		//����������
				else if(((rect.left+rect.right)/2 - point.x) > 0)		//1 page ����
					tchartNextPage(-1);
				else //1 page ����
					tchartNextPage(1);
			}

		}
	}
	if(pMsg->message == WM_KEYDOWN) // �̺�Ʈ ������ �����Ƿ� �˾Ƽ�...
	{
		//bool bShift = ::GetAsyncKeyState(VK_LSHIFT);
		//bool bCtrl = ::GetAsyncKeyState(VK_LCONTROL);
		bool bShift = IsSHIFTpressed();
		bool bCtrl = IsCTRLpressed();
		


		if(bShift && bCtrl && (pMsg->wParam == 83)) //ctrl + shift + A
		{
#ifdef FOR_ASAN
			::AfxMessageBox("This version is for Asan data"); 
#else
			::AfxMessageBox("This version is for normal data");
#endif

		}

		else if (pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return true;
		}
		else if(pMsg->wParam == VK_RETURN)
		{

			if(GetDlgItem(IDC_CHECK_CAM_START)->IsWindowEnabled())
			{
				((CButton*)GetDlgItem(IDC_CHECK_CAM_START))->SetCheck(!((CButton*)GetDlgItem(IDC_CHECK_CAM_START))->GetCheck());
				OnBnClickedCheckCamStart();
				return TRUE;
				
			}
			else
			{
				::TranslateMessage(pMsg);
				::DispatchMessage(pMsg);
				return true;
			}

		}
		else if((pMsg->wParam>=0x70) && (pMsg->wParam<0x79))
		{
			if(this->m_bUnderTest)
				this->addEvent(this->m_strEvents[pMsg->wParam-0x70]);
			//::TranslateMessage(pMsg);
			//::DispatchMessage(pMsg);
			return true;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSMEyeAppDlg::playAvi(bool bPlay)
{
	if(bPlay)	//start
		this->SetTimer(1, 33, NULL);
	else		//stop
		this->KillTimer(1);
	
}

void CSMEyeAppDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	this->m_stereoCam.stop();
	this->KillTimer(0);
		


	//���ӵ� ����
	m_CommACC.stop();
	m_CommACC.ClosePort();
	
	
	if(m_CommLED.m_bConnected)
	{
		this->m_CommLED.setCommandStop();
		//center led on
		this->m_CommLED.setCommand(0xfe, 0x01);
		m_CommLED.ClosePort();
	}


	this->m_aviLoaderManager.close();
	
	for(int i=0; i<7; i++)
		m_seriesBrush[i].DeleteObject();

}


CString strTimer;
double x = 0;
void CSMEyeAppDlg::OnTimer(UINT nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//frame rate
	if(nIDEvent == 0)
	{
		updateCamInfo();
	}
	if(nIDEvent == 1)		//play avi
	{
		
		if(!m_aviLoaderManager.next())	//avi�� ���̸� timer����
		{
			this->KillTimer(nIDEvent);
			((CButton*)GetDlgItem(IDC_CHECK_CAM_START))->SetCheck(false);	//start
			this->OnBnClickedCheckCamStart();
		}
	}
	
	
	CDialog::OnTimer(nIDEvent);
}


void CSMEyeAppDlg::OnBnClickedCheckCamStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//event button ids
	DWORD ids[EVENT_STRING_COUNT] = {IDC_BUTTON_EVENT1, IDC_BUTTON_EVENT2, IDC_BUTTON_EVENT3, IDC_BUTTON_EVENT4,
					IDC_BUTTON_EVENT5, IDC_BUTTON_EVENT6, IDC_BUTTON_EVENT7, IDC_BUTTON_EVENT8, IDC_BUTTON_EVENT9};
			

	if(((CButton*)GetDlgItem(IDC_CHECK_CAM_START))->GetCheck())	//start
	{

		activateTestCtrls(false);


		// display source�� cam�� ���
		if(this->m_displaySource == DISPLAY_SOURCE_CAM)
		{
			this->m_stereoCam.setTriggerMode(true);

			this->m_stereoCam.pause(true);


			//event ��ư�� Ȱ��ȭ��Ų��.
			for(int i=0; i<EVENT_STRING_COUNT; i++)
				GetDlgItem(ids[i])->EnableWindow(true);


			TRACE("CAM start button and puase\n");

			#ifdef DEBUG_FUNC			
			#endif

			int n = 0;
			if(m_pCurrentTI)
			{
				initTChart(m_pCurrentTI->m_showChart);
			}
			else 
				initTChart(0xff);
			
			#ifdef DEBUG_FUNC			
			#endif
			//TRACE("tchart init\n");

			if(m_appOption.m_bSaveAVI)
			{
				m_stereoCam.setSaveImage(true, this->m_appOption.m_strAppFolder + AVI_FILENAME);		
				#ifdef DEBUG_FUNC			
				#endif
				
			}
			//TRACE("SET save image\n");

			

			m_timeStart = CTime::GetCurrentTime();
			
			

			

			//TRACE("cam start!\n");

			
			
			

			//testFile�� open�Ѵ�.
			ED_TestInfo* pTI = this->m_treeTest.getCurrentTestInfo();
			if(pTI)
			{
				//test time�� update�Ѵ�
				pTI->m_timeTime = CTime::GetCurrentTime();
				//file name�� �����ϰ� Test Info�� ���´�.
				CString strFname;
				m_treeTest.getCurrentItemGeneology(&strFname);
				corDeleteUnfilenameCharacter(&strFname, "!");	//file name���� ���� �� ���� char�� !�� �ٲ۴�.
				//���� Ȯ���ڸ� ���δ�.
				CString strTestDataFname;
				strTestDataFname = strFname + ".emd";	//eye movement data
				//tree�� testinfo�� file name�� �����Ѵ�.
				::memcpy(pTI->m_chFileName, strTestDataFname.GetBuffer(), strTestDataFname.GetLength());
				//tree�� calibration�� ������ calibration���� ��ü
				pTI->m_calibration[0] = this->m_stereoCam.m_eyeCalibration[0];
				pTI->m_calibration[1] = this->m_stereoCam.m_eyeCalibration[1];

				#ifdef DEBUG_FUNC			
				#endif
			

				if(this->m_stereoCam.m_euTestFile.open(m_strSubjectFolder + strTestDataFname, pTI, true))
				{
					#ifdef DEBUG_FUNC			
					#endif
				
					//tree disable
					//GetDlgItem(IDC_TREE_TEST)->EnableWindow(false);
					GetDlgItem(IDC_CHECK_CAM_START)->SetWindowText("Stop");

					m_stereoCam.setUnderTest(true);
					this->m_bUnderTest = true;					

					#ifdef DEBUG_FUNC			
					#endif
			
				}
				else
					::AfxMessageBox("������ ������ �ۼ��� �� �����ϴ�. ");
			}

			
				

			

			//EOG ����̸� EOG ���� ������ open�Ѵ�.
			if(this->m_appOption.m_bUseEOG)
			{
				CString strFname;
				m_treeTest.getCurrentItemGeneology(&strFname);
				corDeleteUnfilenameCharacter(&strFname, "!");	//file name���� ���� �� ���� char�� !�� �ٲ۴�.
				//���� Ȯ���ڸ� ���δ�.
				CString strEOGDataFname;
				strEOGDataFname = strFname + ".eog";	//eye movement data

				if(m_CommEOG.OpenPort(m_appOption.m_strComportEOG))
				{
					this->m_CommEOG.startRecord(this->m_strSubjectFolder + strEOGDataFname);
					unsigned char eogBuf = 0xff;
					m_CommEOG.WriteComm(&eogBuf, 1);
				}
			}
			
			
			//stimulation ����
			if(m_bUnderTest && pTI)
			{
				// All LED OFF�鰡�ӵ� ���� open
				//if((m_pCurrentTI->m_stiModeCommand == 0xfe) && m_pCurrentTI->m_stiSubmode ==0x01)
				//�м������ head thrust�� ��� ACC������ ����Ѵ�.
				// zyro�� �̿��� Į���극�̼��� ��� GYRO(ACC)�� ����Ѵ�.

				if((pTI->m_analMethod == ANAL_METHOD_HEADTHRUST) || 
					((pTI->m_stiModeCommand == ED_TestProtocole::getStiModeCommand(STI_CALIBRATION)) && (pTI->m_stiSubmode == ED_TestProtocole::getStiSubmodeNum(STI_CALIBRATION)-1)))
				{
					CString strFname;
					m_treeTest.getCurrentItemGeneology(&strFname);
					corDeleteUnfilenameCharacter(&strFname, "!");	//file name���� ���� �� ���� char�� !�� �ٲ۴�.
					//���� Ȯ���ڸ� ���δ�.
					CString strACCDataFname;
					strACCDataFname = m_strSubjectFolder + strFname + ".gyr";	//eye movement data


					//Head Thrust�� ���
					
					m_CommACC.clearCommBuf();
					Sleep(100);		
					m_CommACC.start(strACCDataFname);
			
					
					
				}
				else
				{
					//LED Stimulation Open
					m_CommLED.OpenPort(m_appOption.m_strComportTarget);
					m_CommLED.setCommand(m_pCurrentTI->m_stiModeCommand, m_pCurrentTI->m_stiSubmode);
				}
			}

			this->m_stereoCam.pause(false);
			this->m_stereoCam.setTriggerMode(false);
			
		}
		//display source�� avi�� ��� play��Ų��.
		else
		{
			playAvi(true);	//���� play�� toggle��Ų��.
			m_timeStart = CTime::GetCurrentTime();
			//GetDlgItem(IDC_TREE_TEST)->EnableWindow(false);
			GetDlgItem(IDC_CHECK_CAM_START)->SetWindowText("Stop");
		}

		//treectrl�� disable�Ǹ鼭 keydown �޼����� �������Ƿ�
		//focus�� start button���� �д�.
		//this->SetFocus();
		GetDlgItem(IDC_CHECK_CAM_START)->SetFocus();
		

	}
	else		//stop
	{
		if(this->m_displaySource == DISPLAY_SOURCE_CAM)
		{
			m_stereoCam.setTriggerMode(true);
			m_stereoCam.pause(true);
			m_stereoCam.setUnderTest(false);
			this->m_bUnderTest = false;

			Sleep(100);
	
			
			if(this->m_CommLED.m_bConnected)
			{
				this->m_CommLED.setCommandStop();
				this->m_CommLED.ClosePort();
			}
			
			
			//���ӵ� port close
			Sleep(100);
			this->m_CommACC.stop();
								
			

			
			::AfxGetApp()->BeginWaitCursor();

			
			//Sleep(100);
			m_stereoCam.setSaveImage(false, "");
			

			//������ ���� ������ �ݴ´�.
			this->m_stereoCam.m_euTestFile.close();

			//EOG data ������ �ݴ´�.
			if(this->m_appOption.m_bUseEOG)
			{
				unsigned char eogBuf = 0xfe;
				m_CommEOG.WriteComm(&eogBuf, 1);
				m_CommEOG.ClosePort();
				this->m_CommEOG.stopRecord();


			}

			//avi�� ���������� �ű��.
			if(this->m_appOption.m_bSaveAVI)
			{
				CString strFname;
				m_treeTest.getCurrentItemGeneology(&strFname);
				corDeleteUnfilenameCharacter(&strFname, "!");	//file name���� ���� �� ���� char�� !�� �ٲ۴�.

				CString strTestAviFname = strFname + ".avi";
				//avifile�� �ű��.
				//CString strAviSrc = this->m_strAppPath + "\\" + AVI_FILENAME;
				CString strAviSrc = this->m_appOption.m_strAppFolder + AVI_FILENAME;
				CString strAviDst = this->m_strSubjectFolder + strTestAviFname;
				CFile::Rename(strAviSrc, strAviDst);
			}

			//timestamp ������ �ű��.
			CString strTimestampTarget, appFolder;
			corGetProgramFolder(&appFolder);
			strTimestampTarget.Format("%s\\%s", appFolder, TIMESTAMPE_FILE_NAME);

			CString strTimestampDest;
			m_treeTest.getCurrentItemGeneology(&strTimestampDest);
			corDeleteUnfilenameCharacter(&strTimestampDest, "!");	//file name���� ���� �� ���� char�� !�� �ٲ۴�
			strTimestampDest = m_strSubjectFolder + strTimestampDest + ".tsp";
			//���� �ִٸ� �ݴ´�.
			if(this->m_stereoCam.m_fileTimestamp.m_hFile != CFile::hFileNull)
				m_stereoCam.m_fileTimestamp.Close();
			//dest�� ���� �̸��� ������ �ִٸ� �����.
			CFileStatus fs;
			if(CFile::GetStatus(strTimestampDest, fs))
				CFile::Remove(strTimestampDest);
			// data folder�� �ű��.
			CFile::Rename(strTimestampTarget, strTimestampDest);



			//update�� tree ������ �����Ѵ�.
			this->m_treeTest.SaveToXML(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
										m_strSubjectFolder + m_treeTest.getDefaultTIName());



			//event file�� �����Ѵ�.
			CString strEventFilename;
			m_treeTest.getCurrentItemGeneology(&strEventFilename);
			corDeleteUnfilenameCharacter(&strEventFilename, "!");	//file name���� ���� �� ���� char�� !�� �ٲ۴�
			strEventFilename = m_strSubjectFolder + strEventFilename + ".evt";
			EU_EventFile::saveEventFile(strEventFilename, m_ChartCtrl, SERIES_EVENT);		//horizontal ctrl�κ��� �����.



			//TRACE("STOP and puased\n");

			//addRealtimeXY�� ���� �����͸� addXY�� �ٽ� �ִ´�.
			//drawTemporaryData();
			this->loadCurrentTestData();

			//TI ctrl update
			this->updateTestInfo();





			//for(int i=0; i<CHART_NUM; i++)
			//	tchartFastLineToLine(&(m_ChartCtrl[i]));
			//TRACE("tchart fast line converted to line\n");

			
			//tree enable
			//GetDlgItem(IDC_TREE_TEST)->EnableWindow(true);
			GetDlgItem(IDC_CHECK_CAM_START)->SetWindowText("Start");

			//check ��ư�� enable��Ų��.
			for(int i=0; i<5; i++)
				((CButton*)GetDlgItem(IDC_CHECK_CHART_H+i))->EnableWindow(true);
			for(int i=0; i<7; i++)
				((CButton*)GetDlgItem(IDC_CHECK_EYE_LEFT+i))->EnableWindow(true);

			//event ��ư�� ��Ȱ��ȭ��Ų��.
			for(int i=0; i<EVENT_STRING_COUNT; i++)
				GetDlgItem(ids[i])->EnableWindow(true);

			
			::AfxGetApp()->EndWaitCursor();
			
			
			//cam�� ���߰� avi���� -> ���� test�� �̵��ϹǷ� avi ���� ���� �ʴ´�.
			//m_displaySource = DISPLAY_SOURCE_AVI;
			//m_stereoCam.pause(false);

			//�׽�Ʈ�� ���� �������� bold��
			m_treeTest.SetItemState(m_treeTest.GetSelectedItem(), TVIS_BOLD, TVIS_BOLD);
			// ���� ���õǾ��� item�� highlight��Ų��.
			// highlight�� ������ ���� ontvnselchangedt ���� �����ش�.
			m_treeTest.SetItemState(m_treeTest.GetSelectedItem(), TVIS_DROPHILITED, TVIS_DROPHILITED);
			

			this->m_stereoCam.pause(false);
			m_stereoCam.setTriggerMode(false);

			//���� test item ����
			this->selectNextTest(m_treeTest.GetSelectedItem());

		}
		else
		{
			playAvi(false);
			this->m_aviLoaderManager.setPos(0);
			//GetDlgItem(IDC_TREE_TEST)->EnableWindow(true);

			GetDlgItem(IDC_CHECK_CAM_START)->SetWindowText("Start");
		}

		activateTestCtrls(true);
	}
}






void CSMEyeAppDlg::OnNMClickTreeTest(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	

	*pResult = 0;

	//hit tet
	UINT uFlag = 0;
	HTREEITEM hItem = getTreeItemOnMouse(&uFlag);

	m_pCurrentTI = m_treeTest.getTestInfoFromItem(hItem);

    //item�� Ŭ���ϰ�
	if((hItem != NULL) && (TVHT_ONITEM & uFlag))
	{
		// ti�� ������
		if(m_pCurrentTI)
		{
		}
		//ti�� ���ٸ�
		else
		{
			this->m_hItemSelected = NULL;
		}
	}
	
}


void CSMEyeAppDlg::OnNMRclickTreeTest(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	//hit tet
	UINT uFlag = 0;
	HTREEITEM hItem = getTreeItemOnMouse(&uFlag);

	ED_TestInfo* pTI = m_treeTest.getTestInfoFromItem(hItem);

    //item�� Ŭ���ϰ�
	CMenu menu;
	CMenu* pSubmenu;
	
	//menu�� �̺�Ʈ �ڵ鷯���� ����ϱ� ���� hitem�� ������ ���´�.
	m_hItemforContextMenu = hItem;

	if((hItem != NULL) && (TVHT_ONITEM & uFlag))
	{
		// list�� item�� Ŭ������ ��
		
		// child �϶�
		if(this->m_treeTest.getTestInfoFromItem(hItem))
            menu.LoadMenu(IDR_CONTEXT_TEST_LIST_CHILD);
		// group�϶�
		else
			menu.LoadMenu(IDR_CONTEXT_TEST_LIST_GROUP);
		
	}
	else
	{
		

		//����� Ŭ������ ��
		menu.LoadMenu(IDR_CONTEXT_TEST_LIST_BLANK);
		
	}
	//show context menu
	CPoint point;
	GetCursorPos(&point);
	pSubmenu = menu.GetSubMenu(0);
	pSubmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}


void CSMEyeAppDlg::OnContextChildEditTestInfo()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if(m_hItemforContextMenu)
	{
		CDlgTestOption dlgTestInfo;
		ED_TestInfo* pTI = m_treeTest.getTestInfoFromItem(this->m_hItemforContextMenu);
		if(pTI)
		{
			m_stereoCam.pause(true);

			CString str = m_treeTest.GetItemText(this->m_hItemforContextMenu);
			dlgTestInfo.setTestName(str);
			dlgTestInfo.setTestInfo(pTI);
			if(dlgTestInfo.DoModal() == IDOK)
			{
				m_treeTest.SetItemText(m_hItemforContextMenu, dlgTestInfo.getTestName());
				if(pTI->m_showChart== 0) pTI->m_showChart = 1;
				this->setChartVisibility(pTI->m_showChart);

				//update�� tree ������ �����Ѵ�.
				this->m_treeTest.SaveToXML(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
										m_strSubjectFolder + m_treeTest.getDefaultTIName());

				//������ ���� data�� �����ϸ� data�� test info�� update�ؾ� �Ѵ�.
				EU_TestFileV2::updateTestInfo(m_strSubjectFolder+pTI->m_chFileName, pTI);

				//event string�� �ٲ۴�.
				this->setEventStrings(pTI);

			}

			if(this->m_displaySource == DISPLAY_SOURCE_CAM)
				m_stereoCam.pause(false);
		}
	}
}

void CSMEyeAppDlg::OnContextChildDeleteTestInfo()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// test tree���� �ش� child�� �����.

	if(m_hItemforContextMenu)
	{
		CDlgTestOption dlgTestInfo;
		ED_TestInfo* pTI = m_treeTest.getTestInfoFromItem(this->m_hItemforContextMenu);

		if(pTI)
		{
			
			//data�� �����Ѵ�.
			deleteDataFromTreeItem(m_hItemforContextMenu);

			// tree���� �ش� child�� �����Ѵ�.
			delete pTI;	
			m_bDeleteFromContextMenu = true;
			m_treeTest.DeleteItem(m_hItemforContextMenu);			
			//�������� OnTvnDeleteitemTreeTest ���� ó���Ѵ�.

			//update�� tree ������ �����Ѵ�.
			this->m_treeTest.SaveToXML(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
									m_strSubjectFolder + m_treeTest.getDefaultTIName());
		}
	}
}


void CSMEyeAppDlg::OnContextChildClearData()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// �ش� child�� data�� �����.

	if(m_hItemforContextMenu)
	{
		if(this->m_displaySource == DISPLAY_SOURCE_CAM)
			m_stereoCam.pause(true);


		if(::AfxMessageBox("Confirm clear data.", MB_OKCANCEL)==IDOK)
		{
			this->m_aviLoaderManager.close();
			deleteDataFromTreeItem(m_hItemforContextMenu);
			m_treeTest.SetItemState(m_hItemforContextMenu, 0, TVIS_BOLD);

			//ti������ update�Ѵ�.
			ED_TestInfo* pTI = m_treeTest.getTestInfoFromItem(m_hItemforContextMenu);
			::memset(pTI->m_chFileName, 0, MAX_FILE_NAME);
			pTI->m_lAccDataCount = pTI->m_lEyeDataCount = 0;


			//update�� tree ������ �����Ѵ�.
			this->m_treeTest.SaveToXML(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
										m_strSubjectFolder + m_treeTest.getDefaultTIName());

			if(this->m_displaySource == DISPLAY_SOURCE_CAM)
				m_stereoCam.pause(false);
			else if(this->m_displaySource == DISPLAY_SOURCE_AVI)
			{
				m_displaySource = DISPLAY_SOURCE_CAM;
				m_stereoCam.pause(false);
			}
			this->initTChart(0xff);
		}
	}

}


void CSMEyeAppDlg::setChartVisibility(unsigned int showChart)
{
	//layout visible chart
	unsigned int visibleChartcount = 0;
	for(int i=0; i<CHART_NUM; i++)
	{
		if(showChart & (1<<i))
		{
			visibleChartcount++;
			this->m_bChartVisible[i] = true;
		}
		else 
			this->m_bChartVisible[i] = false;
	}
	int plotHeight = m_rectChartRegion.Height()/(visibleChartcount);
	int plotWidth = m_rectChartRegion.Width();

	int count = 0;
	for(int i=0; i<=RT_PROC; i++)
	{
		if(m_bChartVisible[i])
		{
			m_ChartCtrl[i].SetVisible(true);
			((CButton*)GetDlgItem(IDC_CHECK_CHART_H+i))->SetCheck(true);
            m_ChartCtrl[i].MoveWindow(m_rectChartRegion.left, m_rectChartRegion.top + count*plotHeight, plotWidth, plotHeight);
			count++;
		}
		else
		{
			m_ChartCtrl[i].SetVisible(false);
			((CButton*)GetDlgItem(IDC_CHECK_CHART_H+i))->SetCheck(false);
		}
	}

	//m_ChartCtrl[CHART_ANALYZE_0].SetVisible(m_bChartVisible[CHART_ANALYZE_0]);
}


HTREEITEM CSMEyeAppDlg::getTreeItemOnMouse(UINT* pFlag)
{
	//���� ���콺 ��ġ�� tree item�� �����Ѵ�.
	// ������ null
	
	CPoint point;
	GetCursorPos(&point);
	this->m_treeTest.ScreenToClient(&point);

	//���õ� item�� ��´�.
	
	HTREEITEM hItem = m_treeTest.HitTest(point, pFlag);

    return hItem;
}


void CSMEyeAppDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//this->m_stereoCam.suspend();
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//fake data�� �����.

	m_stereoCam.setTriggerMode(!m_stereoCam.m_triggerMode);
	

	/*m_stereoCam.pause(true);

	EA_Nystagmus ea_nys;
	ea_nys.setResultSeries(&(m_ChartCtrl[CHART_ANALYZE_0]), &(m_ChartCtrl[CHART_HORIZONTAL]), 0);
	

	CValueList y = m_ChartCtrl[CHART_HORIZONTAL].Series(0).GetYValues();
	unsigned long count = y.GetCount();
	CSeries series = m_ChartCtrl[CHART_ANALYZE_0].Series(0);
	int n= 0;
	for(unsigned long i=0; i<count; i++)
	{
		if(ea_nys.putSignal(y.GetValue(i)))
			n++;
		
		//series.GetAsFastLine().AddRealTime(double(i)/FRAMERATE, y.GetValue(i), NULL, 0);
	}
	n += 0;
	

	m_stereoCam.pause(false);*/
}

void CSMEyeAppDlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFile file;
	if(file.Open("c:\\temp\\testInfo.dat", CFile::modeCreate | CFile::modeWrite))
	{

		//���� �־�� �� ��.
		// unsigned int m_showChart;	//bit(0): h, bit(1): v, bit(2):t, bit(3) r, bit(4): acc, bit(5), zyro, bit(6), vel, bit(7) analysis
		//unsigned char m_analMethod;	//bit0: NYSTAGMUS bit1:HEAD_TRUST*/
		//	unsigned char m_analTarget;
		//  unsigned char m_stiMode;
		// unsigned char m_stiSubmode;

		byte buf[TI_SIZE];
		::memset(buf, 0, TI_SIZE);

		ED_TestInfo ti;
		ti.m_showChart = VISIBLE_BIT_H | VISIBLE_BIT_V | VISIBLE_BIT_T;
		ti.m_analMethod = ANAL_METHOD_NYSTAGMUS;
		ti.m_analTarget = HORIZONTAL;
		ti.m_stiModeCommand = STI_OKN;
		ti.m_stiSubmode = 0;

				
		::memcpy(buf, &ti, sizeof(ED_TestInfo));
		file.Write(buf, TI_SIZE);		//OKN 1
		file.Write(buf, TI_SIZE);		//OKN 2
		file.Write(buf, TI_SIZE);		//OKN 3
		file.Write(buf, TI_SIZE);		//OKN 4
		file.Write(buf, TI_SIZE);		//OKN 5
		file.Write(buf, TI_SIZE);		//OKN 6
		file.Write(buf, TI_SIZE);		//OKN 7
		file.Write(buf, TI_SIZE);		//OKN 8
		file.Write(buf, TI_SIZE);		//OKN 9
		file.Write(buf, TI_SIZE);		//OKN 10

		/*ti.m_nTestParams[0] = TEST_CLASS_GAZE;
		::memcpy(buf, &ti, sizeof(ED_TestInfo));
		file.Write(buf, TI_SIZE);		//GAZE
		file.Write(buf, TI_SIZE);		//GAZE*/

		ti.m_analMethod = ANAL_METHOD_HEADTHRUST;
		ti.m_showChart = VISIBLE_BIT_H | VISIBLE_BIT_V | VISIBLE_BIT_T;
		ti.m_analTarget = HORIZONTAL;
		ti.m_stiModeCommand = STI_LED;
		ti.m_stiSubmode = 0;	//all led off
		::memcpy(buf, &ti, sizeof(ED_TestInfo));
		file.Write(buf, TI_SIZE);		//head trust 1
		file.Write(buf, TI_SIZE);		//head trust 2
		file.Write(buf, TI_SIZE);		//head trust 3
		file.Write(buf, TI_SIZE);		//head trust 4
		file.Write(buf, TI_SIZE);		//head trust 5
		file.Write(buf, TI_SIZE);		//head trust 6
		file.Write(buf, TI_SIZE);		//head trust 7
		file.Write(buf, TI_SIZE);		//head trust 8
		file.Write(buf, TI_SIZE);		//head trust 9
		file.Write(buf, TI_SIZE);		//head trust 10
		

		file.Close();
	}




	
	
}

void CSMEyeAppDlg::OnBnClickedButtonOpen()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->m_stereoCam.pause(true);

	CDlgResultList dlgResultList;
	
	dlgResultList.setListFolder(this->m_appOption.m_strDataFolder);
	if((dlgResultList.DoModal() == IDOK) && dlgResultList.getSelectedFolder() != "")
	{
		
		initTChart(0xff);
		m_treeTest.clear();
		
		m_treeTest.LoadFromXML(dlgResultList.getSelectedFolder()+ TEST_LIST_FILENAME, 
								dlgResultList.getSelectedFolder() + TEST_INFO_FILENAME);

		m_strSubjectFolder = dlgResultList.getSelectedFolder();
		this->m_patientInfo.load(dlgResultList.getSelectedFolder() + PATIENT_INFO_FILENAME);
		GetDlgItem(IDC_STATIC_SUBJECT_INFO)->SetWindowText(m_patientInfo.makePIString());

		m_pCurrentTI = NULL;

		//enable edit subject info button
		GetDlgItem(IDC_BUTTON_EDIT_SUBJECT_INFO)->EnableWindow(TRUE);
	}
	
	this->m_stereoCam.pause(false);
}


void CSMEyeAppDlg::updateCamInfo()
{
	CString strCamInfo;

	//frame rate

	double fr0, fr1;
	unsigned int mfn0, mfn1;
	this->m_stereoCam.getFrameInfo(&fr0, &fr1, &mfn0, &mfn1);

	
	if(m_bUnderTest)
	{
		CTime t = CTime::GetCurrentTime();
		CTimeSpan ts = t - m_timeStart;
		strCamInfo.Format("Threshold : (%d, %d)\nFramerates : (%.1f, %.1f) Hz\nMissing frames : (%d, %d) frames\nTest Time: %dh %d m %d s", 
						m_appOption.m_ucThresLeft, m_appOption.m_ucThresRight,
						fr0, fr1, mfn0, mfn1,
						ts.GetHours(), ts.GetMinutes(), ts.GetSeconds());
	}
	else
	{
		strCamInfo.Format("Threshold : (%d, %d)\nFramerates : (%.1f, %.1f) Hz\nMissing frames : (%d, %d) frames\nTest Time: %dh %d m %d s", 
						m_appOption.m_ucThresLeft, m_appOption.m_ucThresRight,
						fr0, fr1, mfn0, mfn1,
						0, 0, 0);

	}
	if(GetDlgItem(IDC_STATIC_CAM_INFO)->GetSafeHwnd())
		GetDlgItem(IDC_STATIC_CAM_INFO)->SetWindowText(strCamInfo);

	
}
void CSMEyeAppDlg::updateEyeInfo()
{
}

void CSMEyeAppDlg::updateTestInfo()
{
	CString str;
	if(this->m_pCurrentTI)
	{
		//hit tet
		CPoint point;
		GetCursorPos(&point);
		this->m_treeTest.ScreenToClient(&point);

		//���õ� item�� ��´�.
		UINT uFlag = 0;
		HTREEITEM hItem = m_treeTest.HitTest(point, &uFlag);

		CString testName;
		if(m_hItemSelected)
			testName = m_treeTest.GetItemText(m_hItemSelected);
		else
            testName = m_treeTest.GetItemText(hItem);

		CString testDate = m_pCurrentTI->m_timeTime.Format( "%A, %B %d, %Y" );
		CString testTime = m_pCurrentTI->m_timeTime.Format( "%H : %M :  %S" );
		double testDuration = double(m_pCurrentTI->m_lEyeDataCount)/FRAMERATE;
		

		if(m_pCurrentTI->m_lEyeDataCount)
			str.Format("Name: %s\n\nDate: %s\n\nTime: %s\n\nDuration: %.1f sec",
						testName, testDate, testTime, testDuration);
		else
			str.Format("Name: %s\n\nDate: %s\n\nTime: %s\n\nDuration:",
						testName, "", "");
	}
	else if(isCaloricGroup2(this->m_treeTest.GetSelectedItem(), NULL))
	{
		//caloric gruop�� �������� ��
		
	}
	else
		str = "Name: \n\nDate: \n\nTime: \n\nDuration:";

	GetDlgItem(IDC_STATIC_TEST_INFO)->SetWindowText(str);

	// event string�� �ٲ��ش�.
	this->setEventStrings(this->m_pCurrentTI);

}


void CSMEyeAppDlg::tchartNextPage(int advance)
{
	//advance ����ŭ 10�ʾ� �̵�
	for(int j=0; j<CHART_NUM; j++)
	{
		CTChart* pTChart = &(m_ChartCtrl[j]);
		if(advance>0)
		{
			double m = pTChart->GetAxis().GetBottom().GetMaximum();
			pTChart->GetAxis().GetBottom().SetMinMax(m,m+20);			
		}
		else if(advance<0)
		{
			double m = pTChart->GetAxis().GetBottom().GetMinimum();
			m = max(0, m-20);
			pTChart->GetAxis().GetBottom().SetMinMax(m, m+20);
		}
	}
}

void CSMEyeAppDlg::tchartLastPage(int advance)
{
	if(advance<0)	//ó������
	{
		for(int i=0; i<CHART_NUM; i++)
		{
			CTChart* pTChart = &(m_ChartCtrl[i]);
			pTChart->GetAxis().GetBottom().SetMinMax(0, 20);
		}
	}
	else	//������
	{
		
	
		double lastX;
		lastX = m_ChartCtrl[0].Series(0).GetXValues().GetMaximum();

		double m = max(20, int(lastX/10+0.5)*20);

		for(int i=0; i<CHART_NUM; i++)
		{
			CTChart* pTChart = &(m_ChartCtrl[i]);
			pTChart->GetAxis().GetBottom().SetMinMax(m-20, m);
		}
	
	}
		
}BEGIN_EVENTSINK_MAP(CSMEyeAppDlg, CDialog)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_HORIZONTAL, 221, OnColorLineToolDragLineTchartHorizontal, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_VERTICAL, 221, OnColorLineToolDragLineTchartHorizontal, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_TORSIONAL, 221, OnColorLineToolDragLineTchartHorizontal, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_RADIUS, 221, OnColorLineToolDragLineTchartHorizontal, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_ACC, 221, OnColorLineToolDragLineTchartHorizontal, VTS_NONE)

ON_EVENT(CSMEyeAppDlg, IDC_TCHART_HORIZONTAL, 27, OnZoomTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_VERTICAL, 27, OnZoomTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_TORSIONAL, 27, OnZoomTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_RADIUS, 27, OnZoomTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_ACC, 27, OnZoomTchart, VTS_NONE)

ON_EVENT(CSMEyeAppDlg, IDC_TCHART_HORIZONTAL, 26, OnUndoZoomTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_VERTICAL, 26, OnUndoZoomTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_TORSIONAL, 26, OnUndoZoomTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_RADIUS, 26, OnUndoZoomTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_ACC, 26, OnUndoZoomTchart, VTS_NONE)


ON_EVENT(CSMEyeAppDlg, IDC_TCHART_HORIZONTAL, 24, OnScrollTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_VERTICAL, 24, OnScrollTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_TORSIONAL, 24, OnScrollTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_HORIZONTAL, 24, OnScrollTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_RADIUS, 24, OnScrollTchart, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_ACC, 24, OnScrollTchart, VTS_NONE)

ON_EVENT(CSMEyeAppDlg, IDC_TCHART_HORIZONTAL, 4, OnClickAxisTchart, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_VERTICAL, 4, OnClickAxisTchart, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_HORIZONTAL, 7, OnClickSeriesTchart, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_VERTICAL, 7, OnClickSeriesTchart, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_HORIZONTAL, 211, OnBeforePrintTchartHorizontal, VTS_NONE)
/*ON_EVENT(CSMEyeAppDlg, IDC_TCHART_HORIZONTAL, 29, OnBeforeDrawSeriesTchartHorizontal, VTS_NONE)
ON_EVENT(CSMEyeAppDlg, IDC_TCHART_VERTICAL, 29, OnBeforeDrawSeriesTchartVertical, VTS_NONE)*/
END_EVENTSINK_MAP()

void CSMEyeAppDlg::OnColorLineToolDragLineTchartHorizontal()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	//���콺�� ��ġ�� ��´�
	CRect rect;
	CPoint point;
	::GetCursorPos(&point);
	HWND hwnd = ::WindowFromPoint(point);

	int idxOnCursor = -1;
	for(int i=0; i<CHART_NUM; i++)
	{
		this->m_ChartCtrl[i].GetWindowRect(rect);
		if(m_ChartCtrl[i].GetEnabled() && rect.PtInRect(point))
			idxOnCursor = i;

	}

	if(idxOnCursor >= 0)
	{
		double x = m_ChartCtrl[idxOnCursor].GetTools().GetItems(0).GetAsColorLine().GetValue();
		double fnamenum = int(x*30);

		this->m_aviLoaderManager.setPos((unsigned long)fnamenum);
	}

}
void getDataFromSeries(double* pDat, CSeries* pSeries);
void CSMEyeAppDlg::OnBnClickedButtonAnalyze()
{
	if(m_displaySource == DISPLAY_SOURCE_CAM)
			this->m_stereoCam.pause(true);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//caloric group�� ���õǾ� �ִٸ�
	CString testFnames[4];
	if(this->isCaloricGroup2(m_treeTest.GetSelectedItem(), testFnames))
	{
		CString strAnalyzeFileName;
		this->m_treeTest.getCurrentItemGeneology(&strAnalyzeFileName);
		corDeleteUnfilenameCharacter(&strAnalyzeFileName, "!");	//file name���� ���� �� ���� char�� !�� �ٲ۴�.
		strAnalyzeFileName = strAnalyzeFileName + ".ema";	//eye movement data
		strAnalyzeFileName = m_strSubjectFolder + strAnalyzeFileName;
				
		CDlgAnalyzeCaloric dlg;
		//test data�� ��� �ִ� ������ ���� ���� �Ѱ��ش�.
		dlg.setTestFiles(testFnames);
		dlg.setAnalyzeFile(strAnalyzeFileName);
		dlg.m_strSubjectFolder = this->m_strSubjectFolder;
		dlg.m_patientInfo = this->m_patientInfo;
		if(dlg.DoModal() == IDOK)
		{
		}
        
	}
	else
	{
		//caloric group�� �ƴ϶��

		if(!this->m_pCurrentTI)
		{
			this->m_stereoCam.pause(false);
			return;
		}



		if(this->m_ChartCtrl[0].Series(LEFT).GetCount() == 0)
			::AfxMessageBox("No data");
		else
		{
			
				

			if(m_pCurrentTI->m_analMethod == ANAL_METHOD_CALIBRATION)
			{
				CDlgAnalCalibration dlg;
				dlg.setTestFile(m_strSubjectFolder+m_pCurrentTI->m_chFileName);
				dlg.m_patientInfo = this->m_patientInfo;

				// LED�� �ƴ� gyro �� �̿��� calibration
				if( (m_pCurrentTI->m_stiModeCommand == ED_TestProtocole::getStiModeCommand(STI_CALIBRATION)) && 
					(m_pCurrentTI->m_stiSubmode == ED_TestProtocole::getStiSubmodeNum(STI_CALIBRATION)-1) )
					dlg.setCalbrationUsingGyro(true);
				

				if(dlg.DoModal() == IDOK)
				{
					if(::AfxMessageBox("Calibration ����� ����Ͻðڽ��ϱ�?", MB_YESNO | MB_ICONQUESTION, 0) == IDYES)
					{
						CCalibration*  pCal = dlg.getCalibration();
						this->m_stereoCam.m_eyeCalibration[0] = pCal[0];
						this->m_stereoCam.m_eyeCalibration[1] = pCal[1];

						//replace calibration parameters
						CFile file;
						if(file.Open(m_strSubjectFolder+m_pCurrentTI->m_chFileName, CFile::modeReadWrite))
						{
                            char buf[TEST_INFO_SIZE];
							//ti�� �о�´�.
							file.SeekToBegin();
							file.Read(buf, TEST_INFO_SIZE);
							ED_TestInfo* pTI = (ED_TestInfo*)buf;
							//calibration�� �ٲ۴�.
							memcpy(pTI->m_calibration, m_stereoCam.m_eyeCalibration, sizeof(CCalibration)*2);
							file.SeekToBegin();
							//���Ͽ� ���´�.
							file.Write(buf, TEST_INFO_SIZE);
							//�ݴ���.
							file.Close();

							loadCurrentTestData();
						}



					}

				}
			}
			else if(m_pCurrentTI->m_analMethod == ANAL_METHOD_SACCADE)
			{
				CDlgAnalSaccade dlg;
				dlg.setTestFile(m_strSubjectFolder+m_pCurrentTI->m_chFileName);
				dlg.m_patientInfo = this->m_patientInfo;
				dlg.DoModal();		
			}
			else if(m_pCurrentTI->m_analMethod == ANAL_METHOD_PURSUIT)
			{
				CDlgAnalyzePursuit dlg;
				dlg.setTestFile(m_strSubjectFolder+m_pCurrentTI->m_chFileName);
				dlg.m_patientInfo = this->m_patientInfo;
				dlg.DoModal();					
			}
			else if(m_pCurrentTI->m_analMethod == ANAL_METHOD_HEADTHRUST)
			{
				CDlgAnalyzeHeadThrust dlg;
				dlg.setTestFile(m_strSubjectFolder+m_pCurrentTI->m_chFileName);
				dlg.m_patientInfo = this->m_patientInfo;
				dlg.DoModal();				
			}	
			else 
			{
				CDlgAnalNystag dlg;

				dlg.setTestFile(m_strSubjectFolder+m_pCurrentTI->m_chFileName);
				dlg.m_patientInfo = this->m_patientInfo;
				dlg.DoModal();
			}
			

		}

		
	}

	if(m_displaySource == DISPLAY_SOURCE_CAM)
		m_stereoCam.pause(false);
}


void getDataFromSeries(double* pDat, CSeries* pSeries)
{
	
	CValueList vl = pSeries->GetYValues();
	unsigned long count = vl.GetCount();
	for(unsigned long i=0; i<count; i++)
		pDat[i] = vl.GetValue(i);
}
void CSMEyeAppDlg::OnBnClickedCheckChartSeries()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->m_stereoCam.pause(true);

	int showSeries[7];
	//check box ���¸� ���´�.
	for(int i=0; i<7; i++)
		showSeries[i] = ((CButton*)GetDlgItem(IDC_CHECK_EYE_LEFT+i))->GetCheck();

	//left, right visible
	for(int i=CHART_HORIZONTAL; i<=CHART_RADIUS; i++)
	{
        this->m_ChartCtrl[i].Series(SERIES_LEFT_EYE).SetActive(showSeries[0]);
		this->m_ChartCtrl[i].Series(SERIES_RIGHT_EYE).SetActive(showSeries[1]);

		//�߰����� series�� ������
		if(m_ChartCtrl[i].GetSeriesCount() == 5)
		{
			m_ChartCtrl[i].Series(SERIES_LEFT_EYE+3).SetActive(showSeries[0]);
			m_ChartCtrl[i].Series(SERIES_RIGHT_EYE+3).SetActive(showSeries[1]);
		}
	}

	//���ӵ� 
	for(int i=SERIES_GYRO_X; i<=SERIES_GYRO_Z; i++)
		m_ChartCtrl[CHART_ACC].Series(i).SetActive(showSeries[i+2]);

	//analyze
	if(m_ChartCtrl[CHART_ANALYZE_0].GetSeriesCount()>=2)
	{
		m_ChartCtrl[CHART_ANALYZE_0].Series(SERIES_LEFT_EYE).SetActive(showSeries[0]);
		m_ChartCtrl[CHART_ANALYZE_0].Series(SERIES_RIGHT_EYE).SetActive(showSeries[1]);
	}
	if(m_ChartCtrl[CHART_ANALYZE_0].GetSeriesCount()>=4)
	{
		m_ChartCtrl[CHART_ANALYZE_0].Series(SERIES_LEFT_EYE+2).SetActive(showSeries[0]);
		m_ChartCtrl[CHART_ANALYZE_0].Series(SERIES_RIGHT_EYE+2).SetActive(showSeries[1]);
	}

	//repaint
	for(int i=0; i<CHART_NUM; i++)
		m_ChartCtrl[i].Repaint();
	

	//SOURCE�� CAM�� ��� CAM START
	if(this->m_displaySource == DISPLAY_SOURCE_CAM)
		this->m_stereoCam.pause(false);
}

void CSMEyeAppDlg::OnBnClickedCheckChart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->m_stereoCam.pause(true);

	bool showCharts[CHART_ACC];
	//check box ���¸� ���´�.
	for(int i=0; i<CHART_ACC; i++)
		showCharts[i] = (((CButton*)GetDlgItem(IDC_CHECK_CHART_H+i))->GetCheck() == BST_CHECKED);

	for(int i=0; i<CHART_ACC; i++)
	{
		if( m_ChartCtrl[i].GetVisible() != showCharts[i] )
			m_ChartCtrl[i].SetVisible(showCharts[i]);
	}

	//���� ��ġ��Ų��.
	int visibleChartcount = 0;
	for(int i=0; i<CHART_NUM; i++)
	{
		if(m_ChartCtrl[i].GetVisible())
			visibleChartcount++;
	}
	
	if(visibleChartcount)
	{
		int plotHeight = m_rectChartRegion.Height()/(visibleChartcount);
		int plotWidth = m_rectChartRegion.Width();

		int count = 0;
		for(int i=0; i<CHART_NUM; i++)
		{
			if(m_ChartCtrl[i].GetVisible())
			{
				m_ChartCtrl[i].MoveWindow(m_rectChartRegion.left, m_rectChartRegion.top + count*plotHeight, plotWidth, plotHeight);
				count++;
			}
			
		}
	}
	if(this->m_displaySource == DISPLAY_SOURCE_CAM)
		this->m_stereoCam.pause(false);

}

HBRUSH CSMEyeAppDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.

	for(int i=0; i<7; i++)
	{
		if(pWnd->GetDlgCtrlID() == (IDC_STATIC_SERIES_EYE_LEFT_COLOR+i))
		{
			return this->m_seriesBrush[i];
		}
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CSMEyeAppDlg::OnTvnSelchangedTreeTest(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	*pResult = 0;

	//���� �Ű����� item�� �����͸� load�Ѵ�.
	m_hItemSelected = pNMTreeView->itemNew.hItem;
	if(this->loadDataFromTreeItem(pNMTreeView->itemNew.hItem))
	{
		GetDlgItem(IDC_BUTTON_ANALYZE)->EnableWindow(true);		//data �� load�Ǿ����� analyze ��ư Ȱ��ȭ
	}
	else if(isCaloricGroup2(this->m_treeTest.GetSelectedItem(), NULL))	//caloric group�� �������� ��
		GetDlgItem(IDC_BUTTON_ANALYZE)->EnableWindow(true);
	else
		GetDlgItem(IDC_BUTTON_ANALYZE)->EnableWindow(false);


	this->updateTestInfo();
	
	m_treeTest.SetItemState(pNMTreeView->itemOld.hItem, 0, TVIS_DROPHILITED);
	
}

bool CSMEyeAppDlg::loadDataFromTreeItem(HTREEITEM hItem)
{
	bool bRtn = false;
	m_pCurrentTI = m_treeTest.getTestInfoFromItem(hItem);

    //item�� Ŭ���ϰ�
	if((hItem != NULL))// && (TVHT_ONITEM & uFlag))
	{
		// ti�� ������ 
		if(m_pCurrentTI)
		{
			

			m_aviLoaderManager.close();
			
			this->loadCurrentTestData();	
			if(m_pCurrentTI->m_lEyeDataCount)
			{
				//�����Ͱ� ������ �ִٸ� image source�� avi�� �ٲٰ� ķ�� ����
				m_displaySource = DISPLAY_SOURCE_AVI;
				this->m_stereoCam.pause(true);		//cam ����
				bRtn = true;

				

			}
			else
			{
                
				this->m_stereoCam.pause(true);
				//center LED ON
				this->m_CommLED.OpenPort(this->m_appOption.m_strComportTarget);
				Sleep(50);
				this->m_CommLED.setCommand(0xfe, 0x01);
				this->m_CommLED.ClosePort();


				m_displaySource = DISPLAY_SOURCE_CAM;
				this->m_stereoCam.pause(false);			//cam ����
			}

			//pTest�� ���� �������̹Ƿ� �׽�Ʈ ctrol���� Ȱ��ȭ��Ų��.
			GetDlgItem(IDC_CHECK_CAM_START)->EnableWindow(true);
		}
		//ti�� ���ٸ�
		else
		{
			//set no stimulation
			if(this->m_CommLED.m_bConnected)
				this->m_CommLED.setCommandNoStimulation();

			this->m_pCurrentTI = NULL;
			m_displaySource = DISPLAY_SOURCE_CAM;
			this->initTChart(0xff);

			this->m_stereoCam.pause(false);

			GetDlgItem(IDC_CHECK_CAM_START)->EnableWindow(false);

			//Ȥ�� Caloric�� group item�̸鼭 4�� ��� data�� ���� ��
			//if(isCaloricGroup(hItem))
			//	bRtn = true;

		}
	

	}

	return bRtn;
}

bool CSMEyeAppDlg::isCaloricGroup(HTREEITEM itemParent, CString* pstrFname)
{
	//�װ��� �׽�Ʈ�� �� �ְ�, �����͵� ���� ��� true�� �����Ѵ�.
	bool bRtn = false;

	if(this->m_treeTest.ItemHasChildren(itemParent))
	{
		

		unsigned char childSubmode[4];
		::ZeroMemory(childSubmode, 4);

		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_treeTest.GetChildItem(itemParent);

		ED_TestInfo* pTI = NULL;
		while (hChildItem != NULL)
		{
			pTI = m_treeTest.getTestInfoFromItem(hChildItem);

			if(pTI)
			{
				//caloric�̰� ���� �����Ͱ� �ִٸ�
				if((pTI->m_stiModeCommand == 0xa1) && (pTI->m_lEyeDataCount))
				//if((pTI->m_stiModeCommand == 0xa1) )
				{
					childSubmode[pTI->m_stiSubmode] = 1;
					//file name�� ��û�ϸ� �Ѱ��ش�.
					if(pstrFname)
						pstrFname[pTI->m_stiSubmode] = this->m_strSubjectFolder + "\\" + pTI->m_chFileName;
				}
			}
			hNextItem = m_treeTest.GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}

		bRtn = true;
		//4�� ��ο� �����Ͱ� �ִ��� Ȯ���Ѵ�.
		for(unsigned char i=0; i<4; i++)
		{
			if(childSubmode[i] == 0) 
				bRtn = false;
		}
	}

	return bRtn;
}


bool CSMEyeAppDlg::isCaloricGroup2(HTREEITEM itemParent, CString* pstrFname)
{
	//1�� �̻��� Į�θ� �׽�Ʈ�� �ְ�, �����͵� ���� ��� true�� �����Ѵ�.
	bool bRtn = false;

	if(this->m_treeTest.ItemHasChildren(itemParent))
	{
		
		unsigned char childSubmode[4];
		::ZeroMemory(childSubmode, 4);

		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_treeTest.GetChildItem(itemParent);

		ED_TestInfo* pTI = NULL;
		while (hChildItem != NULL)
		{
			pTI = m_treeTest.getTestInfoFromItem(hChildItem);

			if(pTI)
			{
				//caloric�̰� ���� �����Ͱ� �ִٸ�
				if((pTI->m_stiModeCommand == 0xa1) && (pTI->m_lEyeDataCount))
				//if((pTI->m_stiModeCommand == 0xa1) )
				{
					childSubmode[pTI->m_stiSubmode] = 1;
					//file name�� ��û�ϸ� �Ѱ��ش�.
					if(pstrFname)
						pstrFname[pTI->m_stiSubmode] = this->m_strSubjectFolder + pTI->m_chFileName;
				}
			}
			hNextItem = m_treeTest.GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}

		bRtn = false;
		//4�� ��ο� �����Ͱ� �ִ��� Ȯ���Ѵ�.
		for(unsigned char i=0; i<4; i++)
		{
			if(childSubmode[i] == 1) 
				bRtn = true;
		}
	}

	return bRtn;
}


void CSMEyeAppDlg::deleteDataFromTreeItem(HTREEITEM hItem)
{	
	ED_TestInfo* pTI = m_treeTest.getTestInfoFromItem(hItem);

	if(pTI)
	{
		CString fname(pTI->m_chFileName);
		if(fname.GetLength())
		{
			//data file ����	
			CFileStatus fs;

			if(CFile::GetStatus(m_strSubjectFolder + fname,fs))
				CFile::Remove(m_strSubjectFolder + fname);
			//avi file ����
			fname.Replace(".emd", ".avi");
			if(m_aviLoaderManager.getFname() == (m_strSubjectFolder + fname))
				m_aviLoaderManager.close();
			if(CFile::GetStatus(m_strSubjectFolder + fname,fs))
				CFile::Remove(m_strSubjectFolder + fname);
			//eog ���� ����
			fname.Replace(".avi", ".eog");
			if(CFile::GetStatus(m_strSubjectFolder + fname,fs))
				CFile::Remove(m_strSubjectFolder + fname);
			//gyr ���� ����
			fname.Replace(".eog", ".gyr");
			if(CFile::GetStatus(m_strSubjectFolder + fname,fs))
				CFile::Remove(m_strSubjectFolder + fname);

			//timestamp ���� ����
			fname.Replace(".gyr", ".tsp");
			if(CFile::GetStatus(m_strSubjectFolder + fname, fs))
				CFile::Remove(m_strSubjectFolder + fname);
		}
	}
}

void CSMEyeAppDlg::OnContextBlankSetasdefaultTestInfo()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// TREE ��Ʈ�ѿ��� ����� �� Ŭ�� ���� �� ��Ÿ����.
	
	// ���� Ʈ�� ��Ʈ���� ������ DEFAULT ���Ϸ� �����.
	//XML ���ϰ� TI ������ ������ �Ѵ�.

	//������ ������ش�.
	if(!corFolderExist(m_appOption.m_strAppFolder + "data\\"))
		corCreateDirectory(m_appOption.m_strAppFolder + "data\\");

	m_treeTest.SaveToXML(m_appOption.m_strAppFolder + "data\\" + TEST_LIST_FILENAME, m_appOption.m_strAppFolder + "data\\" + TEST_INFO_FILENAME, true);

}

void CSMEyeAppDlg::OnContextBlankAddnewgroup()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	//test list �� ���ο� group�� �����.
	CDlgEnterName dlg;
	if(dlg.DoModal() == IDOK)
	{
		HTREEITEM hItem = m_treeTest.addNewGroup(dlg.getNewName());

		//update�� tree ������ �����Ѵ�.
		this->m_treeTest.SaveToXML(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
									m_strSubjectFolder + m_treeTest.getDefaultTIName());
	}

}

void CSMEyeAppDlg::OnContextGroupRename()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CDlgEnterName dlg;
	m_stereoCam.pause(true);

	if((dlg.DoModal() == IDOK) && (m_hItemforContextMenu))
		m_treeTest.SetItemText(m_hItemforContextMenu, dlg.getNewName());

	//update�� tree ������ �����Ѵ�.
	this->m_treeTest.SaveToXML(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
								m_strSubjectFolder + m_treeTest.getDefaultTIName());

	if(this->m_displaySource == DISPLAY_SOURCE_CAM)
		m_stereoCam.pause(false);
	
}


void CSMEyeAppDlg::OnContextGroupAddTest()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if(m_hItemforContextMenu)
	{
		m_stereoCam.pause(true);

		ED_TestInfo* pTestInfo = new ED_TestInfo;
		CDlgTestOption dlg;
		dlg.setTestInfo(pTestInfo);
		if(dlg.DoModal() == IDOK)
		{
			CString strTestName = dlg.getTestName();
			m_treeTest.addNewTest(m_hItemforContextMenu, strTestName, (DWORD_PTR)pTestInfo);			

			//update�� tree ������ �����Ѵ�.
			this->m_treeTest.SaveToXML(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
									m_strSubjectFolder + m_treeTest.getDefaultTIName());
		}

		if(this->m_displaySource == DISPLAY_SOURCE_CAM)
			m_stereoCam.pause(false);
		
	}

}

void CSMEyeAppDlg::OnContextGroupDelete()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if(m_hItemforContextMenu)
	{
		m_treeTest.deleteGroup(m_hItemforContextMenu);

		//update�� tree ������ �����Ѵ�.
		this->m_treeTest.SaveToXML(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
									m_strSubjectFolder + m_treeTest.getDefaultTIName());
	}
}

void CSMEyeAppDlg::OnContextBlankClear()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_treeTest.clear();

	//update�� tree ������ �����Ѵ�.
	this->m_treeTest.SaveToXML(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
								m_strSubjectFolder + m_treeTest.getDefaultTIName());
}

void CSMEyeAppDlg::OnContextBlankLoad()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// test list �� load �Ѵ�.
	
	CString filter="Test List file (*.xml)|*.xml|";
	CFileDialog fileDlg(TRUE, "xml", "*.xml", NULL, filter, NULL);
	fileDlg.m_ofn.lpstrTitle="select test list file.";

	if(fileDlg.DoModal() == IDOK) 
	{
		CString fullpath = fileDlg.GetPathName();
		CString filename = fileDlg.GetFileTitle() + ".dat";
		m_treeTest.LoadFromXML(fullpath, filename);
	}
}

void CSMEyeAppDlg::OnContextBlankSave()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// test list �� �����Ѵ�.
	CString filter="Test List file (*.xml)|*.xml|";
	CFileDialog fileDlg(FALSE, "xml", "*.xml", NULL, filter, NULL);
	fileDlg.m_ofn.lpstrTitle="Select test list file.";

	if(fileDlg.DoModal() == IDOK) 
	{
		CString fullpath = fileDlg.GetPathName();
		CString filename = fileDlg.GetFileTitle() + ".dat";
		m_treeTest.SaveToXML(fullpath, filename, true);
	}
}


void CSMEyeAppDlg::OnTvnDeleteitemTreeTest(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	// context menu���� child item�� ���� ��츦 ���� �ڵ��̴�.
	// �� �κ��� tree�� clear�ϴ� ��쿡�� �Ҹ��鼭 error�� �����.
	// �̸� ���Ͽ� ������ �����Ͽ� context menu�� ���Ͽ� �Ҹ� ���� �����Ѵ�.
	
	if(this->m_bDeleteFromContextMenu)
	{
		// �̵��ϴ� item�� chart�� �׷���� �Ѵ�.
		//this->loadDataFromTreeItem(pNMTreeView->itemNew.hItem);
		
		
		//Context menu�� ���Ͽ� �̵��� item�� ����
		m_hItemforContextMenu = pNMTreeView->itemNew.hItem;
	}
	m_bDeleteFromContextMenu = false;
}

void CSMEyeAppDlg::OnBnClickedButtonEditSubjectInfo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->m_stereoCam.pause(true);

	CDlgSubjectInfo dlgSinfo;
	dlgSinfo.m_PI = this->m_patientInfo;
	if(dlgSinfo.DoModal() == IDOK)
	{
		//���� �̸� ����
	/*	CString oldName, newName;
		SHFILEOPSTRUCT sf;
		oldName.Format("%s%s%c", m_appOption.m_strDataFolder, m_patientInfo.makeSubjectFolderName(), '\0');
		newName.Format("%s%s%c", m_appOption.m_strDataFolder, dlgSinfo.m_PI.makeSubjectFolderName(), '\0');
		sf.wFunc = FO_RENAME;
		sf.pFrom = (LPSTR)(LPCTSTR)oldName;
		sf.pTo = (LPSTR)(LPCTSTR)newName;
		sf.fFlags = FOF_NOCONFIRMATION;
		SHFileOperation(&sf);

		//
		//m_strSubjectFolder = m_strAppPath + "data\\" + dlgSinfo.m_PI.makeSubjectFolderName() + "\\";
		m_strSubjectFolder = this->m_appOption.m_strDataFolder + dlgSinfo.m_PI.makeSubjectFolderName() + "\\";*/
		this->m_patientInfo = dlgSinfo.m_PI;
		GetDlgItem(IDC_STATIC_SUBJECT_INFO)->SetWindowText(m_patientInfo.makePIString());

		m_patientInfo.save(m_strSubjectFolder + PATIENT_INFO_FILENAME);

	}

	if(this->m_displaySource == DISPLAY_SOURCE_CAM)
		this->m_stereoCam.pause(false);

}

void CSMEyeAppDlg::OnBnClickedButtonNewSubjectInfo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->m_stereoCam.pause(true);

	//subject info
	CDlgSubjectInfo dlgSinfo;
	if(dlgSinfo.DoModal() == IDOK)
	{
		//m_strSubjectFolder = m_strAppPath + "data\\" + dlgSinfo.m_PI.makeSubjectFolderName() + "\\";
		m_strSubjectFolder = this->m_appOption.m_strDataFolder + dlgSinfo.m_PI.makeSubjectFolderName() + "\\";
		this->m_patientInfo = dlgSinfo.m_PI;
		GetDlgItem(IDC_STATIC_SUBJECT_INFO)->SetWindowText(m_patientInfo.makePIString());

		GetDlgItem(IDC_BUTTON_EDIT_SUBJECT_INFO)->EnableWindow(TRUE);

		if(corFolderExist(m_strSubjectFolder))
			::corDeleteAllFilesInFolder(m_strSubjectFolder);
		else
			::corCreateDirectory(m_strSubjectFolder);

		this->m_patientInfo.save(m_strSubjectFolder + PATIENT_INFO_FILENAME);

		//init tchart
		initTChart(0xff);

		//tree ���� �о�´�.
		this->m_treeTest.LoadFromXML(this->m_appOption.m_strAppFolder + "data\\" + TEST_LIST_FILENAME, m_appOption.m_strAppFolder + "data\\" + TEST_INFO_FILENAME);

		m_aviLoaderManager.close();
		m_displaySource = DISPLAY_SOURCE_CAM;
	}		
	
	if(this->m_displaySource == DISPLAY_SOURCE_CAM)
		this->m_stereoCam.pause(false);
	

}

void CSMEyeAppDlg::OnBnClickedButtonOption()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	this->m_stereoCam.pause(true);

	
	CDlgAppOption dlg;
	m_appOption.m_calibration[0] = m_stereoCam.m_eyeCalibration[0];
	m_appOption.m_calibration[1] = m_stereoCam.m_eyeCalibration[1];
	m_appOption.m_bSelectCodecByUser = m_stereoCam.m_aviGen.m_bSelectCodecByUser;
	dlg.setAppOption(this->m_appOption);
	
	if(dlg.DoModal() == IDOK)
	{
		this->m_appOption = dlg.getAppOption();
		this->m_stereoCam.m_eyeCalibration[0] = m_appOption.m_calibration[0];
		this->m_stereoCam.m_eyeCalibration[1] = m_appOption.m_calibration[1];
		m_stereoCam.m_aviGen.m_bSelectCodecByUser = m_appOption.m_bSelectCodecByUser;

		m_appOption.saveToRegistry();

		this->updateParams(true);

		if(m_CommACC.m_bConnected)
		{
			m_CommACC.ClosePort();
			Sleep(100);
			m_CommACC.OpenPort(m_appOption.m_strComportAcc);
		}
	}

	

	if(this->m_displaySource == DISPLAY_SOURCE_CAM)
		this->m_stereoCam.pause(false);
	
}

void CSMEyeAppDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnRButtonUp(nFlags, point);

	CRect rectCam, rectH, rectV;
	CMenu menu;
	CMenu* pSubmenu;

	//cam ������Ŭ������ ��
	GetDlgItem(IDC_STATIC_EYE_IMAGE_DISPLAY)->GetWindowRect(&rectCam);
//	GetDlgItem(IDC_STATIC_SERIES_EYE_LEFT_COLOR)->GetWindowRect(&rectH);
//	GetDlgItem(IDC_STATIC_SERIES_EYE_RIGHT_COLOR)->GetWindowRect(&rectV);
	
	
	if(rectCam.PtInRect(point))
	{
		//item�� Ŭ���ϰ�		
		menu.LoadMenu(IDR_CONTEXT_IMAGE);
	}
//	else if(rectH.PtInRect(point))
//	{
//		menu.LoadMenu(IDR_CONTEXT_CHART_POS);
//	}
//	else if(rectV.PtInRect(point))
//	{
//		menu.LoadMenu(IDR_CONTEXT_CHART_POS);
//	}
	
	if(menu.GetSafeHmenu())
	{

		//show context menu
	//	CPoint point;
	//	GetCursorPos(&point);
		pSubmenu = menu.GetSubMenu(0);
		pSubmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	return;


	
}

void CSMEyeAppDlg::OnContextImageResetTorsion()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CRect rect;
	CPoint point;

	::GetCursorPos(&point);
	HWND hwnd = ::WindowFromPoint(point);

	GetDlgItem(IDC_STATIC_EYE_IMAGE_DISPLAY)->GetWindowRect(rect);
	if(rect.PtInRect(point))
	{
		rect.right = rect.left + rect.Width()/2;
		int camIndex = 0;
		if(rect.PtInRect(point))	//right image
			camIndex = 1;
		else						//right image
			camIndex = 0;


		this->m_stereoCam.resetTorsion(camIndex);
	}
}


void CSMEyeAppDlg::OnZoomTchart()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	//���콺�� ��ġ�� chart�� ã�´�.
	for(int i=0; i<CHART_NUM; i++)
	{
		this->m_ChartCtrl[i].GetWindowRect(rect);
		if(this->m_ChartCtrl[i].GetVisible() && rect.PtInRect(pt))
			chartIdx = i;
	}

	//chartIdx �� bottom axis�� ���� �����.
	if(chartIdx != -1)
	{
		double m = this->m_ChartCtrl[chartIdx].GetAxis().GetBottom().GetMinimum();
		double M = this->m_ChartCtrl[chartIdx].GetAxis().GetBottom().GetMaximum();

		for(int i=0; i<CHART_NUM; i++)
		{
			if(i!= chartIdx)
			{
				this->m_ChartCtrl[i].GetAxis().GetBottom().SetMinMax(m, M);
				//�ٸ� chart�� zoom�� ����.
                m_ChartCtrl[i].GetZoom().SetEnable(false);
				m_ChartCtrl[i].GetScroll().SetEnable(false);
			}
			
		}
	}

	
	if(!m_bUnderTest)
	{
		m_ChartCtrl[0].Repaint();
		m_ChartCtrl[1].Repaint();

		EU_EventFile::adjustEventLocation(&(m_ChartCtrl[0]), SERIES_EVENT);
		EU_EventFile::adjustEventLocation(&(m_ChartCtrl[1]), SERIES_EVENT);
	}
	
}

void CSMEyeAppDlg::OnUndoZoomTchart()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	//���콺�� ��ġ�� chart�� ã�´�.
	for(int i=0; i<CHART_NUM; i++)
	{
		this->m_ChartCtrl[i].GetWindowRect(rect);
		if(this->m_ChartCtrl[i].GetVisible() && rect.PtInRect(pt))
			chartIdx = i;
	}

	//chartIdx �� bottom axis�� ���� �����.
	if(chartIdx != -1)
	{
		double m = this->m_ChartCtrl[chartIdx].GetAxis().GetBottom().GetMinimum();
		double M = this->m_ChartCtrl[chartIdx].GetAxis().GetBottom().GetMaximum();

		for(int i=0; i<CHART_NUM; i++)
		{
			if(i!= chartIdx)
			{
				this->m_ChartCtrl[i].GetAxis().GetBottom().SetMinMax(m, M);
				m_ChartCtrl[i].GetZoom().SetEnable(true);
				m_ChartCtrl[i].GetScroll().SetEnable(pmBoth);

			}

			
		}
	}

	
	if(!m_bUnderTest)
	{
		m_ChartCtrl[0].Repaint();
		m_ChartCtrl[1].Repaint();

		EU_EventFile::adjustEventLocation(&(m_ChartCtrl[0]), SERIES_EVENT);
		EU_EventFile::adjustEventLocation(&(m_ChartCtrl[1]), SERIES_EVENT);
	}

}



void CSMEyeAppDlg::OnScrollTchart()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	int chartIdx = -1;

	CRect rect;
	CPoint pt;
	::GetCursorPos(&pt);

	//���콺�� ��ġ�� chart�� ã�´�.
	for(int i=0; i<CHART_NUM; i++)
	{
		this->m_ChartCtrl[i].GetWindowRect(rect);
		if(this->m_ChartCtrl[i].GetVisible() && rect.PtInRect(pt))
			chartIdx = i;
	}

	//chartIdx �� bottom axis�� ���� �����.
	if(chartIdx != -1)
	{
		double m = this->m_ChartCtrl[chartIdx].GetAxis().GetBottom().GetMinimum();
		double M = this->m_ChartCtrl[chartIdx].GetAxis().GetBottom().GetMaximum();

		for(int i=0; i<CHART_NUM; i++)
		{
			if(i!= chartIdx)
			{
				this->m_ChartCtrl[i].GetAxis().GetBottom().SetMinMax(m, M);
				m_ChartCtrl[i].GetZoom().SetEnable(false);
				m_ChartCtrl[i].GetScroll().SetEnable(false);
			}

			
		}
	}

	
	if(!m_bUnderTest)
	{
		EU_EventFile::adjustEventLocation(&(m_ChartCtrl[0]), SERIES_EVENT);
		EU_EventFile::adjustEventLocation(&(m_ChartCtrl[1]), SERIES_EVENT);
	}


	
}





void CSMEyeAppDlg::OnClickAxisTchart(long Axis, long Button, long Shift, long X, long Y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// context �޴��� ��Ÿ����.

	// �׽�Ʈ���̸� ����
	if(this->m_bUnderTest)
		return;

	// axis�� leftAxis�� mouse ������ ��ư�� ���
	if((Axis == atLeft) && (Button == mbRight))
	{
		//axis�� Ŭ������ ��� �¿� series ��� ����
		m_cClickedSeries = BOTH;


		CMenu menu;
		CMenu* pSubmenu;
		CPoint point;
		GetCursorPos(&point);

		//���콺�� ��ġ�� ���� 
		m_cClickedChart = getChartIdxOnMouse(); 
		

		//horizontal�̳� vertical�� �ƴϸ� -1�� �����Ѵ�.
		if((m_cClickedChart == HORIZONTAL) || (m_cClickedChart == VERTICAL))
		{
			menu.LoadMenu(IDR_CONTEXT_CHART_POS);
			if(menu.GetSafeHmenu())
			{

				//show context menu
				
				pSubmenu = menu.GetSubMenu(0);
				pSubmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
			}
		}

		return;
	}
}


void CSMEyeAppDlg::OnClickSeriesTchart(long SeriesIndex, long ValueIndex, long Button, long Shift, long X, long Y)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
/*
	//�׽�Ʈ���̸� ����
	if(this->m_bUnderTest)
		return;



	if(((SeriesIndex == LEFT) ||  (SeriesIndex == RIGHT)) && (Button == mbRight))	//left Ȥ�� right�̸鼭 ���콺 ������ ��ư�̸�
	{
		m_cClickedChart = getChartIdxOnMouse();
		if((m_cClickedChart == HORIZONTAL) || (m_cClickedChart == VERTICAL))
		{
			m_cClickedSeries = SeriesIndex;

			CMenu menu;
			CMenu* pSubmenu;
			CPoint point;
			GetCursorPos(&point);

			menu.LoadMenu(IDR_CONTEXT_CHART_POS);
			if(menu.GetSafeHmenu())
			{

				//show context menu				
				pSubmenu = menu.GetSubMenu(0);
				pSubmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
			}
		}

	}*/
}


char CSMEyeAppDlg::getChartIdxOnMouse()
{
	char chartIdx = -1;

	CRect rectH, rectV;
	CPoint point;
	GetCursorPos(&point);

	//cam ������Ŭ������ ��
	m_ChartCtrl[HORIZONTAL].GetWindowRect(&rectH);
	m_ChartCtrl[VERTICAL].GetWindowRect(&rectV);
	
	
	if(rectH.PtInRect(point))
		chartIdx = HORIZONTAL;			
	else if(rectV.PtInRect(point))
		chartIdx = VERTICAL;			
	else
		chartIdx = -1;

	return chartIdx;
}

void CSMEyeAppDlg::OnContextChartposRemoveoffset()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if((m_cClickedChart == -1) || (m_cClickedSeries == -1))
		return;
	if(!m_pCurrentTI)
		return;
	//�¿� ��� ������ �Ѵٸ�
	if(m_cClickedSeries == BOTH)
	{
		removeOffset(&(this->m_ChartCtrl[m_cClickedChart].Series(LEFT).GetYValues()));
		removeOffset(&(this->m_ChartCtrl[m_cClickedChart].Series(RIGHT).GetYValues()));
	}
	//
	else
	{
		//left Ȥ�� right���� ���
		removeOffset(&(this->m_ChartCtrl[m_cClickedChart].Series(m_cClickedSeries).GetYValues()));
	}
	
	
}

double CSMEyeAppDlg::removeOffset(CValueList* pValueList)
{
	unsigned long count = pValueList->GetCount();
	if(count == 0)
		return 0;

	double* pDat = new double[count];

	for(unsigned long i=0; i<count; i++)
		pDat[i] = pValueList->GetValue(i);

	double m = corMedian(pDat, count);

	for(unsigned long i=0; i<count; i++)
		pValueList->SetValue(i, pDat[i]-m);


	delete []pDat;

	return 1;

}


void CSMEyeAppDlg::OnBnClickedButtonPrint()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_stereoCam.pause(true);

	CDlgPrintSetup dlgPrintSetup;
	dlgPrintSetup.setSubjectFolder(this->m_strSubjectFolder);
	dlgPrintSetup.setTIFilename(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
										m_strSubjectFolder + m_treeTest.getDefaultTIName());

	if(dlgPrintSetup.DoModal() == IDOK)
	{
		


		//test info�� ���� ����Ǿ��� �� �����Ƿ� tree�� ���� load.
		this->m_treeTest.clear();
		this->m_treeTest.LoadFromXML(m_strSubjectFolder + m_treeTest.getDefaultXMLName(), 
										m_strSubjectFolder + m_treeTest.getDefaultTIName());

		if(this->m_hItemSelected)
			this->m_treeTest.SelectItem(this->m_hItemSelected);
		

		ED_rptMaker rpt;
		
		//data folder �ؿ� �ӽ� print folder�� �����.
		//CString printFolder = m_strSubjectFolder + "print\\";
		//if(!corFolderExist(printFolder))
		//	corCreateDirectory(printFolder);

		if(rpt.init(m_strSubjectFolder))
		{
			CString strCurrentFolder = setCurrentWorkPath(rpt.getPrintFolder());

			rpt.setPatientInfo(this->m_patientInfo);
			
			rpt.makeReport(&m_treeTest);

			rpt.close();

			CReportGenerator report;
			report.New();

			POSITION pos;
			pos = rpt.m_listReportFiles.GetHeadPosition();
			CString fname = rpt.m_listReportFiles.GetNext(pos);
			report.AddTemplate(fname, TEMPLATE_TYPE_SECTION);
			for(int i=1; i<rpt.m_listReportFiles.GetCount(); i++)
			{
				fname = rpt.m_listReportFiles.GetNext(pos);
				
				report.AddTemplate(fname, TEMPLATE_TYPE_SECTION);

//				report.Clear();

			}
            report.Print();
			report.Clear();

			setCurrentWorkPath(strCurrentFolder);


		}

		//folder ������ �����
		//corDeleteAllFilesInFolder(printFolder);

		

	}

	m_stereoCam.pause(false);
}


void CSMEyeAppDlg::selectNextTest(HTREEITEM hCurrent)
{
	//testinfo�� ���� ������ ã�´�.
	HTREEITEM hNext = this->m_treeTest.GetNextChildItem(hCurrent);
	ED_TestInfo* pInfo = m_treeTest.getTestInfoFromItem(hNext);
	while(!pInfo)
	{
		hNext = m_treeTest.GetNextChildItem(hNext);
		//������ item�̶�� return
		if(hNext == NULL)
			return;
		pInfo = m_treeTest.getTestInfoFromItem(hNext);
	}

	//���϶���Ʈ�� �Ű��ش�.
	m_treeTest.SetItemState(hCurrent, 0, TVIS_DROPHILITED);
	m_treeTest.SetItemState(hNext, TVIS_DROPHILITED, TVIS_DROPHILITED );

	//Select���ش�.
	m_treeTest.Select(hNext, TVGN_CARET);
	this->m_pCurrentTI = pInfo;

	// data�� �ִٸ� load
	m_hItemSelected = hNext;
	if(this->loadDataFromTreeItem(hNext))
	{
		GetDlgItem(IDC_BUTTON_ANALYZE)->EnableWindow(true);		//data �� load�Ǿ����� analyze ��ư Ȱ��ȭ
	}
	else
		GetDlgItem(IDC_BUTTON_ANALYZE)->EnableWindow(false);

	//test info update
	this->updateTestInfo();
	
			
}
void CSMEyeAppDlg::OnBnClickedCheckRemoveOffset()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	this->loadCurrentTestData();
}


void CSMEyeAppDlg::setEventStrings(ED_TestInfo* pTI)
{

	if((!pTI) || (!pTI->m_strEvent))
	{

		//TI�� null �̰ų� , event string�� ���ٸ� Event #
		for(int i=0; i<EVENT_STRING_COUNT; i++)
			this->m_strEvents[i].Format("Event %d", i+1);
	}
	else
	{
		//TI�� �ְ� event string�� �ִٸ� event string���� �����.
		for(int i=0; i<EVENT_STRING_COUNT; i++)
			this->m_strEvents[i] = pTI->getEventString(i);
	}


	//event button string�� �ٲ۴�.
	DWORD ids[EVENT_STRING_COUNT] = {IDC_BUTTON_EVENT1, IDC_BUTTON_EVENT2, IDC_BUTTON_EVENT3, IDC_BUTTON_EVENT4,
					IDC_BUTTON_EVENT5, IDC_BUTTON_EVENT6, IDC_BUTTON_EVENT7, IDC_BUTTON_EVENT8, IDC_BUTTON_EVENT9};
	CString str;
	for(int i=0; i<EVENT_STRING_COUNT; i++)
	{
		str.Format("%s (F%d)", m_strEvents[i], i+1);
		GetDlgItem(ids[i])->SetWindowText(str);
	}
}
void CSMEyeAppDlg::OnBnClickedButtonEvent()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	// test ���� �ƴ϶�� �ƹ� �ϵ� ���� �ʴ´�.
	if(!m_bUnderTest)
		return;
	
	CPoint pt;
	::GetCursorPos(&pt);
	int idx = -1;



	//mouse�� ��� ��ư�� �ִ��� ã�´�.
	DWORD ids[EVENT_STRING_COUNT] = {IDC_BUTTON_EVENT1, IDC_BUTTON_EVENT2, IDC_BUTTON_EVENT3, IDC_BUTTON_EVENT4,
					IDC_BUTTON_EVENT5, IDC_BUTTON_EVENT6, IDC_BUTTON_EVENT7, IDC_BUTTON_EVENT8, IDC_BUTTON_EVENT9};
	CString str;
	CRect rect;
	for(int i=0; i<EVENT_STRING_COUNT; i++)
	{
		GetDlgItem(ids[i])->GetWindowRect(&rect);
		if(rect.PtInRect(pt))
			idx = i;
	}

	if((idx != -1) && (idx<EVENT_STRING_COUNT))
		addEvent(this->m_strEvents[idx]);

}

void CSMEyeAppDlg::addEvent(CString strEvent)
{
	//������ chart�� real time processin ��
	 double t = double(m_stereoCam.getTestFrameNum())/FRAMERATE;

	 double minY, maxY;
     long len;


	 //horizontal�� vertical���� �ٿ��ش�.
	double x, y;
	for(int i=0; i<2; i++)
	{
		minY = m_ChartCtrl[i].GetAxis().GetLeft().GetMinimum();
	    maxY = m_ChartCtrl[i].GetAxis().GetLeft().GetMaximum();

		len = m_ChartCtrl[i].GetAxis().GetLeft().CalcYPosValue(minY)-m_ChartCtrl[i].GetAxis().GetLeft().CalcYPosValue(maxY)-10;

		x = m_ChartCtrl[i].Series(0).GetXValues().GetLast();
		y = minY;
		m_ChartCtrl[i].Series(SERIES_EVENT).GetMarks().SetArrowLength(len);
		//x = y = 0;

		m_ChartCtrl[i].Series(SERIES_EVENT).AddXY(x,y,strEvent, NULL);			
	}
}

void CSMEyeAppDlg::OnBeforePrintTchartHorizontal()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	
}

/*
void CSMEyeAppDlg::OnBeforeDrawSeriesTchartHorizontal()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(!m_bUnderTest && m_ChartCtrl[0].Series(SERIES_EVENT).GetCount())
		EU_EventFile::adjustEventLocation(&(m_ChartCtrl[0]), SERIES_EVENT);

}

void CSMEyeAppDlg::OnBeforeDrawSeriesTchartVertical()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(!m_bUnderTest && m_ChartCtrl[1].Series(SERIES_EVENT).GetCount())
		EU_EventFile::adjustEventLocation(&(m_ChartCtrl[1]), SERIES_EVENT);
}*/
