#include "avltree.h"
#include <Windows.h>
using namespace std;

void Filesave(char* path);
void Fileopen(char *path);

//데이터가 있는 트리
AVLTree car;

void Filesave(char* path)
{
	FILE *fout = fopen(path, "w+b");
	car.save(fout);
	fclose(fout);
}

void Fileopen(char *path)
{
	FILE *fin = fopen(path, "rb");	
	car.load(fin);
	fclose(fin);
}