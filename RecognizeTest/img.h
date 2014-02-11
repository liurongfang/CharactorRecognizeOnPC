/**************************************************************************************
*	描述：这个文件是对img.c里用到的函数进行声明
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. 引用需注明出处
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/


#ifndef _MY_IMG_
#define _MY_IMG_

#include "stdio.h"
#include "base.h"

//矩形结构的定义
typedef struct Rct
{
	int X1;		//左上角X和Y坐标
	int Y1;
	int X2;		//右下角X和Y坐标
	int Y2;
	//int lefttopX;
	//int lefttopY;
	//int rightbottomX;
	//int rightbottomY;
	// int leftbottomX;
	// int leftbottomY;
	// int righttopX;
	// int righttopY;
}DRect;



/*************************************************
Function: detectRect()
Description: 粗略分离出字符所在的矩形区域
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Input: Scr 指向源图像的指针，srcWidth 源图像的宽度，srcHeight 指向源图像的高度
Output: Dst 目标图像的指针
Return: 正常返回值 0
Date: 2014/01/29 
*************************************************/
int DetectRect(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth);



void SlopeAdjust(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth);

//归一化单个字符到指定宽和高
void StdImage(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth, int dstHeight, int dstWidth);

void TZTQ(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth);

/*************************************************
Function: detectNum()
Description: 分离出单个字符组成的数组
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Input: Scr 指向源图像的指针，srcWidth 源图像的宽度，srcHeight 指向源图像的高度
Output: Dst 目标图像的指针
Return: 正常返回值 0
Date: 2014/01/29 
*************************************************/
int DetectNum(UCHAR **Dst[], UCHAR **Src, int srcHeight, int srcWidth , int num);

/*************************************************
Function: InvertImg()
Description: 将图像反相，0变成1,1变成0
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Input: Scr 指向源图像的指针，srcWidth 源图像的宽度，srcHeight 指向源图像的高度
Output: Dst 目标图像的指针
Return: 正常返回值 0
Date: 2014/01/29 
*************************************************/
void InvertImg(UCHAR **Dst, UCHAR **Src , int srcHeight, int srcWidth);

/*************************************************
Function: SetImg()
Description: 将图像的像素值全部置为tag
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Input: Dst 指向源图像的指针，tag 要置的值
Output: Dst 目标图像的指针
Return: 正常返回值 0
Date: 2014/01/29 
*************************************************/
int SetImg(UCHAR **Dst, int srcHeight, int srcWidth, int tag);

/*************************************************
Function: BinaryImg()
Description: 将图像二值化
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Input: Src 指向源图像的指针，thres 阈值
Output: Dst 目标图像的指针
Return: 正常返回值 0
Date: 2014/01/29 
*************************************************/
int BinaryImg(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth,  int thres);

/*************************************************
Function: Template()
Description:  对图像进行模板操作
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Input: Src 指向源图像的指针，srcWidth 源图像的宽度，srcHeight 源图像的高度，tepl 模板数组，size 模板大小，fac 系数
Output: Dst 目标图像的指针
Return: 正常返回值 0
Date: 2014/01/29 
*************************************************/
int Template(UCHAR **Dst, UCHAR **Src, int srcHeight, \
			 int srcWidth, UCHAR **tepl, int size, double fac);
















#endif