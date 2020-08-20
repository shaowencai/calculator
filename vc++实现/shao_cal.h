#ifndef _SHAO_CAL_H
#define _SHAO_CAL_H

#define MAX_COUNT_NUMBERS  50
#define MAX_OP_NUMBERS     50 


/* DEV-C++���ı�����ANSI��ʽ */ 
struct Calculator_T
{
	double cal_count_tab[MAX_COUNT_NUMBERS]; /*���ڴ���û�������������ӵ�**/
    char   cal_op_tab[MAX_OP_NUMBERS];       /*���ڴ���û������������ŵ�**/
	char   op;                               /*�����������ʱ��ŵ�ǰ�������**/
	int    count_number;                     /*��ʾ�������ӵ���Ŀ*/
	int    op_number;                        /*��ʾ������ŵ���Ŀ*/
	int    method_idex;                      /*���㷽�����õ�����ֵ*/
	int    num_f;                            /*�����������������±궯̬�ƶ�*/
	int    num_c;                            /*����������������±궯̬�ƶ�*/
	int    (*check_error)(int,int,const char *);/* �����û�����һ������������ʽ�Ƿ���ȷ**/
	int    (*check_illegal)(char);            /*�����Ƿ�֧���û�������ַ�**/
	/*���û������һ���ַ�����ת��Ϊһ���������Ӻ�һ���������*/
	int    (*parse)(const char *,int,int,struct Calculator_T *);
	double (*Cal_Result)(struct Calculator_T *);/*�û����ô˺�����ֱ�ӵó�������**/
};
typedef  struct Calculator_T  Shao_Calculator;

extern void Cal_Init(struct Calculator_T *);/*������������ע���**/

#endif
