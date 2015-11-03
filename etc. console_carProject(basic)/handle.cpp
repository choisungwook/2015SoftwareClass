#include "handle.h"
#include <Windows.h>
#include <process.h>

//������ �ڵ�
HANDLE		T_carController;
HANDLE		T_carThreads[100];
HANDLE		T_readerThread;

//�������� �ڵ�
HANDLE		T_countingturnel;
HANDLE		T_waitReader;
HANDLE		T_hiReader;
HANDLE		T_selectReader;
HANDLE		T_byeReader;
HANDLE		T_downReader;


//���ؽ� �ڵ�
HANDLE		M_accessTree;
HANDLE		M_accessSeat;

//�����÷���
//extern	bool		exitFlag;
//extern	int			numOfturnel;


void createThreads()
{
	//T_carController = (HANDLE)_beginthreadex(NULL, NULL, createCarThreads, NULL, 0, NULL);
	//T_readerThread = (HANDLE)_beginthreadex(NULL, NULL, readerThread, NULL, 0, NULL);
}

//������ ���õ� �Լ���

void initializeHandles()
{
	initializeSemaphores();
	initializeMutexs();
}

void initializeSemaphores()
{
	//T_countingturnel = CreateSemaphore(NULL, numOfturnel, numOfturnel, NULL);
	T_waitReader = CreateSemaphore(NULL, 1, 1, NULL);
	T_hiReader = CreateSemaphore(NULL, 0, 1, NULL);
	T_selectReader = CreateSemaphore(NULL, 0, 1, NULL);
	T_byeReader = CreateSemaphore(NULL, 0, 1, NULL);
	T_downReader = CreateSemaphore(NULL, 0, 1, NULL);
}

void initializeMutexs()
{
	M_accessTree = CreateMutex(NULL, FALSE, NULL);
	M_accessSeat = CreateMutex(NULL, FALSE, NULL);
}

//�ı��� ���õ� �Լ���
void destoryThreads()
{
	//�������� �ı�
	//��������Ʈ���ı�
	if (T_carController)
	{
		//exitFlag = true;
		Sleep(1000); // ��� �����尡 ����ɶ����� ����Ѵ�
		//destorycarThread(); // �������� ����
		//destoryReaderThread(); // ���������� ����
		TerminateThread(T_carController, 0);
		CloseHandle(T_carController);
		//exitFlag = false;
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
}

void destoryMutexs()
{
	CloseHandle(M_accessTree);
	CloseHandle(M_accessSeat);
}

void destoryHandles()
{
	destorySemaphores();
	destoryMutexs();
}

void AllActivateHandles()
{
	//ReleaseSemaphore(T_countingturnel, numOfturnel, NULL);
	ReleaseSemaphore(T_waitReader, 1, NULL);
	ReleaseSemaphore(T_hiReader, 1, NULL);
	ReleaseSemaphore(T_selectReader, 1, NULL);
	ReleaseSemaphore(T_byeReader, 1, NULL);
	ReleaseSemaphore(T_downReader, 1, NULL);
	ReleaseMutex(M_accessTree);
	ReleaseMutex(M_accessSeat);
}