


#include <math.h>
#include <time.h>
#include "base.h"
#include "bp.h"

#pragma pack(2)


//测试输出权值矩阵
void test(double **w, int n1, int n2, double *output, int n_out)
{
	int i,j;
	FILE *fp = fopen("C:\\Users\\Mirgo\\Desktop\\test.txt","w");
	//fseek(fp, 0, 0);

	//system("cls");
	printf("下面打印权值矩阵\n\n");
	for (i = 0; i<=n2; i++)
	{
		for (j = 0; j<=n1; j++)
		{
			fprintf(fp,"%0.4lf",w[i][j]);
			printf("%4.2lf ",w[i][j]);
		}
		fputc('\n',fp);
		printf("\n");
	}

	printf("输出响应：\n");
	for (i = n_out; i>0; i--)
	{
		printf("%4.2lf ",output[i]);
	}
	printf("\n");

	fclose(fp);

	//system("pause");
}

//分配二维数组内存
double **allloc_mem2d_dbl(int height, int width)
{
	int i,j;

	//申请内存
	double **mem = (double **)malloc(sizeof(double *)*height);
	for (i = 0; i<height; i++)
	{
		mem[i] = (double *)malloc(sizeof(double )*width);
	}

	//初始化
	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			mem[i][j] = 0.000001;
		}
	}

	return (mem);
}

//释放二维数组内存
int **detect_mem2d_dbl(double **mem, int height, int width)
{
	int i = 0;

	for (i = 0; i<height; i++)
	{
		free(mem[i]);
	}
	free(mem);

	return 0;
}

//给一维数组申请内存，并初始化
double *alloc_mem1d_dbl(int n)
{
	int i;

	//分配空间
	double *mem = (double *)malloc(sizeof(double )*n);

	//初始化
	for (i = 0; i<n; i++)
	{
		mem[i] = 0.00000001;
	}

	return (mem);
}

//激活函数：S型函数[-1,1]->[0,1]
double squash(double x)
{
	double ans = 0;
	
	ans = 1 /(1 + exp(-x ) );
	//printf("Sqush Ans:%0.4lf\n",ans);

	return ans;
}

//初始化权值矩阵
void InitWeight(double **weight, int n2, int n1, double v)
{
	int i, j;

	for (i = 0; i<n2; i++)
	{
		for (j = 0; j<n1; j++)
		{
			weight[i][j] = v;
		}
	}
}

//随机初始化权值矩阵
void RandomWeight(double **weight, int n2, int n1)
{
	int i, j;
	//double t = 0;

	srand((unsigned )time(NULL));	//设置随机种子

	for (i = 0; i<n2; i++)
	{
		for (j = 0; j<n1; j++)
		{
			weight[i][j] = (double )(rand()/(double)32767 );		//产生的是0到1之间的随机数
		}
	}
}


//信号前向传播，lay1 -> lay2
int ForwardLayer(double *lay1, double *lay2, double **connWeight, int n1, int n2)		//前向传播
{
	int i,j;
	double sum = 0;
	lay1[0] = 0;	//X0设为1，作为偏置
	lay2[0] = 0;	//后一级的X0也置为0

	//对于第二层的每个神经元
	for (i = 1; i<=n2; i++)
	{
		sum = 0;

		//计算输入的加权总和
		for (j = 0; j<=n1; j++)
		{
			sum += lay1[j] * connWeight[i][j];		//O = X*W
		}

		lay2[i] = squash(sum);		//送入激活函数,F1(O)
	}

	//printf("Print lay2i\n");
	//for (i = 0; i<=n2; i++)
	//{
	//	printf("%0.4lf\n",lay2[i]);
	//}

	return 0;
}

//计算输出层误差
int CalcOutError(double *delta, double *output, double *target, int n)			//输出层误差计算
{
	int i;
	double o,m_delta;

	for (i = 1; i<=n; i++)
	{
		o = output[i];		//
		m_delta = target[i-1] - output[i];		//期望输出-实际输出
		delta[i] = o * (1.0 - o) * m_delta;
	}

	return 0;
}


//计算隐含层误差，也为反向传播
//delta_h 隐含层误差数组，n_h 隐含层节点数，delta_o 输出层误差数组，n_o 输出层节点个数，weight 连接权值数组，hidden 隐含层节点数组
void CalcHiddErr(double *delta_h, int n_h, double *delta_o, int n_o, double **weight, double *hidden)			//隐含层误差计算
{
	int i = 0, j = 0;
	double h, errsum;

	//对隐含层的每个节点，计算加权误差和
	for (j = 1; j<=n_h; j++)
	{
		errsum = 0.0;
		h = hidden[j];

		for (i = 1; i<=n_o; i++)
		{
			//fprintf(stderr,"*%d*",j);
			errsum += delta_o[i] * weight[i][j];		//反向传播：输出节点误差 * 连接权值
		}

		delta_h[j] = h * (1.0 - h) * errsum;
	}

}

//根据当前层节点值、计算误差和前一层节点值，修正连接权
//delta 当前层误差数组，n_delta 当前层节点数，forwd 前一层误差数组，n_fo 前一层节点个数，
//		weight 连接权值数组，oldw 旧连接权数组，lrate 学习率，momentum 动量因子
void AdjustWeight(double *delta, int n_delta, double *forwd, int n_fo, double **weight, double **oldw, double lrate, double momentum)		//误差反向传播，调整权值
{
	int i, j;
	double new_w = 0;	//保存下一个权值

	//对当前层的每一个节点，计算对前一层的反馈
	for (i = 1; i<=n_delta; i++)
	{
		for (j = 1; j<=n_fo; j++)	//遍历前一层的节点
		{
			//Wij_new=学习因子*输出节点delta*前层节点的计算输出+动量因子*Wij_old
			new_w = lrate*delta[i]*forwd[j] + momentum*oldw[i][j-1];
			//new_w = delta[i]*1/n_fo;
			//printf("NEW_W:%0.6lf== %0.6lf*%0.6lf*%0.6lf + %0.6lf*%0.6lf\n",new_w,lrate,delta[i],forwd[j],momentum,oldw[i][j-1]);
			//if (new_w == 0)	system("pause");
			weight[i][j] += new_w;
			oldw[i][j] = new_w;
		}
	}
}

//保存权值
void w_weight(double **w,int n1,int n2,char*name)
{
	int i,j;
	//double *buffer;
	FILE *fp;
	fp = fopen(name, "w");
	//buffer = (double *)malloc((n1+1)*(n2+1)*sizeof(double));

	//for(i = 0; i<=n1; i++)
	//{
	//	for(j=0; j<=n2; j++)
	//	{
	//		buffer[i*(n2+1) + j] = w[i][j];		//把权值送入buffer
	//	}
	//}

	//fwrite((char*)buffer,sizeof(double),(n1+1)*(n2+1),fp);

	for (i = 0; i<=n2; i++)
	{
		for (j = 0; j<=n1; j++)
		{
			fprintf(fp,"%0.8lf ",w[i][j]);		//保存数组到fp
		}
		fprintf(fp,"\n");
	}

	fclose(fp);
	//free(buffer);
}



//读取权值
int  r_weight(double **w,int n1,int n2,char *name)
{
	int i,j;
//	double *buffer;
	FILE *fp;

	if((fp=fopen(name,"r"))==NULL)
	{
		fprintf(stderr, "无法读取权值信息");
		return -1;
	}

	//buffer=(double * )malloc((n1+1)*(n2+1)*sizeof(double ));

	////从文件读取
	//fread((char*)buffer,sizeof(double),(n1+1)*(n2+1),fp);

	//for(i=0;i<=n1;i++)
	//{
	//	for(j=0;j<=n2;j++)
	//		w[i][j]=buffer[i*(n2+1)+j];
	//}
	//
	//free(buffer);

	for (i = 0; i<=n2; i++)
	{
		for (j = 0; j<=n1; j++)
		{
			fscanf(fp,"%lf",&w[i][j]);		//从fp读取数据到w数组
		}
		//fseek(fp, 1L, 1);	//从当前位置跳过一个字节
	}

	fclose(fp);
	return 0;
}

//保存各层结点的数目
void w_num(int n1,int n2,int n3,char*name)
{
	int i;
	FILE *fp;
	int *buffer = (int *)malloc(3*sizeof(int ));
	fp=fopen(name,"w");

	//准备数据
	buffer[0]=n1;
	buffer[1]=n2;
	buffer[2]=n3;

	//写数据
	for (i = 0; i<3; i++)
	{
		fprintf(fp, "%4d", buffer[i]);
	}

	//fwrite((char*)buffer,sizeof(int),3,fp);
	fclose(fp);
	free(buffer);
}

//读取各层结点数目
int r_num(int *n,char *name)
{
	int *buffer;
	FILE *fp;
	buffer=(int *)malloc(3*sizeof(int));

	if((fp=fopen(name,"rb")) == NULL)
	{
		fprintf(stderr, "打开文件失败");
		return -1;
	}

	//读取数组
	fread((char*)buffer, sizeof(int), 3, fp);

	//送入buffer
	n[0]=buffer[0];
	n[1]=buffer[1];
	n[2]=buffer[2];

	fclose(fp);
	free(buffer);

	return 0;
}

//继续训练神经网络
int RxTrainBpNet(double ***data_in, int n_in, int n_hi, double min_ex, long max_cyc, double learnRate, int num)
{
	//计算用变量
	long c = 0;
	int k = 0, i = 0, sheet = 0;
	double ex = 0;		//均方误差

	//网络参数
	//int n_in = 13;	//输入层节点数
	//int n_hi = 10;	//隐含层节点数
	int n_out = 4;		//输出层节点数
	//double learnRate = 0.001;	//学习率
	double momentum = 0.001;	//动量因子

	//权值矩阵、误差矩阵、当前层值矩阵变量声明与内存申请
	double **w_input = allloc_mem2d_dbl(n_hi+1, n_in+1);	//输入层与隐含层之间的权值数组
	double **w_hidden = allloc_mem2d_dbl(n_out+1, n_hi+1);	//隐含层与输出层之间的权值数组
	double **w_oldInput = allloc_mem2d_dbl(n_hi+1, n_in+1);		//调整权值时用来保存，输入层与隐含层之间的权值数组
	double **w_oldHidden = allloc_mem2d_dbl(n_out+1, n_hi+1);	//调整权值时用来保存，隐含层与输出层之间的权值数组

	double *h_delta = alloc_mem1d_dbl(n_hi+1);	//隐含层误差
	double *o_delta = alloc_mem1d_dbl(n_out+1);	//输出层误差

	double *input = alloc_mem1d_dbl(n_in+1);		//输入数组（向量）
	double *hidden = alloc_mem1d_dbl(n_hi+1);		//隐含层数组
	double *output = alloc_mem1d_dbl(n_out+1);		//输出数组

	//目标输出
	double target[][4] = {	0.1,0.1,0.1,0.1,	//0
		0.1,0.1,0.1,0.9,	//1
		0.1,0.1,0.9,0.1,	//2
		0.1,0.1,0.9,0.9,	//3
		0.1,0.9,0.1,0.1,	//4
		0.1,0.9,0.1,0.9,	//5
		0.1,0.9,0.9,0.1,	//6
		0.1,0.9,0.9,0.9,	//7
		0.9,0.1,0.1,0.1,	//8
		0.9,0.1,0.1,0.9		//9
	};

	//权值矩阵初始化
	RandomWeight(w_input, n_hi+1, n_in+1);		//前向矩阵初始化
	RandomWeight(w_hidden, n_out+1, n_hi+1);
	InitWeight(w_oldInput, n_hi+1, n_in+1, 0);		//输入层与隐层之间的权值矩阵
	InitWeight(w_oldHidden, n_out+1, n_hi+1, 0);		//隐层与输出层之间的权值矩阵

	//测试用
	//test(w_input, n_in, n_hi, output, n_out);				//测试用
	//test(w_hidden, n_hi, n_out, output, n_out);			//测试用


	//调用函数
	for (c = 0; c<max_cyc; c++)		//不大于最大训练次数
	{
		ex = 0;
		for (sheet = 0; sheet<5; sheet++)
		{
			for (k = 0; k<num; k++)
			{
				//将提取的样本的特征向量输送到输入层上
				for(i=1;i<=n_in;i++)
					input[i] = data_in[sheet][k][i-1];			//将特征向量的单个值输入

				//将预定的理想输出输送到BP网络的理想输出单元
				for(i=1;i<=n_out;i++)
					output[i] = target[k][i-1];		//将理想输出送到输出层

				//前向传输激活
				//正向传播，input->hidden->output
				ForwardLayer(input, hidden, w_input, n_in, n_hi);
				ForwardLayer(hidden, output, w_hidden, n_hi, n_out);

				//printf("after,Print lay\n");
				//for (i = 0; i<=n_out; i++)
				//{
				//	printf("%0.4lf\n",output[i]);
				//}

				//计算误差
				//if (k == 9)
				//{
				//	system("pause");
				//}
				CalcOutError(o_delta, output, (double *)&target[k], n_out);
				CalcHiddErr(h_delta, n_hi, o_delta, n_out, w_hidden, hidden);

				for (i = 1; i<n_out; i++)
				{
					ex += (output[i] - target[k][i-1] ) * (output[i] - target[k][i-1] );		//计算方差
				}

				if (ex <= min_ex)		//如果达到了需要的精度（方差小于预设）
				{
					break;
				}
				else	//调整权值
				{
					//if (k == 8) system("pause");
					AdjustWeight(h_delta, n_hi, input, n_in, w_input, w_oldInput, learnRate, momentum);		//调整输入层与隐含层之间的权值
					//test(w_input, n_in, n_hi, output, n_out);				//测试用
					AdjustWeight(o_delta, n_out, hidden, n_hi, w_hidden, w_oldHidden, learnRate, momentum);	//调整隐含层与输出层之间的权值
					//test(w_hidden, n_hi, n_out, output, n_out);			//测试用
					//if (c%2 == 0)
					//{
					//	printf("数字%d,第%d次\n",k,c);
					//	//system("pause");
					//	//system("cls");
					//	//system("pause");
					//}
				}


			}//num个数字循环结束
		}


		//计算均方误差
		ex = ex/(double)(num*n_out);

		//如果均方误差已经足够的小，跳出循环，训练完毕
		if(ex < min_ex)
		{
			break;
		}

	}

	//训练结束
	if (ex <= min_ex)
	{
		printf("训练完毕,迭代次数：%d次,均方误差：%0.4f\n",c,ex);
	}
	else
	{
		printf("训练失败！迭代次数：%d次,均方误差：%0.4f\n",c,ex);
	}

	////测试用
	//test(w_input, n_in, n_hi, output, n_out);				//测试用
	//test(w_hidden, n_hi, n_out, output, n_out);			//测试用

	//保存权值矩阵
	w_weight(w_input,n_in,n_hi,"C:\\Users\\Mirgo\\Desktop\\win.dat");			//保存输入层与隐层之间的权值
	w_weight(w_hidden,n_hi,n_out,"C:\\Users\\Mirgo\\Desktop\\whi.dat");		//保存隐层与输出层之间的权值

	//保存各层结点的个数
	w_num(n_in,n_hi,n_out,"C:\\Users\\Mirgo\\Desktop\\num.dat");


	//释放内存
	detect_mem2d_dbl(w_input, n_hi+1, n_in+1);
	detect_mem2d_dbl(w_hidden, n_out+1, n_hi+1);
	detect_mem2d_dbl(w_oldInput, n_hi+1, n_in+1);
	detect_mem2d_dbl(w_oldHidden, n_out+1, n_hi+1);
	//detect_mem2d_dbl(h_delta, n_hi+1, n_in+1);
	//detect_mem2d_dbl(o_delta, n_out+1, n_hi+1);
	free(h_delta);
	free(o_delta);
	free(input);
	free(hidden);
	free(output);

	return 0;

}


//训练神经网络，输入层与隐层之间的权值，隐层与输出层之间的权值，
int TrainBpNet(double ** data_in, int n_in, int n_hi, double min_ex, long max_cyc, double learnRate, int num)	//训练BP神经网络
{
	//计算用变量
	long c = 0;
	int k = 0, i = 0;
	//double s = 0;
	double ex = 0;		//均方误差

	//网络参数
	//int n_in = 13;	//输入层节点数
	//int n_hi = 10;	//隐含层节点数
	int n_out = 4;		//输出层节点数
	//double learnRate = 0.001;	//学习率
	double momentum = 0.001;	//动量因子

	//权值矩阵、误差矩阵、当前层值矩阵变量声明与内存申请
	double **w_input = allloc_mem2d_dbl(n_hi+1, n_in+1);	//输入层与隐含层之间的权值数组
	double **w_hidden = allloc_mem2d_dbl(n_out+1, n_hi+1);	//隐含层与输出层之间的权值数组
	double **w_oldInput = allloc_mem2d_dbl(n_hi+1, n_in+1);		//调整权值时用来保存，输入层与隐含层之间的权值数组
	double **w_oldHidden = allloc_mem2d_dbl(n_out+1, n_hi+1);	//调整权值时用来保存，隐含层与输出层之间的权值数组

	double *h_delta = alloc_mem1d_dbl(n_hi+1);	//隐含层误差
	double *o_delta = alloc_mem1d_dbl(n_out+1);	//输出层误差

	double *input = alloc_mem1d_dbl(n_in+1);		//输入数组（向量）
	double *hidden = alloc_mem1d_dbl(n_hi+1);		//隐含层数组
	double *output = alloc_mem1d_dbl(n_out+1);		//输出数组

	//目标输出
	double target[][4] = {	0.1,0.1,0.1,0.1,	//0
							0.1,0.1,0.1,0.9,	//1
							0.1,0.1,0.9,0.1,	//2
							0.1,0.1,0.9,0.9,	//3
							0.1,0.9,0.1,0.1,	//4
							0.1,0.9,0.1,0.9,	//5
							0.1,0.9,0.9,0.1,	//6
							0.1,0.9,0.9,0.9,	//7
							0.9,0.1,0.1,0.1,	//8
							0.9,0.1,0.1,0.9		//9
						};

	//权值矩阵初始化
	RandomWeight(w_input, n_hi+1, n_in+1);		//前向矩阵初始化
	RandomWeight(w_hidden, n_out+1, n_hi+1);
	InitWeight(w_oldInput, n_hi+1, n_in+1, 0);		//输入层与隐层之间的权值矩阵
	InitWeight(w_oldHidden, n_out+1, n_hi+1, 0);		//隐层与输出层之间的权值矩阵

	//测试用
	//test(w_input, n_in, n_hi, output, n_out);				//测试用
	//test(w_hidden, n_hi, n_out, output, n_out);			//测试用


	//调用函数
	for (c = 0; c<max_cyc; c++)
	{
		ex = 0;
		for (k = 0; k<num; k++)
		{
			//将提取的样本的特征向量输送到输入层上
			for(i=1;i<=n_in;i++)
				input[i] = data_in[k][i-1];			//将特征向量的单个值输入

			//将预定的理想输出输送到BP网络的理想输出单元
			for(i=1;i<=n_out;i++)
				output[i] = target[k][i-1];		//将理想输出送到输出层

			//前向传输激活
			//正向传播，input->hidden->output
			ForwardLayer(input, hidden, w_input, n_in, n_hi);
			ForwardLayer(hidden, output, w_hidden, n_hi, n_out);

			//printf("after,Print lay\n");
			//for (i = 0; i<=n_out; i++)
			//{
			//	printf("%0.4lf\n",output[i]);
			//}

			//计算误差
			//if (k == 9)
			//{
			//	system("pause");
			//}
			CalcOutError(o_delta, output, (double *)&target[k], n_out);
			CalcHiddErr(h_delta, n_hi, o_delta, n_out, w_hidden, hidden);

			for (i = 1; i<n_out; i++)
			{
				ex += (output[i] - target[k][i-1] ) * (output[i] - target[k][i-1] );		//计算方差
			}

			if (ex <= min_ex)		//如果达到了需要的精度（方差小于预设）
			{
				break;
			}
			else	//调整权值
			{
				//if (k == 8) system("pause");
				AdjustWeight(h_delta, n_hi, input, n_in, w_input, w_oldInput, learnRate, momentum);		//调整输入层与隐含层之间的权值
				//test(w_input, n_in, n_hi, output, n_out);				//测试用
				AdjustWeight(o_delta, n_out, hidden, n_hi, w_hidden, w_oldHidden, learnRate, momentum);	//调整隐含层与输出层之间的权值
				//test(w_hidden, n_hi, n_out, output, n_out);			//测试用
				//if (c%2 == 0)
				//{
				//	printf("数字%d,第%d次\n",k,c);
				//	//system("pause");
				//	//system("cls");
				//	//system("pause");
				//}
			}

		}//数字循环结束
		//计算均方误差
		ex = ex/(double)(num*n_out);

		//如果均方误差已经足够的小，跳出循环，训练完毕
		if(ex < min_ex)
		{
			break;
		}

	}

	//训练结束
	if (ex <= min_ex)
	{
		printf("训练完毕,迭代次数：%d次,均方误差：%0.6f\n",c,ex);
	}
	else
	{
		printf("训练失败！迭代次数：%d次,均方误差：%0.6f\n",c,ex);
	}

	////测试用
	//test(w_input, n_in, n_hi, output, n_out);				//测试用
	//test(w_hidden, n_hi, n_out, output, n_out);			//测试用

	//保存权值矩阵
	w_weight(w_input,n_in,n_hi,"C:\\Users\\Mirgo\\Desktop\\win.dat");			//保存输入层与隐层之间的权值
	w_weight(w_hidden,n_hi,n_out,"C:\\Users\\Mirgo\\Desktop\\whi.dat");		//保存隐层与输出层之间的权值

	//保存各层结点的个数
	w_num(n_in,n_hi,n_out,"C:\\Users\\Mirgo\\Desktop\\num.dat");


	//释放内存
	detect_mem2d_dbl(w_input, n_hi+1, n_in+1);
	detect_mem2d_dbl(w_hidden, n_out+1, n_hi+1);
	detect_mem2d_dbl(w_oldInput, n_hi+1, n_in+1);
	detect_mem2d_dbl(w_oldHidden, n_out+1, n_hi+1);
	//detect_mem2d_dbl(h_delta, n_hi+1, n_in+1);
	//detect_mem2d_dbl(o_delta, n_out+1, n_hi+1);
	free(h_delta);
	free(o_delta);
	free(input);
	free(hidden);
	free(output);

	return 0;
}

//用训练好的神经网络来识别
int NumRecongnize(double **data_in, int n_in, int n_hi, int num)	//用训练好的网络来识别
{
	//循环变量声明
	int i;
	int k = 0;
	int result = 0;
	int *recognize = (int *)malloc(sizeof(int )*num);

	FILE *fp = NULL;

	//网络参数
	int n_out = 4;

	//值矩阵声明与内存分配
	double *input = (double *)malloc(sizeof(double)*(n_in+1));
	double *hidden = (double *)malloc(sizeof(double)*(n_hi+1));
	double *output = (double *)malloc(sizeof(double)*(n_out+1));		//输出用8421编码，4位二进制数

	//权值矩阵声明与分配
	double **w_input = allloc_mem2d_dbl(n_hi+1, n_in+1);
	double **w_hidden = allloc_mem2d_dbl(n_out+1, n_hi+1);

	//读取权值
	if( 0 != r_weight(w_input, n_in,n_hi, "C:\\Users\\Mirgo\\Desktop\\win.dat"))
	{
		return -1;
	}
	if( 0 != r_weight(w_hidden, n_hi, n_out, "C:\\Users\\Mirgo\\Desktop\\whi.dat"))
	{
		return -1;
	}

	//保存权值矩阵，测试读入是否正确
	w_weight(w_input,n_in,n_hi,"C:\\Users\\Mirgo\\Desktop\\win2.dat");			//保存输入层与隐层之间的权值
	w_weight(w_hidden,n_hi,n_out,"C:\\Users\\Mirgo\\Desktop\\whi2.dat");		//保存隐层与输出层之间的权值

	for (k = 0; k<num; k++)
	{
		result = 0;

		//将提取的样本的特征向量输送到输入层上
		for(i=1;i<=n_in;i++)
			input[i]=data_in[k][i-1];

		//前向输入激活
		ForwardLayer(input, hidden, w_input, n_in, n_hi);		//输入层与隐含层之间
		ForwardLayer(hidden, output, w_hidden, n_hi, n_out);		//隐含层与输出层之间

		for (i = 1; i<=n_out; i++)
		{
			//如果大于0.5判为1
			if(output[i] >= 0.5)
			{
				result += (int )(pow(2, (double)(4-i))  );
				printf("1");
			}
			else
			{
				printf("0");
			}

		}

		printf("    %d\n",result);

		//如果判定的结果小于等于9，认为合理
		if(result <= 9)
		{
			recognize[k] = result;
		}

		//如果判定的结果大于9，认为不合理将结果定位为一个特殊值20
		if(result > 9)
		{
			recognize[k] = 20;
		}
	}//数字循环结束

	//识别结束

	//将识别结果写入文件，并在屏幕上显示出来
	fp=fopen("result.txt","w+");

	printf("识别结果：\n");
	for (i = 0; i<num; i++)
	{
		if (recognize[i] == 20)
		{
			printf("N");
			fprintf(fp, "N", recognize[i]);
		}
		else
		{
			printf("%d",recognize[i]);
			fprintf(fp, "%d",recognize[i]);
		}
	}

	printf("\n");

	//释放内存
	detect_mem2d_dbl(w_input, n_hi+1, n_in+1);
	detect_mem2d_dbl(w_hidden, n_out+1, n_hi+1);
	free(recognize);
	free(input);
	free(hidden);
	free(output);

	return result;
}




