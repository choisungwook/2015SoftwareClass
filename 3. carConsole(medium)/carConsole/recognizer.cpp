#include "recognizer.h"
#include "carThread.h"
#include "Main.h"
#include <stdio.h>


//차량인식기
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

//차량인식기
//1. 1대의 차만 인식할수 있는 동기화 기능
//2. 인식한 차가 영화를 고를때까지 대기하는 동기화 기능
//3. 영화를 다 고른 차가 나갈때까지 대기하는 동기화 기능
//4. 영화를 다 고른 차가 나가면 다음차에게 오라고 하는 동기화 기능
unsigned WINAPI Entrance_worker(void *arg)
{
	int end = 0;
	while (end++ < numOfCar)
	{
		//차량인식기에 차가 올때까지 대기
		WaitForSingleObject(Hellow_READER, INFINITE);
		printf("============== 차량 들어왔습니다 =============\n");

		
		//영화를 다골랐다는 신호를 받음

		//해당  데이터의 입력이 끝나고 문을 올림
		ReaderDown = false;

		//지나가라는 신호를 줌
		ReleaseSemaphore(Leave_READER, 1, NULL);

		//문을 닫으라는 신호를 받음
		WaitForSingleObject(hDown_READER, INFINITE);
		ReaderDown = true;

		//터널에서 대기중인 차량 오라고 신호를 줌
		ReleaseSemaphore(Enter_READER, 1, NULL);
	}

	
	return 0;
}
