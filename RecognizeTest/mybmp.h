#ifndef _MY_BMP_
#define _MY_BMP_

#include "stdio.h"
#include "stdlib.h"
#include "base.h"

//���ýṹ��2���ֽڶ��룬����Ľṹ��Ϳ��Ժ�ʵ���ڴ�һһ��Ӧ�ˣ������ļ�ͷsizeof()=16
#pragma pack(2)

/*λͼ�ļ�ͷ*/
typedef struct BMP_FILE_HEADER
{
	WORD bType;             /*  �ļ���ʶ��          */
	DWORD bSize;            /*  �ļ��Ĵ�С          */
	WORD bReserved1;        /*  ����ֵ,��������Ϊ0  */       
	WORD bReserved2;        /*  ����ֵ,��������Ϊ0  */
	DWORD bOffset;          /*  �ļ�ͷ�����ͼ������λ��ʼ��ƫ����    */
} BMPFILEHEADER;

/*λͼ��Ϣͷ*/
typedef struct BMP_INFO
{
	DWORD bInfoSize;       /*  ��Ϣͷ�Ĵ�С             */
	DWORD bWidth;          /*  ͼ��Ŀ��               */
	DWORD bHeight;         /*  ͼ��ĸ߶�               */
	WORD bPlanes;          /*  ͼ���λ����             */
	WORD bBitCount;        /*  ÿ�����ص�λ��           */
	DWORD bCompression;    /*  ѹ������                 */
	DWORD bmpImageSize;    /*  ͼ��Ĵ�С,���ֽ�Ϊ��λ  */
	DWORD bXPelsPerMeter;  /*  ˮƽ�ֱ���               */
	DWORD bYPelsPerMeter;  /*  ��ֱ�ֱ���               */
	DWORD bClrUsed;        /*  ʹ�õ�ɫ����             */
	DWORD bClrImportant;   /*  ��Ҫ����ɫ��             */
} BMPINF;

/*��ɫ��*/
typedef struct RGB_QUAD 
{
	WORD rgbBlue;         /*  ��ɫǿ��  */
	WORD rgbGreen;        /*  ��ɫǿ��  */
	WORD rgbRed;          /*  ��ɫǿ��  */
	WORD rgbReversed;     /*  ����ֵ    */
} RGBQUAD;

//����һ���ֽ�����ΪUCHAR
typedef unsigned char UCHAR;

//��������
UCHAR **allloc_mem2d(int height, int width);	//����ά�������ռ�
int init_mem2d(UCHAR **img, int height, int width, UCHAR tag);		//��ʼ����ά����
void delete_mem2d(UCHAR **mem, int height, int width);		//���ٶ�ά����ռ�
int readBmp(UCHAR **image, int height, int width, char *filename);		//��ȡ24λBMP�ļ�
void displayImg(UCHAR **image, int height, int width);			//��cmd����ʾͼ�񣬹����������64*64
int saveImg(UCHAR **image, int height, int width, char *filename);		//�洢ͼ������Ϊtxt��ʽ�������������128*128
void test(UCHAR *image);	//���Ժ���
UCHAR BGR2GRAY(UCHAR a, UCHAR b, UCHAR c);		//BGRת��ΪGRAY


#endif