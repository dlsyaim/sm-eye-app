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

	

	CString m_strTestFname;				// �м� test file name
	CAnalyzeNystagmus2 m_analyzeNys;

	double m_peakRegion[2];				//peak���� ��ġ�� window ��ġ
	double m_peakValue;					//peak value. manual peak select�� 0 Ȥ�� 1�̸� load�� ���� ������� ���� region ������ ����� ��� �Ѵ�. 2�̸� �׳� ����Ѵ�.

	bool m_bUseFixationBlock;			//fixation ������ ��������
	double m_fixationBlock[2];			//fixation ����
	double m_fixationIdx;				// fixation index ��

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
	
	bool m_bCompensateSpontaneous;		//�ڹ� ������ ������ ������
	CString m_strSpontaneousFileName;
	double m_dSpontaneousValue[2];			// ���� ��. ���� ������ �� ���� ����� ��. left, right eye�� �� ���� ���ϰ� �ִ´�.
	bool m_bSpontaneousFromUser;		//������ �׽�Ʈ�κ��� ���� �ʰ� ������ ���� �Է��� ���� ���

	//int m_nManualPeakSelect;

	CCaloricData m_caloricData[4];

	double m_caloricVestibularParesis;
	double m_caloricDirectionalpreponderance;
	double m_caloricTemperatureEffect;
private:

};

