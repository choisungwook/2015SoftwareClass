#include "Main.h"
#include <atlimage.h> //CImage

extern HBITMAP		hbackbit;
extern HBITMAP		hbackground;
extern HBITMAP		hcar[numOfCar];

extern carArg		arg[numOfCar];

extern char			*carImagePath;
extern char			*backgroundImagePath;


//화면의 정보를 업데이트함
void Update()
{
	HDC hdc, MemDC;
	HDC BitDC;
	BITMAP bitmap;
	HBITMAP MyBit, OldBit;
	RECT crt;
	int bw, bh;

	//초기화
	GetClientRect(hWnd, &crt);
	hdc = GetDC(hWnd);
	if (hbackbit == NULL)
		hbackbit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	MemDC = CreateCompatibleDC(hdc);
	BitDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);	

	//이미지그리기
	MyBit = (HBITMAP)LoadImage(hInst, "background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	OldBit = (HBITMAP)SelectObject(BitDC, MyBit);
	GetObject(MyBit, sizeof(BITMAP), &bitmap);
	bw = bitmap.bmWidth;
	bh = bitmap.bmHeight;
	BitBlt(MemDC, 0, 0, bw, bh, BitDC, 0, 0, SRCCOPY);
	//TransparentBlt(MemDC, 0, 0, bw, bh, BitDC, 0, 0, bw, bh, RGB(0, 0, 0));
	DeleteObject(SelectObject(BitDC, OldBit));

	//차그리기
	for (int i = 0; i < numOfCar; i++)
	{		
		MyBit = (HBITMAP)LoadImage(hInst, "car.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		OldBit = (HBITMAP)SelectObject(BitDC,   MyBit);
		GetObject(MyBit, sizeof(BITMAP), &bitmap);
		bw = bitmap.bmWidth;
		bh = bitmap.bmHeight;				
		TransparentBlt(MemDC, arg[i].posX, arg[i].posY, bw, bh, BitDC, 0, 0, bw, bh, RGB(255, 255, 255));		
		//BitBlt(MemDC, arg[i].posX, arg[i].posY, bw, bh, BitDC, 0, 0, SRCCOPY);
		DeleteObject(SelectObject(BitDC, OldBit));
	}

	//종료
	DeleteDC(MemDC);
	DeleteDC(BitDC);
	ReleaseDC(hWnd, hdc);
	InvalidateRect(hWnd, NULL, FALSE);
}


//png파일의 정보를 hbitmap으로 변환
void getPNGhBitmap(HBITMAP *hbitmap, char *path)
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


//해당 PC의 최대 해상도크기를 구함
void getMaxScreen(int *width, int *height)
{
	//윈도우창 최대 크기 구하기
	*width = GetSystemMetrics(SM_CXSCREEN);
	*height = GetSystemMetrics(SM_CXSCREEN);
}
