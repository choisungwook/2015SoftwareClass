
#include <stdio.h>
#include "carThread.h"
#include "Main.h"

//차 쓰레드
unsigned WINAPI car(void *arg)
{
	
	carArg *Argument = ((carArg*)arg);

	printf("[INFO] %d Thread run\n", Argument->id);

	for (int i = 0; i < 5; i++)
	{
		printf("[%d] x : %d y : %d\n", Argument->id, Argument->posX, Argument->posX);
		Argument->posX += 10;
		InvalidateRgn(hWnd,NULL,TRUE);
		//InvalidateRect(hWnd, NULL, TRUE);
		Sleep(100);
	}

	printf("[INFO] %d Thread done\n", Argument->id);
	return 0;
}