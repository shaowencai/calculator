#ifndef _SHAO_CAL_H
#define _SHAO_CAL_H

#define MAX_COUNT_NUMBERS  50
#define MAX_OP_NUMBERS     50 


/* DEV-C++中文编码是ANSI格式 */ 
struct Calculator_T
{
	double cal_count_tab[MAX_COUNT_NUMBERS]; /*用于存放用户输入的运算因子的**/
    char   cal_op_tab[MAX_OP_NUMBERS];       /*用于存放用户输入的运算符号的**/
	char   op;                               /*计算过程中临时存放当前运算符号**/
	int    count_number;                     /*表示运算因子的数目*/
	int    op_number;                        /*表示运算符号的数目*/
	int    method_idex;                      /*运算方法调用的索引值*/
	int    num_f;                            /*用于运算因子数组下标动态移动*/
	int    num_c;                            /*用于运算符号数组下标动态移动*/
	int    (*check_error)(int,int,const char *);/* 检验用户输入一套完整运算后格式是否正确**/
	int    (*check_illegal)(char);            /*检验是否支持用户输入的字符**/
	/*将用户输入的一串字符解析转换为一串运算因子和一串运算符号*/
	int    (*parse)(const char *,int,int,struct Calculator_T *);
	double (*Cal_Result)(struct Calculator_T *);/*用户调用此函数可直接得出运算结果**/
};
typedef  struct Calculator_T  Shao_Calculator;

extern void Cal_Init(struct Calculator_T *);/*计算器驱动的注册绑定**/

#endif
