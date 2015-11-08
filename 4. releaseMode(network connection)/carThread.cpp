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

//â �ڵ�
extern	HWND			hWndMain;
//������
extern	list<HANDLE>	T_carThreads;
//���ý�
extern	HANDLE			M_accessArg;
extern	HANDLE			M_accessSeat;
extern	HANDLE			M_accessCollsion;
//�������� �ڵ�
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
//������
list<carArgument>		L_carArg;

//������
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

//�¼� ���õ� �Լ�
bool isSeatempty(int seat);
int selectseat();
void releaseseat(int seat);

//��ȭ�� �����Լ�
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

	//�۾� �Ϸ��� ����Ʈ ����	
	int id = pArg->id;
	watiAndcheckExited(M_accessArg);
	L_carArg.erase(pArg);
	ReleaseMutex(M_accessArg);
	deletecarduplicate(id);
	
	if (!L_carArg.size())
		MessageBox(hWndMain, "�ùķ��̼� ����", "����", MB_OK);

	return 0;
}

unsigned WINAPI createCarThreads(void *arg)
{
	for (int i = 0; i < numOfseat; i++)
		seats[i] = false;

	initializecarmap();
	initializeCreateBuf();

	//�����ʱ�ȭ
	for (int i = 0; i < numOfcar; i++)
	{
		carArgument input;
		memset(&input, 0, sizeof(input));
		do
		{
			input.id = myrand(1000);
		} while (checkduplication(input.id));
		//input.id = i;
		//�����Ǵ� ��ġ		
		if (i == 0)
			input.posX = collectionXY::createdX - carhorizon::width;
		else
			input.posX = collectionXY::createdX ;
		input.posY = collectionXY::createdY;		
		
		input.sort = myrand(sortOfcar);
		//�浹����
		input.rect.left = input.posX;
		input.rect.top = input.posY;
		input.rect.right = input.posX + carhorizon::width;
		input.rect.bottom = input.posY + carhorizon::height;

		while (!isempty(4));
		watiAndcheckExited(M_accessArg);
		L_carArg.push_back(input);
		ReleaseMutex(M_accessArg);
			
		//�������� ����
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
////�����νı� ��ó�����̵�
void movetoReader(list<carArgument>::iterator arg)
{
	//4�� --> 3��
	while (!isempty(3));	
	move(0, 900, arg);	
	releaseCreateBuf(4);
	//3�� --> 2��
	while (!isempty(2));
	move(0, 780, arg);
	releaseCreateBuf(3);
	//3�� --> 2��
	while (!isempty(1));
	move(0, 650, arg);
	releaseCreateBuf(2);
	////2�� --> 1��
	while (!isempty(0));
	move(0, 530, arg);
	releaseCreateBuf(1);
	
	watiAndcheckExited(T_countingturnel);

	move(0, collectionXY::ReadernearX, arg);
	releaseCreateBuf(0);
	
}

////step2
////�����νı�տ��� �۾�
void talktoReader(list<carArgument>::iterator arg)
{
	watiAndcheckExited(T_waitReader);
	move(0, collectionXY::ReaderfrontX, arg);

	arg->direction = 2;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carvertical::width, arg->posY + carvertical::height);
	
	ReleaseSemaphore(T_hiReader, 1, NULL);

	//��ȭ����
	arg->movieID = selectmovie();
	arg->moviePrice = Movietag[arg->movieID].price;
	arg->movieTime = Movietag[arg->movieID].time;
	//�¼�����
	arg->seat = selectseat();

	//DB����
	//����ð��� ����
	time_t timer;
	struct tm t;
	timer = time(NULL); // ���� �ð��� �� ������ ���
	localtime_s(&t, &timer); // �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
	
	//������ ����
	//��������
	SOCKET s = connect_Server();
	//���� ó�� ȸ������� �ִ��� Ȯ��
	//������ ��� ��û 
	//������ ���� ����
	char recvBuf[1024];
	char sendBuf[1024];
	sprintf(sendBuf, "Connect.%d", arg->id);	
	/*OutputDebugString(sendBuf);*/
	send(s, sendBuf, sizeof(sendBuf), 0);
	recv(s, recvBuf, sizeof(recvBuf), 0);	
	//���ѹ�, ������, ��ȭ����, �湮�ð�, ��ȭ�̸�
	sprintf(sendBuf, "Update.%d.1.%d.2.%d.3.%d-%d-%d %d�� %d��.4.%s",
		arg->id, arg->sort, arg->moviePrice,
		t.tm_year + 1900, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min,
		Movietag[arg->movieID].name);
	send(s, sendBuf, sizeof(sendBuf), 0);
	recv(s, recvBuf, sizeof(recvBuf), 0);
	strcpy(sendBuf, "Disconnect");
	send(s, sendBuf, sizeof(sendBuf), 0);
	closesocket(s);

	// �¼��� ��ȭ�� ����ٴ� ��ȣ�� ��
	ReleaseSemaphore(T_selectReader, 1, NULL);
	//������ ���ٴ� ��ȣ�� ����
	watiAndcheckExited(T_byeReader);
	//�����νı� ������ ����
	move(1, collectionXY::cornerY + 200, arg);	
	
	//���� ��ٸ��� �ִ� ���� ����� ��ȣ��
	ReleaseSemaphore(T_countingturnel, 1, NULL);
	//�νı� ������ �ȴٰ� ��ȣ�� ��
	ReleaseSemaphore(T_downReader, 1, NULL);

	move(1, collectionXY::cornerY, arg);
}

//step3
//�¼������̵�
void movetoseat(list<carArgument>::iterator arg)
{
	int x;

	//���������� ������ȯ �� �浹ü ũ�� ����
	arg->direction = 1;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carhorizon::width, arg->posY + carhorizon::height);
	
	//x��ǥ ����
	switch (arg->seat)
	{
	case 0: x = collectionXY::seat0X; break;
	case 1: x = collectionXY::seat1X; break;
	case 2: x = collectionXY::seat2X; break;
	case 3: x = collectionXY::seat3X; break;
	case 4: x = collectionXY::seat4X; break;
	case 5: x = collectionXY::seat5X; break;
	}	
	//�¼� x����� �̵�
	move(2, x, arg);
	
	//�Ʒ��� ������ȯ �� �浹ü ũ�� ����
	arg->direction = 3;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carvertical::width, arg->posY + carvertical::height);
	move(3, collectionXY::seatY, arg);
}

//step4
//��ȭ��û��
void watchMovie(list<carArgument>::iterator arg)
{

}

//step5
//������� ������ ��
void movetocashier(list<carArgument>::iterator arg)
{
	//������ �ձ��� �̵�
	move(3, collectionXY::seatBottomY, arg);

	releaseseat(arg->seat);

	//�����浹����
	move(3, collectionXY::seatBottomY + 70, arg);

	arg->direction = 1;
	arg->posY = collectionXY::nextY;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carhorizon::width, carhorizon::height);
	
	move(2, collectionXY::cornerbottomX, arg);
	
	//���� ������ȯ
	//�浹���� ����	
	arg->direction = 2;
	SetRect(&arg->rect, arg->posX, arg->posY, arg->posX + carvertical::width, carvertical::height);
	move(1, collectionXY::cashiernearY, arg);
}

//step6 �����
void paytocashier(list<carArgument>::iterator arg)
{
	watiAndcheckExited(T_waitCashier);
	move(1, collectionXY::cashierY, arg);
	ReleaseSemaphore(T_hiCashier, 1, NULL);
	
	//��곡�������� ���
	watiAndcheckExited(T_waitPay);
	//�����νı� �÷��� �ȴٰ� ��ȣ����
	ReleaseSemaphore(T_downCashier, 1, NULL);
		
	//��ȭ�� ��������
	move(1, collectionXY::exitY, arg);
}

//////////////////////////////////////////////////////
//�¼� ���õ� �Լ� ����
//////////////////////////////////////////////////////

//��� ������ true ����
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
//�¼� ���õ� �Լ� ����
//////////////////////////////////////////////////////

//��ȭ�� ��
int selectmovie()
{
	int movie = myrand(Movietag.size());

	return movie;
}