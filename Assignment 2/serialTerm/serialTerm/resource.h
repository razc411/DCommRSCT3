#define IDR_MENU1                       101
#define	IDM_CONNECT						40004
#define IDM_DISCONNECT					40006
#define IDM_HELP						40005
#define WIN_WIDTH						800
#define WIN_HEIGHT						500
#define WM_PAINT_TRIGGER				30302042

#include <windows.h>
#include "API\SkyeTekAPI.h"

#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        102
#define _APS_NEXT_COMMAND_VALUE         40003
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI execRead(LPVOID);
void endRead(HWND, int, int, HMENU);
void openConnection(HWND, HMENU);
int increaseY();
