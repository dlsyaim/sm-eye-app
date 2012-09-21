#pragma once

struct structSaccade
{
	unsigned long startIdx;	//target 시작점
	unsigned long endIdx;	//saccade 끝점
	double peakVel;		//degree/s
	double accuracy;	//%
	unsigned int latency;		//idx 개수
	double targetLeap;	//degree
};

double getMaxAbsDiff(double* pDat, unsigned long startIdx, unsigned long endIdx, unsigned long* pMaxIdx, unsigned char M);

class CAnalyzeSaccade
{
public:
	CAnalyzeSaccade(void);
	~CAnalyzeSaccade(void);

	void analyze(double* pEye, double* pTarget, unsigned long count);
	CString getResultString();

	unsigned int getCount();
		
	void getPeakVelocity(double& minVal, double& maxVal, double& meanVal);
	void getLatency(double& minVal, double& maxVal, double& meanVal);
	void getAccuracy(double& minVal, double& maxVal, double& meanVal);

	bool load(CString fname);		//분석 결과 load
	bool save(CString fname);		//분석 결과 save

	unsigned long addSaccade(unsigned long idx, double* pEye, double* pTarget, unsigned long count);

	CList<structSaccade*, structSaccade*> m_listSaccade;

	unsigned long getClosestSaccade(double t);
protected:
	void deleteAllSaccade();
};
