#include "main.h"
#include "carThread.h"
#include "resource.h"
#include "utility.h"
#include "enum.h"

#pragma comment (lib, "msimg32.lib")

//utility.h
std::thread *t;

//main.h
HINSTANCE		g_hInst;
HWND			hWndMain;
HBITMAP			hbackbit; //백버퍼

//carThread.h
unsigned int				numOfcar;
unsigned int				numOfturnel;
carArgument					*carArg;
std::thread					*carThreads;

//local
LPCTSTR		lpszClass = TEXT("WM_INITDIALOG2");


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	RECT wrt, crt;

	switch (iMessage) {
	case WM_INITDIALOG:
		GetWindowRect(GetParent(hDlg), &wrt);
		GetWindowRect(hDlg, &crt);
		SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
			wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:	
			destorythread();		

			numOfcar = GetDlgItemInt(hDlg, IDC_EDIT_CAR, NULL, FALSE);
			numOfturnel = GetDlgItemInt(hDlg, IDC_EDIT_TURNEL, NULL, FALSE);
			EndDialog(hDlg, IDC_OK);
			//쓰레드생성						
			t = new std::thread(&CreateCarThread);

			return TRUE;
		case IDC_CANCEL:
			EndDialog(hDlg, IDC_CANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_START:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
			break;
		}
		return 0;
	case WM_PAINT:
		HDC hdc, MemDC;
		RECT crt;
		HBITMAP OldBit;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &crt);
		MemDC = CreateCompatibleDC(hdc);
		OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);
		BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
		//TransparentBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, crt.right, crt.bottom, RGB(255, 255, 255));		
		SelectObject(MemDC, OldBit);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);

		return 0;
	case WM_DESTROY:			
		destorythread();
		PostQuitMessage(0);
	
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
