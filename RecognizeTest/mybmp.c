
#include "mybmp.h"

//给二维数组分配内存空间，用来保存图像
UCHAR **allloc_mem2d(int height, int width)
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

//初始化二维数组
int init_mem2d(UCHAR **img, int height, int width, UCHAR tag)
{
	int i,j;

	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			img[i][j] = tag;
		}
		
	}

	return 0;
}

//释放二维数组占用的内存空间，若直接用free(mem)会造成内存泄露
void delete_mem2d(UCHAR **mem, int height, int width)
{
	int i;

	for ( i = 0; i<height; i++)
	{
		//逐个释放
		free(mem[i]);
	}
	free(mem);
	mem = NULL;
}

//读取图像
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
		//出错处理
		printf("Cann't Open the BMP File!\nPlease Check the Filename!\n");
		exit(0);
	}

	fseek(fp, 0, 0);
	fread(&fileHeader, sizeof(fileHeader), 1, fp);
	fread(&infoHeader, sizeof(infoHeader), 1, fp);

	//计算并输出位图数据的偏移量，图像的大小，宽度和高度，每个像素点所占的字节
	size = fileHeader.bSize;
	offset = fileHeader.bOffset;
	bmpImageSize = infoHeader.bmpImageSize;
	mwidth = infoHeader.bWidth;
	mheight = infoHeader.bHeight;
	bitCount = infoHeader.bBitCount;
	bytesPerPixel = infoHeader.bBitCount / 8;

	//printf("%c %d %d %d %d %d %d %d\n",fileHeader.bType, size, offset, bmpImageSize, mwidth, mheight, bitCount, bytesPerPixel);
	//printf("%d %d\n",sizeof(fileHeader),sizeof(infoHeader));

	//输出每个像素点所占字节中的内容
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

//显示图像
void displayImg(UCHAR **image, int height, int width)
{
	int i,j;
	int adjust1 = 1, adjust2 = 1;

	if (width > 64)
	{
		adjust1 = width/64;
	}

	if (height >64)
	{
		adjust2 = height/64;
	}

	for (i = 0; i<height; i = i+adjust2)
	{
		for (j = 0; j<width; j = j+adjust1)	//这里跳点采样，以适合cmd显示
		{
			/*if (0 == image[i][j] )
			{
				printf("%c",0);
			}
			else
			{
				printf("%c",2);
			}*/

			printf("%d",image[i][j]);
		}
		printf("\n");
	}

	system("pause");
}

//保存图像到TXT文件
int saveImg(UCHAR **image, int height, int width, char *filename)
{
	FILE *fp = NULL;
	int i, j, k = 0;
	int adjust1 = 1,adjust2 = 1;
	int count = 128;
	//UCHAR c

	if (width > count)
	{
		adjust1 = width/count;		//确定跳点采样比例
	}
	
	if (height > count)
	{
		adjust2 = height/count;
	}

	fp = fopen(filename,"w");

	//fwrite(image, sizeof(UCHAR), 64*64, fp);

	for (i = 0; i<height; i = i+adjust2)
	{
		for (j = 0; j<width; j = j+adjust1)
		{
			//输出到文件
			if (0 == image[i][j] )
			{
				fputc(' ', fp);
			}
			else
			{
				fputc('1', fp);
			}
			
			k++;
		}
		//一行完成，换行
		fputc('\n',fp);
	}

	//printf("高度:%d & 宽度:%d",i/adjust2,j/adjust1);

	//printf("调用saveImg()保存完成，像素点：%d个\n",k);
	//system("pause");

	fclose(fp);

	//system("pause");
	
	return 0;
}

//完成BGR到GRAY的转换
UCHAR BGR2GRAY(UCHAR a, UCHAR b, UCHAR c)
{
	return (UCHAR)((a*11 + b*59 + c*30 + 50)/100);
}