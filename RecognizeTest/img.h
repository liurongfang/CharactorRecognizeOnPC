/**************************************************************************************
*	����������ļ��Ƕ�img.c���õ��ĺ�����������
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. ������ע������
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/


#ifndef _MY_IMG_
#define _MY_IMG_

#include "stdio.h"
#include "base.h"

//��׼����͸�
#define STD_W 8
#define STD_H 16

//���νṹ�Ķ���
typedef struct Rct
{
	int X1;		//���Ͻ�X��Y����
	int Y1;
	int X2;		//���½�X��Y����
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


//������������Ԫ
typedef struct rectLink
{
	DRect data;
	struct rectLink *next;
}RectLink;

//һ��������������mybmp.h�ж��岢��mybmp��ʵ��
extern UCHAR **allloc_mem2d(int height, int width);
extern void delete_mem2d(UCHAR **mem, int height, int width);
extern void displayImg(UCHAR **image, int height, int width);

//��������Ϊnum�ĵ����������
RectLink *CreateRectLink(int num);

//�ø�����DRect��ʼ����������
int InitRectLink(RectLink *head, DRect rect);

//��������
int DeRectLink(RectLink *head);

//��ʾ�õ��ľ�������
void ShowRectLink(RectLink *rlink);

//���Է�����ַ����ڵľ�������
DRect DetectRect(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth);

//��б�Ƚ���
void SlopeAdjust(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth);

//��һ�������ַ���ָ����͸�
int StdAlignImg(UCHAR **Dst, UCHAR **Src, int dstHeight, int dstWidth, int srcHeight, int srcWidth, RectLink *rlink, int num);

//��׼��ͼ��
int StandardImg(UCHAR **Dst, UCHAR **Src, DRect dstRect, DRect srcRect);

//������ȡ��13������
int TZTQ13(UCHAR **tz, UCHAR **Src, int srcHeight, int srcWidth, RectLink *rlink, int num);

//����������ַ���ɵ�����
RectLink *DetectNum(UCHAR **Src, int srcHeight, int srcWidth , int num);

//��ͼ���࣬0���1,1���0
void InvertImg(UCHAR **Dst, UCHAR **Src , int srcHeight, int srcWidth);

//��ͼ�������ֵȫ����Ϊtag
int SetImg(UCHAR **Dst, int srcHeight, int srcWidth, UCHAR tag);

//��ͼ���ֵ��
int BinaryImg(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth,  int thres);

//��ͼ�����ģ�����
int Template(UCHAR **Dst, UCHAR **Src, int srcHeight, \
			 int srcWidth, UCHAR **tepl, int size, double fac);
















#endif