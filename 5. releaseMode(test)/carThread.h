#ifndef __CARTHREAD_H__
#define __CARTHREAD_H__

#include <vector>
#include <Windows.h>
#include <string>
#include <thread>

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
	std::string movieID;
	int moviePrice;
	int movieTime;
}carArgument;

extern		unsigned int				numOfcar;
extern		unsigned int				numOfturnel;
extern		carArgument					*carArg;
extern		std::thread					*carThreads;

//�Լ�
void carInit();
void CreateCarThread();
void destorycarThread();
void car(int carID);
#endif