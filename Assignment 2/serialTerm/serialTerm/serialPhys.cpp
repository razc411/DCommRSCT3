/*------------------------------------------------------------------------------------------------------------------
--		SOURCE FILE:	serialPhys.cpp
--		PROGRAM:		DumbTerminalEmulator
--		Functions
--			init(ioStruct *io)
--			execRead(LPVOID ioS)
			writeOutputBuffer(HANDLE hdSerial, WPARAM chBuffer)
			void endSession(HANDLE hdSerial, HANDLE readThrd)
--
--		DATE:			September 21st, 2013
--		DESIGNER:		Ramzi Chennafi	
--		PROGRAMMER:		Ramzi Chennafi
--
--		NOTES:
--		Physical Layer of the program. Opens the serial port for communication, then provides functions that can
		read and write to that serial port. A new thread is created for the read portion of this layer. Also preforms
		buffer flushing and session termination.
----------------------------------------------------------------------------------------------------------------------*/
#include <Windows.h>
#include "resource.h"
/*----------------------------------------------------------------------------------------------------------------------
--		FUNCTION:		init
--		DATE:			September 21st, 2012
--		REVISIONS:		n/a
--		DESIGNER:		Ramzi Chennafi
--		PROGRAMMER:		Ramzi Chennafi	
--
--		INTERFACE:		HANDLE init(ioStruct *io)				
--
--		RETURNS:		The port handle on successful opening of connection and NULL on failure, along with a 
						warning message on failure.
--
--		NOTES:
--		Opens the selected serial port in synchronous i/o mode. 
----------------------------------------------------------------------------------------------------------------------*/

HANDLE init(HWND hwnd, LPCWSTR port, COMMCONFIG cc){
	COMMTIMEOUTS timeouts;
	HANDLE hdSerial = CreateFile(port, 
							GENERIC_READ | GENERIC_WRITE, 
							0, 
							0, 
							OPEN_EXISTING, 
							FILE_FLAG_OVERLAPPED, 
							0);

	if(hdSerial == INVALID_HANDLE_VALUE){
		MessageBox (hwnd, TEXT("Failed to Connect"), TEXT("Dumb Terminal"), MB_OK);
		return NULL;
	}
	
	
	cc.dwSize = sizeof(COMMCONFIG);
	cc.wVersion = 0x100;
	GetCommConfig (hdSerial, &cc, &cc.dwSize); 
	if (!CommConfigDialog (port, hwnd, &cc)){
		GetLastError();
	}

	timeouts.ReadIntervalTimeout        = MAXDWORD; 
	timeouts.ReadTotalTimeoutMultiplier    = 0;
	timeouts.ReadTotalTimeoutConstant    = 0;
	timeouts.WriteTotalTimeoutMultiplier    = 0;
	timeouts.WriteTotalTimeoutConstant    = 0;

			
	if(!SetCommState(hdSerial, &cc.dcb))
		MessageBox(hwnd, TEXT("Could not set comm state"), TEXT("RFID Terminal"), MB_OK | MB_ICONEXCLAMATION);
			
	if(!SetCommTimeouts(hdSerial, &timeouts))
		MessageBox(hwnd, TEXT("Could not set timeout state"), TEXT("RFID Terminal"), MB_OK | MB_ICONEXCLAMATION);
					
	return hdSerial;
	}
/*------------------------------------------------------------------------------------------------------------------
--		FUNCTION:		execRead
--		DATE:			September 21st, 2012
--		REVISIONS:		n/a
--		DESIGNER:		Ramzi Chennafi
--		PROGRAMMER:		Ramzi Chennafi	
--
--		INTERFACE:		DWORD WINAPI execRead(LPVOID ioS)						
--
--		RETURNS:		DWORD exit code
--
--		NOTES:
--		New thread function called within the wndProc. Deals with the reading/formatting of recieved text and its display. Recieves an
		ioStruct containing the serial port handle and the window HWND.
----------------------------------------------------------------------------------------------------------------------*/
void readPort(HANDLE hndSerial, LPOVERLAPPED osReader){
		
	
	WCHAR			charBuff[100];
	DWORD			nBytesRead	= 0; 
	DWORD			sEvent;
	DWORD			nBytesTransferred = 0;


	SetCommMask(hndSerial, EV_RXCHAR);

	if(WaitCommEvent(hndSerial, &sEvent, osReader)){
		
			GetOverlappedResult(hndSerial, osReader, &nBytesTransferred, FALSE);
			//ClearCommError(io->hdSerial, &sError, &cs);
			ReadFile(hndSerial, &charBuff, nBytesTransferred, &nBytesRead, osReader);
			paintFile(charBuff, nBytesRead);	
		
	}

	ResetEvent(osReader->hEvent);
	
	
}
/*------------------------------------------------------------------------------------------------------------------
--		FUNCTION:		endSession
--		DATE:			September 21st, 2012
--		REVISIONS:		n/a
--		DESIGNER:		Ramzi Chennafi
--		PROGRAMMER:		Ramzi Chennafi	
--
--		INTERFACE:		endSession(HANDLE hdSerial, HANDLE readThrd)					
--
--		RETURNS:		Nothing
--
--		NOTES:
--		Flushes the I/O buffer, closes the serial port and terminates the read thread. Will not do so unless
		handles have been intiated.
----------------------------------------------------------------------------------------------------------------------*/
void endSession(HANDLE hdSerial, HANDLE readThrd){
	
	if(hdSerial != NULL && readThrd != NULL){
		CancelSynchronousIo(readThrd);
		CloseHandle(hdSerial);
		TerminateThread(readThrd, 0);
	}
}

void locProcessCommError (DWORD dwError, HANDLE hdSerial){
	DWORD lrc;
	COMSTAT cs;

	ClearCommError (hdSerial, &lrc, &cs);

}
