#ifndef __CARTHREAD_H__
#define __CARTHREAD_H__

#include <Windows.h>

typedef struct _carArg
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
	int movieID;
	int moviePrice;
	int movieTime;
}carArgument;



//�Լ�
unsigned WINAPI createCarThreads(void *arg);
unsigned WINAPI carThread(void *arg);
void destorycarThread();




#endif