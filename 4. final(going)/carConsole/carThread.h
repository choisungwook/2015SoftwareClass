#ifndef __CARTHREAD_H__
#define __CARTHREAD_H__

#define numOfCar	12
#define numOfSeat	6
#define CAR00HEIGHT	32 //���� 0���� ���� ������ ����
#define CAR00WIDTH	100 //���� 0���� ���� ������ ����
#define CAR02HEIGHT	68 //���� 0���� ���� �� ����
#define CAR02WIDTH	66 //���� 0���� ���� �� ����

#include <process.h>
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;

typedef struct carArg
{	
	int id;
	int posX;
	int posY;
	RECT rect;
	int sort;
	int direction;
	//�¼�
	unsigned int seat;
	//��ȭ
	string movieID;
	int moviePrice;
	int movieTime;
}carArg;


//��������
extern HANDLE		carThread[numOfCar];
//�������� ����
extern carArg		arg[numOfCar];
//�¼�
extern BOOL seatArray[numOfCar];


//�� ������ �Լ�
unsigned WINAPI CreateCarThread(void *ARG);
void destoryCarThread();
unsigned WINAPI car(void *arg);

//�ൿ�Լ�
void enterTurnel(carArg *Arg);
void TalktoReader(carArg *Arg);
void selectMovie(carArg *arg); //��ȭ�� ����
void goseat(carArg *Arg);
unsigned int getSeat(); //�¼��� ����
bool isemptySeat(unsigned int seat);
void ReleaseSeat(int seat);



#endif