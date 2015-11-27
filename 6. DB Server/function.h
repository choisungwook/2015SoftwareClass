#ifndef __FUNCTION_H__
#define	__FUNCTION_H__
#define KEY_NOTFOUND -1
#define KEY_ADD -1
#define KEY_EXISTED 1
#define KEY_UPDATE 1
#define KEY_DELETE 1

#include "carArgument.h"
#include <Windows.h>
#include <map>
#include <string>
#include <vector>

typedef carArgument carArg;

typedef int(*commandFunction)(int, std::vector<std::string> &);
typedef std::map <std::string, commandFunction> FunctionSet;
typedef std::pair<std::string, commandFunction> commandPair;

int checkExisted(int hsock, std::vector<std::string> &data);
int UpdateDB(int hsock, std::vector<std::string> &data);
int closeDB(int hsock, std::vector<std::string> &data);
int DeleteDB(int hsock, std::vector<std::string> &data);

void initializeCommand();
#endif