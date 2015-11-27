#pragma comment(lib, "ws2_32.lib")

#include "socket.h"
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

void ErrorHandling(char* message)
{	
	MessageBox(hWndMain, message, "error", MB_OK);	
	exit(-1);
}

////////////////////////////////////////
//////// WINSOCK 초기화 및 파괴
////////////////////////////////////////
void initializeWinsock()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");
}

void destoryWinsock()
{
	WSACleanup();
}

SOCKET openDB()
{
	SOCKADDR_IN servAddr;
	SOCKET hsock;

	hsock = socket(PF_INET, SOCK_STREAM, 0);

	if (hsock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(port);

	if (connect(hsock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	return hsock;
}

#define BUFSIZE 1024
int recvfromDB(SOCKET hsock)
{
	char message[BUFSIZE];
	int strLen, r;

	strLen = recv(hsock, message, BUFSIZE - 1, 0);

	if (strLen == -1)
		ErrorHandling("read() error!");	
	
	sscanf(message, "%d", &r);

	return r;	
}

//차량 번호가 서버에 있는지 체크
void checkExisted(SOCKET hsock, int carID)
{
	char sendMessage[BUFSIZE];
	sprintf(sendMessage, "Check.%d", carID);
	send(hsock, sendMessage, strlen(sendMessage), 0);
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

	send(hsock, sendMessage, strlen(sendMessage), 0);
}

//연결종료
void closeDB(SOCKET hsock)
{
	char sendMessage[BUFSIZE];
	sprintf(sendMessage, "Close");
	send(hsock, sendMessage, strlen(sendMessage), 0);

	closesocket(hsock);
}