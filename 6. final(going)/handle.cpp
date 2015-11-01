#include "handle.h"
#include "carThread.h"
#include <Windows.h>
#include <process.h>

//쓰레드 핸들
HANDLE		T_carController;
HANDLE		T_carThreads[100];

//세마포어 핸들


//뮤텍스 핸들
HANDLE		M_accessTree;

void createThreads()
{
	T_carController = (HANDLE)_beginthreadex(NULL, NULL, createCarThreads, NULL, 0, NULL);
}

//생성과 관련된 함수들

void initializeHandles()
{
	initializeSemaphores();
	initializeMutexs();
}

void initializeSemaphores()
{

}

void initializeMutexs()
{
	M_accessTree = CreateMutex(NULL, FALSE, NULL);
}

//파괴와 관련된 함수들
void destoryThreads()
{
	//차쓰레드 파괴
	//쓰레드컨트롤파괴
	if (T_carController)
		TerminateThread(T_carController, 0);
}

void destorySemaphores()
{

}

void destoryMutexs()
{
	CloseHandle(M_accessTree);
}

void destoryHandles()
{	
	destorySemaphores();
	destoryMutexs();
}