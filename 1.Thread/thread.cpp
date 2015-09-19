#include <Windows.h>
#include <iostream>
#include <string>
#include <process.h>
#include <ctime>
#include <functional>
#include <random>
#include <cstdlib>


#define THREAD_CNT 50
#define MAXOFTURNEL 5
#define SLEEPTIME 1000
#define SEAT_MAX 10
using namespace std;

typedef struct UserData
{
	int car_id; //쓰레드 ID 순차적 증가
	string movie; //영화 번호
	unsigned int time; //영화 시청 시간
	unsigned int price; //영화 시청 시간
	unsigned int seat; //영화 시청 좌석
}UserData;

string movie[] = { "앤트맨","사도","어벤져스","헐크" };
BOOL seatArray[SEAT_MAX];
int movie_price[] = { 4500, 4000, 5000, 4000 };
int movieTime[] = { 3,4,5,6 };
int movieSize = sizeof(movie) / sizeof(string) - 1;


HANDLE SEMA_turnel; //터널 카운터 동기화
HANDLE SEMA_EnterEntrance; //입구들어가는거 동기화
HANDLE SEMA_LeaveEntrance; //입구나가는거 동기화
HANDLE SEMA_Hellow_READER; // 차가 고르는 작업 동기화
HANDLE SEMA_SelectStart; // 차가 고르는 작업 동기화
HANDLE SEMA_SelectFinish; // 차가 고르는 작업 동기화
HANDLE speakmutex; //출력 한번만 하게 하는 동기화
HANDLE SEMA_Hellow_CASHRIER; // 차가 계산하는 작업 동기화
HANDLE SEMA_RECEIPT; // 차가 계산하는 작업 동기화
HANDLE seatmutex;

//Thread
unsigned WINAPI car(void *arg);
unsigned WINAPI Entrance_worker(void *arg);
unsigned WINAPI Exit_worker(void *arg);
//function
void enter_turnel(int carNum);
void enter_entrance(UserData *input);
void recognize_car();
unsigned int myrand(int max);
void pay(UserData *input);
unsigned int getSeat();
void ReleaseSeat(unsigned int n);
void statusSeat();

int main()
{
	HANDLE handleOfcar[THREAD_CNT];
	HANDLE car_reader; //차량인식기
	HANDLE cashier; //계산대
	UserData param[THREAD_CNT];

	//초기화
	ZeroMemory(handleOfcar, sizeof(handleOfcar));
	ZeroMemory(param, sizeof(UserData));
	ZeroMemory(seatArray, sizeof(seatArray));

	//필요한 세마포어 만든다
	SEMA_turnel = CreateSemaphore(NULL, MAXOFTURNEL, MAXOFTURNEL, NULL);
	SEMA_Hellow_READER = CreateSemaphore(NULL, 0, 1, NULL);
	SEMA_SelectStart = CreateSemaphore(NULL, 0, 1, NULL);
	SEMA_SelectFinish = CreateSemaphore(NULL, 0, 1, NULL);
	SEMA_EnterEntrance = CreateSemaphore(NULL, 1, 1, NULL);
	SEMA_LeaveEntrance = CreateSemaphore(NULL, 0, 1, NULL);
	SEMA_Hellow_CASHRIER = CreateSemaphore(NULL, 0, 1, NULL);
	SEMA_RECEIPT = CreateSemaphore(NULL, 0, 1, NULL);
	speakmutex = CreateMutex(NULL, FALSE, NULL);
	seatmutex = CreateMutex(NULL, FALSE, NULL);

	//차량인식기 쓰레드 생성
	car_reader = (HANDLE)_beginthreadex(NULL, NULL, Entrance_worker, NULL, 0, NULL);

	//계산하는 사람 쓰레드 생성
	cashier = (HANDLE)_beginthreadex(NULL, NULL, Exit_worker, NULL, 0, NULL);
	//car쓰레드 생성
	for (int i = 0; i < THREAD_CNT; i++)
	{
		param[i].car_id = i + 1;
		handleOfcar[i] = (HANDLE)_beginthreadex(NULL, NULL, car, (void*)&param[i], 0, NULL);
		Sleep(1);
	}

	//모든 쓰레드 종료할때까지 대기
	WaitForMultipleObjects(THREAD_CNT, handleOfcar, TRUE, INFINITE);
	WaitForSingleObject(car_reader, INFINITE);
	WaitForSingleObject(cashier, INFINITE);

	//핸들 닫기
	CloseHandle(SEMA_turnel);
	CloseHandle(SEMA_SelectStart);
	CloseHandle(SEMA_SelectFinish);
	CloseHandle(SEMA_EnterEntrance);
	CloseHandle(SEMA_LeaveEntrance);
	CloseHandle(SEMA_Hellow_READER);
	CloseHandle(SEMA_Hellow_CASHRIER);
	CloseHandle(SEMA_RECEIPT);
	CloseHandle(speakmutex);
	CloseHandle(seatmutex);

	return 0;
}

//////////////////////////////////////////////////////////////
///////////////////////////// 쓰레드 ////////////////////////
//////////////////////////////////////////////////////////////

//자동차 쓰레드
//1. 터널에 MAXOFTURNEL의 변수 값만틈 터널에 갈수있는 카운터 기능
//2. 차량인식기에서 오라고하는 신호를 대기하는 동기화 기능
//3. 인식기앞에 서면 난수함수를 통해 영화 고르는 기능
//4. 영화를 고르면 차량인식기에 다 골랐다는 신호 동기화 기능
//5. 차량인식기에서 나가는 동기화 기능
//6. 차량인식기에 빠져나가면 터널에 있는 차량 오라고 하는 카운터 기능
unsigned WINAPI car(void *arg)
{
	UserData input = *((UserData*)arg);

	//터널에 5명이상 있으면 내 차례가 될때까지 도로에서 대기
	WaitForSingleObject(SEMA_turnel, INFINITE);
	enter_turnel(input.car_id);

	//차량인식기에 내 차례가 될때까지 터널에서 대기
	WaitForSingleObject(SEMA_EnterEntrance, INFINITE);
	//차량인식기에 앞에 섬
	enter_entrance(&input);
	//챠량인식기에서 나감
	ReleaseSemaphore(SEMA_LeaveEntrance, 1, NULL);

	//뒤에 대기하던 차에게 터널로 들어오라고 신호줌	
	ReleaseSemaphore(SEMA_turnel, 1, NULL);

	//영화를 시청한다
	statusSeat();
	cout << "\t[차가 말한다] : " << input.car_id << "번 차량 " << input.seat << " 자리에서 " << input.time / 1000 << "분 동안 영화 시청중" << endl;
	Sleep(input.time);

	//영화 시청을 마치고 계산대로 간다.
	//계산하는 사람에게 나 계산한다고 신호를 준다.
	ReleaseSemaphore(SEMA_Hellow_CASHRIER, 1, NULL);
	//돈을 지불한다
	pay(&input);

	//퇴장한다
	WaitForSingleObject(speakmutex, INFINITE);
	cout << "\t[차가 말한다] : " << input.car_id << "번 차량 " << " [다시는 안 온다고 다짐하고 퇴장] " << endl;
	ReleaseMutex(speakmutex);
	ReleaseSeat(input.seat);
	return 0;
}


//차량인식기
//1. 1대의 차만 인식할수 있는 동기화 기능
//2. 인식한 차가 영화를 고를때까지 대기하는 동기화 기능
//3. 영화를 다 고른 차가 나갈때까지 대기하는 동기화 기능
//4. 영화를 다 고른 차가 나가면 다음차에게 오라고 하는 동기화 기능
unsigned WINAPI Entrance_worker(void *arg)
{
	int end = 0;
	while (end++ < THREAD_CNT)
	{
		//차량인식기에 차가 올때까지 대기
		WaitForSingleObject(SEMA_Hellow_READER, INFINITE);
		recognize_car();
		//들어온차가 영화 고르라고 신호를 줌		
		ReleaseSemaphore(SEMA_SelectStart, 1, NULL);
		//출력화면 보기위해 딜레이를 줌
		Sleep(SLEEPTIME);
		//들어온차가 영화를 고를때까지 대기
		WaitForSingleObject(SEMA_SelectFinish, INFINITE);
		//들어온차가 차량인식기에서 떠날때까지 대기
		WaitForSingleObject(SEMA_LeaveEntrance, INFINITE);
		WaitForSingleObject(speakmutex, INFINITE);
		cout << "\t\t[차량인식기가 말한다] : " << "차나감" << endl;
		ReleaseMutex(speakmutex);
		//다음차에게 오라고 신호
		ReleaseSemaphore(SEMA_EnterEntrance, 1, NULL);
		//다른차 오라고 신호줌
		WaitForSingleObject(speakmutex, INFINITE);
		cout << "======================== 다른차 들어오세요 =====================" << endl;
		ReleaseMutex(speakmutex);
	}

	cout << "\t\t[차량인식기가 말한다] =========================== 칼퇴근 합니다 =============================" << endl;
	return 0;
}

//계산하는 사람
unsigned WINAPI Exit_worker(void *arg)
{
	int end = 0;

	while (end++ < THREAD_CNT)
	{
		WaitForSingleObject(SEMA_Hellow_CASHRIER, INFINITE);
		//돈을 받는다
		//손님에게 쿠폰과 영수증을 준다
		ReleaseSemaphore(SEMA_RECEIPT, 1, NULL);
	}

	cout << "\t\t\t[계산이 말한다] =========================== 칼퇴근 합니다 =============================" << endl;
	return 0;
}

//////////////////////////////////////////////////////////////
///////////////////////////// 함수 ////////////////////////
//////////////////////////////////////////////////////////////

//차량인식
void recognize_car()
{
	WaitForSingleObject(speakmutex, INFINITE);
	cout << "\t\t[차량인식기가 말한다] : " << "차들어옴" << endl;
	ReleaseMutex(speakmutex);
}

//터널 카운팅 세마포어
void enter_turnel(int carNum)
{
	WaitForSingleObject(speakmutex, INFINITE);
	cout << "[차가 말한다] : " << carNum << "번 차량 터널들어감" << endl;
	ReleaseMutex(speakmutex);
}

//차량인식기에 1명만 들어갈 수 있음
void enter_entrance(UserData *input)
{
	WaitForSingleObject(speakmutex, INFINITE);
	cout << "\t[차가 말한다] : " << input->car_id << "번 차량 차량인식기앞 도착" << endl;
	ReleaseMutex(speakmutex);

	//차량인식기에 나왔다고 통보
	ReleaseSemaphore(SEMA_Hellow_READER, 1, NULL);
	//차량인식기에서 영화고르라는 신호를 대기
	WaitForSingleObject(SEMA_SelectStart, INFINITE);

	//영화고름	
	int select = myrand(movieSize);
	input->movie = movie[select];
	input->price = movie_price[select];
	input->time = movieTime[select] * 1000;
	input->seat = getSeat();
	WaitForSingleObject(speakmutex, INFINITE);
	cout << "\t[차가 말한다] : " << input->car_id << "번 차량 '" << input->movie << "' 영화 고름" << endl;
	ReleaseMutex(speakmutex);

	//차량인식기에 영화전부 골랐다는 신호를 줌
	ReleaseSemaphore(SEMA_SelectFinish, 1, NULL);

}

//차가 계산하는 과정
void pay(UserData *input)
{
	WaitForSingleObject(speakmutex, INFINITE);
	cout << "\t[차가 말한다] : " << input->car_id << "번 차량 " << input->price << " 아까운 돈을 건네준다" << endl;
	ReleaseMutex(speakmutex);
	WaitForSingleObject(SEMA_RECEIPT, INFINITE);
}

//난수함수
unsigned int myrand(int max)
{
	mt19937 engine((unsigned int)time(NULL));                  // MT19937 난수 엔진
	uniform_int_distribution<int> distribution(0, max);       // 생성 범위
	auto generator = bind(distribution, engine);

	return generator();
}

//좌석고름
unsigned int getSeat()
{
	unsigned int seat = 0;
	WaitForSingleObject(seatmutex, INFINITE);
	while (TRUE)
	{
		seat = myrand(SEAT_MAX);

		if (!seatArray[seat])
		{
			seatArray[seat] = TRUE;
			break;
		}

	}
	ReleaseMutex(seatmutex);
	return seat;
}

void ReleaseSeat(unsigned int n)
{
	WaitForSingleObject(seatmutex, INFINITE);
	seatArray[n] = FALSE;
	ReleaseMutex(seatmutex);
}

void statusSeat()
{
	WaitForSingleObject(seatmutex, INFINITE);
	for (int i = 0; i < SEAT_MAX; i++)
		cout << seatArray[i] << ", ";
	cout << endl;
	ReleaseMutex(seatmutex);
}
