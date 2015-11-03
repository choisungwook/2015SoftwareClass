#ifndef __READER_THREAD_H__
#define __READER_THREAD_H__

#include <Windows.h>

unsigned WINAPI readerThread(void *arg);
void destoryReaderThread();

#endif