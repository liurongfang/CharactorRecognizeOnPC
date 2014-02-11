
//包含一些头文件
#include "stdio.h"
#include "stdlib.h"		//system()所在
#include "mybmp.h"		//图像处理、显示、存储，及存储空间的分配
#include "img.h"

//图像的宽高
int height = 120;	//注意！不要写反！
int width = 449;

//截取后的宽高
extern after_h;
extern after_w;

//标准化宽和高
int std_h = 64;
int std_w = 64;

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
	UCHAR **dst = allloc_mem2d(std_h,std_w);

	//开始
	readBmp(img, height, width, filename);		//读取图片到img
	BinaryImg(img, img, height, width, 100);			//二值化图像
	DetectRect(img, img, height, width);		//分离数字所在矩形区域,得到了after_h和after_w
	displayImg(img, after_h, after_w);			//显示img
	system("pause");
	//SlopeAdjust(img, img, height, width);		//倾斜度矫正
	//StdImage(dst, img, after_h, after_w, std_h, std_w);

	saveImg(img, after_h, after_w, outfname);		//保存img到文本
	saveImg(dst, std_h, std_w, outfname2);		//保存img到文本

	displayImg(img, after_h, after_w);			//显示img

	//暂停，等待按键
	system("pause");
	system("cls");

	displayImg(dst, std_h, std_w);			//显示img

	system("pause");

	//释放空间
	delete_mem2d(img, height, width);
	delete_mem2d(dst, std_h, std_w);

	return 0;
}

