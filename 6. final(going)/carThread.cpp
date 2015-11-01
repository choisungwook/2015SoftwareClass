//#include "carThread.h"
//#include "enum.h"
//#include "main.h"
//#include "screen.h"
//
//void carInit()
//{
//	carArg = new carArgument[numOfcar];
//	carThreads = new std::thread[numOfcar];
//}
//
//void CreateCarThread()
//{
//	//int startX = 1000, startY = 670;
//	int startX = 600, startY = 470;
//
//	MessageBox(hWndMain, "A", "AA", MB_OK);
//	carInit();
//	Sleep(10);
//	
//	//초기화
//	for (unsigned int i = 0; i < numOfcar; i++)
//	{	
//		//id 
//		carArg[i].id = i;
//		//시작좌표 
//		if (i == 0)
//			carArg[i].posX = startX - carhorizon::horizionwidth;
//		else 
//			carArg[i].posX = startX;
//		carArg[i].posY = startY;
//		
//		//차량 종류
//		carArg[i].sort = 0;
//		
//		//충돌 영역		
//		carArg[i].rect.left = carArg[i].posX;
//		carArg[i].rect.top = carArg[i].posY;
//		carArg[i].rect.right = carArg[i].posX + carhorizon::horizionwidth;
//		carArg[i].rect.bottom = carArg[i].posX + carhorizon::horizionheight;		
//	}
//
//	//쓰레드 생성
//	for (unsigned int i = 0; i < numOfcar; i++)
//		carThreads[i] = std::thread(&car,i);
//
//	/*for (unsigned int i = 0; i < numOfcar; i++)
//		carThreads[i].join();*/
//	
//	////차 쓰레드 인자 초기화
//	//ZeroMemory(arg, sizeof(arg));
//
//	//Sleep(100);
//
//	//for (int i = 0; i < numOfCar; i++)
//	//{
//	//	arg[i].id = i;
//
//	//	if (i == 0)
//	//		arg[i].posX = posX - CAR00WIDTH;
//	//	else
//	//		arg[i].posX = posX;
//
//	//	arg[i].posY = posY;
//	//}
//
//	//for (int i = 0; i < numOfCar; i++)
//	//{
//	//	//차지하는 영역 
//	//	arg[i].rect.left = arg[i].posX;
//	//	arg[i].rect.top = arg[i].posY;
//	//	arg[i].rect.right = arg[i].posX + CAR00WIDTH;
//	//	arg[i].rect.bottom = arg[i].posY + CAR00HEIGHT;
//	//	carThread[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&arg[i], 0, NULL);
//
//	//	Sleep(100);
//	//}
//
//	////차쓰레드가 모두 종료할때까지 대기한다
//	//WaitForMultipleObjects(numOfCar, carThread, TRUE, INFINITE);
//
//}
//
//void destorycarThread()
//{
//	for (unsigned int i = 0; i < numOfcar; i++)
//	{
//		carThreads[i].detach();
//		if (carThreads[i].joinable())
//			carThreads[i].join();
//	}
//
//	delete[] carArg; carArg = NULL;
//	delete[] carThreads; carThreads = NULL;
//}
//
//void car(int carID)
//{
//	Update();
//}

#include "carThread.h"
#include "main.h"
#include "avltree.h"
#include "enum.h"

#include <chrono>         // std::chrono::seconds
#include <thread>
#include <process.h>

BTreeNode		*avlRoot;
int				numOfcar;
int				numOfturnel;

//
extern			HWND		hWndMain;
//핸들 extern
extern			HANDLE		T_carThreads[100];
//뮤텍스
extern			HANDLE		M_accessTree;


unsigned WINAPI createCarThreads(void *arg)
{
	//avlTremm 초기화
	BSTMakeAndInit(&avlRoot);

	//인자초기화
	for (int i = 0; i < numOfcar; i++)
	{
		carArgument input; 
		memset(&input, 0, sizeof(input));
		//ID
		input.id = i; 
		//생성되는 위치
		/*if (i == 0)
			input.posX = collectionXY::createdX - carhorizon::horizionwidth;
		else*/
			input.posX = collectionXY::createdX;
		input.posY = collectionXY::createdY;
		//충돌영역
		input.rect.left = input.posX;
		input.rect.top = input.posY;
		input.rect.right = input.posX + carhorizon::horizionwidth;
		input.rect.right = input.posY + carhorizon::horizionheight;

		
		WaitForSingleObject(M_accessTree, INFINITE);
		//avl 트리 삽입
		BSTInsert(&avlRoot, i, input);
		ReleaseMutex(M_accessTree);
		

		//차쓰레드 생성
		T_carThreads[i] = (HANDLE)_beginthreadex(NULL, NULL, carThread, (void*)&i, 0, NULL);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));;
	}	

	return 0;
}

unsigned WINAPI carThread(void *arg)
{
	int carID = *((int*)arg);

	movetoreader(carID);

	return 0;
}


void move(BTreeNode **src, int dx, int dy)
{
	(*src)->carData.posX += dx;
	(*src)->carData.posY += dy; 
}

//충돌 발생시 true 리턴
bool collision(BTreeNode *root, BTreeNode *src, int carID, int mode)
{
	bool r = false;

	if (root)
	{
		if (carID != root->key)
		{
			//아래
			if (mode == 1)
			{
				if ((src->carData.rect.left < root->carData.rect.right) &&
					(src->carData.rect.top < root->carData.rect.bottom) &&
					(src->carData.rect.right > root->carData.rect.left) &&
					(src->carData.rect.bottom > root->carData.rect.top))
				{
					//포지션으로 체크해도 되지만 간단히 ID값으로 체크
					if (carID < root->key)
						return false;

					return true;
				}
			}
			//위
			else
			{

			}
		}

		r |= collision(root->left, src, carID, mode);
		r |= collision(root->right, src, carID, mode);
	}

	return r;
}

void setcollsion(BTreeNode *src, int left, int top, int right, int bottom)
{
	src->carData.rect.left = left;
	src->carData.rect.top = top;
	src->carData.rect.right = right;
	src->carData.rect.bottom = bottom;
}


//step1
//차량인식기까지 이동
void movetoreader(int carID)
{
	int width = carhorizon::horizionwidth + 5;
	int height = carhorizon::horizionheight;

	WaitForSingleObject(M_accessTree, INFINITE);
	BTreeNode* carNode = BSTSearch(avlRoot, carID);
	ReleaseMutex(M_accessTree);

	while (carNode->carData.posX >= collectionXY::ReadernearX)
	{
		//WaitForSingleObject(M_accessTree, INFINITE);
		//충돌체크
		if (!collision(avlRoot, carNode, carID, 1))
		{
			move(&carNode, -20, 0);
			setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
				carNode->carData.posX + width, carNode->carData.posY + height);
			WaitForSingleObject(M_accessTree, INFINITE);
			Update();			
			ReleaseMutex(M_accessTree);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));;
		}
		//ReleaseMutex(M_accessTree);
		
	}
	

}

