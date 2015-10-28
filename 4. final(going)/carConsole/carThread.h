#ifndef __CARTHREAD_H__
#define __CARTHREAD_H__

#define numOfCar	12
#define numOfSeat	6
#define CAR00HEIGHT	32 //차량 0번의 방향 오른쪽 세로
#define CAR00WIDTH	100 //차량 0번의 방향 오른쪽 가로
#define CAR02HEIGHT	68 //차량 0번의 방향 위 세로
#define CAR02WIDTH	66 //차량 0번의 방향 위 가로

#include <process.h>
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;

typedef struct carArg
{	
	int id;
	int posX;
	int posY;
	RECT rect;
	int sort;
	int direction;
	//좌석
	unsigned int seat;
	//영화
	string movieID;
	int moviePrice;
	int movieTime;
}carArg;


//차쓰레드
extern HANDLE		carThread[numOfCar];
//차쓰레드 인자
extern carArg		arg[numOfCar];
//좌석
extern BOOL seatArray[numOfCar];


//차 쓰레드 함수
unsigned WINAPI CreateCarThread(void *ARG);
void destoryCarThread();
unsigned WINAPI car(void *arg);

//행동함수
void enterTurnel(carArg *Arg);
void TalktoReader(carArg *Arg);
void selectMovie(carArg *arg); //영화를 고른다
void goseat(carArg *Arg);
unsigned int getSeat(); //좌석을 고른다
bool isemptySeat(unsigned int seat);
void ReleaseSeat(int seat);



#endif