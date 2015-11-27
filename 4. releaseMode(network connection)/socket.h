#ifndef __SOCKET_H__
#define	__SOCKET_H__

#define KEY_NOTFOUND -1
#define KEY_ADD -1
#define KEY_EXISTED 1
#define KEY_UPDATE 1
#define KEY_DELETE 1

#include <winsock2.h>

void initializeWinsock();
void destoryWinsock();

SOCKET openDB();
void checkExisted(SOCKET hsock, int carID);
void UpdateDB(SOCKET hsock, const char* format, ...);
void closeDB(SOCKET hsock);
int recvfromDB(SOCKET hsock);

#endif