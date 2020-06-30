#include "UltrasonicRanging.h"
#define TRIG        PTB20
#define ECHG        PTB23
#define FLAGWAIT    0xFFFFF

/*uint32 timevar,flag0;		
//	gpio_init(PTB9,GPO,1);			//超声波使能
//	gpio_init(PTB11,GPO,1);
void PIT3_ult(void)		//超声波服务程序
{
  		flag0 = 0;
        gpio_set(TRIG,1);               //产生触发脉冲
		DELAY_MS(25);
        gpio_set(TRIG,0);
        
        while(gpio_get(ECHG) == 0);             //等待电平变高，低电平一直等待
        pit_time_start  (PIT0);                 //开始计时
        while(gpio_get(ECHG) == 1)              //等待电平变低，高电平一直等待
        {
            flag0++;
            if(flag0 >FLAGWAIT)
            {
                break;
            }
        };            
        
        timevar =pit_time_get_us   (PIT0);    //停止计时，获取计时时间
        if(flag0 <FLAGWAIT )
        {
            timevar = timevar * 340 /2/1000;		//单位为MM毫米
                
        }
	PIT_Flag_Clear(PIT3);       //清楚中断标志
 	
}

void PIT3_ult_init(void)			//超声波测距中断初始化
{
  	gpio_init(TRIG,GPO,0);
    gpio_init(ECHG,GPI,0);
	
	pit_init_ms(PIT3,200);	
	set_vector_handler(PIT3_VECTORn,PIT3_ult);		//设置中断服务函数
	enable_irq(PIT3_IRQn);			//使能中断
	
}           */

