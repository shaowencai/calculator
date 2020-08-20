#include"shao_cal.h"

#define screen   ((char far *)0xb8000000)
#define show(i)  (160*10+40+(i)*2)
main()
{
	char input;
	int i=0;
	double result;
	char a[100];
	Shao_Calculator *ji_suan_qi=0; 
	system("cls");
	
	ji_suan_qi=(Shao_Calculator *)malloc(sizeof(Shao_Calculator)); 
	Cal_Init(ji_suan_qi);
	while(1)
	{
		input=getch();
		if(! ji_suan_qi->check_illegal(input))
		{	
		    if(input == '.')
			{
				if(i == 0)
				{
					screen[show(i)]='0';
					i++;
				}
				else if(screen[show(i-1)]<'0' || screen[show(i-1)]>'9')
				{
					screen[show(i)]='0';
					i++;
				}
			}
			screen[show(i)]=input;
			if(input=='=')break;
			i++;
        }
		else if(input==0x8)
		{
			if(i>0)
			{
				screen[show(i-1)]=' ';
				i--;
			}
		}
	}
	printf("input char number is:%d\n",i+1);
    for(j=0;j<=i;j++)
		a[j]=screen[show(j)];
	if(!ji_suan_qi->parse(a,0,i+1,ji_suan_qi))
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