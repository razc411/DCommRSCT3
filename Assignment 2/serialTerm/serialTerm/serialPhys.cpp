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
//
//DWORD init(HWND hwnd, LPCWSTR port, COMMCONFIG cc, LPSKYETEK_DEVICE device){
//	COMMTIMEOUTS timeouts;
//
//	device->readFD = CreateFile(device->address,
//					GENERIC_READ,
//					FILE_SHARE_WRITE,
//					NULL,
//					OPEN_EXISTING,
//					FILE_FLAG_OVERLAPPED,
//					NULL);
//
//	if( device->readFD == INVALID_HANDLE_VALUE )
//	{
//
//	}
//
//	device->writeFD = CreateFile(device->address,
//					GENERIC_WRITE,
//					FILE_SHARE_READ,
//					NULL,
//					OPEN_EXISTING,
//					FILE_FLAG_OVERLAPPED,
//					NULL);
//
//	if( device->writeFD == INVALID_HANDLE_VALUE )
//	{
//		CloseHandle(device->readFD);
//		device->readFD = device->writeFD = 0;
//	}
//	
//	
//	cc.dwSize = sizeof(COMMCONFIG);
//	cc.wVersion = 0x100;
//	GetCommConfig (device->writeFD, &cc, &cc.dwSize); 
//	if (!CommConfigDialog (port, hwnd, &cc)){
//		GetLastError();
//	}
//
//	timeouts.ReadIntervalTimeout        = MAXDWORD; 
//	timeouts.ReadTotalTimeoutMultiplier    = 0;
//	timeouts.ReadTotalTimeoutConstant    = 0;
//	timeouts.WriteTotalTimeoutMultiplier    = 0;
//	timeouts.WriteTotalTimeoutConstant    = 0;
//
//			
//	if(!SetCommState(device->writeFD, &cc.dcb))
//		MessageBox(hwnd, TEXT("Could not set comm state"), TEXT("RFID Terminal"), MB_OK | MB_ICONEXCLAMATION);
//			
//	if(!SetCommTimeouts(device->writeFD, &timeouts))
//		MessageBox(hwnd, TEXT("Could not set timeout state"), TEXT("RFID Terminal"), MB_OK | MB_ICONEXCLAMATION);
//					
//	return 1;
//	}
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
