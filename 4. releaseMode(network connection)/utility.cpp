#include "utility.h"
#include "handle.h"
#include <Windows.h>
#include <process.h>
#include <ctime>
#include <functional>
#include <random>
using namespace std;

//종료플래그
extern	bool		exitFlag;

void checkexited()
{
	if (exitFlag)
	{
		AllActivateHandles();
		//_endthreadex(0);
		ExitThread(0);
	}
}

void watiAndcheckExited(HANDLE hobject)
{
	WaitForSingleObject(hobject, INFINITE);
	checkexited();
}

//난수함수
unsigned int myrand(int max)
{
	mt19937 engine((unsigned int)time(NULL));                  // MT19937 난수 엔진
	uniform_int_distribution<int> distribution(0, max - 1);       // 생성 범위
	auto generator = bind(distribution, engine);

	return generator();
}