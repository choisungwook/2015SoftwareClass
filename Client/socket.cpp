#pragma comment(lib, "ws2_32.lib")

#include "socket.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <iostream>
using namespace std;

bool _servStatus;
bool _exitFlag;
WSADATA wsaData;

extern	HWND	hWndMain;
int port;

extern void ErrorHandling(char* message);


////////////////////////////////////////
//////// WINSOCK 초기화 및 파괴
////////////////////////////////////////
void initializeWinsock()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling(ERR_WSASTART);
}

void destoryWinsock()
{
	WSACleanup();
}
////////////////////////////////////////

SOCKET openDB()
{
	SOCKADDR_IN servAddr;
	SOCKET hsock;

	hsock = socket(PF_INET, SOCK_STREAM, 0);

	if (hsock == INVALID_SOCKET)
		ErrorHandling(ERR_SOCKET);

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(port);

	if (connect(hsock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling(ERR_CONNECT);

	return hsock;
}

#define BUFSIZE 1024
int recvfromDB(SOCKET hsock)
{
	char message[BUFSIZE];
	int strLen, r;

	strLen = recv(hsock, message, BUFSIZE - 1, 0);
	
	OutputDebugString("받은값 : ");
	OutputDebugString(message);

	if (strLen == -1)
		ErrorHandling(ERR_READ);
	
	sscanf(message, "%d", &r);

	return r;	
}

//차량 번호가 서버에 있는지 체크
void checkExisted(SOCKET hsock, int carID)
{
	char sendMessage[BUFSIZE];
	sprintf(sendMessage, "Check.%d", carID);
	if(send(hsock, sendMessage, strlen(sendMessage), 0) < 0)
		ErrorHandling(ERR_SEARCH);
}

//업데이트
void UpdateDB(SOCKET hsock, const char* format, ...)
{
	va_list arglist;
	char pharsingBuf[BUFSIZE];
	char sendMessage[BUFSIZE] = "Update.";

	va_start(arglist, format);
	vsprintf(pharsingBuf, format, arglist);
	va_end(arglist);

	strcat(sendMessage, pharsingBuf);
	//OutputDebugString(sendMessage);

	if(send(hsock, sendMessage, strlen(sendMessage), 0) < 0)
		ErrorHandling(ERR_UPDATE);
}

//연결종료
void closeDB(SOCKET hsock)
{
	char sendMessage[BUFSIZE];
	sprintf(sendMessage, "Close");
	if(send(hsock, sendMessage, strlen(sendMessage), 0) < 0)
		ErrorHandling(ERR_CLOSE);

	closesocket(hsock);
}

void serchDB(SOCKET hsock,int id)
{
	char sendMessage[BUFSIZE];
	sprintf(sendMessage, "Search.%d",id);
	if(send(hsock, sendMessage, strlen(sendMessage), 0) < 0)
		ErrorHandling(ERR_SEARCH);
}