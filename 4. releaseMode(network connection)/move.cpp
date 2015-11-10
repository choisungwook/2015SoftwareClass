#include "carThread.h"
#include "main.h"
#include "enum.h"
#include "utility.h"
#include <list>
#include <chrono>         // std::chrono::seconds
#include <thread>
#define MOVESTEP 10
using namespace std;

bool createbuf[5];
extern HANDLE	M_accessCreate;

void initializeCreateBuf();
void move(int opcode, int dst, list<carArgument>::iterator car);

//충돌체크 include
extern bool PriorityLeftCollision(int carID, RECT *src);
extern bool PriorityTopCollision(int carID, RECT *src);
extern bool PriorityRightCollision(int carID, RECT *src);
extern bool PriorityDownCollision(int carID, RECT *src);

void moveLeft(int dst, list<carArgument>::iterator car)
{
	while (car->posX >= dst)
	{
		if (!PriorityLeftCollision(car->id, &car->rect))
		{			
			car->posX -= MOVESTEP;
			SetRect(&car->rect, car->posX, car->posY, car->posX + carhorizon::width, car->posY + carhorizon::height);
			Update();
		}		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}	
}

void moveUp(int dst, list<carArgument>::iterator car)
{
	while (car->posY >= dst)
	{
		if (!PriorityTopCollision(car->id, &car->rect))
		{
			car->posY -= MOVESTEP;
			SetRect(&car->rect, car->posX, car->posY, car->posX + carvertical::width, car->posY + carvertical::height);
			Update();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void moveRight(int dst, list<carArgument>::iterator car)
{
	while (car->posX <= dst)
	{
		if (!PriorityRightCollision(car->id, &car->rect))
		{
			car->posX += MOVESTEP;
			SetRect(&car->rect, car->posX, car->posY, car->posX + carhorizon::width, car->posY + carhorizon::height);
			Update();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void moveDown(int dst, list<carArgument>::iterator car)
{
	while (car->posY <= dst)
	{
		if (!PriorityDownCollision(car->id, &car->rect))
		{
			car->posY += MOVESTEP;
			SetRect(&car->rect, car->posX, car->posY, car->posX + carvertical::width, car->posY + carvertical::height);
			Update();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void(*moveFunc[4])(int, list<carArgument>::iterator) =
{
	moveLeft,
	moveUp,
	moveRight,
	moveDown
};


void move(int opcode, int dst, list<carArgument>::iterator car)
{
	moveFunc[opcode](dst, car);	
}

//생성되는 위치 배열
void initializeCreateBuf()
{
	for (int i = 0; i < 5; i++)
		createbuf[i] = false;
}

//비어있으면 true
bool isempty(int index)
{
	bool r = false;

	watiAndcheckExited(M_accessCreate);
	if (createbuf[index] == false)
	{
		r = true;
		createbuf[index] = true;
	}

	ReleaseMutex(M_accessCreate);

	return r;
}

void releaseCreateBuf(int index)
{
	watiAndcheckExited(M_accessCreate);
	createbuf[index] = false;
	ReleaseMutex(M_accessCreate);
}