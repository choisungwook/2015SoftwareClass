#include "Main.h"

extern HBITMAP		hbackbit;

void DrawonMemory(HDC hdc, HBITMAP hdst, int width, int height)
{
	HDC hbackDC = NULL;	
	HDC hMemDC = NULL;
	HBITMAP holdbitmap = NULL;
	
	//�ʱ�ȭ
	hbackDC = CreateCompatibleDC(hdc); //�ĸ���� ����
	hbackbit = CreateCompatibleBitmap(hdc, width, height);
	hMemDC = CreateCompatibleDC(hdc);
	holdbitmap = (HBITMAP)SelectObject(hMemDC, hbackbit);
	
	//������۸� �۾�
	SelectObject(hbackDC, hdst);	
	BitBlt(hMemDC, 0, 0, width, height, hbackDC, 0, 0, SRCCOPY);
	DeleteDC(hbackDC);

	//����
	DeleteObject(holdbitmap);	
	DeleteDC(hMemDC);
}