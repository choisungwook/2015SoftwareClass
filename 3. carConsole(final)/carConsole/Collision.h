#ifndef __CARCOLIISION_H__
#define __CARCOLIISION_H__

#include <Windows.h>
#include "carThread.h"

//test
bool testCollision(int carID, RECT *src);
void setCollision(carArg *arg, int a, int b);
bool testBottomCollision(int carID, RECT *src);	//����ΰ��� �浹���
bool crossCollision(int carID, RECT *src);
#endif