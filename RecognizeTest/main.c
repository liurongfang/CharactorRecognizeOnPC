
//����һЩͷ�ļ�
#include "stdio.h"
#include "stdlib.h"		//system()����
#include "mybmp.h"		//ͼ������ʾ���洢�����洢�ռ�ķ���
#include "img.h"

//ͼ��Ŀ��
int height = 120;	//ע�⣡��Ҫд����
int width = 449;

//��ȡ��Ŀ��
extern after_h;
extern after_w;

//��׼����͸�
int std_h = 64;
int std_w = 64;

//������
int main()
{
	//������������
	char *filename = "C:\\Users\\Mirgo\\Desktop\\01.bmp";		//Ҫ��ȡ��ͼ���ļ�·��
	char *outfname = "C:\\Users\\Mirgo\\Desktop\\a.txt";		//Ҫ������ı�����·��
	char *outfname2 = "C:\\Users\\Mirgo\\Desktop\\a1.txt";
	int i = 0;

	//����ά���鶯̬����ռ�
	UCHAR **img = allloc_mem2d(height,width);
	UCHAR **dst = allloc_mem2d(std_h,std_w);

	//��ʼ
	readBmp(img, height, width, filename);		//��ȡͼƬ��img
	BinaryImg(img, img, height, width, 100);			//��ֵ��ͼ��
	DetectRect(img, img, height, width);		//�����������ھ�������,�õ���after_h��after_w
	displayImg(img, after_h, after_w);			//��ʾimg
	system("pause");
	//SlopeAdjust(img, img, height, width);		//��б�Ƚ���
	//StdImage(dst, img, after_h, after_w, std_h, std_w);

	saveImg(img, after_h, after_w, outfname);		//����img���ı�
	saveImg(dst, std_h, std_w, outfname2);		//����img���ı�

	displayImg(img, after_h, after_w);			//��ʾimg

	//��ͣ���ȴ�����
	system("pause");
	system("cls");

	displayImg(dst, std_h, std_w);			//��ʾimg

	system("pause");

	//�ͷſռ�
	delete_mem2d(img, height, width);
	delete_mem2d(dst, std_h, std_w);

	return 0;
}

