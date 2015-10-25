#include <stdio.h>
#include "carThread.h"
#include "Main.h"
#include "Collision.h"

////충돌관련
//extern int			collisionBuf[WIDTH][HEIGHT];
//extern HANDLE		collisionMutex;
//
//
//
//extern HBITMAP		hbackbit;
//extern HANDLE		carThread[numOfCar];
//extern carArg		arg[numOfCar];
//
////동기화
//extern HANDLE		SEMA_turnel; //터널 카운터 동기화
//extern HANDLE		Hellow_READER;
//extern HANDLE		Leave_READER;

unsigned WINAPI CreateCarThread(void *ARG)
{
	RECT rt;

	int posX = 0, posY = 670;
	int ID = 0;

	//차 쓰레드 인자 초기화
	ZeroMemory(arg, sizeof(arg));

	
	for (int i = 0; i < numOfCar; i++)
	{
		arg[i].id = i;

		if (i == 0)
			arg[i].posX = CAR1WIDTH;
		else
			arg[i].posX = 0;

		arg[i].posY = posY;
	}

	for (int i = 0; i < numOfCar; i++)
	{
		//차지하는 영역 
		arg[i].rect.left = arg[i].posX;
		arg[i].rect.top = arg[i].posY;
		arg[i].rect.right = arg[i].posX + CAR1WIDTH;
		arg[i].rect.bottom = arg[i].posY + CAR1HEIGHT;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);

		Sleep(500);
	}	

	//차쓰레드가 모두 종료할때까지 대기한다
	WaitForMultipleObjects(numOfCar, carThread, TRUE, INFINITE);

	return 0;
}

void destoryCarThread()
{
	//차쓰레드 파괴
	for (int i = 0; i < numOfCar; i++)
		CloseHandle(carThread[i]);
}

//차 쓰레드
unsigned WINAPI car(void *arg)
{
	carArg *Argument = ((carArg*)arg);

	//터널에 n명이상 있으면 내 차례가 될때까지 도로에서 대기
	WaitForSingleObject(SEMA_turnel, INFINITE);
	//터널에 들어감
	enterTurnel(Argument);	
	//터널들어가라고 신호줌
	ReleaseSemaphore(SEMA_turnel, 1, NULL);
	printf("[INFO] %d Thread done\n", Argument->id);

	return 0;
}

void enterTurnel(carArg *Arg)
{
	int a = 10, b = 0;

	while (Arg->posX < 700)
	{
		WaitForSingleObject(collisionMutex, INFINITE);
		if (!testCollision(Arg->id, &Arg->rect))
		{		
			setCollision(Arg, a, b);
			Update();
		}
		ReleaseMutex(collisionMutex);
		Sleep(100);	
	}
}