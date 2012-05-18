#include "StdAfx.h"
#include ".\1298que.h"
#include "corwlDefines.h"

C1298Que::C1298Que(void)
{
	init();
}

C1298Que::~C1298Que(void)
{
}

void C1298Que::init()
{
	m_1298List.RemoveAll();
	memset(m_currentEOGValue, 0, sizeof(double)*4);
}

int C1298Que::addRawData(byte* pData, unsigned int count)
{
	if(count<12)
		return 0;

	int num = 0;

	long temp = 0;
	C1298Data data;

	memset(m_currentEOGValue, 0, sizeof(double)*4);

    for(int i=0; i<count/12; i++)
	{
		for(int j=0; j<4; j++)
		{
			temp = (pData[i*12 + j*3]<<16) + (pData[i*12 + j*3 +1]<<8) + pData[i*12 + j*3 +2];
						
			
			if(temp > 0x7fffff)
				temp = -1*(0xffffff - temp)-1 ;

			m_currentEOGValue[j] += temp;
			
			//data.ecg[j] = temp;
		}

		//this->add(data);
		num++;
	}

	for(int i=0; i<4; i++)
	{
		m_currentEOGValue[i] /= num;
		m_currentEOGValue[i] = m_currentEOGValue[i]*EOG_GAIN; 
	}

	TRACE("EOG Value is %.2f, %.2f %.2f %.2f \r\n", m_currentEOGValue[0], m_currentEOGValue[1], m_currentEOGValue[2], m_currentEOGValue[3]);

	/*unsigned char buf[3];
    for(int i=0; i<count-3; i++)
	{
		if((pData[4*20] != 0x04) | (pData[4*20+1] !=  0x64))
			int k = 0;

		if(((pData[i]&0xc0) == 0x00) &&
			((pData[i+1] & 0xc0) == 0x40) && 
			((pData[i+2] & 0xc0) == 0x80) && 
			((pData[i+3] & 0xc0) == 0xc0))
		{
			buf[0] = ((pData[i] & 0x3f) << 2) | ((pData[i+1] & 0x30)>>4);
			buf[1] = ((pData[i+1] & 0x0f) << 4) | ((pData[i+2] & 0x3c)>>2);
			buf[2] = ((pData[i+2] & 0x03)<<6) | (pData[i+3] & 0x3f);

			//temp = (buf[0]<<16) + (buf[1]<<8) + buf[2];
			temp = (buf[2]<<16) + (buf[1]<<8) + buf[0];

			
			
				

			i += 4;
		}
		

		//TRACE("temp is %d\r\n", temp);

		if(temp > 0x7fffff)
			temp = -1*(0xffffff - temp) -1;

		double ecg = temp;




		C1298Data data;
		data.ecg = temp;

		this->add(data);
		num++;
	}
*/
	

	return num;
}