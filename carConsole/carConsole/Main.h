#ifndef __MAIN_H__
#define __MAIN_H__
#define WIDTH 1024
#define HEIGHT 768
#define MAXOFTURNEL 2 //터널 카운트 설정

#include <Windows.h>
#include "carThread.h"

//충돌체크 배열
extern int			collisionBuf[WIDTH][HEIGHT];

//충돌체크 뮤텍스
extern HANDLE		collisionMutex;

extern char			*carImagePath;
extern char			*backgroundImagePath;

extern HINSTANCE	hInst;
extern HWND			hWnd;
extern HBITMAP		hbackbit;
extern HBITMAP		hbackground;
extern HBITMAP		hcar[numOfCar];

//차쓰레드 관련
extern HANDLE		carThread[numOfCar];
extern carArg		arg[numOfCar];

//차량인식기
extern HANDLE		carReader;

//세마포어
extern HANDLE		SEMA_turnel; //터널 카운터 동기화
extern HANDLE		Hellow_READER;
extern HANDLE		Leave_READER;

//함수
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DestoryThread();
void SemaphoreInit();
void DestorySemaphore();

//Screen
void Update();
void getPNGhBitmap(HBITMAP *hbitmap, char *path);
void getMaxScreen(int *width, int *height);

#endif