
//包含一些头文件
#include "stdio.h"
#include "stdlib.h"		//system()所在
#include "mybmp.h"		//图像处理、显示、存储，及存储空间的分配
#include "img.h"

//图像的宽高
int height = 120;	//注意！不要写反！
int width = 449;

void ImageHandle(UCHAR **tz, UCHAR **img, int srcHeight, int srcWidth, int num);

//主函数
int main()
{
	//基本变量定义
	char *filename = "C:\\Users\\Mirgo\\Desktop\\01.bmp";		//要读取的图像文件路径
	char *outfname = "C:\\Users\\Mirgo\\Desktop\\a.txt";		//要保存的文本数据路径
	char *outfname2 = "C:\\Users\\Mirgo\\Desktop\\a1.txt";
	int i = 0;

	//给二维数组动态分配空间
	UCHAR **img = allloc_mem2d(height,width);
	UCHAR **tz = allloc_mem2d(8,13);

	//开始
	readBmp(img, height, width, filename);		//读取图片到img
	//BinaryImg(img, img, height, width, 100);	//二值化图像

	//displayImg(img, height, width);			//显示img

	//saveImg(img, height, width, outfname);		//保存图像

	//暂停，等待按键
	//system("pause");

	ImageHandle(tz, img, height, width, 8);
	printf("分别8块像素和、总像素和、水平两条、竖直两条\n");
	displayImg(tz, 8, 13);

	printf("ImageHandle OK!\n");
	system("pause");

	//释放空间
	delete_mem2d(img, height, width);
	delete_mem2d(tz, 8, 13);

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
	BinaryImg(img, img, srcHeight, srcWidth, 100);
	//displayImg(img, srcHeight, srcWidth);
	
	//去噪
	//倾斜矫正
	//SlopeAdjust(img, img, srcHeight, srcWidth);
	//分离数字所在的矩形，dst在函数内部被分配空间
	rect = DetectRect(img1, img, srcHeight, srcWidth);
	h = rect.Y2 - rect.Y1 + 1;
	w = rect.X2 - rect.X1 + 1;

	saveImg(img1, h, w, "C:\\Users\\Mirgo\\Desktop\\a.txt");		//保存图像
	//displayImg(img1, rect.Y2 - rect.Y1 + 1, rect.X2 - rect.X1 + 1);
	//system("pause");
	//分割得到单个数字矩形链表，rlink在函数内部被分配空间
	rlink = DetectNum(img1, h, w, num);
	//ShowRectLink(rlink);	//显示得到的矩形链表
	//紧缩重排，标准化之后的图像
	//displayImg(img1, h, w);
	
	StdAlignImg(alignImg, img1, STD_H, num*STD_W, h, w, rlink, num);
	//displayImg(alignImg, STD_H, num*STD_W);
	ShowRectLink(rlink);	//显示得到的矩形链表
	
	//提取特征
	TZTQ13(tz, alignImg, STD_H, num*STD_W, rlink, num);

	//释放内存
	delete_mem2d(alignImg, STD_H, num*STD_W);
	delete_mem2d(img1, srcHeight, srcWidth);
	DeRectLink(rlink);

}