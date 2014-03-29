

/**************************************************************************************
*	描述：这个文件是对与BP神经网络计算相关的函数进行声明
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. 引用需注明出处
*	联系作者请发邮件至：752444247@qq.com
**************************************************************************************/

#ifndef _MY_BPNET_
#define _MY_BPNET_

//设置结构体2个字节对齐
#pragma pack(2)

//内存分配函数
double **allloc_mem2d_dbl(int height, int width);		//在mybmp.h中定义

//释放二维数组内存
int **detect_mem2d_dbl(double **mem, int height, int width);

//激活函数：S型函数[-1,1]->[0,1]
double squash(double x);

//初始化权值矩阵
void InitWeight(double **weight, int n2, int n1, double v);

//随机初始化权值矩阵，-1到1
void RandomWeight(double **weight, int n2, int n1);

/*************************************************
函数名称: ForwardLayer()
描述:  对图像进行模板操作
输入: lay1 前级节点数组，lay2 后级节点数组，connWeight 连接权值，n1 前级节点个数，n2 后级节点个数
输出: 
返回值: 正常返回值 0
写作日期: 2014/01/29 
*************************************************/
int ForwardLayer(double *lay1, double *lay2, double **connWeight, int n1, int n2);		//前向传播

/*************************************************
函数名称: CalcOutError()
描述:  计算输出层误差
输入: delta 误差数组，输出 输出节点数组，target 目标输出，n 输出层节点个数
输出: 
返回值: 正常返回值 0
写作日期: 2014/01/29 
*************************************************/

//计算输出层误差
int CalcOutError(double *delta, double *output, double *target, int n);			//输出层误差计算

/*************************************************
函数名称: CalcHiddErr()
描述:  计算隐含层误差
输入: delta_h 隐含层误差数组，n_h 隐含层节点数，delta_o 输出层误差数组，n_o 输出层节点个数，weight 连接权值数组，hidden 隐含层节点数组
输出: 
返回值: 正常返回值 0
写作日期: 2014/01/29 
*************************************************/

void CalcHiddErr(double *delta_h, int n_h, double *delta_o, int n_o, double **weight, double *hidden);			//隐含层误差计算
					
/*************************************************
函数名称: AdjustWeight()
描述:  根据当前层节点值、计算误差和前一层节点值，修正连接权
输入: delta 当前层误差数组，n_delta 当前层节点数，forwd 前一层误差数组，n_fo 前一层节点个数，
		weight 连接权值数组，oldw 旧连接权数组，lrate 学习率，momentum 动量因子
输出: 
返回值: 正常返回值 0
写作日期: 2014/01/29 
*************************************************/

void AdjustWeight(double *delta, int n_delta, double *forwd, int n_fo, \
				  double **weight, double **oldw, double lrate, double momentum);		//误差反向传播，调整权值

//保存权值矩阵
void w_weight(double **w,int n1,int n2,char*name);

//读取权值矩阵
int r_weight(double **w,int n1,int n2,char *name);

//保存节点数目
void w_num(int n1,int n2,int n3,char*name);

//读取节点数目
int r_num(int *n,char *name);



//多次训练神经网络
int RxTrainBpNet(double ***data_in, int n_in, int n_hi, double min_ex, long max_cyc, double learnRate, int num);
				
				
/*************************************************
函数名称: TrainBpNet()
描述:  训练神经网络
输入: preci 想达到的精度
输出: Dst 目标图像的指针
返回值: 正常返回值 0
写作日期: 2014/01/29 
*************************************************/
//训练神经网络，输入层与隐层之间的权值，隐层与输出层之间的权值
int TrainBpNet(double ** data_in, int n_in, int n_hi, double min_ex, long max_cyc, double learnRate, int num);

/*************************************************
函数名称: TrainBpNet()
描述:  对图像进行模板操作
输入: Src 指向源图像的指针，srcWidth 源图像的宽度，srcHeight 源图像的高度，tepl 模板数组，size 模板大小，fac 系数
输出: Dst 目标图像的指针
返回值: 正常返回值 0
写作日期: 2014/01/29 
*************************************************/
int NumRecongnize(double **data_in, int n_in, int n_hi, int num);	//用训练好的网络来识别

//测试输出权值矩阵
//void test(double **w, int n1, int n2, double *output, int n_out);


#endif


