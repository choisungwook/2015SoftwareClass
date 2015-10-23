#include "Main.h"

extern HBITMAP		hbackbit;

void DrawonMemory(HDC hdc, HBITMAP hdst, int width, int height)
{
	HDC hbackDC = NULL;	
	HDC hMemDC = NULL;
	HBITMAP holdbitmap = NULL;
	
	//초기화
	hbackDC = CreateCompatibleDC(hdc); //후면버퍼 생성
	hbackbit = CreateCompatibleBitmap(hdc, width, height);
	hMemDC = CreateCompatibleDC(hdc);
	holdbitmap = (HBITMAP)SelectObject(hMemDC, hbackbit);
	
	//더블버퍼링 작업
	SelectObject(hbackDC, hdst);	
	BitBlt(hMemDC, 0, 0, width, height, hbackDC, 0, 0, SRCCOPY);
	DeleteDC(hbackDC);

	//삭제
	DeleteObject(holdbitmap);	
	DeleteDC(hMemDC);
}