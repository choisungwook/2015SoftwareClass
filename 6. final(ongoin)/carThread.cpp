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
////�¼�
//bool			seats[numOfseat];
//
//extern	HWND		hWndMain;
////������
//extern	HANDLE		T_carThreads[100];
////���ؽ�
//extern	HANDLE		M_accessTree;
//extern	HANDLE		M_accessSeat;
////�������� �ڵ�
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
////�ൿ�Լ�
//////////////////////////////////////////////////
////step1
////�����νı���� �̵�
//void movetoreader(int carID, BTreeNode *carNode)
//{
////	int carID = carNode->key;
//
//	while (carNode->carData.posX >= collectionXY::ReadernearX)
//	{
//		//WaitForSingleObject(M_accessTree, INFINITE);
//		//�浹üũ
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
////�����νı⿡�� �۾���
//void talktoReader(BTreeNode *carNode)
//{
//	int carID = carNode->key;
//	int width = carhorizon::width + 5;
//	int height = carhorizon::height;
//	bool flag = false;
//
//	//�νı� �ڸ��� �������� ����Ѵ�
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
//	//�¼��� ��
//	carNode->carData.seat = selectseat();
//	//��ȭ�� ��
//	carNode->carData.movieID = selectmovie();
//	//��ȭ�ð� ��ȭ������ ��ȭ�� ���� ������
//
//	//������ ���ƴٰ� �����⿡ �˸�
//	ReleaseSemaphore(T_selectReader, 1, NULL);
//	//������ ���ٴ� ��ȣ�� ����
//	watiAndcheckExited(T_byeReader);
//		
//
//	//�ڳʱ��� �̵�
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
////�ڱ��¼����� �̵�
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
//	//ȸ�� �� �¼� x����� �̵�
//	carNode->carData.direction = 1;
//	setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//		carNode->carData.posX + carhorizon::width, carNode->carData.posY + carhorizon::height);
//
//	while (carNode->carData.posX <= destinationX)
//	{
//		//WaitForSingleObject(M_accessTree, INFINITE);
//		//�浹üũ
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
//	//ȸ�� �� �¼� y����� �̵�
//	carNode->carData.direction = 3;
//	setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//		carNode->carData.posX + carvertical::width, carNode->carData.posY + carvertical::height);
//
//	while (carNode->carData.posY <= collectionXY::seatY)
//	{
//		//WaitForSingleObject(M_accessTree, INFINITE);
//		//�浹üũ
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
////����� �̵�
//void gotocashier(BTreeNode* carNode)
//{
//	int carID = carNode->key;
//
//	//�Ʒ����� �̵�
//	//�ڳʱ��� �̵�
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
//	////�����浹üũ
//	////�浹�̾ȵȴٸ� ���ڸ��� �̵�
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
//	//�ڸ�����
//	releaseseat(carNode->carData.seat);
//
//	//X���̵�
//	while (carNode->carData.posX <= collectionXY::cornerbottomX)
//	{
//		//WaitForSingleObject(M_accessTree, INFINITE);
//		//�浹üũ
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
//	//������ȯ �� �浹 ���� ����
//	carNode->carData.direction = 2;
//	setcollsion(carNode, carNode->carData.posX, carNode->carData.posY,
//		carNode->carData.posX + carvertical::width, carNode->carData.posY + carvertical::height);
//
//
//	//y���̵�
//	while (carNode->carData.posY >= collectionXY::seatTop)
//	{
//		//WaitForSingleObject(M_accessTree, INFINITE);
//		//�浹üũ
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
////����� ��ȭ �� ����
////Ʈ������ ����
//
//
////������ ���� ��Ʈ�ѷ�
//unsigned WINAPI createCarThreads(void *arg)
//{
//	//avlTremm �ʱ�ȭ
//	BSTMakeAndInit(&avlRoot);
//
//	for (int i = 0; i < numOfseat; i++)
//		seats[i] = false;
//
//	//�����ʱ�ȭ
//	for (int i = 0; i < numOfcar; i++)
//	{
//		carArgument input; 
//		memset(&input, 0, sizeof(input));
//		//ID
//		input.id = i; 
//		//�����Ǵ� ��ġ	
//		input.posX = collectionXY::createdX;
//		input.posY = collectionXY::createdY;
//		//�浹����
//		input.rect.left = input.posX;
//		input.rect.top = input.posY;
//		input.rect.right = input.posX + carhorizon::width;
//		input.rect.right = input.posY + carhorizon::height;
//				
//		watiAndcheckExited(T_countingturnel);
//		watiAndcheckExited(M_accessTree);
//		//avl Ʈ�� ����
//		BSTInsert(&avlRoot, i, input);
//		ReleaseMutex(M_accessTree);
//		
//
//		//�������� ����
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
////�浹 �߻��� true ����
//bool collision(BTreeNode *root, BTreeNode *src, int carID, int mode)
//{
//	bool r = false;
//
//	if (root)
//	{
//		if (carID != root->key)
//		{
//			//�Ʒ�
//			if (mode == 1)
//			{
//				if ((src->carData.rect.left < root->carData.rect.right) &&
//					(src->carData.rect.top < root->carData.rect.bottom) &&
//					(src->carData.rect.right > root->carData.rect.left) &&
//					(src->carData.rect.bottom > root->carData.rect.top))
//				{
//					//���������� üũ�ص� ������ ������ ID������ üũ
//					if (carID < root->key)
//						return false;
//
//					return true;
//				}
//			}
//			//�����浹
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
////��� ������ true ����
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

//������
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

}

//step6 �����
void paytocashier(list<carArgument>::iterator arg)
{

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
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
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
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
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
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
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
				std::this_thread::sleep_for(std::chrono::milliseconds(speed));;
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

//////////////////////////////////////////////////////
//�浹 üũ ��
//////////////////////////////////////////////////////


//////////////////////////////////////////////////////
//�¼� ���õ� �Լ� ����
//////////////////////////////////////////////////////

//��� ������ true ����
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
//�¼� ���õ� �Լ� ����
//////////////////////////////////////////////////////

//��ȭ�� ��
int selectmovie()
{
	int movie = myrand(numOfmovie);

	return movie;
}