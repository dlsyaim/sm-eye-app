#include "StdAfx.h"
#include ".\analyzecaloric.h"
#include "math.h"



CCaloricData::CCaloricData(void)
{
	m_strTestFname = "";
	m_peakValue  = 0;
	m_peakRegion[0] = 0;
	m_peakRegion[1] = 10;
	

	m_bUseFixationBlock = false;
	m_fixationBlock[0] = 40;
	m_fixationBlock[1] = 50;

	m_nManualPeakSelect = 0;


}

CCaloricData::~CCaloricData(void)
{
}


void CCaloricData::calcPeakRegion(int test, double spon)
{
	//nystagmus 개수
	long count = long(m_analyzeNys.m_listNystagmus.GetCount());

	if(count)
	{
	
		int sec = 0;
		double maxVel = 0;
		int maxVelCount = 0;

		double ydir = test<2 ? 1:-1;

		if(this->m_nManualPeakSelect == 1)	//x축만 사용자가 지정. 이때의 peak value를 찾는다.
		{
			POSITION pos = m_analyzeNys.m_listNystagmus.GetHeadPosition();
			// 사용자가 지정한 영역에서 구한다면
			for(int i=0; i<count; i++)
			{
				structNystag* pNys = m_analyzeNys.m_listNystagmus.GetNext(pos);
				if(pNys)
				{
					sec = long((pNys->endI + pNys->startI)/2./FRAMERATE +.5);
					if((sec>m_peakRegion[0]) && (sec<m_peakRegion[1]))
					{
						//방향이 맞는 것만 계산한다.
						if(ydir*(pNys->vel + spon) > 0)
						{
							maxVel += pNys->vel;
							maxVelCount++;
						}
						else
							int l = 0;
					}

				}

			}
			if(maxVelCount)
				this->m_peakValue = maxVel/maxVelCount;
			else
				m_peakValue = 0;

		}
		else if(this->m_nManualPeakSelect == 0)	//전체 구간에서 찾는다.
		{
			//전체 구간에서 찾는다.
			int direction = test<2 ? 1:-1;

			//구간 길이(의 반)
			int width = int((m_peakRegion[1]-m_peakRegion[0])/2 + .5);

			//buf length를 계산한다. 마지막 nystagmus
			structNystag* pNys = m_analyzeNys.m_listNystagmus.GetTail();
			long bufLen = long((pNys->endI+pNys->startI )/2./FRAMERATE +.5) + width +1;

			//
			double* pBuf = new double[bufLen];
			int* pBufCount = new int[bufLen];
			memset(pBuf, 0, sizeof(double)*(bufLen));
			memset(pBufCount, 0, sizeof(int)*(bufLen));

			//구간 내의 peak 평균을 구하기 위해 buf를 채운다.
			POSITION pos = m_analyzeNys.m_listNystagmus.GetHeadPosition();
			
			for(int i=0; i<count; i++)
			{
				pNys = m_analyzeNys.m_listNystagmus.GetNext(pos);
				if(pNys)
				{
					sec = long((pNys->endI + pNys->startI)/2./FRAMERATE +.5);
					for(int j=-5; j<=5; j++)
					{
						if((sec+j>=0) && (sec+j < bufLen))
						{
							//방향이 맞는 것만 계산한다.
							if(ydir*(pNys->vel + spon) > 0)
							{
								pBuf[sec+j] += pNys->vel;
								pBufCount[sec+j]++;
							}
							else
								int k = 0;
						}
					}
				}
			}

			//평균 속도를 구한다.
			for(int i=0; i<bufLen; i++)
			{
				if(pBufCount[i] && (pBufCount[i] > width*2*0.4))		//10초라면 4개 이상이 있어야 한다.
					pBuf[i] /= pBufCount[i];
				else
					pBuf[i] = 0;
			}

			//최대값을 구한다. test가 2(LC), 3(RW)일 경우 최소값을 찾는다.
			double maxVel = 0;
			int maxIdx = 0;
			if(test<=RC)
			{
				for(int i=0; i<bufLen; i++)
				{
					if(maxVel < pBuf[i])
					{
						maxVel = pBuf[i];
						maxIdx = i;
					}
				}
			}
			else
			{
				for(int i=0; i<bufLen; i++)
				{
					if(maxVel > pBuf[i])
					{
						maxVel = pBuf[i];
						maxIdx = i;
					}
				}
			}

			this->m_peakValue = maxVel;
			this->m_peakRegion[0] = maxIdx-width;
			this->m_peakRegion[1] = maxIdx+width;
			
		}
		else
		{
			//사용자가 X,Y를 모두 지정. rect tool dragged에서 입력되었으므로 따로 계산하지 않는다.
		}
	}
	else
	{
		m_peakValue = 0;
		m_peakRegion[0] = 0;
			m_peakRegion[1] = 10;
	}

}


void CCaloricData::calcFixationIndex(int test, double spon)
{
	double ydir = test<2 ? 1 : -1;


	//fixation 구간이 잘 설정되어 있다면
	if(this->m_fixationBlock[0] < m_fixationBlock[1])
	{
		long count = (long)this->m_analyzeNys.m_listNystagmus.GetCount();
		if(count)
		{
			POSITION pos = m_analyzeNys.m_listNystagmus.GetHeadPosition();
			double sec;
			double fixVel, unfixVel;
			int fixCount, unfixCount;
			fixVel = unfixVel = fixCount = unfixCount = 0;
			for(int i=0; i<count; i++)
			{
				structNystag* pNys = m_analyzeNys.m_listNystagmus.GetNext(pos);
				if((pNys) && (ydir*(pNys->vel+spon) > 0))
				{
					
					sec = (pNys->startI + pNys->endI)/2./FRAMERATE;
					//fixation 구간에 있다면
					if((sec>= m_fixationBlock[0]) && ( sec<= m_fixationBlock[1]))
					{
						fixVel += pNys->vel;
						fixCount++;
					}
					else
					{
						//아니라면
						unfixVel += pNys->vel;
						unfixCount++;
					}
				}
			}
			
			if(fixCount && unfixCount)
			{
				fixVel /= fixCount;
				unfixVel /= unfixCount;

				m_fixationIdx = fixVel/unfixVel;

			}
			else
				m_fixationIdx = 0;

		}
		else
			this->m_fixationIdx = 0;
	}
	else
		m_fixationIdx = 0;
}

void CCaloricData::makeSaveFormat(unsigned char* pBuf, int count)
{
	if(!pBuf)
		return;

	memset(pBuf, 0, 1024);



	int offset = 0;
	memset(pBuf+offset, 0, MAX_PATH+1);
	//test file name
	memcpy(pBuf+offset, this->m_strTestFname.GetBuffer(), m_strTestFname.GetLength());
	offset += MAX_PATH+1;
	// peak value
	memcpy(pBuf+offset, &m_peakValue, sizeof(double));
	offset += sizeof(double);
	// peak region
	memcpy(pBuf+offset, m_peakRegion, sizeof(double)*2);
	offset += sizeof(double)*2;
	// bool fixation region
	memcpy(pBuf+offset, &m_bUseFixationBlock, sizeof(bool));
	offset += sizeof(bool);
	// fixation region
	memcpy(pBuf+offset, m_fixationBlock, sizeof(double)*2);
	offset += sizeof(double)*2;
	//fixation index
	memcpy(pBuf+offset, &m_fixationIdx, sizeof(double));
	offset += sizeof(double);

	//manual로 peak 영역을 지정할 지
	memcpy(pBuf+offset, &m_nManualPeakSelect, sizeof(int));
	offset += sizeof(int);


}
void CCaloricData::readSaveFormat(unsigned char* pBuf, int count)
{
	if(!pBuf)
		return;

	int offset = 0;
	//test file name
	m_strTestFname.Format("%s", pBuf);
	offset += MAX_PATH+1;
	//peak value
	memcpy(&m_peakValue, pBuf+offset, sizeof(double));
	offset += sizeof(double);
	// peak region
	memcpy(m_peakRegion, pBuf+offset, sizeof(double)*2);
	offset += sizeof(double)*2;
	// bool fixation retion
	memcpy(&m_bUseFixationBlock, pBuf+offset, sizeof(bool));
	offset +=sizeof(bool);
	// fixation region
	memcpy(m_fixationBlock, pBuf+offset, sizeof(double)*2);
	offset += sizeof(double)*2;
	// fixation index
	memcpy(&m_fixationIdx, pBuf+offset, sizeof(double));
	offset += sizeof(double);
	//manual로 peak region을 지정할지
	memcpy(&m_nManualPeakSelect, pBuf+offset, sizeof(int));
	offset += sizeof(int);

	if(m_peakRegion[0] >= m_peakRegion[1])
		m_peakRegion[1] = m_peakRegion[0]+10;
}


///////////////////////////////////////////////////////////////////////////////////////////
CAnalyzeCaloric::CAnalyzeCaloric(void)
{
	m_nEyeSelect = LEFT;			//left eye or right eye
	
	m_bCompensateSpontaneous = false;
	m_bSpontaneousFromUser = false;
	m_dSpontaneousValue[0] = m_dSpontaneousValue[1] = 0;
//	m_nManualPeakSelect = 0;
}

CAnalyzeCaloric::~CAnalyzeCaloric(void)
{
}

void CAnalyzeCaloric::setTestFileName(CString fnames[4])
{
	for(int i=0; i<4; i++)
		this->m_caloricData[i].m_strTestFname = fnames[i];
}

bool CAnalyzeCaloric::load(CString fname)
{
	bool bRtn = false;

	CFile file;
	if(file.Open(fname, CFile::modeRead))
	{
		int offset = 0;
		unsigned char buf[1024];
		
		//read header
		file.Read(buf, 1024);

		memcpy(&m_nEyeSelect, buf+offset, sizeof(int)); 
		offset += sizeof(int);
		memcpy(&m_bCompensateSpontaneous, buf+offset, sizeof(bool));	// 자발 안진 보상할지
		offset += sizeof(bool);
		memcpy(m_dSpontaneousValue, buf+offset, sizeof(double)*2);		//자발 안진 값
		offset += sizeof(double)*2;
		memcpy(&m_bSpontaneousFromUser, buf+offset, sizeof(bool));
		offset += sizeof(bool);
//		memcpy(&m_nManualPeakSelect, buf+offset, sizeof(int));
//		offset += sizeof(int);
		//보상 test 파일 명
		this->m_strSpontaneousFileName.Format("%s", buf+offset);
		offset += MAX_PATH;
		
		
		//read analyzed data
		for(int i=0; i<4; i++)
		{
			file.Read(buf, 1024);
			this->m_caloricData[i].readSaveFormat(buf, 1024);
		}


		bRtn = true;
	}

	return bRtn;
}
bool CAnalyzeCaloric::save(CString fname)
{
	bool bRtn = false;

	CFile file;
	if(file.Open(fname, CFile::modeCreate | CFile::modeWrite))
	{
		int offset = 0;
		//header
		unsigned char buf[1024];
		memset(buf, 0, 1024);
		memcpy(buf+offset, &m_nEyeSelect, sizeof(int));			//좌추
		offset += sizeof(int);
		memcpy(buf+offset, &m_bCompensateSpontaneous, sizeof(bool));	// 자발 안진 보상할지
		offset += sizeof(bool);
		memcpy(buf+offset, m_dSpontaneousValue, sizeof(double)*2);		//자발 안진 값
		offset += sizeof(double)*2;
		memcpy(buf+offset, &m_bSpontaneousFromUser, sizeof(bool));
		offset += sizeof(bool);
//		memcpy(buf+offset, &m_nManualPeakSelect, sizeof(int));
//		offset += sizeof(int);
		memcpy(buf+offset, m_strSpontaneousFileName.GetBuffer(), m_strSpontaneousFileName.GetLength());
		
		
		file.Write(buf, 1024);

		//분석 결과를 저장한다.
		for(int i=0; i<4; i++)
		{
			m_caloricData[i].makeSaveFormat(buf, 1024);
	
			file.Write(buf, 1024);
		}


		


		bRtn = true;
	}

	return bRtn;
}

void CAnalyzeCaloric::analyze()
{
	// spontaneous eye movement를 보상해야 할 경우
	double spon = 0;
	if(this->m_bCompensateSpontaneous)
		spon = m_dSpontaneousValue[this->m_nEyeSelect];

	for(int i=0; i<4; i++)
	{
		//peak region을 계산한다.
		m_caloricData[i].calcPeakRegion(i, spon);

		//fixation index
		m_caloricData[i].calcFixationIndex(i, spon);
	}

	

	
	double lw = fabs(m_caloricData[LW].m_peakValue + spon);
	double rc = fabs(m_caloricData[RC].m_peakValue + spon);
	double lc = fabs(m_caloricData[LC].m_peakValue + spon);
	double rw = fabs(m_caloricData[RW].m_peakValue + spon);

	//vestibular paresis
	// ((RC+RW)-(LC+LW)) / ((RC+RW)+(LC+LW)) *100 (%)
	if((rc+rw) + (lc+lw))
		this->m_caloricVestibularParesis = ((rc+rw)-(lc+lw)) / ((rc+rw) + (lc+lw));
	else
		m_caloricVestibularParesis = 0;

	// Directional preponderance  
	// ((RC+LW)-(RW+LC)) / ((RC+LW)+(RW+LC)) * 100 (%)
	if((rc+lw)+(rw+lc))
		this->m_caloricDirectionalpreponderance = ((rc+lw)-(rw+lc)) / ((rc+lw)+(rw+lc));
	else
        m_caloricDirectionalpreponderance = 0;

	// Temperature Effect
	// ((RW+LW)-(RC+LC)) / ((RW+LW)+(RC+LC)) *100 (%)
	if((rw+lw)+(rc+lc))
		this->m_caloricTemperatureEffect = ((rw+lw)-(rc+lc)) / ((rw+lw)+(rc+lc));
	else
		m_caloricTemperatureEffect = 0;

}

double CAnalyzeCaloric::getPeakVlue(int test)
{
	if(test>= 4)
		return 0;
	//spontaneous 값으로 보정해 준다.
	double spon = 0;
	if(this->m_bCompensateSpontaneous)
		spon = this->m_dSpontaneousValue[this->m_nEyeSelect];

	return this->m_caloricData[test].m_peakValue + spon;
}


void CAnalyzeCaloric::setPeakValue(int test, double peakVal)
{
	if(test>=4)
		return ;

	double spon = 0;
	if(this->m_bCompensateSpontaneous)
		spon = this->m_dSpontaneousValue[this->m_nEyeSelect];

	this->m_caloricData[test].m_peakValue = peakVal - spon;
}