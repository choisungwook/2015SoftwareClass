#ifndef __UTILITY_H__
#define	__UTILITY_H__

#define MAXOFTURNEL 4 //터널 카운트 설정
#define numOfmovie	5 //영화 개수
#include <iostream>
#include <Windows.h>
#include <string>

//영화
extern std::string movie[numOfmovie];
extern int moviePrice[numOfmovie];
extern int movieTime[numOfmovie];

//세마포어
extern HANDLE		SEMA_turnel; //터널 카운터 동기화
extern HANDLE		Hellow_READER;
extern HANDLE		Leave_READER;
extern HANDLE		Enter_READER;
extern HANDLE		hDown_READER;
extern HANDLE		hselect_READER;

//뮤텍스
extern HANDLE		collisionMutex; //충돌체크 뮤텍스
extern HANDLE		screenMutex;
extern HANDLE		seatMutex;


//난수함수
unsigned int myrand(int max);

//핸들함수
void SemaphoreInit();
void DestorySemaphore();

//영화함수
void movieInit();
void printmovie();

//충돌사각형영역 변경
void changeRectPosition(RECT *pRect, int left, int top, int right, int bottom);

//좌석함수
void seatInit();
#endif