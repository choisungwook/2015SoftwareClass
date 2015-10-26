#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

int main()
{
	wstring tmp = L"tmp.dll";
	wstring dllPath = L"msvcr120d.dll";
	wstring dstPath = L"C:\\Windows\\System32";


	cout << "실행에 필요한 파일 복사 준비중 ......" << endl;

	if (CopyFile(tmp.c_str(), (dstPath + tmp).c_str(), 0))
	{
		cout << "설치를 완료했습니다." << endl;
	}
	else
	{
		cout << "관리자 권한으로 실행하세요" << endl;
	}
	

	system("pause");

	return 0;
}