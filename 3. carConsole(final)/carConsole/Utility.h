#ifndef __UTILITY_H__
#define	__UTILITY_H__

#define MAXOFTURNEL 4 //�ͳ� ī��Ʈ ����
#define numOfmovie	5 //��ȭ ����
#include <iostream>
#include <Windows.h>
#include <string>

//��ȭ
extern std::string movie[numOfmovie];
extern int moviePrice[numOfmovie];
extern int movieTime[numOfmovie];

//��������
extern HANDLE		SEMA_turnel; //�ͳ� ī���� ����ȭ
extern HANDLE		Hellow_READER;
extern HANDLE		Leave_READER;
extern HANDLE		Enter_READER;
extern HANDLE		hDown_READER;
extern HANDLE		hselect_READER;

//���ؽ�
extern HANDLE		collisionMutex; //�浹üũ ���ؽ�
extern HANDLE		screenMutex;
extern HANDLE		seatMutex;


//�����Լ�
unsigned int myrand(int max);

//�ڵ��Լ�
void SemaphoreInit();
void DestorySemaphore();

//��ȭ�Լ�
void movieInit();
void printmovie();

//�浹�簢������ ����
void changeRectPosition(RECT *pRect, int left, int top, int right, int bottom);

//�¼��Լ�
void seatInit();
#endif