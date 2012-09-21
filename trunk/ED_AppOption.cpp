#include "StdAfx.h"
#include ".\ed_appoption.h"
#include "corwlUtil.h"

ED_AppOption::ED_AppOption(void)
{
	//registry로부터 읽어올 때 default값을 위하여 설정
	corGetProgramFolder(&m_strAppFolder);
	m_strAppFolder += "\\";
	this->m_strDataFolder += m_strAppFolder+"data\\";
	m_bSwitchCameras = false;

	m_bUseEOG = false;

	loadFromRegistry();
}

ED_AppOption::~ED_AppOption(void)
{
}


void ED_AppOption::saveToRegistry()
{
	::AfxGetApp()->WriteProfileString("SMEyeApp", "PortNumAcc", m_strComportAcc);
	::AfxGetApp()->WriteProfileString("SMEyeApp", "PortNumTarget", m_strComportTarget);
	::AfxGetApp()->WriteProfileString("SMEyeApp", "DataFolder", m_strDataFolder);
	::AfxGetApp()->WriteProfileInt("SMEyeApp", "SaveAVI", m_bSaveAVI ? 1 : 0);
	::AfxGetApp()->WriteProfileInt("SMEyeApp", "SelectCodecByUser", m_bSelectCodecByUser ? 1 : 0);
	::AfxGetApp()->WriteProfileInt("SMEyeApp", "SwitchCameras", m_bSwitchCameras ? 1 : 0);

	::AfxGetApp()->WriteProfileInt("SMEyeApp", "thresLeft", m_ucThresLeft);
	::AfxGetApp()->WriteProfileInt("SMEyeApp", "thresRight", m_ucThresRight);

	::AfxGetApp()->WriteProfileInt("SMEyeApp", "UseEOG", m_bUseEOG ? 1:0);
	::AfxGetApp()->WriteProfileString("SMEyeApp", "PortNumEOG", m_strComportEOG);

	//calibration coeff
	unsigned char buf[104];
	::ZeroMemory(buf, 104);
	memcpy(buf, m_calibration[0].m_dHCoeff, sizeof(double)*6);
	memcpy(&(buf[48]), m_calibration[0].m_dVCoeff, sizeof(double)*6);
	memcpy(&(buf[96]), &(m_calibration[0].m_dRCoeff), sizeof(double));
	::AfxGetApp()->WriteProfileBinary("SMEyeApp", "calibLeft", buf, 104);

	memcpy(buf, m_calibration[1].m_dHCoeff, sizeof(double)*6);
	memcpy(&(buf[48]), m_calibration[1].m_dVCoeff, sizeof(double)*6);
	memcpy(&(buf[96]), &(m_calibration[1].m_dRCoeff), sizeof(double));
	::AfxGetApp()->WriteProfileBinary("SMEyeApp", "calibRight", buf, 104);

	


}

void ED_AppOption::loadFromRegistry()
{
	m_strComportAcc = ::AfxGetApp()->GetProfileString("SMEyeApp", "PortNumAcc", "COM2");
	m_strComportTarget = ::AfxGetApp()->GetProfileString("SMEyeApp", "PortNumSti", "COM1");
	m_strDataFolder = ::AfxGetApp()->GetProfileString("SMEyeApp", "DataFolder", m_strDataFolder);
	this->m_bSaveAVI = ::AfxGetApp()->GetProfileInt("SMEyeApp", "SaveAVI", 0)==1;
	m_bSelectCodecByUser = ::AfxGetApp()->GetProfileInt("SMEyeApp", "SelectCodecByUser", 0)==1;
	this->m_bSwitchCameras = ::AfxGetApp()->GetProfileInt("SMEyeApp", "SwitchCameras", 0)==1;	//default은 false

	m_ucThresLeft = ::AfxGetApp()->GetProfileInt("SMEyeApp", "thresLeft", 3);
	m_ucThresRight = ::AfxGetApp()->GetProfileInt("SMEyeApp", "thresRight", 3);

	m_bUseEOG = ::AfxGetApp()->GetProfileInt("SMEyeApp", "UseEOG", 0) == 1;
	m_strComportEOG = ::AfxGetApp()->GetProfileString("SMEyeApp", "PortNumEOG", "COM1");

	UINT count = 0;
	unsigned char* pBuf;
	

	if(::AfxGetApp()->GetProfileBinary("SMEyeApp", "calibLeft", (LPBYTE*)&pBuf, &count))
	{
		if(count == 104)
		{
			memcpy(m_calibration[0].m_dHCoeff, pBuf, sizeof(double)*6);
			memcpy(m_calibration[0].m_dVCoeff, &(pBuf[48]), sizeof(double)*6);
			memcpy(&(m_calibration[0].m_dRCoeff), &(pBuf[96]), sizeof(double));
			m_calibration[0].m_bCalibrated = true;
		}

		delete []pBuf;
	}
	
	if(::AfxGetApp()->GetProfileBinary("SMEyeApp", "calibRight", (LPBYTE*)&pBuf, &count))
	{
		if(count == 104)
		{
			memcpy(m_calibration[1].m_dHCoeff, pBuf, sizeof(double)*6);
			memcpy(m_calibration[1].m_dVCoeff, &(pBuf[48]), sizeof(double)*6);
			memcpy(&(m_calibration[1].m_dRCoeff), &(pBuf[96]), sizeof(double));
			m_calibration[1].m_bCalibrated = true;
		}

		delete []pBuf;
	}
}
