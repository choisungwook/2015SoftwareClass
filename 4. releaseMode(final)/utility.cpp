#include "utility.h"
#include "handle.h"
#include <Windows.h>
#include <process.h>
#include <ctime>
#include <functional>
#include <random>
using namespace std;

//�����÷���
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

//�����Լ�
unsigned int myrand(int max)
{
	mt19937 engine((unsigned int)time(NULL));                  // MT19937 ���� ����
	uniform_int_distribution<int> distribution(0, max - 1);       // ���� ����
	auto generator = bind(distribution, engine);

	return generator();
}