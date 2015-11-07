#include "utility.h"
#include "handle.h"
#include <Windows.h>
#include <process.h>
#include <ctime>
#include <functional>
#include <random>
#include <map>
using namespace std;

//종료플래그
extern	bool		exitFlag;
extern	HANDLE		M_accesscarMap;
//차 고유값 가지고 있는지 체크
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

//난수함수
unsigned int myrand(int max)
{
	mt19937 engine((unsigned int)time(NULL));                  // MT19937 난수 엔진
	uniform_int_distribution<int> distribution(0, max - 1);       // 생성 범위
	auto generator = bind(distribution, engine);

	return generator();
}


void initializecarmap()
{
	carmap.clear();
}

//차 아이디 중복 검사 
//중복이면 true
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