window.onload=function(){
    var i = 0,flag = 0;
    var Cal = new Calculator_T();
    var display;
    var button =  document.getElementsByClassName("openCal")[0];
    var div =  document.getElementsByClassName("cal")[0];
    var table = document.getElementsByTagName("table")[0];
    var close  = document.getElementsByTagName("span")[0];
    //var input = document.getElementsByTagName("input")[0];
    //var input1 = document.getElementsByTagName("input")[1];
    var input = document.getElementsByTagName("textarea")[0];
    input.readOnly = true;
    window.selected = false;
    var screen = [];



    close.addEventListener("mouseover",function() {
        close.style.background="red";
    },true);

    close.addEventListener("mouseout",function() {
        close.style.setProperty("background","#eeddcc");
    },true);

    close.addEventListener("click",function() {
        div.style.setProperty("display","");
    },true);


    button.onclick=function(){
      display = div.style.getPropertyValue("display");
      if(display == "") {
          div.style.setProperty("display","block");
      }
    };

    table.onclick=function(e){
        if(e.target.className == "lable" || e.target.className == "lable"){
            return;
		}
		
		if(e.target.innerText.length > 2)
		{
			return;
		}

        if ( e.target.innerText == 'CE') {
            i = 0;
            screen.length = 0;
            flag = 0;
            //input.setAttribute("value", "0");
            input.innerHTML="0";
            input.scrollTop = 0;
        }
        else if(flag == 0) {
            if (e.target.innerText == '<') {
                if (i > 0) {
                    screen.length--;
                    i--;
                }
            }
            else {
                if (e.target.innerText == '.') {
                    if (i == 0) {
                        screen[i] = '0';
                        i++;
                    }
                    else if (screen[i - 1] < "0" || screen[i - 1] > "9") {
                        screen[i] = '0';
                        i++;
                    }
                }
                screen[i] = e.target.innerText;
                if (e.target.innerText == '=') {
                    console.log(screen);
                    if (Cal.parse(screen, 0, i + 1) == 0) {
                        if (Cal.check_error(Cal.count_number, Cal.op_number, Cal.cal_op_tab) == 0) {
                            result = Cal.Cal_Result();
                            //input.setAttribute("value", screen.join(""));
                            //input1.setAttribute("value",result.toString());
                            input.innerHTML=screen.join("")+"\n"+result.toString();
                            input.scrollTop = input.scrollHeight;
                            flag = 1;
                        }
                    }
                    return;
                }
                i++;
            }
            input.innerHTML=screen.join("");
            input.scrollTop = input.scrollHeight;
            //input.setAttribute("value", screen.join(""));
        }
    }
};