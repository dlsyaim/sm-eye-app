#include "StdAfx.h"
#include ".\ptcamera.h"

using namespace FlyCapture2;

#define _ENTER_MUTEX( handle, time ) \
   { \
      DWORD dwErrorMutex = ::WaitForSingleObject( handle, time ); \
      ASSERT( dwErrorMutex == WAIT_OBJECT_0 ); \
   } \

#define _EXIT_MUTEX( handle ) ::ReleaseMutex( handle );



CPTCamera::CPTCamera(void)
:
//m_pContext(NULL)
//, m_arpBuffers(NULL)
 m_pImage(NULL)
, m_pImageRGB(NULL)
, m_nCameraIndex(-1)
, m_ucBandwidth(100)
, m_ulFrameNum(0)
,m_timeCycleCount(0)
, m_bTorsionReset(true)
{



	initCV();		
	allocateBuffers();

	m_pGrabEvent = NULL;
	m_bGrabThreadOn  = false;


	m_bIsNewOne = false;

}

CPTCamera::~CPTCamera(void)
{
	this->m_bGrabThreadOn = false;
}

UINT CPTCamera::grabThread(void* lParam)
{
	CPTCamera* pCam = (CPTCamera*)lParam;
	
		
	while(pCam->m_bGrabThreadOn==true)
	{
		if(pCam->retrieveBuffer())
		{
			SetEvent(pCam->m_pGrabEvent);
		}
		
	}

	
	//::AfxEndThread(0);
	return 0;
}

void CPTCamera::allocateBuffers(void)
{
	if(!m_arpBuffers)
	{
		m_arpBuffers = new unsigned char*[_BUFFERS];
		for(unsigned int i=0; i<_BUFFERS; i++)
			m_arpBuffers[i] = new unsigned char[_MAX_IMAGE_SIZE];
	}

}

void CPTCamera::deallocateBuffers(void)
{
	try
	{
		if(m_arpBuffers)
		{
			for(unsigned int i=0; i<_BUFFERS; i++)
			{
				delete []m_arpBuffers[i];
				m_arpBuffers[i] = NULL;
			}
			delete []m_arpBuffers;
			m_arpBuffers = NULL;
		}
	}
	catch(char* e)
	{
		::AfxMessageBox("deallocation error");
	}

}

void CPTCamera::initCV()
{
	if(!m_pImage)
		m_pImage = ::cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
	if(!m_pImageRGB)
		m_pImageRGB = ::cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
}

void CPTCamera::releaseCV()
{
	if(m_pImage)
		::cvReleaseImage(&m_pImage);
	if(m_pImageRGB)
		::cvReleaseImage(&m_pImageRGB);
}

bool CPTCamera::start()
{
	m_bTorsionReset = true;

	Error error = m_fcCamera.StartCapture();
    if (error != PGRERROR_OK)
    {
		::AfxMessageBox(_T("Error in StartCapture()"));
		return false;
    }


	m_bThreadSuspend = false;
	if(!m_bGrabThreadOn)
	{
		m_bGrabThreadOn = true;

		::Sleep(100);
		CWinThread *pThread = ::AfxBeginThread(grabThread, this, THREAD_PRIORITY_TIME_CRITICAL);
		this->m_hThread = pThread->m_hThread;
	}
	return true;
}


bool CPTCamera::stop()
{
	// Stop capturing images
    Error error = m_fcCamera.StopCapture();
    if (error != PGRERROR_OK)
    {
        ::AfxMessageBox(_T("Error in StopCapture()"));
		return false;
    }      

    // Disconnect the camera
    error = m_fcCamera.Disconnect();
    if (error != PGRERROR_OK)
    {
        ::AfxMessageBox(_T("Error in Disconnect()"));
        return false;
    }

	m_bGrabThreadOn = false;

	WaitForSingleObject( m_hThread, 1000);
	

	return true;

	
	
}

bool CPTCamera::pause(bool bPause)
{
	bool bRtn = true;

	
	unsigned int ulISOEN = 0;

	m_fcCamera.ReadRegister(0x614, &ulISOEN);

	if(bPause)
	{
		ulISOEN = ulISOEN & 0x7FFFFFFF;
	}
	else
	{
		ulISOEN |= ( 0x1 << 31 );
	}

	Error error = this->m_fcCamera.WriteRegister(0x614, ulISOEN);
	if (error != PGRERROR_OK)
    {
        //::AfxMessageBox(_T("Error in WriteRegister()"));
		return false;
    }    
	else
		return true;


	
	return bRtn;
}

bool CPTCamera::initCamera(FlyCapture2::PGRGuid* pguid, int idx, HANDLE* pGrabEvent)
{
//	this->m_pContext = pContext;
	this->m_nCameraIndex = idx;

	this->m_pGrabEvent = pGrabEvent;
	*m_pGrabEvent = CreateEvent(NULL, false, false, "Grab Event");

	//connect to camera
	Error error = m_fcCamera.Connect( pguid );
    if (error != PGRERROR_OK)
    {
        //::AfxMessageBox(_T("Error in Connect()."));
    }

	//get the camera information
	error = m_fcCamera.GetCameraInfo( &m_camInfo );
	if (error != PGRERROR_OK)
	{
		//::AfxMessageBox(_T("Error in GetCameraInfo()."));
	}

	// Query for available Format 7 modes
    Format7Info fmt7Info;
    bool supported;
    fmt7Info.mode = MODE_1;
    error = m_fcCamera.GetFormat7Info( &fmt7Info, &supported );
    if (error != PGRERROR_OK)
	{
		//::AfxMessageBox(_T("Error in GetFormat7Info()."));
	}
    if ( (PIXEL_FORMAT_MONO8 & fmt7Info.pixelFormatBitField) == 0 )
	{
		//::AfxMessageBox(_T("Error in pixelFormatBitField()."));
	}

	Format7ImageSettings fmt7ImageSettings;
    fmt7ImageSettings.mode = MODE_1;
    fmt7ImageSettings.offsetX = 0;
    fmt7ImageSettings.offsetY = 0;
    fmt7ImageSettings.width = IMAGE_WIDTH;
    fmt7ImageSettings.height = IMAGE_HEIGHT;
    fmt7ImageSettings.pixelFormat = PIXEL_FORMAT_MONO8;

    bool valid;
    Format7PacketInfo fmt7PacketInfo;

    // Validate the settings to make sure that they are valid
    error = m_fcCamera.ValidateFormat7Settings(
        &fmt7ImageSettings,
        &valid,
        &fmt7PacketInfo );
    if (error != PGRERROR_OK)
	{
		//::AfxMessageBox(_T("Error in ValidateFormat7Settings()."));
	}

    if ( !valid )
	{
		//::AfxMessageBox(_T("Error in Format 7 setting()."));
	}

    // Set the settings to the camera
    error = m_fcCamera.SetFormat7Configuration(
        &fmt7ImageSettings,
        fmt7PacketInfo.recommendedBytesPerPacket );

	if (error != PGRERROR_OK)
	{
		//::AfxMessageBox(_T("Error in SetFormat7Configuration()."));
	}

	// Enable timestamping
    EmbeddedImageInfo embeddedInfo;

    error = m_fcCamera.GetEmbeddedImageInfo( &embeddedInfo );
    if ( error != PGRERROR_OK )
	{
		//::AfxMessageBox(_T("Error in GetEmbeddedImageInfo()."));
	}

    if ( embeddedInfo.timestamp.available != 0 )
    {       
        embeddedInfo.timestamp.onOff = true;
    }    

    error = m_fcCamera.SetEmbeddedImageInfo( &embeddedInfo );
    if ( error != PGRERROR_OK )
	{
		//::AfxMessageBox(_T("Error in SetEmbeddedImageInfo()."));
	}


	

	//frame 정보 관련 변수 초기화
	m_ulFrameNum = 0;


	//strobe signal
	unsigned int ulValue = 0;

	m_fcCamera.ReadRegister(1504, &ulValue );

	ulValue |= 0x02000000;
	
	error = this->m_fcCamera.WriteRegister(0x1504, ulValue);


	/*

	// Power-up the camera (for cameras that support this feature)
    error = flycaptureSetCameraRegister( *m_pContext, CAMERA_POWER, 0x80000000 );
	if(error != FLYCAPTURE_OK)
		return false;


	//time stamp를 셋팅한다.
	//set timestamping
	error = ::flycaptureSetImageTimestamping(*m_pContext , true );

	//custom mode로 만든다.
	bool bAvailable;
	unsigned int uiMaxImagePixelWidth, uiMaxImagePixelHeight, uiPixelUnitHorz, uiPixelUnitVert, uiOffsetUnitHorz, uiOffsetUnitVert, uiPixelFormats;
	error = ::flycaptureQueryCustomImageEx(*m_pContext, FLYCAPTURE_VIDEOMODE_320x240YUV422,//FLYCAPTURE_VIDEOMODE_CUSTOM, 
		&bAvailable, 
		&uiMaxImagePixelWidth, 
		&uiMaxImagePixelHeight, 
		&uiPixelUnitHorz, 
		&uiPixelUnitVert, 
		&uiOffsetUnitHorz, 
		&uiOffsetUnitVert, 
		&uiPixelFormats);

*/
/*
	//TAN2005002 참조
	//strobe signal을 GPIO1 에서 출력하게 한다.
	unsigned long ulValue;
	//PIO_DIRECTION register를 얻어온다.
	error = flycaptureGetCameraRegister(*m_pContext, 0x11f8, &ulValue );
	ulValue = 0x40000000;	//bit 1을 set. GPIO1 output
	flycaptureSetCameraRegister(*m_pContext, 0x11f8, ulValue);

	//STROBE_1_INQ register setting
	error = flycaptureGetCameraRegister(*m_pContext, 0x1404, &ulValue );
	//적절한 값인지 확인한다.

	//STROBE_1_CNT register setting
	error = flycaptureGetCameraRegister(*m_pContext, 0x1504, &ulValue );



	if(error != FLYCAPTURE_OK)
		return false;

	
	///
	
	flycaptureGetCameraRegister(*m_pContext, 0x830, &ulValue);
	*/

	return true;
}




bool CPTCamera::retrieveBuffer()
{
	Error error = m_fcCamera.RetrieveBuffer( &m_rawImage );
    if (error != PGRERROR_OK)
    {
        //TRACE(_T("Error in RetrieveBuffer()\n"));
		return false;
    }

	TimeStamp timestamp = m_rawImage.GetTimeStamp();
	this->m_timeCycleCount = timestamp.cycleCount;
	//if(timestamp.cycleCount != m_prevTimestamp.cycleCount)
	{
		//if(this->m_nCameraIndex == 0)
		{
			int gap = timestamp.cycleCount-m_prevTimestamp.cycleCount;
			if(gap > 70)
			{
				//TRACE(".......%d, %d us \n",m_nCameraIndex,  timestamp.cycleCount-m_prevTimestamp.cycleCount);
			}
		}
		extractImageData();
		m_bIsNewOne = true;
		m_prevTimestamp = timestamp;
	}

	return true;

}


//bMakeResult == true면 result 이미지를 만든다.
bool CPTCamera::processImage(bool bMakeResult)
{
	//image data를 cvimage로 옮긴다.
	//if(this->extractImageData())
	{

		//eye position을 찾는다.
		//bMakeResult면 result image를 만든다.
		//bTorsionReset이면 torsion을 initialize한다.
		
		
		::smGetEyeInfoMono(m_pImage, &m_EyeInfo, m_pImageRGB, this->m_nCameraIndex, m_bTorsionReset);

		//calibratione
		double eyePixel[2] = {m_EyeInfo.m_fCx, m_EyeInfo.m_fCy};
		

		m_bTorsionReset = false;

		m_bIsNewOne = false;


		return true;
	}

	return false;

	
}

double CPTCamera::getFramerate()
{
	return 0;
}

ED_EyeInfo* CPTCamera::getEyeInfo()
{
	return &m_EyeInfo;
}


bool CPTCamera::extractImageData(void)
{

	// Retrieve frame rate property
    Property frmRate;
    frmRate.type = FRAME_RATE;
    m_fcCamera.GetProperty( &frmRate );

	if(!m_pImage || !this->m_rawImage.GetData())
	{
		//unsigned char* pBuf = m_rawImage.GetData();
		//::AfxMessageBox("error in CPTCamera::extractImageData pointer assertion!!");
		return false;
	}

    ::memcpy(m_pImage->imageData, m_rawImage.GetData(), sizeof(char)*IMAGE_WIDTH*IMAGE_HEIGHT);

	return true;
	
}

IplImage* CPTCamera::getResultImage()
{
	return this->m_pImageRGB;
}

IplImage* CPTCamera::getOriginalImage()
{
	return this->m_pImage;
}

void CPTCamera::resetTorsion()
{
	this->m_bTorsionReset = true;
}


void CPTCamera::setTriggerMode(bool on)
{
	unsigned int ulValue = 0;

	m_fcCamera.ReadRegister(0x830, &ulValue );

	if(on)
		ulValue |= 0x02000000;
	else
		ulValue &= ~0x02000000;

	Error error = this->m_fcCamera.WriteRegister(0x830, ulValue);
	if (error != PGRERROR_OK)
    {
        ::AfxMessageBox(_T("Error in WriteRegister()"));
    }    


}