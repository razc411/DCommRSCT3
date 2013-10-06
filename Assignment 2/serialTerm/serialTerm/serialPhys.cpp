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

HANDLE init(ioStruct *io){
	COMMTIMEOUTS timeouts;
	HANDLE hdSerial = CreateFile(io->port, 
							GENERIC_READ | GENERIC_WRITE, 
							0, 
							0, 
							OPEN_EXISTING, 
							FILE_FLAG_OVERLAPPED, 
							0);

	if(hdSerial == INVALID_HANDLE_VALUE){
		MessageBox (io->hwnd, TEXT("Failed to Connect"), TEXT("Dumb Terminal"), MB_OK);
		return NULL;
	}
	
	
	io->cc.dwSize = sizeof(COMMCONFIG);
	io->cc.wVersion = 0x100;
	GetCommConfig (hdSerial, &io->cc, &io->cc.dwSize); 
	if (!CommConfigDialog (io->port, io->hwnd, &io->cc)){
		GetLastError();
	}

	timeouts.ReadIntervalTimeout = 20; 
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.ReadTotalTimeoutConstant = 100;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 100;
			
	if(!SetCommState(hdSerial, &io->cc.dcb))
		MessageBox(io->hwnd, TEXT("Could not set comm state"), TEXT("RFID Terminal"), MB_OK | MB_ICONEXCLAMATION);
			
	if(!SetCommTimeouts(hdSerial, &timeouts))
		MessageBox(io->hwnd, TEXT("Could not set timeout state"), TEXT("RFID Terminal"), MB_OK | MB_ICONEXCLAMATION);
					
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
DWORD WINAPI execRead(LPVOID ioS){
		
	
	WCHAR			charBuff[100];
	COMSTAT			cs;
	DWORD			nBytesRead	= 0, sError, sEvent, nBytesTransferred = 0;
	static unsigned k			= 0;
	struct ioStruct	*io			= (struct ioStruct *)ioS;
	HDC				hdc			= GetDC(io->hwnd);
	TEXTMETRIC		tm;
	int				Y			= 0;
	int				X			= 0;
	
	OVERLAPPED osReader = {0};
	io->olapIO = osReader;
	io->olapIO.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	GetTextMetrics(hdc, &tm);

	SetCommMask(io->hdSerial, EV_RXCHAR);

	while(1){	
		if(WaitCommEvent(io->hdSerial, &sEvent, &io->olapIO)){
				//ClearCommError(io->hdSerial, &sError, &cs);
				if(!GetOverlappedResult(io->hdSerial, &io->olapIO, &nBytesTransferred, FALSE)){
					locProcessCommError(GetLastError(), io->hdSerial);
				}
				else{
					if(nBytesTransferred){	
						ReadFile(io->hdSerial, &charBuff, nBytesTransferred, &nBytesRead, &io->olapIO);
						if(X >= WIN_WIDTH){
							Y = Y + tm.tmHeight + tm.tmExternalLeading;
							X = 0;
						}
						TextOut(hdc, X, Y, charBuff, nBytesRead); 
						X += tm.tmMaxCharWidth;
					}
				}
			}
		else {
			locProcessCommError (GetLastError(), io->hdSerial);
		}
	}


	ReleaseDC(io->hwnd, hdc);	
	return 0;
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
