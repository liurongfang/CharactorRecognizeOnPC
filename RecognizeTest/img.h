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

//标准化宽和高
#define STD_W 8
#define STD_H 16

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


//建立矩形链表单元
typedef struct rectLink
{
	DRect data;
	struct rectLink *next;
}RectLink;

//一下三个函数均在mybmp.h中定义并在mybmp中实现
extern UCHAR **allloc_mem2d(int height, int width);
extern void delete_mem2d(UCHAR **mem, int height, int width);
extern void displayImg(UCHAR **image, int height, int width);

//创建长度为num的单向矩形链表
RectLink *CreateRectLink(int num);

//用给定的DRect初始化矩形链表
int InitRectLink(RectLink *head, DRect rect);

//销毁链表
int DeRectLink(RectLink *head);

//显示得到的矩形链表
void ShowRectLink(RectLink *rlink);

//粗略分离出字符所在的矩形区域
DRect DetectRect(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth);

//倾斜度矫正
void SlopeAdjust(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth);

//归一化单个字符到指定宽和高
int StdAlignImg(UCHAR **Dst, UCHAR **Src, int dstHeight, int dstWidth, int srcHeight, int srcWidth, RectLink *rlink, int num);

//标准化图像
int StandardImg(UCHAR **Dst, UCHAR **Src, DRect dstRect, DRect srcRect);

//特征提取，13个特征
int TZTQ13(UCHAR **tz, UCHAR **Src, int srcHeight, int srcWidth, RectLink *rlink, int num);

//分离出单个字符组成的数组
RectLink *DetectNum(UCHAR **Src, int srcHeight, int srcWidth , int num);

//将图像反相，0变成1,1变成0
void InvertImg(UCHAR **Dst, UCHAR **Src , int srcHeight, int srcWidth);

//将图像的像素值全部置为tag
int SetImg(UCHAR **Dst, int srcHeight, int srcWidth, UCHAR tag);

//将图像二值化
int BinaryImg(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth,  int thres);

//对图像进行模板操作
int Template(UCHAR **Dst, UCHAR **Src, int srcHeight, \
			 int srcWidth, UCHAR **tepl, int size, double fac);
















#endif