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
	int posX = 0, posY = 670;

	//�� ������ ���� �ʱ�ȭ
	ZeroMemory(arg, sizeof(arg));

	for (int i = 0; i < numOfCar; i++)
		arg[i].posY = posY;

	//�� ������ ����
	for (int i = 0; i < numOfCar; i++)
	{
		arg[i].id = i;
		arg[i].posX = posX;
		arg[i].posY = posY;
		//posX += 100;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);
		
		Sleep(300);
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
	////�����νı⿡ ���Դٰ� �뺸
	//ReleaseSemaphore(Hellow_READER, 1, NULL);

	////�ڿ� ����ϴ� ������ �ͳη� ������� ��ȣ��	
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

	//�ͳ�
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