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
	functionset.insert(commandPair("Check", checkExisted)); //레코드가 있는지 체크
	functionset.insert(commandPair("Update", UpdateDB)); //데이터수정
	functionset.insert(commandPair("Close", closeDB)); //연결종료
	functionset.insert(commandPair("Delete", DeleteDB)); //삭제
}

//처음 연결 요청
//DB에 차가 등록되어 있는지 확인한다
//없다면 등록하고 -1을 리턴
//있다면 1을 리턴
//@data[0] 차번호
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

//레코드 삭제
//실패하면 -1 리턴
//@data 차번호
int DeleteDB(int hsock, vector<string> &data)
{
	//삭제 성공
	if(car.remove(stoi(data[0])))
		return KEY_DELETE;
	
	return KEY_NOTFOUND;
}

//레코드 추가/수정
//@data 차번호
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

//연결을 종료하고
//파일 출력하고
//쓰레드를 종료한다.
//@data 존재하지 않음
int closeDB(int hsock, vector<string> &data)
{
	closesocket(hsock);
	//car.save();
	_endthreadex(0);

	return 0;
}