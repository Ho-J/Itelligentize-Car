#include "PID.h"
#include "include.h"
#include "Fuzzy.h"

//===========================================
//              PID������ʼ��
//Sample usage: 
//  PID motor;
//  PID *P=&motor;
//  PID_init(p);
//===========================================


void PID_init(struct PID *spid)
{
  spid->sumError=0;     //����ۼ�
  spid->lastError=0;    //ǰһ����� Error[-1]
  spid->preError=0;     //ǰ������� Error[-2]
  spid->P=0;            //�������� Proportional Const
  spid->I=0;            //���ֳ��� Integral Const
  spid->D=0;            //΢�ֳ��� Derivative Const
  spid->getError=0;     //ƫ��ֵ/����getError��setpoint�ʺ�������ǰ�Ĺ�ϵ
  
}

//===========================================
//              ����ƫ��
//Sample usage:
//  PID *P=&motor;
//  PID_set(p,a,b);
//  aΪ�趨(����)ֵ��bΪʵʱ����ֵ
//===========================================

void PID_set(struct PID *spid,int a,int b)
{   
    spid->getError=a-b;     // ���ƫ��
}

//===========================================
//              ����ʽPID������ã�
//      @date:2018-4-23
//      @brief:
//===========================================
    
 int e_0,e_1,e_2;
 float inc; 
float PID_Calc(PID *pd)
{ 
 //PID����ʽ���� 
  e_0=pd->getError;
  e_1=pd->lastError;
  e_2=pd->preError;
 
//PID����ʽ
  inc=pd->P*(e_0-e_1)+pd->I*e_0+pd->D*(e_0-2*e_1+e_2); 
    //ƫ��洢��//�����´Ρ����´μ���
 
  pd->preError=pd->lastError;
  pd->lastError=e_0;
   //ƫ��洢������ 
  return inc;
}

//===========================================
//              ����P
//===========================================
void PID_kp(struct PID *spid,float setkp)
{
  spid->P=setkp;
}

//===========================================
//              ����I
//===========================================
void PID_ki(struct PID *spid,float setki)
{
  spid->I=setki;
}

//===========================================
//              ����D
//===========================================
void PID_kd(struct PID *spid,float setkd)
{
  spid->D=setkd;
}

//===========================================
//              λ��ʽPID������ã�+ģ��
//      @date:2018-4-23
//      @brief:
//===========================================
FuzzyStruct Servo_p={
  Fuzzy_Rank7,
  0,
  0,
  {-90,-50,-20,0,20,50,90},
  {-40,-20,-8,0,8,20,40},
  {0.15,0.20,0.25,0.35,0.4,0.5,0.7},//�賿3��//�����С�����0.65���ܿ���,���׶��˲Ż�����
//P5.6 D11-13//ǰհ35��(���46��) Encoder150 
    
  //{-20,-8,-4,0,4,8,20}, 
 //{0.2,0.25,0.35,0.53,0.56,0.75,0.85},//4�㣬Сs����
 //{0.1,0.15,0.35,0.53,0.56,0.75,0.85},//������е��  P5.6 D13
 //{0.1,0.15,0.35,0.45,0.50,0.75,0.85},
  
  
  
  
  //{0.1,0.15,0.2,0.25,0.3,0.5,0.6},//19��14�� //19��14�� �����С�����0.65���ܿ���
   //{0.05,0.1,0.2,0.25,0.3,0.45,0.55},//�����С�����0.65���ܿ��� 
  
//{-20,-8,-4,0,4,8,20},
//{-40,-20,-8,0,8,20,40},
//{0.25,0.3,0.35,0.45,0.5,0.6,0.65}, //ֱ���������������ȶ�
  
//{-40,-18,-6,0,6,18,40},
//{0.2,0.25,0.3,0.35,0.4,0.6,0.65},




{
  

    {6,5,4,3,3,2,1},        
    {5,4,3,2,2,1,2},        
    {4,3,2,1,1,2,3},       
    {3,2,1,0,1,2,3},      
    {3,2,1,1,2,3,4},      
    {2,1,2,2,3,4,5},      
    {1,2,3,3,4,5,6},                    //ԭ���
  
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


   int8    midpoint_error=0;   //�������
   int8   lastpoint_error=0;   //�ϴ����
   int Servo_PWM;       //������ʾ���PWM
   float z=1,y=1;//���Ҳ���
   int ServoCentreValue=1913;    //�����ֵ     //1831(����ƫ)  144  1531  156  1275
   
   int Servo_PWM_deta;          //��� ͨ����� P D ���������ռ�ձȷ���
   
   int chasu_PWM_data;          //ͨ��ƫ��PD����ó��Ŀ�ռ�ձȷ��Ӹ������
   
void Servo_PID_Ctrl(PID *pd,int8 Error)//+Fuzzy_Update(&Servo_p,midpoint_error,(midpoint_error-lastpoint_error))//���Բ���ģ���Ƚ��ȶ���������ƫ��仯�ʵ�����
{    
  if(Error>=0)y=1.0;//���Ҳ��ԳƵ������ //�ұ��ǲ��ǹմ���
  else y=1.0;
  z=1.0;
  lastpoint_error=midpoint_error;
  midpoint_error=Error;
  
  //PID����ʽ����
  
  //ƫ��洢��//�����´Ρ����´μ���
  //lastpoint_error=Error;
  //ƫ��洢������
  Servo_PWM_deta=(int)(pd->P*y*midpoint_error + pd->D*(midpoint_error-lastpoint_error));        //ͨ����� P D ���������ռ�ձȷ���  
  //Servo_PWM=ServoCentreValue + Servo_PWM_deta;//DΪ0 PΪ2.5    PΪ0  DΪ5
  
  Servo_PWM=(int)(pd->P*y*midpoint_error + pd->D*(midpoint_error-lastpoint_error)+ServoCentreValue);       
  //���  P D  ����
  chasu_PWM_data=(int)(Motor_chasu_PID->P*midpoint_error+Motor_chasu_PID->D*2*(midpoint_error-lastpoint_error));   //ͨ��ƫ�� PID ���� �������
  //���� P D ����  
  
  
  if(Servo_PWM<ServoCentreValue-Servo_limit)            //����޷�
    Servo_PWM=ServoCentreValue-Servo_limit;
  else if(Servo_PWM>ServoCentreValue+Servo_limit)
    Servo_PWM=ServoCentreValue+Servo_limit;

  FTM_PWM_Duty(FTM1,FTM_CH1,Servo_PWM);//���PWM
}