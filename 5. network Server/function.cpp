#include <iostream>
#include <string>
#include "function.h"
using namespace std;

FunctionSet functionset;

void initializeCommand()
{
	functionset.insert(commandPair("Login", serverLogin));
	functionset.insert(commandPair("Insert", serverInsert));
	
}

void serverLogin(int hsock, string data)
{
	cout << "login called" << endl;
}

void serverRevise(clntsockData clntData, carArg argument)
{

}

void serverInsert(int hsock, string data)
{
	cout << "Insert called" << endl;
}

void serverRemove(clntsockData clntData, carArg argument)
{

}

