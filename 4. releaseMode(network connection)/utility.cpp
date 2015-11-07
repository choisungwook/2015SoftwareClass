#include "utility.h"
#include "handle.h"
#include <Windows.h>
#include <process.h>
#include <ctime>
#include <functional>
#include <random>
#include <map>
using namespace std;

//�����÷���
extern	bool		exitFlag;
extern	HANDLE		M_accesscarMap;
//�� ������ ������ �ִ��� üũ
map<int, bool> carmap;


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


void initializecarmap()
{
	carmap.clear();
}

//�� ���̵� �ߺ� �˻� 
//�ߺ��̸� true
bool checkduplication(int carID)
{
	bool r = false;

	watiAndcheckExited(M_accesscarMap);
	map<int, bool>::iterator findIter = carmap.find(carID);

	if (findIter != carmap.end())
		r = true;
	else
		carmap.insert(map<int, bool>::value_type(carID, false));

	ReleaseMutex(M_accesscarMap);
	return r;
}

void deletecarduplicate(int carID)
{
	watiAndcheckExited(M_accesscarMap);
	carmap.erase(carID);
	ReleaseMutex(M_accesscarMap);
}