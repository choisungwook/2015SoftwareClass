
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
	printf("[INFO] %d Thread done\n", Argument->id);
	//�ڸ�����
	ReleaseSeat(Argument->seat);

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

