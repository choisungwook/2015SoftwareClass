#include <regex>
#include <vector>
using namespace std;

////////////////////////////////////////
//////// 정규식 토큰화
////////////////////////////////////////

void token(regex &re, string msg, vector<string> &a);

void token(regex &re, string msg, vector<string> &a)
{
	sregex_token_iterator it(msg.begin(), msg.end(), re, -1);

	sregex_token_iterator reg_end;
	for (; it != reg_end; ++it) {
		a.push_back(it->str());
	}
}
