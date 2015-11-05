#ifndef __FUNCTION_H__
#define	__FUNCTION_H__

#include "carArgument.h"
#include <Windows.h>
#include <map>
#include <string>

typedef struct _clntsockData
{
	SOCKET hsock;
	std::string msg;
}clntsockData;

typedef carArgument carArg;

typedef void(*commandFunction)(int, std::string);
typedef std::map <std::string, commandFunction> FunctionSet;
typedef std::pair<std::string, commandFunction> commandPair;

void serverLogin(int hsock, std::string data);
void serverInsert(int hsock, std::string data);
//void serverRemove(clntsockData clntData, carArg argument);

void initializeCommand();



#endif