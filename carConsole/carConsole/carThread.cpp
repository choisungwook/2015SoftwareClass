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
	int posX = 0, posY = 670;

	//차 쓰레드 인자 초기화
	ZeroMemory(arg, sizeof(arg));

	for (int i = 0; i < numOfCar; i++)
		arg[i].posY = posY;

	//차 쓰레드 생성
	for (int i = 0; i < numOfCar; i++)
	{
		arg[i].id = i;
		arg[i].posX = posX;
		arg[i].posY = posY;
		//posX += 100;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);
		
		Sleep(300);
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
	enterTurnel(Argument);
	//for (int i = 0; i < 8; i++)
	//{
	//	WaitForSingleObject(collisionMutex,INFINITE);

	//	if (collisionBuf[Argument->posX + 100][Argument->posY])
	//	{
	//		i--;
	//		ReleaseMutex(collisionMutex);
	//		continue;
	//	}
	//	else
	//	{
	//		collisionBuf[Argument->posX][Argument->posY] = 0;
	//		Argument->posX += 100;
	//		collisionBuf[Argument->posX][Argument->posY] = 1;
	//		ReleaseMutex(collisionMutex);
	//	}

	//	Sleep(100);
	//	//Argument->posX += 1;
	//	Update();		
	//}
	//
	////차량인식기에 나왔다고 통보
	//ReleaseSemaphore(Hellow_READER, 1, NULL);

	////뒤에 대기하던 차에게 터널로 들어오라고 신호줌	
	//ReleaseSemaphore(SEMA_turnel, 1, NULL);

	//for (int i = 0; i < 20; i++)
	//{
	//	WaitForSingleObject(collisionMutex, INFINITE);

	//	if (collisionBuf[Argument->posX][Argument->posY-30])
	//	{
	//		i--;
	//		ReleaseMutex(collisionMutex);
	//		continue;
	//	}
	//	else
	//	{
	//		collisionBuf[Argument->posX][Argument->posY] = 0;
	//		Argument->posY -= 30;
	//		collisionBuf[Argument->posX][Argument->posY] = 1;
	//		ReleaseMutex(collisionMutex);
	//	}

	//	Sleep(100);
	//	//Argument->posX += 1;
	//	Update();
	//}

	//터널
	ReleaseSemaphore(SEMA_turnel, 1, NULL);
	printf("[INFO] %d Thread done\n", Argument->id);

	

	
	return 0;
}

void enterTurnel(carArg *Arg)
{
	int a = 10, b = 0;

	while (Arg->posX < 700)
	{
		for (int i = 0; i < numOfCar; i++)
		{
			if (i == Arg->id)
				continue;

			if (!detectCollisionOfRight(Arg->id, Arg->posX, Arg->posY))
			{
				Update();
			}
		}
		/*if (!detectCollisionOfRight(Arg->id, Arg->posX, Arg->posY))
		{
			setCollision(&(*Arg).posX, &(*Arg).posY, a, b);
			Update();
			Sleep(300);
		}*/
		/*if (!detectCollision(Arg->posX + a, Arg->posY))
		{
			setCollision(&(*Arg).posX, &(*Arg).posY, a, b);
			Update();
			Sleep(300);
		}*/
	}
}