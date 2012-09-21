#pragma once
#include "Calibration.h"

class ED_AppOption
{
public:
	ED_AppOption(void);
	~ED_AppOption(void);

public:
	CString m_strComportAcc;
	CString m_strComportTarget;
	CString m_strDataFolder;
	CString m_strAppFolder;	 // application folder

	bool m_bSaveAVI;
	bool m_bSelectCodecByUser;
	bool m_bSwitchCameras;

	unsigned char m_ucThresLeft;
	unsigned char m_ucThresRight;

	bool m_bUseEOG;
	CString m_strComportEOG;

	CCalibration m_calibration[2];

	void saveToRegistry();
	void loadFromRegistry();
};
