#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <winsock.h>

//�Լ�
SOCKET serverSocket(int port);
unsigned WINAPI startServer(void *arg);
bool statusSevrer();
void StopServer();
void initializeWinsock();
void destoryWinsock();


#endif