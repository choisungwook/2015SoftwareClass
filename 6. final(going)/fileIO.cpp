#include <stdio.h>
#include "avltree.h"
#include "handle.h"

extern	BTreeNode		*avlRoot;
extern  HANDLE			M_accessTree;
extern	HWND			hWndMain;

int No;

void preorderOutput(BTreeNode *root,FILE *fp)
{
	if (root)
	{
		fwrite(&root->carData.id, 1, sizeof(root->carData.id), fp);
		//������
		fwrite(&root->carData.direction, 1, sizeof(root->carData.direction), fp);
		//������
		fwrite(&root->carData.sort, 1, sizeof(root->carData.sort), fp);
		//��ġ
		fwrite(&root->carData.posX, 1, sizeof(root->carData.posX), fp);
		fwrite(&root->carData.posY, 1, sizeof(root->carData.posY), fp);
		//�浹üũ��
		fwrite(&root->carData.rect.left, 1, sizeof(root->carData.rect.left), fp);
		fwrite(&root->carData.rect.top, 1, sizeof(root->carData.rect.top), fp);
		fwrite(&root->carData.rect.right, 1, sizeof(root->carData.rect.right), fp);
		fwrite(&root->carData.rect.bottom, 1, sizeof(root->carData.rect.bottom), fp);
		//��ȭ,����
		fwrite(&root->carData.movieID, 1, sizeof(root->carData.movieID), fp);
		fwrite(&root->carData.moviePrice, 1, sizeof(root->carData.moviePrice), fp);

		preorderOutput(root->left, fp);				
		preorderOutput(root->right, fp);
	}
}

void fileinput()
{

}

void fileoutput()
{
	char file[20] = "save";
	char tail[15] = { 0, };	

	sprintf(tail, "%d.dat", No++);
	strcat(file, tail);

	FILE *fp = fopen(file, "wb");

	WaitForSingleObject(M_accessTree, INFINITE);
	preorderOutput(avlRoot, fp);
	ReleaseMutex(M_accessTree);

	MessageBox(hWndMain, "�����Է� ����", "�����˸�", MB_OK);
	fclose(fp);
}