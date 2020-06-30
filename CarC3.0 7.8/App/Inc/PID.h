#ifndef __PID_H
#define __PID_H
#include "common.h"
#define Servo_limit 180

typedef struct PID              
{
  int getError;                //ƫ��ֵ
  int sumError;              //����ۼ�
  
  float P;     //�������� Proportional Const
  float I;     //���ֳ��� Integral Const
  float D;     //΢�ֳ��� Derivative Const
  
  int lastError;       //ǰһ����� Error[-1]
  int preError;        //ǰ������� Error[-2]
  
}PID;


//========================�ڲ���������Start================//
void PID_init(struct PID *spid);
float PID_Calc(PID *pd);
void PID_set(struct PID *spid,int a,int b);
void PID_kp(struct PID *spid,float setkp);
void PID_ki(struct PID *spid,float setki);
void PID_kd(struct PID *spid,float setkd);
//========================�ڲ���������End================//

//============#���ⲿ���õĺ����ӿ�����Start#===============//
extern void Servo_PID_Ctrl(PID *pd,int8 Error);

//============#���ⲿ���õĺ����ӿ�����End#=================//


//========================�ڲ���������Start================//
extern int ServoCentreValue;
extern int Servo_PWM;
extern int Servo_PWM_deta;  //ͨ����� P D ���������ռ�ձȷ���
extern int chasu_PWM_data;          //ͨ��ƫ��PD����ó��Ŀ�ռ�ձȷ��Ӹ������
//========================�ڲ���������End================//











#endif