#ifndef __MAIN_H__
#define __MAIN_H__
#define SPEED 10
#define WIDTH 1024
#define HEIGHT 768
#define MAXOFTURNEL 4 //터널 카운트 설정

#include <Windows.h>
#include "carThread.h"


//이미지경로
extern char			*backgroundImagePath;
extern char			*carReaderDownImagePath; 
extern char			*carReaderUpImagePath;
extern char			*car0rightImagePath;
extern char			*car0leftImagePath;
extern char			*car0upImagePath;


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
extern bool			ReaderDown;

//세마포어
extern HANDLE		SEMA_turnel; //터널 카운터 동기화
extern HANDLE		Hellow_READER;
extern HANDLE		Leave_READER;
extern HANDLE		Enter_READER;
extern HANDLE		hDown_READER;

//뮤텍스
extern HANDLE		collisionMutex; //충돌체크 뮤텍스
extern HANDLE		screenMutex;


//함수
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DestoryThread();
void SemaphoreInit();
void DestorySemaphore();



#endif