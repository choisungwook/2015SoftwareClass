#include "handle.h"
#include "carThread.h"
#include "readerThread.h"
#include "cashierThread.h"
#include <Windows.h>
#include <process.h>
#include <list>

using namespace std;

//쓰레드 핸들
HANDLE			T_carController;
list<HANDLE>	T_carThreads;
HANDLE			T_readerThread;
HANDLE			T_cashierThread;

//세마포어 핸들
HANDLE		T_countingturnel;
HANDLE		T_waitReader;
HANDLE		T_hiReader;
HANDLE		T_selectReader;
HANDLE		T_byeReader;
HANDLE		T_downReader;
HANDLE		T_waitCashier;
HANDLE		T_hiCashier;
HANDLE		T_waitPay;
HANDLE		T_downCashier;

//뮤텍스 핸들
HANDLE		M_accessArg;
HANDLE		M_accessSeat;
HANDLE		M_accessCollsion;
HANDLE		M_accesscarMap;
HANDLE		M_accessCreate;

//코너 뮤텍스
HANDLE		M_corner1Wait;
HANDLE		M_corner2Wait;

//종료플래그
extern	bool		exitFlag;
extern	int			numOfturnel;

extern unsigned WINAPI cornerworker1(void *arg);

void createThreads()
{
	T_carController = (HANDLE)_beginthreadex(NULL, NULL, createCarThreads, NULL, 0, NULL);
	T_readerThread = (HANDLE)_beginthreadex(NULL, NULL, readerThread, NULL, 0, NULL);
	T_cashierThread = (HANDLE)_beginthreadex(NULL, NULL, cashierThread, NULL, 0, NULL);
}

//생성과 관련된 함수들

void initializeHandles()
{
	initializeSemaphores();
	initializeMutexs();
}

void initializeSemaphores()
{	
	T_countingturnel = CreateSemaphore(NULL, numOfturnel + 1, numOfturnel + 1, NULL);
	T_waitReader = CreateSemaphore(NULL, 1, 1, NULL);
	T_hiReader = CreateSemaphore(NULL, 0, 1, NULL);
	T_selectReader = CreateSemaphore(NULL, 0, 1, NULL);
	T_byeReader = CreateSemaphore(NULL, 0, 1, NULL);
	T_downReader = CreateSemaphore(NULL, 0, 1, NULL);
	T_waitCashier = CreateSemaphore(NULL, 1, 1, NULL);
	T_hiCashier = CreateSemaphore(NULL, 0, 1, NULL);
	T_waitPay = CreateSemaphore(NULL, 0, 1, NULL);
	T_downCashier = CreateSemaphore(NULL, 0, 1, NULL);
}

void initializeMutexs()
{
	M_accessArg = CreateMutex(NULL, FALSE, NULL);
	M_accessSeat = CreateMutex(NULL, FALSE, NULL);
	M_accessCollsion = CreateMutex(NULL, FALSE, NULL);
	M_accesscarMap = CreateMutex(NULL, FALSE, NULL);
	M_accessCreate = CreateMutex(NULL, FALSE, NULL);
	M_corner1Wait = CreateMutex(NULL, FALSE, NULL);
	M_corner2Wait = CreateMutex(NULL, FALSE, NULL);
}

//파괴와 관련된 함수들
void destoryThreads()
{
	//차쓰레드 파괴
	//쓰레드컨트롤파괴
	if (T_carController)
	{
		exitFlag = true;
		Sleep(1000); // 모든 쓰레드가 종료될때까지 대기한다
		destorycarThread(); // 차쓰레드 종료
		destoryReaderThread(); // 리더쓰레드 종료
		destorycashierThread();
		TerminateThread(T_carController, 0);
		CloseHandle(T_carController);
		exitFlag = false;
	}	
}

void destorySemaphores()
{
	CloseHandle(T_countingturnel);
	CloseHandle(T_waitReader);
	CloseHandle(T_hiReader);
	CloseHandle(T_selectReader);
	CloseHandle(T_byeReader);
	CloseHandle(T_downReader);
	CloseHandle(T_waitCashier);
	CloseHandle(T_hiCashier);
	CloseHandle(T_waitPay);
	CloseHandle(T_downCashier);	
}

void destoryMutexs()
{
	CloseHandle(M_accessArg);
	CloseHandle(M_accessSeat);
	CloseHandle(M_accessCollsion);
	CloseHandle(M_accesscarMap);
	CloseHandle(M_accessCreate);
	CloseHandle(M_corner1Wait);
	CloseHandle(M_corner2Wait);
}

void destoryHandles()
{	
	destorySemaphores();
	destoryMutexs();
}

void AllActivateHandles()
{
	ReleaseSemaphore(T_countingturnel, numOfturnel, NULL);
	ReleaseSemaphore(T_waitReader, 1, NULL);
	ReleaseSemaphore(T_hiReader, 1, NULL);
	ReleaseSemaphore(T_selectReader, 1, NULL);
	ReleaseSemaphore(T_byeReader, 1, NULL);
	ReleaseSemaphore(T_downReader, 1, NULL);
	ReleaseSemaphore(T_waitCashier, 1, NULL);
	ReleaseSemaphore(T_hiCashier, 1, NULL);
	ReleaseSemaphore(T_waitPay, 1, NULL);	
	ReleaseSemaphore(T_downCashier, 1, NULL);
	ReleaseMutex(M_accessArg);
	ReleaseMutex(M_accessSeat);
	ReleaseMutex(M_accessCollsion);
	ReleaseMutex(M_accesscarMap);
	ReleaseMutex(M_accessCreate);
	ReleaseMutex(M_corner1Wait);
	ReleaseMutex(M_corner2Wait);

}