#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <Windows.h>

//���� ���� 
//0 : ���� 
//1 : ������ 
//2 : ��
//3 : �Ʒ�


//Screen
void Update();
void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode);
void getPNGhBitmap(HBITMAP *hbitmap, char *path);
void getMaxScreen(int *width, int *height);


#endif