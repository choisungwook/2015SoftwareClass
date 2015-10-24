
#include <stdio.h>
#include "carThread.h"
#include "Main.h"

extern HBITMAP		hbackbit;

//차 쓰레드
unsigned WINAPI car(void *arg)
{	
	carArg *Argument = ((carArg*)arg);

	for (int i = 0; i < 100; i++)
	{
		printf("[%d] x : %d y : %d\n", Argument->id, Argument->posX, Argument->posX);
		Argument->posX += 10;		
		Update();
		Sleep(100);
	}

	printf("[INFO] %d Thread done\n", Argument->id);
	return 0;
}