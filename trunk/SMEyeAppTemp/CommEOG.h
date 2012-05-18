#pragma once

#include "1298Que.h"

#define WM_COMMDRIVE_EOG_READ	(WM_USER+3)
#define BUFFSIZE		4096*512	//4096*256


class CCommEOG
{
public:
	CCommEOG(void);
	~CCommEOG(void);

	HANDLE		m_hComm;				// �����Ʈ ���� �ڵ�
	CString		m_sPortName;			// ��Ʈ �̸� (COM1, ...)
	WORD		m_wDeviceID;			// CCommEOG ��ü�� ������ �϶�, 
										// ������ ���ؼ� WM_COMMDRIVE_READ�� �Բ� ������ �μ�.
	// OVERLAPPED structure contains information used in asynchronous input/output
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch �Լ� Thread �ڵ�

	BOOL	m_bConnected;			// ��Ʈ�� ���ȴ��� ������ ��Ÿ����.
	
	bool startRecord(CString fname);
	void stopRecord();
	CFile m_file;

	

// Implementation
public:
	BOOL	OpenPort(CString sPortName);
	void	ClosePort();

	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);
	// ReadCommByte() �� ReadComm() �ӿ� ���յ�. nToRead == 1 �� ���.

	double* getEOG(){return this->m_1298Que.getEOG();};
	
	unsigned short	m_bufData[128];
	int m_bufDataCount;

	C1298Que m_1298Que;


	unsigned long m_RxEventCount;
};

// Thread�� ����� �Լ�
DWORD	ThreadWatchCommEOG (CCommEOG* pComm);