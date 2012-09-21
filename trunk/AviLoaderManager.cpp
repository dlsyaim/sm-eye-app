#include "StdAfx.h"
#include ".\aviloadermanager.h"
#include "corwlDefines.h"
#include ".\tchart\toollist.h"
#include ".\tchart\colorlinetool.h"
#include ".\tchart\tools.h"
#include ".\tchart\axes.h"
#include ".\tchart\axis.h"
#include ".\tchart\series.h"
#include ".\tchart\valuelist.h"



CAviLoaderManager::CAviLoaderManager(void)
{
	m_pAviLoader = NULL;
	m_ChartCtrl = NULL;
	m_pImageDisplay = NULL;
	this->m_ulPos = -1;
	m_bAviExist = false;

	m_pStereoImage = NULL;
}

CAviLoaderManager::~CAviLoaderManager(void)
{
	if(m_pAviLoader)
	{
		m_pAviLoader->close();
		delete m_pAviLoader;
	}

	if(m_pStereoImage)
	{
		::cvReleaseImage(&m_pStereoImage);
		m_pStereoImage = NULL;
	}
}

void CAviLoaderManager::setData(CTChart* pTChart, CImageDisplay* pImageDisplay)
{
	this->m_ChartCtrl = pTChart;
	this->m_pImageDisplay = pImageDisplay;
}
bool CAviLoaderManager::open(CString strAviFname, CString strDataFname)
{
	m_strAviFname = strAviFname;
	m_strDataFname = strDataFname;
	
	//aviloader가 있으면 지운다.
	if(m_pAviLoader)
	{
		m_pAviLoader->close();
		delete m_pAviLoader;
	}
	m_pAviLoader = new CAviLoader;
	this->m_bAviExist = m_pAviLoader->open(m_strAviFname);
	this->m_ulPos = -1;

	//iplimage
	if(!m_pStereoImage)
		m_pStereoImage = ::cvCreateImage(cvSize(IMAGE_WIDTH*2, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
	
	if(m_bAviExist)
	{
		
		
		

		this->next();	//첫번째 image 인덱스 0
		for(int i=0; i<CHART_NUM; i++)
		{
			m_ChartCtrl[i].GetTools().GetItems(0).SetActive(true);
			m_ChartCtrl[i].GetTools().GetItems(0).GetAsColorLine().SetValue(0);

		}
	}
	else
	{
		delete m_pAviLoader;
		m_pAviLoader = NULL;
	}

	return m_bAviExist;;
}
void CAviLoaderManager::close()
{
	m_strAviFname = "";
	
	//this->m_testFile.close();

    if(m_pAviLoader)
	{
		m_pAviLoader->close();
		delete m_pAviLoader;
		m_pAviLoader = NULL;

		if(m_pImageDisplay && (m_pImageDisplay->GetSafeHwnd()))
			m_pImageDisplay->SetData(NULL);

		if(m_pStereoImage)
		{
			::cvReleaseImage(&m_pStereoImage);
			m_pStereoImage = NULL;
		}
	}

	
	for(int i=0; i<CHART_NUM; i++)
	{
		if(m_ChartCtrl[i].GetSafeHwnd())
		{
			m_ChartCtrl[i].GetTools().GetItems(0).GetAsColorLine().SetValue(0);
			m_ChartCtrl[i].GetTools().GetItems(0).SetActive(false);
		}
	}

	

	m_bAviExist = false;
	m_ulPos = -1;
}

unsigned long CAviLoaderManager::next()
{
	if(!m_pAviLoader)
		return -1;

	m_ulPos++;

	if(m_pAviLoader->grabFrame())
	{
		if(m_pImageDisplay && (m_pImageDisplay->GetSafeHwnd()))
		{
			memcpy(m_pStereoImage->imageData, m_pAviLoader->retrieveFrame(), 320*240*3*2);
			//BcvCircle(m_pStereoImage, cvPoint(160, 120), 10, cvScalar(255, 0,0,0));

			double buf[15];
			::ZeroMemory(buf, sizeof(double)*15);
			EU_TestFileV2 file;
			if(file.open(m_strDataFname, NULL, false))
			{
				file.seekToData();
				file.m_file.Seek(sizeof(double)*15*4*(m_ulPos-2) , CFile::current);
				file.getNextData(buf);
				file.close();
			}
			
			
			//draw cross line
			//Left eye
			//cvLine(m_pStereoImage, cvPoint(int((buf[0])+320), 0), cvPoint(int((buf[0])+320), 240), cvScalar(128,128,255, 0));
			//cvLine(m_pStereoImage, cvPoint(320, int(buf[2])), cvPoint(640, int(buf[2])), cvScalar(128,128,255, 0));

			//Right eye
			//cvLine(m_pStereoImage, cvPoint(int(buf[1]), 0), cvPoint(int(buf[1]), 240), cvScalar(128,128,255, 0));
			//cvLine(m_pStereoImage, cvPoint(0, int(buf[3])), cvPoint(320, int(buf[3])), cvScalar(128,128,255, 0));





			m_pImageDisplay->SetData((unsigned char*)(m_pStereoImage->imageData));
		}
		

		double t = double(m_ulPos)/double(m_pAviLoader->fps);

		//bottom axis 범위를 넘어가면 옮긴다.
		double M = this->m_ChartCtrl[0].GetAxis().GetBottom().GetMaximum();
		if(M <= t)
		{
            for(int i=0; i<CHART_NUM; i++)
				m_ChartCtrl[i].GetAxis().GetBottom().SetMinMax(M, M+20);
		}
		else if(m_ulPos == 0)
		{
			for(int i=0; i<CHART_NUM; i++)
				m_ChartCtrl[i].GetAxis().GetBottom().SetMinMax(0, 20);
		}

		//color line을 옮긴다.
		for(int i=0; i<CHART_NUM; i++)
			m_ChartCtrl[i].GetTools().GetItems(0).GetAsColorLine().SetValue(t);


		
	}
	else
	{
		this->setPos(0);
	}

	

	return m_ulPos;
}
void CAviLoaderManager::setPos(unsigned long pos)
{
	if(this->m_pAviLoader)
	{
		m_pAviLoader->setProperty(CV_CAP_PROP_POS_FRAMES1, pos);
		
		this->m_ulPos = pos-1;
		this->next();
		//color line을 옮긴다.
		for(int i=0; i<CHART_NUM; i++)
			m_ChartCtrl[i].GetTools().GetItems(0).GetAsColorLine().SetValue(double(m_ulPos)/double(m_pAviLoader->fps));
	}

}
unsigned long CAviLoaderManager::getPos()
{
	return m_ulPos;
}

