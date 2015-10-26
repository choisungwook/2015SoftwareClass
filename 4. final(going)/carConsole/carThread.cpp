#include <stdio.h>
#include "carThread.h"
#include "Main.h"
#include "Collision.h"
#include "Screen.h"

unsigned WINAPI CreateCarThread(void *ARG)
{
	int posX = 1000, posY = 670;
	int ID = 0;

	//차 쓰레드 인자 초기화
	ZeroMemory(arg, sizeof(arg));

	
	for (int i = 0; i < numOfCar; i++)
	{
		arg[i].id = i;

		if (i == 0)
			arg[i].posX = posX - CAR00WIDTH;
		else
			arg[i].posX = posX;

		arg[i].posY = posY;
	}

	for (int i = 0; i < numOfCar; i++)
	{
		//차지하는 영역 
		arg[i].rect.left = arg[i].posX;
		arg[i].rect.top = arg[i].posY;
		arg[i].rect.right = arg[i].posX + CAR00WIDTH;
		arg[i].rect.bottom = arg[i].posY + CAR00HEIGHT;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);

		Sleep(100);
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
	//터널에 들어가서 차량인식기 앞까지 이동
	enterTurnel(Argument);	
	WaitForSingleObject(Enter_READER, INFINITE);
	//차량인식기 앞에서 여러가지 작업을 한 후 안으로 들어감
	TalktoReader(Argument);
	

	/*while (true)
	{
		ReaderDown ^= true;
		Update();
		Sleep(300);
	}*/

	//터널들어가라고 신호줌
	ReleaseSemaphore(SEMA_turnel, 1, NULL);
	printf("[INFO] %d Thread done\n", Argument->id);

	return 0;
}

//터널안에 들어가서 하는 작업내용
void enterTurnel(carArg *Arg)
{
	int a = -10, b = 0;

	//차량인식기앞까지 이동
	while (Arg->posX > 40)
	{
		WaitForSingleObject(collisionMutex, INFINITE);
		if (!testCollision(Arg->id, &Arg->rect))
		{		
			setCollision(Arg, a, b);
			WaitForSingleObject(screenMutex, INFINITE);
			Update();
			ReleaseMutex(screenMutex);
		}
		ReleaseMutex(collisionMutex);
		Sleep(SPEED);
	}

	
}

//차량인식기 앞에서 하는 작업내용
void TalktoReader(carArg *Arg)
{
	int a = 0, b = -10;
	bool downflag = false;

	//차량인식기에 신호를 줌
	ReleaseSemaphore(Hellow_READER, 1, NULL);

	//영화를 고름

	//영화전부골랐다고 신호를 줌

	//차량인식기에서 지나가라는 신호를 대기
	//WaitForSingleObject(Leave_READER, INFINITE);

	//차량방향이 바껴 위치 재설정
	Arg->direction = 2;
	Arg->rect.right = Arg->rect.left + CAR02HEIGHT;
	Arg->rect.bottom = Arg->rect.top + CAR02WIDTH;

	
	//안으로들어감
	while (Arg->posY > -100)
	{		
		//안으로들어가고 차량인식기에 게이트 내리라고 신호를 줌
		if (downflag == false && Arg->posY < 600)
		{
			downflag = true;
			ReleaseSemaphore(hDown_READER, 1, NULL);
		}

		WaitForSingleObject(collisionMutex, INFINITE);
		if (!testCollision(Arg->id, &Arg->rect))
		{
			setCollision(Arg, a, b);
			WaitForSingleObject(screenMutex, INFINITE);			
			Update();
			ReleaseMutex(screenMutex);
		}
		ReleaseMutex(collisionMutex);
		Sleep(SPEED);
	}
}