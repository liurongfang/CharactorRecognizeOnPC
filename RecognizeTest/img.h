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



/*************************************************
Function: detectRect()
Description: ���Է�����ַ����ڵľ�������
Calls: // �����������õĺ����嵥
Called By: // ���ñ������ĺ����嵥
Input: Scr ָ��Դͼ���ָ�룬srcWidth Դͼ��Ŀ�ȣ�srcHeight ָ��Դͼ��ĸ߶�
Output: Dst Ŀ��ͼ���ָ��
Return: ��������ֵ 0
Date: 2014/01/29 
*************************************************/
int DetectRect(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth);



void SlopeAdjust(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth);

//��һ�������ַ���ָ����͸�
void StdImage(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth, int dstHeight, int dstWidth);

void TZTQ(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth);

/*************************************************
Function: detectNum()
Description: ����������ַ���ɵ�����
Calls: // �����������õĺ����嵥
Called By: // ���ñ������ĺ����嵥
Input: Scr ָ��Դͼ���ָ�룬srcWidth Դͼ��Ŀ�ȣ�srcHeight ָ��Դͼ��ĸ߶�
Output: Dst Ŀ��ͼ���ָ��
Return: ��������ֵ 0
Date: 2014/01/29 
*************************************************/
int DetectNum(UCHAR **Dst[], UCHAR **Src, int srcHeight, int srcWidth , int num);

/*************************************************
Function: InvertImg()
Description: ��ͼ���࣬0���1,1���0
Calls: // �����������õĺ����嵥
Called By: // ���ñ������ĺ����嵥
Input: Scr ָ��Դͼ���ָ�룬srcWidth Դͼ��Ŀ�ȣ�srcHeight ָ��Դͼ��ĸ߶�
Output: Dst Ŀ��ͼ���ָ��
Return: ��������ֵ 0
Date: 2014/01/29 
*************************************************/
void InvertImg(UCHAR **Dst, UCHAR **Src , int srcHeight, int srcWidth);

/*************************************************
Function: SetImg()
Description: ��ͼ�������ֵȫ����Ϊtag
Calls: // �����������õĺ����嵥
Called By: // ���ñ������ĺ����嵥
Input: Dst ָ��Դͼ���ָ�룬tag Ҫ�õ�ֵ
Output: Dst Ŀ��ͼ���ָ��
Return: ��������ֵ 0
Date: 2014/01/29 
*************************************************/
int SetImg(UCHAR **Dst, int srcHeight, int srcWidth, int tag);

/*************************************************
Function: BinaryImg()
Description: ��ͼ���ֵ��
Calls: // �����������õĺ����嵥
Called By: // ���ñ������ĺ����嵥
Input: Src ָ��Դͼ���ָ�룬thres ��ֵ
Output: Dst Ŀ��ͼ���ָ��
Return: ��������ֵ 0
Date: 2014/01/29 
*************************************************/
int BinaryImg(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth,  int thres);

/*************************************************
Function: Template()
Description:  ��ͼ�����ģ�����
Calls: // �����������õĺ����嵥
Called By: // ���ñ������ĺ����嵥
Input: Src ָ��Դͼ���ָ�룬srcWidth Դͼ��Ŀ�ȣ�srcHeight Դͼ��ĸ߶ȣ�tepl ģ�����飬size ģ���С��fac ϵ��
Output: Dst Ŀ��ͼ���ָ��
Return: ��������ֵ 0
Date: 2014/01/29 
*************************************************/
int Template(UCHAR **Dst, UCHAR **Src, int srcHeight, \
			 int srcWidth, UCHAR **tepl, int size, double fac);
















#endif