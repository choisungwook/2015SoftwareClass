#ifndef __CARCOLIISION_H__
#define __CARCOLIISION_H__


bool detectCollision(int x, int y);
bool detectCollision(int carID, int x, int y);
//������ �浹üũ
bool detectCollisionOfRight(int carID, int x, int y);


void setCollision(int *x, int *y, int a, int b);

#endif