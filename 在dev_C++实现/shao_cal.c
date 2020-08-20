/*********************************************************************************
 *�汾�ţ�  shao1.0
 *�������ڣ�2017.10.16
 *���ߣ�    shaowencai
 *ʹ��˵����ֻ����������д����Լ�ע��󶨼��������������ݽӿ�˵���Ϳ���ʹ���ˡ�
 *          �����Ҫ��չ�¹���ʱ�򣬴�1�� 2�� ... 5���������Ƿ���Ҫ����Ӧ������ӡ�
 *          DEV-C++���ı�����ANSI��ʽ  
 *�޸ļ�¼��
 *********************************************************************************/
 
#include"shao_cal.h"

/*��Ҫ�����ⲿ�ĺ�����,��Щ������֧�֣���Щ��֧�� ��֧�ֵ�ʱ���Լ�Ѱַ֧�ֵĿ�Ŷ*/
double atof(const char *);
double pow(double,double);

/*start --------- 1��--------------*/
static double cal_first_method(Shao_Calculator *Calculator); /*��һ�ַ���*/
static double cal_second_method(Shao_Calculator *Calculator);/*�ڶ��ַ���*/ 
static double cal_three_method(Shao_Calculator *Calculator); /*�����ַ���*/
static double cal_four_method(Shao_Calculator *Calculator);  /*�����ַ���*/
/*end------------ 1��--------------*/
 
 
static double get_factor(Shao_Calculator *Calculator);
static void match_and_next(Shao_Calculator *Calculator,char ex);
static double Cal_Result(Shao_Calculator *Calculator);
static int cal_check_error(int count_number,int op_number,const char *ops);
static int cal_check_illegal(char input_char);
static int cal_parse(const char *input_tab,int pos,int size, Shao_Calculator *Calculator);

/*start---------- 2��--------------*/
/*�Ϸ����ַ��б�,ÿ��һ�������㶼������������ϣ��Ա�ʾ֧�����������*/
static const char legal_op_tab[]=
{'+', '-', '*', '/', '%', '^', '|', '&', '=', '.', '(', ')', '\0'};
/*end------------ 2��--------------*/


/*start---------- 3��--------------*/
static double (*cal_method[])(Shao_Calculator *)=
{
	cal_first_method,
	cal_second_method,
	cal_four_method,    
	cal_three_method,
	get_factor
};
/*end------------ 3��--------------*/

/************************************************
�������ܣ�����������������ע��
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

/*start--------- 4��--------------*/
static double cal_first_method(Shao_Calculator *Calculator)      
{
	double temp;
	int i; /*�������浱ǰ��������һ������������ֵ*/
	
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
	int i; /*�������浱ǰ��������һ������������ֵ*/
	
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
	int i; /*�������浱ǰ��������һ������������ֵ*/
	
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
	int i; /*�������浱ǰ��������һ������������ֵ*/
	
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
/*end----------- 4��--------------*/


/******************************************************
�������ܣ��õ���������
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
�������ܣ� ƥ���ַ�Ȼ��������һ���ַ�
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
 *�������ܣ��������������ʽ����
 *����    ��count_numbers  �������ָ���
 *          ops            �洢������ռ��׵�ַ
 *����    ��0 ��ʾ�ɹ�
 *          1 ��ʾ����ƥ��ʧ��
 *		    2 ��ʾ�������ֺ��������ƥ��
 *          3 ��ʾ��count_numbers�������Ϸ�
 **************************************************/
static int cal_check_error(int count_number,int op_number,const char *ops)
{
	int i = 0;
	int match_brackets = 0;/* ƥ������ */
	int count_match_op = 0;/* ����ƥ������� */
	
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
		/*start--------- 5�� --------------*/
		/*��������չ��ѧ�����ϵ��¸�ʽ����*/
		
		/*end----------- 5��--------------*/
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
 *�������ܣ��������������Ƿ�Ϸ�
 *����    ��input ������ַ�
 *����    ��0 ��ʾ�ɹ�
            1 ��ʾ���벻�Ϸ�
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
 *�������ܣ��������һ�����ַ������� ����������ʶ���������ź���������
 *����    ��input_tab ����������׵�ַ
 *          pos �������е�ƫ����
 *          size �ַ���Ŀ
 *          *cal_count_number  �������ĸ���
 *          *cal_op_number  ������ĸ���
 *����    ��0��ʾ �ɹ�
 *          1��ʾ ����')'����'('�Ĵ���
 *			2��ʾ ����һ������������ڶ��С�������
 *			3��ʾ ��������Ƿ�
************************************************************************/
static int cal_parse(const char *input_tab,int pos,int size, Shao_Calculator *Calculator)
{
	int i = 0;
	int k = 0;
	int j = 0;
	int count_number = 0;
	int op_number = 0;
	int find_count_flag = 0; /* 0��ʾ��û���ҵ�����  1��ʾ�ҵ������� */
	int radix_point_number = 0;/* С������� */
	char str[16];
	
	if((size > (MAX_COUNT_NUMBERS + MAX_OP_NUMBERS)) || (!Calculator))
	{
		return 3;
	}
	
	/*��ʼ��һ������*/
	for(j=0; j<16; j++)
	{
		str[j]=0;
	}
	
	for(i=0; i<size; i++)
	{
		if((input_tab[pos+i]<='9'&&input_tab[pos+i]>='0')||input_tab[pos+i]=='.')
		{
			str[k]=input_tab[pos+i];
			/*һ�����������治���Դ�������С����*/
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
			/*��������ͱ�ʾһ������������ ����radix_point_number��0*/
			radix_point_number = 0;
			if(k != 0)/*k != 0 ��ʾstr[]�������Ѿ�����һ��������*/
			{
				Calculator->cal_count_tab[count_number]=atof(str);
				count_number++;
				
				for(j=0; j<16; j++)/*�������һ��*/
				{
					str[j]=0;
				} 
				k=0;
			}
			/*��ʼ������ һ�����Ǹս����������ʱ�̣�һ����������'('*/
			/*�ڿ�ʼ��λ����'+'����'-'������������Զ���ǰ���0 */
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
