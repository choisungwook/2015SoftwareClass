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


//쓰레드 함수
unsigned WINAPI car(void *arg);

#endif