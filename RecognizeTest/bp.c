#include <math.h>
#include "base.h"
#include "bp.h"

//�����ά�����ڴ�
double **allloc_mem2d_dbl(int height, int width)
{
	int i;
	double **mem = (double **)malloc(sizeof(double *)*height);

	for (i = 0; i<height; i++)
	{
		mem[i] = (double *)malloc(sizeof(double )*width);
	}

	return (mem);
}

//�ͷŶ�ά�����ڴ�
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

//�������S�ͺ���[-1,1]->[0,1]
double squash(double x)
{
	return 1/(1+exp(-x));
}

//��ʼ��Ȩֵ����
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


//�ź�ǰ�򴫲���lay1 -> lay2
int ForwardLayer(double *lay1, double *lay2, double **connWeight, int n1, int n2)		//ǰ�򴫲�
{
	int i,j;
	double sum = 0;
	lay1[0] = 1;	//X0��Ϊ1

	//���ڵڶ����ÿ����Ԫ
	for (i = 1; i<=n2; i++)
	{
		sum = 0;

		//��������ļ�Ȩ�ܺ�
		for (j = 0; j<=n1; j++)
		{
			sum += lay1[j] * connWeight[i][j];		//O = X*W
		}

		lay2[i] = squash(sum);		//���뼤���,F1(O)
	}

	return 0;
}

//������������
int CalcOutError(double *delta, double *output, double *target, int n)			//�����������
{
	int i;
	double o,m_delta;

	for (i = 1; i<=n; i++)
	{
		o = output[i];		//
		m_delta = target[i] - output[i];		//�������-ʵ�����
		delta[i] = o * (1.0 - o) * m_delta;
	}

	return 0;
}


//������������ҲΪ���򴫲�
//delta_h ������������飬n_h ������ڵ�����delta_o �����������飬n_o �����ڵ������weight ����Ȩֵ���飬hidden ������ڵ�����
void CalcHiddErr(double *delta_h, int n_h, double *delta_o, int n_o, double **weight, double *hidden)			//������������
{
	int i, j;
	double h, errsum;

	//���������ÿ���ڵ㣬�����Ȩ����
	for (j = 1; j<=n_h; j++)
	{
		errsum = 0.0;
		h = hidden[j];

		for (i = 1; i<=n_o; j++)
		{
			errsum += delta_o[i] * weight[i][j];		//���򴫲�������ڵ���� * ����Ȩֵ
		}

		delta_h[j] = h * (1.0 - h) * errsum;
	}

}

//���ݵ�ǰ��ڵ�ֵ����������ǰһ��ڵ�ֵ����������Ȩ
//delta ��ǰ��������飬n_delta ��ǰ��ڵ�����forwd ǰһ��������飬n_fo ǰһ��ڵ������
//		weight ����Ȩֵ���飬oldw ������Ȩ���飬lrate ѧϰ�ʣ�momentum ��������
void AdjustWeight(double *delta, int n_delta, double *forwd, int n_fo, double **weight, double **oldw, double lrate, double momentum)		//���򴫲�������Ȩֵ
{
	int i, j;
	double new_w = 0;	//������һ��Ȩֵ

	//�Ե�ǰ���ÿһ���ڵ㣬�����ǰһ��ķ���
	for (i = 1; i<=n_delta; i++)
	{
		for (j = 1; j<=n_fo; j++)	//����ǰһ��Ľڵ�
		{
			//Wij_new=ѧϰ����*����ڵ�delta*ǰ��ڵ�ļ������+��������*Wij_old
			new_w = lrate*delta[i]*forwd[j] + momentum*oldw[i][j];	
			weight[i][j] += new_w;
			oldw[i][j] = new_w;
		}
	}
}

//����Ȩֵ
void w_weight(double **w,int n1,int n2,char*name)
{
	int i,j;
	double *buffer;
	FILE *fp;
	fp = fopen(name, "wb+");
	buffer=(double*)malloc((n1+1)*(n2+1)*sizeof(double));
	for(i = 0; i<=n1; i++)
	{
		for(j=0; j<=n2; j++)
		{
			buffer[i*(n2+1) + j] = w[i][j];		//��Ȩֵ����buffer
		}
	}
	fwrite((char*)buffer,sizeof(double),(n1+1)*(n2+1),fp);
	fclose(fp);
	free(buffer);
}



//��ȡȨֵ
int  r_weight(double **w,int n1,int n2,char *name)
{
	int i,j;
	double *buffer;
	FILE *fp;

	if((fp=fopen(name,"rb"))==NULL)
	{
		fprintf(stderr, "�޷���ȡȨֵ��Ϣ");
		return -1;
	}

	buffer=(double*)malloc((n1+1)*(n2+1)*sizeof(double));

	//���ļ���ȡ
	fread((char*)buffer,sizeof(double),(n1+1)*(n2+1),fp);

	for(i=0;i<=n1;i++)
	{
		for(j=0;j<=n2;j++)
			w[i][j]=buffer[i*(n2+1)+j];
	}
	fclose(fp);
	free(buffer);

	return 0;
}

//������������Ŀ
void w_num(int n1,int n2,int n3,char*name)
{
	FILE *fp;
	int *buffer = (int *)malloc(3*sizeof(int ));
	fp=fopen(name,"wb+");

	//׼������
	buffer[0]=n1;
	buffer[1]=n2;
	buffer[2]=n3;

	fwrite((char*)buffer,sizeof(int),3,fp);
	fclose(fp);
	free(buffer);
}

//��ȡ��������Ŀ
int r_num(int *n,char *name)
{
	int *buffer;
	FILE *fp;
	buffer=(int *)malloc(3*sizeof(int));

	if((fp=fopen(name,"rb")) == NULL)
	{
		fprintf(stderr, "���ļ�ʧ��");
		return -1;
	}

	//��ȡ����
	fread((char*)buffer, sizeof(int), 3, fp);

	//����buffer
	n[0]=buffer[0];
	n[1]=buffer[1];
	n[2]=buffer[2];

	fclose(fp);
	free(buffer);

	return 0;
}


//ѵ�������磬�����������֮���Ȩֵ�������������֮���Ȩֵ��
int TrainBpNet(double ** data_in, double** data_out, int n_in, int n_hi, double min_ex, double learnRate, int num)	//ѵ��BP������
{
	//�����ñ���
	int c = 0, k = 0, i = 0;
	//double s = 0;
	double ex = 0;		//�������

	//�������
	//int n_in = 13;	//�����ڵ���
	//int n_hi = 10;	//������ڵ���
	int n_out = 4;	//�����ڵ���
	//double learnRate = 0.001;	//ѧϰ��
	double momentum = 0.1;	//��������

	//Ȩֵ���������󡢵�ǰ��ֵ��������������ڴ�����
	double **w_input = allloc_mem2d_dbl(n_hi+1, n_in+1);	//�������������֮���Ȩֵ����
	double **w_hidden = allloc_mem2d_dbl(n_out+1, n_hi+1);	//�������������֮���Ȩֵ����
	double **w_oldInput = allloc_mem2d_dbl(n_hi+1, n_in+1);		//����Ȩֵʱ�������棬�������������֮���Ȩֵ����
	double **w_oldHidden = allloc_mem2d_dbl(n_out+1, n_hi+1);	//����Ȩֵʱ�������棬�������������֮���Ȩֵ����

	double *h_delta = (double *)malloc(sizeof(double )*(n_hi+1));	//���������
	double *o_delta = (double *)malloc(sizeof(double )*(n_out+1));	//��������

	double *input = (double *)malloc(sizeof(double)*(n_in+1));		//�������飨������
	double *hidden = (double *)malloc(sizeof(double)*(n_hi+1));		//����������
	double *output = (double *)malloc(sizeof(double)*(n_out+1));		//�������

	//Ŀ�����
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

	//Ȩֵ�����ʼ��
	InitWeight(w_input, n_hi+1, n_in+1, 0.5);		//�����������֮���Ȩֵ����
	InitWeight(w_hidden, n_out+1, n_hi+1, 0.5);		//�����������֮���Ȩֵ����


	//���ú���
	for (c = 0; c<15000; c++)
	{
		ex = 0;
		for (k = 0; k<num; k++)
		{
			//����ȡ�������������������͵��������
			for(i=1;i<=n_in;i++)
				input[i] = data_in[k][i-1];			//�����������ĵ���ֵ����

			//��Ԥ��������������͵�BP��������������Ԫ
			for(i=1;i<=n_out;i++)
				output[i] = target[k][i-1];		//����������ε������

			//ǰ���伤��
			//���򴫲���input->hidden->output
			ForwardLayer(input, hidden, w_input, n_in, n_hi);
			ForwardLayer(hidden, output, w_hidden, n_hi, n_out);

			//�������
			CalcOutError(o_delta, output, (double *)&target[k], n_out);
			CalcHiddErr(h_delta, n_hi, o_delta, n_out, w_hidden, hidden);

			for (i = 1; i<n_out; i++)
			{
				ex += (output[i] - target[k][i-1] ) * (output[i] - target[k][i-1] );		//���㷽��
			}

			if (ex <= min_ex)		//����ﵽ����Ҫ�ľ��ȣ�����С��Ԥ�裩
			{
				break;
			}
			else	//����Ȩֵ
			{
				AdjustWeight(h_delta, n_hi, input, n_in, w_input, w_oldInput, learnRate, momentum);		//�����������������֮���Ȩֵ
				AdjustWeight(o_delta, n_out, hidden, n_hi, w_hidden, w_oldHidden, learnRate, momentum);	//�����������������֮���Ȩֵ
			}

		}//����ѭ������
		//����������
		ex = ex/(double)(num*n_out);

		//�����������Ѿ��㹻��С������ѭ����ѵ�����  
		if(ex<min_ex)break;

	}

	//ѵ������

	//����Ȩֵ����

	//���������������֮���Ȩֵ
	w_weight(w_input,n_in,n_hi,"win.dat");
	//���������������֮���Ȩֵ
	w_weight(w_hidden,n_hi,n_out,"whi.dat");

	//���������ĸ���
	w_num(n_in,n_hi,n_out,"num");


	//�ͷ��ڴ�
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

//��ѵ���õ�����ʶ��
int Recongnize(double **data_in, int n_in, int n_hi, int num)	//��ѵ���õ�������ʶ��
{
	//ѭ����������
	int i;
	int k = 0;
	int result = 0;
	int *recognize = (int *)malloc(sizeof(int )*num);

	FILE *fp = NULL;

	//�������
	int n_out = 4;

	//ֵ�����������ڴ����
	double *input = (double *)malloc(sizeof(double)*(n_in+1));
	double *hidden = (double *)malloc(sizeof(double)*(n_hi+1));
	double *output = (double *)malloc(sizeof(double)*(n_out+1));		//�����8421���룬4λ��������

	//Ȩֵ�������������
	double **w_input = allloc_mem2d_dbl(n_hi+1, n_in+1);
	double **w_hidden = allloc_mem2d_dbl(n_out+1, n_hi+1);

	//��ȡȨֵ
	if( 0 != r_weight(w_input, n_in,n_hi, "win.dat"))
	{
		return -1;
	}
	if( 0 != r_weight(w_hidden, n_hi, n_out, "whi.dat"))
	{
		return -1;
	}

	for (k = 0; k<num; k++)
	{
		//����ȡ�������������������͵��������
		for(i=1;i<=n_in;i++)
			input[i]=data_in[k][i-1];

		//ǰ�����뼤��
		ForwardLayer(input, hidden, w_input, n_in, n_hi);		//�������������֮��
		ForwardLayer(hidden, output, w_hidden, n_hi, n_out);		//�������������֮��

		for (i = 1; i<n_out; i++)
		{
			//�������0.5��Ϊ1
			if(output[i] > 0.5)
			{
				result += (int )(pow(2, (double)(4-i)) );
			}
		}

		//����ж��Ľ��С�ڵ���9����Ϊ����
		if(result <= 9)
		{
			recognize[k] = result;
		}

		//����ж��Ľ������9����Ϊ�����������λΪһ������ֵ20
		if(result > 9)
		{
			recognize[k] = 20;
		}
	}//����ѭ������

	//ʶ�����

	//��ʶ����д���ļ���������Ļ����ʾ����
	fp=fopen("result.txt","w+");

	printf("ʶ������\n");
	for (i = 0; i<num; i++)
	{
		if (recognize[i] == 20)
		{
			printf("ʶ��ʧ��");
			fprintf(fp, "ʶ��ʧ��", recognize[i]);
		}
		else
		{
			printf("%d",recognize[i]);
			fprintf(fp, "%d",recognize[i]);
		}
	}

	//�ͷ��ڴ�
	detect_mem2d_dbl(w_input, n_hi+1, n_in+1);
	detect_mem2d_dbl(w_hidden, n_out+1, n_hi+1);
	free(recognize);
	free(input);
	free(hidden);
	free(output);

	return result;
}




