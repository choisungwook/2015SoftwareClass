#ifndef __CARTHREAD_H__
#define __CARTHREAD_H__

#define numOfCar	4
#define CAR00HEIGHT	32 //���� 0���� ���� ������ ����
#define CAR00WIDTH	100 //���� 0���� ���� ������ ����
#define CAR02HEIGHT	68 //���� 0���� ���� �� ����
#define CAR02WIDTH	66 //���� 0���� ���� �� ����

#include <process.h>
#include <Windows.h>

typedef struct carArg
{	
	int id;
	int posX;
	int posY;
	RECT rect;
	int sort;
	int direction;
}carArg;

//ġ ������ �Լ�
unsigned WINAPI CreateCarThread(void *ARG);
void destoryCarThread();
unsigned WINAPI car(void *arg);

//�ൿ�Լ�
void enterTurnel(carArg *Arg);
void TalktoReader(carArg *Arg);

#endif