#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <Windows.h>

void watiAndcheckExited(HANDLE hobject);
void checkexited();
unsigned int myrand(int max);

//차ID중복검사
bool checkduplication(int carID);
void initializecarmap();
void deletecarduplicate(int carID);

#endif