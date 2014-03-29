
//����һЩͷ�ļ�
#include "stdio.h"
#include "stdlib.h"		//system()����
#include "mybmp.h"		//ͼ������ʾ���洢�����洢�ռ�ķ���
#include "img.h"
#include "bp.h"


//2���ֽڶ��뷽ʽ
#pragma pack(2)

//ͼ��Ŀ��
int height = 100;	//ע�⣡��Ҫд����
int width = 600;

void ImageHandle(UCHAR **tz, UCHAR **img, int srcHeight, int srcWidth, int num);
int Reconnize(UCHAR **tz, int num, int mode);		//����������ı䣬��Ҫ�ı亯�����tzCount
void TrainIt(UCHAR ***tz, int num);			//���ͼ��������ѵ��
int ReadTxtAsImg(UCHAR **image, int height, int width, char *filename);			//���ı�����ͼ������

//������
int main()
{
	//������������
	char *filename[5] = {"C:\\Users\\Mirgo\\Desktop\\learn0.txt",
						"C:\\Users\\Mirgo\\Desktop\\learn1.txt",
						"C:\\Users\\Mirgo\\Desktop\\learn2.txt",
						"C:\\Users\\Mirgo\\Desktop\\learn3.txt",
						"C:\\Users\\Mirgo\\Desktop\\learn4.txt"};		//Ҫ��ȡ��ͼ���ļ�·��,5��ͼ��ÿ����������10��
	char *outfname = "C:\\Users\\Mirgo\\Desktop\\a.txt";		//Ҫ������ı�����·��
	char *outfname2 = "C:\\Users\\Mirgo\\Desktop\\img.txt";
	UCHAR ***tz = NULL;
	int i = 0,j,k;
	int num = 10, tzCount = 13;
	int result = 0;

	//����ά���鶯̬����ռ�
	UCHAR **img = allloc_mem2d(height,width);	//�����ȡ����ͼ��
	for (i = 0; i<5; i++)		//����5����������ռ�
	{
		tz[i] = allloc_mem2d(num,tzCount);
	}


	for (k = 0; k<5; k++)
	{
		//��ʼ
		ReadTxtAsImg(img, height, width, filename[k]);		//��ȡͼƬ��img
		BinaryImg(img, img, height, width, 100);	//��ֵ��ͼ��

		//displayImg(img, height, width);			//��ʾimg

		saveImg(img, height, width, outfname2);		//����ͼ��

		//��ͣ���ȴ�����
		//system("pause");

		ImageHandle(tz[k], img, height, width, num);

		printf("�ֱ�8�����غ͡������غ͡�ˮƽ��������ֱ����\n");
		displayImg(tz[k], num, 13);

		for (i = 0; i<num; i++)
		{
			for (j = 0; j<tzCount; j++)
			{
				printf("%3d ",tz[k][i][j]);
			}
			printf("\n");
		}
	}



	//printf("ImageHandle OK!\n");
	system("pause");

	//printf("��ʼѵ��\n");
	//Reconnize(tz, num, 1);		//��ʼѵ��
	TrainIt(tz, num);				//ѵ��
	//Reconnize(tz, num, 2);		//��ʼʶ��

	system("pause");

	//�ͷſռ�
	delete_mem2d(img, height, width);
	for (k = 0; k<5; k++)
	{
		delete_mem2d(tz[k], num, tzCount);
	}

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
	//BinaryImg(img, img, srcHeight, srcWidth, 100);
	//displayImg(img, srcHeight, srcWidth);
	
	//ȥ��
	//��б����
	//SlopeAdjust(img, img, srcHeight, srcWidth);
	//�����������ڵľ��Σ�dst�ں����ڲ�������ռ�
	rect = DetectRect(img1, img, srcHeight, srcWidth);
	h = rect.Y2 - rect.Y1 + 1;
	w = rect.X2 - rect.X1 + 1;

	saveImg(img1, h, w, "C:\\Users\\Mirgo\\Desktop\\img1.txt");		//����ͼ��
	//displayImg(img1, rect.Y2 - rect.Y1 + 1, rect.X2 - rect.X1 + 1);
	//system("pause");
	//�ָ�õ��������־�������rlink�ں����ڲ�������ռ�
	rlink = DetectNum(img1, h, w, num);
	//ShowRectLink(rlink);	//��ʾ�õ��ľ�������
	//�������ţ���׼��֮���ͼ��
	//displayImg(img1, h, w);
	
	StdAlignImg(alignImg, img1, STD_H, num*STD_W, h, w, rlink, num);
	//displayImg(alignImg, STD_H, num*STD_W);
	saveImg(alignImg, STD_H, num*STD_W, "C:\\Users\\Mirgo\\Desktop\\img2.txt");		//����ͼ��

	ShowRectLink(rlink);	//��ʾ�õ��ľ�������
	
	//��ȡ����
	TZTQ13(tz, alignImg, STD_H, num*STD_W, rlink, num);

	//�ͷ��ڴ�
	delete_mem2d(alignImg, STD_H, num*STD_W);
	delete_mem2d(img1, srcHeight, srcWidth);
	DeRectLink(rlink);

}

//ʶ����&����ͼ��������ѵ��
int Reconnize(UCHAR **tz, int num, int mode)
{
	//ѭ������
	int i, j;
	int tzCount = 13;		//ÿ���ַ���������

	//ʶ����
	int result = 0;

	//�������
	int n_in = 13;	
	int n_hi = 10;

	//ʶ�����
	long max_cyc = 10000000;		//������������100���
	double min_ex = 0.0001;
	double learnRate = 0.015;

	double **tezheng = allloc_mem2d_dbl(num, tzCount);
	double min = 99999,max = 0;			//�ֱ��¼���������е������Сֵ

	//Ԥ������UCHAR **tz���double **tz
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			tezheng[i][j] = (double)tz[i][j];		//ǿ��ת��Ϊdouble��
			if (tezheng[i][j] > max) max = tezheng[i][j];		//�������ֵ����Сֵ
			if (tezheng[i][j] < min) min = tezheng[i][j];
		}
	}

	//�����������һ����0��1֮��
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			tezheng[i][j] = (tezheng[i][j] - min + 1 )/(max - min + 1);		//��һ����0��1֮�䣬��1�Ƿ�ֹ��ĸΪ0
		}
	}


	if (mode == 1)		//ģʽ1��ѵ��ģʽ
	{
		TrainBpNet(tezheng, n_in, n_hi, min_ex, max_cyc, learnRate, num);		//һ��ѵ��һ��ͼ��
	}
	else
	if (mode == 2)		//ģʽ2��ʶ��ģʽ
	{
		result = NumRecongnize(tezheng, n_in, n_hi, num);
	}
	

	//�ͷſռ�
	detect_mem2d_dbl(tezheng, num, tzCount);

	return result;
}

//���ͼ��������ѵ��
void TrainIt(UCHAR ***tz, int num)
{
	//ѭ������
	int i, j, k;
	int tzCount = 13;		//ÿ���ַ���������

	//ʶ����
	int result = 0;

	//�������
	int n_in = 13;	
	int n_hi = 10;

	//ʶ�����
	long max_cyc = 150000;		//������������15w��
	double min_ex = 0.0001;
	double learnRate = 0.015;

	double ***tezheng =NULL;
	double min = 99999,max = 0;			//�ֱ��¼���������е������Сֵ

	for (i = 0; i<5; i++)
	{
		tezheng[i] = allloc_mem2d_dbl(num, tzCount);
	}

	//Ԥ������UCHAR **tz���double **tz
	for (k = 0; k<5; k++)
	{
		for (i = 0; i<num; i++)
		{
			for (j = 0; j<tzCount; j++)
			{
				tezheng[k][i][j] = (double)tz[k][i][j];		//ǿ��ת��Ϊdouble��
				if (tezheng[k][i][j] > max) max = tezheng[k][i][j];		//�������ֵ����Сֵ
				if (tezheng[k][i][j] < min) min = tezheng[k][i][j];
			}
		}
	}
	

	//�����������һ����0��1֮��
	for (k = 0; k<5; k++)
	{
		for (i = 0; i<num; i++)
		{
			for (j = 0; j<tzCount; j++)
			{
				tezheng[k][i][j] = (tezheng[k][i][j] - min + 1 )/(max - min + 1);		//��һ����0��1֮�䣬��1�Ƿ�ֹ��ĸΪ0
			}
		}
	}

	//��ʼѵ��
	RxTrainBpNet(tezheng, n_in, n_hi, min_ex, max_cyc, learnRate, num);		//һ��ѵ�����ͼ��

	//�ͷſռ�
	for (k = 0; k<5; k++)
	{
		detect_mem2d_dbl(tezheng[k], num, tzCount);
	}
	
}

//���ı�����ͼ������
int ReadTxtAsImg(UCHAR **image, int height, int width, char *filename)
{
	int i,j;
	FILE *fp = NULL;

	if ((fp  = fopen(filename, "r")) == NULL )
	{
		printf("file not found,please check dir:%s\n",filename);
		return -1;
	}

	//���ı���ȡ
	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			 fscanf(fp, "%d", &image[i][j]);
		}
	}

	fclose(fp);

	return 0;
}