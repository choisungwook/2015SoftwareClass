#include "recognizer.h"
#include "carThread.h"
#include "Main.h"
#include <stdio.h>


//�����νı�
extern HANDLE		carReader;
extern HANDLE		Hellow_READER;
extern HANDLE		Leave_READER;

void createReaderThread()
{
	carReader = (HANDLE)_beginthreadex(NULL, NULL, Entrance_worker, NULL, 0, NULL);
}

void DestoryReaderThread()
{
	CloseHandle(carReader);
}

//�����νı�
//1. 1���� ���� �ν��Ҽ� �ִ� ����ȭ ���
//2. �ν��� ���� ��ȭ�� �������� ����ϴ� ����ȭ ���
//3. ��ȭ�� �� �� ���� ���������� ����ϴ� ����ȭ ���
//4. ��ȭ�� �� �� ���� ������ ���������� ����� �ϴ� ����ȭ ���
unsigned WINAPI Entrance_worker(void *arg)
{
	int end = 0;
	while (end++ < numOfCar)
	{
		//�����νı⿡ ���� �ö����� ���
		WaitForSingleObject(Hellow_READER, INFINITE);
		printf("============== ���� ���Խ��ϴ� =============\n");

		
		//��ȭ�� �ٰ���ٴ� ��ȣ�� ����

		//�ش�  �������� �Է��� ������ ���� �ø�
		ReaderDown = false;

		//��������� ��ȣ�� ��
		ReleaseSemaphore(Leave_READER, 1, NULL);

		//���� ������� ��ȣ�� ����
		WaitForSingleObject(hDown_READER, INFINITE);
		ReaderDown = true;

		//�ͳο��� ������� ���� ����� ��ȣ�� ��
		ReleaseSemaphore(Enter_READER, 1, NULL);
	}

	
	return 0;
}
