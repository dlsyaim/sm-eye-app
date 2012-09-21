#pragma once
#include "corwlDefines.h"
#include "ed_eyeinfo.h"

class CCalibration
{
public:
	

	CCalibration(void);
	~CCalibration(void);

	bool load(CString fname);
	bool save(CString fname);

	
	void calibrate(ED_EyeInfo* pEI);
	void uncalibrate(ED_EyeInfo* pEI);
	
	void pixel2degree(double* pPixel, double* pDegree);
	double pixel2mm(double rPixel);	

	bool analyze(double* pEyeH, double* pEyeV, double* pStiH, double* pStiV, unsigned long count);
	bool analyzeCustom(double* pEyeH, double* pEyeV, unsigned long eyeCount, unsigned long* pSeperate, unsigned char numSeperate, 
						double* pTargetH, double* pTargetV, unsigned char targetCount);
	
	bool analyzeUsingGyro(double* pEyeH, double* pEyeV, unsigned long eyeCount, double* pGyroH, double* pGyroV, unsigned long gyroCount);

	CString getModelStr();

	CCalibration& operator=(const CCalibration &rhs);


	bool m_bCalibrated;
	bool analyze5x5(double* pX, double* pY, double* pDegreeH, double* pDegreeV);
	bool analyze3x3(double* pX, double* pY, double* pDegreeH, double* pDegreeV);
	double m_dHCoeff[6];
	double m_dVCoeff[6];
	double m_dRCoeff;

	//bool m_bCustom;
	//double m_ulSeperateTime[5];	//custom일 때만 사용. Target의 구간 사이 위치
	//double m_dCustomLRTB[2];	//[0] : LR degree, [1] : Top-Bottom degree

	

	void degree2pixel(double* pDegree, double* pPixel);
	double mm2pixel(double rMM);

#ifdef FOR_ASAN
	void setCalibrationUsingGyro(bool bGyro) {m_bCalibrationUsingGyro = bGyro;};

protected:
	bool m_bCalibrationUsingGyro;
#endif
};
