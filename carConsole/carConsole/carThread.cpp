#include <stdio.h>
#include "carThread.h"
#include "Main.h"

//충돌체크 배열
extern int			collisionBuf[WIDTH][HEIGHT];
//충돌체크 뮤텍스
extern HANDLE		collisionMutex;

extern HBITMAP		hbackbit;
extern HANDLE		carThread[numOfCar];
extern carArg		arg[numOfCar];
extern HANDLE		SEMA_turnel; //터널 카운터 동기화

unsigned WINAPI CreateCarThread(void *ARG)
{
	int posX = 0, posY = 670;

	//차 쓰레드 인자 초기화
	ZeroMemory(arg, sizeof(arg));

	//차 쓰레드 생성
	for (int i = 0; i < numOfCar; i++)
	{
		arg[i].id = i;
		arg[i].posX = posX;
		arg[i].posY = posY;
		//posX += 100;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);
		Sleep(1100);
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

	for (int i = 0; i <= 500; i+=10)
	{
		Sleep(100);
		//WaitForSingleObject(collisionMutex, INFINITE);
		Argument->posX += i;
		Update();
	}



	printf("[INFO] %d Thread done\n", Argument->id);

	//뒤에 대기하던 차에게 터널로 들어오라고 신호줌	
	ReleaseSemaphore(SEMA_turnel, 1, NULL);

	return 0;
}