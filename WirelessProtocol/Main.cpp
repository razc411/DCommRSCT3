#include <windows.h>
#include "data.h"
#include "resource.h"

HANDLE sensor_data_sem;


LPCSTR szTextName = TEXT("TEXT");
LPCSTR szStatsName = TEXT("STATISTICS");
LPCSTR szWinName   = TEXT("WIRELESS");
LPCSTR szMenuName  = TEXT("MAINMENU");



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK StatWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TextWndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hprevInstance,
 						  LPSTR lspszCmdParam, int nCmdShow)
{
	HWND hwnd;
	MSG Msg;
	WNDCLASSEX Wcl;


	// Define a Window class
	Wcl.cbSize = sizeof (WNDCLASSEX);
	Wcl.style			= 0;
	Wcl.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	Wcl.hIconSm			= NULL;
	Wcl.hCursor			= LoadCursor(NULL, IDC_ARROW);
	Wcl.lpfnWndProc		= WndProc;
	Wcl.hInstance		= hInst; 
	Wcl.hbrBackground	= (HBRUSH) GetStockObject(GRAY_BRUSH);
	Wcl.lpszClassName	= szWinName;
	Wcl.lpszMenuName	= szMenuName;
	Wcl.cbClsExtra		= 0;
	Wcl.cbWndExtra		= 0;

	if (!RegisterClassEx (&Wcl)) return 0;


	hwnd = CreateWindow (
		szWinName,
		szWinName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
   		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInst,
		NULL 
	);
	
	// Display the window
	ShowWindow (hwnd, nCmdShow);
	UpdateWindow (hwnd);



	// Define a Text class
	Wcl.cbSize			= sizeof (WNDCLASSEX);
	Wcl.style			= 0;
	Wcl.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	Wcl.hIconSm			= NULL;
	Wcl.hCursor			= LoadCursor(NULL, IDC_ARROW);
	Wcl.lpfnWndProc		= TextWndProc;
	Wcl.hInstance		= hInst; 
	Wcl.hbrBackground	= (HBRUSH) GetStockObject(GRAY_BRUSH);
	Wcl.lpszClassName	= szTextName;
	Wcl.lpszMenuName	= NULL;
	Wcl.cbClsExtra		= 0;
	Wcl.cbWndExtra		= 0;

	if (!RegisterClassEx (&Wcl)) return 0;



	// Define a Statistics class
	Wcl.cbSize			= sizeof (WNDCLASSEX);
	Wcl.style			= 0;
	Wcl.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	Wcl.hIconSm			= NULL;
	Wcl.hCursor			= LoadCursor(NULL, IDC_ARROW);
	Wcl.lpfnWndProc		= StatWndProc;
	Wcl.hInstance		= hInst; 
	Wcl.hbrBackground	= (HBRUSH) GetStockObject(GRAY_BRUSH);
	Wcl.lpszClassName	= szStatsName;
	Wcl.lpszMenuName	= NULL;
	Wcl.cbClsExtra		= 0;
	Wcl.cbWndExtra		= 0;

	if (!RegisterClassEx (&Wcl)) return 0;


	sensor_data_sem = CreateSemaphore(NULL, 1, 1, TEXT("sensor_data"));


	// Create the message loop
	while( GetMessage(&Msg, NULL, 0, 0) )
	{
		TranslateMessage (&Msg);
		DispatchMessage (&Msg);
	}

	return Msg.wParam;
}



