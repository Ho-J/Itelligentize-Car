#include "Init.h"
#include "include.h"
#include "VAR.h"
#include "PID.h"
#include "Beep.h"
#include "GUI.h"
#include "image.h"
#include "elec.h"
//=============================�����ڲ�����=====================================
#define LSB1 PTA12
#define DIR1 PTA13
#define LSB2 PTB18
#define DIR2 PTB19

//���ձ������������ı��� 2019-6-6
int16  CntEncoder_left;//����10ms�ɼ���������
int16  CntEncoder_right;//�ҵ��10ms�ɼ���������
//���ת�ٱջ���PID�����ṹ�� 2019-6-6
PID Var_PID_motor_L;
PID *PID_motor_L=&Var_PID_motor_L;
PID Var_PID_motor_R;
PID *PID_motor_R=&Var_PID_motor_R;

uint8 RunState_flag=0;//����״̬��־
uint8 IsDown_flag=0;//�����Ƿ���
uint8 Send_Model=1;//����ģʽ�趨(Ĭ������ģʽ)
uint8 Servo_Model=1;//���PD�㷨ģʽ��Ĭ�ϼ�PD�㷨��
int THOD=-45;//ͼ����ֵ

//���ģ��
PID Var_PID_Servo;              //���PID�ṹ����
PID *PID_Servo=&Var_PID_Servo;

PID Motor_Chasu_PID;            //�������PID=========================
PID *Motor_chasu_PID=&Motor_Chasu_PID;


int chasu_P=100;        //����P
int chasu_D=15;        //����D


int Servo_P=20;   //8���ֱ����//6�����DֵС�ˣ�//6�����DֵС�ˣ�//6�����DֵС�ˣ����Ȳ�����
int Servo_D=50;   //5          //5                //8                //15
//int Motor_P_L=28-0.3;
int Motor_P_L=28;
//int Motor_I_L=8+0.2;
int Motor_I_L=8;
int Motor_D_L=0;
//int Motor_P_R=28-0.2;
int Motor_P_R=22;
int Motor_I_R=8;
int Motor_D_R=0;

int Encoder_set=100; //�趨ֵ

int LRT_flag=0;//��ʼ�������ת��־λ
int Encoder_inc=0;//�ۼ�������
int Speed_L;      //�����������趨
int Speed_R;            //�����������趨
int Laser_Value=0;  //����⵽�ľ���     ��λ��mm

Xianp *xianpleft=NULL;
Xianp *xianpright=NULL;
Xianp *xianpcenter=NULL;
//=============================//�ڲ���������Start//============================


//=============================//�ڲ���������End//==============================

void history_xian()
{
//  Xianp Xianpleft;        //������߽�ͷβ�ڵ�
//  Xianp Xianpright;        //�����ұ߽�ͷβ�ڵ�
//  Xianp Xianpcenter;        //��������ͷβ�ڵ�
  xianpleft=(Xianp *)malloc(sizeof(Xianp));        //������߽�ͷβ�ڵ�
  xianpright=(Xianp *)malloc(sizeof(Xianp));        //�����ұ߽�ͷβ�ڵ�
  xianpcenter=(Xianp *)malloc(sizeof(Xianp));        //��������ͷβ�ڵ�
  //xianpleft;
  //xianpright;
  //xianpcenter;
//  Xian Pheadleft;         //��߽�ṹ��
//  Xian Pheadright;         //�ұ߽�ṹ��
//  Xian Pheadcenter;         //���߽ṹ��
//  Xian *pheadleft=(Xian *)malloc(sizeof(Xian));   //����ͷ�ڵ�
//  Xian *pheadright=(Xian *)malloc(sizeof(Xian));   //����ͷ�ڵ�
//  Xian *pheadcenter=(Xian *)malloc(sizeof(Xian));   //����ͷ�ڵ�
  
  xianpleft->front=NULL;                //ͷָ���ʼ��
  xianpright->front=NULL;
  xianpcenter->front=NULL;
  
  xianpleft->rear=NULL;                 //βָ���ʼ��  
  xianpright->rear=NULL;
  xianpcenter->rear=NULL;
}

/*
  @note      ϵͳ��ʼ���������ⲿ���ã�
  @brief     ����main��������
  @since     v1.0
*/
void System_Init(void)
{	
  //��������ͷ��ʼ��
  camera_all_init();
  
  uart_init (UART2, 9600);        //��ʼ������3��������Ϊ9600
  set_vector_handler(UART2_RX_TX_VECTORn , uart2_test_handler);

  
  NVIC_SetPriorityGrouping(4);            //�������ȼ�����,4bit ��ռ���ȼ�,û�������ȼ�


  NVIC_SetPriority(PORTD_IRQn,0);         //����ͷ���ж�
  NVIC_SetPriority(PIT0_IRQn,1);          //��ʱ��0�ж�
  NVIC_SetPriority(PIT1_IRQn,2);          //��ʱ��1�ж�
  NVIC_SetPriority(DMA0_IRQn,3);          //����ͷDMA�����ж� 

  BeepInit();//��������ʼ��
  //BeepON();  //����������ʾϵͳ��ʼ����ʼ
  
  LCD_init();//LCD��ʼ��
  key_init(KEY_MAX);//���򰴼�
  
  flash_init();
  DELAY_MS(500);

  for(uint8 i=0;i<Num;i++)
  {
    if(i==3)
    {
      THOD=flash_read(FLASH_SECTOR_NUM-1-3,0,int);
      if(THOD>100 || THOD<25)
        THOD=45;
      SCCB_WriteByte(OV7725_CNST,THOD);//д������ͷ�Ĵ���
    }
    ///else if(i==5);//��ʱ���ε��PID  i==6||i==4||
    else
      *Value_T[i]=flash_read(FLASH_SECTOR_NUM-1-i,0,int);

  }
  
  Menu_Var_init();
    
  
  
//2019-6-7  ���ջ� Start
  motor_init();//���ҵ����������ת����ʼ����ʹ��
  Sample_period_init();//�������ڼ���ʱ���жϳ�ʼ����ʱ�䣩
  encoder_init();//������

  PID_init(PID_motor_L);//����PID�ṹ��
  PID_init(PID_motor_R);//�ҵ��PID�ṹ��
  
  //����Encoder_set=160ʱ���趨���ҵ��P��I��D��ʼֵ
//  PID_kp(PID_motor_L,Motor_P_L-0.3);//P��28-0.3
//  PID_ki(PID_motor_L,Motor_I_L+0.2);//I��8+0.2���7.7V
//  PID_kd(PID_motor_L,Motor_D_L);
  
//    PID_kp(PID_motor_L,58);//P��28-0.3
//  PID_ki(PID_motor_L,1);//I��8+0.2���7.7V
//  PID_kd(PID_motor_L,0);
  
  //30   8   8
  
//    PID_kp(PID_motor_L,Motor_P_L);//P��28-0.3
//  PID_ki(PID_motor_L,Motor_I_L);//I��8+0.2���7.7V
//  PID_kd(PID_motor_L,Motor_D_L);
//  
//    PID_kp(PID_motor_R,Motor_P_L);//P��22.8
//  PID_ki(PID_motor_R,Motor_I_L);    //I��8
//  PID_kd(PID_motor_R,Motor_D_L);
  
      PID_kp(PID_motor_L,30);//P��28-0.3
  PID_ki(PID_motor_L,8);//I��8+0.2���7.7V
  PID_kd(PID_motor_L,8);
  
    PID_kp(PID_motor_R,30);//P��22.8
  PID_ki(PID_motor_R,8);    //I��8
  PID_kd(PID_motor_R,8);
  
  
//    PID_kp(PID_motor_L,75);//P��28-0.3                      75
//  PID_ki(PID_motor_L,13.6);//I��8+0.2���7.7V                 14
//  PID_kd(PID_motor_L,0);
  
  
  
//  PID_kp(PID_motor_R,Motor_P_R+0.8);//P��22.8
//  PID_ki(PID_motor_R,Motor_I_R);    //I��8
//  PID_kd(PID_motor_R,Motor_D_R);
  
//  PID_kp(PID_motor_R,58);//P��22.8
//  PID_ki(PID_motor_R,1);    //I��8
//  PID_kd(PID_motor_R,0);
  
//  PID_kp(PID_motor_R,Motor_P_R);//P��22.8
//  PID_ki(PID_motor_R,Motor_I_R);    //I��8
//  PID_kd(PID_motor_R,Motor_D_R);

  
//  PID_kp(PID_motor_R,75);//P��22.8
//  PID_ki(PID_motor_R,13.6);    //I��8
//  PID_kd(PID_motor_R,0);
//2019-6-7  ���ջ�  End
  

  
  
  adc_init(ADC0_SE17);          //AD�ɼ���ʼ��
  adc_init(ADC0_SE18);
  adc_init(ADC1_SE6a);
  
  //�趨�����P��I��D��ʼֵ
  PID_kp(PID_Servo,(float)(1.0*Servo_P/10));
  PID_kd(PID_Servo,(float)(Servo_D*1.0/10));
  
  //�趨�������
  PID_kp(Motor_chasu_PID,(float)(chasu_P*1.0/10));           //���� P��ֵ
  PID_kd(Motor_chasu_PID,(float)(chasu_D*1.0/10));           //���� D��ֵ
  
  
  FTM_PWM_init(FTM1,FTM_CH1,85,ServoCentreValue);//�����ʼ��������Ƶ��85Hz
  //FTM_PWM_init(FTM1,FTM_CH1,85,1913);//�����ʼ��������Ƶ��85Hz
  //BeepOFF(); //����������ʾϵͳ��ʼ������
  
  

}

//=============================���屾ҳ�ĺ���===================================

/*==============�������ڼ���ʱ���жϳ�ʼ��==================================// 
���ܣ�����������������ڣ�10ms
����˵����
          ��ʼ��PIT0����ʱʱ��Ϊ�� 10ms
          ����PIT0���жϷ�����Ϊ PIT0_IRQHandler
          ʹ��PIT0�ж�
*/ 
void Sample_period_init(void)
{
  //�������ڼ������������
  pit_init_ms(PIT0, 10);                                
  set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      
  enable_irq (PIT0_IRQn); 
  //��ʱ���ж�
  pit_init_ms(PIT1,10);                                
  set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);      
  enable_irq (PIT1_IRQn); 
}
/*
  @brief      ��������ʼ��
  @note       
        1.��������LSB����������ʼ��(��DMA��ʼ��)
        2.DIR��ת�����ʼ��
        3.ͨ����λ
 @date       2019-6-7
*/
void encoder_init(void) 
{
  DMA_count_Init(DMA_CH3,LSB1,1000,DMA_rising);		//DMA1��ʼ�������
  DMA_count_Init(DMA_CH4,LSB2,1000,DMA_rising);		//DMA2��ʼ�������
  gpio_init (DIR1, GPI,1);                          //��ʼ����������ȡ��ת����
  gpio_init (DIR2, GPI,1);                          //��ʼ����������ȡ��ת����
  DMA_count_reset(DMA_CH3);                         //��λ
  DMA_count_reset(DMA_CH4);                         //��λ
}

/* 
@brief  �����ʼ��
@note   ʹ�����ų�ʼ��gpio.c(���˿ڣ������źŷ��򣨼�оƬ���Ŷ�ȡ������źţ��������ʼ״̬)
        ͨ���ڳ�ʼ����chip/ftm.c(Ƶ��Ϊ����ͺš��������Ƶ��������)����ǰ״̬10k
@date   2019-6-7
*/
void motor_init(void)
{
  FTM_PWM_init(FTM0, FTM_CH4,10*1000, 0);//T����ת��
  FTM_PWM_init(FTM0, FTM_CH5,10*1000, 0);//F
  FTM_PWM_init(FTM0, FTM_CH6,10*1000, 0);//F
  FTM_PWM_init(FTM0, FTM_CH7,10*1000, 0);//T����ת��
  gpio_init (PTB0, GPO,1);//���ʹ��
}
//==============================================================================
//  @brief      PIT0��ʱ�������������������(����������)
//  @since      v2.0
//  @note       
//  @date       2019-5-25
//==============================================================================

int16 Encoder_read_L(void)
{ 
  uint8 i=0;
  int16 DMA_cnt;
  static int16 speedcnt_L[5];
  
  //���Ϊ�棬���ַ�ת�ߵ�ƽ����ת�͵�ƽ   
  if (gpio_get(DIR1)) 
  {DMA_cnt=DMA_count_get(DMA_CH3);}
  else
  {DMA_cnt=-DMA_count_get(DMA_CH3);}
  
  //ͨ����������
  DMA_count_reset(DMA_CH3);
  DMA_EN(DMA_CH3);
  for(i=0;i<4;i++)
  {
    speedcnt_L[i]=speedcnt_L[i+1];    
    
  }
  
  speedcnt_L[4]=DMA_cnt;    //���µ�ǰ�ļ���ֵ  
  //return DMA_cnt;
 return (int16)((speedcnt_L[0]+speedcnt_L[1]+speedcnt_L[2]+speedcnt_L[3]+speedcnt_L[4])/5);//
  
}
int16 Encoder_read_R(void)
{
  uint8 i=0;
  int16 DMA_cnt;
  static int16 speedcnt_R[5];
  
  //���Ϊ�棬���ַ�ת�ߵ�ƽ����ת�͵�ƽ   
  if (gpio_get(DIR2)) 
  {
    DMA_cnt=-DMA_count_get(DMA_CH4);
  }
  else
  {DMA_cnt=DMA_count_get(DMA_CH4);}
  
  //ͨ����������
  DMA_count_reset(DMA_CH4);
  DMA_EN(DMA_CH4);
  
  for(i=0;i<4;i++)
  {
    speedcnt_R[i]=speedcnt_R[i+1];    
    
  }
  speedcnt_R[4]=DMA_cnt;    //���µ�ǰ�ļ���ֵ  
  
  //return DMA_cnt;
  return (int16)((speedcnt_R[0]+speedcnt_R[1]+speedcnt_R[2]+speedcnt_R[3]+speedcnt_R[4])/5);
}

int PWM_duty_deta_L=0;//����ռ�ձȷ����ۼ�ֵ
int PWM_duty_deta_R=0;//�ҵ��ռ�ձȷ����ۼ�ֵ

/* 
@brief  PWM�޷�
@pram   
@note   
@date   2019-6-8
*/
//#define PWM_VALUE_MAX 10000
//int PWM_limit(int pwm_value)
//{
//  if(pwm_value>=PWM_VALUE_MAX)
//  {
//    pwm_value=PWM_VALUE_MAX;//ռ�ձȷ��ӱ��ʹ���
//  }
//  else
//  {
//    pwm_value=pwm_value;
//  }
//  return pwm_value
//}
/* 
@brief  PWM���Ƶ��
@pram   PWM_duty_deta_L����ռ�ձȷ��ӣ�PWM_duty_deta_R����ռ�ձȷ��ӡ�
@note   
@date   2019-6-8
*/
//void motor_Ctrl0(int PWM_duty_deta_L,int PWM_duty_deta_R)
//{
//  uint16 pwm_duty_left,pwm_duty_right;
//  
//  //����
//  if(PWM_duty_deta_L>=0)
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH5, 0);//��ת
//    pwm_duty_left = PWM_duty_deta_L;//���������ת��ѹ
//    pwm_duty_left=PWM_limit(pwm_duty_left);//ռ�ձȷ��ӱ��ʹ���   
//    FTM_PWM_Duty(FTM0, FTM_CH4, pwm_duty_left);//��ת
//  }
//  else
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH4, 0);//��ת
//    pwm_duty_left = PWM_duty_deta_L;//���������ת��ѹ
//    pwm_duty_left=PWM_limit(pwm_duty_left);//ռ�ձȷ��ӱ��ʹ���   
//    FTM_PWM_Duty(FTM0, FTM_CH5, pwm_duty_left);//��ת
//  }
////�ҵ��
//  if(PWM_duty_deta_R>=0)
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH6, 0);//��ת
//    pwm_duty_right = PWM_duty_deta_R;//���������ת��ѹ
//    pwm_duty_right=PWM_limit(pwm_duty_right);//ռ�ձȷ��ӱ��ʹ���   
//    FTM_PWM_Duty(FTM0, FTM_CH7, pwm_duty_right);//��ת
//  }
//  else
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH7, 0);//��ת
//    pwm_duty_right = PWM_duty_deta_R;//���������ת��ѹ
//    pwm_duty_right=PWM_limit(pwm_duty_right);//ռ�ձȷ��ӱ��ʹ���   
//    FTM_PWM_Duty(FTM0, FTM_CH6, pwm_duty_right);//��ת
//  }
//}
/* 
@brief  motor_PID_Ctrl
@pram   
@note   
@date   2019-6-8
*/
//void motor_PID_Ctrl()
//{
//    PWM_duty_deta_L+=(int32)PID_Calc(PID_motor_L);
//}

float casu;                     //���ҵ������ 
float casu_iip=0.000013;         //Ħ��ϵ��


/*=========================================================================
�������ƣ�motor_encoder_set
��������:ͨ��casu ��������������������ҵ���ø���������
ʱ�䣺2019��6��11��18:01:52
������casu ����        Encoder_set ����������
����ֵ��Speed_L  Speed_R ���ҵ��������
=========================================================================*/
void motor_encoder_set(int Encoder_set)
{
  int sp=0;
  if(protect_flag==0&&Encoder_set!=0)
  {
    sp=Encoder_set+Expectation_speed;   // Expectation_speed  ��Ϊ������  30   15   0
    sp=Encoder_set;
//    if(outside_flag)
//      Encoder_set=90;
    Speed_L=(int)(sp*(1+casu));
    Speed_R=(int)(sp*(1-casu));    
    
    
    //Speed_L=(int)(Encoder_set*(1+casu));
    //Speed_R=(int)(Encoder_set*(1-casu));
//    Speed_L=Encoder_set;
//    Speed_R=Encoder_set;
  }
}
int show_chasu=0;
void chasu_yici()
{
  
 casu=(float)((chasu_PWM_data*1.0/1000)/2);    //chasu_PWM_data     error= -80����0����80  ������� p 100  d  100  
 
 int sp=0;
 //image_finish_F_Mot
 //-6800<chasu_PWM_data<6800
 // -068<(chasu_PWM_data*1.0/10000)<0.68   P=850
 if(protect_flag==0&&Encoder_set!=0)
 {
   sp=Encoder_set+Expectation_speed;   // Expectation_speed  ��Ϊ������  30   15   0
   sp=Encoder_set;
   Speed_L=(int)(sp*(1+casu));
   Speed_R=(int)(sp*(1-casu));    
 }

  //show_chasu=casu;
  casu=-casu;
  if(casu>0.34)  //������������һ��Ҫ���ӵı���ϵ��
  {
    casu=0.34;
  }
  if(casu<-0.34)
  {
    casu=-0.34;
  }
  
}

/*=========================================================================
�������ƣ�chasu_set
��������:ͨ����� P D ���������ռ�ձȷ���  casu_iip  Ħ��ϵ��
ʱ�䣺2019��6��11��18:01:52
������Servo_PWM_deta ���PD����ֵ      casu_iip Ħ��ϵ��
����ֵ��casu  
=========================================================================*/
void chasu_set()
{
  if(Servo_PWM_deta>0)  //��
  {
    casu=-(Servo_PWM_deta*Servo_PWM_deta)*casu_iip;//Servo_PWM_deta ͨ����� P D ���������ռ�ձȷ���  casu_iip  Ħ��ϵ��
    /*
    ���������  1��Servo_PWM_deta=(int)(pd->P*y*midpoint_error + pd->D*(midpoint_error-lastpoint_error));
                2��casu_iip=0.00013;
    ����1��        ��С midpoint_error-lastpoint_error->0            midpoint_error->0
                   ��� midpoint_error-lastpoint_error->40           midpoint_error->80                     
    */
  }else
  {
    casu=(Servo_PWM_deta*Servo_PWM_deta)*casu_iip;
    
  }
  
  
  if(casu>0.34)  //������������һ��Ҫ���ӵı���ϵ��
  {
    casu=0.34;
  }
  if(casu<-0.34)
  {
    casu=-0.34;
  }
}


void motor_Ctrl(void)
{

 //ͣ��||��ʱ�� 
  int speed_L,speed_R;
  if(RunState_flag&& protect_flag==0)//����protect_flag  ��protect_flagΪ0ʱ�жϳ���������  ����Ϊprotect_flag=1
  {
  speed_L=Speed_L;//��ʱ��
  speed_R=Speed_R;//��ʱ��
  }
  else
  {
    speed_L=0;//ͣ��
    speed_R=0;//ͣ��
    LRT_flag=0;
    RunState_flag=0;
  }
  
  PID_set(PID_motor_L,speed_L,CntEncoder_left);//����PIDƫ��  
  PWM_duty_deta_L+=(int32)PID_Calc(PID_motor_L);//�ۼ�ռ�ձȣ�����ʽPID���㣩
  if(PWM_duty_deta_L>10000)//ռ�ձ��޷�
  {
    PWM_duty_deta_L=10000;
  }
  if(PWM_duty_deta_L<-10000) 
  {
    PWM_duty_deta_L=-10000;
  }
  
  PID_set(PID_motor_R,speed_R,CntEncoder_right);//����PIDƫ��
  PWM_duty_deta_R+=PID_Calc(PID_motor_R);//�ۼ�ռ�ձȣ�����ʽPID���㣩 
  
  if(PWM_duty_deta_R>10000)//ռ�ձ��޷�
    PWM_duty_deta_R=10000;
  if(PWM_duty_deta_R<-10000)
    PWM_duty_deta_R=-10000;
 //����   
  if(PWM_duty_deta_L>=0) 
  {
    FTM_PWM_Duty(FTM0, FTM_CH4, PWM_duty_deta_L);//��ת
    FTM_PWM_Duty(FTM0, FTM_CH5, 0);//��ת
  }
  else 
  {
    FTM_PWM_Duty(FTM0, FTM_CH5, -PWM_duty_deta_L);//��ת
    FTM_PWM_Duty(FTM0, FTM_CH4, 0);//��ת
  }
 //�ҵ��    
  if(PWM_duty_deta_R>=0)
  {
    FTM_PWM_Duty(FTM0, FTM_CH6,0);//��ת 
    FTM_PWM_Duty(FTM0, FTM_CH7, PWM_duty_deta_R);//��ת
  }
  else 
  {
    FTM_PWM_Duty(FTM0, FTM_CH7, 0);//��ת
    FTM_PWM_Duty(FTM0, FTM_CH6, -PWM_duty_deta_R);//��ת 
  }
  
      //�����ת����
//  if(LRT_flag)
//  {
//    if((abs(PWM_duty_deta_L)>=8000)&&CntEncoder_left<50)
//    { 
//      PWM_duty_deta_L=0;
//      gpio_init (PTB0, GPO,0);
//    }
//    else;
//    
//    if((abs(PWM_duty_deta_R)>=8000)&&CntEncoder_right<50)
//    { 
//      PWM_duty_deta_R=0;
//      gpio_init (PTB0, GPO,0);
//    }
//    else;
//  }
  
//  if(key_check(KEY_L)==KEY_DOWN && GUI_flag==0)
//  {
//    IsDown_flag=1;//������������ʱ��־λ
//    IsBeep=1;     //����������ʾ��ʼ����ʱ
//  }
//  if(key_check(KEY_R)==KEY_DOWN && GUI_flag==0)
//  {
//    Send_Model=~Send_Model;
//  }
  
}

SV sv_l;
SV sv_r;

SV * sv_L=&sv_l;   // ����������� ��������� ·�� �ٶ�  �ۼ������� 

SV * sv_R=&sv_r;    // �����ұ����� ��������� ·�� �ٶ�  �ۼ�������  

char distance_laser[50];//��ȡ�������ַ���


extern volatile IMG_STATUS_e ov7725_eagle_img_flag;

void PIT0_IRQHandler()
{
  
  CntEncoder_left=Encoder_read_L();     //��ȡ���ұ�����ʵʱ��
  CntEncoder_right=Encoder_read_R();    //��ȡ���ұ�����ʵʱ��
  
  //if(ov7725_eagle_img_flag==IMG_FINISH)
    ElecValue_dispose();                //������ݴ���
  
  
  //----------����--------------------
  //chasu_set();          //������� ���ι�ϵ
    
 chasu_yici();
  
  motor_encoder_set(Encoder_set);       //�õ����ҵ��������������  һ�ι�ϵ
  
  sv_L->Add_CntEncoder=CntEncoder_left+sv_L->Add_CntEncoder;     //�������ۼ�ֵ
  sv_L->S=(sv_L->Add_CntEncoder)*1.0/5300;                        //�ߵ�·��  ��λm   4300   5270          10ms ��Add_CntEncoder/5300��m    m*100=cm      
 // sv_L->V=(CntEncoder_left*1.0/5300)/3;                       //�ٶ�  ��λ m/ms
  
  sv_R->Add_CntEncoder=CntEncoder_right+sv_R->Add_CntEncoder;     //�������ۼ�ֵ
  sv_R->S=(sv_R->Add_CntEncoder)*1.0/5300;                        //�ߵ�·��  ��λm    4300   5270
 // sv_R->V=(CntEncoder_right*1.0/5300)/3;                       //�ٶ�  ��λ m/ms
  
   Encoder_inc=CntEncoder_right+Encoder_inc;
   
   

  
 motor_Ctrl();//�������

  //Send_16bitData(Encoder_set,CntEncoder_left,PWM_duty_deta_L);//�������Ӧ�������ж����淢�Ͳ��Σ������β�ƽ�����Һ���ʱ����Ҳ��һ��
  
  PIT_Flag_Clear(PIT0);       //���жϱ�־λ
  

}




int error_be=0;
int time=0;//���жϴ���
uint8 Timer_n=0;//��������
int Image_num=0;//ͼ�����
uint8 image_num_kaiqi=0;    //    image_num_kaiqi  Ϊ 1 ��ʼ  Image_num 
int Image_num1=0;
void PIT1_IRQHandler(void)
{
  
  Image_num++;                          //ͼ��ʹ��
  Image_num1++;                         //ͼ��ʹ��
  speed_dingshi++;                      //ͼ������ٶ�ʹ��
  if(Image_num>9999)Image_num=0;
  if(Image_num1>9999)Image_num1=0;
  if(speed_dingshi>999)speed_dingshi=0;
  if(IsDown_flag==1)
  {
    if(++time==200)
    {
      time=0;
      RunState_flag=1;//��������200*10ms���������״̬
      IsDown_flag=0;//���������־
      IsBeep=0;     //����������ʾ����ʱ����
    }    
  }
  if(RunState_flag==1)
  {
//    if(++time==300 && Send_Model==1)//��ʱ����ģʽ
//    {
//      RunState_flag=0;
//    }
    if(++time==300)//300*10ms
    {
      Started_check_Zebra=1;//�����󵹼�ʱ3�룬�а����߾�ͣ��
      LRT_flag=1;//�����󵹼�ʱ3�룬��ʼ������Ƿ��ж�ת
    }
    else;
    
    
  }
//  if(Send_Model==254)
//  {
//    Send_16bitData(error,(int)(100*show_chasu),elec_M);
//    error_be=error;
//  }
  //Send_16bitData(Encoder_set,CntEncoder_right,PWM_duty_deta_R);//�������Ӧ�������ж����淢�Ͳ��Σ������β�ƽ�����Һ���ʱ����Ҳ��һ��
  //Send_16bitData_var6(Encoder_set,CntEncoder_right,PWM_duty_deta_R,CntEncoder_left,PWM_duty_deta_L,Encoder_set);
  //Send_16bitData(Encoder_set,CntEncoder_left,PWM_duty_deta_L);//�������Ӧ�������ж����淢�Ͳ��Σ������β�ƽ�����Һ���ʱ����Ҳ��һ��
  PIT_Flag_Clear(PIT1);       //���жϱ�־λ
}

/*!
 *  @brief      UART0�����жϷ�����
 *  @since      v5.0
 *  @warning    �˺�����Ҫ�û������Լ�������ɣ�����������ṩһ��ģ��
 *  Sample usage:       set_vector_handler(UART0_RX_TX_VECTORn , uart0_test_handler);    //�� uart0_handler ������ӵ��ж�����������Ҫ�����ֶ�����
 */

int op=0;
int valuejuli=0;
char s[20];   //���յ����ַ���
int klkl=0;
void uart2_test_handler(void)
{
  uart_rx_irq_dis(UART2); //�ش���0�����ж�
  klkl=1;
  int pnum=0;
  //int shuF=0;
 
  for(int i=19;i>0;i--)    //�� 0���󸲸�     
  {
    s[i]=s[i-1];
  }
 // UARTn_e uratn = UART2;
  uart_getchar(UART2,&s[0]); //��ȡ���յ�����
   
  
  
   valuejuli=0;
  for(int j=0;j<10;j++)
 {
   
   if(s[j]=='m'&&s[j+1]=='m')
   {
    for(int pp=j+2;pp<=19;pp++)
     {
       if(s[pp]>'9'||s[pp]<'0')
         break;
       pnum++;
        
      }
     int cifang=0;
     for(int pp=j+2;pp<j+2+pnum&&pp<19;pp++)
      {
        
      
        valuejuli=(s[pp]-'0')*(int)(pow(10,cifang))+valuejuli;
        cifang++;
      }
     Laser_Value=valuejuli;
      break;
    }
   
 }
  
  //pass();
  
  
  uart_rx_irq_en(UART2);         //������0�����ж�
  
}


/*
@note      ��������ͷ��ʼ���������ڲ����ã�
@brief     ���ڱ�ҳϵͳ��ʼ����������
@since     v1.0
*/
void camera_all_init(void)
{
  //����ʹ������ͷ���жϣ����жϣ����жϷ�����ΪPORTD_IRQHandler
  set_vector_handler(PORTD_VECTORn , PORTD_IRQHandler);  
  
  //����DMA0�жϷ�����ΪDMA0_IRQHandler
  set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);   

  //��ʼ������ͷ 
  camera_init(imgbuff);  //�����������Ƿ���Ҫ��˳�����ǰ�����ж�����֮���д����� 

}

/*
 @note      DMA0�жϷ��������ڲ����ã�
 @brief     �������ͨ������ͷ��DMA�ɼ�ͨ��: DMA_CH0 
 @since     v1.0
*/
void DMA0_IRQHandler(void)
{
   //--��ͼ��������   ���ͨ��  
    camera_dma();      
}
/*
 @note       PORTD�жϷ��������ڲ����ã�
 @brief      ��ʹ������ͷ���жϣ���ʹ�����жϣ������жϺ�����VCAN_OV7725_Eagle.c��û
             �ж��塣
 @since      v1.0
*/
void PORTD_IRQHandler(void)
{
    uint8  n;    //���ź�
    uint32 flag;

   while(!PORTD_ISFR);
    flag = PORTD_ISFR;
    PORTD_ISFR  = ~0;                                   //���жϱ�־λ

    n = 0;                                             //ʹ�ó��ж�
    if(flag & (1 << n))                                //PTD0�����ж�
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //��ʹ�����ж�
    n = 18;
    if(flag & (1 << n))                                 //PTC18�����ж�
    {
        camera_href();
    }
#endif
}


//=============================���屾ҳ�ĺ���end================================