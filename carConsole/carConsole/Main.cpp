#include "Main.h"
#include <stdio.h>
#include "carThread.h"
#include "recognizer.h"


//윈도우창 제목 관련 변수
char		*title					= "3조";
char		*lpszname				= "softwareclass";
//이미지경로
char		*backgroundImagePath	= "img\\background.png";
char		*carImagePath			= "img\\car.png";

//세마포어
HANDLE		SEMA_turnel; //터널 카운터 동기화
HANDLE		Hellow_READER;
HANDLE		Leave_READER;

//충돌체크 뮤텍스
HANDLE		collisionMutex;

//WM_PAINT관련 전역 변수
HINSTANCE	hInst;
HWND		hWnd;
HBITMAP		hbackbit;

//차쓰레드
HANDLE carController; //차 컨트롤러
HANDLE carThread[numOfCar];

//차량인식기쓰레드
HANDLE carReader;

//차의 좌표
carArg arg[numOfCar];

HBITMAP hbackground;
HBITMAP hcar[numOfCar];

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
		, NULL
		, lpszname
	};

	::RegisterClass(&wc);


	hWnd = CreateWindow(lpszname
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
		SemaphoreInit();
		carController = (HANDLE)_beginthreadex(NULL, NULL, CreateCarThread, NULL, 0, NULL);		
		createReaderThread();
	}

	else if (msg == WM_DESTROY)
	{
		DestoryThread();
		::PostQuitMessage(0);		
		return 0;
	}

	else if (msg == WM_PAINT)
	{	
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

void DestoryThread()
{
	//컨트롤러파괴
	CloseHandle(carController);

	//차쓰레드 파괴
	destoryCarThread();
	//차량인시기 파괴
	DestoryReaderThread();
	//세마포어 파괴
	DestorySemaphore();
}

void DestorySemaphore()
{
	CloseHandle(SEMA_turnel);
	CloseHandle(collisionMutex);
	CloseHandle(Hellow_READER);
	CloseHandle(Leave_READER);
}

void SemaphoreInit()
{
	collisionMutex = CreateMutex(NULL, FALSE, NULL);
	SEMA_turnel = CreateSemaphore(NULL, MAXOFTURNEL, MAXOFTURNEL, NULL);
	Hellow_READER = CreateSemaphore(NULL, 0, 1, NULL);
	Leave_READER = CreateSemaphore(NULL, 0, 1, NULL);
}