#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <Windows.h>

//��������
typedef struct _Paydata
{
	int id;
	int cash;
	int movietime;
}Paydata;

void watiAndcheckExited(HANDLE hobject);
void checkexited();
unsigned int myrand(int max);

//��ID�ߺ��˻�
bool checkduplication(int carID);
void initializecarmap();
void deletecarduplicate(int carID);

#endif