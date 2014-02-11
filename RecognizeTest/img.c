/**************************************************************************************
*	描述：这个文件对图像进行后期处理和特征提取
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. 引用需注明出处
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#include "img.h"

int after_h = 0;
int after_w = 0;

//粗略分离出字符所在的矩形区域
//Scr 指向源图像的指针，srcWidth 源图像的宽度，srcHeight 指向源图像的高度
int DetectRect(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth)
{
	int i,j;			//指向Src
	int k = 0, l = 0;		//指向Dst
	DRect rect = {0,0,0,0};
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
				if (i<=minY) minY = i;	//各方向求极值，得到矩形边框
				if (i>=maxY) maxY = i;
				if (j<=minX) minX = j;
				if (j>=maxX) maxX = j;
			}
		}
	}

	//将得到的坐标转换为矩形对角坐标,并外扩一个像素
	rect.X1 = minX - 1;
	rect.Y1 = minY - 1;
	rect.X2 = maxX + 1;
	rect.Y2 = maxY + 1;

	//将源图像中的目标矩形区域的像素复制给目标图像
	for (i = rect.Y1, k = 0; i <= rect.Y2; i++, k++)
	{
		for (j = rect.X1, l = 0; j <= rect.X2; j++, l++)
		{
			
			Dst[k][l] = Src[i][j];
		}
	}

	//同步修改图像宽度和高度
	after_h = rect.Y2 - rect.Y1 + 1;
	after_w = rect.X2 - rect.X1 + 1;

	return 0;
}


//倾斜度矫正
void SlopeAdjust(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth)
{
	int i,j;
	int i_src;
	//图像左半边的平均高度
	double leftaver=0.0;

	//图像右半边的平均高度
	double rightaver=0.0;

	//图像的倾斜度
	double slope;

	//统计循环变量
	int counts=0;
	//int x1,x2;
	//double delta_x;
	//
	////从上往下，从左往右，记录下左顶点
	//for (i = 0; i<srcHeight; i++)
	//{
	//	for (j = 0; j<srcWidth; j++)
	//	{
	//		if (0 == Src[i][j])		//如果碰到黑点
	//		{
	//			x1 = j;		//记录下当前x值，作为左顶点x

	//			j = srcWidth;		//强制赋值以退出循环
	//			i = srcHeight;
	//		}
	//	}
	//}

	////从下往上，从左往右，记录下左顶点，结合上面得到deltaX
	//for (i = srcHeight - 1; i>=0; i--)
	//{
	//	for (j = 0; j<srcWidth; j++)
	//	{
	//		if (0 == Src[i][j])		//如果碰到黑点
	//		{
	//			x2 = j;		//记录下当前x值，作为左顶点x

	//			j = srcWidth;		//强制赋值以退出循环
	//			i = 0;
	//		}
	//	}
	//}

	//delta_x = x2 - x1;		//得到deltaX

	

	//扫描左半边的图像，求黑色象素的平均高度

	//行
	for (i=0;i<srcHeight;i++)
	{   

		//列
		for (j=0;j<srcWidth/2;j++)
		{
			//如果为黑点
			if (0 == Src[i][j])
			{

				//对其高度进行统计叠加
				counts +=srcWidth/2 -j;
				leftaver += i*(srcWidth/2 -j);

			}

		}
	}

	//计算平均高度
	leftaver /= counts;

	//将统计循环变量重新赋值
	counts =0;

	//扫描右半边的图像，求黑色象素的平均高度

	//行
	for (i =0;i<srcHeight;i++)
	{

		//列
		for (j=srcWidth/2;j<srcWidth;j++)
		{
			//如果为黑点
			if (0 == Src[i][j])
			{

				//进行统计叠加
				counts +=srcWidth -j;
				rightaver += i*(srcWidth -j);
			}
		}
	}

	//计算右半边的平均高度
	rightaver /= counts;

	//计算斜率
	slope = (leftaver - rightaver) / (srcWidth/2);

	//根据斜率，把当前新图像的点映射到源图像的点

	//行
	for (i=0;i<srcHeight;i++)
	{
		//列
		for (j=0;j<srcWidth;j++)
		{	
			//计算映射位置	
			i_src = (int)(i - (j-srcWidth/2)*slope);

			//如果点在图像外，象素置白色
			if (i_src <0 || i_src >=srcHeight )
			{
				Dst[i][j] = 1;
			}
			else
			{	
				//否则到源图像中找点，取得象素值

				//指向第i_src行第j个象素的指针
				Dst[i][j] = Src[i][j];
			}
		}
	}

}

//归一化单个字符到指定宽和高
void StdImage(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth, int dstHeight, int dstWidth)
{
	int i,j;	//指向源图像的变量
	int k,l;	//指向目标图像的变量
	int xScale = 0, yScale = 0;

	//得到放缩比
	xScale = (int ) (srcHeight / (double )dstHeight);
	yScale = (int ) (srcWidth / (double )dstWidth);

	//循环遍历，稀疏采样
	for (i = 0; i<dstHeight; i++)
	{
		for (j = 0; j<dstWidth; j++)
		{
			Dst[i][j] = Src[i*yScale][j*xScale];
		}
	}

	after_h = dstHeight;
	after_w = dstWidth;
}

//特征提取
void TZTQ(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth)
{

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
	UCHAR *p = (UCHAR *)&Dst;
	int i = 0;

	while(p[i] != '\0')
	{
		p[i] = (UCHAR)tag;
		i++;
	}

	return 0;
}

//将图像二值化
//Src 指向源图像的指针，thres 阈值
int BinaryImg(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth, int thres)
{
  //  UCHAR *p1 = (UCHAR *)&Src;
  //  UCHAR *p2 = (UCHAR *)&Dst;
  //  int i = 0;

  //  //一种访问二维数组的方式,
  //  for ( ; i < srcHeight*srcWidth; i++)
  //  {
  //      if (*p1 >= (UCHAR)thres)		//大于阈值，置为1
  //      {
  //          p2[i] = 1;
  //      }
  //      else
  //      {
  //          p2[i] = 0;
  //      }
		//p1++;
  //  }

	int i, j;

	//逐个像素扫描
	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if (Src[i][j] > (UCHAR)thres)		//大于阈值
			{
				Dst[i][j] = 1;		//置为1
			} 
			else
			{
				Dst[i][j] = 0;		
			}
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