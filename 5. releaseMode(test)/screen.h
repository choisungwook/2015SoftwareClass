#ifndef __SCRENN_H__
#define __SCRENN_H__

#include	"enum.h"
#include	<Windows.h>


//차량 방향 
//0 : 왼쪽 
//1 : 오른쪽 
//2 : 위
//3 : 아래

void Update();
void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode);

//차종류 그리기
void drawcar0(int carID, HDC bitDC, HDC memDC);
#endif