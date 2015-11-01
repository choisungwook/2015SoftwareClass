#include "avltree.h"
#include <malloc.h>
#include <stdio.h>


BTreeNode * MakeBTreeNode(void)
{
	BTreeNode * nd = (BTreeNode*)malloc(sizeof(BTreeNode));

	nd->left = NULL;
	nd->right = NULL;
	return nd;
}

BTkey Getkey(BTreeNode * bt)
{
	return bt->key;
}

void Setkey(BTreeNode * bt, BTkey key)
{
	bt->key = key;
}

void SetcarData(BTreeNode *bt, carArgument carData)
{
	bt->carData = carData;
}

carArgument GetcarData(BTreeNode * bt)
{
	return bt->carData;
}


BTreeNode * GetLeftSubTree(BTreeNode * bt)
{
	return bt->left;
}

BTreeNode * GetRightSubTree(BTreeNode * bt)
{
	return bt->right;
}

void MakeLeftSubTree(BTreeNode * main, BTreeNode * sub)
{
	if (main->left != NULL)
		free(main->left);

	main->left = sub;
}

void MakeRightSubTree(BTreeNode * main, BTreeNode * sub)
{
	if (main->right != NULL)
		free(main->right);

	main->right = sub;
}

void PreorderTraverse(BTreeNode * bt, VisitFuncPtr action)
{
	if (bt == NULL)
		return;

	action(bt->carData);
	PreorderTraverse(bt->left, action);
	PreorderTraverse(bt->right, action);
}

void InorderTraverse(BTreeNode * bt, VisitFuncPtr action)
{
	if (bt == NULL)
		return;

	InorderTraverse(bt->left, action);
	action(bt->carData);
	InorderTraverse(bt->right, action);
}

void PostorderTraverse(BTreeNode * bt, VisitFuncPtr action)
{
	if (bt == NULL)
		return;

	PostorderTraverse(bt->left, action);
	PostorderTraverse(bt->right, action);
	action(bt->carData);
}

BTreeNode * RemoveLeftSubTree(BTreeNode * bt)
{
	BTreeNode * delNode = NULL;

	if (bt != NULL) {
		delNode = bt->left;
		bt->left = NULL;
	}
	return delNode;
}

BTreeNode * RemoveRightSubTree(BTreeNode * bt)
{
	BTreeNode * delNode = NULL;

	if (bt != NULL) {
		delNode = bt->right;
		bt->right = NULL;
	}
	return delNode;
}

void ChangeLeftSubTree(BTreeNode * main, BTreeNode * sub)
{
	main->left = sub;
}

void ChangeRightSubTree(BTreeNode * main, BTreeNode * sub)
{
	main->right = sub;
}



///////////////////////////////////////////////////////////
//////////////// avlƮ��
///////////////////////////////////////////////////////////
void BSTMakeAndInit(BTreeNode ** pRoot)
{
	*pRoot = NULL;
}

BSTkey BSTGetNodekey(BTreeNode * bst)
{
	return Getkey(bst);
}

BTreeNode * BSTInsert(BTreeNode ** pRoot, BSTkey key, carArgument carData)
{
	if (*pRoot == NULL)
	{
		*pRoot = MakeBTreeNode();
		Setkey(*pRoot, key);
		SetcarData(*pRoot, carData);
	}
	else if (key < Getkey(*pRoot))
	{
		BSTInsert(&((*pRoot)->left), key, carData);
		*pRoot = Rebalance(pRoot);		
	}
	else if (key > Getkey(*pRoot))
	{
		BSTInsert(&((*pRoot)->right), key, carData);
		*pRoot = Rebalance(pRoot);		
	}
	else
	{
		return NULL; // Ű�� �ߺ��� ������� �ʴ´�.
	}
	return *pRoot;
}



BTreeNode * BSTSearch(BTreeNode * bst, BSTkey target)
{
	BTreeNode * cNode = bst;    // current node
	BSTkey cd;    // current key

	while (cNode != NULL)
	{
		cd = Getkey(cNode);

		if (target == cd)
			return cNode;
		else if (target < cd)
			cNode = GetLeftSubTree(cNode);
		else
			cNode = GetRightSubTree(cNode);
	}

	return NULL;
}

BTreeNode * BSTRemove(BTreeNode ** pRoot, BSTkey target)
{
	// ���� ����� ��Ʈ ����� ��츦 ������ ����ؾ� �Ѵ�.

	BTreeNode * pVRoot = MakeBTreeNode();    // ���� ��Ʈ ���;

	BTreeNode * pNode = pVRoot;    // parent node
	BTreeNode * cNode = *pRoot;    // current node
	BTreeNode * dNode;    // delete node

	// ��Ʈ ��带 pVRoot�� ����Ű�� ����� ������ ���� ��尡 �ǰ� �Ѵ�.
	ChangeRightSubTree(pVRoot, *pRoot);

	// ���� ����� ������ ��� Ž��
	while (cNode != NULL && Getkey(cNode) != target)
	{
		pNode = cNode;

		if (target < Getkey(cNode))
			cNode = GetLeftSubTree(cNode);
		else
			cNode = GetRightSubTree(cNode);
	}

	if (cNode == NULL)    // ���� ����� �������� �ʴ´ٸ�,
		return NULL;

	dNode = cNode;    // ���� ����� dNode�� ����Ű�� �Ѵ�.

	// ù ��° ���: ������ ��尡 �ܸ� ����� ���
	if (GetLeftSubTree(dNode) == NULL && GetRightSubTree(dNode) == NULL)
	{
		if (GetLeftSubTree(pNode) == dNode)
			RemoveLeftSubTree(pNode);
		else
			RemoveRightSubTree(pNode);
	}

	// �� ��° ���: �ϳ��� �ڽ� ��带 ���� ���
	else if (GetLeftSubTree(dNode) == NULL || GetRightSubTree(dNode) == NULL)
	{
		BTreeNode * dcNode;    // delete node�� �ڽ� ���

		if (GetLeftSubTree(dNode) != NULL)
			dcNode = GetLeftSubTree(dNode);
		else
			dcNode = GetRightSubTree(dNode);

		if (GetLeftSubTree(pNode) == dNode)
			ChangeLeftSubTree(pNode, dcNode);
		else
			ChangeRightSubTree(pNode, dcNode);
	}

	// �� ��° ���: �� ���� �ڽ� ��带 ��� ���� ���
	else
	{
		BTreeNode * mNode = GetRightSubTree(dNode);    // mininum node
		BTreeNode * mpNode = dNode;    // mininum node�� �θ� ���
		int delkey;

		// ������ ��带 ��ü�� ��带 ã�´�.
		while (GetLeftSubTree(mNode) != NULL)
		{
			mpNode = mNode;
			mNode = GetLeftSubTree(mNode);
		}

		// ��ü�� ��忡 ����� ���� ������ ��忡 �����Ѵ�.
		delkey = Getkey(dNode);    // ���� �� ������ ���
		Setkey(dNode, Getkey(mNode));

		// ��ü�� ����� �θ� ���� �ڽ� ��带 �����Ѵ�.
		if (GetLeftSubTree(mpNode) == mNode)
			ChangeLeftSubTree(mpNode, GetRightSubTree(mNode));
		else
			ChangeRightSubTree(mpNode, GetRightSubTree(mNode));

		dNode = mNode;
		Setkey(dNode, delkey);    // ��� ������ ����
	}

	// ������ ��尡 ��Ʈ ����� ��쿡 ���� ó��
	if (GetRightSubTree(pVRoot) != *pRoot)
		*pRoot = GetRightSubTree(pVRoot);

	free(pVRoot);

	*pRoot = Rebalance(pRoot); 	// ��� ���� �� ���뷱��!
	return dNode;
}

//
//void ShowIntkey(int key)
//{
//	printf("%d ", key);
//}
//
//void BSTShowAll(BTreeNode * bst)
//{
//	InorderTraverse(bst, ShowIntkey);
//}










// LL ȸ��
BTreeNode * RotateLL(BTreeNode * bst)
{
	BTreeNode * pNode;
	BTreeNode * cNode;

	pNode = bst;
	cNode = GetLeftSubTree(pNode);

	ChangeLeftSubTree(pNode, GetRightSubTree(cNode));
	ChangeRightSubTree(cNode, pNode);
	return cNode;
}

// RR ȸ��
BTreeNode * RotateRR(BTreeNode * bst)
{
	BTreeNode * pNode;
	BTreeNode * cNode;

	pNode = bst;
	cNode = GetRightSubTree(pNode);

	ChangeRightSubTree(pNode, GetLeftSubTree(cNode));
	ChangeLeftSubTree(cNode, pNode);
	return cNode;
}

// RL ȸ��
BTreeNode * RotateRL(BTreeNode * bst)
{
	BTreeNode * pNode;
	BTreeNode * cNode;

	pNode = bst;
	cNode = GetRightSubTree(pNode);

	ChangeRightSubTree(pNode, RotateLL(cNode));   // �κ��� LL ȸ��
	return RotateRR(pNode);     // RR ȸ��
}

// LR ȸ��
BTreeNode * RotateLR(BTreeNode * bst)
{
	BTreeNode * pNode;
	BTreeNode * cNode;

	pNode = bst;
	cNode = GetLeftSubTree(pNode);

	ChangeLeftSubTree(pNode, RotateRR(cNode));   // �κ��� RR ȸ��
	return RotateLL(pNode);     // LL ȸ��
}

// Ʈ���� ���̸� ����Ͽ� ��ȯ
int GetHeight(BTreeNode * bst)
{
	int leftH;		// left height
	int rightH;		// right height

	if (bst == NULL)
		return 0;

	// ���� ���� Ʈ�� ���� ���
	leftH = GetHeight(GetLeftSubTree(bst));

	// ������ ���� Ʈ�� ���� ���
	rightH = GetHeight(GetRightSubTree(bst));

	// ū ���� ���̸� ��ȯ�Ѵ�.
	if (leftH > rightH)
		return leftH + 1;
	else
		return rightH + 1;
}

// �� ���� Ʈ���� ������ ���� ��ȯ
int GetHeightDiff(BTreeNode * bst)
{
	int lsh;    // left sub tree height
	int rsh;    // right sub tree height

	if (bst == NULL)
		return 0;

	lsh = GetHeight(GetLeftSubTree(bst));
	rsh = GetHeight(GetRightSubTree(bst));

	return lsh - rsh;
}

// Ʈ���� ������ ��´�.
BTreeNode * Rebalance(BTreeNode ** pRoot)
{
	int hDiff = GetHeightDiff(*pRoot);

	if (hDiff > 1)     // ���� ���� Ʈ�� �������� ���̰� 2 �̻� ũ�ٸ�
	{
		if (GetHeightDiff(GetLeftSubTree(*pRoot)) > 0)
			*pRoot = RotateLL(*pRoot);
		else
			*pRoot = RotateLR(*pRoot);
	}

	if (hDiff < -1)     // ������ ���� Ʈ�� �������� 2 �̻� ũ�ٸ�
	{
		if (GetHeightDiff(GetRightSubTree(*pRoot)) < 0)
			*pRoot = RotateRR(*pRoot);
		else
			*pRoot = RotateRL(*pRoot);
	}

	return *pRoot;
}
