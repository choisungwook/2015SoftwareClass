#pragma comment(lib, "ws2_32.lib")

#include "socket.h"
#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
using namespace std;

bool _servStatus;
bool _exitFlag; 
WSADATA wsaData;

FunctionSet::iterator FunctionSetFinder;
extern FunctionSet functionset;

void ErrorHandling(char* message)
{
	fprintf(stderr, message);
	exit(-1);
}

////////////////////////////////////////
//////// 정규식 토큰화
////////////////////////////////////////

void token(string msg, vector<string> &a)
{
	regex re("\\.+");
	sregex_token_iterator it(msg.begin(), msg.end(), re, -1);
	
	sregex_token_iterator reg_end;
	for (; it != reg_end; ++it) {
		//std::cout << it->str() << std::endl;
		a.push_back(it->str());
	}
}

////////////////////////////////////////////////
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
////////////////////////////////////////
//////// 종료
////////////////////////////////////////

////////////////////////////////////////
//////// 서버소켓 
////////////////////////////////////////
SOCKET serverSocket(int port)
{
	SOCKADDR_IN servAddr;
	SOCKET hsock;

	hsock = socket(PF_INET, SOCK_STREAM, 0);

	if (hsock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(hsock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hsock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	_exitFlag = true;

	return hsock;
}

#define BUFSIZE 1024
unsigned WINAPI startServer(void *arg)
{
	int port = *((int*)arg);

	cout << "port : " << port << endl;
	SOCKET hserversock = serverSocket(port);

	while (_exitFlag)
	{
		SOCKET hClntSock;
		SOCKADDR_IN ClntAddr;
		char recvBuf[BUFSIZE] = { 0, };
		char hello[] = "hello";

		int ClntAddrsize = sizeof(ClntAddr);

		if (!_servStatus)
			_servStatus = true;

		hClntSock = accept(hserversock, (SOCKADDR*)&ClntAddr, &ClntAddrsize);
		
		if (hClntSock == INVALID_SOCKET)
			ErrorHandling("accept() error");

		int recvLen = recv(hClntSock, recvBuf, sizeof(recvBuf), 0);		
		vector<string> tokenBuf;
		string msg(recvBuf);
		token(msg, tokenBuf);
		
		FunctionSetFinder = functionset.find(tokenBuf[0]);

		if (FunctionSetFinder != functionset.end())
		{
			clntsockData sockData;
			sockData.hsock = hClntSock;

			cout << "Search Function data " << endl;
			void (*Func)(int, string);
			Func = FunctionSetFinder->second;
			Func(hClntSock, msg);
		}

		//send(hClntSock, hello, sizeof(hello), 0);
		printf("sended\n");

		closesocket(hClntSock);
	}

	_servStatus = false;
	closesocket(hserversock);

	_endthreadex(0);
	return 0;
}

//실행중이면 true
bool statusSevrer()
{
	if (_servStatus)
		return true;
	
	return false;
}
void StopServer()
{
	_exitFlag = false;
	_servStatus = false;
}

////////////////////////////////////////
//////// 종료
////////////////////////////////////////

