// CommDrive.h: interface for the CCommDrive class.
//
//******************************
//
//	통신 클래스	CCommDrive : PBM-N100 과 XPOD-M100 모듈을 위한 클래스
//		CCommThread와 채널 설정과 통신프로토콜에 차이가 있다.
//
//******************************
#if !defined(AFX_COMMDRIVE_H__89735F7C_0161_4157_8A5C_343576B92068__INCLUDED_)
#define AFX_COMMDRIVE_H__89735F7C_0161_4157_8A5C_343576B92068__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define WM_COMMDRIVE_READ	(WM_USER+2)



#define BUFFSIZE		4096*512	//4096*256
//#define NUM				100			// Order of numerator in the transfer function for IIR filters


// PBM-N100, PBM-B080 interface protocol
#define	IP_START_CD		0xAA		// transmit biosignals
#define	IP_STOP_CD		0xBB		// break transmission

// XPOD-M100: Hobak specific interface protocol

#include "pbmQue.h"


class CCommDrive  
{
public:
	CCommDrive();
	virtual ~CCommDrive();

	
	HANDLE		m_hComm;				// 통신포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1, ...)
	
										// 구별을 위해서 WM_COMMDRIVE_READ와 함께 보내는 인수.
	// OVERLAPPED structure contains information used in asynchronous input/output
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch 함수 Thread 핸들

	BOOL	m_bConnected;			// 포트가 열렸는지 유무를 나타낸다.
	bool	m_bInitSerial;
	BOOL	m_bUploadFlag;			// XPOD-M100의 경우, maxsaveblock info.에 따른 종료 flag

	int	m_nMinimumPacketSize;
	
	CFile m_file;
	CString m_strFname;
	

public:
	CpbmQue m_que;
	//int m_iEstimatedDataSize;
	void	Create(WORD wPortID);
	BOOL	OpenPort(CString sPortName);
	void	ClosePort();
	void	clearCommBuf();

	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);

	double* getCurrentValue(){return this->m_que.getCurrentValue();};

	void stop();
	void start(CString fname);
	
};

// Thread로 사용할 함수
DWORD	ThreadWatchComm (CCommDrive* pComm);

#endif // !defined(AFX_COMMDRIVE_H__89735F7C_0161_4157_8A5C_343576B92068__INCLUDED_)
