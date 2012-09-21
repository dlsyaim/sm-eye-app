#pragma once

#define HEAD_THRUST_POSITIVE 0
#define HEAD_THRUST_NEGATIVE 1


// head와 eye의 samplerate이 다르기 때문에 두개의 index를 모두 갖고 있는다.
// 다른 값이지만 framerate로 나누면 동일한(혹은 근접한 값을 같는다.)
struct structHeadThrust
{
	unsigned long eyeOnsetIdx;
	unsigned long headOnsetIdx;	//head onset �ġ 
	double headPeakVel;		//degree/s
	unsigned long headPeakIdx;
	double eyePeakVel;	//%
	unsigned long eyePeakIdx;
	bool valid;
};



class CAnalyzeHeadThrust
{
public:
	CAnalyzeHeadThrust(void);
	~CAnalyzeHeadThrust(void);

	CList<structHeadThrust*, structHeadThrust*> m_listHeadThrust;

	double* m_ppMeanEyeVel[2];	//positive와 negative
	double* m_ppMeanHeadVel[2];	//positive와 negative
	structHeadThrust m_structMeanHT[2];	//mean 파형의 분석값

	// max head speed가 넘을 경우 discard 시킬 것인지
	bool m_bUseMaxHeadSpeedLimit;
	double m_dMaxHeadSpeedLimit;
	double m_dMinHeadSpeedLimit;



public:
	void analyze(double* pEyeVel, unsigned long eyeCount, double* pTarget, unsigned long targetCount);
	void calculateMeanVelocity();
	
	bool load(CString fname);		//분석 결과 load
	bool save(CString fname);		//분석 결과 save

	int getValidHTCount(int sign);
	int getTotalHTCount() { return (int)m_listHeadThrust.GetCount();};
	int findHT(double headPeakVel, double eyePeakVel);
	bool deleteHT(int idx);

private:
	
	
	double* m_pdEyeVel;
	double* m_pdHeadVel;
	unsigned long m_ulEyeDataCount;
	unsigned long m_ulHeadDataCount;

	//void deleteAllHeadThrust();
	
	void deleteAllHTs();
};
