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
	functionset.insert(commandPair("Check", checkExisted)); //���ڵ尡 �ִ��� üũ
	functionset.insert(commandPair("Update", UpdateDB)); //�����ͼ���
	functionset.insert(commandPair("Close", closeDB)); //��������
	functionset.insert(commandPair("Delete", DeleteDB)); //����
}

//ó�� ���� ��û
//DB�� ���� ��ϵǾ� �ִ��� Ȯ���Ѵ�
//���ٸ� ����ϰ� -1�� ����
//�ִٸ� 1�� ����
//@data[0] ����ȣ
int checkExisted(int hsock, vector<string> &data)
{
	if (car.search(stoi(data[0])) == NULL)
	{
		carArgument t(stoi(data[0]));		
		car.insert(t.carnumber, t);
		return KEY_NOTFOUND;
	}
	return car.getvisitCount();
}

//���ڵ� ����
//�����ϸ� -1 ����
//@data ����ȣ
int DeleteDB(int hsock, vector<string> &data)
{
	//���� ����
	if(car.remove(stoi(data[0])))
		return KEY_DELETE;
	
	return KEY_NOTFOUND;
}

//���ڵ� �߰�/����
//@data ����ȣ
//@data[n/2 + 1] command
//@data[n/2] revise data
int UpdateDB(int hsock, vector<string> &data)
{
	AVLNODE *updateNode = car.search(stoi(data[0]));

	if (updateNode == NULL)
		return KEY_NOTFOUND;

	for (int i = 1; i < data.size(); i += 2)
	{
		int operand = stoi(data[i]);
		car.update(updateNode, operand, (void*)&data[i+1]);
	}	

	return KEY_UPDATE;
}

//������ �����ϰ�
//���� ����ϰ�
//�����带 �����Ѵ�.
//@data �������� ����
int closeDB(int hsock, vector<string> &data)
{
	closesocket(hsock);
	//car.save();
	_endthreadex(0);

	return 0;
}