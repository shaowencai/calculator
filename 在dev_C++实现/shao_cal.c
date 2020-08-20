/*********************************************************************************
 *版本号：  shao1.0
 *生产日期：2017.10.16
 *作者：    shaowencai
 *使用说明：只需在你程序中创建以及注册绑定计算器驱动，根据接口说明就可以使用了。
 *          如果需要扩展新功能时候，从1区 2区 ... 5区处分析是否需要在相应区间添加。
 *          DEV-C++中文编码是ANSI格式  
 *修改记录：
 *********************************************************************************/
 
#include"shao_cal.h"

/*需要调用外部的函数库,有些编译器支持，有些不支持 不支持的时候自己寻址支持的库哦*/
double atof(const char *);
double pow(double,double);

/*start --------- 1区--------------*/
static double cal_first_method(Shao_Calculator *Calculator); /*第一种方法*/
static double cal_second_method(Shao_Calculator *Calculator);/*第二种方法*/ 
static double cal_three_method(Shao_Calculator *Calculator); /*第三种方法*/
static double cal_four_method(Shao_Calculator *Calculator);  /*第四种方法*/
/*end------------ 1区--------------*/
 
 
static double get_factor(Shao_Calculator *Calculator);
static void match_and_next(Shao_Calculator *Calculator,char ex);
static double Cal_Result(Shao_Calculator *Calculator);
static int cal_check_error(int count_number,int op_number,const char *ops);
static int cal_check_illegal(char input_char);
static int cal_parse(const char *input_tab,int pos,int size, Shao_Calculator *Calculator);

/*start---------- 2区--------------*/
/*合法的字符列表,每增一个新运算都必须在这里加上，以表示支持新运算符号*/
static const char legal_op_tab[]=
{'+', '-', '*', '/', '%', '^', '|', '&', '=', '.', '(', ')', '\0'};
/*end------------ 2区--------------*/


/*start---------- 3区--------------*/
static double (*cal_method[])(Shao_Calculator *)=
{
	cal_first_method,
	cal_second_method,
	cal_four_method,    
	cal_three_method,
	get_factor
};
/*end------------ 3区--------------*/

/************************************************
函数功能：将计算器方法函数注册
************************************************/
void Cal_Init(Shao_Calculator *Calculator)
{
	int i=0;
	if(!Calculator)
	{
		return;
	}
	for(i=0; i<MAX_COUNT_NUMBERS; i++)
	{
		Calculator->cal_count_tab[i]=0;
	}
	for(i=0; i<MAX_OP_NUMBERS; i++)
	{
		Calculator->cal_op_tab[i]=0;
	}
	Calculator->op = '\0';
	Calculator->count_number = 0;
	Calculator->op_number = 0;
	Calculator->num_c = '\0';
    Calculator->num_f = 0;
	Calculator->method_idex=-1;
	Calculator->check_error = cal_check_error;
	Calculator->check_illegal = cal_check_illegal;
	Calculator->parse = cal_parse;	
	Calculator->Cal_Result = Cal_Result;
}

double Cal_Result(Shao_Calculator *Calculator)
{
   double temp;
   if(!Calculator)
   {
	   return 0;
   }
   Calculator->op = Calculator->cal_op_tab[0];
   Calculator->method_idex++;
   temp = cal_method[Calculator->method_idex](Calculator);
   Calculator->num_c = '\0';
   Calculator->num_f = 0;
   return temp;
}

/*start--------- 4区--------------*/
static double cal_first_method(Shao_Calculator *Calculator)      
{
	double temp;
	int i; /*用来保存当前函数的下一个函数的索引值*/
	
    Calculator->method_idex++;
	i=Calculator->method_idex;
	temp=cal_method[i](Calculator);
	
	while(Calculator->op=='+'||Calculator->op=='-')
	{
	    Calculator->method_idex = i;
		switch(Calculator->op)
		{
			case '+':match_and_next(Calculator,'+'); 
					temp+=cal_method[i](Calculator);
					break;
			case '-':match_and_next(Calculator,'-'); 
		            temp-=cal_method[i](Calculator);
					break;		
		}
	}
	return temp;
}

static double cal_second_method(Shao_Calculator *Calculator)           
{
    double temp;
	double temp1;
	int i; /*用来保存当前函数的下一个函数的索引值*/
	
    Calculator->method_idex++;
	i=Calculator->method_idex;
	temp=cal_method[i](Calculator);
	
	while(Calculator->op=='*'||Calculator->op=='/'||Calculator->op=='%')
	{
		Calculator->method_idex = i;
		switch(Calculator->op)
		{
			case '*':match_and_next(Calculator,'*'); 
					temp*=cal_method[i](Calculator);
					break;
			case '/':match_and_next(Calculator,'/'); 
		            temp1=cal_method[i](Calculator);
		            if(temp1>-0.000000000000000001 && temp1<0.0000000000000000001)
		            {
		            	exit(1);
					}
					temp=temp/temp1;
					break;		
			case '%':match_and_next(Calculator,'%'); 
		            temp=((long int)temp)%((long int)cal_method[i](Calculator));
					break;						
		}
	}
	return temp;
}

static double cal_three_method(Shao_Calculator *Calculator)           
{
    double temp;
	int i; /*用来保存当前函数的下一个函数的索引值*/
	
    Calculator->method_idex++;
	i=Calculator->method_idex;
	temp=cal_method[i](Calculator);
	
	while(Calculator->op=='^')
	{
		Calculator->method_idex = i;
		switch(Calculator->op)
		{
			case '^':match_and_next(Calculator,'^');
					temp=pow(temp,cal_method[i](Calculator));
					break;		
		}
	}
	return temp;
}
static double cal_four_method(Shao_Calculator *Calculator)           
{
    double temp;
	int i; /*用来保存当前函数的下一个函数的索引值*/
	
    Calculator->method_idex++;
	i=Calculator->method_idex;
	temp=cal_method[i](Calculator);
	
	while(Calculator->op=='|' || Calculator->op=='&')
	{
		Calculator->method_idex = i;
		switch(Calculator->op)
		{
			case '|':match_and_next(Calculator,'|');
					temp=((long int)temp)|((long int)cal_method[i](Calculator));
					break;	
			case '&':match_and_next(Calculator,'&');
					temp=((long int)temp)&((long int)cal_method[i](Calculator));
					break;	
		}
	}
	return temp;
}
/*end----------- 4区--------------*/


/******************************************************
函数功能：得到运算因子
******************************************************/
static double get_factor(Shao_Calculator *Calculator)
{
	double temp;
	if(Calculator->op=='(')   
	{
		match_and_next(Calculator,'(');
		Calculator->method_idex = 0;
		temp=cal_method[Calculator->method_idex](Calculator);
		match_and_next(Calculator,')');
	}
	else
	{
		temp=Calculator->cal_count_tab[Calculator->num_f];
		Calculator->num_f++;
	}
	return temp;
}

/************************************************************
函数功能： 匹配字符然后跳到下一个字符
*************************************************************/
static void match_and_next(Shao_Calculator *Calculator,char ex)
{
	if(ex==Calculator->op)
	{
		Calculator->num_c++;
		Calculator->op=Calculator->cal_op_tab[Calculator->num_c];
	}
}

/***************************************************
 *函数功能：检测计算器输入格式错误
 *参数    ：count_numbers  运算数字个数
 *          ops            存储运算符空间首地址
 *返回    ：0 表示成功
 *          1 表示括号匹配失败
 *		    2 表示运算数字和运算符不匹配
 *          3 表示：count_numbers参数不合法
 **************************************************/
static int cal_check_error(int count_number,int op_number,const char *ops)
{
	int i = 0;
	int match_brackets = 0;/* 匹配括号 */
	int count_match_op = 0;/* 数字匹配运算符 */
	
	if(count_number > MAX_COUNT_NUMBERS || op_number > MAX_OP_NUMBERS)
	{
		return 3;
	}
	
	for(i=0; i<op_number; i++)
	{
		if(ops[i] == '(')
		{
			match_brackets++;
		}
		else if((ops[i] == ')') && (match_brackets != 0))
		{
			match_brackets--;
		}
		else count_match_op++;
		/*start--------- 5区 --------------*/
		/*这块可以扩展数学运算上的新格式检验*/
		
		/*end----------- 5区--------------*/
	}
	
	if(match_brackets != 0)
	{	
		return 1;
	}		
	
	for(i=0; i<count_number; i++)
	{
		count_match_op--;
	}
	if(count_match_op != 0) 
	{
		return 2;
	}
	
	return 0;		
}

/*******************************************
 *函数功能：检测计算器输入是否合法
 *参数    ：input 输入的字符
 *返回    ：0 表示成功
            1 表示输入不合法
*******************************************/
static int cal_check_illegal(char input_char)
{
	int i = 0;
	
	if((input_char <= '9' && input_char >= '0'))
	{
		return 0;
	}
	
	for(i=0; legal_op_tab[i]; i++)
	{
		if(input_char == legal_op_tab[i])
		{
			return 0;
		}
	}
	
	return 1;
}

/***********************************************************************
 *函数功能：将输入的一连串字符解析成 计算器可以识别的运算符号和运算数字
 *参数    ：input_tab 输入数组的首地址
 *          pos 在数组中的偏移量
 *          size 字符数目
 *          *cal_count_number  运算数的个数
 *          *cal_op_number  运算符的个数
 *返回    ：0表示 成功
 *          1表示 发生')'相邻'('的错误
 *			2表示 发生一个运算数里存在多个小数点错误
 *			3表示 输入参数非法
************************************************************************/
static int cal_parse(const char *input_tab,int pos,int size, Shao_Calculator *Calculator)
{
	int i = 0;
	int k = 0;
	int j = 0;
	int count_number = 0;
	int op_number = 0;
	int find_count_flag = 0; /* 0表示还没有找到数字  1表示找到数字了 */
	int radix_point_number = 0;/* 小数点个数 */
	char str[16];
	
	if((size > (MAX_COUNT_NUMBERS + MAX_OP_NUMBERS)) || (!Calculator))
	{
		return 3;
	}
	
	/*初始化一下数组*/
	for(j=0; j<16; j++)
	{
		str[j]=0;
	}
	
	for(i=0; i<size; i++)
	{
		if((input_tab[pos+i]<='9'&&input_tab[pos+i]>='0')||input_tab[pos+i]=='.')
		{
			str[k]=input_tab[pos+i];
			/*一个运算数里面不可以存在两个小数点*/
			if(str[k] == '.')
			{
				radix_point_number++;
			}
			if(radix_point_number > 1)
			{
				return 2;
			}	
			k++;
			find_count_flag = 1;
		}
		
		else
		{
			/*进来这里就表示一个运算数完了 所以radix_point_number清0*/
			radix_point_number = 0;
			if(k != 0)/*k != 0 表示str[]数组里已经放了一个运算数*/
			{
				Calculator->cal_count_tab[count_number]=atof(str);
				count_number++;
				
				for(j=0; j<16; j++)/*用完清空一下*/
				{
					str[j]=0;
				} 
				k=0;
			}
			/*开始有两个 一个就是刚进入计算器的时刻，一个就是遇到'('*/
			/*在开始的位置是'+'或者'-'，本处理就是自动在前面加0 */
			else if(find_count_flag == 0 &&(input_tab[pos+i]=='+'||input_tab[pos+i]=='-'))
			{
				Calculator->cal_count_tab[count_number]=atof(str);
				count_number++;
			}
			if(input_tab[pos+i]=='(')
			{
				find_count_flag = 0;
			}
			if(input_tab[pos+i]=='(' && input_tab[pos+i-1]==')')
			{
				return 1;
			}
			else
			{
				Calculator->cal_op_tab[op_number]=input_tab[pos+i];
				op_number++;
			}
		}
	}
	Calculator->count_number = count_number;
	Calculator->op_number = op_number;
	return 0;
}
