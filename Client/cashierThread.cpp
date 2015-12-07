#include "socket.h"
#include "cashierThread.h"
#include "utility.h"
#include "main.h"
#include <chrono>         // std::chrono::seconds
#include <thread>
#include "enum.h"
#include "carThread.h"

bool topUpReader;

//������
extern HANDLE		T_cashierThread;
//��������
extern HANDLE		T_waitCashier;
extern HANDLE		T_hiCashier;
extern HANDLE		T_waitPay;
extern HANDLE		T_downCashier;

Paydata paydata;
extern Cloud cloud[numOfCloud];


void chagneCloud(int index, char *text, bool status)
{
	if (status == true)
	{
		cloud[index].settext(text);
		cloud[index].setstatus(true);
		Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	else
	{
		cloud[index].setstatus(false);
		Update();
	}
}

unsigned WINAPI cashierThread(void *arg)
{
	topUpReader = false;

	while (true)
	{
		//������ȣ�ö����� ���
		watiAndcheckExited(T_hiCashier);

		//DB����
		SOCKET sock = openDB();

		serchDB(sock, paydata.id);
		int visitresult = recvfromDB(sock);

		char buf[100]; sprintf(buf, "������ %d", visitresult);
		OutputDebugString(buf);

		if (visitresult == KEY_NOTFOUND)
		{
			exit(-1);
		}
		else
		{
			if (visitresult > 3)
			{
				paydata.cash = paydata.cash - paydata.cash / 2;
				chagneCloud(3, "20% �����մϴ�", true);
			}
			else
				chagneCloud(3, "���δ���� �ƴմϴ�", true);
		}

		UpdateDB(sock, "%d.2.%d.5.%d.6.1",
			paydata.id,
			paydata.cash,
			paydata.movietime);

		if (recvfromDB(sock) == KEY_NOTFOUND)
			exit(-1);

		closeDB(sock);

		chagneCloud(3, "����� �������ϴ�.", true);
		chagneCloud(3, "�ȳ��� ������.", true);

		//��� ���ߴٰ� �˷���
		ReleaseSemaphore(T_waitPay, 1, NULL);
		//�����νı⸦ �ø�
		topUpReader = true;
		Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		chagneCloud(3, "NULL", false);

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