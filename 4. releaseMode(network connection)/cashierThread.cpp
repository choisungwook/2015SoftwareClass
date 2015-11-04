#include "cashierThread.h"
#include "utility.h"
#include "main.h"
#include <chrono>         // std::chrono::seconds
#include <thread>
bool topUpReader;

//������
extern HANDLE		T_cashierThread;
//��������
extern HANDLE		T_waitCashier;
extern HANDLE		T_hiCashier;
extern HANDLE		T_waitPay;
extern HANDLE		T_downCashier;

unsigned WINAPI cashierThread(void *arg)
{
	topUpReader = false;

	while (true)
	{
		//������ȣ�ö����� ���
		watiAndcheckExited(T_hiCashier);

		//DB����
		//���� ���
		//�湮Ƚ�� ���

		//��� ���ߴٰ� �˷���
		ReleaseSemaphore(T_waitPay, 1, NULL);
		//�����νı⸦ �ø�
		topUpReader = true;
		Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		//�����νı⸦ ������ ��ȣ ���
		watiAndcheckExited(T_downCashier);
		topUpReader = false;
		Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		//������ ����� ��ȣ��
		ReleaseSemaphore(T_waitCashier, 1, NULL);
	}
	return 0;
}

void destorycashierThread()
{
	TerminateThread(T_cashierThread, 0);
	CloseHandle(T_cashierThread);
}