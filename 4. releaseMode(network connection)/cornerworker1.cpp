#include <Windows.h>
#include "utility.h"

extern HANDLE		M_corner1;
extern HANDLE		M_corner1Wait;

//���� �ڳ� ���� ���� �����ϴ� ��Ŀ
unsigned WINAPI cornerworker1(void *arg)
{
	while (true)
	{
		//�ڳʸ� �����ִ� ������ ���� ���Ҵٴ� ��ȣ�� �ش�.
		watiAndcheckExited(M_corner1);
		//�ڳʸ� ������ ���� �������� ���Ƶ� �ȴٴ� ��ȣ�� �ش�.
		ReleaseMutex(M_corner1Wait);
	}
	

	
}