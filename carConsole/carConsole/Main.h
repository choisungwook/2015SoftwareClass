#ifndef __MAIN_H__
#define __MAIN_H__
#define WIDTH 1024
#define HEIGHT 768
#define CAR_WIDTH 100
#define CAR_HEIGHT 53


#include <Windows.h>
#include "carThread.h"
extern char			*carImagePath;
extern char			*backgroundImagePath;

extern HINSTANCE	hInst;
extern HWND			hWnd;
extern HBITMAP		hbackbit;
extern HBITMAP		hbackground;
extern HBITMAP		hcar[numOfCar];


extern carArg		arg[numOfCar];

//ÇÔ¼ö
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void getPNGhBitmap(HBITMAP *hbitmap, char *path);
void DestoryThread();
void _createThread();


//test
void DrawonMemory();
void Update();


#endif