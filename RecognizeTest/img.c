/**************************************************************************************
*	����������ļ���ͼ����к��ڴ����������ȡ
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. ������ע������
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#include "img.h"

int after_h = 0;
int after_w = 0;

//���Է�����ַ����ڵľ�������
//Scr ָ��Դͼ���ָ�룬srcWidth Դͼ��Ŀ�ȣ�srcHeight ָ��Դͼ��ĸ߶�
int DetectRect(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth)
{
	int i,j;			//ָ��Src
	int k = 0, l = 0;		//ָ��Dst
	DRect rect = {0,0,0,0};
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
				if (i<=minY) minY = i;	//��������ֵ���õ����α߿�
				if (i>=maxY) maxY = i;
				if (j<=minX) minX = j;
				if (j>=maxX) maxX = j;
			}
		}
	}

	//���õ�������ת��Ϊ���ζԽ�����,������һ������
	rect.X1 = minX - 1;
	rect.Y1 = minY - 1;
	rect.X2 = maxX + 1;
	rect.Y2 = maxY + 1;

	//��Դͼ���е�Ŀ�������������ظ��Ƹ�Ŀ��ͼ��
	for (i = rect.Y1, k = 0; i <= rect.Y2; i++, k++)
	{
		for (j = rect.X1, l = 0; j <= rect.X2; j++, l++)
		{
			
			Dst[k][l] = Src[i][j];
		}
	}

	//ͬ���޸�ͼ���Ⱥ͸߶�
	after_h = rect.Y2 - rect.Y1 + 1;
	after_w = rect.X2 - rect.X1 + 1;

	return 0;
}


//��б�Ƚ���
void SlopeAdjust(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth)
{
	int i,j;
	int i_src;
	//ͼ�����ߵ�ƽ���߶�
	double leftaver=0.0;

	//ͼ���Ұ�ߵ�ƽ���߶�
	double rightaver=0.0;

	//ͼ�����б��
	double slope;

	//ͳ��ѭ������
	int counts=0;
	//int x1,x2;
	//double delta_x;
	//
	////�������£��������ң���¼���󶥵�
	//for (i = 0; i<srcHeight; i++)
	//{
	//	for (j = 0; j<srcWidth; j++)
	//	{
	//		if (0 == Src[i][j])		//��������ڵ�
	//		{
	//			x1 = j;		//��¼�µ�ǰxֵ����Ϊ�󶥵�x

	//			j = srcWidth;		//ǿ�Ƹ�ֵ���˳�ѭ��
	//			i = srcHeight;
	//		}
	//	}
	//}

	////�������ϣ��������ң���¼���󶥵㣬�������õ�deltaX
	//for (i = srcHeight - 1; i>=0; i--)
	//{
	//	for (j = 0; j<srcWidth; j++)
	//	{
	//		if (0 == Src[i][j])		//��������ڵ�
	//		{
	//			x2 = j;		//��¼�µ�ǰxֵ����Ϊ�󶥵�x

	//			j = srcWidth;		//ǿ�Ƹ�ֵ���˳�ѭ��
	//			i = 0;
	//		}
	//	}
	//}

	//delta_x = x2 - x1;		//�õ�deltaX

	

	//ɨ�����ߵ�ͼ�����ɫ���ص�ƽ���߶�

	//��
	for (i=0;i<srcHeight;i++)
	{   

		//��
		for (j=0;j<srcWidth/2;j++)
		{
			//���Ϊ�ڵ�
			if (0 == Src[i][j])
			{

				//����߶Ƚ���ͳ�Ƶ���
				counts +=srcWidth/2 -j;
				leftaver += i*(srcWidth/2 -j);

			}

		}
	}

	//����ƽ���߶�
	leftaver /= counts;

	//��ͳ��ѭ���������¸�ֵ
	counts =0;

	//ɨ���Ұ�ߵ�ͼ�����ɫ���ص�ƽ���߶�

	//��
	for (i =0;i<srcHeight;i++)
	{

		//��
		for (j=srcWidth/2;j<srcWidth;j++)
		{
			//���Ϊ�ڵ�
			if (0 == Src[i][j])
			{

				//����ͳ�Ƶ���
				counts +=srcWidth -j;
				rightaver += i*(srcWidth -j);
			}
		}
	}

	//�����Ұ�ߵ�ƽ���߶�
	rightaver /= counts;

	//����б��
	slope = (leftaver - rightaver) / (srcWidth/2);

	//����б�ʣ��ѵ�ǰ��ͼ��ĵ�ӳ�䵽Դͼ��ĵ�

	//��
	for (i=0;i<srcHeight;i++)
	{
		//��
		for (j=0;j<srcWidth;j++)
		{	
			//����ӳ��λ��	
			i_src = (int)(i - (j-srcWidth/2)*slope);

			//�������ͼ���⣬�����ð�ɫ
			if (i_src <0 || i_src >=srcHeight )
			{
				Dst[i][j] = 1;
			}
			else
			{	
				//����Դͼ�����ҵ㣬ȡ������ֵ

				//ָ���i_src�е�j�����ص�ָ��
				Dst[i][j] = Src[i][j];
			}
		}
	}

}

//��һ�������ַ���ָ����͸�
void StdImage(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth, int dstHeight, int dstWidth)
{
	int i,j;	//ָ��Դͼ��ı���
	int k,l;	//ָ��Ŀ��ͼ��ı���
	int xScale = 0, yScale = 0;

	//�õ�������
	xScale = (int ) (srcHeight / (double )dstHeight);
	yScale = (int ) (srcWidth / (double )dstWidth);

	//ѭ��������ϡ�����
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

//������ȡ
void TZTQ(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth)
{

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
	UCHAR *p = (UCHAR *)&Dst;
	int i = 0;

	while(p[i] != '\0')
	{
		p[i] = (UCHAR)tag;
		i++;
	}

	return 0;
}

//��ͼ���ֵ��
//Src ָ��Դͼ���ָ�룬thres ��ֵ
int BinaryImg(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth, int thres)
{
  //  UCHAR *p1 = (UCHAR *)&Src;
  //  UCHAR *p2 = (UCHAR *)&Dst;
  //  int i = 0;

  //  //һ�ַ��ʶ�ά����ķ�ʽ,
  //  for ( ; i < srcHeight*srcWidth; i++)
  //  {
  //      if (*p1 >= (UCHAR)thres)		//������ֵ����Ϊ1
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

	//�������ɨ��
	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			if (Src[i][j] > (UCHAR)thres)		//������ֵ
			{
				Dst[i][j] = 1;		//��Ϊ1
			} 
			else
			{
				Dst[i][j] = 0;		
			}
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