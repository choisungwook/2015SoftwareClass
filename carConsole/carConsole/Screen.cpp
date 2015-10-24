#include "Main.h"
#include <atlimage.h> //CImage

extern HBITMAP		hbackbit;
extern HBITMAP		hbackground;
extern HBITMAP		hcar[numOfCar];

extern carArg		arg[numOfCar];

extern char			*carImagePath;
extern char			*backgroundImagePath;

void DrawonMemory()
{
	HDC hdc = GetDC(hWnd);
	HDC hbackDC = NULL;
	HDC hMemDC = NULL;
	HBITMAP holdbitmap = NULL;

	//�ʱ�ȭ
	hbackDC = CreateCompatibleDC(hdc); //�ĸ���� ����
	hbackbit = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
	hMemDC = CreateCompatibleDC(hdc);
	holdbitmap = (HBITMAP)SelectObject(hMemDC, hbackbit);

	//������۸� �۾�
	//1.���
	SelectObject(hbackDC, hbackground);
	BitBlt(hMemDC, 0, 0, WIDTH, HEIGHT, hbackDC, 0, 0, SRCCOPY);

	//2. 
	for (int i = 0; i < numOfCar; i++)
	{
		SelectObject(hbackDC, hcar[i]);
		BitBlt(hMemDC, 100, 150, arg[i].posX, arg[i].posY, hbackDC, 0, 0, SRCCOPY);
	}

	DeleteDC(hbackDC);

	//����
	DeleteObject(holdbitmap);	
	DeleteDC(hMemDC);
	ReleaseDC(hWnd, hdc);
}

void Update()
{
	HDC hdc, MemDC;
	HDC BitDC;
	BITMAP bitmap;
	HBITMAP MyBit, OldBit;
	RECT crt;
	int bw, bh;

	GetClientRect(hWnd, &crt);
	hdc = GetDC(hWnd);
	if (hbackbit == NULL)
		hbackbit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	MemDC = CreateCompatibleDC(hdc);
	BitDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);	
	
	CImage backgroundimg;
	backgroundimg.Load(backgroundImagePath);
	OldBit = (HBITMAP)SelectObject(BitDC, backgroundimg);
	bw = backgroundimg.GetWidth();
	bh = backgroundimg.GetHeight();
	//MyBit = (HBITMAP)LoadImage(hInst, "background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	/*OldBit = (HBITMAP)SelectObject(BitDC, MyBit);
	GetObject(MyBit, sizeof(BITMAP), &bitmap);
	bw = bitmap.bmWidth;
	bh = bitmap.bmHeight;*/
	BitBlt(MemDC, 0, 0, bw, bh, BitDC, 0, 0, SRCCOPY);
	DeleteObject(SelectObject(BitDC, OldBit));

	//���׸���
	for (int i = 0; i < numOfCar; i++)
	{		
		MyBit = (HBITMAP)LoadImage(hInst, "car.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);				
		OldBit = (HBITMAP)SelectObject(BitDC,   MyBit);
		GetObject(MyBit, sizeof(BITMAP), &bitmap);
		bw = bitmap.bmWidth;
		bh = bitmap.bmHeight;		
		//TransparentBlt(MemDC, arg[i].posX, arg[i].posY, bw, bh, BitDC, bw, bh, 0, 0, RGB(255, 255, 255));		
		BitBlt(MemDC, arg[i].posX, arg[i].posY, bw, bh, BitDC, 0, 0, SRCCOPY);
		DeleteObject(SelectObject(BitDC, OldBit));
	}
	


	//����
	DeleteDC(MemDC);
	DeleteDC(BitDC);
	ReleaseDC(hWnd, hdc);
	InvalidateRect(hWnd, NULL, FALSE);
}