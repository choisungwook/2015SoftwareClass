#ifndef __RECOGNIZER_H__
#define	__RECOGNIZER_H__

#include <Windows.h>
#include <process.h>

void createReaderThread();
void DestoryReaderThread();
unsigned WINAPI Entrance_worker(void *arg);

#endif