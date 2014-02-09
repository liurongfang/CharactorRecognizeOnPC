
//包含一些头文件
#include "stdio.h"
#include "stdlib.h"		//system()所在
#include "mybmp.h"		//图像处理、显示、存储，及存储空间的分配


//主函数
int main()
{
	//基本变量定义
	char *filename = "C:\\Users\\Mirgo\\Desktop\\01.bmp";		//要读取的图像文件路径
	char *outfname = "C:\\Users\\Mirgo\\Desktop\\a.txt";		//要保存的文本数据路径
	int height = 120;
	int width = 449;
	int i = 0;

	//给二维数组动态分配空间
	UCHAR **img = allloc_mem(height,width);

	//开始
	readBmp(img,height,width,filename);		//读取图片到img
	saveImg(img,height,width,outfname);		//保存img到文本
	displayImg(img,height,width);			//显示img

	//暂停，等待按键
	system("pause");

	//释放空间
	delete_mem(img, height, width);		//无论几维数组都是这样释放的

	return 0;
}

