#include "StdAfx.h"
#include ".\commled.h"
#include "ed_testprotocole.h"

#define BUFFSIZE		4096

CCommLED::CCommLED(void)
: m_bConnected(false)
, m_bUnderTest(false)
, m_hComm(NULL)
, m_hThreadWatchComm(NULL)
, m_nGetFeedback(-1)
{
	m_blockSize = 3;



	m_mode = m_subMode = 0;
	m_stiValue[0] = m_stiValue[1] = 0;


	m_stiAmplitude = 0;
	m_bStop = false;
	m_bStiH = true;
}

CCommLED::~CCommLED(void)
{
	if(this->m_bConnected)
		this->ClosePort();
}

DWORD	ThreadLEDComm(CCommLED* pComm);

bool CCommLED::OpenPort(CString strPortNum)
{
	bool bRtn;

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
		return false;
	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if(!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		return false;

	// ------------------------------------------------------------------------------- //
	// create a comm. resource (serial port: COM1 or COM2, ...) (�����Ʈ ������ ����)
	// ------------------------------------------------------------------------------- //
	m_sPortName = strPortNum;
	//m_sPortName.Format("COM%d",nPortNum);
	m_hComm = CreateFile(
		m_sPortName,					// the name of the object to create or open
		GENERIC_READ | GENERIC_WRITE,	// specify read/write operation to the object
		0,								// the object can not be shared and can not be opened again until the handle is closed
		NULL,							// the handle can not be inherited
		OPEN_EXISTING,					// opens the file, ����̽� ������ �̹� �����ϰ� �ִ�. (�� COM1)
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// specify file attributes and flags, return ERROR_IO_PENDING flag
		NULL);

	if(m_hComm == (HANDLE)-1)
		return false;

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
	if(!PurgeComm(m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR))
		AfxMessageBox("_____________PURGE COMM ERROR____________");

	// ------------------------------------------------------------------------------- //
	// set and query the timeout parameters for a comm. device : timeout ���� (setup again !!!)
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
	GetCommState(m_hComm, &dcb);	// ���� ���� �д´�.

	// �Ʒ� ������ H/W�� ���� �޶��� ���� �ִ�. 
	dcb.BaudRate = CBR_19200;
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
	dcb.fNull = FALSE;				// DON'T discard NULL bytes received
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
		return false;

	// ------------------------------------------------------------------------------- //
	// ��Ʈ ���� Thread ����
	// ------------------------------------------------------------------------------- //
	m_hThreadWatchComm = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)ThreadLEDComm, this, 0, &dwThreadID);

	// BVA1.1���� �Ʒ��� �ִ�.
	// ::SetThreadPriority(m_hComm, THREAD_PRIORITY_HIGHEST);

	if(!m_hThreadWatchComm)
	{
		ClosePort();
		return false;
	}
	
	bRtn = m_bConnected = true;
	TRACE("...... CCommDrive OpenPort(): ThreadWatchComm Process�� �����Ͽ��� ......\n");

	

	return bRtn;
}
void CCommLED::ClosePort()
{
	SetCommMask(m_hComm, 0);	// 0 : disables all events
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	CloseHandle(m_hComm);

	m_bConnected = false;
}

unsigned int CCommLED::ReadComm(unsigned char *pBuf, unsigned int nToRead)
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

	//TRACE("%d", dwInBuff);

	if(dwInBuff >= this->m_blockSize)
	{
		if(!ReadFile(m_hComm, pBuf, nToRead, &dwRead, &m_osRead))
		{
			if(GetLastError() == ERROR_IO_PENDING)
			{
				AfxMessageBox("--------------IO PENDING ERROR !!!");
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
		m_osRead.Offset = (m_osRead.Offset+dwRead)%BUFFSIZE;
	}
	else
		dwRead = 0;
	
    
	

	return dwRead;
}

void CCommLED::setCommandNoStimulation()
{
	m_mode = 0xfe;
	m_subMode = 0x00;

	this->setCommand(m_mode, m_subMode);

    
	this->m_stiValue[0] = m_stiValue[1] = 0;
}

void CCommLED::setComandLEDCenter()
{
	m_mode = 0xfe;
	m_subMode = 0x01;
	this->setCommand(m_mode, m_subMode);
	this->m_stiValue[0] = m_stiValue[1] = 0;
}

void CCommLED::setCommandTest()
{
	m_mode = 0xfb;
	m_subMode = 0x01;

	this->setCommand(m_mode, m_subMode);
	
}

void CCommLED::setCommandStop()
{
	m_mode = 0xff;
	m_subMode = 0xff;

	this->setCommand(m_mode, m_subMode);

		
	this->m_stiValue[0] = m_stiValue[1] = 0;
}

void CCommLED::setCommand(unsigned char mode, unsigned char submode)
{
	if(mode == 0xa1)	//caloric�� ��� �ƹ��͵� ���� ����.
		return;

	unsigned char buf[3] = {0xAA, mode, submode};
	m_mode = mode;
	m_subMode = submode;
	m_nGetFeedback = -1;	//wait status

	// OKN or Pursuit�� ��쿡�� ���
	if((mode == 0x06) || (mode == 0x03))
	{
		m_stiAmplitude = ED_TestProtocole::getStiValue(mode, submode);
		m_bStiH = ED_TestProtocole::getStiDirection(mode, submode);

	}
	else
		m_stiAmplitude = 0;

	this->m_blockSize = 3;

	if(mode == 0xff)	//stop ����� ���
        m_bStop = true;
	else
		m_bStop = false;

	this->WriteComm(buf, 3);

}



unsigned int CCommLED::WriteComm(unsigned char *pBuf, unsigned int nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags, dwNum;
	COMSTAT	comstat;

	if(!WriteFile(m_hComm, pBuf, nToWrite, &dwWritten, &m_osWrite))
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



void CCommLED::setReadData(unsigned char* pBuf, unsigned int nData)
{
	unsigned int count = 0;
	
	TRACE("got data \n");

	while(count < nData)
	{
        if(pBuf[count] == 0xAA)		//feedback ��ȣ
		{
			//m_bGetFeedback�� ��ȣ���ޱ� ������ ���(-1)
			//������ 0(����) Ȥ�� 1(����)

			//���´� ��ȣ�� feedback��ȣ�� ������ Ȯ��
			if ((m_mode == pBuf[count+1]) && (m_subMode == pBuf[count+2]))
			{
				this->m_nGetFeedback = 1;
				//Sync ��ȣ�� �ޱ� ���� blocksize�� 1�� �ٲ۴�.
				this->m_blockSize = 1;
			}
			else if((pBuf[count+1] == 0xff) && (pBuf[count+2] == 0xff))	//stop��ȣ�� ��
				this->m_bStop = true;
			else
				this->m_nGetFeedback = 0;

			count += 3;
		}
		else
		{
			//Sync data
			// bit7 : 0 = X��, 1 = Y��
			// bit6 : 0 = Left, 1 = Right
			// bit0~5 : degree (0~30)
			int pn = (pBuf[count] & 0x40) ? 1 : -1;	//�¿�
			TRACE("pBuf[count]: %d, pn: %d\r\n", pBuf[count], pn);
			int degree = pBuf[count] & 0x3f;		//degree

			if(m_mode == ED_TestProtocole::getStiModeCommand(STI_OKN))
			{
				m_stiValue[0] = m_stiValue[1] = 0;
				//pn = (pBuf[count] & 0x40) ? 1 : -1;
				pn = pBuf[count] ? -1 :1;
				if(this->m_bStiH)
					this->m_stiValue[0] = pn*this->m_stiAmplitude;
				else
					this->m_stiValue[1] = pn*this->m_stiAmplitude;
				

			}
			else if(m_mode == ED_TestProtocole::getStiModeCommand(STI_PURSUIT))
			{
				m_stiValue[0] = m_stiValue[1] = 0;
				pn = (pBuf[count] & 0x40) ? 1 : -1;
				
				if(this->m_bStiH)
					this->m_stiValue[0] = pn*this->m_stiAmplitude;
				else
					this->m_stiValue[1] = pn*this->m_stiAmplitude;

				TRACE("sti value is %f, %f", m_stiValue[0], m_stiValue[1]);

			}
			else if(m_mode == ED_TestProtocole::getStiModeCommand(STI_LED))
			{
			}
			else
			{
				// sync data�� Ÿ���� ��ġ
				if ( !(pBuf[count]  & 0x80) )	// x axis
				{
					m_stiValue[0] = pn*degree;
					m_stiValue[1] = 0;
				}
				else	// y axis
				{
					m_stiValue[0] = 0;
					m_stiValue[1] = pn*degree;
				}
			}
			count++;
		}
	}

}

DWORD ThreadLEDComm(CCommLED* pComm)
{
	DWORD		dwEvent;
	OVERLAPPED	os;
	BOOL		bOk = TRUE;
	BYTE		buff[2048];		// �б� ����
	DWORD		dwRead;			// ���� ����Ʈ ��
	

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
		bOk = FALSE;
	}
	if(!bOk)
	{
		TRACE("------- Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		return FALSE;
	}

	TRACE("...... ThreadWatchComm entered !!! \n");

	// ��Ʈ�� �����ϴ� ����.
	while(pComm->m_bConnected)
	{

		//TRACE("......TRUE - -  Watching communication port !!!\n");
		dwEvent = 0;
		// waits for an event to occur for a specified communications device
		WaitCommEvent(pComm->m_hComm, &dwEvent, NULL);
			
		if((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{
			memset(buff, NULL, 2048);
			dwRead = pComm->ReadComm(buff, 2048);
			
			pComm->setReadData(buff, dwRead); 
			
		}
		

	}	
	
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

