#include "Main.h"
#include <stdio.h>
#include <atlimage.h> //CImage
#include "carThread.h"




char		*backgroundImagePath	= "C:\\Users\\choisunguk\\Pictures\\softwareclass\\background2.png";
char		*carImagePath			= "C:\\Users\\choisunguk\\Pictures\\car4.png";
HINSTANCE	hInst;
HWND		hWnd;
HBITMAP		hbackbit;

//차쓰레드
HANDLE carThread[numOfCar];
//차의 좌표
carArg arg[numOfCar];


HBITMAP hbackground;
HBITMAP carbitmap[numOfCar];
CImage Ibackground;
CImage Icar[numOfCar];

//INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
int main()
{
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
		, WIDTH
		, HEIGHT
		, NULL
		, NULL
		, hInst
		, NULL);


	::ShowWindow(hWnd, SW_SHOW);
	::UpdateWindow(hWnd);
	::ShowCursor(TRUE);

	_createThread();

	//////////////////////////////////////////////
	// 이미지 가져오기
	/////////////////////////////////////////////

	//자동차 이미지
	/*for (int i = 0; i < numOfCar; i++)
		getPNGhBitmap(&carbitmap[i], carImagePath);*/


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
	/*HBITMAP hbackground;
	HBITMAP OldBitmap;*/
	
	if (msg == WM_CREATE)
	{
		////////////////////////////////////////
		//배경이미지 초기화
		////////////////////////////////////////
		//Ibackground.Load(backgroundImagePath);
		////자동차
		//for (int i = 0; i < numOfCar; i++)
		//	Icar[i].Load(carImagePath);

		getPNGhBitmap(&hbackground, backgroundImagePath);
	}

	else if (msg == WM_DESTROY)
	{
		DestoryThread();
		::PostQuitMessage(0);		
		return 0;
	}

	else if (msg == WM_PAINT)
	{			
		PAINTSTRUCT ps = { 0, };
		HDC hdc = NULL, MemDC = NULL;
		HBITMAP holdbit = NULL;

		//초기화
		hdc = BeginPaint(hWnd, &ps);		
		MemDC = CreateCompatibleDC(hdc);	
		
		DrawonMemory(hdc, hbackground, WIDTH, HEIGHT); //배경 그리기

		//후면 버퍼 전면으로 옮기기
		holdbit = (HBITMAP)SelectObject(MemDC, hbackbit);
		/*SelectObject(MemDC, hbackground);
		TransparentBlt(hdc, 0, 0, WIDTH, HEIGHT, MemDC, 0, 0, WIDTH, HEIGHT, RGB(255, 255, 255));*/
		TransparentBlt(hdc, 0, 0, WIDTH, HEIGHT, MemDC, 0, 0, WIDTH, HEIGHT, RGB(255, 255, 255)); 

		//삭제
		SelectObject(MemDC, holdbit);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
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
		posY += 20;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);
	}
}