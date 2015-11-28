#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <Windows.h>

//돈계산관련
typedef struct _Paydata
{
	int id;
	int cash;
	int movietime;
}Paydata;

void watiAndcheckExited(HANDLE hobject);
void checkexited();
unsigned int myrand(int max);

//차ID중복검사
bool checkduplication(int carID);
void initializecarmap();
void deletecarduplicate(int carID);

#endif