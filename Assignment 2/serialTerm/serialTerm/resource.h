#define IDR_MENU1                       101
#define ID_PORT_COMM1                   40001
#define ID_PORT_COMM3                   40002
#define IDM_DATA_SETTINGS				40003
#define	IDM_CONNECT						40004
#define IDM_HELP						40005
#define COMM1							1
#define COMM3							3
#define WIN_WIDTH						800
#define WIN_HEIGHT						500

#include <windows.h>
#include "API\SkyeTekAPI.h"
#include "API\SkyeTekProtocol.h"


#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        102
#define _APS_NEXT_COMMAND_VALUE         40003
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
void checkItem(HMENU, UINT);
DWORD WINAPI execRead(LPVOID);
DWORD paintFile(WCHAR*, DWORD);
