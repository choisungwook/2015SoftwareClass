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

//ġ ������ �Լ�
unsigned WINAPI CreateCarThread(void *ARG);
void destoryCarThread();
unsigned WINAPI car(void *arg);

//�ൿ�Լ�
void enterTurnel(carArg *Arg);

#endif