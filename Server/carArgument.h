#ifndef __CAR_ARGUMENT_H__
#define __CAR_ARGUMENT_H__

#include <string>
#include <list>

class carArgument
{
public:
	int						carnumber;	//차번호
	int						sortOfcar;  //차종류
	int						totalPrice; //총금액
	int						visitCount;
	std::string				lastVisit; //마지막방문날짜	
	//영화	
	std::list<std::string>	movieName; //자기가 본영화 목록들		
	int						totalmovieTime;	//영화를 총 시간 (초단위로 저장)
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
		lastVisit = "방문기록없음";
		visitCount = 0;
	};	
	~carArgument()
	{
		movieName.clear();
	}
};


#endif