

/**************************************************************************************
*	����������ļ��Ƕ���BP�����������صĺ�����������
*	Copy(c) Rights By RongfangLiu,2014-2019
*	All Rights Reserved. ������ע������
*	��ϵ�����뷢�ʼ�����752444247@qq.com
**************************************************************************************/

#ifndef _MY_BPNET_
#define _MY_BPNET_

//���ýṹ��2���ֽڶ���
#pragma pack(2)

//�ڴ���亯��
double **allloc_mem2d_dbl(int height, int width);		//��mybmp.h�ж���

//�ͷŶ�ά�����ڴ�
int **detect_mem2d_dbl(double **mem, int height, int width);

//�������S�ͺ���[-1,1]->[0,1]
double squash(double x);

//��ʼ��Ȩֵ����
void InitWeight(double **weight, int n2, int n1, double v);

//�����ʼ��Ȩֵ����-1��1
void RandomWeight(double **weight, int n2, int n1);

/*************************************************
��������: ForwardLayer()
����:  ��ͼ�����ģ�����
����: lay1 ǰ���ڵ����飬lay2 �󼶽ڵ����飬connWeight ����Ȩֵ��n1 ǰ���ڵ������n2 �󼶽ڵ����
���: 
����ֵ: ��������ֵ 0
д������: 2014/01/29 
*************************************************/
int ForwardLayer(double *lay1, double *lay2, double **connWeight, int n1, int n2);		//ǰ�򴫲�

/*************************************************
��������: CalcOutError()
����:  ������������
����: delta ������飬��� ����ڵ����飬target Ŀ�������n �����ڵ����
���: 
����ֵ: ��������ֵ 0
д������: 2014/01/29 
*************************************************/

//������������
int CalcOutError(double *delta, double *output, double *target, int n);			//�����������

/*************************************************
��������: CalcHiddErr()
����:  �������������
����: delta_h ������������飬n_h ������ڵ�����delta_o �����������飬n_o �����ڵ������weight ����Ȩֵ���飬hidden ������ڵ�����
���: 
����ֵ: ��������ֵ 0
д������: 2014/01/29 
*************************************************/

void CalcHiddErr(double *delta_h, int n_h, double *delta_o, int n_o, double **weight, double *hidden);			//������������
					
/*************************************************
��������: AdjustWeight()
����:  ���ݵ�ǰ��ڵ�ֵ����������ǰһ��ڵ�ֵ����������Ȩ
����: delta ��ǰ��������飬n_delta ��ǰ��ڵ�����forwd ǰһ��������飬n_fo ǰһ��ڵ������
		weight ����Ȩֵ���飬oldw ������Ȩ���飬lrate ѧϰ�ʣ�momentum ��������
���: 
����ֵ: ��������ֵ 0
д������: 2014/01/29 
*************************************************/

void AdjustWeight(double *delta, int n_delta, double *forwd, int n_fo, \
				  double **weight, double **oldw, double lrate, double momentum);		//���򴫲�������Ȩֵ

//����Ȩֵ����
void w_weight(double **w,int n1,int n2,char*name);

//��ȡȨֵ����
int r_weight(double **w,int n1,int n2,char *name);

//����ڵ���Ŀ
void w_num(int n1,int n2,int n3,char*name);

//��ȡ�ڵ���Ŀ
int r_num(int *n,char *name);



//���ѵ��������
int RxTrainBpNet(double ***data_in, int n_in, int n_hi, double min_ex, long max_cyc, double learnRate, int num);
				
				
/*************************************************
��������: TrainBpNet()
����:  ѵ��������
����: preci ��ﵽ�ľ���
���: Dst Ŀ��ͼ���ָ��
����ֵ: ��������ֵ 0
д������: 2014/01/29 
*************************************************/
//ѵ�������磬�����������֮���Ȩֵ�������������֮���Ȩֵ
int TrainBpNet(double ** data_in, int n_in, int n_hi, double min_ex, long max_cyc, double learnRate, int num);

/*************************************************
��������: TrainBpNet()
����:  ��ͼ�����ģ�����
����: Src ָ��Դͼ���ָ�룬srcWidth Դͼ��Ŀ�ȣ�srcHeight Դͼ��ĸ߶ȣ�tepl ģ�����飬size ģ���С��fac ϵ��
���: Dst Ŀ��ͼ���ָ��
����ֵ: ��������ֵ 0
д������: 2014/01/29 
*************************************************/
int NumRecongnize(double **data_in, int n_in, int n_hi, int num);	//��ѵ���õ�������ʶ��

//�������Ȩֵ����
//void test(double **w, int n1, int n2, double *output, int n_out);


#endif


