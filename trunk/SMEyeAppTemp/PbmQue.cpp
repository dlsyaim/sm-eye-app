#include "StdAfx.h"
#include ".\pbmque.h"
#include "corwlUtil.h"

CpbmQue::CpbmQue(void)
{
	init();
}

CpbmQue::~CpbmQue(void)
{
	
}


void CpbmQue::init()
{
	memset(m_currentData, 0, sizeof(double)*6);
}

int CpbmQue::addRawData(byte* pData, unsigned int count)
{
	//protocole
	//https://docs.google.com/spreadsheet/ccc?key=0ArVfD1a509hxdEVhZ0szZTFlamJqLXpub09tZGRZaWc&hl=en_US#gid=0
	int num = 0;

	unsigned char ch = 0;
	unsigned short val = 0;
	for(unsigned int i=0; i<count-1; i++)
	{
		// 00xxxxxx, 01xxxxxx, 10xxxxxx ���� Ȯ���Ѵ�.
		if(( (pData[i]>>6) == 0)			//00xxxxxx
			&& ((pData[i+1]>>6) == 1)			//01xxxxxx
			&& ((pData[i+2]>>6) == 2)		//10xxxxxx
			&& ((pData[i+3]>>6) == 3)	)	//11xxxxxx
		{
			ch = (pData[i] & 0x30) >> 4;	//xCCCxxxxx ���� ä���� ����.
			if(ch < 3)
			{
				unsigned short val;
				unsigned char lo, hi;
				lo = (pData[i]&0x0f)<<4;
				lo = ((pData[i]&0x0f)<<4)  + (pData[i+1]&0x0f);
				hi = ((pData[i+2]&0x0f)<<4)  + (pData[i+3]&0x0f);
				val = hi;
				val = (val<<8)+lo;


				//500 dps �� ��� sensitivity: 0.0175 dps/digit
				this->m_currentData[ch] = get2sComplement(val)*0.0175;
				
				i++;	//�ι���Ʈ ������.(���� i++�� �ѹ� �����Ƿ� �ѹ��� ���ش�.)
			}
		}
	}


	return num;
}





