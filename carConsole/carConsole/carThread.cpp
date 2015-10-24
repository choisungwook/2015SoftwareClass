#include <stdio.h>
#include "carThread.h"
#include "Main.h"

//�浹üũ �迭
extern int			collisionBuf[WIDTH][HEIGHT];
//�浹üũ ���ؽ�
extern HANDLE		collisionMutex;

extern HBITMAP		hbackbit;
extern HANDLE		carThread[numOfCar];
extern carArg		arg[numOfCar];
extern HANDLE		SEMA_turnel; //�ͳ� ī���� ����ȭ

unsigned WINAPI CreateCarThread(void *ARG)
{
	int posX = 0, posY = 670;

	//�� ������ ���� �ʱ�ȭ
	ZeroMemory(arg, sizeof(arg));

	//�� ������ ����
	for (int i = 0; i < numOfCar; i++)
	{
		arg[i].id = i;
		arg[i].posX = posX;
		arg[i].posY = posY;
		//posX += 100;
		carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);
		Sleep(1100);
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

	for (int i = 0; i <= 500; i+=10)
	{
		Sleep(100);
		//WaitForSingleObject(collisionMutex, INFINITE);
		Argument->posX += i;
		Update();
	}



	printf("[INFO] %d Thread done\n", Argument->id);

	//�ڿ� ����ϴ� ������ �ͳη� ������� ��ȣ��	
	ReleaseSemaphore(SEMA_turnel, 1, NULL);

	return 0;
}