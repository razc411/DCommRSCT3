#include "data.h"
#include "resource.h"

LPCSTR szCommName  = TEXT("COM1");

extern LPCSTR szTextName;
extern LPCSTR szStatsName;
extern LPCSTR szWinName;

LPTHREAD_START_ROUTINE start_opengl(LPVOID);
LPTHREAD_START_ROUTINE thread_transmit(LPVOID  pVoid);


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK StatWndProc(HWND, UINT, WPARAM, LPARAM);

p_sensor_data sensor_data_interface( struct sensor_data* );


// main window proc
LRESULT CALLBACK WndProc (HWND hwnd, UINT Message,
                          WPARAM wParam, LPARAM lParam)
{
	DWORD threadId;
	HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

	static HWND			hwndclient;
	CLIENTCREATESTRUCT	clientcreate;
		
	HWND				hwndchild;
	MDICREATESTRUCT		mdicreate ;

	static p_sensor_data psd = NULL;


	/////////////////////sd.y = rand() % 20;
	/////////////////////sensor_data_interface(&sd);

	switch (Message)
	{
		case WM_CREATE:

			psd = (p_sensor_data)malloc(sizeof(sensor_data));
			psd->idle = true;


			clientcreate.hWindowMenu = NULL;
			clientcreate.idFirstChild = IDM_FIRSTCHILD;
			hwndclient = CreateWindow (
				TEXT ("MDICLIENT"), 
				NULL, 
				WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,  
				0, 
				0, 
				0, 
				0,
				hwnd, 
				NULL, 
				hinst, 
				(void*) &clientcreate
			); 

		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				// network controls start here
				case IDM_RECEIVE:
				break;

				case IDM_TRANSMIT:
					
					if ((psd->hcomm = CreateFile (szCommName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) {
						MessageBox (NULL, TEXT("Error opening COM port"), NULL,MB_OK);
						//return FALSE;
					}

					if(! psd->idle ) return FALSE;

					psd->idle = false;
					//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_opengl, (LPVOID)&idle, NULL, &threadId);
					//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_transmit, (LPVOID)psd, NULL, &threadId);

					SendMessage(hwnd, WM_COMMAND, IDR_TRANSMITTEXT, 0);

				break;

				case IDM_DISCONNECT:
					// crit section
					psd->idle = true;
					// end
				break;


				// MDI from here till end
				case IDR_TRANSMITTEXT:
					mdicreate.szClass = szTextName;
					mdicreate.szTitle = TEXT ("Text") ;
					mdicreate.hOwner  = hinst;
					mdicreate.x       = CW_USEDEFAULT ;
					mdicreate.y       = CW_USEDEFAULT ;
					mdicreate.cx      = CW_USEDEFAULT ;
					mdicreate.cy      = CW_USEDEFAULT ;
					mdicreate.style   = 0 ;
					mdicreate.lParam  = (LPARAM)&psd ;
					hwndchild = (HWND) SendMessage (hwndclient, WM_MDICREATE | WM_SIZE, 0, (LPARAM) (LPMDICREATESTRUCT) &mdicreate) ;
	
					if(NULL == hwndchild) return 0;
					SendMessage (hwndclient, WM_MDITILE, 0, 0) ;
				break;

				case IDM_NEW_STATISTICS:
					mdicreate.szClass = szStatsName;
					mdicreate.szTitle = TEXT ("Stats") ;
					mdicreate.hOwner  = hinst;
					mdicreate.x       = CW_USEDEFAULT;
					mdicreate.y       = CW_USEDEFAULT;
					mdicreate.cx      = CW_USEDEFAULT;
					mdicreate.cy      = CW_USEDEFAULT;
					mdicreate.style   = 0 ;
					mdicreate.lParam  = 0 ;
					hwndchild = (HWND) SendMessage (hwndclient, WM_MDICREATE, 0, (LPARAM) (LPMDICREATESTRUCT) &mdicreate) ;
	
					if(NULL == hwndchild) return 0;
					SendMessage (hwndclient, WM_MDITILE, 0, 0) ;
				break;
				
				case IDM_TILE:
					SendMessage (hwndclient, WM_MDITILE, 0, 0) ;
				break;

				case IDM_CASCADE:
					SendMessage (hwndclient, WM_MDICASCADE, 0, 0) ;
				break;

				case IDM_ARRANGE:
					SendMessage (hwndclient, WM_MDIICONARRANGE, 0, 0) ;
				break;
			}
		break;


		case WM_DESTROY:
			free(psd);
			PostQuitMessage (0);
		break;
		
		default: // Let Win32 process all other messages
		return DefFrameProc (hwnd, hwndclient, Message, wParam, lParam);
	}
	return 0;	
}

LRESULT CALLBACK TextWndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	RECT rect;

	static p_sensor_data psd;
	static HWND hTextbox;
	static HWND hTransmitbutton;

	CHAR buf[1024];
	int length;
	
	switch(Message)
	{

		case WM_CREATE:

			psd = (p_sensor_data)lParam;
			psd->idle = true;

			psd->ring_buff.tail = 0;
			psd->ring_buff.head = 0;
			psd->ring_buff.count = 0;

			GetClientRect(hwnd, &rect);
			hTextbox = CreateWindow (
				TEXT("edit"), 
				NULL, 
				WS_CHILD | WS_VISIBLE,
   				0,
				45, 
				rect.right,
				rect.bottom,
				hwnd , 
				(HMENU)1,
                hinst, 
				NULL);

			hTransmitbutton = CreateWindow (
				TEXT("button"), 
				TEXT("Transmit"), 
				BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE,
   				10, 10,
				100, 
				25,
				hwnd , 
				(HMENU)TRANSMIT_BUTTON,
                hinst, 
				NULL);
		break;

		case WM_SIZE:
			GetClientRect(hwnd, &rect);
			MoveWindow(hTextbox, rect.left, rect.top + 45, rect.right - rect.left, rect.bottom - rect.top, false);
		break;

		case WM_COMMAND:

			case BN_CLICKED:
				if(hTransmitbutton == (HWND)lParam) {

					length = SendMessage(hTextbox, WM_GETTEXTLENGTH, 0, 0);
					SendMessage(hTextbox, WM_GETTEXT, length+1, LPARAM(buf));


					//!!!!! MessageBox(hwnd, buf, NULL, 0);
				}
			break;

		break;


		default:
		return DefMDIChildProc (hwnd, Message, wParam, lParam) ;
	}
	return 0;
}


// statistics window proc
LRESULT CALLBACK StatWndProc (HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {

	HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	static HWND hListbox;
	RECT rect;

	switch(Message)
	{

		case WM_CREATE:
			GetClientRect(hwnd, &rect);
			
			hListbox = CreateWindow (
				TEXT("listbox"), 
				NULL, 
				WS_CHILD | WS_VISIBLE,
   				0, 0,
				rect.right - rect.left, 
				rect.bottom - rect.top,
				hwnd , 
				NULL ,
                hinst, 
				NULL);
		break;

		case WM_SIZE:
			GetClientRect(hwnd, &rect);
			MoveWindow(hListbox, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, false);

		break;

		default:
		return DefMDIChildProc (hwnd, Message, wParam, lParam) ;
	}

  return 0;
}