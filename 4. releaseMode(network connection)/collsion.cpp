#include <Windows.h>
#include "utility.h"
#include "carThread.h"
#include <list>
using namespace std;

extern list<carArgument>		L_carArg;
extern HANDLE					M_accessArg;

bool PriorityLeftCollision(int carID, RECT *src);
bool PriorityTopCollision(int carID, RECT *src);
bool PriorityRightCollision(int carID, RECT *src);
bool PriorityDownCollision(int carID, RECT *src);

bool PriorityLeftCollision(int carID, RECT *src)
{
	bool r = false;

	watiAndcheckExited(M_accessArg);
	list < carArgument >::iterator End = L_carArg.end();

	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		if (iterPos->id == carID)
			continue;

		RECT tmp;
		if (IntersectRect(&tmp, src, &iterPos->rect))
		{
			if (src->left > iterPos->rect.left)
			{
				char printbuf[100];
				sprintf(printbuf,"%d와 %d 차량 충돌 발생 %d<-->%d",
					carID, iterPos->id,
					src->right, iterPos->rect.left);
				OutputDebugString(printbuf);
				r = true;
			}
			break;
		}
	}

	ReleaseMutex(M_accessArg);

	return r;
}

bool PriorityTopCollision(int carID, RECT *src)
{
	bool r = false;

	watiAndcheckExited(M_accessArg);
	list < carArgument >::iterator End = L_carArg.end();

	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		if (iterPos->id == carID)
			continue;

		RECT tmp;
		if (IntersectRect(&tmp, src, &iterPos->rect))
		{
			if (src->bottom > iterPos->rect.bottom)
				r = true;
			break;
		}
	}

	ReleaseMutex(M_accessArg);

	return r;
}

bool PriorityRightCollision(int carID, RECT *src)
{
	bool r = false;

	watiAndcheckExited(M_accessArg);
	list < carArgument >::iterator End = L_carArg.end();

	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		if (iterPos->id == carID)
			continue;

		RECT tmp;
		if (IntersectRect(&tmp, src, &iterPos->rect))
		{
			if (src->right < iterPos->rect.right)
				r = true;
			break;
		}
	}

	ReleaseMutex(M_accessArg);

	return r;
}

//사용할일이없지만 ....
bool PriorityDownCollision(int carID, RECT *src)
{
	bool r = false;

	watiAndcheckExited(M_accessArg);
	list < carArgument >::iterator End = L_carArg.end();

	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		if (iterPos->id == carID)
			continue;

		RECT tmp;
		if (IntersectRect(&tmp, src, &iterPos->rect))
		{
			if (src->top < iterPos->rect.top)
				r = true;
			break;
		}
	}

	ReleaseMutex(M_accessArg);

	return r;
}