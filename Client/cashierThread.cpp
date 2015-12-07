#include "socket.h"
#include "cashierThread.h"
#include "utility.h"
#include "main.h"
#include <chrono>         // std::chrono::seconds
#include <thread>
#include "enum.h"
#include "carThread.h"

bool topUpReader;

//쓰레드
extern HANDLE		T_cashierThread;
//세마포어
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
		//차량신호올때까지 대기
		watiAndcheckExited(T_hiCashier);

		//DB연결
		SOCKET sock = openDB();

		serchDB(sock, paydata.id);
		int visitresult = recvfromDB(sock);

		char buf[100]; sprintf(buf, "받은값 %d", visitresult);
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
				chagneCloud(3, "20% 할인합니다", true);
			}
			else
				chagneCloud(3, "할인대상이 아닙니다", true);
		}

		UpdateDB(sock, "%d.2.%d.5.%d.6.1",
			paydata.id,
			paydata.cash,
			paydata.movietime);

		if (recvfromDB(sock) == KEY_NOTFOUND)
			exit(-1);

		closeDB(sock);

		chagneCloud(3, "계산이 끝났습니다.", true);
		chagneCloud(3, "안녕히 가세요.", true);

		//계산 다했다고 알려줌
		ReleaseSemaphore(T_waitPay, 1, NULL);
		//차량인식기를 올림
		topUpReader = true;
		Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		chagneCloud(3, "NULL", false);

		//차량인식기를 내리는 신호 대기
		watiAndcheckExited(T_downCashier);
		topUpReader = false;
		Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		

		//다음차 오라고 신호줌
		ReleaseSemaphore(T_waitCashier, 1, NULL);
	}
	return 0;
}

void destorycashierThread()
{
	TerminateThread(T_cashierThread, 0);
	CloseHandle(T_cashierThread);
}