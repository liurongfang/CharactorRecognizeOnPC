#include <math.h>
#include "base.h"
#include "bp.h"

//�������S�ͺ���[-1,1]->[0,1]
double squash(double sum)
{
	return 1/(1+exp(-sum));
}


//��ͼ�����ģ�����
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
	double o,t,m_delta;

	for (i = 1; i<=n; i++)
	{
		o = output[i];		//
		m_delta = target[i] - output[i];		//�������-ʵ�����
		delta[i] = o * (1.0 - o) * m_delta;
	}

	return 0;
}


//�������������
//delta_h ������������飬n_h ������ڵ�����delta_o �����������飬n_o �����ڵ������weight ����Ȩֵ���飬hidden ������ڵ�����
void CalcHiddErr(double *delta_h, int n_h, double *delta_o, \
				 int n_o, double **weight, double *hidden)			//������������
{
	int i, j;
	double h, errsum;

	//���������ÿ���ڵ㣬�����Ȩ����
	for (i = 1; i<=n_h; i++)
	{
		errsum = 0.0;
		h = hidden[i];

		for (j = 1; j<=n_o; j++)
		{
			errsum += delta_o[j] * weight[j][i];		//���򴫲�������ڵ���� * ����Ȩֵ
		}

		delta_h[j] = h * (1.0 - h) * errsum;
	}

	return 0;
}

//���ݵ�ǰ��ڵ�ֵ����������ǰһ��ڵ�ֵ����������Ȩ
//delta ��ǰ��������飬n_delta ��ǰ��ڵ�����forwd ǰһ��������飬n_fo ǰһ��ڵ������
//		weight ����Ȩֵ���飬oldw ������Ȩ���飬lrate ѧϰ�ʣ�momentum ��������
void AdjustWeight(double *delta, int n_delta, double *forwd, int n_fo, \
				  double **weight, double **oldw, double lrate, double momentum)		//���򴫲�������Ȩֵ
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


//ѵ��������
int TrainBpNet(double preci, int num)	//ѵ��BP������
{
	//�����ñ���
	int c = 0, k = 0, i = 0;
	double s = 0;

	//�������
	int n_in = 13;	//�����ڵ���
	int n_hi = 10;	//������ڵ���
	int n_out = 4;	//�����ڵ���
	double learnRate = 0.001;	//ѧϰ��

	double **w_input;	//�������������֮���Ȩֵ����
	double **w_hidden;	//�������������֮���Ȩֵ����

	double **h_delta;	//���������
	double **o_delta;	//��������

	double *input;		//�������飨������
	double *hidden;		//����������
	double *output;		//�������

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

	//�ڴ�����


	////���ú���
	//for (c = 0; c<15000; c++)
	//{
	//	for (k = 0; k<num; k++)
	//	{


	//		//���򴫲���input->hidden->output
	//		ForwardLayer(input, hidden, w_input, n_in, n_hi);
	//		ForwardLayer(hidden, output, w_hidden, n_hi, n_out);

	//		//�������
	//		CalcOutError(o_delta, output, target, n_out);
	//		CalcHiddErr(h_delta, n_hi, o_delta, n_out, w_hidden, hidden);

	//		for (i = 0; i<n_out; i++)
	//		{
	//			s += o_delta[i]*o_delta[i];		//���㷽��
	//		}

	//		if (s <= preci)		//����ﵽ����Ҫ�ľ���
	//		{
	//			break;
	//		}
	//		else	//����Ȩֵ
	//		{
	//			AdjustWeight(h_delta, n_hi, input, n_in, w_input, double **oldw, learnRate, momentum);
	//			AdjustWeight(o_delta, n_out, hidden, n_hi, w_hidden, double **oldw, learnRate, momentum);
	//		}
	//	}//end of num Loop

	//} //end of c Loop


	return 0;
}

//��ѵ���õ�����ʶ��
int Recongnize()	//��ѵ���õ�������ʶ��
{

}




