#include "Utility.h"
#include <ctime>
#include <functional>
#include <random>
#include "carThread.h"
using namespace std;

//난수함수
unsigned int myrand(int max)
{
	mt19937 engine((unsigned int)time(NULL));                  // MT19937 난수 엔진
	uniform_int_distribution<int> distribution(0, max - 1);       // 생성 범위
	auto generator = bind(distribution, engine);

	return generator();
}

void DestorySemaphore()
{
	//mutex
	CloseHandle(collisionMutex);
	CloseHandle(screenMutex);
	CloseHandle(seatMutex);

	//semaphore
	CloseHandle(SEMA_turnel);
	CloseHandle(Hellow_READER);
	CloseHandle(Leave_READER);
	CloseHandle(Enter_READER);
	CloseHandle(hDown_READER);
	CloseHandle(hselect_READER);
}

void SemaphoreInit()
{
	//mutex
	collisionMutex = CreateMutex(NULL, FALSE, NULL);
	screenMutex = CreateMutex(NULL, FALSE, NULL);
	seatMutex = CreateMutex(NULL, FALSE, NULL);

	//semaphore
	SEMA_turnel = CreateSemaphore(NULL, MAXOFTURNEL, MAXOFTURNEL, NULL);
	Hellow_READER = CreateSemaphore(NULL, 0, 1, NULL);
	Leave_READER = CreateSemaphore(NULL, 0, 1, NULL);
	Enter_READER = CreateSemaphore(NULL, 1, 1, NULL);
	hDown_READER = CreateSemaphore(NULL, 0, 1, NULL);
	hselect_READER = CreateSemaphore(NULL, 0, 1, NULL);
}

void movieInit()
{
	for (int i = 0; i < numOfmovie; i++)
	{
		movie[i] = i + 'A';
		moviePrice[i] = i + 1000;
		movieTime[i] = i + 2;
	}
}

void seatInit()
{
	for (int i = 0; i < numOfSeat; i++)
		seatArray[i] = 0;
}

void printmovie()
{
	for (int i = 0; i < numOfmovie; i++)	
		cout << movie[i] << " " << moviePrice[i] << " " << movieTime[i] << endl;	
}

void changeRectPosition(RECT *pRect,int left, int top, int right, int bottom)
{
	pRect->left = left;
	pRect->top = top;
	pRect->right = right;
	pRect->bottom = bottom;
}
