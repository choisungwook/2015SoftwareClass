#ifndef __CARTHREAD_H__
#define __CARTHREAD_H__

#define numOfCar 5

#include <process.h>
#include <Windows.h>

typedef struct carArg
{
	int id;
	int posX;
	int posY;
}carArg;

//ġ ������ �Լ�
unsigned WINAPI CreateCarThread(void *ARG);
void destoryCarThread();
unsigned WINAPI car(void *arg);

//�ൿ�Լ�
void enterTurnel(carArg *Arg);

#endif