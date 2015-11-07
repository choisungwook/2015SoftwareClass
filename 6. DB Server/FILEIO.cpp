#include "avltree.h"
#include <string>
#include <Windows.h>
#include <vector>
#include <regex>
using namespace std;

void Filesave(char* path);
extern void token(regex &re, string msg, vector<string> &a);

extern HANDLE M_accessTree;

//데이터가 있는 트리
AVLTree car;

void Filesave(char* path)
{
	//덮어쓰기
	FILE *fout = fopen(path, "w+b");
	WaitForSingleObject(M_accessTree, INFINITE);
	car.save(fout);
	ReleaseMutex(M_accessTree);
	fclose(fout);
}

void Fileopen(char *path)
{
	char buf[1024] = { 0, };
	regex re("[\\.]+|[\\n.]+");

	FILE *fin = fopen(path, "rb");

	while (fgets(buf, sizeof(buf), fin))
	{
		vector<string> msg;
		carArgument t;		

		token(re, buf, msg);
		t.carnumber = stoi(msg[0]);
		t.sortOfcar = stoi(msg[1]);
		t.totalPrice = stoi(msg[2]);
		t.totalmovieTime = stoi(msg[3]);
		t.lastVisit = msg[4];

		for (int i = 5; i < msg.size(); i++)
			t.movieName.push_back(msg[i]);
		
		car.insert(t.carnumber, t);		
	}

	fclose(fin);
}