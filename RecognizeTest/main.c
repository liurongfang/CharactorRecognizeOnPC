
//����һЩͷ�ļ�
#include "stdio.h"
#include "stdlib.h"		//system()����
#include "mybmp.h"		//ͼ������ʾ���洢�����洢�ռ�ķ���
#include "img.h"


//������
int main()
{
	//������������
	char *filename = "C:\\Users\\Mirgo\\Desktop\\01.bmp";		//Ҫ��ȡ��ͼ���ļ�·��
	char *outfname = "C:\\Users\\Mirgo\\Desktop\\a.txt";		//Ҫ������ı�����·��
	int height = 120;
	int width = 449;
	int i = 0;

	//����ά���鶯̬����ռ�
	UCHAR **img = allloc_mem2d(height,width);

	//��ʼ
	readBmp(img,height,width,filename);		//��ȡͼƬ��img
	//BinaryImg(img, img, height,width, 100);			//��ֵ��ͼ��
	//DetectRect(img, img, height, width);
	saveImg(img,height,width,outfname);		//����img���ı�
	displayImg(img,height,width);			//��ʾimg

	//��ͣ���ȴ�����
	system("pause");

	//�ͷſռ�
	delete_mem2d(img, height, width);

	return 0;
}

