#ifndef _INIT_H
#define _INIT_H
#include "include.h"
#include "common.h"
#include "PID.h"

//============#给外部调用的变量接口声明Start===============//

extern int Servo_P;//舵机PD值
extern int Servo_D;//舵机PD值
extern int Motor_P_L;//电机PID值
extern int Motor_I_L;//电机PID值
extern int Motor_D_L;//电机PID值
extern int Motor_P_R;//电机PID值
extern int Motor_I_R;//电机PID值
extern int Motor_D_R;//电机PID值
extern int Encoder_set;//设定脉冲数

extern int16 CntEncoder;//编码器计数，即为当前速度的脉冲数


extern PID *PID_Servo;//电机PID结构体
extern PID *Motor_chasu_PID;//差速PID结构体

extern uint8 Send_Model;
extern uint16  PowerAd; //电池电压检测#用于GUI显示
extern uint8 Send_Model;//运行模式
extern uint8 RunState_flag;
extern uint8 Servo_Model;//舵机PD算法模式
extern int THOD;//图像阈值
extern int Image_num;//图像计数 蜂鸣器
extern int Image_num1;//图像计数     断路
extern int PWM_duty_deta_L;//左电机占空比分子累加值
extern int PWM_duty_deta_R;//右电机占空比分子累加值
extern int16  CntEncoder_left;//左电机10ms采集的脉冲数
extern int16  CntEncoder_right;//右电机10ms采集的脉冲数
extern int Encoder_inc;       //累计脉冲数
extern int Image_num;//图像计数（100ms）
extern uint8 image_num_kaiqi;    //    image_num_kaiqi  为 1 开始 
extern int valuejuli;

typedef struct sv{              //存 路程 速度  累计脉冲数 的值
  float S;                //路程 单位  毫米  mm
  float V;                //速度   m/s
  int Add_CntEncoder;   //累计脉冲数的值
}SV;

extern SV * sv_L;   // 根据左编码器 计算出来的 路程 速度  累计脉冲数 

extern SV * sv_R;    // 根据右编码器 计算出来的 路程 速度  累计脉冲数     

extern uint8 IsDown_flag;

extern char distance_laser[50];

extern int Laser_Value;   //激光测到的距离  单位：mm


typedef struct xian                     //  线   存储左边界 中线 右边界 数组的值
{
  unsigned char border[2][100];         //存储二维数组  边界
  unsigned char number;                 //有效数
  unsigned char flag;                   //存储其他的标志位
  struct xian *next;//存储下一个结构体地址
}Xian;       
typedef struct xianp
{
  Xian *front;  //头指针
  Xian *rear;  //尾指针
  int number;
}Xianp;

extern Xianp *xianpleft;        //左边界头尾指针
extern Xianp *xianpright;       //右边界头尾指针             
extern Xianp *xianpcenter;      //中线头尾指针


extern int chasu_P;    //差速 P
extern int chasu_D;    //差速 D
//============给外部调用的变量接口声明End=================//

//=============================//内部函数声明Start//============================
void DMA0_IRQHandler(void);
void PORTD_IRQHandler(void);
void uart2_test_handler();
void camera_all_init(void);
void PIT0_IRQHandler();
void PIT1_IRQHandler(void);
void encoder_init(void);
void motor_init(void);
void Sample_period_init(void);
void motor_Ctrl();
 void    chasu_yici();
 void chasu_set();
void motor_encoder_set(int Encoder_set);
    
  void  motor_Ctrl();//电机控制
//=============================//内部函数声明End//==============================





//============#给外部调用的函数接口声明Start#===============//


//系统初始化函数，用于main函数调用
extern void System_Init(void);


//============给外部调用的函数接口声明End=================//

#endif
