#include "handle.h"
#include "carThread.h"
#include <Windows.h>
#include <process.h>

//������ �ڵ�
HANDLE		T_carController;
HANDLE		T_carThreads[100];

//�������� �ڵ�


//���ؽ� �ڵ�
HANDLE		M_accessTree;

void createThreads()
{
	T_carController = (HANDLE)_beginthreadex(NULL, NULL, createCarThreads, NULL, 0, NULL);
}

//������ ���õ� �Լ���

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

//�ı��� ���õ� �Լ���
void destoryThreads()
{
	//�������� �ı�
	//��������Ʈ���ı�
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