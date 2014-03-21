
//包含一些头文件
#include "stdio.h"
#include "stdlib.h"		//system()所在
#include "mybmp.h"		//图像处理、显示、存储，及存储空间的分配
#include "img.h"
#include "bp.h"


//2个字节对齐方式
#pragma pack(2)

//图像的宽高
int height = 100;	//注意！不要写反！
int width = 600;

void ImageHandle(UCHAR **tz, UCHAR **img, int srcHeight, int srcWidth, int num);
int Reconnize(UCHAR **tz, int num, int mode);		//如果特征数改变，需要改变函数里的tzCount

//主函数
int main()
{
	//基本变量定义
	char *filename = "C:\\Users\\Mirgo\\Desktop\\test2.bmp";		//要读取的图像文件路径
	char *outfname = "C:\\Users\\Mirgo\\Desktop\\a.txt";		//要保存的文本数据路径
	char *outfname2 = "C:\\Users\\Mirgo\\Desktop\\img.txt";
	int i = 0,j;
	int num = 10,tzCount = 13;
	int result = 0;

	//给二维数组动态分配空间
	UCHAR **img = allloc_mem2d(height,width);
	UCHAR **tz = allloc_mem2d(num,tzCount);


	//开始
	readBmp(img, height, width, filename);		//读取图片到img
	BinaryImg(img, img, height, width, 100);	//二值化图像

	//displayImg(img, height, width);			//显示img

	saveImg(img, height, width, outfname2);		//保存图像

	//暂停，等待按键
	//system("pause");

	ImageHandle(tz, img, height, width, num);
	printf("分别8块像素和、总像素和、水平两条、竖直两条\n");
	//displayImg(tz, num, 13);
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			printf("%3d ",tz[i][j]);
		}
		printf("\n");
	}

	//printf("ImageHandle OK!\n");
	system("pause");

	//printf("开始训练\n");
	//Reconnize(tz, num, 1);		//开始训练
	Reconnize(tz, num, 2);			//开始识别

	system("pause");

	//释放空间
	delete_mem2d(img, height, width);
	delete_mem2d(tz, num, tzCount);

	return 0;
}

//在这里完成图像处理的相关操作
void ImageHandle(UCHAR **tz, UCHAR **img, int srcHeight, int srcWidth, int num)
{
	UCHAR **img1 = allloc_mem2d(srcHeight, srcWidth);
	UCHAR **alignImg = allloc_mem2d(STD_H, num*STD_W);
	DRect rect;
	RectLink *rlink = NULL;
	int h = 0, w = 0;

	//二值化
	//displayImg(img, srcHeight, srcWidth);
	//BinaryImg(img, img, srcHeight, srcWidth, 100);
	//displayImg(img, srcHeight, srcWidth);
	
	//去噪
	//倾斜矫正
	//SlopeAdjust(img, img, srcHeight, srcWidth);
	//分离数字所在的矩形，dst在函数内部被分配空间
	rect = DetectRect(img1, img, srcHeight, srcWidth);
	h = rect.Y2 - rect.Y1 + 1;
	w = rect.X2 - rect.X1 + 1;

	saveImg(img1, h, w, "C:\\Users\\Mirgo\\Desktop\\img1.txt");		//保存图像
	//displayImg(img1, rect.Y2 - rect.Y1 + 1, rect.X2 - rect.X1 + 1);
	//system("pause");
	//分割得到单个数字矩形链表，rlink在函数内部被分配空间
	rlink = DetectNum(img1, h, w, num);
	//ShowRectLink(rlink);	//显示得到的矩形链表
	//紧缩重排，标准化之后的图像
	//displayImg(img1, h, w);
	
	StdAlignImg(alignImg, img1, STD_H, num*STD_W, h, w, rlink, num);
	//displayImg(alignImg, STD_H, num*STD_W);
	saveImg(alignImg, STD_H, num*STD_W, "C:\\Users\\Mirgo\\Desktop\\img2.txt");		//保存图像

	ShowRectLink(rlink);	//显示得到的矩形链表
	
	//提取特征
	TZTQ13(tz, alignImg, STD_H, num*STD_W, rlink, num);

	//释放内存
	delete_mem2d(alignImg, STD_H, num*STD_W);
	delete_mem2d(img1, srcHeight, srcWidth);
	DeRectLink(rlink);

}

//识别处理
int Reconnize(UCHAR **tz, int num, int mode)
{
	//循环变量
	int i, j;
	int tzCount = 13;		//每个字符的特征数

	//识别结果
	int result = 0;

	//网络参数
	int n_in = 13;	
	int n_hi = 10;

	//识别参数
	long max_cyc = 10000000;		//最大迭代次数，100万次
	double min_ex = 0.0001;
	double learnRate = 0.015;

	double **tezheng = allloc_mem2d_dbl(num, tzCount);
	double min = 99999,max = 0;			//分别记录特征矩阵中的最大最小值

	//预处理，将UCHAR **tz变成double **tz
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			tezheng[i][j] = (double)tz[i][j];		//强制转换为double型
			if (tezheng[i][j] > max) max = tezheng[i][j];		//更新最大值、最小值
			if (tezheng[i][j] < min) min = tezheng[i][j];
		}
	}

	//将特征矩阵归一化到0和1之间
	for (i = 0; i<num; i++)
	{
		for (j = 0; j<tzCount; j++)
		{
			tezheng[i][j] = (tezheng[i][j] - min + 1 )/(max - min + 1);		//归一化到0和1之间，加1是防止分母为0
		}
	}

	//
	if (mode == 1)		//模式1，训练模式
	{
		TrainBpNet(tezheng, n_in, n_hi, min_ex, max_cyc, learnRate, num);
	}
	else
	if (mode == 2)		//模式2，识别模式
	{
		result = NumRecongnize(tezheng, n_in, n_hi, num);

	}

	//释放空间
	detect_mem2d_dbl(tezheng, num, tzCount);

	return result;
}