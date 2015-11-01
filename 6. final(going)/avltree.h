#ifndef __AVLTREEL_H__
#define __AVLTREEL_H__

#include "carThread.h"

typedef int BTkey;

///////////////////////////////////////////////////////////
//////////////// 이진트리
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

// 왼쪽 자식 노드 제거, 제거된 노드의 주소 값이 반환된다.
BTreeNode * RemoveLeftSubTree(BTreeNode * bt);

// 오른쪽 자식 노드 제거, 제거된 노드의 주소 값이 반환된다.
BTreeNode * RemoveRightSubTree(BTreeNode * bt);

// 메모리 소멸을 수반하지 않고 main의 왼쪽 자식 노드를 변경한다. 
void ChangeLeftSubTree(BTreeNode * main, BTreeNode * sub);

// 메모리 소멸을 수반하지 않고 main의 오른쪽 자식 노드를 변경한다. 
void ChangeRightSubTree(BTreeNode * main, BTreeNode * sub);

///////////////////////////////////////////////////////////
//////////////// avl트리
///////////////////////////////////////////////////////////
typedef BTkey	BSTkey;

// 이진 탐색 트리의 생성 및 초기화
void BSTMakeAndInit(BTreeNode ** pRoot);
// 노드에 저장된 데이터 반환
BSTkey BSTGetNodekey(BTreeNode * bst);
// 이진 탐색 트리를 대상으로 데이터 저장(노드의 생성과정 포함)
BTreeNode * BSTInsert(BTreeNode ** pRoot, BSTkey key, carArgument carData);
// 이진 탐색 트리를 대상으로 데이터 탐색
BTreeNode * BSTSearch(BTreeNode * bst, BSTkey target);
// 트리에서 노드를 제거하고 제거된 노드의 주소 값을 반환한다. 
BTreeNode * BSTRemove(BTreeNode ** pRoot, BSTkey target);
// 이진 탐색 트리에 저장된 모든 노드의 데이터를 출력한다.
void BSTShowAll(BTreeNode * bst);
// 트리의 균형을 잡는다.
BTreeNode * Rebalance(BTreeNode ** pRoot);

//회전
BTreeNode * RotateLL(BTreeNode * bst);
BTreeNode * RotateRR(BTreeNode * bst);
BTreeNode * RotateRL(BTreeNode * bst);
BTreeNode * RotateLR(BTreeNode * bst);
int GetHeight(BTreeNode * bst);
int GetHeightDiff(BTreeNode * bst);
BTreeNode * Rebalance(BTreeNode ** pRoot);
#endif