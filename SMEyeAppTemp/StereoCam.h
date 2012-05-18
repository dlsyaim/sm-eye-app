#pragma once
#include "PTCamera.h"
//#include "pgrflycapture.h"
//#include "pgrflycaptureplus.h"
#include "imageDisplay.h"
//#include "avifile.h"
#include "avigenerator.h"
#include "ptavifile.h"
#include "eu_queeyeinfos.h"
#include "commdrive.h"
#include "commled.h"
#include "eu_testfilev2.h"
#include "ea_analzeManager.h"
#include "ea_nystagmus.h"
#include "ed_appoption.h"
#include "flycapture2.h"

#define _MAX_CAMERAS 64
#define WM_DRAW_CHART	(WM_USER+4)

//#define PTAVI

class CStereoCam
{
public:
	CStereoCam(void);
	~CStereoCam(void);

	bool m_bUnderTest;
	bool m_bAnalyze;
protected:
	unsigned int m_uiCameras;
	CameraInfo m_arInfo[_MAX_CAMERAS];
	CPTCamera m_Cameras[_MAX_CAMERAS];
	
	ED_AppOption* m_pAppOption;

	CCommDrive* m_pCommDrive;
	CCommLED* m_pCommLED;

	unsigned long m_ulFrameNum;			//전체 grab한 frame num
	unsigned long m_ulTestFrameNum;		//test 시작후 grab한 framenum

	IplImage* m_pStereoImage;
	IplImage* m_pStereoImageMono;
	void initCV();
	void releaseCV();

public:
	
	bool start(void);
	bool stop(void);
	bool pause(bool bPause);
	void resetTorsion(unsigned int camIdx);
	void setUnderTest(bool bUnderTest);

private:
	HWND m_hWndParent;
	bool m_bGrabThreadOn;
	static UINT grabThread(void* lParam);
	HANDLE m_hThread;
	bool m_bThreadSuspend;

	HANDLE m_eventGrab[2];
	
	
public:
	void processImage(void);
	void setImageDisplay(CImageDisplay* pDisplay);
	void setAppOption(ED_AppOption* pOption){this->m_pAppOption = pOption;};
	void setSaveImage(bool bSave, CString fname);
	ED_EyeInfo* getEyeInfo();
	unsigned long getFrameNum();
	unsigned long getTestFrameNum();
	void setParentHWnd(HWND hwnd);
	EU_QueEyeInfos m_QueEyeInfos;
	double m_ACCes[64];
	void setTriggerMode(bool on);
	bool m_triggerMode;

	//analyze
	EA_AnalzeManager m_analManager;

	void getFrameInfo(double* fr0, double* fr1, unsigned int* mfn0, unsigned int *mfn1);
private:
	CImageDisplay* m_pImageDisplay;

public:
#ifdef PTAVI
	CPTAviFile m_ptAviWriter;
#else
	//CAviFile m_aviWriter;
#endif
	CAVIGenerator m_aviGen;




	//결과 저장
	ED_EyeInfo m_EyeInfos[2];	
public:
	void setCommDrive(CCommDrive* pCommDrive, CCommLED* pCommLED);
	EU_TestFileV2 m_euTestFile;
	CCalibration m_eyeCalibration[2];
};
