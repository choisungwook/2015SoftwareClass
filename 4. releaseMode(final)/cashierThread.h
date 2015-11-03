#ifndef __CASHIER_THREAD_H__
#define __CASHIER_THREAD_H__

#include <Windows.h>

unsigned WINAPI cashierThread(void *arg);
void destorycashierThread();

#endif