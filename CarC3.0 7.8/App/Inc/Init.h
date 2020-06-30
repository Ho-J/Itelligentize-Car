#ifndef _INIT_H
#define _INIT_H
#include "include.h"
#include "common.h"
#include "PID.h"

//============#���ⲿ���õı����ӿ�����Start===============//

extern int Servo_P;//���PDֵ
extern int Servo_D;//���PDֵ
extern int Motor_P_L;//���PIDֵ
extern int Motor_I_L;//���PIDֵ
extern int Motor_D_L;//���PIDֵ
extern int Motor_P_R;//���PIDֵ
extern int Motor_I_R;//���PIDֵ
extern int Motor_D_R;//���PIDֵ
extern int Encoder_set;//�趨������

extern int16 CntEncoder;//��������������Ϊ��ǰ�ٶȵ�������


extern PID *PID_Servo;//���PID�ṹ��
extern PID *Motor_chasu_PID;//����PID�ṹ��

extern uint8 Send_Model;
extern uint16  PowerAd; //��ص�ѹ���#����GUI��ʾ
extern uint8 Send_Model;//����ģʽ
extern uint8 RunState_flag;
extern uint8 Servo_Model;//���PD�㷨ģʽ
extern int THOD;//ͼ����ֵ
extern int Image_num;//ͼ����� ������
extern int Image_num1;//ͼ�����     ��·
extern int PWM_duty_deta_L;//����ռ�ձȷ����ۼ�ֵ
extern int PWM_duty_deta_R;//�ҵ��ռ�ձȷ����ۼ�ֵ
extern int16  CntEncoder_left;//����10ms�ɼ���������
extern int16  CntEncoder_right;//�ҵ��10ms�ɼ���������
extern int Encoder_inc;       //�ۼ�������
extern int Image_num;//ͼ�������100ms��
extern uint8 image_num_kaiqi;    //    image_num_kaiqi  Ϊ 1 ��ʼ 
extern int valuejuli;

typedef struct sv{              //�� ·�� �ٶ�  �ۼ������� ��ֵ
  float S;                //·�� ��λ  ����  mm
  float V;                //�ٶ�   m/s
  int Add_CntEncoder;   //�ۼ���������ֵ
}SV;

extern SV * sv_L;   // ����������� ��������� ·�� �ٶ�  �ۼ������� 

extern SV * sv_R;    // �����ұ����� ��������� ·�� �ٶ�  �ۼ�������     

extern uint8 IsDown_flag;

extern char distance_laser[50];

extern int Laser_Value;   //����⵽�ľ���  ��λ��mm


typedef struct xian                     //  ��   �洢��߽� ���� �ұ߽� �����ֵ
{
  unsigned char border[2][100];         //�洢��ά����  �߽�
  unsigned char number;                 //��Ч��
  unsigned char flag;                   //�洢�����ı�־λ
  struct xian *next;//�洢��һ���ṹ���ַ
}Xian;       
typedef struct xianp
{
  Xian *front;  //ͷָ��
  Xian *rear;  //βָ��
  int number;
}Xianp;

extern Xianp *xianpleft;        //��߽�ͷβָ��
extern Xianp *xianpright;       //�ұ߽�ͷβָ��             
extern Xianp *xianpcenter;      //����ͷβָ��


extern int chasu_P;    //���� P
extern int chasu_D;    //���� D
//============���ⲿ���õı����ӿ�����End=================//

//=============================//�ڲ���������Start//============================
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
    
  void  motor_Ctrl();//�������
//=============================//�ڲ���������End//==============================





//============#���ⲿ���õĺ����ӿ�����Start#===============//


//ϵͳ��ʼ������������main��������
extern void System_Init(void);


//============���ⲿ���õĺ����ӿ�����End=================//

#endif
