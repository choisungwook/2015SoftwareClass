#include "readerThread.h"
#include "utility.h"
#include "main.h"

//�������ڵ�
extern	HANDLE		T_readerThread;
//���ý� �ڵ�

//�������� �ڵ�
extern	HANDLE		T_waitReader;
extern	HANDLE		T_hiReader;
extern	HANDLE		T_selectReader;
extern	HANDLE		T_byeReader;
extern	HANDLE		T_downReader;

bool upReader;

unsigned WINAPI readerThread(void *arg)
{
	upReader = false;

	while (true)
	{	
		//������ȣ��ٸ�
		watiAndcheckExited(T_hiReader);
		//������ �۾��� ��ĥ������ ���
		watiAndcheckExited(T_selectReader);
		//�������� ������ ���ٴ� ��ȣ�� ����
		ReleaseSemaphore(T_byeReader, 1, NULL);
		//�����νı� �ø�
		upReader = true;		
		Update();
		//�����νı� ������ ���ٴ� ��ȣ�� ����
		watiAndcheckExited(T_downReader);
		//�����νı� ����
		upReader = false;
		
		
		//������ ����� ��ȣ�� ��
		ReleaseSemaphore(T_waitReader, 1, NULL);
	}

	return 0;
}

void destoryReaderThread()
{
	TerminateThread(T_readerThread, 0);
	CloseHandle(T_readerThread);
}