/**************************************************************************************
*	描述：这个文件是对程序中通用的一些宏和结构体类型进行声明
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. 引用需注明出处
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#ifndef _MY_BASE_
#define _MY_BASE_

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

//断言，错误就直接返回
//#define assert(x) ( (x)?():(return))


#define TRUE (-1)		//C语言规定，0为假，非0为真，所以这里这样定义
#define FALSE (0)


//基本数据类型定义

/*定义UCHAR为一个字节的类型*/
typedef unsigned char UCHAR;

/*定义WORD为两个字节的类型*/
typedef unsigned short WORD;

/*定义DWORD为e四个字节的类型*/
typedef unsigned long DWORD;


#endif