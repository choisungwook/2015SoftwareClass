#ifndef __SCRENN_H__
#define __SCRENN_H__

#include	"enum.h"
#include	<Windows.h>


//���� ���� 
//0 : ���� 
//1 : ������ 
//2 : ��
//3 : �Ʒ�

void Update();
void drawOnMemory(char* Imagepath, HDC BackDC, HDC MemDC, int memoryX, int memoryY, int mode);

//������ �׸���
void drawcar0(int carID, HDC bitDC, HDC memDC);
#endif