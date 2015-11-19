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
	int movieEvent;
	//좌석
	unsigned int seat;
	//영화
	int movieID;
	int moviePrice;
	int movieTime;
}carArgument;

typedef struct _movie
{
	char name[20];
	int price;
	int time;
}movieTag;

class Person
{
public:
	int id;
	int character;
	int direction;
	int posX;
	int posY;

	Person(int id,  int posX, int character)
	{
		this->id = id;
		this->posX = posX;
		this->character = character;
		direction = 2;
		posY = 380;
	};
};

//typedef struct _cloud
//{
//	int id;
//	int posx;
//	int posY;
//}Cloude;
//
//typedef struct _Texture
//{
//	int id;
//	char txt[100];
//}Texture;


//함수
unsigned WINAPI createCarThreads(void *arg);
unsigned WINAPI carThread(void *arg);
void destorycarThread();




#endif