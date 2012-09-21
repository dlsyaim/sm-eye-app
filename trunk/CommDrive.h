// CommDrive.h: interface for the CCommDrive class.
//
//******************************
//
//	��� Ŭ����	CCommDrive : PBM-N100 �� XPOD-M100 ����� ���� Ŭ����
//		CCommThread�� ä�� ������ ����������ݿ� ���̰� �ִ�.
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

	
	HANDLE		m_hComm;				// �����Ʈ ���� �ڵ�
	CString		m_sPortName;			// ��Ʈ �̸� (COM1, ...)
	
										// ������ ���ؼ� WM_COMMDRIVE_READ�� �Բ� ������ �μ�.
	// OVERLAPPED structure contains information used in asynchronous input/output
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch �Լ� Thread �ڵ�

	BOOL	m_bConnected;			// ��Ʈ�� ���ȴ��� ������ ��Ÿ����.
	bool	m_bInitSerial;
	BOOL	m_bUploadFlag;			// XPOD-M100�� ���, maxsaveblock info.�� ���� ���� flag

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

// Thread�� ����� �Լ�
DWORD	ThreadWatchComm (CCommDrive* pComm);

#endif // !defined(AFX_COMMDRIVE_H__89735F7C_0161_4157_8A5C_343576B92068__INCLUDED_)
