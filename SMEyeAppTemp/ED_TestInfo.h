#pragma once

#include "calibration.h"


#define MAX_FILE_NAME	256
#define TEST_PARAM_NUM	32
#define TI_SIZE			1024		//파일에 저장할 ED_TestInfo의 싸이즈

//PARAM_ID
#define	TEST_CLASS		0			//test의 종류. 0번 바이트에 들어있다.
//#define TEST_CLASS_PARAM 1			//test class에 부가하여 필요한 값들
//#define TEST_DIRECTION	2			//0이면 HORIZONTAL, 1면 VERTICAL, 2면 모두



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

	unsigned int m_calibrated;	//		calibrated(1): degree 단위, unclibrated(0): pixel 단위
	
	
	unsigned int m_HV_NOT_USED;			//		not used
	
	//이 5개가 중요.
	unsigned int m_showChart;	//bit(0): h, bit(1): v, bit(2):t, bit(3) r, bit(4): acc, bit(5), zyro, bit(6), vel, bit(7) analysis
    unsigned char m_analMethod;	//bit0: NYSTAGMUS bit1:HEAD_TRUST*/
	unsigned char m_analTarget;

	unsigned char m_stiModeCommand;
	unsigned char m_stiSubmode;
	

	CCalibration m_calibration[2];

	unsigned char m_flipImage;

	
	unsigned char m_ucPrint;
	//새 멤버변수는 반드시 가장 밑에 순서대로 적어준다.
	//전체 클래스 사이즈는 1024를 넘지 말아야 한다.

	

	

};
