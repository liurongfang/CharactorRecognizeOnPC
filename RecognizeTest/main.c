
//����һЩͷ�ļ�
#include "stdio.h"
#include "stdlib.h"		//system()����
#include "mybmp.h"		//ͼ������ʾ���洢�����洢�ռ�ķ���
#include "img.h"

//ͼ��Ŀ��
int height = 120;	//ע�⣡��Ҫд����
int width = 449;

void ImageHandle(UCHAR **tz, UCHAR **img, int srcHeight, int srcWidth, int num);

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
	UCHAR **tz = allloc_mem2d(8,13);

	//��ʼ
	readBmp(img, height, width, filename);		//��ȡͼƬ��img
	//BinaryImg(img, img, height, width, 100);	//��ֵ��ͼ��

	//displayImg(img, height, width);			//��ʾimg

	//saveImg(img, height, width, outfname);		//����ͼ��

	//��ͣ���ȴ�����
	//system("pause");

	ImageHandle(tz, img, height, width, 8);
	printf("�ֱ�8�����غ͡������غ͡�ˮƽ��������ֱ����\n");
	displayImg(tz, 8, 13);

	printf("ImageHandle OK!\n");
	system("pause");

	//�ͷſռ�
	delete_mem2d(img, height, width);
	delete_mem2d(tz, 8, 13);

	return 0;
}

//���������ͼ�������ز���
void ImageHandle(UCHAR **tz, UCHAR **img, int srcHeight, int srcWidth, int num)
{
	UCHAR **img1 = allloc_mem2d(srcHeight, srcWidth);
	UCHAR **alignImg = allloc_mem2d(STD_H, num*STD_W);
	DRect rect;
	RectLink *rlink = NULL;
	int h = 0, w = 0;

	//��ֵ��
	//displayImg(img, srcHeight, srcWidth);
	BinaryImg(img, img, srcHeight, srcWidth, 100);
	//displayImg(img, srcHeight, srcWidth);
	
	//ȥ��
	//��б����
	//SlopeAdjust(img, img, srcHeight, srcWidth);
	//�����������ڵľ��Σ�dst�ں����ڲ�������ռ�
	rect = DetectRect(img1, img, srcHeight, srcWidth);
	h = rect.Y2 - rect.Y1 + 1;
	w = rect.X2 - rect.X1 + 1;

	saveImg(img1, h, w, "C:\\Users\\Mirgo\\Desktop\\a.txt");		//����ͼ��
	//displayImg(img1, rect.Y2 - rect.Y1 + 1, rect.X2 - rect.X1 + 1);
	//system("pause");
	//�ָ�õ��������־�������rlink�ں����ڲ�������ռ�
	rlink = DetectNum(img1, h, w, num);
	//ShowRectLink(rlink);	//��ʾ�õ��ľ�������
	//�������ţ���׼��֮���ͼ��
	//displayImg(img1, h, w);
	
	StdAlignImg(alignImg, img1, STD_H, num*STD_W, h, w, rlink, num);
	//displayImg(alignImg, STD_H, num*STD_W);
	ShowRectLink(rlink);	//��ʾ�õ��ľ�������
	
	//��ȡ����
	TZTQ13(tz, alignImg, STD_H, num*STD_W, rlink, num);

	//�ͷ��ڴ�
	delete_mem2d(alignImg, STD_H, num*STD_W);
	delete_mem2d(img1, srcHeight, srcWidth);
	DeRectLink(rlink);

}