/**************************************************************************************
*	����������ļ���ͼ����к��ڴ����������ȡ
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. ������ע������
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#include "img.h"

//���Է�����ַ����ڵľ�������
//Scr ָ��Դͼ���ָ�룬srcWidth Դͼ��Ŀ�ȣ�srcHeight ָ��Դͼ��ĸ߶�
int DetectRect(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth)
{
	int i,j;
	DRect rect = {0,0,0,0/* ,0,0,0,0 */};
	int minX = srcWidth;
	int maxX = 0;
	int minY = srcHeight;
	int maxY = 0;

	assert(Src != NULL);		//������飬����

	//�㷨һ
	//���X����ļ���С�����Y����ļ���С���꣬���Ǿ�������
	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if (0 == Src[i][j])		//ɨ�赽��ɫ��
			{
				if (i<minY) minY = i;	//��������ֵ���õ����α߿�
				if (i>maxY) maxY = i;
				if (j<minX) minX = j;
				if (j>maxX) maxX = j;
			}
		}
	}

	//���õ�������ת��Ϊ���ζԽ�����
	rect.lefttopX = minX;
	rect.lefttopY = minY;
	rect.rightbottomX = maxX;
	rect.rightbottomY = maxY;

	//��Դͼ���е�Ŀ�������������ظ��Ƹ�Ŀ��ͼ��
	for (i = rect.lefttopY; i <= rect.rightbottomY; i++)
	{
		for (j = rect.lefttopX; j <= rect.rightbottomX; j++)
		{
			Dst[i][j] = Src[i][j];
		}
	}

	return 0;
}



//����������ַ���ɵ�����
//Src ָ��Դͼ���ָ�룬srcWidth Դͼ��Ŀ�ȣ�srcHeight ָ��Դͼ��ĸ߶�
int DetectNum(UCHAR **Dst[], UCHAR **Src, int srcHeight, int srcWidth , int num)
{
	int i, j, count = 0;
	int x, y;		//�����ַ�������
	int cutFlag = FALSE;		//�иʼ�ı�־
	int allWhite = TRUE;		//��ǰ��ȫ��Ϊ��ɫ�ı�־
	int pos[60] = {0};		//���������ַ���ʼĩX����� 60/2=30 ���ַ�

	//�������£���������ɨ��,��ֱ���С�
	for(j = 0; j<srcWidth; j++)
	{
		allWhite = TRUE;	//���赱ǰ��ȫ��Ϊ��ɫ���ص�
		for(i = 0; i<srcHeight; i++)
		{
			//�ҳ������ַ��ľ�������
			if (0 == Src[i][j])		
			{
				allWhite = FALSE;
				if (cutFlag == FALSE)	//���Ϊ��ɫ���ص� && �иû�п�ʼ
				{
					cutFlag = TRUE;		//���и��־λ����ʼ
					pos[count*2] = j;		//��¼��ʼXֵ
					count++;
				}
			}
		}

		if (allWhite)		//�����ǰ��ȫ��Ϊ��ɫ���ص�
		{
			cutFlag = FALSE;	//�и��
			pos[count*2-1] = j;		//��¼����X����
		}
	}

	num = count;		//�����ַ�����ֵ

	//���������ַ���ͼ������
	for (count = 0; count<num ; count++)
	{
		x = 0;		//����ͼ�ġ�ָ�롱����
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

//��ͼ���࣬0���1,1���0
//Src ָ��Դͼ���ָ�룬srcWidth Դͼ��Ŀ�ȣ�srcHeight ָ��Դͼ��ĸ߶�
void InvertImg(UCHAR **Dst, UCHAR **Src , int srcHeight, int srcWidth)
{
	int i, j;

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			Dst[i][j] = 1 - Src[i][j];		//0-1����
		}
	}

	//return 0;
}

//��ͼ�������ֵȫ����Ϊtag
//Dst ָ��Դͼ���ָ�룬tag Ҫ�õ�ֵ
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

//��ͼ���ֵ��
//Src ָ��Դͼ���ָ�룬thres ��ֵ
int BinaryImg(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth, int thres)
{
	UCHAR *p1 = (UCHAR *)Src;
	UCHAR *p2 = (UCHAR *)Dst;
	int i = 0;

	//һ�ַ��ʶ�ά����ķ�ʽ
	for ( ; i < srcHeight*srcWidth; i++)
	{
		if (p1[i] >= thres)		//������ֵ����Ϊ1
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

//��ͼ�����ģ�����
//Src ָ��Դͼ���ָ�룬srcWidth Դͼ��Ŀ�ȣ�srcHeight Դͼ��ĸ߶ȣ�tepl ģ�����飬size ģ���С��fac ϵ��
int Template(UCHAR **Dst, UCHAR **Src, int srcHeight, \
			 int srcWidth, UCHAR **tepl, int size, double fac)
{	
	int i,j;
	int k;
	int sum = 0;
	UCHAR *p = NULL;

	assert(size%2 == 1);		//����ģ���С����Ϊ����

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if ((j<size/2) || (j>srcWidth - size/2) \
				|| (i<size/2) || (i>srcHeight - size/2))
			{
				//����Ǳ߽����ص㣬���ı�����ֵ
				Dst[i][j] = Src[i][j];
			}
			else	//���򣬾��
			{
				p = Src[i-size/2][j-size/2];		//��Ŀ����ģ����ʼ���긳��p

				if (p != NULL)		//��ָֹ�����
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