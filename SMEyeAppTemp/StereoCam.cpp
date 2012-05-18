#include "StdAfx.h"
#include ".\stereocam.h"
#include "corwlDefines.h"
#include "corwlUtil.h"


using namespace FlyCapture2;


CStereoCam::CStereoCam(void)
: m_uiCameras(_MAX_CAMERAS)
, m_pStereoImage(NULL)
, m_pStereoImageMono(NULL)
, m_ulFrameNum(0)
, m_pImageDisplay(NULL)
, m_hThread(NULL)
, m_bGrabThreadOn(false)
{
//	this->m_ptAviWriter.enumerateCompressors(240, 640, 8, NULL, 0);
	m_bUnderTest = false;
	m_pCommDrive = NULL;
	m_pCommLED = NULL;
	m_pAppOption = NULL;
	
	this->initCV();

	//enumerate camera
	BusManager busMgr;
	Error error = busMgr.GetNumOfCameras(&m_uiCameras);
	if (error != PGRERROR_OK)
	{
		//::AfxMessageBox(_T("Error in GetNumOfCameras()."));
	}

	if(m_uiCameras < 2)
		::AfxMessageBox(_T("Insufficent number of cameras."));

	for(int i=0; i<2; i++)
	{
        PGRGuid guid;
        error = busMgr.GetCameraFromIndex( i, &guid );
        if (error != PGRERROR_OK)
        {
            //::AfxMessageBox(_T("Error in GetCameraFromIndex()."));
        }

		
        // Connect to a camera
		m_Cameras[i].initCamera(&guid, i, &(m_eventGrab[i]));
		

	}


//	this->m_Cameras[0].initCamera(&(m_arContext[0]), 0);
	//this->m_Cameras[1].initCamera(&(m_arContext[1]), 1);

	::ZeroMemory(m_ACCes, sizeof(double)*64);

	m_hWndParent = NULL;	
	m_triggerMode = false;

}

CStereoCam::~CStereoCam(void)
{
	this->releaseCV();

}

bool CStereoCam::start()
{
	m_bThreadSuspend = false;

	if(!m_bGrabThreadOn)
	{
		//ť�� ����.
		this->m_QueEyeInfos.clear();

		//plot event�� �����Ѵ�.
		

		this->m_ulFrameNum = 0;
		
		
		

		

		//����
		if((m_Cameras[0].start() &&  m_Cameras[1].start()))
		{
			m_bGrabThreadOn = true;

			::Sleep(100);
			CWinThread *pThread = ::AfxBeginThread(grabThread, this, THREAD_PRIORITY_TIME_CRITICAL);
			this->m_hThread = pThread->m_hThread;

		}
		else
		{
			m_Cameras[0].stop();
			m_Cameras[1].stop();

			
	#ifdef PTAVI
			this->m_ptAviWriter.close();
	#else
//			m_aviWriter.close();
	#endif
			m_aviGen.ReleaseEngine();
			
			return false;
		}
	}

	

	return true;
}

bool CStereoCam::stop()
{
	this->pause(false);

	m_bGrabThreadOn = false;

	if(WaitForSingleObject( m_hThread, INFINITE) == WAIT_OBJECT_0)
	{
		//cameras�� thread���� ���� �ش�.
		//m_Cameras[0].stop();
		//m_Cameras[1].stop();
		


		/*::Sleep(200);
#ifdef PTAVI
			this->m_ptAviWriter.close();
#else
            m_aviWriter.close();
#endif*/
		m_aviGen.ReleaseEngine();
		
	}


	
	return true;
}


bool CStereoCam::pause(bool bPause)
{
	bool bRtn;
	if(m_Cameras[0].pause(bPause) && m_Cameras[1].pause(bPause))
		bRtn = true;
	else
		bRtn = false;

	return bRtn;	
}

//int nnn = 0;
UINT CStereoCam::grabThread(void* lParam)
{
	CStereoCam* pStereo = (CStereoCam*)lParam;
	
		
	while(pStereo->m_bGrabThreadOn==true)
	{
		
		
		//if((pStereo->m_Cameras[0].retrieveBuffer()) && pStereo->m_Cameras[1].retrieveBuffer())
		DWORD wait = ::WaitForMultipleObjects(2, pStereo->m_eventGrab, TRUE, 500);
		if(wait != WAIT_TIMEOUT)
		{
			int n = 0;
			pStereo->processImage();		
			//nnn++;
			//TRACE("............................%d \n", nnn);
		}
		else		//lock ����
		{
			::AfxMessageBox("lock frame failed");
			pStereo->m_bGrabThreadOn = false;
		}

	}

	pStereo->m_Cameras[0].stop();
	pStereo->m_Cameras[1].stop();

	//::AfxEndThread(0);
	return 0;
}


void CStereoCam::processImage(void)
{
	//suspend�� �ƹ��͵� ���� ���� return
	//CPTCamera.processImage���� unlock����� �ϴ°͸� �̰����� ���ش�.
	//try
	{
		bool bFlip = this->m_pAppOption->m_bSwitchCameras;
		
		// eye positioin�� ��´�.
		// �����ư��鼭 result image�� �����.
		

		
		if(m_Cameras[0].m_bIsNewOne && m_Cameras[1].m_bIsNewOne)
		{
			this->m_Cameras[0].processImage((m_ulFrameNum%4)==0);
			this->m_Cameras[1].processImage((m_ulFrameNum%4)==0);
			

			m_Cameras[0].getEyeInfo()->m_fCx = 320-m_Cameras[0].getEyeInfo()->m_fCx;
			m_Cameras[1].getEyeInfo()->m_fCx = 320-m_Cameras[1].getEyeInfo()->m_fCx;

			//����� backup�ϰ� ť�� �ִ´�.
			this->m_EyeInfos[0] = *(m_Cameras[0].getEyeInfo());
			this->m_EyeInfos[1] = *(m_Cameras[1].getEyeInfo());
			
			m_ulFrameNum++;

			//test ���̸� chart�� �׸� �����͸� que�� �ִ´�.
			if(m_bUnderTest)
			{
				int qcount = m_QueEyeInfos.getCount();
				double* pGyro = m_pCommDrive->getCurrentValue();
				if(m_pCommDrive)
				{
					
				}
				m_ulTestFrameNum++;

				double* pLEDData = this->m_pCommLED->getStiValue();
				//�����͸� �����Ѵ�.
				double data[15] = {m_EyeInfos[0].m_fCx, m_EyeInfos[1].m_fCx,
									m_EyeInfos[0].m_fCy, m_EyeInfos[1].m_fCy,
									m_EyeInfos[0].m_fTorsion, m_EyeInfos[1].m_fTorsion,
									m_EyeInfos[0].m_fRadius, m_EyeInfos[1].m_fRadius,
									pGyro[0], pGyro[1], pGyro[2], 0, 0,
									pLEDData[0], pLEDData[1]};
				
				this->m_euTestFile.addData((byte*)data, sizeof(double)*15);
				//missing�Ǿ��� �̹����� ������ ���� �����͸� �ѹ� �� �־��ش�.
				/*if((m_Cameras[1].m_nFrameMissed) )
				{
					int n = m_Cameras[1].m_nFrameMissed;
					for(int i=0; i<n; i++)
						m_euTestFile.addData((byte*)data, sizeof(double)*15);
				}*/


				//que�� eye�����͸� �ִ´�.
				//�ֱ� ���� calibration�Ѵ�.
				this->m_eyeCalibration[0].calibrate(&(m_EyeInfos[0]));
				this->m_eyeCalibration[1].calibrate(&(m_EyeInfos[1]));			

				this->m_QueEyeInfos.push(m_EyeInfos);
                //if((m_Cameras[0].m_bFrameMissed) || (m_Cameras[1].m_bFrameMissed))
				

				//analyze
				//analyze
				//if(IS_NYSTAGMUS(m_pCurrentTI->m_nTestParams[0]))
				//if(m_bAnalyze)
				//	this->m_analManager.putSignal(m_EyeInfos[0].m_fCx, m_EyeInfos[1].m_fCx);
				
				if(qcount > 3) //4������ �׸���.(30Hz)
				{
					SendMessage(this->m_hWndParent, WM_DRAW_CHART, 0, 0);
				}
				
				//float* pAcc = this->m_pCommDrive->GetRealDataArray();
				//SendMessage(this->m_hWndParent, WM_DRAW_CHART, (WPARAM)m_EyeInfos, (LPARAM)m_ACCes);



				
				


					//SetEvent(m_hPlotEvent);
			}
			
			
			

			//display�� �� �̹����� ��ģ��.
			if(m_ulFrameNum%4 == 0)
			{
				//cvRect rectLeft, rectRight;

				::cvSetImageROI(m_pStereoImage, cvRect(320, 0, 320,240));	//lEFT
				::cvCopy(m_Cameras[0].getResultImage(), m_pStereoImage);
				::cvResetImageROI(m_pStereoImage);
				::cvSetImageROI(m_pStereoImage, cvRect(0,0,320,240));		//RIGHT
				::cvCopy(m_Cameras[1].getResultImage(), m_pStereoImage);
				::cvResetImageROI(m_pStereoImage);
			}
			//image�� �׷��ش�.
			else if((m_ulFrameNum%4) == 1)
			{
				::WaitForSingleObject(m_pImageDisplay->m_eventDisplay, 30);
				this->m_pImageDisplay->SetData((unsigned char*)(m_pStereoImage->imageData));
			}
			//avi�� image�� ��ģ��.
			else if((m_ulFrameNum%4) == 2)
			{
				::cvSetImageROI(m_pStereoImageMono, cvRect(320, 0, 320,240));	//Left
				::cvCopy(m_Cameras[0].getOriginalImage(), m_pStereoImageMono);
				::cvResetImageROI(m_pStereoImageMono);
				::cvSetImageROI(m_pStereoImageMono, cvRect(0,0,320,240));
				::cvCopy(m_Cameras[1].getOriginalImage(), m_pStereoImageMono);
				::cvResetImageROI(m_pStereoImageMono);
				::cvMerge(m_pStereoImageMono, m_pStereoImageMono, m_pStereoImageMono,NULL, m_pStereoImage);
			}
			else if((m_ulFrameNum%4) == 3)		//�̹��� ����
			{
				
				//avi Writer�� image�� ������.
				//avi Writer�� �����Ǿ� ������ �����Ѵ�.
		#ifdef PTAVI
				this->m_ptAviWriter.appendFrame((unsigned char*)(m_Cameras[0].getOriginalImage()->imageData));
		#else
		//		this->m_aviWriter.addFrame(m_Cameras[0].getOriginalImage(), m_Cameras[1].getOriginalImage());
		#endif

				m_aviGen.AddFrame((BYTE*)(m_pStereoImage->imageData));


				
			}


			m_Cameras[0].m_nFrameMissed = 0;
			m_Cameras[1].m_nFrameMissed = 0;
		}
			



	}
//	catch(char* e)
	{
		//printf("error in try");
	}
	


	
}


void CStereoCam::initCV()
{
	if(!m_pStereoImage)
		m_pStereoImage = ::cvCreateImage(cvSize(IMAGE_WIDTH*2, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	if(!m_pStereoImageMono)
		m_pStereoImageMono = ::cvCreateImage(cvSize(IMAGE_WIDTH*2, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);

}
void CStereoCam::releaseCV()
{
	if(m_pStereoImage)
	{
		::cvReleaseImage(&m_pStereoImage);
		m_pStereoImage = NULL;
	}

	if(m_pStereoImageMono)
	{
		::cvReleaseImage(&m_pStereoImageMono);
		m_pStereoImage = NULL;
	}
}

void CStereoCam::setImageDisplay(CImageDisplay* pDisplay)
{
	this->m_pImageDisplay = pDisplay;
}

void CStereoCam::setParentHWnd(HWND hwnd)
{
	m_hWndParent = hwnd;
}

void CStereoCam::setSaveImage(bool bSave, CString fname)
{
	//save ����
	if(bSave)
	{
		/*CString strPath;
		corGetProgramFolder(&strPath);
		strPath = strPath + "\\" + AVI_FILENAME;*/

		//���� ������ �����ϸ� ����
		CFileStatus fs;
		if(CFile::GetStatus(fname, fs))
			CFile::Remove(fname);


		//pause(true);
		//avi writer�� �����.
#ifdef PTAVI
		this->m_ptAviWriter.open(fname, IMAGE_HEIGHT, IMAGE_WIDTH, 8, 30);
#else
//		m_aviWriter.open(strPath, cvSize(IMAGE_WIDTH*2, IMAGE_HEIGHT), 1);
#endif
		this->m_aviGen.SetFileName(fname);
		this->m_aviGen.SetRate(30);
		this->m_aviGen.SetBitmapHeader(&(m_pImageDisplay->m_pBitmapInfo->bmiHeader));
		HRESULT hr;
		hr=m_aviGen.InitEngine();
		if (FAILED(hr))
			AfxMessageBox(m_aviGen.GetLastErrorMessage());
			


		//pause(false);
	}
	//save ��
	else
	{

		pause(true);
	//	Sleep(100);

#ifdef PTAVI
		this->m_ptAviWriter.close();
#else
//         m_aviWriter.close();
#endif
 		m_aviGen.ReleaseEngine();

		 pause(false);
	}
}

ED_EyeInfo* CStereoCam::getEyeInfo()
{
	//�迭�� ù ���� ������
	return m_EyeInfos;
}

void CStereoCam::getFrameInfo(double* fr0, double* fr1, unsigned int* mfn0, unsigned int *mfn1)
{
	if(fr0)
		*fr0 = this->m_Cameras[0].getFramerate();
	if(mfn0)
        *mfn0 = this->m_Cameras[0].getMissingFrameNum();

	if(fr1)
        *fr1 = this->m_Cameras[1].getFramerate();
	if(mfn1)
        *mfn1 = this->m_Cameras[1].getMissingFrameNum();
}

unsigned long CStereoCam::getFrameNum()
{
	return this->m_ulFrameNum;
}
unsigned long CStereoCam::getTestFrameNum()
{
	return this->m_ulTestFrameNum;
}



void CStereoCam::resetTorsion(unsigned int camIdx)
{
	if(camIdx <= 1)
        m_Cameras[camIdx].resetTorsion();
}

void CStereoCam::setUnderTest(bool bUnderTest)
{
	if(bUnderTest)
	{
		this->m_bUnderTest = true;
		this->m_ulTestFrameNum = 0;
	}
	else
		m_bUnderTest = false;
}
void CStereoCam::setCommDrive(CCommDrive* pCommDrive, CCommLED* pCommLED)
{
	this->m_pCommDrive = pCommDrive;
	this->m_pCommLED = pCommLED;
}


void CStereoCam::setTriggerMode(bool on)
{
	m_Cameras[0].setTriggerMode(on);
	m_Cameras[1].setTriggerMode(on);
	m_triggerMode = on;
}