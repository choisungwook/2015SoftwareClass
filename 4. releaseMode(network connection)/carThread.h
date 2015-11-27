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

class Cloud
{
private:
	bool status;
	char text[50];
	char cloudpath[50];
public:
	int x, y;
	Cloud() { status = false; };	
	void setstatus(bool r)	{status = r;};
	bool getstatus()	{ return status; };
	void settext(char *msg){ strcpy(text, msg); };
	char* gettext(){ return text; };	
	void setcloud(char *path){ strcpy(cloudpath, path); };
	char* getcloud(){ return cloudpath; };
};


//함수
unsigned WINAPI createCarThreads(void *arg);
unsigned WINAPI carThread(void *arg);
void destorycarThread();




#endif