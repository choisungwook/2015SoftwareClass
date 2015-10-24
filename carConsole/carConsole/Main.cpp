#include "Main.h"
#include <stdio.h>
#include <atlimage.h> //CImage
#include "carThread.h"


char		*backgroundImagePath	= "img\\background.png";
char		*carImagePath			= "img\\car.png";
HINSTANCE	hInst;
HWND		hWnd;
HBITMAP		hbackbit;

//차쓰레드
HANDLE carThread[numOfCar];
//차의 좌표
carArg arg[numOfCar];


HBITMAP hbackground;
HBITMAP hcar[numOfCar];
CImage Ibackground;
CImage Icar[numOfCar];

int main()
{
	int	nWidth, nHeight;//윈도우 크기 

	hInst = (HINSTANCE)::GetModuleHandle(NULL);

	//윈도우창 최대 크기 구하기
	nWidth = GetSystemMetrics(SM_CXSCREEN); 
	nHeight = GetSystemMetrics(SM_CXSCREEN); 

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
		, "차영화관"
	};

	::RegisterClass(&wc);


	hWnd = CreateWindow("차영화관"
		, "차차차차"
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

HBITMAP loadBitmap(const wchar_t* path)
{
	HBITMAP tBmp = NULL;
	ULONG_PTR token = 0;
	Gdiplus::GdiplusStartupInput input = NULL;
	Gdiplus::GdiplusStartup(&token, &input, NULL);
	if (token != 0)
	{
		Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(path);
		bmp->GetHBITMAP(Gdiplus::Color::Transparent, &tBmp);
		delete bmp;
		Gdiplus::GdiplusShutdown(token);
	}
	return tBmp;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	

	if (msg == WM_CREATE)
	{
		/*getPNGhBitmap(&hbackground, backgroundImagePath);

		for (int i = 0; i < numOfCar; i++)
			getPNGhBitmap(&hcar[i], carImagePath);*/
		
		//쓰레드 생성
		_createThread();
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
		//BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, crt.right, crt.bottom, RGB(0, 0, 0));		
		SelectObject(MemDC, OldBit);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
		
		return 0;
		
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}




void DestoryThread()
{
	//차쓰레드 파괴
	for (int i = 0; i < numOfCar; i++)
		CloseHandle(carThread[i]);
}



void getPNGhBitmap(HBITMAP *hbitmap,char *path)
{
	CImage cPNGImage;
	HRESULT hRes = S_OK;

	hRes = cPNGImage.Load(path);
	if (hRes != S_OK)
	{
		MessageBox(hWnd, "hRes Failed", NULL, NULL);
		return;
	}
	
	*hbitmap = cPNGImage.Detach();
}

void _createThread()
{
	int posX = 100, posY = 100;

	//쓰레드 인자 초기화
	ZeroMemory(arg, sizeof(arg));

	//쓰레드 생성
	for (int i = 0; i < numOfCar; i++)
	{
		printf("[Info] %d Thread created\n", i);
		arg[i].id = i;
		arg[i].posX = posX;
		arg[i].posY = posY;
		posY += 40;
		posX += 40;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);
		
	}
}