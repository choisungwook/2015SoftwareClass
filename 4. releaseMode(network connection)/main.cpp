#include "socket.h"
#include "main.h"
#include "enum.h"
#include "resource.h"
#include "carThread.h"
#include "handle.h"
#include "fileIO.h"
#include "utility.h"
#include <list>
using namespace std;

#pragma comment (lib, "msimg32.lib")

//local �Լ�
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

//local ����
LPCTSTR			lpszClass = TEXT("WM_INITDIALOG2");
HINSTANCE		g_hInst;
HWND			hWndMain;
HBITMAP			hbackbit; //�����

//carTread �������� ����
extern	int				numOfcar;
extern	int				numOfturnel;

//�����÷���
bool	exitFlag;



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


BOOL CALLBACK DlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	RECT wrt, crt;

	switch (iMessage) {
	case WM_INITDIALOG:
		GetWindowRect(GetParent(hDlg), &wrt);
		GetWindowRect(hDlg, &crt);
		SetWindowPos(hDlg, HWND_NOTOPMOST, wrt.left + (wrt.right - wrt.left) / 2 - (crt.right - crt.left) / 2,
			wrt.top + (wrt.bottom - wrt.top) / 2 - (crt.bottom - crt.top) / 2, 0, 0, SWP_NOSIZE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:	
			destoryThreads(); //�������ı�
			destoryHandles(); //�ڵ��ı�
			//destorythread();	
			numOfcar = GetDlgItemInt(hDlg, IDC_EDIT_CAR, NULL, FALSE);
			numOfturnel = GetDlgItemInt(hDlg, IDC_EDIT_TURNEL, NULL, FALSE);
			EndDialog(hDlg, IDC_OK);
			
			initializeHandles(); //�ڵ��ʱ�ȭ			
			createThreads(); //���������
			return TRUE;

		case IDC_CANCEL:			
			EndDialog(hDlg, IDC_CANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		initializeWinsock();
		exitFlag = false;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_START:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
			break;
		case ID_FILE_SAVE:
			fileoutput();
			break;
		case ID_FILE_OPEN:
			MessageBox(hWndMain, "�غ���", "�غ���", MB_OK);
			break;
		case ID_FILE_EXIT:
			destoryThreads(); //�ڵ��ı�
			destoryHandles(); //�������ı�
			destoryWinsock(); //��������
			PostQuitMessage(0);
			break;
		}
		return 0;
	case WM_PAINT:
		HDC hdc, MemDC;
		RECT crt;
		HBITMAP OldBit;
		PAINTSTRUCT ps;

		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &crt);
		MemDC = CreateCompatibleDC(hdc);
		OldBit = (HBITMAP)SelectObject(MemDC, hbackbit);
		BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
		//TransparentBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, crt.right, crt.bottom, RGB(255, 255, 255));		
		SelectObject(MemDC, OldBit);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);

		return 0;
	case WM_DESTROY:			
		destoryThreads(); //�ڵ��ı�
		destoryHandles(); //�������ı�
		destoryWinsock();
		PostQuitMessage(0);
	
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

/////////////////////////////////////////////////////////////////
//ȭ�� ��¿� ���õ� �Լ���
/////////////////////////////////////////////////////////////////
//������
extern list<carArgument>		L_carArg;
//���ý�
extern	HANDLE					M_accessArg;
//��� ���� �����νı�
extern	bool topUpReader;
//�ϴ� �����νı�
extern	bool upReader;

void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode);
void drawcar0(list<carArgument>::iterator arg, HDC bitDC, HDC memDC);
void drawcar1(list<carArgument>::iterator arg, HDC bitDC, HDC memDC);
void drawcar2(list<carArgument>::iterator arg, HDC bitDC, HDC memDC);
void drawcar3(list<carArgument>::iterator arg, HDC bitDC, HDC memDC);

void(*carfunc[sortOfcar])(list<carArgument>::iterator, HDC, HDC) =
{
	drawcar0,
	drawcar1,
	drawcar2,
	drawcar3,
};


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
	if (upReader)
		drawOnMemory(carReaderUpImagePath, BitDC, MemDC, 10, 580, 2);
	else
		drawOnMemory(carReaderDownImagePath, BitDC, MemDC, 11, 650, 2);

	//��� �������
	if (topUpReader)
		drawOnMemory(TopcarReaderDownImagePath, BitDC, MemDC, 825, 25, 2);
	else
		drawOnMemory(TopcarReaderUpImagePath, BitDC, MemDC, 825, 25, 2);
	
	watiAndcheckExited(M_accessArg);
	list<carArgument>::iterator End = L_carArg.end();
	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		carfunc[iterPos->sort](iterPos, BitDC, MemDC);
	}
	ReleaseMutex(M_accessArg);

	//�ٸ� �׸���
	drawOnMemory(BridgeImagePath, BitDC, MemDC, 145, 245, 2);

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
void drawcar0(list<carArgument>::iterator arg, HDC bitDC, HDC memDC)
{
		//����
		if (arg->direction == 0)
			drawOnMemory(car0leftImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
		//������
		else if (arg->direction == 1)
			drawOnMemory(car0rightImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
		//��
		if (arg->direction == 2)
			drawOnMemory(car0upImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
		//�Ʒ�
		if (arg->direction == 3)
			drawOnMemory(car0downImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	
}

void drawcar1(list<carArgument>::iterator arg, HDC bitDC, HDC memDC)
{
	//����
	if (arg->direction == 0)
		drawOnMemory(car1leftImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//������
	else if (arg->direction == 1)
		drawOnMemory(car1rightImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//��
	if (arg->direction == 2)
		drawOnMemory(car1upImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//�Ʒ�
	if (arg->direction == 3)
		drawOnMemory(car1downImagePath, bitDC, memDC, arg->posX, arg->posY, 2);

}

void drawcar2(list<carArgument>::iterator arg, HDC bitDC, HDC memDC)
{
	//����
	if (arg->direction == 0)
		drawOnMemory(car2leftImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//������
	else if (arg->direction == 1)
		drawOnMemory(car2rightImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//��
	if (arg->direction == 2)
		drawOnMemory(car2upImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//�Ʒ�
	if (arg->direction == 3)
		drawOnMemory(car2downImagePath, bitDC, memDC, arg->posX, arg->posY, 2);

}

void drawcar3(list<carArgument>::iterator arg, HDC bitDC, HDC memDC)
{
	//����
	if (arg->direction == 0)
		drawOnMemory(car3leftImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//������
	else if (arg->direction == 1)
		drawOnMemory(car3rightImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//��
	if (arg->direction == 2)
		drawOnMemory(car3upImagePath, bitDC, memDC, arg->posX, arg->posY, 2);
	//�Ʒ�
	if (arg->direction == 3)
		drawOnMemory(car3downImagePath, bitDC, memDC, arg->posX, arg->posY, 2);

}




