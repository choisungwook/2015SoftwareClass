#ifndef __AVLTREEL_H__
#define __AVLTREEL_H__

#include <list>
#include <string>
#include <stack>
#include <stdio.h>

class carArgument
{
public:
	int						carnumber;	//����ȣ
	int						sortOfcar;  //������
	int						totalPrice; //�ѱݾ�
	std::string				lastVisit; //�������湮��¥	
	//��ȭ	
	std::list<std::string>	movieName; //�ڱⰡ ����ȭ ��ϵ�		
	int						totalmovieTime;	//��ȭ�� �� �ð� (�ʴ����� ����)
	carArgument(int _carnumber, int _sortOfcar, int _totalPrice, int _totalmovieTime,
		std::string _lastVisit, std::list<std::string>movie)
	{
		carnumber = _carnumber; sortOfcar = _sortOfcar;
		totalPrice = _totalPrice; totalmovieTime = _totalmovieTime;
		lastVisit = _lastVisit; movieName = movie;
	}
	carArgument() { };
	~carArgument()
	{
		movieName.clear();
	}
};

typedef struct _Node
{
public:
	int key;
	carArgument carData;
	struct _Node * left;
	struct _Node * right;
	_Node()
	{
		left = NULL;
		right = NULL;
	}
}NODE;


class AVLTREE
{
private:
	NODE *root;
public:
	AVLTREE() { root = NULL; };
	~AVLTREE() { delete root; };
	NODE** getRootAddress();
	NODE* getRoot();
	carArgument& getData();
	//AVL Ʈ�� ���
	NODE* Insert(NODE** pRoot, int key, carArgument &Data);
	void  Remove(NODE **pRoot, int target);
	NODE* Search(NODE* Root, int target);
	NODE* FirstRevise(NODE* Root, int target, int operate, void* data);
	void  AfterRevise(NODE* Root, int operate, void* data);
	//ȸ��
	NODE* RotateLL(NODE* Root);
	NODE* RotateRR(NODE* Root);
	NODE* RotateRL(NODE* Root);
	NODE* RotateLR(NODE* Root);
	//����
	NODE* Rebalance(NODE** pRoot);
	int GetHeightDiffernce(NODE* Root); //���� ������ ������ ���� ���ϴ� �Լ�
	int GetHeight(NODE* Root); //���̸� ���ϴ� �Լ�
	//preOrder ��� x
	void save(FILE *fp);
};


#endif