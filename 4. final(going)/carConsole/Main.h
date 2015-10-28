#ifndef __MAIN_H__
#define __MAIN_H__
#define SPEED 5

#define WIDTH 1024
#define HEIGHT 768

#include <Windows.h>
#include "carThread.h"

//�̹������
extern char			*backgroundImagePath;
extern char			*carReaderDownImagePath; 
extern char			*carReaderUpImagePath;
extern char			*car0rightImagePath;
extern char			*car0leftImagePath;
extern char			*car0upImagePath;
extern char			*car0downImagePath;

extern HINSTANCE	hInst;
extern HWND			hWnd;
extern HBITMAP		hbackbit;
extern HBITMAP		hbackground;
extern HBITMAP		hcar[numOfCar];

//�����νı�
extern HANDLE		carReader;
extern bool			ReaderDown;

//�Լ�
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DestoryThread();


#endif