#include <iostream>
#include <string>
#include "function.h"
#include "avltree.h"
#include <process.h>
using namespace std;

FunctionSet functionset;
extern AVLTree car;

void initializeCommand()
{
	functionset.insert(commandPair("Connect", serverConnect));
	functionset.insert(commandPair("Update", serverUpdate));
	//functionset.insert(commandPair("Disconnect", serverDisconnect));
}

//처음 연결 요청
void serverConnect(int hsock, vector<string> &data)
{
	if (car.search(stoi(data[0])) == NULL)
	{
		carArgument t(stoi(data[0]));		
		car.insert(t.carnumber, t);
	}
}

//데이터 추가/수정
void serverUpdate(int hsock, vector<string> &data)
{
	AVLNODE *updateNode = car.search(stoi(data[0]));

	for (int i = 1; i < data.size(); i += 2)
	{
		int operand = stoi(data[i]);
		car.update(updateNode, operand, (void*)&data[i+1]);
	}	
}

void serverDisconnect(int hsock, vector<string> &data)
{
	_endthreadex(0);
}

