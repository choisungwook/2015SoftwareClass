#include <stdio.h>
#include "carThread.h"
#include "Main.h"
#include "Collision.h"

////�浹����
//extern int			collisionBuf[WIDTH][HEIGHT];
//extern HANDLE		collisionMutex;
//
//
//
//extern HBITMAP		hbackbit;
//extern HANDLE		carThread[numOfCar];
//extern carArg		arg[numOfCar];
//
////����ȭ
//extern HANDLE		SEMA_turnel; //�ͳ� ī���� ����ȭ
//extern HANDLE		Hellow_READER;
//extern HANDLE		Leave_READER;

unsigned WINAPI CreateCarThread(void *ARG)
{
	RECT rt;

	int posX = 0, posY = 670;
	int ID = 0;

	//�� ������ ���� �ʱ�ȭ
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
		//�����ϴ� ���� 
		arg[i].rect.left = arg[i].posX;
		arg[i].rect.top = arg[i].posY;
		arg[i].rect.right = arg[i].posX + CAR1WIDTH;
		arg[i].rect.bottom = arg[i].posY + CAR1HEIGHT;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);

		Sleep(500);
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
	//�ͳο� ��
	enterTurnel(Argument);	
	//�ͳε���� ��ȣ��
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