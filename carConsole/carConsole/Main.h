#ifndef __MAIN_H__
#define __MAIN_H__
#define WIDTH 1024
#define HEIGHT 768
#define MAXOFTURNEL 2 //�ͳ� ī��Ʈ ����

#include <Windows.h>
#include "carThread.h"

//�浹üũ �迭
extern int			collisionBuf[WIDTH][HEIGHT];

//�浹üũ ���ؽ�
extern HANDLE		collisionMutex;

extern char			*carImagePath;
extern char			*backgroundImagePath;

extern HINSTANCE	hInst;
extern HWND			hWnd;
extern HBITMAP		hbackbit;
extern HBITMAP		hbackground;
extern HBITMAP		hcar[numOfCar];

//�������� ����
extern HANDLE		carThread[numOfCar];
extern carArg		arg[numOfCar];

//�����νı�
extern HANDLE		carReader;

//��������
extern HANDLE		SEMA_turnel; //�ͳ� ī���� ����ȭ
extern HANDLE		Hellow_READER;
extern HANDLE		Leave_READER;

//�Լ�
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DestoryThread();
void SemaphoreInit();
void DestorySemaphore();

//Screen
void Update();
void getPNGhBitmap(HBITMAP *hbitmap, char *path);
void getMaxScreen(int *width, int *height);

#endif