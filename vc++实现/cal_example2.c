/* DEV-C++中文编码是ANSI格式 */ 
#include"shao_cal.h"
#include <stdio.h>
#include <stdlib.h>
main()
{
	char input;
	int i=0;
	double result;
	char cal_str[100];
	Shao_Calculator *ji_suan_qi=(Shao_Calculator *)malloc(sizeof(Shao_Calculator)); 
	Cal_Init(ji_suan_qi);
	for(i=0;i<100; i++)
	{
		cal_str[i]=0;
	}
	i=0;
	while(1)
	{
		input=getch();
		/**因为不支持'(' ')' '^' 所以分别用 'o' 'p' 't'代替 **/ 
		switch(input)
		{
			case 'o':input='(';break;
			case 'p':input=')';break;
			case 't':input='^';break;
			default:           break;
		} 
		if(! ji_suan_qi->check_illegal(input))
		{	
		    if(input == '.')
			{
				if(i == 0)
				{
					cal_str[i]='0';
					printf("%c",cal_str[i]);
					i++;
				}
				else if(cal_str[i-1]<'0' || cal_str[i-1]>'9')
				{
					cal_str[i]='0';
					printf("%c",cal_str[i]);
					i++;
				}
			}
			cal_str[i]=input;
			printf("%c",cal_str[i]);
			if(input=='=')break;
			i++;
        }
	}
	printf("\ninput char number is:%d\n",i+1);
	if(!ji_suan_qi->parse(cal_str,0,i+1,ji_suan_qi))
	{
		printf("count:%d\n",ji_suan_qi->count_number);
		printf("op:%d\n",ji_suan_qi->op_number);
		printf("op_tab:%s\n",ji_suan_qi->cal_op_tab);
		if(!ji_suan_qi->check_error(
		ji_suan_qi->count_number,ji_suan_qi->op_number,ji_suan_qi->cal_op_tab))
		{
			result = ji_suan_qi->Cal_Result(ji_suan_qi);
			printf("the result is:%f",result); 
		}
		else{ printf("geshi error\r\n");}
	}
	else{ printf("parse error\r\n");}
	free(ji_suan_qi);
}

