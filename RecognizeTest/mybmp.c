
#include "mybmp.h"

//����ά��������ڴ�ռ䣬��������ͼ��
UCHAR **allloc_mem(int height, int width)
{
	UCHAR **img;
	int i = 0;

	img = (UCHAR **)malloc( (unsigned)(height * sizeof(UCHAR *) ) );
	for (; i<height; i++)
	{
		img[i] = (UCHAR *)malloc( (unsigned)(width * sizeof(UCHAR)));
	}

	if (img == NULL)
	{
		printf("alloc memory failed!");
		exit(0);
	}

	return (img);
}

//�ͷŶ�ά����ռ�õ��ڴ�ռ䣬��ֱ����free(mem)������ڴ�й¶
void delete_mem(UCHAR **mem, int height, int width)
{
	int i;

	for ( i = 0; i<height; i++)
	{
		//����ͷ�
		free(mem[i]);
	}
	free(mem);
	mem = NULL;
}
//��ȡͼ��
int readBmp(UCHAR **image, int height, int width, char *filename)
{
	FILE *fp;
	UCHAR *temp;
	UCHAR c, cl[3];
	BMPFILEHEADER fileHeader;
	BMPINF infoHeader;
	long offset, bmpImageSize, mwidth, mheight, bytesPerPixel, size, bitCount;
	int i = 0;
	int count = 0, l = 0,r = 0,en = 0;

	temp = (UCHAR*)malloc(sizeof(UCHAR)*height*width*3);

	if ((fp = fopen(filename,"rb")) == NULL)
	{
		//������
		printf("Cann't Open the BMP File!\nPlease Check the Filename!\n");
		exit(0);
	}

	fseek(fp, 0, 0);
	fread(&fileHeader, sizeof(fileHeader), 1, fp);
	fread(&infoHeader, sizeof(infoHeader), 1, fp);

	//���㲢���λͼ���ݵ�ƫ������ͼ��Ĵ�С����Ⱥ͸߶ȣ�ÿ�����ص���ռ���ֽ�
	size = fileHeader.bSize;
	offset = fileHeader.bOffset;
	bmpImageSize = infoHeader.bmpImageSize;
	mwidth = infoHeader.bWidth;
	mheight = infoHeader.bHeight;
	bitCount = infoHeader.bBitCount;
	bytesPerPixel = infoHeader.bBitCount / 8;

	//printf("%c %d %d %d %d %d %d %d\n",fileHeader.bType, size, offset, bmpImageSize, mwidth, mheight, bitCount, bytesPerPixel);
	//printf("%d %d\n",sizeof(fileHeader),sizeof(infoHeader));

	//���ÿ�����ص���ռ�ֽ��е�����
	c = fgetc(fp);
	while (!feof(fp))
	{
		//printf("%x", c==0xff?1:c);
		c = fgetc(fp);
		cl[l++] = c;
		if (l == 3)
		{
			temp[count] = BGR2GRAY(cl[0],cl[1],cl[2]);
			l = 0;
			count++;
		}

		if (count % 64 == 0)
		{
			//printf("\n");
		}
		
		r++;
	}

	//printf("\n%d %d\n",count,r);


	count = 0;
	for (i = height-1; i>=0; i--)
	{
		for (l = 0; l<width; l++)
		{
			image[i][l] = temp[count];
			count++;
		}
	}
	

	
	fclose(fp);

	//system("pause");

	return 0;
}

//��ʾͼ��
void displayImg(UCHAR **image, int height, int width)
{
	int i,j;
	int adjust = 1;

	if (width > 64)
	{
		adjust = width/64;
	}

	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j = j+adjust)	//����������������ʺ�cmd��ʾ
		{
			if (image[i][j] == 0)
			{
				printf("%c",0);
			}
			else
			{
				printf("%c",2);
			}
		}
		printf("\n");
	}
}

//����ͼ��TXT�ļ�
int saveImg(UCHAR **image, int height, int width, char *filename)
{
	FILE *fp = NULL;
	int i, j, k = 0;
	int adjust = 1;

	if (width > 128)
	{
		adjust = width/128;		//ȷ�������������
	}
	//UCHAR c;

	fp = fopen(filename,"w");

	//fwrite(image, sizeof(UCHAR), 64*64, fp);

	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j = j+adjust)
		{
			//������ļ�
			
			(image[i][j] == 0)?fputc(0, fp):fputc(2, fp);
			k++;
		}
		//һ����ɣ�����
		fputc('\n',fp);
	}

	//printf("����saveImg()������ɣ����ص㣺%d��\n",k);

	fclose(fp);

	//system("pause");
	
	return 0;
}

//���BGR��GRAY��ת��
UCHAR BGR2GRAY(UCHAR a, UCHAR b, UCHAR c)
{
	return (UCHAR)((a*11 + b*59 + c*30 + 50)/100);
}