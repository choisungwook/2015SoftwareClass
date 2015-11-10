#include <Windows.h>
#include "utility.h"

extern HANDLE		M_corner1;
extern HANDLE		M_corner1Wait;

//왼쪽 코너 도는 것을 지시하는 워커
unsigned WINAPI cornerworker1(void *arg)
{
	while (true)
	{
		//코너를 돌고있는 차량이 전부 돌았다는 신호를 준다.
		watiAndcheckExited(M_corner1);
		//코너를 돌려는 다음 차량에게 돌아도 된다는 신호를 준다.
		ReleaseMutex(M_corner1Wait);
	}
	

	
}