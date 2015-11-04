#ifndef __SOCKET_H__
#define	__SOCKET_H__

#include <winsock2.h>

void initializeWinsock();
void destoryWinsock();
unsigned WINAPI startServer(void *arg);
void startServer(int hServersock);
bool statusSevrer();
void StopServer();

#endif