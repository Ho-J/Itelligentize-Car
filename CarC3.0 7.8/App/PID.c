#include "PID.h"
#include "include.h"
#include "Fuzzy.h"

//===========================================
//              PID参数初始化
//Sample usage: 
//  PID motor;
//  PID *P=&motor;
//  PID_init(p);
//===========================================


void PID_init(struct PID *spid)
{
  spid->sumError=0;     //误差累记
  spid->lastError=0;    //前一拍误差 Error[-1]
  spid->preError=0;     //前两拍误差 Error[-2]
  spid->P=0;            //比例常数 Proportional Const
  spid->I=0;            //积分常数 Integral Const
  spid->D=0;            //微分常数 Derivative Const
  spid->getError=0;     //偏差值/觉得getError比setpoint适和描述当前的关系
  
}

//===========================================
//              计算偏差
//Sample usage:
//  PID *P=&motor;
//  PID_set(p,a,b);
//  a为设定(期望)值，b为实时反馈值
//===========================================

void PID_set(struct PID *spid,int a,int b)
{   
    spid->getError=a-b;     // 电机偏差
}

//===========================================
//              增量式PID（电机用）
//      @date:2018-4-23
//      @brief:
//===========================================
    
 int e_0,e_1,e_2;
 float inc; 
float PID_Calc(PID *pd)
{ 
 //PID计算式结束 
  e_0=pd->getError;
  e_1=pd->lastError;
  e_2=pd->preError;
 
//PID计算式
  inc=pd->P*(e_0-e_1)+pd->I*e_0+pd->D*(e_0-2*e_1+e_2); 
    //偏差存储器//用于下次、下下次计算
 
  pd->preError=pd->lastError;
  pd->lastError=e_0;
   //偏差存储器结束 
  return inc;
}

//===========================================
//              设置P
//===========================================
void PID_kp(struct PID *spid,float setkp)
{
  spid->P=setkp;
}

//===========================================
//              设置I
//===========================================
void PID_ki(struct PID *spid,float setki)
{
  spid->I=setki;
}

//===========================================
//              设置D
//===========================================
void PID_kd(struct PID *spid,float setkd)
{
  spid->D=setkd;
}

//===========================================
//              位置式PID（舵机用）+模糊
//      @date:2018-4-23
//      @brief:
//===========================================
FuzzyStruct Servo_p={
  Fuzzy_Rank7,
  0,
  0,
  {-90,-50,-20,0,20,50,90},
  {-40,-20,-8,0,8,20,40},
  {0.15,0.20,0.25,0.35,0.4,0.5,0.7},//凌晨3点//急弯拐小了最大0.65可能可以,这套抖了才换下面
//P5.6 D11-13//前瞻35行(宽度46列) Encoder150 
    
  //{-20,-8,-4,0,4,8,20}, 
 //{0.2,0.25,0.35,0.53,0.56,0.75,0.85},//4点，小s反向抖
 //{0.1,0.15,0.35,0.53,0.56,0.75,0.85},//弯道内有点大  P5.6 D13
 //{0.1,0.15,0.35,0.45,0.50,0.75,0.85},
  
  
  
  
  //{0.1,0.15,0.2,0.25,0.3,0.5,0.6},//19点14分 //19点14分 急弯拐小了最大0.65可能可以
   //{0.05,0.1,0.2,0.25,0.3,0.45,0.55},//急弯拐小了最大0.65可能可以 
  
//{-20,-8,-4,0,4,8,20},
//{-40,-20,-8,0,8,20,40},
//{0.25,0.3,0.35,0.45,0.5,0.6,0.65}, //直道抖，不过还算稳定
  
//{-40,-18,-6,0,6,18,40},
//{0.2,0.25,0.3,0.35,0.4,0.6,0.65},




{
  

    {6,5,4,3,3,2,1},        
    {5,4,3,2,2,1,2},        
    {4,3,2,1,1,2,3},       
    {3,2,1,0,1,2,3},      
    {3,2,1,1,2,3,4},      
    {2,1,2,2,3,4,5},      
    {1,2,3,3,4,5,6},                    //原电磁
  
//    {6,5,4,3,3,2,1},   
//    {5,4,3,2,2,1,2},        
//    {4,4,2,1,1,2,3},        
//    {3,2,1,0,1,2,3},       
//    {3,2,1,1,2,4,4},       
//    {2,1,2,2,3,4,5},      
//    {1,2,3,3,4,5,6},        
    
    
//    {6,5,3,3,3,2,1},
//    {5,4,2,2,2,1,2},
//    {4,4,2,1,1,2,3},
//    {3,2,1,0,1,2,3},
//    {3,2,1,1,2,4,4},
//    {2,1,2,2,2,4,5},
//    {1,2,3,3,3,5,6},
    
    
}
                      };


   int8    midpoint_error=0;   //中线误差
   int8   lastpoint_error=0;   //上次误差
   int Servo_PWM;       //用于显示舵机PWM
   float z=1,y=1;//左右补偿
   int ServoCentreValue=1913;    //舵机中值     //1831(左满偏)  144  1531  156  1275
   
   int Servo_PWM_deta;          //舵机 通过舵机 P D 计算出来的占空比分子
   
   int chasu_PWM_data;          //通过偏差PD计算得出的控占空比分子给到电机
   
void Servo_PID_Ctrl(PID *pd,int8 Error)//+Fuzzy_Update(&Servo_p,midpoint_error,(midpoint_error-lastpoint_error))//测试不加模糊比较稳定，估计是偏差变化率的问题
{    
  if(Error>=0)y=1.0;//左右不对称的情况下 //右边是不是拐大了
  else y=1.0;
  z=1.0;
  lastpoint_error=midpoint_error;
  midpoint_error=Error;
  
  //PID计算式结束
  
  //偏差存储器//用于下次、下下次计算
  //lastpoint_error=Error;
  //偏差存储器结束
  Servo_PWM_deta=(int)(pd->P*y*midpoint_error + pd->D*(midpoint_error-lastpoint_error));        //通过舵机 P D 计算出来的占空比分子  
  //Servo_PWM=ServoCentreValue + Servo_PWM_deta;//D为0 P为2.5    P为0  D为5
  
  Servo_PWM=(int)(pd->P*y*midpoint_error + pd->D*(midpoint_error-lastpoint_error)+ServoCentreValue);       
  //舵机  P D  计算
  chasu_PWM_data=(int)(Motor_chasu_PID->P*midpoint_error+Motor_chasu_PID->D*2*(midpoint_error-lastpoint_error));   //通过偏差 PID 控制 计算差速
  //差速 P D 计算  
  
  
  if(Servo_PWM<ServoCentreValue-Servo_limit)            //输出限幅
    Servo_PWM=ServoCentreValue-Servo_limit;
  else if(Servo_PWM>ServoCentreValue+Servo_limit)
    Servo_PWM=ServoCentreValue+Servo_limit;

  FTM_PWM_Duty(FTM1,FTM_CH1,Servo_PWM);//输出PWM
}