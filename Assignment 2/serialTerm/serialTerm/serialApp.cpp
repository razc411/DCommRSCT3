/*------------------------------------------------------------------------------------------------------------------
--		SOURCE FILE:	serialApp.cpp
--		PROGRAM:		DumbTerminalEmulator
--		Functions
--				WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
-- 						  LPSTR lspszCmdParam, int nCmdShow)	
--				LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
--				void checkItem(HMENU, UINT);
--
--		DATE:			September 21st, 2013
--		DESIGNER:		Ramzi Chennafi	
--		PROGRAMMER:		Ramzi Chennafi
--
--		NOTES:
--		Application layer of the program. Deals with graphical components and drawing, excluding the external thread draw.
--
--		Handles the window of the terminal. Also deals with intiating read and writes along the serial connection
--		and the settings that go with said serial connection. Prints any data recieved to the window and sends any keyboard input
--		through the serial line after the Connect menu item is pressed. 
----------------------------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include "resource.h"


HWND			mainHWND;
HANDLE			readThrd;
HANDLE			hndSerial;
COMMCONFIG		cc;
LPCWSTR			port		= TEXT("com1");
OVERLAPPED		osReader	= {0,0,0,0, CreateEvent(NULL, TRUE, FALSE, NULL)};
static TCHAR	Name[]		= TEXT("serialTerm");
bool			activePort	= false;
LPSKYETEK_DEVICE device;
LPSKYETEK_DEVICE usbDevice;

/*------------------------------------------------------------------------------------------------------------------
--		FUNCTION:		WinMain
--		DATE:			September 21st, 2012
--		REVISIONS:		n/a
--		DESIGNER:		Ramzi Chennafi
--		PROGRAMMER:		Ramzi Chennafi	
--
--		INTERFACE:		int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hprevInstance,
--											LPSTR lspszCmdParam, int nCmdShow)					
--
--		RETURNS:		int 
--
--		NOTES:
--		Creates a new window for the application and begins calls. Intiates hwnd, port and serialOn trigger of the ioStruct.
----------------------------------------------------------------------------------------------------------------------*/

int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hprevInstance,
 						  LPSTR lspszCmdParam, int nCmdShow)
{
	HWND		hwnd;
	MSG			Msg;
	WNDCLASSEX	Wcl;
	
	Wcl.cbSize = sizeof (WNDCLASSEX);
	Wcl.style = CS_HREDRAW | CS_VREDRAW;
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); 
	Wcl.hIconSm = NULL; 
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  
	
	Wcl.lpfnWndProc = WndProc;
	Wcl.hInstance = hInst;
	Wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); 
	Wcl.lpszClassName = Name;
   
	Wcl.lpszMenuName = TEXT("MYMENU");
	Wcl.cbClsExtra = 0;   
	Wcl.cbWndExtra = 0;
	
	if (!RegisterClassEx (&Wcl))
		return 0;
   
	hwnd = CreateWindow (Name, Name, WS_OVERLAPPEDWINDOW, 350, 160,
   							800, 500, NULL, NULL, hInst, NULL);
	
	mainHWND = hwnd;
	
	ShowWindow (hwnd, nCmdShow);
	UpdateWindow (hwnd);

	while (GetMessage (&Msg, NULL, 0, 0))
	{
   		TranslateMessage (&Msg);
		DispatchMessage (&Msg);
	}

	return Msg.wParam;
}
/*------------------------------------------------------------------------------------------------------------------
--		FUNCTION:		WndProc
--		DATE:			September 21st, 2012
--		REVISIONS:		n/a
--		DESIGNER:		Ramzi Chennafi
--		PROGRAMMER:		Ramzi Chennafi	
--
--		INTERFACE:		LRESULT CALLBACK WndProc (HWND hwnd, UINT Message,
--                          WPARAM wParam, LPARAM lParam)				
--
--		RETURNS:		LRESULT to the system.
--
--		NOTES:
--		Deals with the main window thread. Requests writes, modification of data settings, usage information and 
--		connect/disconnect buttons. Also creates a new thread to perform read operations.
----------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	HMENU menuHnd = GetMenu(hwnd);
	DWORD threadId;
	LPSKYETEK_DEVICE **devices;
	LPSKYETEK_READER **readers;
	int flag = 0;
	HDC hdc = GetDC(hwnd);

	switch (Message)
   {
		case WM_COMMAND:
		   switch (LOWORD (wParam))
			{
				case IDM_CONNECT:
					if(!activePort){
						SkyeTek_DiscoverDevices(devices);
						SkyeTek_DiscoverReaders(devices[0], 1, readers);
						readThrd = CreateThread(NULL, 0, execRead, readers, 0, &threadId);
				break;

				case IDM_HELP:
					MessageBox (hwnd, TEXT("Choose which port you wish to communicate on and then hit data settings to modify the communication settings.\nConnect will begin a session, hit escape to end the current session.\nAny keyboard input will be transferred during a session."), 
						TEXT("Dumb Terminal"), MB_OK | MB_ICONINFORMATION);
				break;
				
				case ID_PORT_COMM1:
					port = TEXT("com1");
					checkItem(menuHnd, COMM1);
				break;
				
				case ID_PORT_COMM3:
					port = TEXT("com3");
					checkItem(menuHnd, COMM3);
				break;

          		case IDM_DATA_SETTINGS:
            		cc.dwSize = sizeof(COMMCONFIG);
					cc.wVersion = 0x100;
					GetCommConfig (hndSerial, &cc, &cc.dwSize);
            		if (!CommConfigDialog (port, hwnd, &cc))
               			break;
				break;
			}
		break;

		case WM_CHAR:	
			
		break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE && activePort == true){
				activePort = false;
				MessageBox (hwnd, TEXT("Client Disconnected"), TEXT("Dumb Terminal"), MB_OK | MB_ICONEXCLAMATION);
				InvalidateRect(hwnd, NULL, FALSE);
				EnableMenuItem(menuHnd, IDM_CONNECT, MF_ENABLED);
				DrawMenuBar(hwnd);
			}
		break;

		case WM_DESTROY:	
      		PostQuitMessage (0);
		break;
		
		default:
			 return DefWindowProc (hwnd, Message, wParam, lParam);
	}
	return 0;
}
/*------------------------------------------------------------------------------------------------------------------
--		FUNCTION:		checkItem
--		DATE:			September 21st, 2012
--		REVISIONS:		n/a
--		DESIGNER:		Ramzi Chennafi
--		PROGRAMMER:		Ramzi Chennafi	
--
--		INTERFACE:		void checkItem(HMENU hnd, UINT port)				
--
--		RETURNS:		Nothing
--
--		NOTES:
--		Check marks the currently used port in the ports menu of the main window. hnd is the menu handle of the item, 
--		while port specifies which item to check. Options are COMM1 and COMM3.
----------------------------------------------------------------------------------------------------------------------*/
void checkItem(HMENU hnd, UINT port){
	switch(port){
		case COMM1 : 
			CheckMenuItem(hnd, ID_PORT_COMM1, MF_CHECKED);
			CheckMenuItem(hnd, ID_PORT_COMM3, MF_UNCHECKED);
		break;
		case COMM3: 
			CheckMenuItem(hnd, ID_PORT_COMM3, MF_CHECKED);
			CheckMenuItem(hnd, ID_PORT_COMM1, MF_UNCHECKED);
		break;
	}
}

/*------------------------------------------------------------------------------------------------------------------
--		FUNCTION:		checkItem
--		DATE:			September 21st, 2012
--		REVISIONS:		n/a
--		DESIGNER:		Ramzi Chennafi
--		PROGRAMMER:		Ramzi Chennafi	
--
--		INTERFACE:		void checkItem(HMENU hnd, UINT port)				
--
--		RETURNS:		Nothing
--
--		NOTES:
--		Check marks the currently used port in the ports menu of the main window. hnd is the menu handle of the item, 
--		while port specifies which item to check. Options are COMM1 and COMM3.
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI execRead(LPVOID reader){

	LPSKYETEK_DATA lpData = NULL;
	LPSKYETEK_TAG lpTag = NULL;
	int flag = 0;

	while(activePort){
		status = SkyeTek_ReadTagData(readers[0],lpTag,&addr,flag,flag,&lpData);
	}

	endSession(hndSerial, readThrd);

	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
--		FUNCTION:		checkItem
--		DATE:			September 21st, 2012
--		REVISIONS:		n/a
--		DESIGNER:		Ramzi Chennafi
--		PROGRAMMER:		Ramzi Chennafi	
--
--		INTERFACE:		void checkItem(HMENU hnd, UINT port)				
--
--		RETURNS:		Nothing
--
--		NOTES:
--		Check marks the currently used port in the ports menu of the main window. hnd is the menu handle of the item, 
--		while port specifies which item to check. Options are COMM1 and COMM3.
----------------------------------------------------------------------------------------------------------------------*/
DWORD paintFile(WCHAR* charBuff, DWORD nBytesRead){
	
	HDC hdc = GetDC(mainHWND);
	int X = 0;
	int Y = 0;
	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	if(X >= WIN_WIDTH){
		Y = Y + tm.tmHeight + tm.tmExternalLeading;
		X = 0;
	}
	TextOut(hdc, X, Y, charBuff, nBytesRead); 
	X += tm.tmMaxCharWidth;

	return 0;
}