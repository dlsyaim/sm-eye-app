#pragma once

#define MODE_CALIBRATION	0x01
#define SUBMODE_CALIBRATION_10	0x00
#define SUBMODE_CALIBRATION_15	0x01
#define SUBMODE_CALIBRATION_20	0x02

#define MODE_GAZE_A			0x02
#define SUBMODE_GAZE_A_H_15	0x00
#define SUBMODE_GAZE_A_H_18	0x01
#define SUBMODE_GAZE_A_H_20	0x02
#define SUBMODE_GAZE_A_H_30	0x03
#define SUBMODE_GAZE_A_V_15	0x04
#define SUBMODE_GAZE_A_V_18	0x05
#define SUBMODE_GAZE_A_V_20	0x06
#define SUBMODE_GAZE_A_V_30	0x07

#define MODE_GAZE_B			0x82
#define SUBMODE_GAZE_B_H	0x00
#define SUBMODE_GAZE_B_V	0x01


#define MODE_PURSUIT		0x03

#define MODE_SACCADE_A		0x04
#define SUBMODE_SACCADE_A_H_10	0x00
#define SUBMODE_SACCADE_A_H_15	0x01
#define SUBMODE_SACCADE_A_H_30	0x02
#define SUBMODE_SACCADE_A_V_10	0x03
#define SUBMODE_SACCADE_A_V_15	0x04
#define SUBMODE_SACCADE_A_V_20	0x05

#define MODE_SACCADE_B		0x84
#define SUBMODE_SACCADE_B_H_10	0x00
#define SUBMODE_SACCADE_B_H_15	0x01
#define SUBMODE_SACCADE_B_H_30	0x02
#define SUBMODE_SACCADE_B_V_10	0x03
#define SUBMODE_SACCADE_B_V_15	0x04
#define SUBMODE_SACCADE_B_V_30	0x05


#define MODE_SACCADE_RAND	0x05
#define SUBMODE_SACCADE_RAND_0	0x00
#define SUBMODE_SACCADE_RAND_1	0x01
#define SUBMODE_SACCADE_RAND_2	0x02


#define MODE_OKN			0x06

#define MODE_TEST			0xfb
#define MODE_CENTER_LED		0xfe
#define MODE_BUSY			0xff
#define MODE_STOP			0xff



class CCommLED
{
public:
	CCommLED(void);
	~CCommLED(void);

	bool OpenPort(CString strPortName);
	void ClosePort();
	unsigned int ReadComm(unsigned char *pBuf, unsigned int nToRead);
	unsigned int WriteComm(unsigned char *pBuf, unsigned int nToWrite);

	void setCommand(unsigned char mode, unsigned char submode);
	void setCommandNoStimulation();
	void CCommLED::setComandLEDCenter();
	void setCommandStop();
	void setCommandTest();
	
	void setReadData(unsigned char* pBuf, unsigned int nData);

	double* getStiValue(){return this->m_stiValue;};


	HANDLE		m_hComm;				// �����Ʈ ���� �ڵ�
	CString		m_sPortName;			// ��Ʈ �̸� (COM1, ...)
	bool m_bConnected;

	unsigned char m_blockSize;

	HANDLE		m_hThreadWatchComm;		// Watch �Լ� Thread �ڵ�
	

	double m_stiValue[2];		// sti bar�κ��� �� ��		//H, V
	double m_stiAmplitude;		// OKN, Pursuit������ ��� �ڱ��� �ӵ� Ȥ�� amplitude�� �����ϰ� �ִ�.
	bool m_bStiH;				// OKN, Pursuit������ ���. H �ڱ����� V�ڱ�����
	bool m_bStop;
private:

    // OVERLAPPED structure contains information used in asynchronous input/output
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure

	bool m_bUnderTest;
	int m_nGetFeedback;		//feedback data�� �޾Ҵ���
	unsigned char m_mode, m_subMode;	//������ ���۵� command. 
	

};
