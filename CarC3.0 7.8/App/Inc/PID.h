#ifndef __PID_H
#define __PID_H
#include "common.h"
#define Servo_limit 180

typedef struct PID              
{
  int getError;                //偏差值
  int sumError;              //误差累计
  
  float P;     //比例常数 Proportional Const
  float I;     //积分常数 Integral Const
  float D;     //微分常数 Derivative Const
  
  int lastError;       //前一拍误差 Error[-1]
  int preError;        //前两拍误差 Error[-2]
  
}PID;


//========================内部函数声明Start================//
void PID_init(struct PID *spid);
float PID_Calc(PID *pd);
void PID_set(struct PID *spid,int a,int b);
void PID_kp(struct PID *spid,float setkp);
void PID_ki(struct PID *spid,float setki);
void PID_kd(struct PID *spid,float setkd);
//========================内部函数声明End================//

//============#给外部调用的函数接口声明Start#===============//
extern void Servo_PID_Ctrl(PID *pd,int8 Error);

//============#给外部调用的函数接口声明End#=================//


//========================内部变量声明Start================//
extern int ServoCentreValue;
extern int Servo_PWM;
extern int Servo_PWM_deta;  //通过舵机 P D 计算出来的占空比分子
extern int chasu_PWM_data;          //通过偏差PD计算得出的控占空比分子给到电机
//========================内部函数声明End================//











#endif