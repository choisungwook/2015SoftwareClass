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


	cout << "���࿡ �ʿ��� ���� ���� �غ��� ......" << endl;

	if (CopyFile(tmp.c_str(), (dstPath + tmp).c_str(), 0))
	{
		cout << "��ġ�� �Ϸ��߽��ϴ�." << endl;
	}
	else
	{
		cout << "������ �������� �����ϼ���" << endl;
	}
	

	system("pause");

	return 0;
}