#include "Main.h"
#include "Screen.h"
#include <atlimage.h> //CImage

//ȭ���� ������ ������Ʈ��
void Update()
{
	HDC hdc, MemDC;
	HDC BitDC;
	HBITMAP OldBit;
	RECT crt;


	//�ʱ�ȭ
	GetClientRect(hWnd, &crt);
	hdc = GetDC(hWnd);
	if (hbackbit == NULL)
		hbackbit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	MemDC = CreateCompatibleDC(hdc);
	BitDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);	

	//���ȭ�� �׸���
	drawOnMemory(backgroundImagePath, BitDC, MemDC, 0, 0, 1);

	//�����νı� �׸���
	if (ReaderDown)	
		drawOnMemory(carReaderDownImagePath, BitDC, MemDC, WIDTH / 2 + 250, HEIGHT / 2 + 250, 2);	
	else	
		drawOnMemory(carReaderUpImagePath, BitDC, MemDC, WIDTH / 2 + 410, HEIGHT / 2 + 110, 2);
	
	//���׸���
	for (int i = 0; i < numOfCar; i++)
	{
		switch (arg[i].direction)
		{
		case 0: //������
			if (arg[i].sort == 0)
				drawOnMemory(car0rightImagePath, BitDC, MemDC, arg[i].posX, arg[i].posY, 2);			
			break;
		case 1: //����
			break;
		case 2: //��
			if (arg[i].sort == 0)
				drawOnMemory(car0upImagePath, BitDC, MemDC, arg[i].posX, arg[i].posY, 2);
			break;
		}
		
	}
	

	//����
	DeleteDC(MemDC);
	DeleteDC(BitDC);
	ReleaseDC(hWnd, hdc);
	InvalidateRect(hWnd, NULL, FALSE);
}

void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode)
{
	HBITMAP ImageBit	= (HBITMAP)LoadImage(hInst, Imagepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);	
	HBITMAP OldBit		= (HBITMAP)SelectObject(BackDC, ImageBit);
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
