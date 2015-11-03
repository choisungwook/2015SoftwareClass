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
#define speed 5
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
extern	HANDLE			T_waitCashier;
extern	HANDLE			T_hiCashier;
extern	HANDLE			T_waitPay;
extern	HANDLE			T_downCashier;
//차인자
list<carArgument>		L_carArg;

list<carArgument>::iterator getArgumentaddress(int carID);
bool detcionMode1(int carID, RECT *src);
bool predictdetcionMode(int carID, RECT *src);
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
	//교차선 앞까지 이동
	move(arg, 4, collectionXY::seatBottomY, STEP);

	//예상충돌감지
	//move(arg, 4, collectionXY::seatBottomY + 70, STEP);
	while (predictdetcionMode(arg->id, &(arg->rect)))	
		std::this_thread::sleep_for(std::chrono::milliseconds(speed));
	
	arg->direction = 1;
	arg->posY = collectionXY::nextY;
	arg->rect.top = arg->posY;
	arg->rect.right = arg->rect.left + carhorizon::width;
	arg->rect.bottom = arg->rect.top + carhorizon::height;
	Update();
	releaseseat(arg->seat);
	
	move(arg, 3, collectionXY::cornerbottomX, STEP);

	//위로 방향전환
	//충돌범위 변경	
	arg->direction = 2;
	arg->rect.right = arg->rect.left + carvertical::width;
	arg->rect.bottom = arg->rect.top + carvertical::height;
	
	move(arg, 2, collectionXY::cashiernearY, STEP);

	
}

//step6 계산함
void paytocashier(list<carArgument>::iterator arg)
{
	
	watiAndcheckExited(T_waitCashier);
	move(arg, 2, collectionXY::cashierY, STEP);

	ReleaseSemaphore(T_hiCashier, 1, NULL);

	//계산끝날때까지 대기
	watiAndcheckExited(T_waitPay);
	//차량인식기 올려도 된다고 신호보냄
	ReleaseSemaphore(T_downCashier, 1, NULL);
	
	//영화관 빠져나감
	move(arg, 2, collectionXY::exitY, STEP);
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
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));
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
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));
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
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));
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
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));
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

//예상충돌
bool predictdetcionMode(int carID, RECT *src)
{
	bool r = false;
	RECT next;
	next.left = src->left;
	next.top = collectionXY::nextY;
	next.right = next.left + carhorizon::width;
	next.bottom = next.top + carhorizon::height;
	
	watiAndcheckExited(M_accessArg);
	list < carArgument >::iterator End = L_carArg.end();
	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		if (iterPos->id == carID)
			continue;

		if ((next.left < iterPos->rect.right) &&
			(next.top < iterPos->rect.bottom) &&
			(next.right > iterPos->rect.left) &&
			(next.bottom > iterPos->rect.top))
		{
			if (next.right > iterPos->rect.left)
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
	bool r = true;

	watiAndcheckExited(M_accessSeat);
	if (seats[seat])
		r = false;
	ReleaseMutex(M_accessSeat);
	
	return r;
}

int selectseat()
{
	while (true)
	{	
		int seat = myrand(numOfseat);
		
		if (isSeatempty(seat))
		{
			watiAndcheckExited(M_accessSeat);
			seats[seat] = true;
			ReleaseMutex(M_accessSeat);
			return seat;
		}
		
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