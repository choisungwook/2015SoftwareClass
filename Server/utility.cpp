#include <regex>
#include <vector>
#include <Windows.h>
#include <atlstr.h>
using namespace std;

////////////////////////////////////////
//////// 정규식 토큰화
////////////////////////////////////////

void token(regex &re, string msg, vector<string> &a);

/*
@[0] match
@[1] in
@[2] out
*/
void token(regex &re, string msg, vector<string> &a)
{
	sregex_token_iterator it(msg.begin(), msg.end(), re, -1);

	sregex_token_iterator reg_end;
	for (; it != reg_end; ++it) {
		a.push_back(it->str());
	}
}


