#include "avltree.h"
#include <iostream>
#include <stdio.h>
using namespace std;

////////////////////////////////////////
//ȸ��
///////////////////////////////////////
NODE* AVLTREE::RotateLL(NODE* Root)
{
	NODE *pNode;
	NODE *cNode;

	pNode = Root;
	cNode = pNode->left;

	pNode->left = cNode;
	cNode->right = pNode;

	return cNode;
}

NODE* AVLTREE::RotateRR(NODE* Root)
{
	NODE* pNode;
	NODE* cNode;

	pNode = Root;
	cNode = pNode->right;

	pNode->right = cNode->left;
	cNode->left = pNode;

	return cNode;
}

NODE* AVLTREE::RotateRL(NODE* Root)
{
	NODE *pNode;
	NODE *cNode;

	pNode = Root;
	cNode = pNode->right;

	pNode->right = RotateLL(cNode); //LLȸ��	
	return RotateRR(pNode);     // RR ȸ��
}

// LR ȸ��
NODE* AVLTREE::RotateLR(NODE* Root)
{
	NODE* pNode;
	NODE* cNode;

	pNode = Root;
	cNode = pNode->left;

	pNode->left = RotateRR(cNode);	// RRȸ��
	return RotateLL(pNode);     // LL ȸ��
}

////////////////////////////////////////
//ȸ�� ��
///////////////////////////////////////

////////////////////////////////////////
//���� 
///////////////////////////////////////
int AVLTREE::GetHeightDiffernce(NODE* Root)
{
	int lsh;    // ���� ���� �����Һ���
	int rsh;    // ������ ���� �����Һ���

	if (Root == NULL)
		return 0;

	lsh = GetHeight(Root->left);
	rsh = GetHeight(Root->right);

	return lsh - rsh;
}

int AVLTREE::GetHeight(NODE* Root)
{
	int leftH;		// ���ʳ���
	int rightH;		// �����ʳ���

	if (Root == NULL)
		return 0;

	// ���� ���� Ʈ�� ���� ���
	leftH = GetHeight(Root->left);

	// ������ ���� Ʈ�� ���� ���
	rightH = GetHeight(Root->right);

	// ū ���� ���̸� ��ȯ�Ѵ�.
	if (leftH > rightH)
		return leftH + 1;
	else
		return rightH + 1;
}

NODE* AVLTREE::Rebalance(NODE** pRoot)
{
	int hDiff = GetHeightDiffernce(*pRoot);

	if (hDiff > 1)     // ���� ���� Ʈ�� �������� ���̰� 2 �̻� ũ�ٸ�
	{
		if (GetHeightDiffernce((*pRoot)->left) > 0)
			*pRoot = RotateLL(*pRoot);
		else
			*pRoot = RotateLR(*pRoot);
	}

	if (hDiff < -1)     // ������ ���� Ʈ�� �������� 2 �̻� ũ�ٸ�
	{
		if (GetHeightDiffernce((*pRoot)->right) < 0)
			*pRoot = RotateRR(*pRoot);
		else
			*pRoot = RotateRL(*pRoot);
	}

	return *pRoot;
}
////////////////////////////////////////
//���� ��
///////////////////////////////////////

NODE* AVLTREE::Insert(NODE** pRoot, int key, carArgument &Data)
{
	if ((*pRoot) == NULL)
	{
		*pRoot = new NODE();
		(*pRoot)->key = key;
		(*pRoot)->carData = Data;
	}
	else if (key < (*pRoot)->key)
	{
		Insert(&((*pRoot)->left), key, Data);
		*pRoot = Rebalance(pRoot);
	}
	else if (key >(*pRoot)->key)
	{
		Insert(&((*pRoot)->right), key, Data);
		*pRoot = Rebalance(pRoot);
	}
	else
	{
		return NULL; // Ű�� �ߺ��� ������� �ʴ´�.
	}
	return *pRoot;
}

void AVLTREE::Remove(NODE **pRoot, int target)
{
	// ���� ����� ��Ʈ ����� ��츦 ������ ����ؾ� �Ѵ�.
	NODE* pVRoot = new NODE();    // ���� ��Ʈ ���;

	NODE* pNode = pVRoot;    // parent node
	NODE* cNode = *pRoot;    // current node
	NODE* dNode;    // delete node

	// ��Ʈ ��带 pVRoot�� ����Ű�� ����� ������ ���� ��尡 �ǰ� �Ѵ�.
	pVRoot->right = *pRoot;

	// ���� ����� ������ ��� Ž��
	while (cNode != NULL && cNode->key != target)
	{
		pNode = cNode;

		if (target < cNode->key)
			cNode = cNode->left;
		else
			cNode = cNode->right;
	}

	if (cNode == NULL)    // ���� ����� �������� �ʴ´ٸ�,
		return;

	dNode = cNode;    // ���� ����� dNode�� ����Ű�� �Ѵ�.

	// ù ��° ���: ������ ��尡 �ܸ� ����� ���
	if (dNode->left == NULL && dNode->right == NULL)
	{
		if (pNode->left == dNode)
			pNode->left = NULL;
		else
			pNode->right = NULL;
	}

	// �� ��° ���: �ϳ��� �ڽ� ��带 ���� ���
	else if (dNode->left == NULL || dNode->right == NULL)
	{
		NODE* dcNode;    // delete node�� �ڽ� ���

		if (dNode->left != NULL)
			dcNode = dNode->left;
		else
			dcNode = dNode->right;

		if (pNode->left == dNode)
			pNode->left = dcNode;
		else
			pNode->right = dcNode;
	}

	// �� ��° ���: �� ���� �ڽ� ��带 ��� ���� ���
	else
	{
		NODE* mNode = dNode->right;    // mininum node
		NODE* mpNode = dNode;    // mininum node�� �θ� ���
		int delkey;

		// ������ ��带 ��ü�� ��带 ã�´�.
		while (mNode->left != NULL)
		{
			mpNode = mNode;
			mNode = mNode->left;
		}

		// ��ü�� ��忡 ����� ���� ������ ��忡 �����Ѵ�.
		delkey = dNode->key;    // ���� �� ������ ���
		dNode->key = mNode->key;

		// ��ü�� ����� �θ� ���� �ڽ� ��带 �����Ѵ�.
		if (mpNode->left == mNode)
			mpNode->left = mNode->right;
		else
			mpNode->right = mNode->right;

		dNode = mNode;
		dNode->key = delkey;
	}

	// ������ ��尡 ��Ʈ ����� ��쿡 ���� ó��
	if (pVRoot->right != *pRoot)
		*pRoot = pVRoot->right;

	delete pVRoot;
	delete dNode;

	*pRoot = Rebalance(pRoot); 	// ��� ���� �� ���뷱��!	
}

NODE* AVLTREE::Search(NODE* Root, int target)
{
	NODE* cNode = Root;    // current node
	int cd;    // current key

	while (cNode != NULL)
	{
		cd = cNode->key;

		if (target == cd)
			return cNode;
		else if (target < cd)
			cNode = cNode->left;
		else
			cNode = cNode->right;
	}

	return NULL;
}


NODE* AVLTREE::FirstRevise(NODE* Root, int target, int operate, void* data)
{
	NODE* cNode = Root;    // current node
	int cd;    // current key

	while (cNode != NULL)
	{
		cd = cNode->key;

		if (target == cd)
		{
			switch (operate)
			{
			case 1: //������ �ٲ��
				cNode->carData.sortOfcar = std::stoi(*static_cast<std::string *>(data)); break;
			case 2: //���ұݾ��� �߰��ϰ� ���� ���				
				cNode->carData.totalPrice += std::stoi(*static_cast<std::string *>(data));	break;
			case 3: //�湮��¥�� �ٲٰ� ���� ���
				cNode->carData.lastVisit = *static_cast<std::string *>(data); break;
			case 4: //�ڽ��� �� ��ȭ�� �߰��ϰ� ������
				cNode->carData.movieName.push_back(*static_cast<std::string *>(data)); break;
			case 5: //��ȭ�� �ð� �߰�
				cNode->carData.totalmovieTime += std::stoi(*static_cast<std::string *>(data));	break;
			}
			return cNode;
		}

		else if (target < cd)
			cNode = cNode->left;
		else
			cNode = cNode->right;
	}

	return NULL;
}

void AVLTREE::AfterRevise(NODE* Root, int operate, void* data)
{
	switch (operate)
	{
	case 1: //������ �ٲ��
		Root->carData.sortOfcar = std::stoi(*static_cast<std::string *>(data)); break;
	case 2: //���ұݾ��� �߰��ϰ� ���� ���				
		Root->carData.totalPrice += std::stoi(*static_cast<std::string *>(data));	break;
	case 3: //�湮��¥�� �ٲٰ� ���� ���
		Root->carData.lastVisit = *static_cast<std::string *>(data); break;
	case 4: //�ڽ��� �� ��ȭ�� �߰��ϰ� ������
		Root->carData.movieName.push_back(*static_cast<std::string *>(data)); break;
	case 5:
		Root->carData.totalmovieTime += std::stoi(*static_cast<std::string *>(data));	break;
	}
}
//utility
NODE** AVLTREE::getRootAddress()
{
	return &root;
}

NODE* AVLTREE::getRoot()
{
	return root;
}

carArgument& AVLTREE::getData()
{
	return root->carData;
}

//������
void AVLTREE::save(FILE* fp)
{
	if (root == NULL)
		return;

	char saveBuf[1024] = { 0, };
	stack<NODE*> nodestack;
	nodestack.push(root);

	while (nodestack.empty() == false)
	{
		NODE* node = nodestack.top();

		//����ȣ, ������, �ѱݾ�, ��ȭ���ð�, �������湮��¥, ����ȭ���
		sprintf(saveBuf, "%d.%d.%d.%d.%s",
			node->carData.carnumber,
			node->carData.sortOfcar,
			node->carData.totalPrice,
			node->carData.totalmovieTime,
			node->carData.lastVisit.c_str());
		fputs(saveBuf, fp);
		//����ȭ ����
		list<string>::iterator End = node->carData.movieName.end();
		for (list<string>::iterator iterPos = node->carData.movieName.begin();
			iterPos != End; iterPos++)
		{
			sprintf(saveBuf, ".%s", iterPos->c_str());
			fputs(saveBuf, fp);
		}
		sprintf(saveBuf, "\n");
		fputs(saveBuf, fp);
		nodestack.pop();

		if (node->right)
			nodestack.push(node->right);
		if (node->left)
			nodestack.push(node->left);
	}
}