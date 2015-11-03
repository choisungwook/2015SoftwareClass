#include "screen.h"
#include <Windows.h>


extern HWND			hWndMain;
extern HBITMAP		hbackbit; //백버퍼
extern HINSTANCE	hInst;

void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode);

void Update()
{
	HDC hdc, MemDC;
	HDC BitDC;
	HBITMAP OldBit;
	RECT crt;

	//초기화
	GetClientRect(hWndMain, &crt);
	hdc = GetDC(hWndMain);
	if (hbackbit == NULL)
		hbackbit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	MemDC = CreateCompatibleDC(hdc);
	BitDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);

	////배경화면 그리기
	//drawOnMemory(backgroundImagePath, BitDC, MemDC, 0, 0, 1);

	//////차량인식기 그리기
	//if (upReader)
	//	drawOnMemory(carReaderUpImagePath, BitDC, MemDC, 10, 580, 2);
	//else
	//	drawOnMemory(carReaderDownImagePath, BitDC, MemDC, 11, 650, 2);

	//자동차그리기



	//종료
	DeleteDC(MemDC);
	DeleteDC(BitDC);
	ReleaseDC(hWndMain, hdc);
	InvalidateRect(hWndMain, NULL, FALSE);
}


void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode)
{
	HBITMAP ImageBit = (HBITMAP)LoadImage(hInst, Imagepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HBITMAP OldBit = (HBITMAP)SelectObject(BackDC, ImageBit);
	BITMAP	bitmap;
	int bw, bh;

	GetObject(ImageBit, sizeof(BITMAP), &bitmap);
	bw = bitmap.bmWidth;
	bh = bitmap.bmHeight;

	if (mode == 1)
		BitBlt(MemDC, memoryX, memoryY, bw, bh, BackDC, 0, 0, SRCCOPY);
	else if (mode == 2)
		TransparentBlt(MemDC, memoryX, memoryY, bw, bh, BackDC, 0, 0, bw, bh, RGB(255, 255, 255));

	DeleteObject(SelectObject(BackDC, OldBit));
}

//////차종류
//void drawcar0(carArgument* data, HDC bitDC, HDC memDC)
//{
//	//왼쪽
//	if (data->direction == 0)
//		drawOnMemory(car0leftImagePath, bitDC, memDC, data->posX, data->posY, 2);
//	//오른쪽
//	else if (data->direction == 1)
//		drawOnMemory(car0rightImagePath, bitDC, memDC, data->posX, data->posY, 2);
//	//위
//	if (data->direction == 2)
//		drawOnMemory(car0upImagePath, bitDC, memDC, data->posX, data->posY, 2);
//	//아래
//	if (data->direction == 3)
//		drawOnMemory(car0downImagePath, bitDC, memDC, data->posX, data->posY, 2);
//
//}
//
