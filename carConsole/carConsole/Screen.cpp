#include "Main.h"
#include <atlimage.h> //CImage

extern HBITMAP		hbackbit;
extern HBITMAP		hbackground;
extern HBITMAP		hcar[numOfCar];

extern carArg		arg[numOfCar];

extern char			*carImagePath;
extern char			*backgroundImagePath;


//ȭ���� ������ ������Ʈ��
void Update()
{
	HDC hdc, MemDC;
	HDC BitDC;
	BITMAP bitmap;
	HBITMAP MyBit, OldBit;
	RECT crt;
	int bw, bh;

	//�ʱ�ȭ
	GetClientRect(hWnd, &crt);
	hdc = GetDC(hWnd);
	if (hbackbit == NULL)
		hbackbit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	MemDC = CreateCompatibleDC(hdc);
	BitDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);	

	//�̹����׸���
	MyBit = (HBITMAP)LoadImage(hInst, "background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	OldBit = (HBITMAP)SelectObject(BitDC, MyBit);
	GetObject(MyBit, sizeof(BITMAP), &bitmap);
	bw = bitmap.bmWidth;
	bh = bitmap.bmHeight;
	BitBlt(MemDC, 0, 0, bw, bh, BitDC, 0, 0, SRCCOPY);
	//TransparentBlt(MemDC, 0, 0, bw, bh, BitDC, 0, 0, bw, bh, RGB(0, 0, 0));
	DeleteObject(SelectObject(BitDC, OldBit));

	//���׸���
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

	//����
	DeleteDC(MemDC);
	DeleteDC(BitDC);
	ReleaseDC(hWnd, hdc);
	InvalidateRect(hWnd, NULL, FALSE);
}


//png������ ������ hbitmap���� ��ȯ
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


//�ش� PC�� �ִ� �ػ�ũ�⸦ ����
void getMaxScreen(int *width, int *height)
{
	//������â �ִ� ũ�� ���ϱ�
	*width = GetSystemMetrics(SM_CXSCREEN);
	*height = GetSystemMetrics(SM_CXSCREEN);
}
