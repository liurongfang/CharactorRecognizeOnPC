#include <math.h>
#include "base.h"
#include "bp.h"

//激活函数：S型函数[-1,1]->[0,1]
double squash(double sum)
{
	return 1/(1+exp(-sum));
}


//对图像进行模板操作
int ForwardLayer(double *lay1, double *lay2, double **connWeight, int n1, int n2)		//前向传播
{
	int i,j;
	double sum = 0;
	lay1[0] = 1;	//X0设为1

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

	return 0;
}

//计算输出层误差
int CalcOutError(double *delta, double *output, double *target, int n)			//输出层误差计算
{
	int i;
	double o,t,m_delta;

	for (i = 1; i<=n; i++)
	{
		o = output[i];		//
		m_delta = target[i] - output[i];		//期望输出-实际输出
		delta[i] = o * (1.0 - o) * m_delta;
	}

	return 0;
}


//计算隐含层误差
//delta_h 隐含层误差数组，n_h 隐含层节点数，delta_o 输出层误差数组，n_o 输出层节点个数，weight 连接权值数组，hidden 隐含层节点数组
void CalcHiddErr(double *delta_h, int n_h, double *delta_o, \
				 int n_o, double **weight, double *hidden)			//隐含层误差计算
{
	int i, j;
	double h, errsum;

	//对隐含层的每个节点，计算加权误差和
	for (i = 1; i<=n_h; i++)
	{
		errsum = 0.0;
		h = hidden[i];

		for (j = 1; j<=n_o; j++)
		{
			errsum += delta_o[j] * weight[j][i];		//反向传播：输出节点误差 * 连接权值
		}

		delta_h[j] = h * (1.0 - h) * errsum;
	}

	return 0;
}

//根据当前层节点值、计算误差和前一层节点值，修正连接权
//delta 当前层误差数组，n_delta 当前层节点数，forwd 前一层误差数组，n_fo 前一层节点个数，
//		weight 连接权值数组，oldw 旧连接权数组，lrate 学习率，momentum 动量因子
void AdjustWeight(double *delta, int n_delta, double *forwd, int n_fo, \
				  double **weight, double **oldw, double lrate, double momentum)		//误差反向传播，调整权值
{
	int i, j;
	double new_w = 0;	//保存下一个权值

	//对当前层的每一个节点，计算对前一层的反馈
	for (i = 1; i<=n_delta; i++)
	{
		for (j = 1; j<=n_fo; j++)	//遍历前一层的节点
		{
			//Wij_new=学习因子*输出节点delta*前层节点的计算输出+动量因子*Wij_old
			new_w = lrate*delta[i]*forwd[j] + momentum*oldw[i][j];	
			weight[i][j] += new_w;
			oldw[i][j] = new_w;
		}
	}
}


//训练神经网络
int TrainBpNet(double preci, int num)	//训练BP神经网络
{
	//计算用变量
	int c = 0, k = 0, i = 0;
	double s = 0;

	//网络参数
	int n_in = 13;	//输入层节点数
	int n_hi = 10;	//隐含层节点数
	int n_out = 4;	//输出层节点数
	double learnRate = 0.001;	//学习率

	double **w_input;	//输入层与隐含层之间的权值数组
	double **w_hidden;	//隐含层与输出层之间的权值数组

	double **h_delta;	//隐含层误差
	double **o_delta;	//输出层误差

	double *input;		//输入数组（向量）
	double *hidden;		//隐含层数组
	double *output;		//输出数组

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

	//内存申请


	////调用函数
	//for (c = 0; c<15000; c++)
	//{
	//	for (k = 0; k<num; k++)
	//	{


	//		//正向传播，input->hidden->output
	//		ForwardLayer(input, hidden, w_input, n_in, n_hi);
	//		ForwardLayer(hidden, output, w_hidden, n_hi, n_out);

	//		//计算误差
	//		CalcOutError(o_delta, output, target, n_out);
	//		CalcHiddErr(h_delta, n_hi, o_delta, n_out, w_hidden, hidden);

	//		for (i = 0; i<n_out; i++)
	//		{
	//			s += o_delta[i]*o_delta[i];		//计算方差
	//		}

	//		if (s <= preci)		//如果达到了需要的精度
	//		{
	//			break;
	//		}
	//		else	//调整权值
	//		{
	//			AdjustWeight(h_delta, n_hi, input, n_in, w_input, double **oldw, learnRate, momentum);
	//			AdjustWeight(o_delta, n_out, hidden, n_hi, w_hidden, double **oldw, learnRate, momentum);
	//		}
	//	}//end of num Loop

	//} //end of c Loop


	return 0;
}

//用训练好的神经来识别
int Recongnize()	//用训练好的网络来识别
{

}




