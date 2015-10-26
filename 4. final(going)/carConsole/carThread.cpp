#include <stdio.h>
#include "carThread.h"
#include "Main.h"
#include "Collision.h"
#include "Screen.h"

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
	

	/*while (true)
	{
		ReaderDown ^= true;
		Update();
		Sleep(300);
	}*/

	//�ͳε���� ��ȣ��
	ReleaseSemaphore(SEMA_turnel, 1, NULL);
	printf("[INFO] %d Thread done\n", Argument->id);

	return 0;
}

//�ͳξȿ� ���� �ϴ� �۾�����
void enterTurnel(carArg *Arg)
{
	int a = -10, b = 0;

	//�����νı�ձ��� �̵�
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

//�����νı� �տ��� �ϴ� �۾�����
void TalktoReader(carArg *Arg)
{
	int a = 0, b = -10;
	bool downflag = false;

	//�����νı⿡ ��ȣ�� ��
	ReleaseSemaphore(Hellow_READER, 1, NULL);

	//��ȭ�� ��

	//��ȭ���ΰ���ٰ� ��ȣ�� ��

	//�����νı⿡�� ��������� ��ȣ�� ���
	//WaitForSingleObject(Leave_READER, INFINITE);

	//���������� �ٲ� ��ġ �缳��
	Arg->direction = 2;
	Arg->rect.right = Arg->rect.left + CAR02HEIGHT;
	Arg->rect.bottom = Arg->rect.top + CAR02WIDTH;

	
	//�����ε�
	while (Arg->posY > -100)
	{		
		//�����ε��� �����νı⿡ ����Ʈ ������� ��ȣ�� ��
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