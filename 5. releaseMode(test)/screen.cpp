#include "screen.h"
#include "main.h"
#include "carThread.h"
using namespace std;

void(*carfunc[sortOfcar])(int, HDC, HDC) = 
{
	drawcar0,
};

//ȭ���� ������ ������Ʈ��
void Update()
{
	HDC hdc, MemDC;
	HDC BitDC;
	HBITMAP OldBit;
	RECT crt;

	//�ʱ�ȭ
	GetClientRect(hWndMain, &crt);
	hdc = GetDC(hWndMain);
	if (hbackbit == NULL)
		hbackbit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	MemDC = CreateCompatibleDC(hdc);
	BitDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);

	//���ȭ�� �׸���
	drawOnMemory(backgroundImagePath, BitDC, MemDC, 0, 0, 1);

	////�����νı� �׸���
	//if (ReaderDown)
	//	drawOnMemory(carReaderDownImagePath, BitDC, MemDC, 10, HEIGHT / 2 + 270, 2);
	//else
	//	drawOnMemory(carReaderUpImagePath, BitDC, MemDC, 11, HEIGHT / 2 + 200, 2);

	//���� �׸���
	for (unsigned int i = 0; i < numOfcar; i++)	
		carfunc[carArg[i].sort](i,BitDC,MemDC);
	
	//for (int i = 0; i < numOfcar; i++)
	//{
	//	
	//}
	////���׸���
	//for (int i = 0; i < numOfCar; i++)
	//{
	//	switch (arg[i].direction)
	//	{
	//	case 0: //����
	//		if (arg[i].sort == 0)
	//			drawOnMemory(car0leftImagePath, BitDC, MemDC, arg[i].posX, arg[i].posY, 2);
	//		break;
	//	case 1: //������
	//		if (arg[i].sort == 0)
	//			drawOnMemory(car0rightImagePath, BitDC, MemDC, arg[i].posX, arg[i].posY, 2);
	//		break;
	//	case 2: //��
	//		if (arg[i].sort == 0)
	//			drawOnMemory(car0upImagePath, BitDC, MemDC, arg[i].posX, arg[i].posY, 2);
	//		break;
	//	case 3: //�Ʒ�
	//		if (arg[i].sort == 0)
	//			drawOnMemory(car0downImagePath, BitDC, MemDC, arg[i].posX, arg[i].posY, 2);
	//		break;
	//	}

	//}


	//����
	DeleteDC(MemDC);
	DeleteDC(BitDC);
	ReleaseDC(hWndMain, hdc);
	InvalidateRect(hWndMain, NULL, FALSE);
}

void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode)
{
	HBITMAP ImageBit = (HBITMAP)LoadImage(g_hInst, Imagepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
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

////������
void drawcar0(int carID, HDC bitDC, HDC memDC)
{
	//����
	if (carArg[carID].direction == 0)
		drawOnMemory(car0leftImagePath, bitDC, memDC, carArg[carID].posX, carArg[carID].posY, 2);
	//������
	else if (carArg[carID].direction == 1)
		drawOnMemory(car0rightImagePath, bitDC, memDC, carArg[carID].posX, carArg[carID].posY, 2);
	//��
	if (carArg[carID].direction == 2)
		drawOnMemory(car0upImagePath, bitDC, memDC, carArg[carID].posX, carArg[carID].posY, 2);
	//�Ʒ�
	if (carArg[carID].direction == 3)
		drawOnMemory(car0downImagePath, bitDC, memDC, carArg[carID].posX, carArg[carID].posY, 2);
}
