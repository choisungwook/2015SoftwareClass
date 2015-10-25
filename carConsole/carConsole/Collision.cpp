#include "Collision.h"
#include "Main.h"

//�浹üũ �迭
//extern int			collisionBuf[WIDTH][HEIGHT];

//�浹üũ����
bool detectCollision(int x, int y)
{
	if (collisionBuf[x][y])
		return true;

	return false;
}

bool detectCollision(int carID, int x, int y)
{
	for (int i = 0; i < numOfCar; i++)
	{
		if (i == carID)
			continue;

		if (detectCollisionOfRight(i, x, y))
			return false;
	}
	return true;
}

//������ �浹üũ
bool detectCollisionOfRight(int carID, int x, int y)
{
	if ((x + 100 <= arg[carID].posX) &&
		(x > arg[carID].posX))
		return true;

	return false;
}

//�浹üũ����
void setCollision(int *x, int *y, int a, int b)
{
	for (int i = *x; i < 100; i++)
	{

	}
	collisionBuf[*x][*y] = 0;
	(*x) += a;
	(*y) += b;
	collisionBuf[*x][*y] = 1;
}