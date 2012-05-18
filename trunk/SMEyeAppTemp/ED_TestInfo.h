#pragma once

#include "calibration.h"


#define MAX_FILE_NAME	256
#define TEST_PARAM_NUM	32
#define TI_SIZE			1024		//���Ͽ� ������ ED_TestInfo�� ������

//PARAM_ID
#define	TEST_CLASS		0			//test�� ����. 0�� ����Ʈ�� ����ִ�.
//#define TEST_CLASS_PARAM 1			//test class�� �ΰ��Ͽ� �ʿ��� ����
//#define TEST_DIRECTION	2			//0�̸� HORIZONTAL, 1�� VERTICAL, 2�� ���



//horizontal, vertical
/*#define TP_HV_HORIZONTAL	1
#define TP_HV_VERTICAL		2

//chart visible
#define	TP_CHART_HORIZONTAL	1
#define TP_CHART_VERTICAL	2
#define TP_CHART_TORSION	4
#define TP_CHART_RADIUS		8
#define TP_CHART_ACC		16
#define TP_CHART_ZYRO		32
#define TP_CHART_VELOCITY	64
#define TP_CHART_ANALYSIS	128*/

//calibrated
#define TP_CALIBRATED		1


// Header file size
#define TEST_INFO_SIZE			TI_SIZE
#define OFFSET_CALIBRATION		768



class ED_TestInfo
{
public:
	ED_TestInfo(void);
	~ED_TestInfo(void);


	int getTestParams(unsigned char PARAM_ID);

	//
	unsigned int getVisibleChartNum();

	int m_nVer; 
    BOOL m_bBinocular;		//monocular(FALSE), binocular(TRUE)
	unsigned long m_lEyeDataCount;
	int m_nEyeFreq;
	unsigned long m_lAccDataCount;
	int m_nAccFreq;
	int m_nTestParams[TEST_PARAM_NUM];		// not used
	char m_chFileName[MAX_FILE_NAME];
	CTime m_timeTime;

	unsigned int m_calibrated;	//		calibrated(1): degree ����, unclibrated(0): pixel ����
	
	
	unsigned int m_HV_NOT_USED;			//		not used
	
	//�� 5���� �߿�.
	unsigned int m_showChart;	//bit(0): h, bit(1): v, bit(2):t, bit(3) r, bit(4): acc, bit(5), zyro, bit(6), vel, bit(7) analysis
    unsigned char m_analMethod;	//bit0: NYSTAGMUS bit1:HEAD_TRUST*/
	unsigned char m_analTarget;

	unsigned char m_stiModeCommand;
	unsigned char m_stiSubmode;
	

	CCalibration m_calibration[2];

	unsigned char m_flipImage;

	
	unsigned char m_ucPrint;
	//�� ��������� �ݵ�� ���� �ؿ� ������� �����ش�.
	//��ü Ŭ���� ������� 1024�� ���� ���ƾ� �Ѵ�.

	

	

};
