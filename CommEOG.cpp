#include "StdAfx.h"
#include ".\commeog.h"

extern HWND hCommWnd;
CCommEOG::CCommEOG(void)
{
	m_bConnected = FALSE;
}

CCommEOG::~CCommEOG(void)
{
}


BOOL CCommEOG::OpenPort(CString sPortName)
{
	if(this->m_wDeviceID == 1)
		int k = 0;

	m_RxEventCount = 0;


	// local variables
	COMMTIMEOUTS	timeouts;	// set and query the time-out parameters
	DCB				dcb;		// defines the control settings for a serial comm. device
	DWORD			dwThreadID;

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
	// create a comm. resource (serial port: COM1 or COM2, ...) (통신포트 파일을 연다)
	// ------------------------------------------------------------------------------- //
	m_sPortName = sPortName;
	m_hComm = CreateFile(
		m_sPortName,					// the name of the object to create or open
		GENERIC_READ | GENERIC_WRITE,	// specify read/write operation to the object
		0,								// the object can not be shared and can not be opened again until the handle is closed
		NULL,							// the handle can not be inherited
		OPEN_EXISTING,					// opens the file, 디바이스 파일은 이미 존재하고 있다. (예 COM1)
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// specify file attributes and flags, return ERROR_IO_PENDING flag
		NULL);

	if(m_hComm == (HANDLE)-1)
		return FALSE;

	// 포트 상태 설정

	// ------------------------------------------------------------------------------- //
	// specifies a set of events to be monitored for a comm. device
	//		EV_RXCHAR: a character was received and placed in the input buffer
	// ------------------------------------------------------------------------------- //
	SetCommMask(m_hComm, EV_RXCHAR);	
	
	// ------------------------------------------------------------------------------- //
	// initialize the size, in BYTES, of input buffer and output buffer
	//		InQueue, OutQueue 크기 설정.(4096 너무 큰것 아닌감.???)
	// ------------------------------------------------------------------------------- //
	SetupComm(m_hComm, BUFFSIZE, BUFFSIZE);

	// ------------------------------------------------------------------------------- //
	// discards all characters from the output or input buffer of a specified comm. resource
	// ------------------------------------------------------------------------------- //
	if(!PurgeComm(m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR))
		AfxMessageBox("_____________PURGE COMM ERROR____________");

	// ------------------------------------------------------------------------------- //
	// set and query the timeout parameters for a comm. device : timeout 설정 (setup again !!!)
	//	 the parameter determines the behavior of ReadFile, WriteFile, ... operation on the device
	// ------------------------------------------------------------------------------- //
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;	// maximum time allowd to elapse between the arrival of two characters
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 1000;
	timeouts.WriteTotalTimeoutMultiplier = 0; //2*CBR_9600 / (m_dlgComOpt.GetBaudRates());
	timeouts.WriteTotalTimeoutConstant = 1000;

	SetCommTimeouts(m_hComm, &timeouts);	// sets the timeout parameters for all read and write operation

	// ------------------------------------------------------------------------------- //
	// DCB (Device Control Block) : defines the control settings for serial communications device
	// ------------------------------------------------------------------------------- //
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hComm, &dcb);	// 현재 값을 읽는다.

	// 아래 설정은 H/W에 따라 달라질 수도 있다. 
	dcb.BaudRate = CBR_115200;
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;	//DTR_CONTROL_ENABLE;
	dcb.fDsrSensitivity = FALSE;			// xxx
	dcb.fTXContinueOnXoff = FALSE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;				// Xon, Xoff control character를 사용한다.
	dcb.fErrorChar = FALSE;
	dcb.fNull = FALSE;				
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
		return FALSE;

	// ------------------------------------------------------------------------------- //
	// 포트 감시 Thread 생성
	// ------------------------------------------------------------------------------- //
	m_hThreadWatchComm = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadWatchCommEOG, this, 0, &dwThreadID);

	// BVA1.1에는 아래가 있다.
	// ::SetThreadPriority(m_hComm, THREAD_PRIORITY_HIGHEST);

	if(!m_hThreadWatchComm)
	{
		ClosePort();
		return FALSE;
	}
	
	m_bConnected = TRUE;
	//TRACE("...... CCommEOG OpenPort(): ThreadWatchComm Process를 생성하였음 ......\n");

	return TRUE;
}


// 포트를 닫는다.
void CCommEOG::ClosePort()
{
	if(this->m_bConnected)
	{
		TRACE("...... Communication Port is closed now !!!\n");

		SetCommMask(m_hComm, 0);	// 0 : disables all events
		PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
		CloseHandle(m_hComm);

		m_bConnected = FALSE;
	}
}

// 포트에 pBuff의 내용을 nToWrite만큼 쓴다.
// 실제로 쓰여진 Byte수를 리턴한다.
// nToWrite = 1 (byte)
DWORD CCommEOG::WriteComm (BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags, dwNum;
	COMSTAT	comstat;

	if(!WriteFile(m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	{
		dwNum = GetLastError();
		if(GetLastError() == ERROR_IO_PENDING)
		{
			// 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우 Overlapped IO의
			// 특성에 따라 ERROR_IO_PENDING 에러 메시지가 전달된다.
			// timeouts에서 설정한 시간만큼 기다려준다.
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

// nToRead==1이면 ReadCommByte()의 기능을 수행함.

DWORD CCommEOG::ReadComm (BYTE *pBuff, DWORD bufCount)
// ------------------------------------------------------------------------------- //
// 포트로부터 pBuff에 nToRead만큼 읽는다.
// 실제로 읽혀진 Byte수를 리턴한다.
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
		str.Format("ErrorFlags= %u", dw);
	 	AfxMessageBox(str + "___________________ClearCommError ()_______________ is failed!!!");
	}

	// check the number of bytes received by serial provider but not yet read by a ReadFile operation
	dwInBuff = comstat.cbInQue;
   
	
	dwRead = 0;

	if(dwInBuff >= bufCount)
	{
		if(!ReadFile(m_hComm, pBuff, bufCount, &dwRead, &m_osRead))	//count가 0이면 readfile은 실행되지 않는다
		{
			if(GetLastError() == ERROR_IO_PENDING)
			{
				AfxMessageBox("--------------IO PENDING ERROR !!!");
				// timeouts에서 정해준 시간만큼 기다려준다.
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

			// specifies a file position at which to start the transfer
			// the file position is a byte offset from the start of the file
		}

	}


	

	return dwRead;
}




bool CCommEOG::startRecord(CString fname)
{
	bool bRtn;

	if(m_file.m_hFile != CFile::hFileNull)
		m_file.Close();

	bRtn = m_file.Open(fname, CFile::modeWrite | CFile::modeCreate);

	return bRtn;

}
void CCommEOG::stopRecord()
{
	if(m_file.m_hFile != CFile::hFileNull)
		m_file.Close();
}








//**********************************************************************************
// CCommThread::ConvertData()에 해당
// 만약 XPOD-M100과 PBM-B080에 따라 달라지는 절차가 있다면, 여기에서 m_wDeviceID를
// 구별하여 그 작업을 수행하면 된다.
// 현재로서는 구별되는 절차가 없으므로, m_wDeviceID 구분없이 동일한 절차를 따른다.


//*********************************************************************
//
//						Thread Watch Comm 함수
//		
//---------------------------------------------------------------------

DWORD ThreadWatchCommEOG(CCommEOG* pComm)
// ------------------------------------------------------------------------------- //
//포트를 감시하고, 읽힌 내용이 있으면 m_ReadData에 저장한 뒤에
// MainWnd에 메시지를 보내어 Buffer의 내용을 읽어 가라고 알려준다.
// ------------------------------------------------------------------------------- //
{
	DWORD		dwEvent;
	OVERLAPPED	os;
	BOOL		bOk = TRUE;
	BYTE		buff[2048];		// 읽기 버퍼
	DWORD		dwRead;			// 읽은 바이트 수
	// DWORD		dwError, dwErrorFlags;
	// COMSTAT		comstat;
	

	CString str;

	// Event, OS 설정
	memset(&os, 0, sizeof(OVERLAPPED));
	//if (!(os.hEvent = CreateEvent (NULL, TRUE, FALSE, NULL)))
	if(!(os.hEvent = CreateEvent (NULL, TRUE, TRUE, NULL)))
	{
		bOk = FALSE;
	}
	if(!SetCommMask (pComm->m_hComm, EV_RXCHAR))
	{
		//TRACE(" 요기 맞지???\n");
		bOk = FALSE;
	}
	if(!bOk)
	{
		TRACE("------- Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		return FALSE;
	}

	TRACE("...... ThreadWatchComm entered !!! \n");

	// 포트를 감시하는 루프.
	while(pComm->m_bConnected)
	{
			dwEvent = 0;
			// waits for an event to occur for a specified communications device
			WaitCommEvent(pComm->m_hComm, &dwEvent, NULL);
			
		
			if((dwEvent & EV_RXCHAR) == EV_RXCHAR)
			{
				memset(buff, 0, 2048);
				dwRead = pComm->ReadComm(buff, 12*4);	//1sample당 12byte, 500Hz->125Hz
				//if(dwRead)
					//TRACE("%d number of EOG data received \r\n", dwRead);
				if(dwRead == 12*4)
				{
					pComm->m_1298Que.addRawData(buff, 12*4);
					
					if(pComm->m_file.m_hFile != CFile::hFileNull)
						pComm->m_file.Write(buff, 12*4);

					
					//SendMessage(hCommWnd, WM_COMMDRIVE_EOG_READ, (unsigned int)&(pComm->m_wDeviceID), 0);
					
				}
				
				
				/*pComm->m_RxEventCount++;


				if(pComm->m_RxEventCount%100 == 0)
					TRACE("id: %d, Rx event Count: %d \r\n", pComm->m_wDeviceID, pComm->m_RxEventCount);

				memset(buff, NULL, 2048);
				dwRead = pComm->ReadComm(buff, 2048);
				if(dwRead >= (DWORD)blocksize)
				{
					//pComm->SetReadData(buff, dwRead); 
					int setCount = 0;
					int blocKCount = dwRead/BLOCK_SIZE;
					
					for(int i=0; i<blocKCount; i++)
					{
						if((pComm->m_ucDummyDelay = pComm->makeECGFromRaw(buff + i*BLOCK_SIZE, pComm->m_bufData+setCount*8))== 0)
						{
							setCount++;
							pComm->m_bufDataCount = setCount*4;
							SendMessage(hCommWnd, WM_COMMDRIVE_READ, (unsigned int)&(pComm->m_wDeviceID), 0);
						}
						else
						{
							//data format이 틀리다!
							
							

							TRACE("id: %d delay: %d, Data Protocole Check Fail!!\r\n", pComm->m_wDeviceID, pComm->m_ucDummyDelay);
						}
					}
					
				}
				else
				{
					str.Format("dwRead = %d", dwRead);
					//AfxMessageBox("..........Data Read = "+str);
				}*/
			}
			else
			{
				// AfxMessageBox("___________WaitCommEvent_ERROR___________");
			}
//		}
	}		// while(pComm->m_bConnected)
	// 포트가 ClosePort에 의해 닫히면 m_bConnected가 FALSE가 되어 종료...
	if(!ResetEvent(os.hEvent))
	{
		TRACE("ThreadWatchComm() ............. ResetHandle Error !!!\n");
	}
	CloseHandle(os.hEvent);
	pComm->m_hThreadWatchComm = NULL;

	TRACE("............. End of ThreadWatchComm() ..................\n");
	return TRUE;
}

