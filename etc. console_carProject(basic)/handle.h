#ifndef __HANDLE_H__
#define __HANDLE_H__

//초기화 관련됨 함수들
void createThreads();
void initializeHandles();
void initializeSemaphores();
void initializeMutexs();
//파괴외 관련된 함수들
void destoryThreads();
void destorySemaphores();
void destoryMutexs();
void destoryHandles();
//모든 핸들 활성화
void AllActivateHandles();

#endif