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

	RECT predict;
	predict.left = src->left - 10;
	predict.right = src->right - 10;
	predict.top = src->top;
	predict.bottom = src->bottom;

	watiAndcheckExited(M_accessArg);
	list < carArgument >::iterator End = L_carArg.end();

	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		if (iterPos->id == carID)
			continue;

		RECT tmp;
		if (IntersectRect(&tmp, src, &iterPos->rect))
		{
			if (predict.left - iterPos->rect.left > 0)
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

	RECT predict;
	predict.left = src->left;
	predict.right = src->right;
	predict.top = src->top + 10;
	predict.bottom = src->bottom + 10;

	watiAndcheckExited(M_accessArg);
	list < carArgument >::iterator End = L_carArg.end();

	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		if (iterPos->id == carID)
			continue;

		RECT tmp;
		if (IntersectRect(&tmp, &predict, &iterPos->rect))
		{
			if (predict.bottom - iterPos->rect.bottom > 0)
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

	RECT predict;
	predict.left = src->left + 10;
	predict.right = src->right + 10;
	predict.top = src->top;
	predict.bottom = src->bottom;

	watiAndcheckExited(M_accessArg);
	list < carArgument >::iterator End = L_carArg.end();

	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		if (iterPos->id == carID)
			continue;

		RECT tmp;
		if (IntersectRect(&tmp, &predict, &iterPos->rect))
		{
			if (predict.right - iterPos->rect.right < 0)				
				r = true;
			if (predict.top - iterPos->rect.top < 0)
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

	RECT predict;
	predict.left = src->left;
	predict.right = src->right;
	predict.top = src->top + 10;
	predict.bottom = src->bottom + 10;

	watiAndcheckExited(M_accessArg);
	list < carArgument >::iterator End = L_carArg.end();

	for (list<carArgument>::iterator iterPos = L_carArg.begin(); iterPos != End; iterPos++)
	{
		if (iterPos->id == carID)
			continue;

		RECT tmp;
		if (IntersectRect(&tmp, &predict, &iterPos->rect))
		{
			if (predict.bottom - iterPos->rect.bottom < 0)
				r = true;
			break;
		}
	}

	ReleaseMutex(M_accessArg);

	return r;
}