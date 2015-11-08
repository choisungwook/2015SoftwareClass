#include "socket.h"
#include "main.h"
#include "carThread.h"
#include "enum.h"
#include "utility.h"
#include <list>
#include <chrono>         // std::chrono::seconds
#include <thread>
#include <process.h>
#include <algorithm>
#include <stdio.h>
#include <vector>

#define STEP 10
#define speed 5
using namespace std;

bool seats[numOfseat];
int numOfcar;
int numOfturnel;

//창 핸들
extern	HWND			hWndMain;
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

//움직임
extern void move(int opcode, int dst, list<carArgument>::iterator car);
extern bool isempty(int index);
extern void initializeCreateBuf();
extern void releaseCreateBuf(int index);
extern int movieNumber;
list<carArgument>::iterator getArgumentaddress(int carID);

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
extern vector<movieTag> Movietag;

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
	int id = pArg->id;
	watiAndcheckExited(M_accessArg);
	L_carArg.erase(pArg);
	ReleaseMutex(M_accessArg);
	deletecarduplicate(id);
	
	if (!L_carArg.size())
		MessageBox(hWndMain, "시뮬레이션 종료", "종료", MB_OK);

	return 0;
}

unsigned WINAPI createCarThreads(void *arg)
{
	for (int i = 0; i < numOfseat; i++)
		seats[i] = false;

	initializecarmap();
	initializeCreateBuf();

	//인자초기화
	for (int i = 0; i < numOfcar; i++)
	{
		carArgument input;
		memset(&input, 0, sizeof(input));
		do
		{
			input.id = myrand(1000);
		} while (checkduplication(input.id));
		//input.id = i;
		//생성되는 위치		
		if (i == 0)
			input.posX = collectionXY::createdX - carhorizon::width;
		else
			input.posX = collectionXY::createdX ;
		input.posY = collectionXY::createdY;		
		
		input.sort = myrand(sortOfcar);
		//충돌영역
		input.rect.left = input.posX;
		input.rect.top = input.posY;
		input.rect.right = input.posX + carhorizon::width;
		input.rect.bottom = input.posY + carhorizon::height;

		while (!isempty(4));
		watiAndcheckExited(M_accessArg);
		L_carArg.push_back(input);
		ReleaseMutex(M_accessArg);
			
		//차쓰레드 생성
		HANDLE Tcar = (HANDLE)_beginthreadex(NULL, NULL, carThread, (void*)&input.id, 0, NULL);
		T_carThreads.push_back(Tcar);

		std::this_thread::sleep_for(std::chrono::milliseconds(300));;
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

////step1
////차량인식기 근처까지이동
void movetoReader(list<carArgument>::iterator arg)
{
	//4번 --> 3번
	while (!isempty(3));	
	move(0, 900, arg);	
	releaseCreateBuf(4);
	//3번 --> 2번
	while (!isempty(2));
	move(0, 780, arg);
	releaseCreateBuf(3);
	//3번 --> 2번
	while (!isempty(1));
	move(0, 650, arg);
	releaseCreateBuf(2);
	////2번 --> 1번
	while (!isempty(0));
	move(0, 530, arg);
	releaseCreateBuf(1);
	
	watiAndcheckExited(T_countingturnel);

	move(0, collectionXY::ReadernearX, arg);
	releaseCreateBuf(0);
	
}

////step2
////차량인식기앞에서 작업
void talktoReader(list<carArgument>::iterator arg)
{
	watiAndcheckExited(T_waitReader);
	move(0, collectionXY::ReaderfrontX, arg);

	arg->direction = 2;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carvertical::width, arg->posY + carvertical::height);
	
	ReleaseSemaphore(T_hiReader, 1, NULL);

	//영화를고름
	arg->movieID = selectmovie();
	arg->moviePrice = Movietag[arg->movieID].price;
	arg->movieTime = Movietag[arg->movieID].time;
	//좌석을고름
	arg->seat = selectseat();

	//DB연결
	//현재시간을 구함
	time_t timer;
	struct tm t;
	timer = time(NULL); // 현재 시각을 초 단위로 얻기
	localtime_s(&t, &timer); // 초 단위의 시간을 분리하여 구조체에 넣기
	
	//서버에 갱신
	//서버연결
	SOCKET s = connect_Server();
	//제일 처음 회원등록이 있는지 확인
	//없으면 등록 요청 
	//있으면 정보 수정
	char recvBuf[1024];
	char sendBuf[1024];
	sprintf(sendBuf, "Connect.%d", arg->id);	
	/*OutputDebugString(sendBuf);*/
	send(s, sendBuf, sizeof(sendBuf), 0);
	recv(s, recvBuf, sizeof(recvBuf), 0);	
	//차넘버, 차종류, 영화가격, 방문시간, 영화이름
	sprintf(sendBuf, "Update.%d.1.%d.2.%d.3.%d-%d-%d %d시 %d분.4.%s",
		arg->id, arg->sort, arg->moviePrice,
		t.tm_year + 1900, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min,
		Movietag[arg->movieID].name);
	send(s, sendBuf, sizeof(sendBuf), 0);
	recv(s, recvBuf, sizeof(recvBuf), 0);
	strcpy(sendBuf, "Disconnect");
	send(s, sendBuf, sizeof(sendBuf), 0);
	closesocket(s);

	// 좌석과 영화를 골랐다는 신호를 줌
	ReleaseSemaphore(T_selectReader, 1, NULL);
	//나가도 좋다는 신호를 받음
	watiAndcheckExited(T_byeReader);
	//차량인식기 밖으로 나감
	move(1, collectionXY::cornerY + 200, arg);	
	
	//다음 기다리고 있는 차량 오라고 신호줌
	ReleaseSemaphore(T_countingturnel, 1, NULL);
	//인식기 내려도 된다고 신호를 줌
	ReleaseSemaphore(T_downReader, 1, NULL);

	move(1, collectionXY::cornerY, arg);
}

//step3
//좌석으로이동
void movetoseat(list<carArgument>::iterator arg)
{
	int x;

	//오른쪽으로 방향전환 및 충돌체 크기 변경
	arg->direction = 1;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carhorizon::width, arg->posY + carhorizon::height);
	
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
	move(2, x, arg);
	
	//아래로 방향전환 및 충돌체 크기 변경
	arg->direction = 3;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carvertical::width, arg->posY + carvertical::height);
	move(3, collectionXY::seatY, arg);
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
	move(3, collectionXY::seatBottomY, arg);

	releaseseat(arg->seat);

	//예상충돌감지
	move(3, collectionXY::seatBottomY + 70, arg);

	arg->direction = 1;
	arg->posY = collectionXY::nextY;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carhorizon::width, carhorizon::height);
	
	move(2, collectionXY::cornerbottomX, arg);
	
	//위로 방향전환
	//충돌범위 변경	
	arg->direction = 2;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carvertical::width, carvertical::height);
	move(1, collectionXY::cashiernearY, arg);
}

//step6 계산함
void paytocashier(list<carArgument>::iterator arg)
{
	watiAndcheckExited(T_waitCashier);
	move(1, collectionXY::cashierY, arg);
	ReleaseSemaphore(T_hiCashier, 1, NULL);
	
	//계산끝날때까지 대기
	watiAndcheckExited(T_waitPay);
	//차량인식기 올려도 된다고 신호보냄
	ReleaseSemaphore(T_downCashier, 1, NULL);
		
	//영화관 빠져나감
	move(1, collectionXY::exitY, arg);
}

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
	int movie = myrand(Movietag.size());

	return movie;
}