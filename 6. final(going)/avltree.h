#ifndef __AVLTREEL_H__
#define __AVLTREEL_H__

#include "carThread.h"

typedef int BTkey;

///////////////////////////////////////////////////////////
//////////////// ����Ʈ��
///////////////////////////////////////////////////////////
typedef struct _bTreeNode
{
	BTkey key;
	carArgument carData;
	struct _bTreeNode * left;
	struct _bTreeNode * right;
} BTreeNode;

BTreeNode * MakeBTreeNode(void);
carArgument GetcarData(BTreeNode * bt);
BTkey Getkey(BTreeNode * bt);
void SetcarData(BTreeNode *bt, carArgument carData);
void Setkey(BTreeNode * bt, BTkey key);

BTreeNode * GetLeftSubTree(BTreeNode * bt);
BTreeNode * GetRightSubTree(BTreeNode * bt);

void MakeLeftSubTree(BTreeNode * main, BTreeNode * sub);
void MakeRightSubTree(BTreeNode * main, BTreeNode * sub);

typedef void VisitFuncPtr(carArgument Data);

void PreorderTraverse(BTreeNode * bt, VisitFuncPtr action);
void InorderTraverse(BTreeNode * bt, VisitFuncPtr action);
void PostorderTraverse(BTreeNode * bt, VisitFuncPtr action);

// ���� �ڽ� ��� ����, ���ŵ� ����� �ּ� ���� ��ȯ�ȴ�.
BTreeNode * RemoveLeftSubTree(BTreeNode * bt);

// ������ �ڽ� ��� ����, ���ŵ� ����� �ּ� ���� ��ȯ�ȴ�.
BTreeNode * RemoveRightSubTree(BTreeNode * bt);

// �޸� �Ҹ��� �������� �ʰ� main�� ���� �ڽ� ��带 �����Ѵ�. 
void ChangeLeftSubTree(BTreeNode * main, BTreeNode * sub);

// �޸� �Ҹ��� �������� �ʰ� main�� ������ �ڽ� ��带 �����Ѵ�. 
void ChangeRightSubTree(BTreeNode * main, BTreeNode * sub);

///////////////////////////////////////////////////////////
//////////////// avlƮ��
///////////////////////////////////////////////////////////
typedef BTkey	BSTkey;

// ���� Ž�� Ʈ���� ���� �� �ʱ�ȭ
void BSTMakeAndInit(BTreeNode ** pRoot);
// ��忡 ����� ������ ��ȯ
BSTkey BSTGetNodekey(BTreeNode * bst);
// ���� Ž�� Ʈ���� ������� ������ ����(����� �������� ����)
BTreeNode * BSTInsert(BTreeNode ** pRoot, BSTkey key, carArgument carData);
// ���� Ž�� Ʈ���� ������� ������ Ž��
BTreeNode * BSTSearch(BTreeNode * bst, BSTkey target);
// Ʈ������ ��带 �����ϰ� ���ŵ� ����� �ּ� ���� ��ȯ�Ѵ�. 
BTreeNode * BSTRemove(BTreeNode ** pRoot, BSTkey target);
// ���� Ž�� Ʈ���� ����� ��� ����� �����͸� ����Ѵ�.
void BSTShowAll(BTreeNode * bst);
// Ʈ���� ������ ��´�.
BTreeNode * Rebalance(BTreeNode ** pRoot);

//ȸ��
BTreeNode * RotateLL(BTreeNode * bst);
BTreeNode * RotateRR(BTreeNode * bst);
BTreeNode * RotateRL(BTreeNode * bst);
BTreeNode * RotateLR(BTreeNode * bst);
int GetHeight(BTreeNode * bst);
int GetHeightDiff(BTreeNode * bst);
BTreeNode * Rebalance(BTreeNode ** pRoot);
#endif