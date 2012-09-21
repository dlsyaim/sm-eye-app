// CommDrive.cpp: implementation of the CCommDrive class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommDrive.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// �޽����� ���� ������ �ڵ��� 
// �θ� �������� HWND hCommWnd = this->m_hWnd;�� �����ȴ�.
extern HWND hCommWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCommDrive::CCommDrive()
{
	// bit for checking the End of Data transferring 
	m_bConnected = FALSE;
	m_bInitSerial = FALSE;
//	m_pOption = NULL;

	m_nMinimumPacketSize = 24;



//	m_iEstimatedDataSize = 1;
	// for test....
//	indexEnter = 0;
}

CCommDrive::~CCommDrive()
{
	unsigned char byte = IP_STOP_CD;
	if(this->m_bConnected)
		this->WriteComm(&byte, 1);
}

void CCommDrive::Create(WORD wDeviceID)
{
	
}

BOOL CCommDrive::OpenPort(CString sPortName)
{
	// local variables
	COMMTIMEOUTS	timeouts;	// set and query the time-out parameters
	DCB				dcb;		// defines the control settings for a serial comm. device
	DWORD			dwThreadID;

//	this->m_pOption = pOption;

	// ------------------------------------------------------------------------------- //
	// initialize the OVERLAPPED structure parameters
	//		OVERLAPPED structure contains information used in asynchronous input and output
	// ------------------------------------------------------------------------------- //
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	if(!(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		return FALSE;
	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if(!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		return FALSE;

	// ------------------------------------------------------------------------------- //
	// create a comm. resource (serial port: COM1 or COM2, ...) (�����Ʈ ������ ����)
	// ------------------------------------------------------------------------------- //
	m_sPortName = sPortName;
	m_hComm = CreateFile(
		m_sPortName,					// the name of the object to create or open
		GENERIC_READ | GENERIC_WRITE,	// specify read/write operation to the object
		0,								// the object can not be shared and can not be opened again until the handle is closed
		NULL,							// the handle can not be inherited
		OPEN_EXISTING,					// opens the file, ����̽� ������ �̹� �����ϰ� �ִ�. (�� COM1)
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// specify file attributes and flags, return ERROR_IO_PENDING flag
		NULL);

	if(m_hComm == (HANDLE)-1)
		return FALSE;

	// ��Ʈ ���� ����

	// ------------------------------------------------------------------------------- //
	// specifies a set of events to be monitored for a comm. device
	//		EV_RXCHAR: a character was received and placed in the input buffer
	// ------------------------------------------------------------------------------- //
	SetCommMask(m_hComm, EV_RXCHAR);	
	
	// ------------------------------------------------------------------------------- //
	// initialize the size, in BYTES, of input buffer and output buffer
	//		InQueue, OutQueue ũ�� ����.(4096 �ʹ� ū�� �ƴѰ�.???)
	// ------------------------------------------------------------------------------- //
	SetupComm(m_hComm, BUFFSIZE, BUFFSIZE);

	// ------------------------------------------------------------------------------- //
	// discards all characters from the output or input buffer of a specified comm. resource
	// ------------------------------------------------------------------------------- //
	if(PurgeComm(m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR))
		;//	AfxMessageBox(_T("_____________PURGE COMM ERROR____________"));

	// ------------------------------------------------------------------------------- //
	// set and query the timeout parameters for a comm. device : timeout ���� (setup again !!!)
	//	 the parameter determines the behavior of ReadFile, WriteFile, ... operation on the device
	// ------------------------------------------------------------------------------- //
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;	// maximum time allowd to elapse between the arrival of two characters
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0; //2*CBR_9600 / (m_dlgComOpt.GetBaudRates());
	timeouts.WriteTotalTimeoutConstant = 0;

	SetCommTimeouts(m_hComm, &timeouts);	// sets the timeout parameters for all read and write operation

	// ------------------------------------------------------------------------------- //
	// DCB (Device Control Block) : defines the control settings for serial communications device
	// ------------------------------------------------------------------------------- //
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hComm, &dcb);	// ���� ���� �д´�.

	// �Ʒ� ������ H/W�� ���� �޶��� ���� �ִ�. 
	dcb.BaudRate = CBR_115200;
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;	//DTR_CONTROL_ENABLE;
	dcb.fDsrSensitivity = FALSE;			// xxx
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;				// Xon, Xoff control character�� ����Ѵ�.
	dcb.fErrorChar = FALSE;
	dcb.fNull = FALSE;				// discard NULL bytes received
	dcb.fRtsControl = RTS_CONTROL_ENABLE;	//RTS_CONTROL_ENABLE;
	dcb.fAbortOnError = TRUE;
	dcb.XonLim = 0;
	dcb.XoffLim = 0;
	dcb.ByteSize = 8;
	dcb.Parity   = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	// ------------------------------------------------------------------------------- //
	// configures a comm. device according to the spec. in a DCB
	// ------------------------------------------------------------------------------- //
	if(!(SetCommState(m_hComm, &dcb))) 
	{
		CloseHandle(m_hComm);
		return FALSE;		
	}

	// ------------------------------------------------------------------------------- //
	// ��Ʈ ���� Thread ����
	// ------------------------------------------------------------------------------- //
	m_hThreadWatchComm = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadWatchComm, this, 0, &dwThreadID);

	// BVA1.1���� �Ʒ��� �ִ�.
	// ::SetThreadPriority(m_hComm, THREAD_PRIORITY_HIGHEST);

	if(!m_hThreadWatchComm)
	{
		ClosePort();
		return FALSE;
	}
	
	m_bConnected = TRUE;
//	m_bEndOfData = FALSE;	// ���⿡���� ������� �ʴ��� Ȯ�� �ʿ�. XPOD�� ��� �ʿ�
	TRACE("...... CCommDrive OpenPort(): ThreadWatchComm Process�� �����Ͽ��� ......\n");

	return TRUE;
}


// ��Ʈ�� �ݴ´�.
void CCommDrive::ClosePort()
{
	if(this->m_bConnected)
	{
		TRACE("...... Communication Port is closed now !!!\n");

		
		
		TRACE("...... Communication Port is closed now !!!\n");

		SetCommMask(m_hComm, 0);	// 0 : disables all events
		PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
		CloseHandle(m_hComm);

		m_bConnected = FALSE;
	}
}


void CCommDrive::clearCommBuf()
{
	PurgeComm(m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
}

// ��Ʈ�� pBuff�� ������ nToWrite��ŭ ����.
// ������ ������ Byte���� �����Ѵ�.
// nToWrite = 1 (byte)
DWORD CCommDrive::WriteComm (BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags, dwNum;
	COMSTAT	comstat;

	if(!WriteFile(m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	{
		dwNum = GetLastError();
		if(GetLastError() == ERROR_IO_PENDING)
		{
			// ���� ���ڰ� ���� �ְų� ������ ���ڰ� ���� ���� ��� Overlapped IO��
			// Ư���� ���� ERROR_IO_PENDING ���� �޽����� ���޵ȴ�.
			// timeouts���� ������ �ð���ŭ ��ٷ��ش�.
			while(!GetOverlappedResult (m_hComm, &m_osWrite, &dwWritten, TRUE))
			{
				dwError = GetLastError();
				if(dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError(m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError(m_hComm, &dwErrorFlags, &comstat);
		}
		//TRACE("... WriteComm() failed: Error Code = %d !!!\n", dwNum);
	}
	else
	{
		//TRACE("...... WriteComm() sucess !!!\n");
	}
	//CString str;
	//str.Format("WriteComm (1) : dwNum=%d, nToWrite=%d, dwWritten = %d", dwNum, nToWrite, dwWritten);
	//AfxMessageBox(str    +   "WriteComm is successful!!!");
	return dwWritten;
}

// nToRead==1�̸� ReadCommByte()�� ����� ������.

DWORD CCommDrive::ReadComm (BYTE *pBuff, DWORD nToRead)
// ------------------------------------------------------------------------------- //
// ��Ʈ�κ��� pBuff�� nToRead��ŭ �д´�.
// ������ ������ Byte���� �����Ѵ�.
// ------------------------------------------------------------------------------- //
{
	DWORD dwRead, dwError, dwErrorFlags;
	DWORD dwInBuff;
	COMSTAT comstat;		// contains information about a comm. device
							// filled by ClearCommError function
	DWORD	dw = GetLastError();
	CString str;

	// ------------------------------------------------------------------------------- //
	// ClearCommError: retrieves information about a communications error and 
	//		reports the current status of a communications device
	// ------------------------------------------------------------------------------- //
	if(!ClearCommError(m_hComm, &dwErrorFlags, &comstat))
	{
		str.Format(_T("ErrorFlags= %u"), dw);
	 	//AfxMessageBox(str + _T("___________________ClearCommError ()_______________ is failed!!!"));
	}

	// check the number of bytes received by serial provider but not yet read by a ReadFile operation
	dwInBuff = comstat.cbInQue;

	//TRACE("%d", dwInBuff);

	// input buffer�� m_iBlockSize byte��ŭ�� �����Ͱ� ���̸� m_iBlockSize ũ���� byte�� �д´�.
	
	if(dwInBuff >= this->m_nMinimumPacketSize)
	//if(dwInBuff >= 24)
	{
		nToRead = nToRead-nToRead%12;	//¦�����θ� �о����.
		if(!ReadFile(m_hComm, pBuff, nToRead, &dwRead, &m_osRead))
		{
			if(GetLastError() == ERROR_IO_PENDING)
			{
				//AfxMessageBox(_T("--------------IO PENDING ERROR !!!"));
				// timeouts���� ������ �ð���ŭ ��ٷ��ش�.
				while(!GetOverlappedResult (m_hComm, &m_osRead, &dwRead, TRUE))
				{
					dwError = GetLastError();
					if(dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError (m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError (m_hComm, &dwErrorFlags, &comstat);
			}
		}
		// specifies a file position at which to start the transfer
		// the file position is a byte offset from the start of the file
		/*if(nToRead == 1)
			m_osRead.Offset = (m_osRead.Offset+1)%BUFFSIZE;
		else
			m_osRead.Offset = (m_osRead.Offset+m_iBlockSize)%BUFFSIZE;*/
		//str.Format("Bytes read = <%d>, osRead.Offset=%d..............", dwRead, m_osRead.Offset);
		//str.Format("________pBuff[0]=%d, %d, %d, %d, %d, %d, %d, %d", 
		//	pBuff[0], pBuff[1], pBuff[2], pBuff[3], pBuff[4], pBuff[5], pBuff[6], pBuff[7]);
		//AfxMessageBox(str);

        if(nToRead != dwRead)
		{
			str.Format(_T(" [%d] bytes could not be read !!!!"), nToRead-dwRead);
			//AfxMessageBox(str);
		}
	}
	else
	{
		dwRead = 0;
	}

	return dwRead;
}

/*unsigned short* CCommDrive::GetRealDataArray()
{
	return m_usData;
}*/



//*********************************************************************
//
//						Thread Watch Comm �Լ�
//		
//---------------------------------------------------------------------

DWORD ThreadWatchComm(CCommDrive* pComm)
// ------------------------------------------------------------------------------- //
//��Ʈ�� �����ϰ�, ���� ������ ������ m_ReadData�� ������ �ڿ�
// MainWnd�� �޽����� ������ Buffer�� ������ �о� ����� �˷��ش�.
// ------------------------------------------------------------------------------- //
{
	DWORD		dwEvent;
	OVERLAPPED	os;
	BOOL		bOk = TRUE;
	BYTE		buff[4096*4];		// �б� ����
	DWORD		dwRead;			// ���� ����Ʈ ��
	// DWORD		dwError, dwErrorFlags;
	// COMSTAT		comstat;
//	int			blocksize = pComm->m_iBlockSize;

	CString str;

	// Event, OS ����
	memset(&os, 0, sizeof(OVERLAPPED));
	//if (!(os.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL)))
	if(!(os.hEvent = CreateEvent (NULL, TRUE, TRUE, NULL)))
	{
		bOk = FALSE;
	}
	if(!SetCommMask (pComm->m_hComm, EV_RXCHAR))
	{
		//TRACE(" ��� ����???\n");
		bOk = FALSE;
	}
	if(!bOk)
	{
		TRACE(_T("------- Error while creating ThreadWatchComm, ") + pComm->m_sPortName);
		return FALSE;
	}

	TRACE(_T("...... ThreadWatchComm entered !!! \n"));

	// ��Ʈ�� �����ϴ� ����.
	while(pComm->m_bConnected)
	{

			//TRACE("......TRUE - -  Watching communication port !!!\n");
			dwEvent = 0;
			// waits for an event to occur for a specified communications device
			WaitCommEvent(pComm->m_hComm, &dwEvent, NULL);
			
		
			if((dwEvent & EV_RXCHAR) == EV_RXCHAR)
			{
				
				memset(buff, NULL, 4096*4);
				dwRead = pComm->ReadComm(buff, 4096*4);
				if(dwRead)
				{
					pComm->m_que.addRawData(buff, dwRead);
				}

			}
			else
			{
				// AfxMessageBox("___________WaitCommEvent_ERROR___________");
			}
//		}
	}		// while(pComm->m_bConnected)
	// ��Ʈ�� ClosePort�� ���� ������ m_bConnected�� FALSE�� �Ǿ� ����...
	if(!ResetEvent(os.hEvent))
	{
		TRACE("ThreadWatchComm() ............. ResetHandle Error !!!\n");
	}
	CloseHandle(os.hEvent);
	pComm->m_hThreadWatchComm = NULL;

	TRACE("............. End of ThreadWatchComm() ..................\n");
	return TRUE;
}


void CCommDrive::start(CString fname)
{
	if(m_bConnected)
	{
		unsigned char byte = IP_START_CD;
		this->WriteComm(&byte, 1);
		this->WriteComm(&byte, 1);
		this->WriteComm(&byte, 1);


		if(m_file.m_hFile != CFile::hFileNull)
			m_file.Close();
		m_strFname = fname;
		m_file.Open(fname, CFile::modeCreate | CFile::modeWrite);
	}

}
void CCommDrive::stop()
{
	if(this->m_bConnected)
	{
		unsigned char byte = IP_STOP_CD;
		this->WriteComm(&byte, 1);
		this->WriteComm(&byte, 1);
		this->WriteComm(&byte, 1);
	}

	if(m_file.m_hFile != CFile::hFileNull)
		m_file.Close();
}
