#ifndef __AVLTREEL_H__
#define __AVLTREEL_H__

#include <list>
#include <string>
#include <stack>
#include <stdio.h>

class carArgument
{
public:
	int						carnumber;	//차번호
	int						sortOfcar;  //차종류
	int						totalPrice; //총금액
	std::string				lastVisit; //마지막방문날짜	
	//영화	
	std::list<std::string>	movieName; //자기가 본영화 목록들		
	int						totalmovieTime;	//영화를 총 시간 (초단위로 저장)
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
	//AVL 트리 기능
	NODE* Insert(NODE** pRoot, int key, carArgument &Data);
	void  Remove(NODE **pRoot, int target);
	NODE* Search(NODE* Root, int target);
	NODE* FirstRevise(NODE* Root, int target, int operate, void* data);
	void  AfterRevise(NODE* Root, int operate, void* data);
	//회전
	NODE* RotateLL(NODE* Root);
	NODE* RotateRR(NODE* Root);
	NODE* RotateRL(NODE* Root);
	NODE* RotateLR(NODE* Root);
	//균형
	NODE* Rebalance(NODE** pRoot);
	int GetHeightDiffernce(NODE* Root); //왼쪽 오른쪽 높이의 차이 구하는 함수
	int GetHeight(NODE* Root); //높이를 구하는 함수
	//preOrder 재귀 x
	void save(FILE *fp);
};


#endif