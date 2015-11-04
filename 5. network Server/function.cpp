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

void serverLogin(clntsockData clntData, carArg argument)
{
	cout << "login called" << endl;
}

void serverRevise(clntsockData clntData, carArg argument)
{

}

void serverInsert(clntsockData clntData, carArg argument)
{
	cout << "Insert called" << endl;
}

void serverRemove(clntsockData clntData, carArg argument)
{

}

