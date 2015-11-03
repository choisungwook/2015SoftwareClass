//#include "carThread.h"
//#include "main.h"
//#include "enum.h"
//#include "utility.h"
//#include <chrono>         // std::chrono::seconds
//#include <thread>
//#include <process.h>
//
//int				speed = 10;
//int				numOfcar;
//int				numOfturnel;
////좌석
//bool			seats[numOfseat];
//
//extern	HWND		hWndMain;
////쓰레드
//extern	HANDLE		T_carThreads[100];
////뮤텍스
//extern	HANDLE		M_accessTree;
//extern	HANDLE		M_accessSeat;
////세마포어 핸들
//extern	HANDLE		T_countingturnel;
//extern	HANDLE		T_waitReader;
//extern	HANDLE		T_hiReader;
//extern	HANDLE		T_selectReader;
//extern	HANDLE		T_byeReader;
//extern	HANDLE		T_downReader;
//
//
//
//int selectseat();
//void releaseseat(int seat);
//int selectmovie();
//
//
//
/////////////////////////////////////////////////
////행동함수
//////////////////////////////////////////////////
////step1
////차량인식기까지 이동
//void movetoreader(int carID, BTreeNode *carNode)
//{
////	int carID = carNode->key;
//
//	while (carNode->carData.posX >= collectionXY::ReadernearX)
//	{
//		//WaitForSingleObject(M_accessTree, INFINITE);
//		//충돌체크
//		if (!collision(avlRoot, carNode, carID, 1))
//		{
//			move(&carNode, -10, 0);
//			setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//				carNode->carData.posX + carhorizon::width, carNode->carData.posY + carhorizon::height);
//			watiAndcheckExited(M_accessTree);
//			Update();
//			ReleaseMutex(M_accessTree);
//			std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
//		}
//		//ReleaseMutex(M_accessTree);		
//	}
//}
//
////step2
////차량인식기에서 작업함
//void talktoReader(BTreeNode *carNode)
//{
//	int carID = carNode->key;
//	int width = carhorizon::width + 5;
//	int height = carhorizon::height;
//	bool flag = false;
//
//	//인식기 자리가 빌때까지 대기한다
//	watiAndcheckExited(T_waitReader);
//	
//	while (carNode->carData.posX >= collectionXY::ReaderfrontX)
//	{	
//		move(&carNode, -1, 0);
//		setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//			carNode->carData.posX + width, carNode->carData.posY + height);
//		watiAndcheckExited(M_accessTree);
//		Update();
//		ReleaseMutex(M_accessTree);
//		std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
//	}
//
//	width = carvertical::width;
//	height = carvertical::height;
//	carNode->carData.direction = 2;
//	
//	setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//		carNode->carData.posX + carvertical::width, carNode->carData.posY + carvertical::height);
//
//	ReleaseSemaphore(T_hiReader, 1, NULL);
//
//	//좌석을 고름
//	carNode->carData.seat = selectseat();
//	//영화를 고름
//	carNode->carData.movieID = selectmovie();
//	//영화시간 영화가격은 영화에 따라 정해짐
//
//	//할일을 마쳤다고 리더기에 알림
//	ReleaseSemaphore(T_selectReader, 1, NULL);
//	//나가도 좋다는 신호를 받음
//	watiAndcheckExited(T_byeReader);
//		
//
//	//코너까지 이동
//	while (carNode->carData.posY >= collectionXY::cornerY)
//	{		
//		if ((flag == false) && (carNode->carData.posY <= 500))
//		{
//			ReleaseSemaphore(T_downReader, 1, NULL);
//			flag = true;
//		}
//
//		if (!collision(avlRoot, carNode, carID, 1))
//		{
//			move(&carNode, 0, -5);
//			setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//				carNode->carData.posX + carvertical::width, carNode->carData.posY + carvertical::height);
//			watiAndcheckExited(M_accessTree);
//			Update();
//			ReleaseMutex(M_accessTree);
//			std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
//		}
//	}
//
//	ReleaseSemaphore(T_countingturnel, 1, NULL);
//}
//
//
////step3
////자기좌석으로 이동
//void goseat(BTreeNode *carNode)
//{
//	int carID = carNode->key;
//	int destinationX = 0;
//
//	switch (carNode->carData.seat)
//	{
//	case 0: destinationX = collectionXY::seat0X; break;
//	case 1: destinationX = collectionXY::seat1X; break;
//	case 2: destinationX = collectionXY::seat2X; break;
//	case 3: destinationX = collectionXY::seat3X; break;
//	case 4: destinationX = collectionXY::seat4X; break;
//	case 5: destinationX = collectionXY::seat5X; break;
//	}
//	
//
//	//회전 후 좌석 x축까지 이동
//	carNode->carData.direction = 1;
//	setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//		carNode->carData.posX + carhorizon::width, carNode->carData.posY + carhorizon::height);
//
//	while (carNode->carData.posX <= destinationX)
//	{
//		//WaitForSingleObject(M_accessTree, INFINITE);
//		//충돌체크
//		if (!collision(avlRoot, carNode, carID, 1))
//		{
//			move(&carNode, 5, 0);
//			setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//				carNode->carData.posX + carhorizon::width, carNode->carData.posY + carhorizon::height);
//			watiAndcheckExited(M_accessTree);
//			Update();
//			ReleaseMutex(M_accessTree);
//			std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
//		}
//		//ReleaseMutex(M_accessTree);		
//	}
//
//	//회전 후 좌석 y축까지 이동
//	carNode->carData.direction = 3;
//	setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//		carNode->carData.posX + carvertical::width, carNode->carData.posY + carvertical::height);
//
//	while (carNode->carData.posY <= collectionXY::seatY)
//	{
//		//WaitForSingleObject(M_accessTree, INFINITE);
//		//충돌체크
//		if (!collision(avlRoot, carNode, carID, 1))
//		{
//			move(&carNode, 0, 5);
//			setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//				carNode->carData.posX + carvertical::width, carNode->carData.posY + carvertical::height);
//			watiAndcheckExited(M_accessTree);
//			Update();
//			ReleaseMutex(M_accessTree);
//			std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
//		}
//		//ReleaseMutex(M_accessTree);		
//	}
//}
//
////step4
////계산대로 이동
//void gotocashier(BTreeNode* carNode)
//{
//	int carID = carNode->key;
//
//	//아래까지 이동
//	//코너까지 이동
//	while (carNode->carData.posY <= collectionXY::seatBottomY)
//	{
//		if (!collision(avlRoot, carNode, carID, 1))
//		{
//			move(&carNode, 0, 5);
//			setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//				carNode->carData.posX + carvertical::width, carNode->carData.posY + carvertical::height);
//			watiAndcheckExited(M_accessTree);
//			Update();
//			ReleaseMutex(M_accessTree);
//			std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
//		}
//	}
//
//	////예상충돌체크
//	////충돌이안된다면 그자리로 이동
//	if (!collision(avlRoot, carNode, carID, 0))
//	{
//		carNode->carData.direction = 1;
//		carNode->carData.posY += 70;
//		/*carNode->carData.rect.top += 70;
//		carNode->carData.rect.bottom += 70;*/
//		setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//			carNode->carData.posX , carNode->carData.posY + 70);
//		watiAndcheckExited(M_accessTree);
//		Update();
//		ReleaseMutex(M_accessTree);
//	}
//
//	//자리해제
//	releaseseat(carNode->carData.seat);
//
//	//X축이동
//	while (carNode->carData.posX <= collectionXY::cornerbottomX)
//	{
//		//WaitForSingleObject(M_accessTree, INFINITE);
//		//충돌체크
//		if (!collision(avlRoot, carNode, carID, 1))
//		{
//			move(&carNode, 10, 0);
//			setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//				carNode->carData.posX + carhorizon::width, carNode->carData.posY + carhorizon::height);
//			watiAndcheckExited(M_accessTree);
//			Update();
//			ReleaseMutex(M_accessTree);
//			std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
//		}
//		//ReleaseMutex(M_accessTree);		
//	}
//
//	//방향전환 및 충돌 범위 변경
//	carNode->carData.direction = 2;
//	setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//		carNode->carData.posX + carvertical::width, carNode->carData.posY + carvertical::height);
//
//
//	//y축이동
//	while (carNode->carData.posY >= collectionXY::seatTop)
//	{
//		//WaitForSingleObject(M_accessTree, INFINITE);
//		//충돌체크
//		if (!collision(avlRoot, carNode, carID, 1))
//		{
//			move(&carNode, 0, -10);
//			setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//				carNode->carData.posX + carvertical::width, carNode->carData.posY + carvertical::height);
//			watiAndcheckExited(M_accessTree);
//			Update();
//			ReleaseMutex(M_accessTree);
//			std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
//		}
//		//ReleaseMutex(M_accessTree);		
//	}
//}
//
//
//
////step5
////계산대와 대화 후 퇴장
////트리에서 삭제
//
//
////쓰레드 생성 컨트롤러
//unsigned WINAPI createCarThreads(void *arg)
//{
//	//avlTremm 초기화
//	BSTMakeAndInit(&avlRoot);
//
//	for (int i = 0; i < numOfseat; i++)
//		seats[i] = false;
//
//	//인자초기화
//	for (int i = 0; i < numOfcar; i++)
//	{
//		carArgument input; 
//		memset(&input, 0, sizeof(input));
//		//ID
//		input.id = i; 
//		//생성되는 위치	
//		input.posX = collectionXY::createdX;
//		input.posY = collectionXY::createdY;
//		//충돌영역
//		input.rect.left = input.posX;
//		input.rect.top = input.posY;
//		input.rect.right = input.posX + carhorizon::width;
//		input.rect.right = input.posY + carhorizon::height;
//				
//		watiAndcheckExited(T_countingturnel);
//		watiAndcheckExited(M_accessTree);
//		//avl 트리 삽입
//		BSTInsert(&avlRoot, i, input);
//		ReleaseMutex(M_accessTree);
//		
//
//		//차쓰레드 생성
//		T_carThreads[i] = (HANDLE)_beginthreadex(NULL, NULL, carThread, (void*)&i, 0, NULL);
//		std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
//	}	
//
//	return 0;
//}
//
//void destorycarThread()
//{
//	for (int i = 0; i < numOfcar; i++)
//	{
//		TerminateThread(T_carThreads[i], 0);
//		CloseHandle(T_carThreads[i]);
//	}
//}
//
//unsigned WINAPI carThread(void *arg)
//{
//	int carID = *((int*)arg);
//
//	watiAndcheckExited(M_accessTree);
//	BTreeNode* carNode = BSTSearch(avlRoot, carID);
//	ReleaseMutex(M_accessTree);
//
//	//movetoreader(carID, carNode);
//	//talktoReader(carNode);
//	//goseat(carNode);
//	//gotocashier(carNode);
//
//	//BSTRemove(&avlRoot, carID);
//	return 0;
//}
//
//void move(BTreeNode **src, int dx, int dy)
//{
//	(*src)->carData.posX += dx;
//	(*src)->carData.posY += dy;
//}
//
////충돌 발생시 true 리턴
//bool collision(BTreeNode *root, BTreeNode *src, int carID, int mode)
//{
//	bool r = false;
//
//	if (root)
//	{
//		if (carID != root->key)
//		{
//			//아래
//			if (mode == 1)
//			{
//				if ((src->carData.rect.left < root->carData.rect.right) &&
//					(src->carData.rect.top < root->carData.rect.bottom) &&
//					(src->carData.rect.right > root->carData.rect.left) &&
//					(src->carData.rect.bottom > root->carData.rect.top))
//				{
//					//포지션으로 체크해도 되지만 간단히 ID값으로 체크
//					if (carID < root->key)
//						return false;
//
//					return true;
//				}
//			}
//			//예상충돌
//			else
//			{
//				int newTop = src->carData.rect.top + 70;
//				int newBottom = src->carData.rect.bottom + 70;
//
//				if ((src->carData.rect.left < root->carData.rect.right) &&
//					(newTop < root->carData.rect.bottom) &&
//					(src->carData.rect.right > root->carData.rect.left) &&
//					(newBottom > root->carData.rect.top))
//				{
//					return true;
//				}
//			}
//		}
//
//		r |= collision(root->left, src, carID, mode);
//		r |= collision(root->right, src, carID, mode);
//	}
//
//	return r;
//}
//
//void setcollsion(BTreeNode *src, int left, int top, int right, int bottom)
//{
//	src->carData.rect.left = left;
//	src->carData.rect.top = top;
//	src->carData.rect.right = right;
//	src->carData.rect.bottom = bottom;
//}
//
//
////비어 있으면 true 리턴
//bool isSeatempty(int seat)
//{
//	if (seats[seat])
//		return false;
//
//	return true;
//}
//
//int selectseat()
//{
//	while (true)
//	{	
//		int seat = myrand(numOfseat);
//
//		watiAndcheckExited(M_accessSeat);
//		if (isSeatempty(seat))
//		{
//			seats[seat] = true;
//			ReleaseMutex(M_accessSeat);
//			return seat;
//		}
//		ReleaseMutex(M_accessSeat);		
//	}
//}
//
//void releaseseat(int seat)
//{
//	watiAndcheckExited(M_accessSeat);
//	seats[seat] = false;
//	ReleaseMutex(M_accessSeat);
//}
//
//int selectmovie()
//{
//	int movie = myrand(numOfseat);
//
//	return movie;
//}

#include "main.h"
#include "carThread.h"
#include "enum.h"
#include "utility.h"
#include <list>
#include <chrono>         // std::chrono::seconds
#include <thread>
#include <process.h>
#include <algorithm>
#define STEP 10
#define speed 30
using namespace std;


bool seats[numOfseat];
int numOfcar;
int numOfturnel;


//쓰레드
extern	list<HANDLE>	T_carThreads;
//뮤택스
extern	HANDLE			M_accessArg;
extern	HANDLE			M_accessSeat;
extern	HANDLE			M_accessCollsion;
//세마포어 핸들
extern	HANDLE			T_countingturnel;
extern	HANDLE			T_waitReader;
extern	HANDLE			T_hiReader;
extern	HANDLE			T_selectReader;
extern	HANDLE			T_byeReader;
extern	HANDLE			T_downReader;

//차인자
list<carArgument>		L_carArg;



list<carArgument>::iterator getArgumentaddress(int carID);
bool detcionMode1(int carID, RECT *src);
void move(list<carArgument>::iterator arg, int mode, int dstPostion, int step);

void movetoReader(list<carArgument>::iterator arg);
void talktoReader(list<carArgument>::iterator arg);
void movetoseat(list<carArgument>::iterator arg);
void watchMovie(list<carArgument>::iterator arg);
void movetocashier(list<carArgument>::iterator arg);
void paytocashier(list<carArgument>::iterator arg);
//좌석 관련된 함수
bool isSeatempty(int seat);
int selectseat();
void releaseseat(int seat);
//영화를 선택함수
int selectmovie();

unsigned WINAPI carThread(void *arg)
{
	int carID = *((int*)arg);

	list<carArgument>::iterator pArg = getArgumentaddress(carID);
	movetoReader(pArg);
	talktoReader(pArg);
	movetoseat(pArg);
	watchMovie(pArg);
	movetocashier(pArg);
	paytocashier(pArg);

	//작업 완료후 리스트 삭제
	watiAndcheckExited(M_accessArg);
	L_carArg.erase(pArg);
	ReleaseMutex(M_accessArg);

	return 0;
}

unsigned WINAPI createCarThreads(void *arg)
{
	for (int i = 0; i < numOfseat; i++)
		seats[i] = false;

	
	//인자초기화
	for (int i = 0; i < numOfcar; i++)
	{
		carArgument input;
		memset(&input, 0, sizeof(input));
		//ID
		input.id = i;
		//생성되는 위치		
		if (i == 0)
			input.posX = collectionXY::createdX - carhorizon::width;
		else
			input.posX = collectionXY::createdX ;
		input.posY = collectionXY::createdY;		
		
		//충돌영역
		input.rect.left = input.posX;
		input.rect.top = input.posY;
		input.rect.right = input.posX + carhorizon::width;
		input.rect.bottom = input.posY + carhorizon::height;

		watiAndcheckExited(T_countingturnel);
		watiAndcheckExited(M_accessArg);
		L_carArg.push_back(input);
		ReleaseMutex(M_accessArg);

		//차쓰레드 생성
		HANDLE Tcar = (HANDLE)_beginthreadex(NULL, NULL, carThread, (void*)&i, 0, NULL);
		T_carThreads.push_back(Tcar);

		std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
	}

	return 0;
}



void destorycarThread()
{
	WaitForSingleObject(M_accessArg, INFINITE);
	L_carArg.clear();
	ReleaseMutex(M_accessArg);

	T_carThreads.clear();
}

list<carArgument>::iterator getArgumentaddress(int carID)
{
	list<carArgument>::iterator r;

	list<carArgument>::iterator end = L_carArg.end();

	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != end; iterPos++)
	{
		if (iterPos->id == carID)
		{
			r = iterPos;
			break;
		}
	}
	ReleaseMutex(M_accessArg);
	return r;
}

//step1
//차량인식기 근처까지이동
void movetoReader(list<carArgument>::iterator arg)
{
	move(arg, 1, collectionXY::ReadernearX, STEP);
}

//step2
//차량인식기앞에서 작업
void talktoReader(list<carArgument>::iterator arg)
{
	//차량인식기 앞자리 비어있을때까지 대기
	watiAndcheckExited(T_waitReader);

	//차량인식기앞까지이동 후 방향전환
	//충돌범위 변경
	move(arg, 1, collectionXY::ReaderfrontX, STEP);
	arg->direction = 2;
	arg->rect.right = arg->rect.left + carvertical::width;
	arg->rect.bottom = arg->rect.top + carvertical::height;

	//차량왔다는 신호를 줌
	ReleaseSemaphore(T_hiReader, 1, NULL);

	//만약 회원등록이 안되어있으면 등록

	//영화를고름
	arg->movieID = selectmovie();
	arg->moviePrice = 100;
	arg->movieTime = 100;
	//좌석을고름
	arg->seat = selectseat();

	//서버에 갱신

	//좌석과 영화를 골랐다는 신호를 줌
	ReleaseSemaphore(T_selectReader, 1, NULL);
	//나가도 좋다는 신호를 받음
	watiAndcheckExited(T_byeReader);
	//차량인식기 밖으로 나감
	move(arg, 2, collectionXY::cornerY + 200, STEP);

	//다음 기다리고 있는 차량 오라고 신호줌
	ReleaseSemaphore(T_countingturnel, 1, NULL);
	//인식기 내려도 된다고 신호를 줌
	ReleaseSemaphore(T_downReader, 1, NULL);
}

//step3
//좌석으로이동
void movetoseat(list<carArgument>::iterator arg)
{
	int x = 0;

	//코너로이동
	move(arg, 2, collectionXY::cornerY, STEP);
	//오른쪽으로 방향전환 및 충돌체 크기 변경
	arg->direction = 1;
	arg->rect.right = arg->rect.left + carhorizon::width;
	arg->rect.bottom = arg->rect.top + carhorizon::height;
	//x좌표 설정
	switch (arg->seat)
	{
	case 0: x = collectionXY::seat0X; break;
	case 1: x = collectionXY::seat1X; break;
	case 2: x = collectionXY::seat2X; break;
	case 3: x = collectionXY::seat3X; break;
	case 4: x = collectionXY::seat4X; break;
	case 5: x = collectionXY::seat5X; break;
	}	
	//좌석 x축까지 이동
	move(arg, 3, x, STEP);

	//아래로 방향전환 및 충돌체 크기 변경
	arg->direction = 3;
	arg->rect.right = arg->rect.left + carvertical::width;
	arg->rect.bottom = arg->rect.top + carvertical::height;
	move(arg, 4, collectionXY::seatY, STEP);
}

//step4
//영화시청함
void watchMovie(list<carArgument>::iterator arg)
{

}

//step5
//계산직원 앞으로 감
void movetocashier(list<carArgument>::iterator arg)
{

}

//step6 계산함
void paytocashier(list<carArgument>::iterator arg)
{

}


//mode 1 : 왼쪽이동
//mode 2 : 위로이동
void move(list<carArgument>::iterator arg, int mode, int dstPostion, int step)
{	
	int *pos = NULL; //모드에 따라 좌표값을 포인터로 지정

	switch (mode){
	case 1: pos = &(arg->posX); step = -step; break; //왼쪽
	case 3: pos = &(arg->posX); break;
	case 2: pos = &(arg->posY); step = -step; break;
	case 4: pos = &(arg->posY); break;
	}

	//왼쪽으로 이동
	if (mode == 1)
	{
		while ((*pos) >= dstPostion)
		{
			if (!detcionMode1(arg->id, &(arg->rect)))
			{
				(*pos) += step;
				arg->rect.left += step;
				arg->rect.right += step;
				Update();
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
			}
		}
	}
	//위로 이동
	else if (mode == 2)
	{
		while ((*pos) >= dstPostion)
		{
			if (!detcionMode1(arg->id, &(arg->rect)))
			{
				(*pos) += step;
				arg->rect.top += step;
				arg->rect.bottom += step;
				Update();
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
			}
		}
	}
	//오른쪽으로 이동
	else if (mode == 3)
	{
		while ((*pos) <= dstPostion)
		{
			if (!detcionMode1(arg->id, &(arg->rect)))
			{
				(*pos) += step;
				arg->rect.left += step;
				arg->rect.right += step;

				Update();
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
			}
		}
	}
	//아래로 이동
	else if (mode == 4)
	{
		while ((*pos) <= dstPostion)
		{
			if (!detcionMode1(arg->id, &(arg->rect)))
			{
				(*pos) += step;
				arg->rect.top += step;
				arg->rect.bottom += step;

				Update();
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
			}
		}
	}
}

//////////////////////////////////////////////////////
//충돌 체크 시작
bool detcionMode1(int carID, RECT *src)
{
	bool r = false;

	watiAndcheckExited(M_accessArg);
	list < carArgument >::iterator End = L_carArg.end();
	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		if (iterPos->id == carID)
			continue;

		if ((src->left < iterPos->rect.right) &&
			(src->top < iterPos->rect.bottom) &&
			(src->right > iterPos->rect.left) &&
			(src->bottom > iterPos->rect.top))
		{
			if (carID < iterPos->id)			
				r = false;
			else
				r = true;
			break;
		}
	}


	ReleaseMutex(M_accessArg);
	return r;
}

//////////////////////////////////////////////////////
//충돌 체크 끝
//////////////////////////////////////////////////////


//////////////////////////////////////////////////////
//좌석 관련된 함수 시작
//////////////////////////////////////////////////////

//비어 있으면 true 리턴
bool isSeatempty(int seat)
{
	if (seats[seat])
		return false;

	return true;
}

int selectseat()
{
	while (true)
	{	
		int seat = myrand(numOfseat);

		watiAndcheckExited(M_accessSeat);
		if (isSeatempty(seat))
		{
			seats[seat] = true;
			ReleaseMutex(M_accessSeat);
			return seat;
		}
		ReleaseMutex(M_accessSeat);		
	}
}

void releaseseat(int seat)
{
	watiAndcheckExited(M_accessSeat);
	seats[seat] = false;
	ReleaseMutex(M_accessSeat);
}


//////////////////////////////////////////////////////
//좌석 관련된 함수 종료
//////////////////////////////////////////////////////

//영화를 고름
int selectmovie()
{
	int movie = myrand(numOfmovie);

	return movie;
}