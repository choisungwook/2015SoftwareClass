#ifndef __AVLTREE_H__
#define __AVLTREE_H__

#include "carArgument.h"

#include <mutex>

class AVLNODE
{
public:
	int key;
	int balance;
	carArgument Data;
	AVLNODE* left, *right, *parent;

	AVLNODE(int k, carArgument data, AVLNODE *p) : key(k), balance(0), parent(p), Data(data), 
		left(0), right(0) {}

	~AVLNODE()
	{
		delete left;
		delete right;
	}
};

class AVLTree
{
private:
	std::mutex access;
	AVLNODE *root;
	void SetBalance(AVLNODE *n);
	void reBalance(AVLNODE *n);
	int height(AVLNODE *n);
	//ȸ��
	AVLNODE* rotateRight(AVLNODE *a);
	AVLNODE* rotateLeftThenRight(AVLNODE *n);
	AVLNODE* rotateLeft(AVLNODE *a);
	AVLNODE* rotateRightThenLeft(AVLNODE *n);
public:
	int getvisitCount();
	AVLTree();
	~AVLTree();
	AVLNODE* getRoot();
	AVLNODE* search(int key);	
	bool insert(int key, carArgument data);
	bool remove(int key);
	void save(FILE* fp);
	void load(FILE* fp);
	void update(AVLNODE* updateNode, int operate, void* data);
};
#endif