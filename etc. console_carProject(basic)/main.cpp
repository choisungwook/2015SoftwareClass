#pragma comment (lib, "msimg32.lib")

#include <Windows.h>
#include "resource.h"
#define WIDTH 1024
#define HEIGHT 768

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

//윈도우창 제목 관련 변수
char		*title = "3조";
char		*lpszname = "softwareclass";

//WM_PAINT관련 전역 변수
HINSTANCE	hInst;
HWND		hWndMain;
//백버퍼
HBITMAP		hbackbit; //백버퍼

int main()
{	
	hInst = (HINSTANCE)::GetModuleHandle(NULL);

	//윈도우 클래스 생성 및 등록
	WNDCLASS wc =
	{
		CS_CLASSDC
		, WndProc
		, 0L
		, 0L
		, hInst
		, NULL
		, LoadCursor(NULL, IDC_ARROW)
		, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
		, MAKEINTRESOURCE(IDR_MENU1)
		, lpszname
	};

	::RegisterClass(&wc);


	HWND hWnd = CreateWindow(lpszname
		, title
		, WS_OVERLAPPEDWINDOW | WS_VISIBLE
		, 0
		, 0
		, WIDTH
		, HEIGHT
		, NULL
		, NULL
		, hInst
		, NULL);

	hWndMain = hWnd;
	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);
	::ShowCursor(TRUE);

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	::UnregisterClass("차영화관", hInst);

	return 0;
}


LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if (msg == WM_CREATE)
	{
		
	}

	else if (msg == WM_DESTROY)
	{
		
		::PostQuitMessage(0);
		return 0;
	}

	else if (msg == WM_PAINT)
	{
		//HDC hdc, MemDC;
		//RECT crt;
		//HBITMAP OldBit;
		//PAINTSTRUCT ps;

		//hdc = BeginPaint(hWnd, &ps);
		//GetClientRect(hWnd, &crt);
		//MemDC = CreateCompatibleDC(hdc);
		//OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);
		//BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
		////TransparentBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, crt.right, crt.bottom, RGB(255, 255, 255));		
		//SelectObject(MemDC, OldBit);
		//DeleteDC(MemDC);
		//EndPaint(hWnd, &ps);

		return 0;
	}

	if (msg == WM_COMMAND)
	{
		switch (LOWORD(wParam))
		{
		case ID_FILE_START:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
			break;
		case ID_FILE_OPEN:			
			break;
		case ID_FILE_EXIT:			
			PostQuitMessage(0);
			break;
		}
		return 0;
	}

	if (msg == WM_KEYDOWN)
	{
		switch (wParam)
		{
			//esc누르면 종료
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}

		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	RECT wrt, crt;

	if (iMessage == WM_INITDIALOG)
	{
		GetWindowRect(GetParent(hDlg), &wrt);
		GetWindowRect(hDlg, &crt);
		SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
			wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
		return TRUE;
	}

	else if (iMessage == WM_COMMAND)
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			//numOfcar = GetDlgItemInt(hDlg, IDC_EDIT_CAR, NULL, FALSE);
			//numOfturnel = GetDlgItemInt(hDlg, IDC_EDIT_TURNEL, NULL, FALSE);
			EndDialog(hDlg, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
	}
	return FALSE;
}
