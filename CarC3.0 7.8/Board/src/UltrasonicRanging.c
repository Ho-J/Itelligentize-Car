#include "UltrasonicRanging.h"
#define TRIG        PTB20
#define ECHG        PTB23
#define FLAGWAIT    0xFFFFF

/*uint32 timevar,flag0;		
//	gpio_init(PTB9,GPO,1);			//������ʹ��
//	gpio_init(PTB11,GPO,1);
void PIT3_ult(void)		//�������������
{
  		flag0 = 0;
        gpio_set(TRIG,1);               //������������
		DELAY_MS(25);
        gpio_set(TRIG,0);
        
        while(gpio_get(ECHG) == 0);             //�ȴ���ƽ��ߣ��͵�ƽһֱ�ȴ�
        pit_time_start  (PIT0);                 //��ʼ��ʱ
        while(gpio_get(ECHG) == 1)              //�ȴ���ƽ��ͣ��ߵ�ƽһֱ�ȴ�
        {
            flag0++;
            if(flag0 >FLAGWAIT)
            {
                break;
            }
        };            
        
        timevar =pit_time_get_us   (PIT0);    //ֹͣ��ʱ����ȡ��ʱʱ��
        if(flag0 <FLAGWAIT )
        {
            timevar = timevar * 340 /2/1000;		//��λΪMM����
                
        }
	PIT_Flag_Clear(PIT3);       //����жϱ�־
 	
}

void PIT3_ult_init(void)			//����������жϳ�ʼ��
{
  	gpio_init(TRIG,GPO,0);
    gpio_init(ECHG,GPI,0);
	
	pit_init_ms(PIT3,200);	
	set_vector_handler(PIT3_VECTORn,PIT3_ult);		//�����жϷ�����
	enable_irq(PIT3_IRQn);			//ʹ���ж�
	
}           */

