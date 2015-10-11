#include <stdio.h>
#include <windows.h>
#include <process.h>
#include "resource.h"
#define CARSIZE 3

typedef struct carArg
{
	int posX;
	int posY;
}carArg;


HINSTANCE hInst;
HWND	hWnd;

//차쓰레드
HANDLE carThread[CARSIZE];
//차의 좌표
carArg arg[CARSIZE];

//함수
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DestoryThread();

//쓰레드 함수
unsigned WINAPI car(void *arg);

//INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
int main()
{
	int posX = 100, posY = 100;

	hInst = (HINSTANCE)::GetModuleHandle(NULL);

	WNDCLASS wc =					// Register the window class
	{
		CS_CLASSDC
		, WndProc
		, 0L
		, 0L
		, hInst
		, NULL
		, LoadCursor(NULL, IDC_ARROW)
		, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
		, NULL
		, "차영화관"
	};

	::RegisterClass(&wc);


	hWnd = CreateWindow("차영화관"
		, "차차차차"
		, WS_OVERLAPPEDWINDOW | WS_VISIBLE
		, 20
		, 10
		, 800
		, 600
		, NULL
		, NULL
		, hInst
		, NULL);


	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);
	::ShowCursor(TRUE);

	//쓰레드 인자 초기화
	ZeroMemory(arg, sizeof(arg));

	//쓰레드 생성
	for (int i = 0; i < CARSIZE; i++)
	{
		printf("[%d] 쓰레드 생성\n",i);
		arg[i].posX = posX;
		arg[i].posY = posY;
		posY += 100;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg, 0, NULL);
	}


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
	HDC hdc, MemDC;
	PAINTSTRUCT ps;
	HBITMAP MyBitmap[CARSIZE], OldBitmap[CARSIZE];

	if (msg == WM_CREATE)
	{

	}
	if (msg == WM_DESTROY)
	{
		::PostQuitMessage(0);
		return 0;
	}
	if (msg == WM_PAINT)
	{
		hdc = BeginPaint(hWnd, &ps);
		MemDC = CreateCompatibleDC(hdc);
		for (int i = 0; i < CARSIZE; i++)
			MyBitmap[i] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		for (int i = 0; i < CARSIZE; i++)
			OldBitmap[i] = (HBITMAP)SelectObject(MemDC, MyBitmap[i]);
		for (int i = 0; i < CARSIZE; i++)
			BitBlt(hdc, arg[i].posX, arg[i].posY, 160, 160, MemDC, 0, 0, SRCCOPY);
		for (int i = 0; i < CARSIZE; i++)
			SelectObject(MemDC, OldBitmap[i]);
		for (int i = 0; i < CARSIZE; i++)
			DeleteObject(MyBitmap[i]);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}




void DestoryThread()
{
	//차쓰레드 파괴
	for (int i = 0; i < CARSIZE; i++)
		CloseHandle(carThread[i]);
}

//쓰레드 함수
//thread 함수
unsigned WINAPI car(void *arg)
{
	carArg *Argument = ((carArg*)arg);
	
	for (int i = 0; i < 100; i++)
	{
		printf("x : %d y : %d\n", Argument->posX, Argument->posX);
		Argument->posX += 10;		
		//InvalidateRgn(hWnd,NULL,TRUE);
		InvalidateRect(hWnd, NULL, TRUE);
		Sleep(100);
	}
	return 0;
}
