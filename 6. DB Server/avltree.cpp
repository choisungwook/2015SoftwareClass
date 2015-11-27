#include "avltree.h"
#include <stack>
using namespace std;

//균형을 잡는다
//균형을 잡는 경우는 삽입, 삭제가 일어날 경우이다
void AVLTree::reBalance(AVLNODE *n)
{
	SetBalance(n);

	if (n->balance == -2)
	{
		if (height(n->left->left) >= height(n->left->right))
			n = rotateRight(n);
		else
			n = rotateLeftThenRight(n);
	}
	else if (n->balance == 2)
	{
		if (height(n->right->right) >= height(n->right->left))
			n = rotateLeft(n);
		else
			n = rotateRightThenLeft(n);
	}

	if (n->parent != NULL)
		reBalance(n->parent);
	else
		root = n;
}

//
void AVLTree::SetBalance(AVLNODE *n)
{
	n->balance = height(n->right) - height(n->left);
}

int AVLTree::height(AVLNODE *n)
{
	if (n == NULL)
		return -1;

	int leftHeight = height(n->left);
	int rightHeight = height(n->right);

	if (leftHeight > rightHeight)
		return leftHeight + 1;

	return rightHeight + 1;

}

///////////////////////////////////
//회전
AVLNODE* AVLTree::rotateLeft(AVLNODE *a)
{
	AVLNODE *b = a->right;
	b->parent = a->parent;
	a->right = b->left;

	if (a->right != NULL)
		a->right->parent = a;

	b->left = a;
	a->parent = b;

	if (b->parent != NULL) {
		if (b->parent->right == a) {
			b->parent->right = b;
		}
		else {
			b->parent->left = b;
		}
	}

	SetBalance(a);
	SetBalance(b);
	return b;
}

AVLNODE* AVLTree::rotateRight(AVLNODE *a) {
	AVLNODE *b = a->left;
	b->parent = a->parent;
	a->left = b->right;

	if (a->left != NULL)
		a->left->parent = a;

	b->right = a;
	a->parent = b;

	if (b->parent != NULL) {
		if (b->parent->right == a) {
			b->parent->right = b;
		}
		else {
			b->parent->left = b;
		}
	}

	SetBalance(a);
	SetBalance(b);
	return b;
}

//LR회전
AVLNODE* AVLTree::rotateLeftThenRight(AVLNODE *n)
{
	n->left = rotateLeft(n->left);
	return rotateRight(n);
}

AVLNODE* AVLTree::rotateRightThenLeft(AVLNODE *n)
{
	n->right = rotateRight(n->right);
	return rotateLeft(n);
}

//insert내부의 재귀 방식은 비효율이므로 다른 함수로 대체한다.
bool AVLTree::insert(int key, carArgument data)
{
	access.lock();
	if (root == NULL)
		root = new AVLNODE(key, data, NULL);

	else
	{
		AVLNODE *parentNode = NULL;
		AVLNODE *curNode = root;

		while (true)
		{
			//값 중복일경우 
			if (curNode->key == key)
			{
				//차종류
				access.unlock();
				return false;
			}

			parentNode = curNode;

			bool goLeft = curNode->key > key;
			curNode = goLeft ? curNode->left : curNode->right;

			//현재 노드가 NULL이라면
			if (curNode == NULL)
			{
				if (goLeft)
					parentNode->left = new AVLNODE(key, data, parentNode);
				else
					parentNode->right = new AVLNODE(key, data, parentNode);

				//균형을 잡는다.
				reBalance(parentNode);
				break;
			}
		}
	}

	access.unlock();
	return true;
}

//트리가 비어있으면 삭제못함
//또는 찾는 키값이 존재 하지 않음
bool AVLTree::remove(int key) {
	bool r = false;

	if (root == NULL)
		return r;

	access.lock();

	AVLNODE *n = root,
			*parentNode = root,
			*delNode = NULL,
			*childNode = root;

	while (childNode != NULL) {
		parentNode = n;
		n = childNode;
		childNode = key >= n->key ? n->right : n->left;
		if (key == n->key)
			delNode = n;
	}

	if (delNode != NULL) {
		delNode->key = n->key;

		childNode = n->left != NULL ? n->left : n->right;

		if (root->key == key) {
			root = childNode;
		}
		else {
			if (parentNode->left == n) {
				parentNode->left = childNode;
			}
			else {
				parentNode->right = childNode;
			}
			reBalance(parentNode);
		}
		r = true;
	}
	
	access.unlock();

	return r;
}

//탐색
AVLNODE* AVLTree::search(int key)
{
	AVLNODE *searchNode = root;
	access.lock();

	if (root != NULL)
	{
		while (true)
		{
			if (searchNode == NULL) //찾는값이 없다면
			{
				searchNode = NULL;
				break;
			}

			if (searchNode->key == key) //찾는값이라면
				break;
			else if (searchNode->key > key)
				searchNode = searchNode->left;
			else
				searchNode = searchNode->right;
		}
	}

	access.unlock();
	return searchNode;
}

void AVLTree::update(AVLNODE* updateNode, int operate, void* data)
{
	switch (operate)
	{
	case 1: //차종류
		updateNode->Data.sortOfcar = std::stoi(*static_cast<std::string *>(data)); break;
	case 2: //지불금액 추가
		updateNode->Data.totalPrice += std::stoi(*static_cast<std::string *>(data)); break;
	case 3: //방문날짜 수정
		updateNode->Data.lastVisit = (*static_cast<std::string *>(data)); break;
	case 4: //영화 추가
		updateNode->Data.movieName.push_back((*static_cast<std::string *>(data))); break;
	case 5: //영화 시청 시간 추가
		updateNode->Data.totalmovieTime += std::stoi(*static_cast<std::string *>(data)); break;
	}
}

void AVLTree::save(FILE *fp)
{
	access.lock();

	if (root == NULL)
		return;

	char saveBuf[1024] = { 0, };
	stack<AVLNODE*> nodestack;
	nodestack.push(root);

	while (nodestack.empty() == false)
	{
		AVLNODE* node = nodestack.top();

		//차번호, 차종류, 총금액, 영화본시간, 마지막방문날짜, 본영화목록
		sprintf(saveBuf, "%d.%d.%d.%d.%s",
			node->Data.carnumber,
			node->Data.sortOfcar,
			node->Data.totalPrice,
			node->Data.totalmovieTime,
			node->Data.lastVisit.c_str());
		fputs(saveBuf, fp);
		//본영화 저장
		list<string>::iterator End = node->Data.movieName.end();
		for (list<string>::iterator iterPos = node->Data.movieName.begin();
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
	access.unlock();
}

//생성자
AVLTree::AVLTree()
{
	root = NULL;
}

//소멸자
AVLTree::~AVLTree()
{
	delete root;
}

AVLNODE* AVLTree::getRoot()
{
	return root;
}

//////////////////////////////////////
//데이터 얻기

int AVLTree::getvisitCount()
{
	return root->visitCount;
}