#ifndef __CARTHREAD_H__
#define __CARTHREAD_H__

#define numOfCar 7

#include <process.h>
#include <Windows.h>

typedef struct carArg
{
	int id;
	int posX;
	int posY;
}carArg;


//������ �Լ�
unsigned WINAPI car(void *arg);

#endif