#include "readerThread.h"
#include "utility.h"
#include "main.h"

//쓰레드핸들
extern	HANDLE		T_readerThread;
//뮤택스 핸들

//세마포어 핸들
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
		//차량신호기다림
		watiAndcheckExited(T_hiReader);
		//차량이 작업을 마칠때까지 대기
		watiAndcheckExited(T_selectReader);
		//차량에게 나가도 좋다는 신호를 보냄
		ReleaseSemaphore(T_byeReader, 1, NULL);
		//차량인식기 올림
		upReader = true;		
		Update();
		//차량인식기 내려도 좋다는 신호를 받음
		watiAndcheckExited(T_downReader);
		//차량인식기 내림
		upReader = false;
		
		
		//다음차 오라고 신호를 줌
		ReleaseSemaphore(T_waitReader, 1, NULL);
	}

	return 0;
}

void destoryReaderThread()
{
	TerminateThread(T_readerThread, 0);
	CloseHandle(T_readerThread);
}