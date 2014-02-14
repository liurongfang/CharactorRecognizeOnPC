/**************************************************************************************
*	����������ļ���ĺ�����ͼ����к��ڴ����������ȡ
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved.	��������ϵ����
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#include "img.h"

//int after_h = 0;
//int after_w = 0;

//������������
RectLink *CreateRectLink(int num)
{
	RectLink *head = NULL;		//����ͷָ��
	RectLink *p, *pTmp;			//p����ǰ��Ԫ��pTmp�������һ����Ԫ
	int i = 0;

	//�ǵ�һ���ڵ㣬��headֱ����ϵ����
	head = p = pTmp = (RectLink *)malloc(sizeof(RectLink));
	p->next = NULL;
	//head->next = p;
	//pTmp = head->next;

	//���ǵ�1���ڵ㣬��pTmp���м����ӵ���β��
	//������num-1��ѭ��
	while (--num)
	{
		p = (RectLink *)malloc(sizeof(RectLink));		//�������ڴ�ռ�
		p->next = NULL;			//β��ֵNULL
		pTmp->next = p;			//����ǰ����Ԫ�����ӵ��������
		pTmp = pTmp->next;		//ָ�����һλ
		i++;
	}

	//printf("Create!%d\n",i);
	//system("pause");

	return (head);

}

//�ø�����DRect��ʼ����������
int InitRectLink(RectLink *head, DRect rect)
{
	RectLink *p = head;
	while (p->next != NULL)
	{
		p->data = rect;
		//printf("Hello!%d %d %d %d",head->data.X1,head->data.Y1,head->data.X2,head->data.Y2);
		p = p->next;
	}
	p->data = rect;
	//printf("Hello!%d %d %d %d",head->data.X1,head->data.Y1,head->data.X2,head->data.Y2);

	return 0;
}

//��������
int DeRectLink(RectLink *head)
{
	RectLink *p;

	while(head->next != NULL)
	{
		p = head->next;
		free(head);
		head = p;
	}
	free(head);		//head->next == NULL�����һ���ڵ㣬�ͷŵ�
	head = NULL;	//��ͷ�����Ϊ��

	return 0;
}

//��ʾ�õ��ľ�������
void ShowRectLink(RectLink *rlink)
{
	int i = 0;
	RectLink *p = rlink;

	printf("��������ӡ��ľ�������\n");
	do
	{
		printf("��%d�����Σ�{%d %d %d %d}\n",i,p->data.X1,p->data.Y1,p->data.X2,p->data.Y2);
		i++;
		p = p->next;
	}
	while(p != NULL);
	printf("�ܹ�%d������\n",i);

	system("pause");
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

//���Է�����ַ����ڵľ�������
DRect DetectRect(UCHAR **Dst, UCHAR **Src, int srcHeight, int srcWidth)
{
	int i,j;			//ָ��Src
	int k = 0, l = 0;		//ָ��Dst
	DRect rect = {0,0,0,0};
	int minX = srcWidth;
	int maxX = 0;
	int minY = srcHeight;
	int maxY = 0;

	//assert(Src != NULL);		//������飬����
	if (Src == NULL)
	{
		return;
	}

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

	//���õ�������ת��Ϊ���ζԽ�����,������һ������
	rect.X1 = (minX - 1)<0?0:(minX - 1);
	rect.Y1 = (minY - 1)<0?0:(minY - 1);
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
	//after_h = rect.Y2 - rect.Y1 + 1;
	//after_w = rect.X2 - rect.X1 + 1;

	return (rect);
}



//����������ַ���ɵ����飬���ѭ������ʱ�ϳ�
RectLink *DetectNum(UCHAR **Src, int srcHeight, int srcWidth , int num)
{
	int i, j, count = 0;
	//int x, y;		//�����ַ�������
	int cutFlag = FALSE;		//�иʼ�ı�־
	int allWhite = TRUE;		//��ǰ��ȫ��Ϊ��ɫ�ı�־
	//int pos[60] = {0};		//���������ַ���ʼĩX����� 60/2=30 ���ַ�
	DRect rect = {0, 0, 0, srcHeight};

	RectLink *rectlink = CreateRectLink(num);		//��������num�����εľ�������
	RectLink *p = rectlink;		//����ָ��rectlink��
	//printf("Hello!");
	InitRectLink(p, rect);		//��rect��ʼ��rectLink
	p = rectlink;
	//printf("Hello!");


	//�������£���������ɨ��,��ֱ���С�
	for(j = 0; j<srcWidth; j++)
	{
		allWhite = TRUE;	//���赱ǰ���С�ȫ��Ϊ��ɫ���ص�
		for(i = 0; i<srcHeight; i++)
		{
			//�ҳ������ַ��ľ�������
			if (0 == Src[i][j])		
			{
				allWhite = FALSE;
				if (cutFlag == FALSE)	//���Ϊ��ɫ���ص� && �иû�п�ʼ
				{
					cutFlag = TRUE;		//���и��־λ����ʼ
					p->data.X1 = j;		//��¼��ʼXֵ
				}
				else		//����иʼ�ˣ�����ѭ��
				{
					break;
				}
			}
		}

		if (allWhite && cutFlag)		//�����ǰ��ȫ��Ϊ��ɫ���ص㣬�и��
		{
			cutFlag = FALSE;	//�и��
			p->data.X2 = j-1;		//��¼����X����

			//printf("NUM:%d\nRECT:{%d %d %d %d}\n",count,p->data.X1,p->data.Y1,p->data.X2,p->data.Y2);
			count++;			//�и������1
			p = p->next;		//׼����һ���������������
		}

		if (count == num)		//����и���num���ַ�����ɣ���������
		{
			break;
		}//else contiune
		
	}

	//printf("TEST1,OK\n");
	//system("pause");

	//num = count;		//�����ַ�����ֵ

	//����ϸ����������
	p = rectlink;		//pָ�븴λ��rectlinkͷ
	count = 0;
	for (; count < num; p = p->next)			//׼����һ��������������ݣ����һ��p = NULL
	{
		//printf("TEST+,OK, In the p is:%#x\n",p);
		//system("pause");

		//if (p == 0) break;
		for (i = p->data.Y1; i <= p->data.Y2; i++)
		{
			allWhite = TRUE;	//���赱ǰ���С�ȫ��Ϊ��ɫ���ص�
			for (j = p->data.X1; j <= p->data.X2; j++)
			{
				if (0 == Src[i][j])
				{
					allWhite = FALSE;
					if (cutFlag == FALSE)	//���Ϊ��ɫ���ص� && �иû�п�ʼ
					{
						cutFlag = TRUE;		//���и��־λ����ʼ
						p->data.Y1 = i;		//��¼��ʼYֵ
					}
					else		//����иʼ�ˣ�����ѭ��
					{
						break;
					}
				}//else continue
			
			}//��ѭ������

			if (allWhite && cutFlag)		//�����ǰ���С�ȫ��Ϊ��ɫ���ص�
			{
				cutFlag = FALSE;	//�и��
				p->data.Y2 = i-1;		//��¼����Y����

				count++;			//�и������1
			}

		}//��ѭ������

	}//����ѭ������

	////���������ַ���ͼ������
	//for (count = 0; count<num ; count++)
	//{
	//	x = 0;		//����ͼ�ġ�ָ�롱����
	//	y = 0;

	//	for (i = 0; i<srcHeight; i++)
	//	{
	//		for (j = pos[count*2]; j<pos[count*2+1]; j++)
	//		{
	//			Dst[count][x++][y++] = Src[i][j];
	//		}
	//	}
	//}

	return (rectlink);
}

//��һ�������ַ���ָ����͸ߣ�����������
int StdAlignImg(UCHAR **Dst, UCHAR **Src, int dstHeight, int dstWidth, int srcHeight, int srcWidth, RectLink *rlink, int num)
{
	//int i,j;	//ָ��Դͼ��ı���
	//int k,l;	//ָ��Ŀ��ͼ��ı���
	int m;
	RectLink *p = rlink;		//��rlinkȡ����
	DRect srcRect = {0, 0, 0, 0};		//Դͼ���еľ�������
	DRect dstRect = {0, 0, (STD_W - 1), (STD_H - 1)};		//Ŀ��ͼ���еľ������򣬳�ʼ��,8x16���
	//DRect oldDstRect = dstRect;

	SetImg(Dst, dstHeight, dstWidth, 1);		//��dstȫ�����Ϊ��ɫ
	//printf("After setimg(),before standardImg()!\n");
	//displayImg(Dst, dstHeight, dstWidth);
	//system("pause");

	for (m = 0; m<num; m++)
	{
		srcRect = p->data;		//��ȡԴͼ���������
		/*for (i = p->data.Y1; i < p->data.Y2; i++)
		{
			for (j  = p->data.X1; j < p->data.X2; j++)
			{
				printf("before\n");
				
				printf("after\n");
			}
			printf("I'am change!I'am change!I'am change!\n");
		}*/

		StandardImg(Dst, Src, dstRect, srcRect);	//��srcRect������dstRect

		//printf("longlonglonglonglonglonglonglonglonglonglonglonglonglong\n");

		//����ǰ����д�뵽rlik��
		p->data = dstRect;

		//������һ��ͼ���Ŀ���������
		dstRect.X1 = dstRect.X1 + STD_W;	//���գ�+ 1�Ϳճ�һ�����صĿ��
		dstRect.X2 = dstRect.X1 + STD_W - 1;

		p = p->next;		//׼����һ����������
	}

	return 0;
}

//��׼��ͼ��
int StandardImg(UCHAR **Dst, UCHAR **Src, DRect dstRect, DRect srcRect)
{
	int i = srcRect.Y1, j = srcRect.X1;		//ָ��Դͼ��
	int k = dstRect.Y1, l = dstRect.X1;		//ָ��Ŀ��ͼ��
	int s, t;		//ָ������
	double xScale = 0, yScale = 0;
	int srcHeight = srcRect.Y2 - srcRect.Y1 + 1;		//�õ�Դͼ����
	int srcWidth = srcRect.X2 - srcRect.X1 + 1;
	int dstHeight = dstRect.Y2 - dstRect.Y1 + 1;
	int dstWidth = dstRect.X2 - dstRect.X1 + 1;

	//�õ������ȣ���������ö������Ż�֮��Q16
	yScale = (double ) (srcHeight / (double )dstHeight);
	xScale = (double ) (srcWidth / (double )dstWidth);

	//printf("%d/%d=%0.2lf %d/%d=%0.2lf\n",srcHeight,dstHeight,yScale,srcWidth,dstWidth,xScale);
	//system("pause");

	//ѭ��������ϡ�����
	for (k = dstRect.Y1; k <= dstRect.Y2; k++)
	{
		s = (k - dstRect.Y1);		//����y����
		for (l = dstRect.X1; l <= dstRect.X2; l++)
		{
			
			 t = (l - dstRect.X1);//����x����
			Dst[k][l] = Src[(int )(i + s * yScale + 0.5) ][(int )(j + t * xScale + 0.5) ];
			//�����ã�������ִ���
			/*if ((i + s * yScale + 0.5) > srcRect.Y2)
			{
				printf("in std, y error!\n");
				printf("1 & 2 is:%d %d\n",(int )(i + s * yScale + 0.5),srcRect.Y2);
				system("pause");
			}
			if ((j + t * xScale + 0.5) > srcRect.X2)
			{
				printf("in std, x error!\n");
				printf("1 & 2 is:%d %d,x1=%d\n",(int )(j + t * xScale + 0.5),srcRect.X2,srcRect.X1);
				printf("s:%d and t:%d\n",s,t);
				system("pause");
			}*/
		}
	}

	//after_h = dstHeight;
	//after_w = dstWidth;

	return 0;
}

//������ȡ��13������������ѭ������ʱ�ϳ�
int TZTQ13(UCHAR **tz, UCHAR **Src, int srcHeight, int srcWidth, RectLink *rlink, int num)
{
	//���ǽ�ͼ����Ϊһ��������ȡ
	int i, j;
	int m, a;
	//int te = 0;
	int height,width;
	RectLink *p = rlink;

	//ˮƽ��������ֱ����������ɨ�裬����ɨ��,

	////ˮƽ�������أ�h������
	//for (i = 0; i<srcHeight; i++)
	//{
	//	for (j = 0; j<srcWidth; j++)
	//	{
	//		if (0 == Src[i][j])
	//		{
	//			TZ[i]++;
	//		}
	//	}
	//}

	////��ֱ�������أ�w������
	//for (j = 0; j<srcWidth; j++)
	//{
	//	for (i = 0; i<srcHeight; i++)
	//	{
	//		if (0 == Src[i][j])
	//		{
	//			TZ[srcHeight + j]++;
	//		}
	//	}
	//}

	//��ȡǰ8������
	//for (m = 0; m<8; m++)
	//{
	//	for (i=int(m/2)*8; i<(int(m/2)+1)*8; i++)
	//	{
	//		for (j=m%2*8+k*width;j<(m%2+1)*8+k*width;j++)
	//		{
	//			TZ[m]++;
	//		}
	//	}
	//}


	//��ȡ��9��������������ֵ

	for (m = 0; m<num; m++)
	{
		//�õ���ǰ���ֵĸߺͿ�
		height = p->data.Y2 - p->data.Y1 + 1;
		width = p->data.X2 - p->data.X1 + 1;

		//��ʼ����������
		for (a = 0; a<13; a++)
		{
			tz[m][a] = 0;
		}
		
		//��8����ȡ��ÿ��������ɫ���صĺ���һ������
		for (a = 0; a<8; a++)
		{
			//��ֱ����ֳ�4��
			for (i = p->data.Y1 + (a/2)*height/4; i <= p->data.Y1-1 + (a/2+1)*height/4; i++)
			{
				//ˮƽ����ֳ�2��
				for (j = p->data.X1 + (a%2)*width/2; j <= p->data.X1-1 + (a%2+1)*width/2; j++)
				{
					//printf("p->data.Y1=%d + (a%4)*height/4=%d\n",(a%4)*height/4,p->data.Y1);
					//printf("p->data.X1=%d + (a%2)*width/2=%d\n",(a%2)*width/2,p->data.X1);
					//printf("the cur:(%d, %d)\n",i,j);
					//system("pause");
					if (0 == Src[i][j])
					{
						tz[m][a]++;
					}
					//te++;
				}
			}
			//printf("��ǰ��������Ŀ:%d\n",te);
			//system("pause");
			//te = 0;
		}


		//��ȡ�����غ���Ϊ��8������
		for (i = p->data.Y1; i <= p->data.Y2; i++)
		{
			for (j = p->data.X1; j <= p->data.X2; j++)
			{
				if (0 == Src[i][j])
				{
					tz[m][8]++;
				}
			}
		}

		//ˮƽ����
		i = (int )(height*1/3.0 );
		for (j = p->data.X1; j<p->data.X2; j++)
		{
			if (0 == Src[i][j]) tz[m][9]++;
		}
		
		i = (int )(height*2/3.0 );
		for (j = p->data.X1; j<p->data.X2; j++)
		{
			if (0 == Src[i][j]) tz[m][10]++;
		}

		//��ֱ����
		 j = (int )(width*1/3.0 );
		 for (i = p->data.Y1; i<p->data.Y2; i++)
		 {
			if (0 == Src[i][j]) tz[m][11]++;
		 }

		 j = (int )(width*2/3.0 );
		 for (i = p->data.Y1; i<p->data.Y2; i++)
		 {
			if (0 == Src[i][j]) tz[m][12]++;
		 }

		 p = p->next;	//׼����һ�����־�������

	}//�����ֲ����

	return 0;
}



//��ͼ���࣬0���1,1���0
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
int SetImg(UCHAR **Dst, int srcHeight, int srcWidth, UCHAR tag)
{
	/*UCHAR *p = (UCHAR *)&Dst;
	int i = 0;

	while(p[i] != '\0')
	{
		p[i] = (UCHAR)tag;
		i++;
	}*/
	int i,j;

	for (i = 0; i<srcHeight; i++)
	{
		for (j = 0; j<srcWidth; j++)
		{
			Dst[i][j] = tag;
		}

	}

	return 0;
}

//��ͼ���ֵ��
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

//��ͼ�����ģ�����,tepl ģ�����飬size ģ���С��fac ϵ��
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
				p = &Src[i-size/2][j-size/2];		//��Ŀ����ģ����ʼ���긳��p

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