/**************************************************************************************
*	描述：这个文件对图像进行后期处理和特征提取
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. 引用需注明出处
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#include "img.h"

//粗略分离出字符所在的矩形区域
//Scr 指向源图像的指针，srcWidth 源图像的宽度，srcHeight 指向源图像的高度
int DetectRect(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth)
{
	int i,j;
	DRect rect = {0,0,0,0/* ,0,0,0,0 */};
	int minX = srcWidth;
	int maxX = 0;
	int minY = srcHeight;
	int maxY = 0;

	assert(Src != NULL);		//参数检查，断言

	//算法一
	//求出X方向的极大极小坐标和Y方向的极大极小坐标，就是矩形区域
	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if (0 == Src[i][j])		//扫描到黑色点
			{
				if (i<minY) minY = i;	//各方向求极值，得到矩形边框
				if (i>maxY) maxY = i;
				if (j<minX) minX = j;
				if (j>maxX) maxX = j;
			}
		}
	}

	//将得到的坐标转换为矩形对角坐标
	rect.lefttopX = minX;
	rect.lefttopY = minY;
	rect.rightbottomX = maxX;
	rect.rightbottomY = maxY;

	//将源图像中的目标矩形区域的像素复制给目标图像
	for (i = rect.lefttopY; i <= rect.rightbottomY; i++)
	{
		for (j = rect.lefttopX; j <= rect.rightbottomX; j++)
		{
			Dst[i][j] = Src[i][j];
		}
	}

	return 0;
}



//分离出单个字符组成的数组
//Src 指向源图像的指针，srcWidth 源图像的宽度，srcHeight 指向源图像的高度
int DetectNum(UCHAR **Dst[], UCHAR **Src, int srcHeight, int srcWidth , int num)
{
	int i, j, count = 0;
	int x, y;		//单个字符的坐标
	int cutFlag = FALSE;		//切割开始的标志
	int allWhite = TRUE;		//当前列全部为白色的标志
	int pos[60] = {0};		//用来保存字符的始末X，最多 60/2=30 个字符

	//从上往下，从左往右扫描,竖直“切”
	for(j = 0; j<srcWidth; j++)
	{
		allWhite = TRUE;	//假设当前列全部为白色像素点
		for(i = 0; i<srcHeight; i++)
		{
			//找出单个字符的矩形区域
			if (0 == Src[i][j])		
			{
				allWhite = FALSE;
				if (cutFlag == FALSE)	//如果为黑色像素点 && 切割还没有开始
				{
					cutFlag = TRUE;		//置切割标志位：开始
					pos[count*2] = j;		//记录开始X值
					count++;
				}
			}
		}

		if (allWhite)		//如果当前列全部为白色像素点
		{
			cutFlag = FALSE;	//切割毕
			pos[count*2-1] = j;		//记录结束X坐标
		}
	}

	num = count;		//保存字符个数值

	//拷贝单个字符进图像数组
	for (count = 0; count<num ; count++)
	{
		x = 0;		//单幅图的“指针”置零
		y = 0;

		for (i = 0; i<srcHeight; i++)
		{
			for (j = pos[count*2]; j<pos[count*2+1]; j++)
			{
				Dst[count][x++][y++] = Src[i][j];
			}
		}
	}

	return 0;
}

//将图像反相，0变成1,1变成0
//Src 指向源图像的指针，srcWidth 源图像的宽度，srcHeight 指向源图像的高度
void InvertImg(UCHAR **Dst, UCHAR **Src , int srcHeight, int srcWidth)
{
	int i, j;

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			Dst[i][j] = 1 - Src[i][j];		//0-1反相
		}
	}

	//return 0;
}

//将图像的像素值全部置为tag
//Dst 指向源图像的指针，tag 要置的值
int SetImg(UCHAR **Dst, int srcHeight, int srcWidth, int tag)
{
	char *p = (char *)Dst;
	int i = 0;

	while(p[i] != '\0')
	{
		p[i] = tag;
		i++;
	}

	return 0;
}

//将图像二值化
//Src 指向源图像的指针，thres 阈值
int BinaryImg(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth, int thres)
{
	UCHAR *p1 = (UCHAR *)Src;
	UCHAR *p2 = (UCHAR *)Dst;
	int i = 0;

	//一种访问二维数组的方式
	for ( ; i < srcHeight*srcWidth; i++)
	{
		if (p1[i] >= thres)		//大于阈值，置为1
		{
			p2[i] = 1;
		}
		else
		{
			p2[i] = 0;
		}
	}

	return 0;
}

//对图像进行模板操作
//Src 指向源图像的指针，srcWidth 源图像的宽度，srcHeight 源图像的高度，tepl 模板数组，size 模板大小，fac 系数
int Template(UCHAR **Dst, UCHAR **Src, int srcHeight, \
			 int srcWidth, UCHAR **tepl, int size, double fac)
{	
	int i,j;
	int k;
	int sum = 0;
	UCHAR *p = NULL;

	assert(size%2 == 1);		//限制模板大小必须为奇数

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if ((j<size/2) || (j>srcWidth - size/2) \
				|| (i<size/2) || (i>srcHeight - size/2))
			{
				//如果是边界像素点，不改变像素值
				Dst[i][j] = Src[i][j];
			}
			else	//否则，卷积
			{
				p = Src[i-size/2][j-size/2];		//将目标点的模板起始坐标赋给p

				if (p != NULL)		//防止指针出错
				{
					for (k = 0; k<size*size; k++, p++)
					{
						sum += (*p) * tepl[k/size][k%size];
					}
				}

				Dst[i][j] = (int)fac*sum;
			}
		}
	}

	return 0;
}