#include "avltree.h"
#include <iostream>
#include <stdio.h>
using namespace std;

////////////////////////////////////////
//회전
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

	pNode->right = RotateLL(cNode); //LL회전	
	return RotateRR(pNode);     // RR 회전
}

// LR 회전
NODE* AVLTREE::RotateLR(NODE* Root)
{
	NODE* pNode;
	NODE* cNode;

	pNode = Root;
	cNode = pNode->left;

	pNode->left = RotateRR(cNode);	// RR회전
	return RotateLL(pNode);     // LL 회전
}

////////////////////////////////////////
//회전 끝
///////////////////////////////////////

////////////////////////////////////////
//균형 
///////////////////////////////////////
int AVLTREE::GetHeightDiffernce(NODE* Root)
{
	int lsh;    // 왼쪽 높이 저장할변수
	int rsh;    // 오른쪽 높이 저장할변수

	if (Root == NULL)
		return 0;

	lsh = GetHeight(Root->left);
	rsh = GetHeight(Root->right);

	return lsh - rsh;
}

int AVLTREE::GetHeight(NODE* Root)
{
	int leftH;		// 왼쪽높이
	int rightH;		// 오른쪽높이

	if (Root == NULL)
		return 0;

	// 왼쪽 서브 트리 높이 계산
	leftH = GetHeight(Root->left);

	// 오른쪽 서브 트리 높이 계산
	rightH = GetHeight(Root->right);

	// 큰 값의 높이를 반환한다.
	if (leftH > rightH)
		return leftH + 1;
	else
		return rightH + 1;
}

NODE* AVLTREE::Rebalance(NODE** pRoot)
{
	int hDiff = GetHeightDiffernce(*pRoot);

	if (hDiff > 1)     // 왼쪽 서브 트리 방향으로 높이가 2 이상 크다면
	{
		if (GetHeightDiffernce((*pRoot)->left) > 0)
			*pRoot = RotateLL(*pRoot);
		else
			*pRoot = RotateLR(*pRoot);
	}

	if (hDiff < -1)     // 오른쪽 서브 트리 방향으로 2 이상 크다면
	{
		if (GetHeightDiffernce((*pRoot)->right) < 0)
			*pRoot = RotateRR(*pRoot);
		else
			*pRoot = RotateRL(*pRoot);
	}

	return *pRoot;
}
////////////////////////////////////////
//균형 끝
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
		return NULL; // 키의 중복을 허용하지 않는다.
	}
	return *pRoot;
}

void AVLTREE::Remove(NODE **pRoot, int target)
{
	// 삭제 대상이 루트 노드인 경우를 별도로 고려해야 한다.
	NODE* pVRoot = new NODE();    // 가상 루트 노드;

	NODE* pNode = pVRoot;    // parent node
	NODE* cNode = *pRoot;    // current node
	NODE* dNode;    // delete node

	// 루트 노드를 pVRoot가 가리키는 노드의 오른쪽 서브 노드가 되게 한다.
	pVRoot->right = *pRoot;

	// 삭제 대상을 저장한 노드 탐색
	while (cNode != NULL && cNode->key != target)
	{
		pNode = cNode;

		if (target < cNode->key)
			cNode = cNode->left;
		else
			cNode = cNode->right;
	}

	if (cNode == NULL)    // 삭제 대상이 존재하지 않는다면,
		return;

	dNode = cNode;    // 삭제 대상을 dNode가 가리키게 한다.

	// 첫 번째 경우: 삭제할 노드가 단말 노드인 경우
	if (dNode->left == NULL && dNode->right == NULL)
	{
		if (pNode->left == dNode)
			pNode->left = NULL;
		else
			pNode->right = NULL;
	}

	// 두 번째 경우: 하나의 자식 노드를 갖는 경우
	else if (dNode->left == NULL || dNode->right == NULL)
	{
		NODE* dcNode;    // delete node의 자식 노드

		if (dNode->left != NULL)
			dcNode = dNode->left;
		else
			dcNode = dNode->right;

		if (pNode->left == dNode)
			pNode->left = dcNode;
		else
			pNode->right = dcNode;
	}

	// 세 번째 경우: 두 개의 자식 노드를 모두 갖는 경우
	else
	{
		NODE* mNode = dNode->right;    // mininum node
		NODE* mpNode = dNode;    // mininum node의 부모 노드
		int delkey;

		// 삭제할 노드를 대체할 노드를 찾는다.
		while (mNode->left != NULL)
		{
			mpNode = mNode;
			mNode = mNode->left;
		}

		// 대체할 노드에 저장된 값을 삭제할 노드에 대입한다.
		delkey = dNode->key;    // 대입 전 데이터 백업
		dNode->key = mNode->key;

		// 대체할 노드의 부모 노드와 자식 노드를 연결한다.
		if (mpNode->left == mNode)
			mpNode->left = mNode->right;
		else
			mpNode->right = mNode->right;

		dNode = mNode;
		dNode->key = delkey;
	}

	// 삭제된 노드가 루트 노드인 경우에 대한 처리
	if (pVRoot->right != *pRoot)
		*pRoot = pVRoot->right;

	delete pVRoot;
	delete dNode;

	*pRoot = Rebalance(pRoot); 	// 노드 제거 후 리밸런싱!	
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
			case 1: //차종류 바뀌면
				cNode->carData.sortOfcar = std::stoi(*static_cast<std::string *>(data)); break;
			case 2: //지불금액을 추가하고 싶은 경우				
				cNode->carData.totalPrice += std::stoi(*static_cast<std::string *>(data));	break;
			case 3: //방문날짜를 바꾸고 싶은 경우
				cNode->carData.lastVisit = *static_cast<std::string *>(data); break;
			case 4: //자신이 본 영화를 추가하고 싶을떄
				cNode->carData.movieName.push_back(*static_cast<std::string *>(data)); break;
			case 5: //영화본 시간 추가
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
	case 1: //차종류 바뀌면
		Root->carData.sortOfcar = std::stoi(*static_cast<std::string *>(data)); break;
	case 2: //지불금액을 추가하고 싶은 경우				
		Root->carData.totalPrice += std::stoi(*static_cast<std::string *>(data));	break;
	case 3: //방문날짜를 바꾸고 싶은 경우
		Root->carData.lastVisit = *static_cast<std::string *>(data); break;
	case 4: //자신이 본 영화를 추가하고 싶을떄
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

//저장기능
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

		//차번호, 차종류, 총금액, 영화본시간, 마지막방문날짜, 본영화목록
		sprintf(saveBuf, "%d.%d.%d.%d.%s",
			node->carData.carnumber,
			node->carData.sortOfcar,
			node->carData.totalPrice,
			node->carData.totalmovieTime,
			node->carData.lastVisit.c_str());
		fputs(saveBuf, fp);
		//본영화 저장
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