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
//�¼� ���õ� �Լ�
bool isSeatempty(int seat);
int selectseat();
void releaseseat(int seat);
//��ȭ�� �����Լ�
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

	//�۾� �Ϸ��� ����Ʈ ����
	watiAndcheckExited(M_accessArg);
	L_carArg.erase(pArg);
	ReleaseMutex(M_accessArg);

	return 0;
}

unsigned WINAPI createCarThreads(void *arg)
{
	for (int i = 0; i < numOfseat; i++)
		seats[i] = false;

	
	//�����ʱ�ȭ
	for (int i = 0; i < numOfcar; i++)
	{
		carArgument input;
		memset(&input, 0, sizeof(input));
		//ID
		input.id = i;
		//�����Ǵ� ��ġ		
		if (i == 0)
			input.posX = collectionXY::createdX - carhorizon::width;
		else
			input.posX = collectionXY::createdX ;
		input.posY = collectionXY::createdY;		
		
		//�浹����
		input.rect.left = input.posX;
		input.rect.top = input.posY;
		input.rect.right = input.posX + carhorizon::width;
		input.rect.bottom = input.posY + carhorizon::height;

		watiAndcheckExited(T_countingturnel);
		watiAndcheckExited(M_accessArg);
		L_carArg.push_back(input);
		ReleaseMutex(M_accessArg);

		//�������� ����
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
//�����νı� ��ó�����̵�
void movetoReader(list<carArgument>::iterator arg)
{
	move(arg, 1, collectionXY::ReadernearX, STEP);
}

//step2
//�����νı�տ��� �۾�
void talktoReader(list<carArgument>::iterator arg)
{
	//�����νı� ���ڸ� ������������� ���
	watiAndcheckExited(T_waitReader);

	//�����νı�ձ����̵� �� ������ȯ
	//�浹���� ����
	move(arg, 1, collectionXY::ReaderfrontX, STEP);
	arg->direction = 2;
	arg->rect.right = arg->rect.left + carvertical::width;
	arg->rect.bottom = arg->rect.top + carvertical::height;

	//�����Դٴ� ��ȣ�� ��
	ReleaseSemaphore(T_hiReader, 1, NULL);

	//���� ȸ������� �ȵǾ������� ���

	//��ȭ����
	arg->movieID = selectmovie();
	arg->moviePrice = 100;
	arg->movieTime = 100;
	//�¼�����
	arg->seat = selectseat();

	//������ ����

	//�¼��� ��ȭ�� ����ٴ� ��ȣ�� ��
	ReleaseSemaphore(T_selectReader, 1, NULL);
	//������ ���ٴ� ��ȣ�� ����
	watiAndcheckExited(T_byeReader);
	//�����νı� ������ ����
	move(arg, 2, collectionXY::cornerY + 200, STEP);

	//���� ��ٸ��� �ִ� ���� ����� ��ȣ��
	ReleaseSemaphore(T_countingturnel, 1, NULL);
	//�νı� ������ �ȴٰ� ��ȣ�� ��
	ReleaseSemaphore(T_downReader, 1, NULL);
}

//step3
//�¼������̵�
void movetoseat(list<carArgument>::iterator arg)
{
	int x = 0;

	//�ڳʷ��̵�
	move(arg, 2, collectionXY::cornerY, STEP);
	//���������� ������ȯ �� �浹ü ũ�� ����
	arg->direction = 1;
	arg->rect.right = arg->rect.left + carhorizon::width;
	arg->rect.bottom = arg->rect.top + carhorizon::height;
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
	move(arg, 3, x, STEP);

	//�Ʒ��� ������ȯ �� �浹ü ũ�� ����
	arg->direction = 3;
	arg->rect.right = arg->rect.left + carvertical::width;
	arg->rect.bottom = arg->rect.top + carvertical::height;
	move(arg, 4, collectionXY::seatY, STEP);
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
	move(arg, 4, collectionXY::seatBottomY, STEP);

	//�����浹����
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

	//���� ������ȯ
	//�浹���� ����	
	arg->direction = 2;
	arg->rect.right = arg->rect.left + carvertical::width;
	arg->rect.bottom = arg->rect.top + carvertical::height;
	
	move(arg, 2, collectionXY::cashiernearY, STEP);

	
}

//step6 �����
void paytocashier(list<carArgument>::iterator arg)
{
	
	watiAndcheckExited(T_waitCashier);
	move(arg, 2, collectionXY::cashierY, STEP);

	ReleaseSemaphore(T_hiCashier, 1, NULL);

	//��곡�������� ���
	watiAndcheckExited(T_waitPay);
	//�����νı� �÷��� �ȴٰ� ��ȣ����
	ReleaseSemaphore(T_downCashier, 1, NULL);
	
	//��ȭ�� ��������
	move(arg, 2, collectionXY::exitY, STEP);
}


//mode 1 : �����̵�
//mode 2 : �����̵�
void move(list<carArgument>::iterator arg, int mode, int dstPostion, int step)
{	
	int *pos = NULL; //��忡 ���� ��ǥ���� �����ͷ� ����

	switch (mode){
	case 1: pos = &(arg->posX); step = -step; break; //����
	case 3: pos = &(arg->posX); break;
	case 2: pos = &(arg->posY); step = -step; break;
	case 4: pos = &(arg->posY); break;
	}

	//�������� �̵�
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
	//���� �̵�
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
	//���������� �̵�
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
	//�Ʒ��� �̵�
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
//�浹 üũ ����
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

//�����浹
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
//�浹 üũ ��
//////////////////////////////////////////////////////


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
	int movie = myrand(numOfmovie);

	return movie;
}