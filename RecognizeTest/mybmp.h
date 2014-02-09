#ifndef _MY_BMP_
#define _MY_BMP_

#include "stdio.h"
#include "stdlib.h"

//���ýṹ��2���ֽڶ��룬����Ľṹ��Ϳ��Ժ�ʵ���ڴ�һһ��Ӧ�ˣ������ļ�ͷsizeof()=16
#pragma pack(2)

/*����UCHARΪһ���ֽڵ�����*/
typedef unsigned char UCHAR;
/*����WORDΪ�����ֽڵ�����*/
typedef unsigned short WORD;
/*����DWORDΪe�ĸ��ֽڵ�����*/
typedef unsigned long DWORD;

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
UCHAR **allloc_mem(int height, int width);
void delete_mem(UCHAR **mem, int height, int width);
int readBmp(UCHAR **image, int height, int width, char *filename);
void displayImg(UCHAR **image, int height, int width);
int saveImg(UCHAR **image, int height, int width, char *filename);
void test(UCHAR *image);
UCHAR BGR2GRAY(UCHAR a, UCHAR b, UCHAR c);


#endif