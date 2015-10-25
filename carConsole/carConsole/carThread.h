#ifndef __CARTHREAD_H__
#define __CARTHREAD_H__

#define numOfCar	30
#define CAR1HEIGHT	32
#define CAR1WIDTH	95 

#include <process.h>
#include <Windows.h>

typedef struct carArg
{
	int id;
	int posX;
	int posY;
	RECT rect;
}carArg;

//치 쓰레드 함수
unsigned WINAPI CreateCarThread(void *ARG);
void destoryCarThread();
unsigned WINAPI car(void *arg);

//행동함수
void enterTurnel(carArg *Arg);

#endif