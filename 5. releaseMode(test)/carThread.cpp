#include "carThread.h"
#include "enum.h"
#include "main.h"
#include "screen.h"

void carInit()
{
	carArg = new carArgument[numOfcar];
	carThreads = new std::thread[numOfcar];
}

void CreateCarThread()
{
	//int startX = 1000, startY = 670;
	int startX = 600, startY = 470;

	MessageBox(hWndMain, "A", "AA", MB_OK);
	carInit();
	Sleep(10);
	
	//초기화
	for (unsigned int i = 0; i < numOfcar; i++)
	{	
		//id 
		carArg[i].id = i;
		//시작좌표 
		if (i == 0)
			carArg[i].posX = startX - carhorizon::horizionwidth;
		else 
			carArg[i].posX = startX;
		carArg[i].posY = startY;
		
		//차량 종류
		carArg[i].sort = 0;
		
		//충돌 영역		
		carArg[i].rect.left = carArg[i].posX;
		carArg[i].rect.top = carArg[i].posY;
		carArg[i].rect.right = carArg[i].posX + carhorizon::horizionwidth;
		carArg[i].rect.bottom = carArg[i].posX + carhorizon::horizionheight;		
	}

	//쓰레드 생성
	for (unsigned int i = 0; i < numOfcar; i++)
		carThreads[i] = std::thread(&car,i);

	/*for (unsigned int i = 0; i < numOfcar; i++)
		carThreads[i].join();*/
	
	////차 쓰레드 인자 초기화
	//ZeroMemory(arg, sizeof(arg));

	//Sleep(100);

	//for (int i = 0; i < numOfCar; i++)
	//{
	//	arg[i].id = i;

	//	if (i == 0)
	//		arg[i].posX = posX - CAR00WIDTH;
	//	else
	//		arg[i].posX = posX;

	//	arg[i].posY = posY;
	//}

	//for (int i = 0; i < numOfCar; i++)
	//{
	//	//차지하는 영역 
	//	arg[i].rect.left = arg[i].posX;
	//	arg[i].rect.top = arg[i].posY;
	//	arg[i].rect.right = arg[i].posX + CAR00WIDTH;
	//	arg[i].rect.bottom = arg[i].posY + CAR00HEIGHT;
	//	carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);

	//	Sleep(100);
	//}

	////차쓰레드가 모두 종료할때까지 대기한다
	//WaitForMultipleObjects(numOfCar, carThread, TRUE, INFINITE);

}

void destorycarThread()
{
	for (unsigned int i = 0; i < numOfcar; i++)
	{
		carThreads[i].detach();
		if (carThreads[i].joinable())
			carThreads[i].join();
	}

	delete[] carArg; carArg = NULL;
	delete[] carThreads; carThreads = NULL;
}

void car(int carID)
{
	Update();
}