#ifndef __CARTHREAD_H__
#define __CARTHREAD_H__

#define numOfCar	4
#define CAR00HEIGHT	32 //차량 0번의 방향 오른쪽 세로
#define CAR00WIDTH	100 //차량 0번의 방향 오른쪽 가로
#define CAR02HEIGHT	68 //차량 0번의 방향 위 세로
#define CAR02WIDTH	66 //차량 0번의 방향 위 가로

#include <process.h>
#include <Windows.h>

typedef struct carArg
{	
	int id;
	int posX;
	int posY;
	RECT rect;
	int sort;
	int direction;
}carArg;

//치 쓰레드 함수
unsigned WINAPI CreateCarThread(void *ARG);
void destoryCarThread();
unsigned WINAPI car(void *arg);

//행동함수
void enterTurnel(carArg *Arg);
void TalktoReader(carArg *Arg);

#endif