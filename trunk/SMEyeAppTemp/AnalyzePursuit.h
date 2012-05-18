#pragma once

struct structPursuit
{
	unsigned long startIdx;	//eye vel zero crossing 시작점
	unsigned long endIdx;	//eye vel zero crossing 끝점
	
	double peakVelEye;				// degree/s
	unsigned long peakVelEyeIdx;	// peak velocit의 위치
	double peakVelTarget;
	unsigned long peakVelTargetIdx;	
};


class CAnalyzePursuit
{
public:
	CAnalyzePursuit(void);
	~CAnalyzePursuit(void);

	double* m_pdEyeVel;
	double* m_pdTargetVel;
	unsigned long m_ulDataCount;
	CList<structPursuit*, structPursuit*> m_listPursuit;

	void analyze(double* pEye, double* pTarget, unsigned long count);
	bool findPursuitParam(structPursuit* pPursuit);
	CString getResultString();

	unsigned int getCount();
		
	void getGain(double& minVal, double& maxVal, double& meanVal);
	
	bool load(CString fname);		//분석 결과 load
	bool save(CString fname);		//분석 결과 save

	unsigned long getClosestPursuit(double t);

	unsigned long addPursuit(unsigned long idx, double* pEye, unsigned long count);
    static void reshapeTarget(double* pTarget, unsigned long count, double amp);
protected:
	void deleteAllPursuit();


	
};
