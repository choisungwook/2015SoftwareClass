#ifndef __HANDLE_H__
#define __HANDLE_H__

//�ʱ�ȭ ���õ� �Լ���
void createThreads();
void initializeHandles();
void initializeSemaphores();
void initializeMutexs();
//�ı��� ���õ� �Լ���
void destoryThreads();
void destorySemaphores();
void destoryMutexs();
void destoryHandles();
//��� �ڵ� Ȱ��ȭ
void AllActivateHandles();

#endif