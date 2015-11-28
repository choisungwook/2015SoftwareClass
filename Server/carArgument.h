#ifndef __CAR_ARGUMENT_H__
#define __CAR_ARGUMENT_H__

#include <string>
#include <list>

class carArgument
{
public:
	int						carnumber;	//����ȣ
	int						sortOfcar;  //������
	int						totalPrice; //�ѱݾ�
	int						visitCount;
	std::string				lastVisit; //�������湮��¥	
	//��ȭ	
	std::list<std::string>	movieName; //�ڱⰡ ����ȭ ��ϵ�		
	int						totalmovieTime;	//��ȭ�� �� �ð� (�ʴ����� ����)
	carArgument(int _carnumber, int _sortOfcar, int _totalPrice, int _totalmovieTime,
		std::string _lastVisit, std::list<std::string> &movie)
	{
		carnumber = _carnumber; sortOfcar = _sortOfcar;
		totalPrice = _totalPrice; totalmovieTime = _totalmovieTime;
		lastVisit = _lastVisit; movieName = movie;
		visitCount = 0;
	}
	carArgument() {};
	carArgument(int _number) 
	{ 
		carnumber = _number;
		sortOfcar = 0;
		totalPrice = 0;
		totalmovieTime = 0;
		lastVisit = "�湮��Ͼ���";
		visitCount = 0;
	};	
	~carArgument()
	{
		movieName.clear();
	}
};


#endif