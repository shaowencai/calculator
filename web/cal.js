(function() {

    var that;

    function Calculator_T() {
        this.cal_count_tab = [];    /*用于存放用户输入的运算因子的**/
        this.cal_op_tab = [];       /*用于存放用户输入的运算符号的**/
        this.op = '';               /*计算过程中临时存放当前运算符号**/
        this.count_number = 0;      /*表示运算因子的数目*/
        this.op_number = 0;         /*表示运算符号的数目*/
        this.method_idex = 0;      /*运算方法调用的索引值*/
        this.num_f = 0;             /*用于运算因子数组下标动态移动*/
        this.num_c = 0;            /*用于运算符号数组下标动态移动*/
        that = this;
    }

    Calculator_T.prototype.check_error = cal_check_error;
    Calculator_T.prototype.check_illegal = cal_check_illegal;
    Calculator_T.prototype.parse = cal_parse;
    Calculator_T.prototype.Cal_Result = Cal_Result;

    var cal_method = [
        cal_first_method,
        cal_second_method,
        cal_four_method,
        cal_three_method,
        get_factor
    ];


    function Cal_Result()
    {
        var temp;
        this.op = this.cal_op_tab[0];
        this.method_idex = 0;
        temp = cal_method[this.method_idex]();
        this.num_c = 0;
        this.num_f = 0;
        return temp;
    }

    /*start--------- 4区--------------*/
    function  cal_first_method()
    {
        var temp;
        var i; /*用来保存当前函数的下一个函数的索引值*/

        that.method_idex++;
        i=that.method_idex;
        temp=cal_method[i]();

        while(that.op=='+'||that.op=='-')
        {
            that.method_idex = i;
            switch(that.op)
            {
                case '+':match_and_next('+');
                    temp += cal_method[i]();
                    break;
                case '-':match_and_next('-');
                    temp -= cal_method[i]();
                    break;
            }
        }
        return temp;
    }

    function cal_second_method()
    {
        var temp;
        var i; /*用来保存当前函数的下一个函数的索引值*/

        that.method_idex++;
        i=that.method_idex;
        temp=cal_method[i]();

        while(that.op=='*'||that.op=='/'||that.op=='%')
        {
            that.method_idex = i;
            switch(that.op)
            {
                case '*':match_and_next('*');
                    temp *= cal_method[i]();
                    break;
                case '/':match_and_next('/');
                    temp /= cal_method[i]();
                    break;
                case '%':match_and_next('%');
                    temp= temp % cal_method[i]();
                    break;
            }
        }
        return temp;
    }

    function cal_three_method()
    {
        var temp;
        var i; /*用来保存当前函数的下一个函数的索引值*/

        that.method_idex++;
        i=that.method_idex;
        temp=cal_method[i]();

        while(that.op=='^')
        {
            that.method_idex = i;
            switch(that.op)
            {
                case '^':match_and_next('^');
                    temp=Math.pow(temp,cal_method[i]());
                    break;
            }
        }
        return temp;
    }
    function cal_four_method()
    {
        var temp;
        var i; /*用来保存当前函数的下一个函数的索引值*/

        that.method_idex++;
        i=that.method_idex;
        temp=cal_method[i]();

        while(that.op=='|' || that.op=='&')
        {
            that.method_idex = i;
            switch(that.op)
            {
                case '|':match_and_next('|');
                    temp=temp | cal_method[i]();
                    break;
                case '&':match_and_next('&');
                    temp=temp & cal_method[i]();
                    break;
            }
        }
        return temp;
    }
    /*end----------- 4区--------------*/


    /******************************************************
     函数功能：得到运算因子
     ******************************************************/
    function get_factor()
    {
        var temp;
        if(that.op=='(')
        {
            match_and_next('(');
            that.method_idex = 0;
            temp=cal_method[that.method_idex]();
            match_and_next(')');
        }
        else
        {
            temp=that.cal_count_tab[that.num_f];
            that.num_f++;
        }
        return temp;
    }

    /************************************************************
     函数功能： 匹配字符然后跳到下一个字符
     *************************************************************/
    function match_and_next(ex)
    {
        if(ex==that.op)
        {
            that.num_c++;
            that.op=that.cal_op_tab[that.num_c];
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
    function cal_check_error(count_number,op_number,ops)
    {
        var i = 0;
        var match_brackets = 0;/* 匹配括号 */
        var count_match_op = 0;/* 数字匹配运算符 */


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
    function cal_check_illegal(input_char)
    {
        var i = 0;

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
     *返回    ：0表示 成功
     *          1表示 发生')'相邻'('的错误
     *			2表示 发生一个运算数里存在多个小数点错误
     *			3表示 输入参数非法
     ************************************************************************/
    function cal_parse(input_tab,pos,size)
    {
        var i = 0;
        var k = 0;
        var j = 0;
        var count_number = 0;
        var op_number = 0;
        var find_count_flag = 0; /* 0表示还没有找到数字  1表示找到数字了 */
        var radix_point_number = 0;/* 小数点个数 */
        var str = [];


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
                    this.cal_count_tab[count_number]=parseFloat(str.join(""));
                    count_number++;

                    str.length = 0;
                    k=0;
                }
                /*开始有两个 一个就是刚进入计算器的时刻，一个就是遇到'('*/
                /*在开始的位置是'+'或者'-'，本处理就是自动在前面加0 */
                else if(find_count_flag == 0 &&(input_tab[pos+i]=='+'||input_tab[pos+i]=='-'))
                {
                    if(str.length == 0)
                    {
                        this.cal_count_tab[count_number]=0;
                    }
                    else{
                        this.cal_count_tab[count_number]=parseFloat(str.join(""));
                    }

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
                    this.cal_op_tab[op_number]=input_tab[pos+i];
                    op_number++;
                }
            }
        }
        this.count_number = count_number;
        this.op_number = op_number;
        return 0;
    }

    window.Calculator_T = Calculator_T;
})();