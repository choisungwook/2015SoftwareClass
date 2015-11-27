#pragma comment(lib, "ws2_32.lib")

#include "socket.h"
#include "function.h"
#include <process.h>
#include <string>
#include <vector>
#include <regex>
#define PORT 3333
using namespace std;

bool _servStatus;
WSADATA wsaData;

FunctionSet::iterator FunctionSetFinder;
extern FunctionSet functionset;


SOCKET accept(int servsock);
extern void token(regex &re, string msg, vector<string> &a);
extern void DebugFunc(TCHAR *pszErr, ...);

void ErrorHandling(char* message)
{
	fprintf(stderr, message);
	exit(-1);
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

	if (listen(hsock, 20) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	return hsock;
}

#define BUFSIZE 1024
void sendtoClnt(SOCKET clntsock, int Result)
{
	char sendmsg[BUFSIZE] = { 0, };
	sprintf(sendmsg, "%d", Result);
	send(clntsock, sendmsg, strlen(sendmsg), 0);
}


unsigned WINAPI serverWork(void *arg)
{
	int clntsock = *((int*)arg);
	char recvBuf[BUFSIZE] = { 0, };

	while (true)
	{
		//상대방이 소켓을 끊은경우 또는 비정상적인 종료
		if (recv(clntsock, recvBuf, sizeof(recvBuf), 0) == -1)
		{
			_endthreadex(0);
		}

		vector<string> tokenBuf;
		regex re("[\\.]+");
		token(re, recvBuf, tokenBuf);

		FunctionSetFinder = functionset.find(tokenBuf[0]);

		if (FunctionSetFinder != functionset.end())
		{
			int(*Func)(int, vector<string> &);
			Func = FunctionSetFinder->second;
			tokenBuf.erase(tokenBuf.begin());
			int result = Func(clntsock, tokenBuf);

			//결과값전송
			sendtoClnt(clntsock, result);			
		}
		else //존재하지 않는 명령어일 경우 소켓/쓰레드 종료
		{
			closesocket(clntsock);
			_endthreadex(0);
		}
	}
	return 0;
}


SOCKET accept(int servsock)
{
	SOCKET clntSock;
	SOCKADDR_IN ClntAddr;
	int ClntAddrsize = sizeof(ClntAddr);

	clntSock = accept(servsock, (SOCKADDR*)&ClntAddr, &ClntAddrsize);

	if (clntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	return clntSock;
}

unsigned WINAPI startServer(void *arg)
{
	int port = *((int*)arg);
	SOCKET servsock = serverSocket(port);

	_servStatus = true;

	while (_servStatus)
	{
		SOCKET hClntSock = accept(servsock);
		char recvBuf[BUFSIZE] = { 0, };

		HANDLE clntHandle = (HANDLE)_beginthreadex(0, 0, serverWork, (void*)&hClntSock, 0, 0);
		CloseHandle(clntHandle);
	}

	_servStatus = false;
	closesocket(servsock);

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
	_servStatus = false;
}

////////////////////////////////////////
//////// 종료
////////////////////////////////////////
