#include "Init.h"
#include "include.h"
#include "VAR.h"
#include "PID.h"
#include "Beep.h"
#include "GUI.h"
#include "image.h"
#include "elec.h"
//=============================定义内部变量=====================================
#define LSB1 PTA12
#define DIR1 PTA13
#define LSB2 PTB18
#define DIR2 PTB19

//接收编码器脉冲数的变量 2019-6-6
int16  CntEncoder_left;//左电机10ms采集的脉冲数
int16  CntEncoder_right;//右电机10ms采集的脉冲数
//电机转速闭环的PID参数结构体 2019-6-6
PID Var_PID_motor_L;
PID *PID_motor_L=&Var_PID_motor_L;
PID Var_PID_motor_R;
PID *PID_motor_R=&Var_PID_motor_R;

uint8 RunState_flag=0;//运行状态标志
uint8 IsDown_flag=0;//按键是否按下
uint8 Send_Model=1;//运行模式设定(默认正常模式)
uint8 Servo_Model=1;//舵机PD算法模式（默认加PD算法）
int THOD=-45;//图像阈值

//舵机模块
PID Var_PID_Servo;              //舵机PID结构体聲明
PID *PID_Servo=&Var_PID_Servo;

PID Motor_Chasu_PID;            //电机差速PID=========================
PID *Motor_chasu_PID=&Motor_Chasu_PID;


int chasu_P=100;        //差数P
int chasu_D=15;        //差速D


int Servo_P=20;   //8弯过直道抖//6弯拐晚（D值小了）//6弯拐晚（D值小了）//6弯拐晚（D值小了，力度不够）
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

int Encoder_set=100; //设定值

int LRT_flag=0;//开始检测电机堵转标志位
int Encoder_inc=0;//累计脉冲数
int Speed_L;      //左轮脉冲数设定
int Speed_R;            //右轮脉冲数设定
int Laser_Value=0;  //激光测到的距离     单位：mm

Xianp *xianpleft=NULL;
Xianp *xianpright=NULL;
Xianp *xianpcenter=NULL;
//=============================//内部函数声明Start//============================


//=============================//内部函数声明End//==============================

void history_xian()
{
//  Xianp Xianpleft;        //储存左边界头尾节点
//  Xianp Xianpright;        //储存右边界头尾节点
//  Xianp Xianpcenter;        //储存中线头尾节点
  xianpleft=(Xianp *)malloc(sizeof(Xianp));        //储存左边界头尾节点
  xianpright=(Xianp *)malloc(sizeof(Xianp));        //储存右边界头尾节点
  xianpcenter=(Xianp *)malloc(sizeof(Xianp));        //储存中线头尾节点
  //xianpleft;
  //xianpright;
  //xianpcenter;
//  Xian Pheadleft;         //左边界结构体
//  Xian Pheadright;         //右边界结构体
//  Xian Pheadcenter;         //中线结构体
//  Xian *pheadleft=(Xian *)malloc(sizeof(Xian));   //创建头节点
//  Xian *pheadright=(Xian *)malloc(sizeof(Xian));   //创建头节点
//  Xian *pheadcenter=(Xian *)malloc(sizeof(Xian));   //创建头节点
  
  xianpleft->front=NULL;                //头指针初始化
  xianpright->front=NULL;
  xianpcenter->front=NULL;
  
  xianpleft->rear=NULL;                 //尾指针初始化  
  xianpright->rear=NULL;
  xianpcenter->rear=NULL;
}

/*
  @note      系统初始化函数（外部调用）
  @brief     用于main函数调用
  @since     v1.0
*/
void System_Init(void)
{	
  //所有摄像头初始化
  camera_all_init();
  
  uart_init (UART2, 9600);        //初始化串口3，波特率为9600
  set_vector_handler(UART2_RX_TX_VECTORn , uart2_test_handler);

  
  NVIC_SetPriorityGrouping(4);            //设置优先级分组,4bit 抢占优先级,没有亚优先级


  NVIC_SetPriority(PORTD_IRQn,0);         //摄像头场中断
  NVIC_SetPriority(PIT0_IRQn,1);          //定时器0中断
  NVIC_SetPriority(PIT1_IRQn,2);          //定时器1中断
  NVIC_SetPriority(DMA0_IRQn,3);          //摄像头DMA传输中断 

  BeepInit();//蜂鸣器初始化
  //BeepON();  //蜂鸣器响提示系统初始化开始
  
  LCD_init();//LCD初始化
  key_init(KEY_MAX);//五向按键
  
  flash_init();
  DELAY_MS(500);

  for(uint8 i=0;i<Num;i++)
  {
    if(i==3)
    {
      THOD=flash_read(FLASH_SECTOR_NUM-1-3,0,int);
      if(THOD>100 || THOD<25)
        THOD=45;
      SCCB_WriteByte(OV7725_CNST,THOD);//写入摄像头寄存器
    }
    ///else if(i==5);//暂时屏蔽电机PID  i==6||i==4||
    else
      *Value_T[i]=flash_read(FLASH_SECTOR_NUM-1-i,0,int);

  }
  
  Menu_Var_init();
    
  
  
//2019-6-7  曾艺煌 Start
  motor_init();//左右电机（正、反转）初始化及使能
  Sample_period_init();//采样周期及定时器中断初始化（时间）
  encoder_init();//编码器

  PID_init(PID_motor_L);//左电机PID结构体
  PID_init(PID_motor_R);//右电机PID结构体
  
  //基于Encoder_set=160时，设定左右电机P、I、D初始值
//  PID_kp(PID_motor_L,Motor_P_L-0.3);//P：28-0.3
//  PID_ki(PID_motor_L,Motor_I_L+0.2);//I：8+0.2电池7.7V
//  PID_kd(PID_motor_L,Motor_D_L);
  
//    PID_kp(PID_motor_L,58);//P：28-0.3
//  PID_ki(PID_motor_L,1);//I：8+0.2电池7.7V
//  PID_kd(PID_motor_L,0);
  
  //30   8   8
  
//    PID_kp(PID_motor_L,Motor_P_L);//P：28-0.3
//  PID_ki(PID_motor_L,Motor_I_L);//I：8+0.2电池7.7V
//  PID_kd(PID_motor_L,Motor_D_L);
//  
//    PID_kp(PID_motor_R,Motor_P_L);//P：22.8
//  PID_ki(PID_motor_R,Motor_I_L);    //I：8
//  PID_kd(PID_motor_R,Motor_D_L);
  
      PID_kp(PID_motor_L,30);//P：28-0.3
  PID_ki(PID_motor_L,8);//I：8+0.2电池7.7V
  PID_kd(PID_motor_L,8);
  
    PID_kp(PID_motor_R,30);//P：22.8
  PID_ki(PID_motor_R,8);    //I：8
  PID_kd(PID_motor_R,8);
  
  
//    PID_kp(PID_motor_L,75);//P：28-0.3                      75
//  PID_ki(PID_motor_L,13.6);//I：8+0.2电池7.7V                 14
//  PID_kd(PID_motor_L,0);
  
  
  
//  PID_kp(PID_motor_R,Motor_P_R+0.8);//P：22.8
//  PID_ki(PID_motor_R,Motor_I_R);    //I：8
//  PID_kd(PID_motor_R,Motor_D_R);
  
//  PID_kp(PID_motor_R,58);//P：22.8
//  PID_ki(PID_motor_R,1);    //I：8
//  PID_kd(PID_motor_R,0);
  
//  PID_kp(PID_motor_R,Motor_P_R);//P：22.8
//  PID_ki(PID_motor_R,Motor_I_R);    //I：8
//  PID_kd(PID_motor_R,Motor_D_R);

  
//  PID_kp(PID_motor_R,75);//P：22.8
//  PID_ki(PID_motor_R,13.6);    //I：8
//  PID_kd(PID_motor_R,0);
//2019-6-7  曾艺煌  End
  

  
  
  adc_init(ADC0_SE17);          //AD采集初始化
  adc_init(ADC0_SE18);
  adc_init(ADC1_SE6a);
  
  //设定舵机的P、I、D初始值
  PID_kp(PID_Servo,(float)(1.0*Servo_P/10));
  PID_kd(PID_Servo,(float)(Servo_D*1.0/10));
  
  //设定电机差速
  PID_kp(Motor_chasu_PID,(float)(chasu_P*1.0/10));           //差速 P给值
  PID_kd(Motor_chasu_PID,(float)(chasu_D*1.0/10));           //差速 D给值
  
  
  FTM_PWM_init(FTM1,FTM_CH1,85,ServoCentreValue);//舵机初始化，驱动频率85Hz
  //FTM_PWM_init(FTM1,FTM_CH1,85,1913);//舵机初始化，驱动频率85Hz
  //BeepOFF(); //蜂鸣器关提示系统初始化结束
  
  

}

//=============================定义本页的函数===================================

/*==============采样周期及定时器中断初始化==================================// 
功能：编码器脉冲采样周期：10ms
代码说明：
          初始化PIT0，定时时间为： 10ms
          设置PIT0的中断服务函数为 PIT0_IRQHandler
          使能PIT0中断
*/ 
void Sample_period_init(void)
{
  //采样周期及电机控制周期
  pit_init_ms(PIT0, 10);                                
  set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);      
  enable_irq (PIT0_IRQn); 
  //定时器中断
  pit_init_ms(PIT1,10);                                
  set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);      
  enable_irq (PIT1_IRQn); 
}
/*
  @brief      编码器初始化
  @note       
        1.编码器（LSB脉冲数）初始化(即DMA初始化)
        2.DIR旋转方向初始化
        3.通道复位
 @date       2019-6-7
*/
void encoder_init(void) 
{
  DMA_count_Init(DMA_CH3,LSB1,1000,DMA_rising);		//DMA1初始脉冲计数
  DMA_count_Init(DMA_CH4,LSB2,1000,DMA_rising);		//DMA2初始脉冲计数
  gpio_init (DIR1, GPI,1);                          //初始化编码器读取旋转方向
  gpio_init (DIR2, GPI,1);                          //初始化编码器读取旋转方向
  DMA_count_reset(DMA_CH3);                         //复位
  DMA_count_reset(DMA_CH4);                         //复位
}

/* 
@brief  电机初始化
@note   使能引脚初始化gpio.c(（端口，引脚信号方向（即芯片引脚读取、输出信号），输出初始状态)
        通道口初始化：chip/ftm.c(频率为电机型号、电机驱动频率所决定)：当前状态10k
@date   2019-6-7
*/
void motor_init(void)
{
  FTM_PWM_init(FTM0, FTM_CH4,10*1000, 0);//T（正转）
  FTM_PWM_init(FTM0, FTM_CH5,10*1000, 0);//F
  FTM_PWM_init(FTM0, FTM_CH6,10*1000, 0);//F
  FTM_PWM_init(FTM0, FTM_CH7,10*1000, 0);//T（正转）
  gpio_init (PTB0, GPO,1);//电机使能
}
//==============================================================================
//  @brief      PIT0定时器量测编码器的脉冲数(单编码器版)
//  @since      v2.0
//  @note       
//  @date       2019-5-25
//==============================================================================

int16 Encoder_read_L(void)
{ 
  uint8 i=0;
  int16 DMA_cnt;
  static int16 speedcnt_L[5];
  
  //如果为真，车轮反转高电平，正转低电平   
  if (gpio_get(DIR1)) 
  {DMA_cnt=DMA_count_get(DMA_CH3);}
  else
  {DMA_cnt=-DMA_count_get(DMA_CH3);}
  
  //通道计数清零
  DMA_count_reset(DMA_CH3);
  DMA_EN(DMA_CH3);
  for(i=0;i<4;i++)
  {
    speedcnt_L[i]=speedcnt_L[i+1];    
    
  }
  
  speedcnt_L[4]=DMA_cnt;    //更新当前的计数值  
  //return DMA_cnt;
 return (int16)((speedcnt_L[0]+speedcnt_L[1]+speedcnt_L[2]+speedcnt_L[3]+speedcnt_L[4])/5);//
  
}
int16 Encoder_read_R(void)
{
  uint8 i=0;
  int16 DMA_cnt;
  static int16 speedcnt_R[5];
  
  //如果为真，车轮反转高电平，正转低电平   
  if (gpio_get(DIR2)) 
  {
    DMA_cnt=-DMA_count_get(DMA_CH4);
  }
  else
  {DMA_cnt=DMA_count_get(DMA_CH4);}
  
  //通道计数清零
  DMA_count_reset(DMA_CH4);
  DMA_EN(DMA_CH4);
  
  for(i=0;i<4;i++)
  {
    speedcnt_R[i]=speedcnt_R[i+1];    
    
  }
  speedcnt_R[4]=DMA_cnt;    //更新当前的计数值  
  
  //return DMA_cnt;
  return (int16)((speedcnt_R[0]+speedcnt_R[1]+speedcnt_R[2]+speedcnt_R[3]+speedcnt_R[4])/5);
}

int PWM_duty_deta_L=0;//左电机占空比分子累加值
int PWM_duty_deta_R=0;//右电机占空比分子累加值

/* 
@brief  PWM限幅
@pram   
@note   
@date   2019-6-8
*/
//#define PWM_VALUE_MAX 10000
//int PWM_limit(int pwm_value)
//{
//  if(pwm_value>=PWM_VALUE_MAX)
//  {
//    pwm_value=PWM_VALUE_MAX;//占空比分子饱和处理
//  }
//  else
//  {
//    pwm_value=pwm_value;
//  }
//  return pwm_value
//}
/* 
@brief  PWM控制电机
@pram   PWM_duty_deta_L：左占空比分子；PWM_duty_deta_R：右占空比分子。
@note   
@date   2019-6-8
*/
//void motor_Ctrl0(int PWM_duty_deta_L,int PWM_duty_deta_R)
//{
//  uint16 pwm_duty_left,pwm_duty_right;
//  
//  //左电机
//  if(PWM_duty_deta_L>=0)
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH5, 0);//反转
//    pwm_duty_left = PWM_duty_deta_L;//给电机加正转电压
//    pwm_duty_left=PWM_limit(pwm_duty_left);//占空比分子饱和处理   
//    FTM_PWM_Duty(FTM0, FTM_CH4, pwm_duty_left);//正转
//  }
//  else
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH4, 0);//正转
//    pwm_duty_left = PWM_duty_deta_L;//给电机加正转电压
//    pwm_duty_left=PWM_limit(pwm_duty_left);//占空比分子饱和处理   
//    FTM_PWM_Duty(FTM0, FTM_CH5, pwm_duty_left);//反转
//  }
////右电机
//  if(PWM_duty_deta_R>=0)
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH6, 0);//反转
//    pwm_duty_right = PWM_duty_deta_R;//给电机加正转电压
//    pwm_duty_right=PWM_limit(pwm_duty_right);//占空比分子饱和处理   
//    FTM_PWM_Duty(FTM0, FTM_CH7, pwm_duty_right);//正转
//  }
//  else
//  {
//    FTM_PWM_Duty(FTM0, FTM_CH7, 0);//正转
//    pwm_duty_right = PWM_duty_deta_R;//给电机加正转电压
//    pwm_duty_right=PWM_limit(pwm_duty_right);//占空比分子饱和处理   
//    FTM_PWM_Duty(FTM0, FTM_CH6, pwm_duty_right);//反转
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

float casu;                     //左右电机差速 
float casu_iip=0.000013;         //摩擦系数


/*=========================================================================
函数名称：motor_encoder_set
函数功能:通过casu 及期望脉冲数计算出左右电机该给的脉冲数
时间：2019年6月11日18:01:52
参数：casu 差数        Encoder_set 期望脉冲数
返回值：Speed_L  Speed_R 左右电机脉冲数
=========================================================================*/
void motor_encoder_set(int Encoder_set)
{
  int sp=0;
  if(protect_flag==0&&Encoder_set!=0)
  {
    sp=Encoder_set+Expectation_speed;   // Expectation_speed  分为三个档  30   15   0
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
  
 casu=(float)((chasu_PWM_data*1.0/1000)/2);    //chasu_PWM_data     error= -80——0——80  舵机差速 p 100  d  100  
 
 int sp=0;
 //image_finish_F_Mot
 //-6800<chasu_PWM_data<6800
 // -068<(chasu_PWM_data*1.0/10000)<0.68   P=850
 if(protect_flag==0&&Encoder_set!=0)
 {
   sp=Encoder_set+Expectation_speed;   // Expectation_speed  分为三个档  30   15   0
   sp=Encoder_set;
   Speed_L=(int)(sp*(1+casu));
   Speed_R=(int)(sp*(1-casu));    
 }

  //show_chasu=casu;
  casu=-casu;
  if(casu>0.34)  //这个算出来的是一个要增加的比例系数
  {
    casu=0.34;
  }
  if(casu<-0.34)
  {
    casu=-0.34;
  }
  
}

/*=========================================================================
函数名称：chasu_set
函数功能:通过舵机 P D 计算出来的占空比分子  casu_iip  摩擦系数
时间：2019年6月11日18:01:52
参数：Servo_PWM_deta 舵机PD计算值      casu_iip 摩擦系数
返回值：casu  
=========================================================================*/
void chasu_set()
{
  if(Servo_PWM_deta>0)  //右
  {
    casu=-(Servo_PWM_deta*Servo_PWM_deta)*casu_iip;//Servo_PWM_deta 通过舵机 P D 计算出来的占空比分子  casu_iip  摩擦系数
    /*
    计算分析：  1、Servo_PWM_deta=(int)(pd->P*y*midpoint_error + pd->D*(midpoint_error-lastpoint_error));
                2、casu_iip=0.00013;
    分析1：        最小 midpoint_error-lastpoint_error->0            midpoint_error->0
                   最大 midpoint_error-lastpoint_error->40           midpoint_error->80                     
    */
  }else
  {
    casu=(Servo_PWM_deta*Servo_PWM_deta)*casu_iip;
    
  }
  
  
  if(casu>0.34)  //这个算出来的是一个要增加的比例系数
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

 //停车||定时起步 
  int speed_L,speed_R;
  if(RunState_flag&& protect_flag==0)//与上protect_flag  当protect_flag为0时判断车在赛道内  否则为protect_flag=1
  {
  speed_L=Speed_L;//定时起步
  speed_R=Speed_R;//定时起步
  }
  else
  {
    speed_L=0;//停车
    speed_R=0;//停车
    LRT_flag=0;
    RunState_flag=0;
  }
  
  PID_set(PID_motor_L,speed_L,CntEncoder_left);//计算PID偏差  
  PWM_duty_deta_L+=(int32)PID_Calc(PID_motor_L);//累加占空比（增量式PID计算）
  if(PWM_duty_deta_L>10000)//占空比限幅
  {
    PWM_duty_deta_L=10000;
  }
  if(PWM_duty_deta_L<-10000) 
  {
    PWM_duty_deta_L=-10000;
  }
  
  PID_set(PID_motor_R,speed_R,CntEncoder_right);//计算PID偏差
  PWM_duty_deta_R+=PID_Calc(PID_motor_R);//累加占空比（增量式PID计算） 
  
  if(PWM_duty_deta_R>10000)//占空比限幅
    PWM_duty_deta_R=10000;
  if(PWM_duty_deta_R<-10000)
    PWM_duty_deta_R=-10000;
 //左电机   
  if(PWM_duty_deta_L>=0) 
  {
    FTM_PWM_Duty(FTM0, FTM_CH4, PWM_duty_deta_L);//正转
    FTM_PWM_Duty(FTM0, FTM_CH5, 0);//反转
  }
  else 
  {
    FTM_PWM_Duty(FTM0, FTM_CH5, -PWM_duty_deta_L);//反转
    FTM_PWM_Duty(FTM0, FTM_CH4, 0);//正转
  }
 //右电机    
  if(PWM_duty_deta_R>=0)
  {
    FTM_PWM_Duty(FTM0, FTM_CH6,0);//反转 
    FTM_PWM_Duty(FTM0, FTM_CH7, PWM_duty_deta_R);//正转
  }
  else 
  {
    FTM_PWM_Duty(FTM0, FTM_CH7, 0);//正转
    FTM_PWM_Duty(FTM0, FTM_CH6, -PWM_duty_deta_R);//反转 
  }
  
      //电机堵转保护
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
//    IsDown_flag=1;//开启开车倒计时标志位
//    IsBeep=1;     //蜂鸣器响提示开始倒计时
//  }
//  if(key_check(KEY_R)==KEY_DOWN && GUI_flag==0)
//  {
//    Send_Model=~Send_Model;
//  }
  
}

SV sv_l;
SV sv_r;

SV * sv_L=&sv_l;   // 根据左编码器 计算出来的 路程 速度  累计脉冲数 

SV * sv_R=&sv_r;    // 根据右编码器 计算出来的 路程 速度  累计脉冲数  

char distance_laser[50];//获取激光测距字符串


extern volatile IMG_STATUS_e ov7725_eagle_img_flag;

void PIT0_IRQHandler()
{
  
  CntEncoder_left=Encoder_read_L();     //获取左右编码器实时脉
  CntEncoder_right=Encoder_read_R();    //获取左右编码器实时脉
  
  //if(ov7725_eagle_img_flag==IMG_FINISH)
    ElecValue_dispose();                //电磁数据处理
  
  
  //----------差速--------------------
  //chasu_set();          //计算差速 二次关系
    
 chasu_yici();
  
  motor_encoder_set(Encoder_set);       //得到左右电机的期望脉冲数  一次关系
  
  sv_L->Add_CntEncoder=CntEncoder_left+sv_L->Add_CntEncoder;     //脉冲数累计值
  sv_L->S=(sv_L->Add_CntEncoder)*1.0/5300;                        //走的路程  单位m   4300   5270          10ms （Add_CntEncoder/5300）m    m*100=cm      
 // sv_L->V=(CntEncoder_left*1.0/5300)/3;                       //速度  单位 m/ms
  
  sv_R->Add_CntEncoder=CntEncoder_right+sv_R->Add_CntEncoder;     //脉冲数累计值
  sv_R->S=(sv_R->Add_CntEncoder)*1.0/5300;                        //走的路程  单位m    4300   5270
 // sv_R->V=(CntEncoder_right*1.0/5300)/3;                       //速度  单位 m/ms
  
   Encoder_inc=CntEncoder_right+Encoder_inc;
   
   

  
 motor_Ctrl();//电机控制

  //Send_16bitData(Encoder_set,CntEncoder_left,PWM_duty_deta_L);//看电机响应必须在中断里面发送波形，否则波形不平滑，且横轴时间间隔也不一样
  
  PIT_Flag_Clear(PIT0);       //清中断标志位
  

}




int error_be=0;
int time=0;//进中断次数
uint8 Timer_n=0;//计数变量
int Image_num=0;//图像计数
uint8 image_num_kaiqi=0;    //    image_num_kaiqi  为 1 开始  Image_num 
int Image_num1=0;
void PIT1_IRQHandler(void)
{
  
  Image_num++;                          //图像使用
  Image_num1++;                         //图像使用
  speed_dingshi++;                      //图像控制速度使用
  if(Image_num>9999)Image_num=0;
  if(Image_num1>9999)Image_num1=0;
  if(speed_dingshi>999)speed_dingshi=0;
  if(IsDown_flag==1)
  {
    if(++time==200)
    {
      time=0;
      RunState_flag=1;//按键按下200*10ms后进入运行状态
      IsDown_flag=0;//清除按键标志
      IsBeep=0;     //蜂鸣器关提示倒计时结束
    }    
  }
  if(RunState_flag==1)
  {
//    if(++time==300 && Send_Model==1)//定时开车模式
//    {
//      RunState_flag=0;
//    }
    if(++time==300)//300*10ms
    {
      Started_check_Zebra=1;//开车后倒计时3秒，有斑马线就停车
      LRT_flag=1;//开车后倒计时3秒，开始检测电机是否有堵转
    }
    else;
    
    
  }
//  if(Send_Model==254)
//  {
//    Send_16bitData(error,(int)(100*show_chasu),elec_M);
//    error_be=error;
//  }
  //Send_16bitData(Encoder_set,CntEncoder_right,PWM_duty_deta_R);//看电机响应必须在中断里面发送波形，否则波形不平滑，且横轴时间间隔也不一样
  //Send_16bitData_var6(Encoder_set,CntEncoder_right,PWM_duty_deta_R,CntEncoder_left,PWM_duty_deta_L,Encoder_set);
  //Send_16bitData(Encoder_set,CntEncoder_left,PWM_duty_deta_L);//看电机响应必须在中断里面发送波形，否则波形不平滑，且横轴时间间隔也不一样
  PIT_Flag_Clear(PIT1);       //清中断标志位
}

/*!
 *  @brief      UART0测试中断服务函数
 *  @since      v5.0
 *  @warning    此函数需要用户根据自己需求完成，这里仅仅是提供一个模版
 *  Sample usage:       set_vector_handler(UART0_RX_TX_VECTORn , uart0_test_handler);    //把 uart0_handler 函数添加到中断向量表，不需要我们手动调用
 */

int op=0;
int valuejuli=0;
char s[20];   //接收到的字符串
int klkl=0;
void uart2_test_handler(void)
{
  uart_rx_irq_dis(UART2); //关串口0接收中断
  klkl=1;
  int pnum=0;
  //int shuF=0;
 
  for(int i=19;i>0;i--)    //从 0往后覆盖     
  {
    s[i]=s[i-1];
  }
 // UARTn_e uratn = UART2;
  uart_getchar(UART2,&s[0]); //读取接收的数组
   
  
  
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
  
  
  uart_rx_irq_en(UART2);         //开串口0接收中断
  
}


/*
@note      所有摄像头初始化函数（内部调用）
@brief     用于本页系统初始化函数调用
@since     v1.0
*/
void camera_all_init(void)
{
  //设置使用摄像头场中断（行中断），中断服务函数为PORTD_IRQHandler
  set_vector_handler(PORTD_VECTORn , PORTD_IRQHandler);  
  
  //设置DMA0中断服务函数为DMA0_IRQHandler
  set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);   

  //初始化摄像头 
  camera_init(imgbuff);  //？？这句语句是否有要求顺序放在前两句中断设置之后，有待测试 

}

/*
 @note      DMA0中断服务函数（内部调用）
 @brief     用于清除通道摄像头的DMA采集通道: DMA_CH0 
 @since     v1.0
*/
void DMA0_IRQHandler(void)
{
   //--让图像接受完毕   清除通道  
    camera_dma();      
}
/*
 @note       PORTD中断服务函数（内部调用）
 @brief      仅使用摄像头场中断，不使用行中断，且行中断函数在VCAN_OV7725_Eagle.c中没
             有定义。
 @since      v1.0
*/
void PORTD_IRQHandler(void)
{
    uint8  n;    //引脚号
    uint32 flag;

   while(!PORTD_ISFR);
    flag = PORTD_ISFR;
    PORTD_ISFR  = ~0;                                   //清中断标志位

    n = 0;                                             //使用场中断
    if(flag & (1 << n))                                //PTD0触发中断
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //不使用行中断
    n = 18;
    if(flag & (1 << n))                                 //PTC18触发中断
    {
        camera_href();
    }
#endif
}


//=============================定义本页的函数end================================