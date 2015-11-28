#include <regex>
#include <vector>
#include <Windows.h>
#include <atlstr.h>
using namespace std;

////////////////////////////////////////
//////// 정규식 토큰화
////////////////////////////////////////

void token(regex &re, string msg, vector<string> &a);
void DebugFunc(TCHAR *pszErr, ...);

void token(regex &re, string msg, vector<string> &a)
{
	sregex_token_iterator it(msg.begin(), msg.end(), re, -1);

	sregex_token_iterator reg_end;
	for (; it != reg_end; ++it) {
		a.push_back(it->str());
	}
}

void DebugFunc(TCHAR *pszErr, ...)
{
	
	static CString strErr;
	va_list ap;
	va_start(ap, pszErr);
	strErr.FormatV(pszErr, ap);
	va_end(ap);
	
	OutputDebugString(("(Program Name) : ") + strErr);
}

