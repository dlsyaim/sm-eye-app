#pragma once

#include "corwlDefines.h"

#include "cv.h"
#include "highgui.h"

#include "smeye.h"
#include "calibration.h"

#include "flycapture2defs.h"
#include "flycapture2.h"

//#include "pgrflycapture.h"
//#include "pgrflycaptureplus.h"


#define INITIALIZE				 0x000
#define CAMERA_POWER		0x610
#define _MAX_IMAGE_SIZE 320*240
#define TIME_STAMP_BUF_LENGTH		10
#define _BUFFERS 				120// Buffers per camera.

using namespace FlyCapture2;


//#define GRAB_TYPE   LOCK_LATEST

class CPTCamera
{
public:
	CPTCamera(void);
	~CPTCamera(void);

protected:
	ED_EyeInfo m_EyeInfo;
	bool m_bTorsionReset;

//pt camera
public:
	Camera m_fcCamera;
	CameraInfo m_camInfo;
	Image m_rawImage;
	bool retrieveBuffer();
	TimeStamp m_prevTimestamp;
	bool m_bIsNewOne;


//	FlyCaptureContext* m_pContext;
//	FlyCaptureImagePlus m_arImagePlus;
	unsigned char m_ucBandwidth;

	unsigned char** m_arpBuffers;

protected:
	int m_nCameraIndex;
	void allocateBuffers(void);
	void deallocateBuffers(void);
	
	
	bool extractImageData(void);
public:
	bool initCamera(FlyCapture2::PGRGuid* pguid, int idx, HANDLE* pGrabEvent);
	bool start();
	bool stop();
	bool pause(bool bPause);
	double getFramerate();
	unsigned int getMissingFrameNum();
	ED_EyeInfo* getEyeInfo();
	void resetTorsion();

	bool processImage(bool bMakeResult = false);
	IplImage* getResultImage();
	IplImage* getOriginalImage();
	
	void setTriggerMode(bool on);
	//eye position calibration
	
	
	
//opencv
protected:
	IplImage* m_pImage;
	IplImage* m_pImageRGB;
	int m_nUnprocessedImage;
	

	HANDLE* m_pGrabEvent;
	bool m_bGrabThreadOn;
	static UINT grabThread(void* lParam);
	HANDLE m_hThread;
	bool m_bThreadSuspend;	

public:
	unsigned int m_timeCycleCount;
protected:
	void initCV();
	void releaseCV();

protected:
	unsigned long m_ulFrameNum;
	//unsigned int m_uiMissingFrameNum;


	
};
