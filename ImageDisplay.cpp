#include "StdAfx.h"
#include "ImageDisplay.h"
#include ".\imagedisplay.h"
#include "corwlDefines.h"


CImageDisplay::CImageDisplay(void)
: m_pImageData(NULL)
, m_pBitmapInfo(NULL)
{
	New();
	m_bDiplayed = true;

	m_eventDisplay = CreateEvent(NULL, FALSE, FALSE, "");	//NONSIGANAL, MANUAL
	
}

CImageDisplay::~CImageDisplay(void)
{
	Delete();
	CloseHandle(m_eventDisplay);
}

void CImageDisplay::New(void)
{
	//made for 640*240 and 24bit image
	int width = IMAGE_WIDTH*2;
	int height = IMAGE_HEIGHT;
	m_pImageData = new unsigned char[width*height*4];
	
	m_pBitmapInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256];
	
	m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biWidth = width;
	m_pBitmapInfo->bmiHeader.biHeight = height;
	m_pBitmapInfo->bmiHeader.biPlanes = 1;
	m_pBitmapInfo->bmiHeader.biBitCount = 24;
	m_pBitmapInfo->bmiHeader.biCompression = 0;
	m_pBitmapInfo->bmiHeader.biSizeImage = width*height;
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBitmapInfo->bmiHeader.biClrUsed = 256;
	m_pBitmapInfo->bmiHeader.biClrImportant = 0;

	for(int i=0;i<256; i++)
	{
		m_pBitmapInfo->bmiColors[i].rgbRed = i;
		m_pBitmapInfo->bmiColors[i].rgbGreen = i;
		m_pBitmapInfo->bmiColors[i].rgbBlue = i;
	}

	ZeroMemory(m_pImageData, width*height*4);
	

}

void CImageDisplay::Delete(void)
{
	if(m_pImageData)
	{
		delete []m_pImageData;
		m_pImageData = NULL;
	}
	
	if(m_pBitmapInfo)
	{
		delete []m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
}
BEGIN_MESSAGE_MAP(CImageDisplay, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CImageDisplay::OnPaint()
{
	
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages
	
	//nonsignaled시킨다.
	ResetEvent(this->m_eventDisplay);

	dc.SetWindowExt(320, 240);
	dc.SetViewportExt(320, -240);
	

	if(this->GetSafeHwnd())
	{
		
		::SetDIBitsToDevice(dc.GetSafeHdc(),1,1,IMAGE_WIDTH*2,IMAGE_HEIGHT,0,0,0,(WORD)IMAGE_HEIGHT,
			m_pImageData,m_pBitmapInfo,DIB_RGB_COLORS);

		
		this->m_bDiplayed = true;
		
		
	}
	
	//signaled시킨다.
	SetEvent(this->m_eventDisplay);
}


void CImageDisplay::SetData(unsigned char* pImage)
{
	

	if(!m_pImageData)
		this->New();

		
	if(this->m_bDiplayed)
	{
		if(!pImage)
			::ZeroMemory(m_pImageData, IMAGE_WIDTH*2*IMAGE_HEIGHT*3);
		else
			memcpy(m_pImageData,pImage,IMAGE_WIDTH*2*IMAGE_HEIGHT*3);

		this->m_bDiplayed = false;
		if(this->GetSafeHwnd())
		{
			Invalidate();
			
		}
		
	}	
}


