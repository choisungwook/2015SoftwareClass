
#include "carThread.h"
#include "Main.h"
#include "Collision.h"
#include "Screen.h"
#include "Utility.h"

unsigned WINAPI CreateCarThread(void *ARG)
{
	int posX = 1000, posY = 670;
	int ID = 0;

	//�� ������ ���� �ʱ�ȭ
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
		//�����ϴ� ���� 
		arg[i].rect.left = arg[i].posX;
		arg[i].rect.top = arg[i].posY;
		arg[i].rect.right = arg[i].posX + CAR00WIDTH;
		arg[i].rect.bottom = arg[i].posY + CAR00HEIGHT;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);

		Sleep(100);
	}	

	//�������尡 ��� �����Ҷ����� ����Ѵ�
	WaitForMultipleObjects(numOfCar, carThread, TRUE, INFINITE);
	
	return 0;
}

void destoryCarThread()
{
	//�������� �ı�
	for (int i = 0; i < numOfCar; i++)
		CloseHandle(carThread[i]);
}

//�� ������
unsigned WINAPI car(void *arg)
{
	carArg *Argument = ((carArg*)arg);

	//�ͳο� n���̻� ������ �� ���ʰ� �ɶ����� ���ο��� ���
	WaitForSingleObject(SEMA_turnel, INFINITE);
	//�ͳο� ���� �����νı� �ձ��� �̵�
	enterTurnel(Argument);	
	WaitForSingleObject(Enter_READER, INFINITE);
	//�����νı� �տ��� �������� �۾��� �� �� ������ ��
	TalktoReader(Argument);	
	//�ͳε���� ��ȣ��
	ReleaseSemaphore(SEMA_turnel, 1, NULL);
	goseat(Argument);
	watchMovieAndEvent(Argument);
	goCashier(Argument);
	
	printf("[INFO] %d Thread done\n", Argument->id);

	return 0;
}

//�ͳξȿ� ���� �ϴ� �۾�����
void enterTurnel(carArg *Arg)
{
	int enterPosX = 60;
	int a = -10, b = 0;

	//�����νı�ձ��� �̵�
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

//�����νı� �տ��� �ϴ� �۾�����
void TalktoReader(carArg *Arg)
{
	int a = 0, b = -10;
	int turnPosX = 0, turnPosY = 250;
	bool downflag = false;

	//�����νı⿡ ��ȣ�� ��
	ReleaseSemaphore(Hellow_READER, 1, NULL);

	//��ȭ�� ��
	selectMovie(Arg);

	//�¼��� ��
	//���� �¼��� �����ٸ� �ڸ��� �������� ���
	Arg->seat = getSeat();
	
	//��ȭ���ΰ���ٰ� ��ȣ�� ��
	ReleaseSemaphore(hselect_READER, 1, NULL);

	//�����νı⿡�� ��������� ��ȣ�� ���
	WaitForSingleObject(Leave_READER, INFINITE);

	//���������� �ٲ� �浹���� �缳��
	Arg->direction = 2;
	changeRectPosition(&(Arg->rect), Arg->rect.left, Arg->rect.top, Arg->rect.left + CAR02HEIGHT, Arg->rect.top + CAR02WIDTH);
	
	//�����ε�
	while (Arg->posY > turnPosY)
	{		
		//�����ε��� �����νı⿡ ����Ʈ ������� ��ȣ�� ��
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

//�¼����� ����
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

	//���������� ȸ��
	//���������� �ٲ� �浹���� �缳��
	Arg->direction = 1;
	changeRectPosition(&(Arg->rect), Arg->rect.left, Arg->rect.top, Arg->rect.left + CAR00WIDTH, Arg->rect.top + CAR00HEIGHT);
	
	//x��ǥ���� �̵�
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

	//�Ʒ��� ȸ��
	//���������� �ٲ� �浹���� �缳��
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

//�¼� ��
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

//�¼��� �����Ѵ�.
void ReleaseSeat(int seat)
{
	WaitForSingleObject(seatMutex, INFINITE);

	seatArray[seat] = false;

	ReleaseMutex(seatMutex);
}

//�¼��� ����ִ��� Ȯ��
//��� ������ true ����
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

void(*eventFunc[4])(int) = 
{
	event0,
	event1,
};

void watchMovieAndEvent(carArg *arg)
{
	int _event = myrand(numOfevent);

	eventFunc[_event](arg->movieTime);
}

void goCashier(carArg *arg)
{
	int posX = 940, posY = 500;
	int cashierposY = 100;

	while (arg->posY < posY)
	{
		if (!testCollision(arg->id, &arg->rect))
		{
			setCollision(arg, 0, 10);
			//WaitForSingleObject(screenMutex, INFINITE);
			Update();
			//ReleaseMutex(screenMutex);
		}
		Sleep(SPEED);
	}

	Sleep(1000);	
	//������ȯ�� �浹üũ
	RECT nextPosition;
	nextPosition.left = arg->rect.left ;
	nextPosition.top = arg->rect.top + CAR02HEIGHT;
	nextPosition.right = nextPosition.left + CAR00WIDTH;
	nextPosition.bottom = nextPosition.top + CAR02HEIGHT;

	//printf("%d %d %d %d\n", arg->rect.left, arg->rect.top, arg->rect.right, arg->rect.bottom);
	//printf("%d %d %d %d\n", nextPosition.left, nextPosition.top, nextPosition.right, nextPosition.bottom);

	while (testBottomCollision(arg->id, &nextPosition))
	{
		Sleep(1000);
	}
		//������ȯ
		arg->direction = 1;
		changeRectPosition(&(arg->rect), nextPosition.left, nextPosition.top, nextPosition.right, nextPosition.bottom);
		arg->posX = arg->rect.left;
		arg->posY = arg->rect.top;
		//printf("posX : %d posY : %d\n", arg->posX, arg->posY);
		//WaitForSingleObject(screenMutex, INFINITE);
		Update();
		
		//ReleaseMutex(screenMutex);
	


	//x��ǥ ������ �̵�
	while (arg->posX < posX)
	{
		if (!testCollision(arg->id, &arg->rect))
		{
			setCollision(arg, 10, 0);
			//WaitForSingleObject(screenMutex, INFINITE);
			Update();
			//ReleaseMutex(screenMutex);
		}

		Sleep(SPEED);
	}

	//�ڸ�����
	ReleaseSeat(arg->seat);

	//���������� �ٲ� �浹���� �缳��
	arg->direction = 2;
	changeRectPosition(&(arg->rect), arg->rect.left, arg->rect.top, arg->rect.left + CAR02HEIGHT, arg->rect.top + CAR02WIDTH);

	//������� �̵�	
	printf("y��ǥ %d\n", arg->posY);
	while (arg->posY > cashierposY)
	{
		if (!crossCollision(arg->id, &arg->rect))
		{
			setCollision(arg, 0, -10);
			//WaitForSingleObject(screenMutex, INFINITE);
			Update();
			//ReleaseMutex(screenMutex);
		}

		Sleep(SPEED);
	}

}
///////////////////////////////////////////////
//////////// �̺�Ʈ ///////////////////////////

//������ �ִ´�
void event0(int time)
{
	printf("event0 called\n");
	Sleep(time);
}

void event1(int time)
{
	printf("event1 called\n");
}