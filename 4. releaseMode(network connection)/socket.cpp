#include "socket.h"

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

SOCKET connect_Server()
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
void recvfromserver(int hsock)
{
	char message[BUFSIZE];
	int strLen;

	strLen = recv(hsock, message, BUFSIZE - 1, 0);

	if (strLen == -1)
		ErrorHandling("read() error!");
	
	MessageBox(hWndMain, message, "OK", MB_OK);
}

void sendfromserver(SOCKET hsock, char *message)
{
	send(hsock, message, strlen(message), 0);
}
