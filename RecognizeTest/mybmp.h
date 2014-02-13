#ifndef _MY_BMP_
#define _MY_BMP_

#include "stdio.h"
#include "stdlib.h"
#include "base.h"

//设置结构体2个字节对齐，下面的结构体就可以和实际内存一一对应了，否则文件头sizeof()=16
#pragma pack(2)

/*位图文件头*/
typedef struct BMP_FILE_HEADER
{
	WORD bType;             /*  文件标识符          */
	DWORD bSize;            /*  文件的大小          */
	WORD bReserved1;        /*  保留值,必须设置为0  */       
	WORD bReserved2;        /*  保留值,必须设置为0  */
	DWORD bOffset;          /*  文件头的最后到图像数据位开始的偏移量    */
} BMPFILEHEADER;

/*位图信息头*/
typedef struct BMP_INFO
{
	DWORD bInfoSize;       /*  信息头的大小             */
	DWORD bWidth;          /*  图像的宽度               */
	DWORD bHeight;         /*  图像的高度               */
	WORD bPlanes;          /*  图像的位面数             */
	WORD bBitCount;        /*  每个像素的位数           */
	DWORD bCompression;    /*  压缩类型                 */
	DWORD bmpImageSize;    /*  图像的大小,以字节为单位  */
	DWORD bXPelsPerMeter;  /*  水平分辨率               */
	DWORD bYPelsPerMeter;  /*  垂直分辨率               */
	DWORD bClrUsed;        /*  使用的色彩数             */
	DWORD bClrImportant;   /*  重要的颜色数             */
} BMPINF;

/*彩色表*/
typedef struct RGB_QUAD 
{
	WORD rgbBlue;         /*  蓝色强度  */
	WORD rgbGreen;        /*  绿色强度  */
	WORD rgbRed;          /*  红色强度  */
	WORD rgbReversed;     /*  保留值    */
} RGBQUAD;

//定义一个字节类型为UCHAR
typedef unsigned char UCHAR;

//函数声明
UCHAR **allloc_mem2d(int height, int width);	//给二维数组分配空间
int init_mem2d(UCHAR **img, int height, int width, UCHAR tag);		//初始化二维数组
void delete_mem2d(UCHAR **mem, int height, int width);		//销毁二维数组空间
int readBmp(UCHAR **image, int height, int width, char *filename);		//读取24位BMP文件
void displayImg(UCHAR **image, int height, int width);			//在cmd里显示图像，过大则放缩到64*64
int saveImg(UCHAR **image, int height, int width, char *filename);		//存储图像数组为txt格式，过大则放缩到128*128
void test(UCHAR *image);	//测试函数
UCHAR BGR2GRAY(UCHAR a, UCHAR b, UCHAR c);		//BGR转换为GRAY


#endif