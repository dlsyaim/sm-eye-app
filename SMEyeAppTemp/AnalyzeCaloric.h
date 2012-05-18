#pragma once

#define LW 0
#define RC 1
#define LC 2
#define RW 3



#include "AnalyzeNystagmus2.h"

class CCaloricData
{
public:
	CCaloricData(void);
	~CCaloricData(void);

	

	CString m_strTestFname;				// 분석 test file name
	CAnalyzeNystagmus2 m_analyzeNys;

	double m_peakRegion[2];				//peak값이 위치한 window 위치
	double m_peakValue;					//peak value. manual peak select가 0 혹은 1이면 load한 값을 사용하지 말고 region 내에서 계산해 줘야 한다. 2이면 그냥 사용한다.

	bool m_bUseFixationBlock;			//fixation 구간을 설정할지
	double m_fixationBlock[2];			//fixation 구간
	double m_fixationIdx;				// fixation index 값

	int m_nManualPeakSelect;
	

	void makeSaveFormat(unsigned char* pBuf, int count);
	void readSaveFormat(unsigned char* pBuf, int count);

	void calcPeakRegion(int test, double spon);
	void calcFixationIndex(int test, double spon);

	
};

class CAnalyzeCaloric
{
public:
	CAnalyzeCaloric(void);
	~CAnalyzeCaloric(void);

	void setTestFileName(CString fnames[4]);

	void analyze();


	bool load(CString fname);
	bool save(CString fname);

	double getPeakVlue(int test);
	void setPeakValue(int test, double peakVal);

	int m_nEyeSelect;			//left eye or right eye
	
	bool m_bCompensateSpontaneous;		//자발 안진을 보상할 것인지
	CString m_strSpontaneousFileName;
	double m_dSpontaneousValue[2];			// 보상 값. 실제 값에서 이 값을 빼줘야 함. left, right eye의 두 값을 지니고 있는다.
	bool m_bSpontaneousFromUser;		//보상값을 테스트로부터 얻지 않고 유저가 직접 입력한 값을 사용

	//int m_nManualPeakSelect;

	CCaloricData m_caloricData[4];

	double m_caloricVestibularParesis;
	double m_caloricDirectionalpreponderance;
	double m_caloricTemperatureEffect;
private:

};

