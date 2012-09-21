#pragma once

#include "1298Que.h"

#define WM_COMMDRIVE_EOG_READ	(WM_USER+3)
#define BUFFSIZE		4096*512	//4096*256


class CCommEOG
{
public:
	CCommEOG(void);
	~CCommEOG(void);

	HANDLE		m_hComm;				// 통신포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1, ...)
	WORD		m_wDeviceID;			// CCommEOG 객체가 여러개 일때, 
										// 구별을 위해서 WM_COMMDRIVE_READ와 함께 보내는 인수.
	// OVERLAPPED structure contains information used in asynchronous input/output
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch 함수 Thread 핸들

	BOOL	m_bConnected;			// 포트가 열렸는지 유무를 나타낸다.
	
	bool startRecord(CString fname);
	void stopRecord();
	CFile m_file;

	

// Implementation
public:
	BOOL	OpenPort(CString sPortName);
	void	ClosePort();

	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);
	// ReadCommByte() 는 ReadComm() 속에 통합됨. nToRead == 1 인 경우.

	double* getEOG(){return this->m_1298Que.getEOG();};
	
	unsigned short	m_bufData[128];
	int m_bufDataCount;

	C1298Que m_1298Que;


	unsigned long m_RxEventCount;
};

// Thread로 사용할 함수
DWORD	ThreadWatchCommEOG (CCommEOG* pComm);