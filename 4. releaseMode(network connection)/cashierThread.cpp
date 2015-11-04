#include "cashierThread.h"
#include "utility.h"
#include "main.h"
#include <chrono>         // std::chrono::seconds
#include <thread>
bool topUpReader;

//쓰레드
extern HANDLE		T_cashierThread;
//세마포어
extern HANDLE		T_waitCashier;
extern HANDLE		T_hiCashier;
extern HANDLE		T_waitPay;
extern HANDLE		T_downCashier;

unsigned WINAPI cashierThread(void *arg)
{
	topUpReader = false;

	while (true)
	{
		//차량신호올때까지 대기
		watiAndcheckExited(T_hiCashier);

		//DB연결
		//쿠폰 등록
		//방문횟수 등록

		//계산 다했다고 알려줌
		ReleaseSemaphore(T_waitPay, 1, NULL);
		//차량인식기를 올림
		topUpReader = true;
		Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

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