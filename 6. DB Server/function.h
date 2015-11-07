#ifndef __FUNCTION_H__
#define	__FUNCTION_H__

#include "carArgument.h"
#include <Windows.h>
#include <map>
#include <string>
#include <vector>

typedef struct _clntsockData
{
	SOCKET hsock;
	std::string msg;
}clntsockData;

typedef carArgument carArg;

typedef void(*commandFunction)(int, std::vector<std::string> &);
typedef std::map <std::string, commandFunction> FunctionSet;
typedef std::pair<std::string, commandFunction> commandPair;

void serverConnect(int hsock, std::vector<std::string> &data);
void serverUpdate(int hsock, std::vector<std::string> &data);
void serverDisconnect(int hsock, std::vector<std::string> &data);

void initializeCommand();



#endif