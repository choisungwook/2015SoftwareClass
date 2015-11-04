#ifndef __SOCKET_H__
#define	__SOCKET_H__

#include <winsock2.h>

void initializeWinsock();
void destoryWinsock();
SOCKET connect_Server();
void recvfromserver(int hsock);

#endif