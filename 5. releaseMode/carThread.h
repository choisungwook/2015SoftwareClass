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
	//좌석
	unsigned int seat;
	//영화
	std::string movieID;
	int moviePrice;
	int movieTime;
}carArgument;

extern		unsigned int				numOfcar;
extern		unsigned int				numOfturnel;
extern		carArgument					*carArg;
extern		std::thread					*carThreads;

//함수
void carInit();
void CreateCarThread();
void destorycarThread();
void car(int carID);
#endif