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
extern	HANDLE			M_corner1Wait;
extern	HANDLE			M_corner2Wait;
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
//캐릭터인자
list<Person*>			L_personArg;

//움직임
extern void move(int opcode, int dst, list<carArgument>::iterator car);
extern void movecharacter(list<Person*>::iterator arg, int dst, int mode);
extern bool PriorityRightCollision(int carID, RECT *src);
extern bool isempty(int index);
extern void initializeCreateBuf();
extern void releaseCreateBuf(int index);
extern int movieNumber;
extern Cloud cloud[numOfCloud];
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
	L_carArg.clear();
	L_personArg.clear();

	//인자초기화
	for (int i = 0; i < numOfcar; i++)
	{
		carArgument input;
		memset(&input, 0, sizeof(input));
		do
		{
			input.id = myrand(50);
		} while (checkduplication(input.id));
		
		//생성되는 위치		
		if (i == 0)
			input.posX = collectionXY::createdX - carhorizon::width;
		else
			input.posX = collectionXY::createdX ;
		input.posY = collectionXY::createdY;		
		
		//차종류
		input.sort = myrand(sortOfcar);
		
		//상태
		input.status = 0;

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

	//서버접속
	int recv;
	SOCKET sock = openDB();
	//서버에 내 정보가 등록되어 있는지 확인
	checkExisted(sock, arg->id);
	recv = recvfromDB(sock);
	switch (recv)
	{
	case KEY_NOTFOUND://존재하지않음
		{
			char msg[100];
			sprintf(msg, "%d님 처음 온 손님입니다", arg->id);
			cloud[0].setstatus(true);
			cloud[0].x = collectionXYcloud::cloud0X + 5; cloud[0].y = collectionXYcloud::cloud0Y;
			cloud[0].settext(msg);
			Update();
			break;
		}
	default:
		{
			char msg[100];
			sprintf(msg, "%d님 %d번째 방문입니다", arg->id, recv + 1);
			cloud[0].setstatus(true);
			cloud[0].x = collectionXYcloud::cloud0X + 10; cloud[0].y = collectionXYcloud::cloud0Y;
			cloud[0].settext(msg);
			Update();
			break;
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(TEXTSPEED));
	cloud[0].setstatus(false);
	Update();

	//영화를고름
	arg->movieID = selectmovie();
	arg->moviePrice = Movietag[arg->movieID].price;
	arg->movieTime = Movietag[arg->movieID].time;

	char moviemsg[50];
	sprintf(moviemsg, "%s 영화 선택", Movietag[arg->movieID].name);
	cloud[1].x = collectionXYcloud::cloud1X; cloud[1].y = collectionXYcloud::cloud1Y;
	cloud[1].settext(moviemsg);
	cloud[1].setstatus(true);
	Update();

	std::this_thread::sleep_for(std::chrono::milliseconds(TEXTSPEED));
	cloud[1].setstatus(false);
	
	cloud[2].x = collectionXYcloud::cloud0X + 10; cloud[2].y = collectionXYcloud::cloud0Y;
	cloud[2].settext("빈자리 탐색중 .....");
	cloud[2].setstatus(true);
	Update();

	//좌석을고름
	arg->seat = selectseat();
	
	char seatmsg[50];
	sprintf(seatmsg, "%d 번으로 가세요", arg->seat + 1);
	cloud[2].settext(seatmsg);
	Update();
	std::this_thread::sleep_for(std::chrono::milliseconds(TEXTSPEED));
	cloud[2].setstatus(false);
	Update();

	//현재시간을 구함
	time_t timer;
	struct tm t;
	timer = time(NULL); // 현재 시각을 초 단위로 얻기
	localtime_s(&t, &timer); // 초 단위의 시간을 분리하여 구조체에 넣기
		
	//업데이트
	UpdateDB(sock, "%d.1.%d.2.%d.3.%d-%d-%d %d시 %d분.4.%s",
		arg->id, arg->sort, arg->moviePrice,
		t.tm_year + 1900, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min,
		Movietag[arg->movieID].name);
	recvfromDB(sock);
	//서버 접속 끊음
	closeDB(sock);

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
}

//step3
//좌석으로이동
void movetoseat(list<carArgument>::iterator arg)
{
	int x;

	//코너 대기
	move(1, collectionXY::cornerY + 50, arg);	
	watiAndcheckExited(M_corner1Wait);
	move(1, collectionXY::cornerY, arg);

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

	move(2, collectionXY::seat0X, arg);
	ReleaseMutex(M_corner1Wait);
	move(2, x, arg);
	
	//아래로 방향전환 및 충돌체 크기 변경
	arg->direction = 3;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carvertical::width, arg->posY + carvertical::height);
	move(3, collectionXY::seatY, arg);
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


int destination[numOfseat][2] =
{
	{ collectionXYofChar::onlandcreatX0, collectionXYofChar::onbridgeX0 },
	{ collectionXYofChar::onlandcreatX1, collectionXYofChar::onbridgeX1 },
	{ collectionXYofChar::onlandcreatX2, collectionXYofChar::onbridgeX2 },
	{ collectionXYofChar::onlandcreatX3, collectionXYofChar::onbridgeX3 },
	{ collectionXYofChar::onlandcreatX4, collectionXYofChar::onbridgeX4 },
	{ collectionXYofChar::onlandcreatX5, collectionXYofChar::onbridgeX5 },
	//{ collectionXYofChar::onlandcreatX5, 225 },
};

Person* createcharacter(int id, int pos)
{	
	int sortOfchar = myrand(numOfchar);
	Person* newPerson = new Person(id, destination[pos][0], sortOfchar);
	L_personArg.push_back(newPerson);

	return newPerson;
}

void deletecharacter(Person* p)
{
	L_personArg.remove(p);
}

list<Person*>::iterator getaddressPerson(int id)
{
	list<Person*>::iterator r;

	list<Person*>::iterator end = L_personArg.end();

	for (list<Person*>::iterator iterPos = L_personArg.begin(); iterPos != end; iterPos++)
	{
		if ((*iterPos)->id == id)
		{
			r = iterPos;
			break;
		}
	}
	ReleaseMutex(M_accessArg);
	return r;
}

void changeStatus(int *status, int change)
{
	*status = change;
	Update();
}

//영화만 시청한다.
void event1(int time, int id, int pos, int* status)
{
	changeStatus(status, 1);
	//sleep
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

//영화절반 시청하고 매점을 간다
void event2(int time, int id, int pos, int* status)
{
	changeStatus(status, 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(time/2));	
	changeStatus(status, 3);

	Person *p = createcharacter(id, pos);
	list<Person*>::iterator search = getaddressPerson(id);
	
	movecharacter(search, destination[pos][1], 1);	
	movecharacter(search, collectionXYofChar::onshopfrontrY, 2);	
	movecharacter(search, collectionXYofChar::onshopfrontrX, 1);
	movecharacter(search, collectionXYofChar::onshopenterY, 2);

	int backupX = (*search)->posX, backupY = (*search)->posY;
	(*search)->posX = 1200; backupY = (*search)->posY = 1200;
	Update();	
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	(*search)->posX = collectionXYofChar::onshopfrontrX; (*search)->posY = collectionXYofChar::onshopenterY;
	movecharacter(search, collectionXYofChar::onshopfrontrY, 2);
	movecharacter(search, destination[pos][1], 1);
	movecharacter(search, collectionXYofChar::onlandcreatY, 2);
	movecharacter(search, destination[pos][0], 1);
	deletecharacter(p);
	changeStatus(status, 1);
	//delete p;	
	std::this_thread::sleep_for(std::chrono::milliseconds(time/2));
}

//영화 절반 시청하고 화장실을 간다
void event3(int time, int id, int pos, int* status)
{
	changeStatus(status, 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(time / 2));
	changeStatus(status, 2);

	Person *p = createcharacter(id, pos);
	list<Person*>::iterator search = getaddressPerson(id);

	movecharacter(search, destination[pos][1], 1);
	movecharacter(search, collectionXYofChar::ontoiletfrontrY, 2);
	movecharacter(search, collectionXYofChar::ontoiletfrontrX, 1);
	movecharacter(search, collectionXYofChar::ontoiletenterY, 2);

	int backupX = (*search)->posX, backupY = (*search)->posY;	
	(*search)->posX = 1200; backupY = (*search)->posY = 1200;
	Update();	
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	(*search)->posX = collectionXYofChar::ontoiletfrontrX; (*search)->posY = collectionXYofChar::ontoiletenterY;
	movecharacter(search, collectionXYofChar::ontoiletfrontrY, 2);
	movecharacter(search, destination[pos][1], 1);
	movecharacter(search, collectionXYofChar::onlandcreatY, 2);
	movecharacter(search, destination[pos][0], 1);

	deletecharacter(p);
	changeStatus(status, 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(time / 2));
}

void(*events[numOfevents])(int, int, int, int*) =
{
	event1,
	event2,
	event3,
};

//step4
//영화시청함
void watchMovie(list<carArgument>::iterator arg)
{
	//영화이벤트 랜덤발생
	arg->movieEvent = myrand(numOfevents);	
	events[arg->movieEvent](arg->movieTime,arg->id, arg->seat, &arg->status);
	changeStatus(&arg->status, 0);

	if (arg->movieEvent == 2)	
		arg->moviePrice += 1000;	
}

//step5
//계산직원 앞으로 감
void movetocashier(list<carArgument>::iterator arg)
{
	//교차선 앞까지 이동
	move(3, collectionXY::seatBottomY, arg);

	releaseseat(arg->seat);

	//충돌안할때까지 대기
	RECT predict;
	predict.left = arg->rect.left;
	predict.top = arg->rect.top + 60;
	predict.right = predict.left + carhorizon::width + 10;
	predict.bottom = predict.top + carhorizon::height;

	while (PriorityRightCollision(arg->id, &predict));
	//예상충돌감지
	move(3, collectionXY::seatBottomY + 70, arg);

	arg->direction = 1;
	arg->posY = collectionXY::nextY;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carhorizon::width, carhorizon::height);
	
	//코너 대기
	move(2, collectionXY::cornerbottomX - carhorizon::width - 10, arg);
	watiAndcheckExited(M_corner2Wait);
	move(2, collectionXY::cornerbottomX, arg);

	//위로 방향전환
	//충돌범위 변경	
	arg->direction = 2;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carvertical::width, carvertical::height);
	move(1, collectionXY::cashiernearY + 300, arg);
	
	ReleaseMutex(M_corner2Wait);
	move(1, collectionXY::cashiernearY, arg);
}

extern Paydata paydata;

//step6 계산함
void paytocashier(list<carArgument>::iterator arg)
{
	watiAndcheckExited(T_waitCashier);
	move(1, collectionXY::cashierY, arg);

	//계산대기중
	cloud[4].settext("계산대기중");
	cloud[4].setstatus(true);
	Update();

	paydata.id = arg->id;
	paydata.cash = arg->moviePrice;
	paydata.movietime = arg->movieTime;

	ReleaseSemaphore(T_hiCashier, 1, NULL);

	//계산끝날때까지 대기
	watiAndcheckExited(T_waitPay);

	//계산정료
	cloud[4].setstatus(false);
	Update();

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