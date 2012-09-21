// DlgAppOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SMEyeApp.h"
#include "DlgAppOption.h"
#include ".\dlgappoption.h"
#include "enumerateserial.h"
#include "corwlUtil.h"


// CDlgAppOption 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAppOption, CDialog)
CDlgAppOption::CDlgAppOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAppOption::IDD, pParent)
{
}

CDlgAppOption::~CDlgAppOption()
{
}

void CDlgAppOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COMPORT_TARGET, m_comboComportTarget);
	DDX_Control(pDX, IDC_COMBO_COMPORT_ACCELERATION, m_comboComportAcc);
	DDX_Control(pDX, IDC_COMBO_COMPORT_EOG, m_comboComportEOG);
}


BEGIN_MESSAGE_MAP(CDlgAppOption, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
//	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT_TARGET, OnCbnSelchangeComboComportTarget)
ON_CBN_SELCHANGE(IDC_COMBO_COMPORT_TARGET, OnCbnSelchangeComboComportTarget)
ON_CBN_SELCHANGE(IDC_COMBO_COMPORT_ACCELERATION, OnCbnSelchangeComboComportAcceleration)
ON_BN_CLICKED(IDC_BUTTON_CHANGE_DATA_FOLDER, OnBnClickedButtonChangeDataFolder)
ON_BN_CLICKED(IDC_BUTTON_OPTION_CALIBRATION_LOAD, OnBnClickedButtonOptionCalibrationLoad)
ON_CBN_SELCHANGE(IDC_COMBO_COMPORT_EOG, OnCbnSelchangeComboComportEog)
ON_BN_CLICKED(IDC_CHECK_USE_EOG, OnBnClickedCheckUseEog)
END_MESSAGE_MAP()


// CDlgAppOption 메시지 처리기입니다.

void CDlgAppOption::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//data folder
	GetDlgItem(IDC_STATIC_DATA_FOLDER)->GetWindowText(this->m_appOption.m_strDataFolder);

	//avi 저장
	this->m_appOption.m_bSaveAVI = ((CButton*)GetDlgItem(IDC_CHECK_SAVE_VIDEO))->GetCheck()==BST_CHECKED;
	this->m_appOption.m_bSelectCodecByUser = ((CButton*)GetDlgItem(IDC_CHECK_SELECT_CODEC_BY_USER))->GetCheck()==BST_CHECKED;
	this->m_appOption.m_bUseEOG  = ((CButton*)GetDlgItem(IDC_CHECK_USE_EOG))->GetCheck()==BST_CHECKED;

	//image display 순서
	// Right - Left 순서면 Flip은 true
	this->m_appOption.m_bSwitchCameras = ((CButton*)GetDlgItem(IDC_CHECK_SWITCH_CAMERAS))->GetCheck() == BST_CHECKED;



	OnOK();
}

BOOL CDlgAppOption::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString strPortName;
	int num = 0;
	int selectedTarget = 0;
	int selectedAcc = 0;
	int selectedEOG = 0;

	/*for(int i=0;i<10; i++)
	{
		if(i<10)
			strPortName.Format("COM%d", i);
		else
			strPortName.Format("\\\\.\\COM%d", i);

		HANDLE hComm = CreateFile(
							strPortName,					// the name of the object to create or open
							GENERIC_READ | GENERIC_WRITE,	// specify read/write operation to the object
							0,								// the object can not be shared and can not be opened again until the handle is closed
							NULL,							// the handle can not be inherited
							OPEN_EXISTING,					// opens the file, 디바이스 파일은 이미 존재하고 있다. (예 COM1)
							FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// specify file attributes and flags, return ERROR_IO_PENDING flag
							NULL);


		//포트가 존재하면 추가
		if(hComm != INVALID_HANDLE_VALUE)
		{
			strPortName.Format("COM%2d", i);

			//target comport
			m_comboComportTarget.AddString(strPortName);
			m_comboComportTarget.SetItemData(num, i);

			//acc comport
			m_comboComportAcc.AddString(strPortName);
			m_comboComportAcc.SetItemData(num, i);

			//eog comport
			m_comboComportEOG.AddString(strPortName);
			m_comboComportEOG.SetItemData(num, i);
			num++;


			SetCommMask(hComm, 0);	// 0 : disables all events
			PurgeComm(hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
			CloseHandle(hComm);

			//target combo의 selected list를 찾는다.
			CString str = m_appOption.m_strComportTarget;
			str.Delete(0, 3);
			if(i == atoi(str.GetBuffer()))
				selectedTarget = num;

			//acc combo의 selected list를 찾는다.
			str = m_appOption.m_strComportAcc;
			str.Delete(0, 3);
			if(i == atoi(str.GetBuffer()))
				selectedAcc = num;

			str = m_appOption.m_strComportEOG;
			str.Delete(0, 3);
			if(i == atoi(str.GetBuffer()))
				selectedEOG = num;

		}
	}

	this->m_comboComportTarget.SetCurSel(selectedTarget-1);
	this->m_comboComportAcc.SetCurSel(selectedAcc-1);
	this->m_comboComportEOG.SetCurSel(selectedEOG-1);*/
	setCombo();

	//data folder
	GetDlgItem(IDC_STATIC_DATA_FOLDER)->SetWindowText(m_appOption.m_strDataFolder);

	//avi 저장
	((CButton*)GetDlgItem(IDC_CHECK_SAVE_VIDEO))->SetCheck(m_appOption.m_bSaveAVI);
	((CButton*)GetDlgItem(IDC_CHECK_SELECT_CODEC_BY_USER))->SetCheck(m_appOption.m_bSelectCodecByUser);

	//use EOG
	((CButton*)GetDlgItem(IDC_CHECK_USE_EOG))->SetCheck(m_appOption.m_bUseEOG);
	if(m_appOption.m_bUseEOG)
		m_comboComportEOG.EnableWindow(true);

	//flip image
	if(this->m_appOption.m_bSwitchCameras == false)
        ((CButton*)GetDlgItem(IDC_CHECK_SWITCH_CAMERAS))->SetCheck(false);
	else
		((CButton*)GetDlgItem(IDC_CHECK_SWITCH_CAMERAS))->SetCheck(true);


	//calibration
	//결과를 보여준다.
	CString strLeft, strRight, str;
	strLeft.Format("\r\n Left \r\n %s", this->m_appOption.m_calibration[0].getModelStr());
	strRight.Format("\r\n Right \r\n %s", this->m_appOption.m_calibration[1].getModelStr());
	str.Format( "%s \r\n\r\n %s", strLeft, strRight);
	GetDlgItem(IDC_STATIC_OPTION_CALIB_COEFF)->SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgAppOption::setCombo()
{
	CUIntArray ports;
	CStringArray friendlyNames;
	CStringArray sPorts;
	INT_PTR i;
	//int num = 0;
	int selectedTarget = -1;
	int selectedEOG = -1;
	int selectedACC = -1;

	CString strPortName;

	int idx;
	if (CEnumerateSerial::UsingSetupAPI2(ports, friendlyNames))
	{
	  for (i=0; i<ports.GetSize(); i++)
	  {

		  strPortName.Format(("COM%d <%s>"), ports[i], friendlyNames[i]);
		  
		  int n = ports[i];

		  idx = m_comboComportAcc.AddString(strPortName);
		  m_comboComportAcc.SetItemData(idx, n);
		  idx = m_comboComportEOG.AddString(strPortName);
		  m_comboComportEOG.SetItemData(idx, n);
		  idx = m_comboComportTarget.AddString(strPortName);
		  m_comboComportTarget.SetItemData(idx, n);

		  n = m_comboComportEOG.GetItemData(idx);

		  //target combo의 selected list를 찾는다.
          CString str = m_appOption.m_strComportTarget;
		  str = str.Right(str.GetLength()-str.Find("COM", 0)-3);
          if(ports[i] == atoi(str.GetBuffer()))
              selectedTarget = idx;

          //acc combo의 selected list를 찾는다.
          str = m_appOption.m_strComportAcc;
          str = str.Right(str.GetLength()-str.Find("COM", 0)-3);
		  if(ports[i] == atoi(str.GetBuffer()))
              selectedACC = idx;

          str = m_appOption.m_strComportEOG;
          str = str.Right(str.GetLength()-str.Find("COM", 0)-3);
		  if(ports[i] == atoi(str.GetBuffer()))
              selectedEOG = idx;
	  }

	  m_comboComportTarget.SetCurSel((selectedTarget != -1) ? selectedTarget : 0);
	  
	  this->m_comboComportTarget.SetCurSel((selectedTarget != -1) ? selectedTarget : 0);
      this->m_comboComportAcc.SetCurSel((selectedACC != -1) ? selectedACC : 0);
      this->m_comboComportEOG.SetCurSel((selectedEOG != -1) ? selectedEOG : 0);
	  
	}
}

//void CDlgAppOption::OnCbnSelchangeComboComportTarget()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

void CDlgAppOption::OnCbnSelchangeComboComportTarget()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int portNum = m_comboComportTarget.GetItemData(m_comboComportTarget.GetCurSel());
	if(portNum<10)
		m_appOption.m_strComportTarget.Format(_T("COM%d"), portNum);
	else
		m_appOption.m_strComportTarget.Format(_T("\\\\.\\COM%d"), portNum);
}

void CDlgAppOption::OnCbnSelchangeComboComportAcceleration()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int portNum = m_comboComportAcc.GetItemData(m_comboComportAcc.GetCurSel());
	if(portNum<10)
		m_appOption.m_strComportAcc.Format(_T("COM%d"), portNum);
	else
		m_appOption.m_strComportAcc.Format(_T("\\\\.\\COM%d"), portNum);
	
}


void CDlgAppOption::OnCbnSelchangeComboComportEog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int portNum = m_comboComportEOG.GetItemData(m_comboComportEOG.GetCurSel());
	if(portNum<10)
		m_appOption.m_strComportEOG.Format(_T("COM%d"), portNum);
	else
		m_appOption.m_strComportEOG.Format(_T("\\\\.\\COM%d"), portNum);

}

void CDlgAppOption::OnBnClickedButtonChangeDataFolder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*ITEMIDLIST *pidlBrowse; 
    char pszPathname[MAX_PATH];
	::ZeroMemory(pszPathname, MAX_PATH);
	::memcpy(pszPathname, m_appOption.m_strDataFolder.GetBuffer(), m_appOption.m_strDataFolder.GetLength());
    BROWSEINFO BrInfo;
    BrInfo.hwndOwner = GetSafeHwnd(); 
    BrInfo.pidlRoot = NULL;

    memset(&BrInfo, 0, sizeof(BrInfo));
    BrInfo.pszDisplayName = pszPathname;
    BrInfo.lpszTitle = "경로 선택 :";
    BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

    pidlBrowse = ::SHBrowseForFolder(&BrInfo);


    if( pidlBrowse != NULL)
    {
        ::SHGetPathFromIDList(pidlBrowse, pszPathname);
        CString str(pszPathname);  // 폴더 경로
		this->m_appOption.m_strDataFolder = str;
		m_appOption.m_strDataFolder += "\\";
		GetDlgItem(IDC_STATIC_DATA_FOLDER)->SetWindowText(m_appOption.m_strDataFolder);
    }*/

	CString strFolder;

	if(corwlSelectFolder(&strFolder, this, m_appOption.m_strDataFolder))
	{
		m_appOption.m_strDataFolder = strFolder + "\\";
		GetDlgItem(IDC_STATIC_DATA_FOLDER)->SetWindowText(m_appOption.m_strDataFolder);




	}
}

void CDlgAppOption::OnBnClickedButtonOptionCalibrationLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString filter="Eye Movement Calibration file (*.emc)|*.emc";
	CFileDialog fileDlg(TRUE, "emc", "*.emc", NULL, filter, NULL);
	fileDlg.m_ofn.lpstrTitle="Input the Eye Movement Calibration data filename.";

	if(fileDlg.DoModal() == IDOK) 
	{

		CString filename = fileDlg.GetPathName();
		
		CFile file;
		if(file.Open(filename, CFile::modeRead))
		{
			unsigned int ver = 1;
			
			file.Read(&ver, sizeof(ver));
			//left
			file.Read(this->m_appOption.m_calibration[0].m_dHCoeff, sizeof(double)*6);
			file.Read(this->m_appOption.m_calibration[0].m_dVCoeff, sizeof(double)*6);
			file.Read(&(m_appOption.m_calibration[0].m_dRCoeff), sizeof(double));
			//right
			file.Read(this->m_appOption.m_calibration[1].m_dHCoeff, sizeof(double)*6);
			file.Read(this->m_appOption.m_calibration[1].m_dVCoeff, sizeof(double)*6);
			file.Read(&(m_appOption.m_calibration[1].m_dRCoeff), sizeof(double));

			file.Close();

			m_appOption.m_calibration[0].m_bCalibrated = true;
			m_appOption.m_calibration[1].m_bCalibrated = true;

		}

		//결과를 보여준다.
		CString strLeft, strRight, str;
		strLeft.Format("\r\n Left \r\n %s", this->m_appOption.m_calibration[0].getModelStr());
		strRight.Format("\r\n Right \r\n %s", this->m_appOption.m_calibration[1].getModelStr());
		str.Format( "%s \r\n %s", strLeft, strRight);
		GetDlgItem(IDC_STATIC_OPTION_CALIB_COEFF)->SetWindowText(str);
	}
}



void CDlgAppOption::OnBnClickedCheckUseEog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(((CButton*)GetDlgItem(IDC_CHECK_USE_EOG))->GetCheck())
		GetDlgItem(IDC_COMBO_COMPORT_EOG)->EnableWindow(true);
	else
		GetDlgItem(IDC_COMBO_COMPORT_EOG)->EnableWindow(false);

}

