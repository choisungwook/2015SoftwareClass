#include "Main.h"
#include "Screen.h"
#include <atlimage.h> //CImage

//화면의 정보를 업데이트함
void Update()
{
	HDC hdc, MemDC;
	HDC BitDC;
	HBITMAP OldBit;
	RECT crt;


	//초기화
	GetClientRect(hWnd, &crt);
	hdc = GetDC(hWnd);
	if (hbackbit == NULL)
		hbackbit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	MemDC = CreateCompatibleDC(hdc);
	BitDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);	

	//배경화면 그리기
	drawOnMemory(backgroundImagePath, BitDC, MemDC, 0, 0, 1);

	//차량인식기 그리기
	if (ReaderDown)	
		drawOnMemory(carReaderDownImagePath, BitDC, MemDC, WIDTH / 2 + 250, HEIGHT / 2 + 250, 2);	
	else	
		drawOnMemory(carReaderUpImagePath, BitDC, MemDC, WIDTH / 2 + 410, HEIGHT / 2 + 110, 2);
	
	//차그리기
	for (int i = 0; i < numOfCar; i++)
	{
		switch (arg[i].direction)
		{
		case 0: //오른쪽
			if (arg[i].sort == 0)
				drawOnMemory(car0rightImagePath, BitDC, MemDC, arg[i].posX, arg[i].posY, 2);			
			break;
		case 1: //왼쪽
			break;
		case 2: //위
			if (arg[i].sort == 0)
				drawOnMemory(car0upImagePath, BitDC, MemDC, arg[i].posX, arg[i].posY, 2);
			break;
		}
		
	}
	

	//종료
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
