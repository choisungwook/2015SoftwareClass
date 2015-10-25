#include "Collision.h"
#include "Main.h"
#include <stdio.h>



void setCollision(carArg *arg,int a, int b)
{
	(arg->posX) += a;
	(arg->posY) += b;

	arg->rect.left += a;
	arg->rect.top += b;
	arg->rect.right += a;
	arg->rect.bottom += b;
}

//test
bool testCollision(int carID, RECT *src)
{
	for (int i = 0; i < numOfCar; i++)
	{
		if (i == carID)
			continue;

		if ((src->left < arg[i].rect.right) &&
			(src->top < arg[i].rect.bottom) &&
			(src->right > arg[i].rect.left) &&
			(src->bottom > arg[i].rect.top))
		{
			if (carID < i)
				return false;
			printf("%d %d충돌 발생\n", carID, i);
			return true;
		}
	}

	return false;
}