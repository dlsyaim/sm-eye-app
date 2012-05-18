#include "StdAfx.h"
#include ".\calibration.h"
#include "math.h"
#include <algorithm>
#include "corwlutil.h"

#define SIZE_CALIBRATION_FILE 1024


CCalibration::CCalibration(void)
{
/*	m_bCustom = false;
	memset(m_ulSeperateTime, 0, sizeof(double)*5);
	m_dCustomLRTB[0]  = m_dCustomLRTB[1] = 10;
*/
	m_bCalibrated = false;
	::ZeroMemory(m_dHCoeff, sizeof(double)*6);
	::ZeroMemory(m_dVCoeff, sizeof(double)*6);

	m_dHCoeff[0] = -80;
	m_dHCoeff[1] = -0.5;		//x

	m_dVCoeff[0] = -60;
	m_dVCoeff[2] = 0.5;		//y
	
	this->m_dRCoeff = 0.2;

#ifdef FOR_ASAN
	m_bCalibrationUsingGyro = false;
#endif
}

CCalibration::~CCalibration(void)
{
	
}


CCalibration& CCalibration::operator=(const CCalibration &rhs)
{
	if(this != &rhs)
	{
		m_bCalibrated  = rhs.m_bCalibrated;
		memcpy(m_dHCoeff, rhs.m_dHCoeff, sizeof(double)*6);
		memcpy(m_dVCoeff, rhs.m_dVCoeff, sizeof(double)*6);
		m_dRCoeff = rhs.m_dRCoeff;		
	}

	return *this;
}


//eye position calibration
void CCalibration::pixel2degree(double* pP, double* pD)
{
	//pP(pixel), pD(degree)는 double[2];
	// 각각 pupil center의 x,y 좌표를 나타낸다.
	// cal = a + bx + cy + dx^2 + ey^2 + fxy;

	double x = pP[0];
	double y = pP[1];

	double x2 = x*x;
	double y2 = y*y;
	
	double x3 = x2*x;
	double y3 = y2*y;
	

	// horizontal
	pD[0] = m_dHCoeff[0] + 
			m_dHCoeff[1]*x + m_dHCoeff[2]*y +
            m_dHCoeff[3]*x2 + m_dHCoeff[4]*y2 + 
			m_dHCoeff[5]*x*y;

	// vertical
	pD[1] = m_dVCoeff[0] + 
			m_dVCoeff[1]*x + m_dVCoeff[2]*y +
            m_dVCoeff[3]*x2 + m_dVCoeff[4]*y2 + 
			m_dVCoeff[5]*x*y;
	
}

void CCalibration::calibrate(ED_EyeInfo* pEI)
{
	double pixel[2] = {pEI->m_fCx, pEI->m_fCy};
	double degree[2];
	this->pixel2degree(pixel, degree);
	pEI->m_fCx = (float)degree[0];
	pEI->m_fCy = (float)degree[1];

	pEI->m_fRadius = (float)pixel2mm(pEI->m_fRadius);
}

void CCalibration::uncalibrate(ED_EyeInfo* pEI)
{
	double degree[2] = {pEI->m_fCx, pEI->m_fCy};
	double pixel[2];
	this->degree2pixel(degree, pixel);
	pEI->m_fCx = (float)pixel[0];
	pEI->m_fCy = (float)pixel[1];

	pEI->m_fRadius = (float)mm2pixel(pEI->m_fRadius);
}

unsigned long getValidCount(double* pData, unsigned long startIdx, char cond, unsigned long count);
double getMedian(double* pData, unsigned long startIdx, unsigned long count);
double findValueAt(double* pData, double Value, double* pDataTarget, unsigned long startIdx, unsigned long count);
bool CCalibration::analyze(double* pEyeH, double* pEyeV, double* pStiH, double* pStiV, unsigned long count)
{
	//데이터의 대표값을 저장할 어레이

//	this->m_bCustom = false;

	if(count<=120)
		return false;

	double x[6], y[6], stiX[6], stiY[6];

	CArray<double> tempArray;
    

	int idx;
	unsigned long num;
	// stimulation 값과 그때의 eye position(pixel 대표값을 찾는다.
	////////////////////////////////////////////////////////////
	//Horizontal
	//get (0,0) count
	idx = 120;		//1초 후
	stiX[0] = pStiH[idx];	//0
	stiY[0] = pStiV[idx];	//0
	//sti가 0인 개수를 찾는다.
	num = getValidCount(pStiH, idx, 0, count);
	if(idx+num+1>=count)	//끝은 아닌지 확인
		return false;
	// 그 구간의 eye의 median을 구한다.
	x[0] = getMedian(pEyeH, idx, num);
	y[0] = findValueAt(pEyeH, x[0], pEyeV, idx, num);


	//get (10, 0)
	idx = idx+num+120;	//1초 후부터
	stiX[1] = pStiH[idx];	//0
	stiY[1] = pStiV[idx];	//0
	num = getValidCount(pStiH, idx, 1, count);
	if(idx+num+1>=count)	//끝은 아닌지 확인
		return false;
	x[1] = getMedian(pEyeH, idx, num);
	y[1] = findValueAt(pEyeH, x[1], pEyeV, idx, num);

	//get(-10,0)
	idx = idx+num+120;	//1초 후부터
	stiX[2] = pStiH[idx];	//0
	stiY[2] = pStiV[idx];	//0
	num = getValidCount(pStiH, idx, -1, count);
	if(idx+num+1>=count)	//끝은 아닌지 확인
		return false;
	x[2] = getMedian(pEyeH, idx, num);
	y[2] = findValueAt(pEyeH, x[2], pEyeV, idx, num);

	////////////////////////////////////////////////////////////
	//Vertical
	//get (0,-10) count
	idx = idx+num+120;	//1초 후부터
	stiX[3] = pStiH[idx];	//0
	stiY[3] = pStiV[idx];	//0
	//sti가 positive인 개수를 찾는다.
	num = getValidCount(pStiV, idx, 1, count);
	if(idx+num+1>=count)	//끝은 아닌지 확인
		return false;
	// 그 구간의 eye의 median을 구한다.
	x[3] = getMedian(pEyeH, idx, num);
	y[3] = findValueAt(pEyeH, x[3], pEyeV, idx, num);


	//get (10, 0)
	idx = idx+num+120;	//1초 후부터
	stiX[4] = pStiH[idx];	//0
	stiY[4] = pStiV[idx];	//0
	num = getValidCount(pStiV, idx, -1, count);
	if(idx+num+1>=count)	//끝은 아닌지 확인
		return false;
	x[4] = getMedian(pEyeH, idx, num);
	y[4] = findValueAt(pEyeH, x[4], pEyeV, idx, num);

	//get(-10,0)
	idx = idx+num+120;	//1초 후부터
	stiX[5] = pStiH[idx];	//0
	stiY[5] = pStiV[idx];	//0
	num = getValidCount(pStiV, idx, 0, count);
	
	x[5] = getMedian(pEyeH, idx, num);
	y[5] = findValueAt(pEyeH, x[5], pEyeV, idx, num);



	////////////////////////////////////////////////////////////
	// coefficient를 찾는다.
	m_bCalibrated = false;
	if (analyze3x3(x, y, stiX, stiY))
		m_bCalibrated = true;

	return m_bCalibrated;

	
}

bool CCalibration::analyzeCustom(double* pEyeH, double* pEyeV, unsigned long eyeCount, unsigned long* pSeperate, unsigned char numSeperate, 
						double* pTargetH, double* pTargetV, unsigned char targetCount)
{
	// pEyeH, pEyeV : eye position data
	// eyeCount : eye position data count
	// pSeperate : target 구간이 나뉘어지는 지점 (CLRTBC 일 경우 [5])
	// numSeperate : 5
	// pTargetH, pTargetV : target의 position
	// targetCount : CLRTBC 이므로 항상 6

//	m_bCustom = true;

	//데이터의 대표값을 저장할 어레이

	if(eyeCount<=120)
		return false;
	if(numSeperate != 5)
		return false;
	

	double x[6], y[6], stiX[6], stiY[6];

	CArray<double> tempArray;
    

	int idx;
	// stimulation 값과 그때의 eye position(pixel 대표값을 찾는다.
	////////////////////////////////////////////////////////////
	//Horizontal
	//get (0,0) count
	idx = 0;		//
	stiX[idx] = pTargetH[idx];	//0
	stiY[idx] = pTargetV[idx];	//0
	// 그 구간의 eye의 median을 구한다.
	x[idx] = getMedian(pEyeH, 0, pSeperate[idx]-1);
	//y[idx] = findValueAt(pEyeH, x[0], pEyeV, 0, pSeperate[idx]);
	y[idx] = getMedian(pEyeV, 0, pSeperate[idx]-1);
	idx++;


	//get (10, 0)
	stiX[idx] = pTargetH[idx];	//0
	stiY[idx] = pTargetV[idx];	//0
	// 그 구간의 eye의 median을 구한다.
	x[idx] = getMedian(pEyeH, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	//y[idx] = findValueAt(pEyeH, x[0], pEyeV, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	y[idx] = getMedian(pEyeV, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	idx++;

	
	//get(-10,0)
	stiX[idx] = pTargetH[idx];	//0
	stiY[idx] = pTargetV[idx];	//0
	// 그 구간의 eye의 median을 구한다.
	x[idx] = getMedian(pEyeH, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	//y[idx] = findValueAt(pEyeH, x[0], pEyeV, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	y[idx] = getMedian(pEyeV, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	idx++;

	////////////////////////////////////////////////////////////
	//Vertical
	//get (0,-10) count
	stiX[idx] = pTargetH[idx];	//0
	stiY[idx] = pTargetV[idx];	//0
	// 그 구간의 eye의 median을 구한다.
	x[idx] = getMedian(pEyeH, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	//y[idx] = findValueAt(pEyeH, x[0], pEyeV, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	y[idx] = getMedian(pEyeV, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	idx++;

	//get (10, 0)
	stiX[idx] = pTargetH[idx];	//0
	stiY[idx] = pTargetV[idx];	//0
	// 그 구간의 eye의 median을 구한다.
	x[idx] = getMedian(pEyeH, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	//y[idx] = findValueAt(pEyeH, x[0], pEyeV, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	y[idx] = getMedian(pEyeV, pSeperate[idx-1], pSeperate[idx]-pSeperate[idx-1]);
	idx++;

	//get(0,0)
	stiX[idx] = pTargetH[idx];	//0
	stiY[idx] = pTargetV[idx];	//0
	// 그 구간의 eye의 median을 구한다.
	x[idx] = getMedian(pEyeH, pSeperate[idx-1], eyeCount-pSeperate[idx-1]);
	//y[idx] = findValueAt(pEyeH, x[0], pEyeV, pSeperate[idx-1], eyeCount-pSeperate[idx-1]);
	y[idx] = getMedian(pEyeV, pSeperate[idx-1], eyeCount-pSeperate[idx-1]);
	


	////////////////////////////////////////////////////////////
	// coefficient를 찾는다.
	m_bCalibrated = false;
	if (analyze3x3(x, y, stiX, stiY))
		m_bCalibrated = true;

	return m_bCalibrated;
}

double getGyroGain(double*pEyee, double*pGyro, unsigned long count);
double getGyroGain2(double*pEyee, double*pGyro, unsigned long count);
bool CCalibration::analyzeUsingGyro(double* pEyeH, double* pEyeV, unsigned long eyeCount, double* pGyroH, double* pGyroV, unsigned long gyroCount)
{
	bool bRtn = true;


	ASSERT(pEyeH && pEyeV && eyeCount && pGyroH && pGyroV && gyroCount);
	ASSERT(eyeCount == gyroCount);

	::ZeroMemory(this->m_dHCoeff, sizeof(double)*6);
	::ZeroMemory(this->m_dVCoeff, sizeof(double)*6);

	// find horizontal coeff
	// h_degree = m_dHCoeff[0] + m_dHCoeff[1]*h_pixel;
	double gainH = getGyroGain2(pEyeH, pGyroH, eyeCount);
	m_dHCoeff[0] = -1*corMean(pEyeH, eyeCount)*gainH;
	m_dHCoeff[1] = gainH;

	// find vertical coeff
	//v_degree = m_dVCoeff[0] + m_dVCoeff[2]*v_pixel;
	double gainV = getGyroGain(pEyeV, pGyroV, eyeCount);
	m_dVCoeff[0] = -1*corMean(pEyeV, eyeCount)*gainV;
	m_dVCoeff[2] = gainV;

	return bRtn;
}

unsigned long getValidCount(double* pData, unsigned long startIdx, char cond, unsigned long count)
{
	//startIdx에서 condition이 유효한 데이터의 개수를 리턴한다.
	// condition은 0(==0) 1(>0), -1( >0)

	unsigned long num = 0;
	for(unsigned long i=startIdx; i<count; i++)
	{
		if(cond == 0)
		{
			if(pData[i] == 0)
				num++;
			else
				break;
		}
		else if(cond > 0)
		{
			if(pData[i] > 0)
				num++;
			else
				break;
		}
		else if(cond < 0)
		{
			if(pData[i] <0 )
				num++;
			else 
				break;
		}

	}
	return num;
}

double getMedian(double* pData, unsigned long startIdx, unsigned long count)
{
	CArray<double> tempArray;
	tempArray.SetSize(count);
	for(unsigned long i=0; i<count; i++)
	{
		tempArray.SetAt(i, pData[startIdx+i]);
	}

	//sort
	std::sort(tempArray.GetData(), tempArray.GetData() + tempArray.GetSize());

	double m = tempArray.GetAt(count>>1);
	tempArray.RemoveAll();
	return m;

}


double findValueAt(double* pData, double Value, double* pDataTarget, unsigned long startIdx, unsigned long count)
{
	// pData에서 Value가 있는 위치의 인덱스를 찾고 
	// pDataTarget에서 해당하는 index의 value를 return한다.

	for(unsigned long i=0; i<count; i++)
	{
		if(pData[startIdx + i] == Value)
			return pDataTarget[startIdx + i];
	}

	return 0;
}


bool CCalibration::analyze5x5(double* pX, double* pY, double* pDegreeH, double* pDegreeV)
{
	//5원 2차 연립 방정식
	//degree = a + bx + cy + dx^2 + ey^2;
	// pX, pY, pDegree는 각각 5개의 data
	//A =		[   1,  x0,  y0, x02, y02]
	//			[   1,  x1,  y1, x12, y12]
	//			[   1,  x2,  y2, x22, y22]
	//			[   1,  x3,  y3, x32, y32]
	//			[   1,  x4,  y4, x42, y42]
	// [a b c d e]' = inv(A) * pDegree

	bool bRtn = false;

	double x0, y0, x02, y02, x1, y1, x12, y12, x2, y2, x22, y22, x3, y3, x32, y32, x4, y4, x42, y42;
	x0 = pX[0];
	y0 = pY[0];
	x02 = x0*x0;
	y02 = y0*y0;
	x1 = pX[1];
	y1 = pY[1];
	x12 = x1*x1;
	y12 = y1*y1;
	x2 = pX[2];
	y2 = pY[2];
	x22 = x2*x2;
	y22 = y2*y2;
	x3 = pX[3];
	y3 = pY[3];
	x32 = x3*x3;
	y32 = y3*y3;
	x4 = pX[4];
	y4 = pY[4];
	x42 = x4*x4;
	y42 = y4*y4;

	double det = y0*x32*x2*y42+x1*x42*y2*y02-x1*y2*x02*y42-y0*x4*x12*y32-x3*y0*x22*y42-x1*x42*y3*y02+x1*y3*x02*y42+x1*y4*x02*y22-x1*y3*x22*y42-x1*x32*y4*y22-y0*x32*x4*y22+y0*x42*x3*y22-x1*y0*x32*y42+x1*x42*y3*y22-x1*y4*x22*y02-y0*x2*x12*y42-x1*y4*x02*y32+y0*x32*x4*y12+y0*x42*x2*y12-x1*x42*y2*y32-y0*x4*x22*y12-y0*x42*x3*y12+x32*x2*y1*y02+x32*y2*x0*y12-x32*x0*y1*y22+y3*x02*x2*y12+x3*x12*y2*y02+x3*y1*x02*y22+y3*x0*x12*y22-x22*x3*y1*y02-x2*y1*x02*y32-y2*x02*x3*y12+x22*x0*y1*y32-x22*y3*x0*y12-x2*x12*y3*y02-y2*x0*x12*y32+y0*x2*x12*y32-y0*x3*x12*y22-y0*x32*x2*y12+y0*x22*x3*y12+x1*y0*x32*y22-x1*y0*x22*y32+x1*x22*y3*y02-x1*y3*x02*y22+x1*y2*x02*y32-x1*x32*y2*y02+x1*y0*x22*y42+x1*y4*x22*y32+y2*x02*x4*y12+x42*y2*x3*y12-x32*y2*x0*y42-x32*y2*x4*y12+x32*y2*x4*y02-x32*x2*y1*y42+x32*x0*y1*y42+x32*y4*x0*y22+x32*y4*x2*y12-x32*y4*x2*y02-x32*y4*x0*y12+x32*x4*y1*y22-x32*x4*y1*y02-y2*x3*x12*y42+y2*x3*x02*y42+x3*x12*y4*y22-x3*x12*y4*y02+y3*x22*x4*y12-x22*x0*y1*y42+x22*y4*x0*y12+x22*x4*y1*y02-y3*x02*x4*y12-y3*x02*x2*y42+y3*x02*x4*y22+y2*x0*x12*y42+x2*y1*x02*y42+x2*x12*y4*y02-y3*x0*x12*y42+x3*y1*x22*y42-x3*y1*x02*y42+x3*x22*y4*y02+x42*y2*x0*y32-x42*y2*x0*y12-x42*x2*y1*y02+x42*x0*y1*y22-x42*y2*x3*y02+x42*x2*y1*y32-x42*x0*y1*y32-x42*y3*x0*y22-x42*y3*x2*y12+x42*y3*x2*y02+x42*y3*x0*y12-x42*x3*y1*y22+x42*x3*y1*y02-y4*x02*x2*y12+y4*x02*x3*y12+y4*x02*x2*y32-y4*x02*x3*y22+y3*x0*x22*y42+y3*x2*x12*y42-y4*x0*x22*y32-y4*x2*x12*y32+y4*x0*x12*y32-y4*x0*x12*y22-x4*y1*x22*y32+x4*y1*x02*y32-x4*y1*x02*y22-x4*x22*y3*y02-y4*x22*x3*y12+y2*x4*x12*y32-y2*x4*x02*y32+x4*x12*y3*y02-x4*x12*y2*y02-x4*x12*y3*y22+y0*x4*x12*y22+y0*x3*x12*y42+x1*x32*y2*y42+x1*y0*x42*y32-y0*x42*x2*y32+x1*x32*y4*y02+y0*x4*x22*y32-x1*y0*x42*y22;

	//a
	if(fabs(det) > 0.00001)
	{
		double a11 = x1*y4*x22*y32-x1*y3*x22*y42-x1*x42*y2*y32+x1*x42*y3*y22+x1*x32*y2*y42-x1*x32*y4*y22+y3*x22*x4*y12+x3*y1*x22*y42-y4*x22*x3*y12-x4*y1*x22*y32+x32*y4*x2*y12-x32*x2*y1*y42+x42*x2*y1*y32-x42*y3*x2*y12+x32*x4*y1*y22+y3*x2*x12*y42-x42*x3*y1*y22+y2*x4*x12*y32-x32*y2*x4*y12+x42*y2*x3*y12+x3*x12*y4*y22-y2*x3*x12*y42-x4*x12*y3*y22-y4*x2*x12*y32;
		double a12 = y0*x4*x22*y32-x3*y0*x22*y42+y0*x32*x2*y42+y0*x42*x3*y22-y0*x32*x4*y22-y0*x42*x2*y32+y3*x0*x22*y42-y4*x0*x22*y32-x4*x22*y3*y02+x3*x22*y4*y02-y3*x02*x2*y42-y2*x4*x02*y32+y4*x02*x2*y32+y3*x02*x4*y22-x42*y2*x3*y02-x42*y3*x0*y22-x32*y4*x2*y02+x42*y3*x2*y02+x32*y4*x0*y22+x42*y2*x0*y32-x32*y2*x0*y42+x32*y2*x4*y02+y2*x3*x02*y42-y4*x02*x3*y22;
		double a13 = x1*y0*x42*y32-x1*y0*x32*y42-x1*x42*y3*y02-x1*y4*x02*y32+x1*y3*x02*y42+x1*x32*y4*y02-y0*x42*x3*y12+y0*x32*x4*y12+y0*x3*x12*y42-y0*x4*x12*y32+x4*x12*y3*y02-x3*y1*x02*y42+y4*x0*x12*y32-y3*x02*x4*y12+y4*x02*x3*y12-x42*x0*y1*y32+x4*y1*x02*y32+x32*x0*y1*y42-y3*x0*x12*y42+x42*y3*x0*y12-x32*y4*x0*y12-x32*x4*y1*y02+x42*x3*y1*y02-x3*x12*y4*y02;
		double a14 = x1*y0*x22*y42-x1*y0*x42*y22-x1*y4*x22*y02+x1*y4*x02*y22+x1*x42*y2*y02-x1*y2*x02*y42-y0*x4*x22*y12-y0*x2*x12*y42+y0*x42*x2*y12+y0*x4*x12*y22+x22*x4*y1*y02-x22*x0*y1*y42+x22*y4*x0*y12-y4*x0*x12*y22+x2*y1*x02*y42-x4*y1*x02*y22-x42*x2*y1*y02-x4*x12*y2*y02-x42*y2*x0*y12-y4*x02*x2*y12+y2*x0*x12*y42+y2*x02*x4*y12+x42*x0*y1*y22+x2*x12*y4*y02;
		double a15 = x1*y0*x32*y22-x1*y0*x22*y32+x1*y2*x02*y32-x1*x32*y2*y02-x1*y3*x02*y22+x1*x22*y3*y02+x32*x2*y1*y02+x32*y2*x0*y12-x32*x0*y1*y22+y3*x02*x2*y12+x3*x12*y2*y02+x3*y1*x02*y22+y3*x0*x12*y22-x22*x3*y1*y02-x2*y1*x02*y32-y2*x02*x3*y12+x22*x0*y1*y32-x22*y3*x0*y12-x2*x12*y3*y02-y2*x0*x12*y32+y0*x2*x12*y32-y0*x3*x12*y22-y0*x32*x2*y12+y0*x22*x3*y12;

		double a21 = x22*y12*y4+y1*x22*y32-y3*x22*y12-y4*x22*y32+y3*x22*y42-x22*y42*y1-x32*y2*y42-x42*y32*y1+x42*y22*y1-y12*x42*y2+x32*y42*y1-y12*x32*y4-x12*y22*y4+y42*y2*x12-y42*x12*y3+x12*y32*y4+y12*x42*y3+x32*y4*y22-x32*y1*y22-x42*y3*y22+x12*y3*y22+x42*y2*y32+x32*y2*y12-x12*y2*y32;
		double a22 = -y0*x32*y42-y0*x42*y22+y0*x42*y32+y0*x22*y42+y0*x32*y22-y0*x22*y32-y4*x02*y32-x42*y2*y32-y3*x22*y42+y4*x02*y22+x32*y4*y02+x42*y2*y02-x42*y3*y02+y4*x22*y32+y3*x02*y42-y2*x02*y42+x42*y3*y22-x32*y4*y22-x22*y4*y02+x32*y2*y42+x22*y3*y02+y2*x02*y32-x32*y2*y02-y3*x02*y22;
		double a23 = y0*y12*x42-y0*y42*x12+y0*x12*y32-y0*x42*y32-y0*x32*y12+y0*x32*y42-x32*y42*y1-x12*y32*y4-y12*x42*y3+y12*x32*y4+y4*x02*y32-y3*x02*y42+y3*x02*y12-x12*y3*y02+x32*y1*y02-y1*x02*y32+y42*y1*x02+y42*x12*y3+x42*y32*y1+y4*y02*x12+x42*y3*y02-y1*y02*x42-x32*y4*y02-y12*y4*x02;
		double a24 = -y0*x22*y42+y0*x22*y12+y0*x42*y22+y0*y42*x12-y0*y12*x42-y0*x12*y22-x22*y1*y02+x22*y4*y02-x22*y12*y4+x22*y42*y1+y12*x42*y2-y42*y2*x12+y1*y02*x42-x42*y2*y02+x12*y22*y4-y4*x02*y22-x42*y22*y1-y42*y1*x02-y4*y02*x12+y12*y4*x02+x12*y2*y02+y2*x02*y42+y1*x02*y22-y2*x02*y12;
		double a25 = -y0*x22*y12+y0*x22*y32+y0*x32*y12-y0*x12*y32-y0*x32*y22+y0*x12*y22-x22*y3*y02+y3*x22*y12+x22*y1*y02-y1*x22*y32-y2*x02*y32-x12*y2*y02+x32*y1*y22-x12*y3*y22+x32*y2*y02-y3*x02*y12-x32*y1*y02+y3*x02*y22+x12*y3*y02-x32*y2*y12+y1*x02*y32-y1*x02*y22+x12*y2*y32+y2*x02*y12;

		double a31 = -x1*x42*y22-x1*x32*y42-x1*x22*y32+x1*x22*y42+x1*x32*y22+x1*x42*y32-x22*x4*y12-x32*x4*y22+x3*x12*y42+x22*x3*y12+x2*x12*y32-x42*x2*y32-x3*x12*y22+x22*y32*x4-x2*x12*y42-x32*x2*y12+x4*x12*y22-x4*x12*y32-x22*y42*x3+x42*x3*y22-x42*x3*y12+x42*x2*y12+x32*x2*y42+x32*x4*y12;
		double a32 = x0*x22*y32+x22*y02*x4-x22*y42*x0+x22*y42*x3-x22*y32*x4-x3*x22*y02+x32*x4*y22-x02*x2*y32+x32*x2*y02+x02*x3*y22-x32*x0*y22-x42*x3*y22-x32*x2*y42-x42*y32*x0+y42*x2*x02+y02*x42*x3-x02*y22*x4+x02*y32*x4+x42*y22*x0+x42*x2*y32-y42*x3*x02+x32*y42*x0-y02*x32*x4-y02*x42*x2;
		double a33 = -x1*y42*x02+x1*x32*y42-x1*x42*y32+x1*y02*x42-x1*x32*y02+x1*x02*y32+x32*x0*y12-x0*y12*x42+x3*x12*y02-y02*x4*x12-x0*x12*y32+y02*x32*x4-y02*x42*x3+y42*x3*x02-x32*x4*y12+x42*y32*x0+y42*x0*x12-x32*y42*x0-x02*x3*y12-x02*y32*x4-x3*x12*y42+x42*x3*y12+x4*x12*y32+x4*y12*x02;
		double a34 = -x1*x22*y42+x1*x22*y02-x1*x02*y22+x1*y42*x02+x1*x42*y22-x1*y02*x42+x22*x4*y12+x22*y42*x0-x22*y02*x4-x22*x0*y12-y42*x0*x12-x4*y12*x02+x02*y22*x4-x4*x12*y22+x02*x2*y12-x2*x12*y02+y02*x4*x12-x42*x2*y12+x0*y12*x42-y42*x2*x02+x2*x12*y42-x42*y22*x0+x0*x12*y22+y02*x42*x2;
		double a35 = -x1*x32*y22-x1*x02*y32-x1*x22*y02+x1*x02*y22+x1*x22*y32+x1*x32*y02+x3*x22*y02+x2*x12*y02+x32*x2*y12+x0*x12*y32-x3*x12*y02+x32*x0*y22-x32*x2*y02-x22*x3*y12-x32*x0*y12-x0*x12*y22+x02*x2*y32-x0*x22*y32+x22*x0*y12-x2*x12*y32+x3*x12*y22-x02*x3*y22-x02*x2*y12+x02*x3*y12;
		
		double a41 = -x1*y32*y4-x1*y3*y22+x1*y2*y32+x1*y4*y22+x1*y42*y3-x1*y2*y42-x4*y1*y22-x4*y12*y3+x3*y2*y42-x2*y1*y32+y3*x4*y22-y42*x3*y1+y32*x4*y1+x2*y1*y42+y2*x4*y12-y2*x3*y12-y3*x2*y42-y4*x2*y12-x4*y2*y32+y3*x2*y12-x3*y4*y22+x3*y1*y22+x3*y12*y4+y4*x2*y32;
		double a42 = -y0*y32*x4-x2*y0*y42+y0*y42*x3-x3*y0*y22+x4*y0*y22+x2*y0*y32-y2*x4*y02+y2*x3*y02-y3*x2*y02-y4*x0*y22+x4*y2*y32-y4*x2*y32+y32*y4*x0+y3*y02*x4-y2*x0*y32+y3*x2*y42+x3*y4*y22-y3*x4*y22-x3*y2*y42-y4*y02*x3-y42*y3*x0+y4*x2*y02+y2*x0*y42+y3*x0*y22;
		double a43 = -x1*y0*y32+x1*y0*y42-x1*y42*y3+x1*y32*y4+x1*y3*y02-x1*y4*y02+y0*y32*x4-y0*y42*x3-y0*x4*y12+y0*x3*y12+x0*y1*y32-y3*x0*y12-x3*y1*y02-x0*y1*y42+y4*x0*y12+x4*y12*y3+y4*y02*x3+y42*y3*x0+x4*y1*y02+y42*x3*y1-y3*y02*x4-x3*y12*y4-y32*y4*x0-y32*x4*y1;
		double a44 = -x1*y0*y42+x1*y0*y22+x1*y2*y42-x1*y4*y22-x1*y2*y02+x1*y4*y02+y2*x0*y12+x2*y1*y02-y0*x2*y12-x0*y1*y22-y2*x0*y42-y2*x4*y12+y2*x4*y02+y0*x4*y12-x2*y1*y42+x2*y0*y42+x0*y1*y42+y4*x0*y22+y4*x2*y12-y4*x2*y02-y4*x0*y12+x4*y1*y22-x4*y1*y02-x4*y0*y22;
		double a45 = -x1*y0*y22+x1*y0*y32-x1*y3*y02+x1*y3*y22-x1*y2*y32+x1*y2*y02+x3*y0*y22+y0*x2*y12-y0*x3*y12-x2*y0*y32+y2*x3*y12+x2*y1*y32+x0*y1*y22-y3*x2*y12+y3*x0*y12+y3*x2*y02+y2*x0*y32-x2*y1*y02-x3*y1*y22-y2*x3*y02-y2*x0*y12+x3*y1*y02-x0*y1*y32-y3*x0*y22;

		double a51 = x1*y3*x22-x1*x32*y2-x1*y4*x22+x1*x42*y2-x1*x42*y3+x1*x32*y4-x42*y2*x3+x4*x12*y3-y2*x4*x12+x42*x3*y1-x32*x4*y1+y4*x2*x12+x32*y2*x4+x3*x22*y4+x4*y1*x22-x3*y1*x22-y3*x2*x12+x2*y1*x32-x32*y4*x2+y2*x3*x12-x2*y1*x42+x42*y3*x2-x3*x12*y4-x4*x22*y3;
		double a52 = x3*y0*x22+y0*x32*x4+y0*x42*x2-y0*x32*x2-y0*x42*x3-y0*x4*x22-x42*y3*x2-x3*x22*y4+x42*y2*x3+x4*x22*y3-x32*y2*x4+x32*y4*x2-y4*x2*x02+y4*x0*x22+y2*x4*x02-y2*x0*x42+y3*x2*x02-y3*x0*x22-y2*x3*x02+y2*x0*x32-y3*x02*x4+x42*y3*x0+y4*x02*x3-x32*y4*x0;
		double a53 = -x1*y0*x42+x1*y0*x32-x1*y3*x02+x1*y4*x02-x1*x32*y4+x1*x42*y3+y0*x4*x12-y0*x32*x4+y0*x42*x3-y0*x3*x12-y4*x0*x12-x4*x12*y3+x3*y1*x02-x0*y1*x32+x32*x4*y1+x0*y1*x42-x42*y3*x0+y3*x02*x4+y3*x0*x12+x32*y4*x0+x3*x12*y4-x42*x3*y1-x4*y1*x02-y4*x02*x3;
		double a54 = -x1*y0*x22+x1*y0*x42+x1*y2*x02-x1*x42*y2-x1*y4*x02+x1*y4*x22-y0*x42*x2+y0*x2*x12+y0*x4*x22-y0*x4*x12+x0*y1*x22+x2*y1*x42+x4*y1*x02+y4*x0*x12-x2*y1*x02-x4*y1*x22+y2*x4*x12-x0*y1*x42+y2*x0*x42-y4*x2*x12-y2*x4*x02-y2*x0*x12+y4*x2*x02-y4*x0*x22;
		double a55 = -x1*y0*x32+x1*x32*y2-x1*y2*x02-x1*y3*x22+x1*y0*x22+x1*y3*x02+y2*x0*x12+x2*y1*x02-y0*x2*x12-x0*y1*x22-y2*x0*x32-y2*x3*x12+y2*x3*x02+y0*x3*x12-x2*y1*x32+y0*x32*x2+x0*y1*x32+y3*x0*x22+y3*x2*x12-y3*x2*x02-y3*x0*x12+x3*y1*x22-x3*y1*x02-x3*y0*x22;


		m_dHCoeff[0] = (pDegreeH[0]*a11 + pDegreeH[1]*a12 + pDegreeH[2]*a13 + pDegreeH[3]*a14 +	pDegreeH[4]*a15)/(det);
		m_dHCoeff[1] = (pDegreeH[0]*a21 + pDegreeH[1]*a22 + pDegreeH[2]*a23 + pDegreeH[3]*a24 + pDegreeH[4]*a25)/(det);
		m_dHCoeff[2] = (pDegreeH[0]*a31 + pDegreeH[1]*a32 + pDegreeH[2]*a33 + pDegreeH[3]*a34 + pDegreeH[4]*a35)/(det);
		m_dHCoeff[3] = (pDegreeH[0]*a41 + pDegreeH[1]*a42 + pDegreeH[2]*a43 + pDegreeH[3]*a44 + pDegreeH[4]*a45)/(det);
		m_dHCoeff[4] = (pDegreeH[0]*a51 + pDegreeH[1]*a52 + pDegreeH[2]*a53 + pDegreeH[3]*a54 + pDegreeH[4]*a55)/(det);
		m_dHCoeff[5] = 0;

		m_dVCoeff[0] = (pDegreeV[0]*a11 + pDegreeV[1]*a12 + pDegreeV[2]*a13 + pDegreeV[3]*a14 +	pDegreeV[4]*a15)/(det);
		m_dVCoeff[1] = (pDegreeV[0]*a21 + pDegreeV[1]*a22 + pDegreeV[2]*a23 + pDegreeV[3]*a24 + pDegreeV[4]*a25)/(det);
		m_dVCoeff[2] = (pDegreeV[0]*a31 + pDegreeV[1]*a32 + pDegreeV[2]*a33 + pDegreeV[3]*a34 + pDegreeV[4]*a35)/(det);
		m_dVCoeff[3] = (pDegreeV[0]*a41 + pDegreeV[1]*a42 + pDegreeV[2]*a43 + pDegreeV[3]*a44 + pDegreeV[4]*a45)/(det);
		m_dVCoeff[4] = (pDegreeV[0]*a51 + pDegreeV[1]*a52 + pDegreeV[2]*a53 + pDegreeV[3]*a54 + pDegreeV[4]*a55)/(det);
		m_dVCoeff[5] = 0;

		bRtn = true;

	}
	else
	{
		bRtn = false;
	}


	return bRtn;



}


bool CCalibration::analyze3x3(double* pX, double* pY, double* pDegreeH, double* pDegreeV)
{
	//3원 2차 연립 방정식
	//degree = a + bx + cy;
	// pX, pY, pDegree는 각각 6개의 data
	//A =		[   1,  x0,  y0]
	//			[   1,  x1,  y1]
	//			[   1,  x2,  y2]
	//B = [pDegree[0]...pDegree[5]]'
	// [a b c]' = inv(A'A)A' * B
	//		
	

	bool bRtn = false;

	double x0, y0, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
	x0 = pX[0]; x1 = pX[1]; x2 = pX[2]; x3 = pX[3]; x4 = pX[4]; x5 = pX[5];
	y0 = pY[0]; y1 = pY[1];	y2 = pY[2];	y3 = pY[3];	y4 = pY[4];	y5 = pY[5];

	//C = A'A 
	double c00, c01, c02, c10, c11, c12, c20, c21, c22;
	c00 = 6;
	c01 = x0+x1+x2+x3+x4+x5;
	c02 = y0+y1+y2+y3+y4+y5;
	c10 = c01;
	c11 = x0*x0 + x1*x1 + x2*x2 + x3*x3 + x4*x4 + x5*x5;
	c12 = x0*y0 + x1*y1 + x2*y2 + x3*y3 + x4*y4 + x5*y5;
	c20 = c02;
	c21 = c12;
	c22 = y0*y0 + y1*y1 + y2*y2 + y3*y3 + y4*y4 + y5*y5;

	//get inv(C)
	double det =  c00*c11*c22-c00*c12*c21-c10*c01*c22+c10*c02*c21+c20*c01*c12-c20*c02*c11;
	

	if(fabs(det) >0.001)
	{

		//horizontal
		m_dHCoeff[0] = ((c11*c22-c12*c21+(-c01*c22+c02*c21)*x0+(c01*c12-c02*c11)*y0)*pDegreeH[0] + 
						(c11*c22-c12*c21+(-c01*c22+c02*c21)*x1+(c01*c12-c02*c11)*y1)*pDegreeH[1] + 
						(c11*c22-c12*c21+(-c01*c22+c02*c21)*x2+(c01*c12-c02*c11)*y2)*pDegreeH[2] +
						(c11*c22-c12*c21+(-c01*c22+c02*c21)*x3+(c01*c12-c02*c11)*y3)*pDegreeH[3] + 
						(c11*c22-c12*c21+(-c01*c22+c02*c21)*x4+(c01*c12-c02*c11)*y4)*pDegreeH[4] + 
						(c11*c22-c12*c21+(-c01*c22+c02*c21)*x5+(c01*c12-c02*c11)*y5)*pDegreeH[5])/det;
		m_dHCoeff[1] = ((-c10*c22+c12*c20+(c00*c22-c02*c20)*x0+(-c00*c12+c02*c10)*y0)*pDegreeH[0] +
						(-c10*c22+c12*c20+(c00*c22-c02*c20)*x1+(-c00*c12+c02*c10)*y1)*pDegreeH[1] +
						(-c10*c22+c12*c20+(c00*c22-c02*c20)*x2+(-c00*c12+c02*c10)*y2)*pDegreeH[2] +
						(-c10*c22+c12*c20+(c00*c22-c02*c20)*x3+(-c00*c12+c02*c10)*y3)*pDegreeH[3] + 
						(-c10*c22+c12*c20+(c00*c22-c02*c20)*x4+(-c00*c12+c02*c10)*y4)*pDegreeH[4] + 
						(-c10*c22+c12*c20+(c00*c22-c02*c20)*x5+(-c00*c12+c02*c10)*y5)*pDegreeH[5])/det;
		m_dHCoeff[2] = ((c10*c21-c11*c20+(-c00*c21+c01*c20)*x0+(c00*c11-c01*c10)*y0)*pDegreeH[0] + 
						(c10*c21-c11*c20+(-c00*c21+c01*c20)*x1+(c00*c11-c01*c10)*y1)*pDegreeH[1] +
						(c10*c21-c11*c20+(-c00*c21+c01*c20)*x2+(c00*c11-c01*c10)*y2)*pDegreeH[2] +
						(c10*c21-c11*c20+(-c00*c21+c01*c20)*x3+(c00*c11-c01*c10)*y3)*pDegreeH[3] + 
						(c10*c21-c11*c20+(-c00*c21+c01*c20)*x4+(c00*c11-c01*c10)*y4)*pDegreeH[4] + 
						(c10*c21-c11*c20+(-c00*c21+c01*c20)*x5+(c00*c11-c01*c10)*y5)*pDegreeH[5])/det;
		m_dHCoeff[3] = 0;
		m_dHCoeff[4] = 0;

		//vertical
		m_dVCoeff[0] = ((c11*c22-c12*c21+(-c01*c22+c02*c21)*x0+(c01*c12-c02*c11)*y0)*pDegreeV[0] + 
						(c11*c22-c12*c21+(-c01*c22+c02*c21)*x1+(c01*c12-c02*c11)*y1)*pDegreeV[1] + 
						(c11*c22-c12*c21+(-c01*c22+c02*c21)*x2+(c01*c12-c02*c11)*y2)*pDegreeV[2] +
						(c11*c22-c12*c21+(-c01*c22+c02*c21)*x3+(c01*c12-c02*c11)*y3)*pDegreeV[3] + 
						(c11*c22-c12*c21+(-c01*c22+c02*c21)*x4+(c01*c12-c02*c11)*y4)*pDegreeV[4] + 
						(c11*c22-c12*c21+(-c01*c22+c02*c21)*x5+(c01*c12-c02*c11)*y5)*pDegreeV[5])/det;
		m_dVCoeff[1] = ((-c10*c22+c12*c20+(c00*c22-c02*c20)*x0+(-c00*c12+c02*c10)*y0)*pDegreeV[0] +
						(-c10*c22+c12*c20+(c00*c22-c02*c20)*x1+(-c00*c12+c02*c10)*y1)*pDegreeV[1] +
						(-c10*c22+c12*c20+(c00*c22-c02*c20)*x2+(-c00*c12+c02*c10)*y2)*pDegreeV[2] +
						(-c10*c22+c12*c20+(c00*c22-c02*c20)*x3+(-c00*c12+c02*c10)*y3)*pDegreeV[3] + 
						(-c10*c22+c12*c20+(c00*c22-c02*c20)*x4+(-c00*c12+c02*c10)*y4)*pDegreeV[4] + 
						(-c10*c22+c12*c20+(c00*c22-c02*c20)*x5+(-c00*c12+c02*c10)*y5)*pDegreeV[5])/det;
		m_dVCoeff[2] = ((c10*c21-c11*c20+(-c00*c21+c01*c20)*x0+(c00*c11-c01*c10)*y0)*pDegreeV[0] + 
						(c10*c21-c11*c20+(-c00*c21+c01*c20)*x1+(c00*c11-c01*c10)*y1)*pDegreeV[1] +
						(c10*c21-c11*c20+(-c00*c21+c01*c20)*x2+(c00*c11-c01*c10)*y2)*pDegreeV[2] +
						(c10*c21-c11*c20+(-c00*c21+c01*c20)*x3+(c00*c11-c01*c10)*y3)*pDegreeV[3] + 
						(c10*c21-c11*c20+(-c00*c21+c01*c20)*x4+(c00*c11-c01*c10)*y4)*pDegreeV[4] + 
						(c10*c21-c11*c20+(-c00*c21+c01*c20)*x5+(c00*c11-c01*c10)*y5)*pDegreeV[5])/det;
		m_dVCoeff[3] = 0;
		m_dVCoeff[4] = 0;
		
		bRtn = true;
	}






	return bRtn;
}

CString CCalibration::getModelStr()
{
	CString str;

	str.Format(" H(degree) = %.2f + %.2f*X(pixel) + %.2f*Y(pixel) \n V(degree) = %.2f + %.2f*X(pixel) + %.2f*Y(pixel) ", 				
				m_dHCoeff[0], m_dHCoeff[1], m_dHCoeff[2],
				m_dVCoeff[0], m_dVCoeff[1], m_dVCoeff[2]);
	return str;
}

void CCalibration::degree2pixel(double* pD, double* pP)
{
	//x
	double det = ((m_dHCoeff[1]*m_dVCoeff[2])-(m_dHCoeff[2]*m_dVCoeff[1]));
	if (det)
	{
		pP[0] = 1/det*
			((pD[0]-m_dHCoeff[0])*(m_dVCoeff[2]) - 
			(pD[1]-m_dVCoeff[0])*(m_dHCoeff[2]));
		pP[1] = 1/det*
			(-1*(pD[0]-m_dHCoeff[0])*(m_dVCoeff[1]) + 
			(pD[1]-m_dVCoeff[0])*(m_dHCoeff[1]));
	}
	else
		pP[0] = pP[1] = 0;
}


//pupil size 칼리브레이션
double CCalibration::pixel2mm(double rPixel)
{
	return rPixel = m_dRCoeff*rPixel;
}

double CCalibration::mm2pixel(double mm)
{
	double pixel = 0;
	if(m_dRCoeff)
		pixel = mm/m_dRCoeff;

	return pixel;
}



bool CCalibration::load(CString fname)
{
	bool bRtn = false;

	CFile file;
	if(file.Open(fname, CFile::modeRead))
	{
		if(file.GetLength() == SIZE_CALIBRATION_FILE)
		{
			int ver = 0;
			file.Read(&ver, sizeof(int));
			file.Seek(256, CFile::begin);
			file.Read(m_dHCoeff, sizeof(double)*6);
			file.Seek(512, CFile::begin);
			file.Read(m_dVCoeff, sizeof(double)*6);
			file.Seek(256*3, CFile::begin);
			file.Read(&m_dRCoeff, sizeof(double)*6);

/*			file.Seek(896, CFile::begin);
			file.Read(&m_ulSeperateTime, sizeof(double)*5);
			file.Seek(960, CFile::begin);
			file.Read(&m_dCustomLRTB, sizeof(double)*2);
			file.Seek(1012, CFile::begin);
			file.Read(&m_bCustom, sizeof(bool));
*/
			bRtn = m_bCalibrated = true;
		}

		file.Close();
	}

	return bRtn;
}
bool CCalibration::save(CString fname)
{
	bool bRtn = false;
	CFile file;
	if(file.Open(fname, CFile::modeCreate | CFile::modeWrite))
	{
		unsigned char buf[SIZE_CALIBRATION_FILE];
		::ZeroMemory(buf, SIZE_CALIBRATION_FILE);

		int ver = 1;
		memcpy(buf, &ver, sizeof(int));
		memcpy(&(buf[256]), m_dHCoeff, sizeof(double)*6);
		memcpy(&(buf[512]), m_dVCoeff, sizeof(double)*6);
		memcpy(&(buf[768]),&m_dRCoeff, sizeof(double));

		/*memcpy(&(buf[896]), m_ulSeperateTime, sizeof(double)*5);
		memcpy(&(buf[960]), m_dCustomLRTB, sizeof(double)*2);
		memcpy(&(buf[1012]), &m_bCustom, sizeof(bool));
*/
		file.Write(buf, SIZE_CALIBRATION_FILE);

		file.Close();

		bRtn = true;

	}
	return bRtn;
}


double getGyroGain(double* pEye, double* pGyro, unsigned long count)
{
	double gain = 0;

	unsigned long startIdx, endIdx;
	double gyroThres = 50;		// 50 degree/sec
	startIdx = endIdx = 0;
	double maxG, minG, maxE, minE;
	maxG = maxE = -1000;
	minG = minE = 1000;
	CList<double, double> listGain;
	for(unsigned long i=1; i<count; i++)
	{
		
		if(startIdx == 0)
		{
			//find start indx
			if( (abs(pGyro[i-1]) < gyroThres) && (abs(pGyro[i]) >= gyroThres))
				startIdx = i;
		}
		else
		{
			// update min max value of gyro and eye velocity
			if(maxG < pGyro[i])			// maximum gyro
				maxG = pGyro[i];
			if(minG > pGyro[i])			// minimum gyro
				minG = pGyro[i];
			if( maxE < (pEye[i]-pEye[i-1])*120 )		//maximum eye velocity
				maxE = (pEye[i]-pEye[i-1])*120;
			if( minE > (pEye[i]-pEye[i-1])*120 )		// minimum eye velocity
				minE = (pEye[i]-pEye[i-1])*120;

			//find end index
			if( (abs(pGyro[i-1]) > gyroThres) && (abs(pGyro[i]) <= gyroThres) )
			{
				endIdx = i;

				if( (endIdx-startIdx)/120.f*1000 > 100)	//100ms 가 떨어져 있어야 한다.
				{
					//peak 값을 얻는다.
					double gyro = abs(maxG)>abs(minG) ? maxG : minG;
					double eyeVel = abs(maxE)>abs(minE) ? maxE : minE;

					double g = (eyeVel!=0) ? (gyro/eyeVel) : 0;
					
					if(g)
						listGain.AddTail(g);
					
				}

				//초기화
				startIdx = endIdx = 0;
				maxG = -1000;
				minG = 1000;
				maxE = -1000;
				minE = 1000;
			}

		}

	}

	if(listGain.GetCount())
	{
		//list 값들을 얻어온다.
		double* pData = new double[listGain.GetCount()];
		int count = listGain.GetCount();
		POSITION pos = listGain.GetHeadPosition();
		for(int i=0; i<count; i++)
			pData[i] = listGain.GetNext(pos);

		//sorting
		corSort(pData, count);

		//get gain
		// 20~80% 사이 값의 mean을 계산한다..
		int startCount = int((count-1)*0.2 + .5);
		int endCount = int((count-1)*0.8 + .5);

		if(startCount<0) startCount = 0;
		if(endCount<0) startCount = 0;

		double sum = 0;
		double sumCount = 0;

		for(int i=startCount; i<=endCount; i++)
		{
			sum += pData[i];
			sumCount++;
		}


		gain = sum/sumCount;


        delete []pData;
	}
	else
		gain = 1;



	return gain;
}


double getGyroGain2(double* pEye, double* pGyro, unsigned long count)
{
	//getGyroGain() 은 peak 순간의 한 point만 사용
	//getGyroGain2 는 좌우 50ms 의 point 의 mean을 사용

	double gain = 0;

	//get eye velocity
	double* pEyeVel = new double[count];
	memset(pEyeVel, 0, sizeof(double)*count);
	for(int i=1; i<count; i++)
		pEyeVel[i] = (pEye[i]-pEye[i-1])*120 ;

	unsigned long startIdx, endIdx;
	double maxG, minG;
	unsigned long maxGIdx, minGIdx;
	maxG = -1000;
	minG = 1000;


	double gyroThres = 50;		// 50 degree/sec
	startIdx = endIdx = 0;
	CList<double, double> listGain;

	int M = 5;
	for(unsigned long i=M; i<count-M-1; i++)
	{
		
		if(startIdx == 0)
		{
			//find start indx
			if( (abs(pGyro[i-1]) < gyroThres) && (abs(pGyro[i]) >= gyroThres))
				startIdx = i;
		}
		else
		{
			// update min max value of gyro and eye velocity
			if(maxG < pGyro[i])			// maximum gyro
			{
				maxG = pGyro[i];
				maxGIdx = i;
			}

			if(minG > pGyro[i])			// minimum gyro
			{
				minG = pGyro[i];
				minGIdx = i;
			}

			//find end index
			if( (abs(pGyro[i-1]) > gyroThres) && (abs(pGyro[i]) <= gyroThres) )
			{
				endIdx = i;

				if( (endIdx-startIdx)/120.f*1000 > 50)	//100ms이상 떨어져 있어야 한다.
				{
					//peak 값을 얻는다.
					double gyro, eyeVel;
					if(abs(maxG) > abs(minG))
					{
						gyro = corMedian(&(pGyro[maxGIdx-M]), 2*M+1);
						eyeVel = corMedian(&(pEyeVel[maxGIdx-M]),2*M+1);
					}
					else
					{
						gyro = corMedian(&(pGyro[minGIdx-M]), 2*M+1);
						eyeVel = corMedian(&(pEyeVel[minGIdx-M]),2*M+1);
					}

					double g = (eyeVel!=0) ? (gyro/eyeVel) : 0;
					
					if(g)
						listGain.AddTail(g);
					
				}

				//초기화
				startIdx = endIdx = 0;
				maxG = -1000;
				minG = 1000;
			}

		}

	}

	if(listGain.GetCount())
	{
		//list 값들을 얻어온다.
		double* pData = new double[listGain.GetCount()];
		int count = listGain.GetCount();
		POSITION pos = listGain.GetHeadPosition();
		for(int i=0; i<count; i++)
			pData[i] = listGain.GetNext(pos);

		//sorting
		corSort(pData, count);

		//get gain
		// 20~80% 사이 값의 mean을 계산한다..
		int startCount = int((count-1)*0.2 + .5);
		int endCount = int((count-1)*0.8 + .5);

		if(startCount<0) startCount = 0;
		if(endCount<0) startCount = 0;

		double sum = 0;
		double sumCount = 0;

		for(int i=startCount; i<=endCount; i++)
		{
			sum += pData[i];
			sumCount++;
		}


		gain = sum/sumCount;


        delete []pData;
	}
	else
		gain = 1;

	delete []pEyeVel;



	return gain;
}