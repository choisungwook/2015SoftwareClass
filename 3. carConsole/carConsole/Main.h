#ifndef __MAIN_H__
#define __MAIN_H__
#define SPEED 10
#define WIDTH 1024
#define HEIGHT 768
#define MAXOFTURNEL 2 //�ͳ� ī��Ʈ ����


#include <Windows.h>
#include "carThread.h"

//�浹üũ ���ؽ�
extern HANDLE		collisionMutex;

//�̹������
extern char			*backgroundImagePath;
extern char			*carReaderDownImagePath; 
extern char			*carReaderUpImagePath;
extern char			*car0rightImagePath;
extern char			*car0upImagePath;


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
extern bool			ReaderDown;

//��������
extern HANDLE		SEMA_turnel; //�ͳ� ī���� ����ȭ
extern HANDLE		Hellow_READER;
extern HANDLE		Leave_READER;
extern HANDLE		Enter_READER;
extern HANDLE		hDown_READER;

//�Լ�
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DestoryThread();
void SemaphoreInit();
void DestorySemaphore();



#endif