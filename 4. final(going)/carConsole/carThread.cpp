
#include "carThread.h"
#include "Main.h"
#include "Collision.h"
#include "Screen.h"
#include "Utility.h"

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
	//터널들어가라고 신호줌
	ReleaseSemaphore(SEMA_turnel, 1, NULL);
	goseat(Argument);
	printf("[INFO] %d Thread done\n", Argument->id);
	//자리해제
	ReleaseSeat(Argument->seat);

	return 0;
}

//터널안에 들어가서 하는 작업내용
void enterTurnel(carArg *Arg)
{
	int enterPosX = 60;
	int a = -10, b = 0;

	//차량인식기앞까지 이동
	while (Arg->posX > enterPosX)
	{
		//WaitForSingleObject(collisionMutex, INFINITE);
		if (!testCollision(Arg->id, &Arg->rect))
		{		
			setCollision(Arg, a, b);
			WaitForSingleObject(screenMutex, INFINITE);
			Update();
			ReleaseMutex(screenMutex);
		}
		//ReleaseMutex(collisionMutex);
		Sleep(SPEED);
	}

	
}

//차량인식기 앞에서 하는 작업내용
void TalktoReader(carArg *Arg)
{
	int a = 0, b = -10;
	int turnPosX = 0, turnPosY = 250;
	bool downflag = false;

	//차량인식기에 신호를 줌
	ReleaseSemaphore(Hellow_READER, 1, NULL);

	//영화를 고름
	selectMovie(Arg);

	//좌석을 고름
	//만약 좌석이 꽉찬다면 자리가 빌때까지 대기
	Arg->seat = getSeat();
	

	//영화전부골랐다고 신호를 줌
	ReleaseSemaphore(hselect_READER, 1, NULL);

	//차량인식기에서 지나가라는 신호를 대기
	WaitForSingleObject(Leave_READER, INFINITE);

	//차량방향이 바껴 충돌영역 재설정
	Arg->direction = 2;
	changeRectPosition(&(Arg->rect), Arg->rect.left, Arg->rect.top, Arg->rect.left + CAR02HEIGHT, Arg->rect.top + CAR02WIDTH);
	
	//안으로들어감
	while (Arg->posY > turnPosY)
	{		
		//안으로들어가고 차량인식기에 게이트 내리라고 신호를 줌
		if (downflag == false && Arg->posY < 500)
		{
			downflag = true;
			ReleaseSemaphore(hDown_READER, 1, NULL);
		}

		//WaitForSingleObject(collisionMutex, INFINITE);
		if (!testCollision(Arg->id, &Arg->rect))
		{
			setCollision(Arg, a, b);
			WaitForSingleObject(screenMutex, INFINITE);			
			Update();
			ReleaseMutex(screenMutex);
		}
		//ReleaseMutex(collisionMutex);
		Sleep(SPEED);
	}
}

//좌석으로 간다
void goseat(carArg *Arg)
{
	int posX = 0, posY = 380;

	switch (Arg->seat)
	{
	case 0: posX = 135; break; //ok
	case 1: posX = 270; break; //ok
	case 2: posX = 392; break; //ok
	case 3: posX = 520; break; //ok
	case 4: posX = 640; break;
	case 5: posX = 752; break; //ok
	}

	//오른쪽으로 회전
	//차량방향이 바껴 충돌영역 재설정
	Arg->direction = 1;
	changeRectPosition(&(Arg->rect), Arg->rect.left, Arg->rect.top, Arg->rect.left + CAR00WIDTH, Arg->rect.top + CAR00HEIGHT);
	

	//x좌표까지 이동
	while (Arg->posX < posX)
	{
		if (!testCollision(Arg->id, &Arg->rect))
		{
			setCollision(Arg, 10, 0);
			WaitForSingleObject(screenMutex, INFINITE);
			Update();
			ReleaseMutex(screenMutex);
		}
		
		Sleep(SPEED);
	}

	//아래로 회전
	//차량방향이 바껴 충돌영역 재설정
	Arg->direction = 3;
	changeRectPosition(&(Arg->rect), Arg->rect.left, Arg->rect.top, Arg->rect.left + CAR02HEIGHT, Arg->rect.top + CAR02WIDTH);
	
	while (Arg->posY < posY)
	{
		if (!testCollision(Arg->id, &Arg->rect))
		{
			setCollision(Arg, 0, 10);
			WaitForSingleObject(screenMutex, INFINITE);
			Update();
			ReleaseMutex(screenMutex);
		}

		Sleep(SPEED);
	}
	
	
}

//좌석 고름
unsigned int getSeat()
{
	unsigned int seat = 0;

	
	while (true)
	{
		seat = myrand(numOfSeat);
		
		if (isemptySeat(seat))
		{
			seatArray[seat] = true;
			break;
		}	
	}
	
	return seat;
}

//좌석을 해제한다.
void ReleaseSeat(int seat)
{
	WaitForSingleObject(seatMutex, INFINITE);

	seatArray[seat] = false;

	ReleaseMutex(seatMutex);
}

//좌석이 비어있는지 확인
//비어 있으면 true 리턴
bool isemptySeat(unsigned int seat)
{
	bool r = true;

	WaitForSingleObject(seatMutex, INFINITE);
	if (seatArray[seat])
		r = false;
	else
		r = true;

	ReleaseMutex(seatMutex);

	return r;
}

void selectMovie(carArg *arg)
{
	int select		= myrand(numOfmovie);
	arg->movieID	= movie[select];
	arg->moviePrice = moviePrice[select];
	arg->movieTime	= movieTime[select];	
}

