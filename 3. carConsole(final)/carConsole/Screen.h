#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <Windows.h>

//차량 방향 
//0 : 왼쪽 
//1 : 오른쪽 
//2 : 위
//3 : 아래


//Screen
void Update();
void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode);
void getPNGhBitmap(HBITMAP *hbitmap, char *path);
void getMaxScreen(int *width, int *height);


#endif