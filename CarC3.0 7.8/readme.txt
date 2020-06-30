#2019-5-9 �����ڵ�LCD_ST7735R��ֲ�����ڵ�LCDST7735S(����RightNow_chaser_(void)����)
��ֲ�ߣ����ջ�
//LCD_ST7735S �� LCD_ST7735R ���һ�����Ĵ����������в�ͬ

����1���޸�ʹ�õ���ʾ��
�޸�λ��1��APP/MK60_conf.h
#define USE_LCD        LCD_ST7735R            //ѡ��ʹ�õ� LCD
�޸�Ϊ
#define USE_LCD        LCD_ST7735S            //ѡ��ʹ�õ� LCD

����2������ʾ����ʼ����ӵ� ϵͳ��ʼ������ ��
λ�ã�App/init.c/System_Init����
������ݣ�LCD_init();//LCD��ʼ��

����3��ȫ������һ���
����LCD_init�����Ķ��崦���ں��������ҵ�LCD_INIT�����������������Ķ��崦���������������滻��LCD_ST7735S_init�����Ķ��崦
�����ĺ�������
ע�͵�������䣺����ݼ���ѡ�����ݺ�Ctrl + K��
    uint8 n ;

    for(n = 0; n < 8; n++)
    {
        gpio_init  ((PTXn_e)(PTC0 + n), GPO, 0);
    }

����4���޸����ţ��򿪹����ļ����µ�1 SCH�ļ����µ�car.pdfԭ��ͼ��
�Ҷ�Ӧ����ʾ��ST7735S�����ţ�
Lcd SCL���Ŷ�Ӧ���ļ���LCD_ST7735S_WR����
MOSI(SDA)���Ŷ�Ӧ���ļ�LCD_ST7735S_RD����
CS���Ŷ�Ӧ���ļ���     LCD_ST7735S_CS����
DC���Ŷ�Ӧ���ļ���     LCD_ST7735S_RS����
RES���Ŷ�Ӧ���ļ���    LCD_ST7735S_RST����

���ڱ������ſ�Ҫ�ɼӿɲ��ӣ����������Ƿ��б��⣬û����ȡ��ע�ͣ�
//#define     LCD_ST7735S_BL      PTC2    //����

���ŷ���VCAN_LCD_ST7735S.hͷ�ļ��޸����ţ�

�޸�λ�ã�Board/VCAN_LCD_ST7735S.h
//LCD �ĹܽŶ���
#define     LCD_ST7735S_WR      PTC9
#define     LCD_ST7735S_RD      PTC10
#define     LCD_ST7735S_CS      PTC11
#define     LCD_ST7735S_RS      PTC12
#define     LCD_ST7735S_RST     PTC13
�޸�Ϊ
//LCD �ĹܽŶ���
#define     LCD_ST7735S_WR      PTB21   //Lcd SCL
#define     LCD_ST7735S_RD      PTB22   //MOSI(SDA)
#define     LCD_ST7735S_CS      PTC1    //CS
#define     LCD_ST7735S_RS      PTC0    //DC
#define     LCD_ST7735S_RST     PTB23   //RES

//δ����� ʱ�䣺2019-5-9 22��45��

����5��������LCD��/д�����޸ĳɴ��ڵ�
�޸�λ�ã�Board/VCAN_LCDST7735S.c��LCD_ST7735S_wr_data������
�޸�����1��
void LCD_ST7735S_wr_data(uint8 data)
{
    LCD_ST7735S_RD_OUT = 1;
    LCD_ST7735S_RS_OUT = 1;
    LCD_ST7735S_CS_OUT = 0;

    LCD_ST7735S_P0 = data;

    ST7735S_DELAY();

    LCD_ST7735S_WR_OUT = 0;
    LCD_ST7735S_WR_OUT = 1;
    LCD_ST7735S_CS_OUT = 1;
}
�޸�Ϊ��
//д����
void LCD_ST7735S_wr_data(uint8 dat)
{
  uint32 i;
  LCD_ST7735S_CS_OUT = 0;
  LCD_ST7735S_RS_OUT = 1;
  for(i=8;i!=0;i--)
  {			  
    LCD_ST7735S_WR_OUT = 0;
    LCD_ST7735S_RD_OUT = !!(dat&0x80);
    LCD_ST7735S_WR_OUT = 1;
    dat<<=1;
    
  }
  LCD_ST7735S_CS_OUT = 1;
}

�޸�����2��
void LCD_ST7735S_wr_cmd(uint8 cmd)
{
    LCD_ST7735S_RD_OUT = 1;
    LCD_ST7735S_RS_OUT = 0;
    LCD_ST7735S_CS_OUT = 0;

    LCD_ST7735S_P0 = cmd;

    ST7735S_DELAY();
    LCD_ST7735S_WR_OUT = 0;
    LCD_ST7735S_WR_OUT = 1;
    LCD_ST7735S_CS_OUT = 1;
}
�޸�Ϊ��

//д����
void LCD_ST7735S_wr_cmd(uint8 cmd)
{

  uint32 i;
  LCD_ST7735S_CS_OUT = 0;
  LCD_ST7735S_RS_OUT = 0;
  for(i=8;i!=0;i--)
  {			  
    LCD_ST7735S_WR_OUT = 0;
    LCD_ST7735S_RD_OUT = !!(cmd&0x80);
    LCD_ST7735S_WR_OUT = 1;
    cmd<<=1;
    
  }	
  LCD_ST7735S_CS_OUT = 1;
}

����6���޸ĺ궨��
�޸�λ�ã�Board/VCAN_LCDST7735S.h
�޸�����1��
#define LCD_ST7735S_WR_8CMD(cmd)    do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=0;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0=(uint8)(cmd);\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;����һ��������

#define LCD_ST7735S_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=1;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0=(uint8)(data);\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;������д�����ݵ�RAM


#define LCD_ST7735S_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((uint8)((data)>>8) );LCD_ST7735S_WR_8DATA( (uint8)(data) );}while(0)       //д����

�޸�Ϊ��
ע�͵�����ĺ궨��

�޸�����2��
//#define LCD_WR_8DATA(data)      LCD_ST7735S_wr_data(data)       //д����
//#define LCD_WR_8CMD(cmd)        LCD_ST7735S_wr_cmd(cmd)         //����
�޸�Ϊ��
#define LCD_ST7735S_WR_8DATA(data)      LCD_ST7735S_wr_data(data)       //д����
#define LCD_ST7735S_WR_8CMD(cmd)        LCD_ST7735S_wr_cmd(cmd)         //д����


��ֲ����ʱ�䣺2019-5-11 21��42��


//2019-5-24 ��������521��mini����������������ʽ �����������룩�������Ա��ҵ�����
1 ��ԭ��ͼ��car.pdf
��ԭ��ͼ��֪P6��������
LSB 2����������裬��LSB2�ӳ����оƬ��PTB18����
DIR 2����������裬��DIR2�ӳ����оƬ��PTB19����

2 �޸ĳ����е����ţ�
·����App\Inc\PORT_cfg.h
//��������ģ��ͨ��  �˿�          ��ѡ��Χ              ����
#define FTM2_QDPHA  PTB18       //PTA10��PTB18
#define FTM2_QDPHB  PTB19       //PTA11��PTB19

3 ��ɽ����ļ�����������ģ����ôʹ�ã�
·����Chip\src\MK60_ftm.c
1)��ʼ��FTM ���������� ����
2)��ȡFTM �������� ��������
3)�� FTM �������� ��������

4 ��ʼ��FTM ���������� ����
·����App\Inc\Init.c
��ϵͳ��ʼ�������е��� ��ʼ��FTM ���������� ����
���÷�ʽ��
void System_Init(void)
{	 
  FTM_QUAD_Init(FTM2); //��ʼ�� FTM2 Ϊ��������ģʽ
}

5 �����һ������������ȫ�ֱ���
·����App\main.c
�����������涨�������int16 count=0;//�洢������

6 ��ȡFTM �������� ��������
·����App\main.c
����������while(1)��ѭ����������� ��ȡFTM �������� ��������
���÷�ʽ��
void  main(void)
{
  
  System_Init();//ϵͳ��ʼ��

  while(1)
  {  
      count = FTM_QUAD_get(FTM2);// //��ȡ  FTM2 ������ ��������
  }
}

##���˾Ϳ��Բ����Ƿ���Ի�ȡ����������������
#����һ�����س��򲢷��棨Download and Debug����ȫ�����У��Ǳ�������ת�������鿴count�Ƿ���ֵ��
#���������������ϵģ�LCD��ʾ����count��ֵ��ʾ������

7 �� FTM �������� ��������
·����App\main.c
����������while(1)��ѭ����������� ��ȡFTM �������� ��������֮���ٵ��� �� FTM �������� ��������
void  main(void)
{
  
  System_Init();//ϵͳ��ʼ��

  while(1)
  {  
    
      count = FTM_QUAD_get(FTM2);
      FTM_QUAD_clean(FTM2);
  }
}

//2019-6-6 ���ջ�
0 ��RightNow_chaser_CarC_Encoder�����ļ��޸�

1 ��main.c�а���GUI.h

2 ��init.c����ӱ��������ŵĺ궨��

#define LSB1 PTA12
#define DIR1 PTA13
#define LSB2 PTB18
#define DIR2 PTB19

2-1 ��ӵ��ת�ٱջ���PID�����ṹ��
PID Var_PID_motor_L;
PID *PID_motor_L=&Var_PID_motor_L;
PID Var_PID_motor_R;
PID *PID_motor_R=&Var_PID_motor_R;

2-2 ��ӽ��ձ������������ı���
int16  CntEncoder_left;
int16  CntEncoder_right;

3 ����System_Init�����м���
  pit_init_ms(PIT1, 10);                                
  set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);      
  enable_irq (PIT1_IRQn); 

3-1 ��ӵ����ʼ������������System_Init�����е��ã�����ǰ��inic.h������
/* 
���ܣ������ʼ��
ʹ�����ų�ʼ��gpio.c(���˿ڣ������źŷ��򣨼�оƬ���Ŷ�ȡ������źţ��������ʼ״̬)
ͨ���ڳ�ʼ����chip/ftm.c(Ƶ��Ϊ����ͺ�������)
*/
void motor_init(void)
{
  FTM_PWM_init(FTM0, FTM_CH4,10*1000, 0);//T����ת��
  FTM_PWM_init(FTM0, FTM_CH5,10*1000, 0);//F
  FTM_PWM_init(FTM0, FTM_CH6,10*1000, 0);//F
  FTM_PWM_init(FTM0, FTM_CH7,10*1000, 0);//T����ת��
  gpio_init (PTB0, GPO,1);//���ʹ��
}

3-2 ��ӱ��������������ʼ���Ӻ���������System_Init�����е��ã�����ǰ��inic.h������
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

3-3 ���PID_init��������ʼ�����ҵ��PID�ṹ��
  PID_init(PID_motor_L);//����PID�ṹ��
  PID_init(PID_motor_R);//�ҵ��PID�ṹ��
  
3-4 �趨���ҵ��P��I��D��ʼֵ
  PID_kp(PID_motor_L,Motor_P);
  PID_ki(PID_motor_L,Motor_I);
  PID_kd(PID_motor_L,Motor_D);
  PID_kp(PID_motor_R,Motor_P);
  PID_ki(PID_motor_R,Motor_I);
  PID_kd(PID_motor_R,Motor_D);
  
4 ���PIT0������,����֮ǰҪ�ȶ���һ��������ƺ���
4-1 ���������ƺ���
void motor_Ctrl(void)
{
  int Speed;
 //ͣ��||��ʱ�� 
  
  if(RunState_flag&& protect_flag==0)//����protect_flag  ��protect_flagΪ0ʱ�жϳ���������  ����Ϊprotect_flag=1
  Speed=Encoder_set;//��ʱ��
  else
  {
    Speed=0;//ͣ��
    LRT_flag=0;
  }
  
  PID_set(PID_motor_L,Speed,CntEncoder_left);//����PIDƫ��  
  PWM_duty_deta_L+=(int32)PID_Calc(PID_motor_L);//�ۼ�ռ�ձȣ�����ʽPID���㣩
  if(PWM_duty_deta_L>10000)//ռ�ձ��޷�
  {
    PWM_duty_deta_L=10000;
  }
  if(PWM_duty_deta_L<-10000) 
  {
    PWM_duty_deta_L=-10000;
  }
  
  PID_set(PID_motor_R,Speed,CntEncoder_right);//����PIDƫ��
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
  
  if(key_check(KEY_Centre)==KEY_DOWN)
  {
    IsDown_flag=1;//������������ʱ��־λ
    IsBeep=1;     //����������ʾ��ʼ����ʱ
  }
  
}
4-2 ��PIT0��ʱ���жϵĺ������е��õ�����ƺ���
int PWM_duty_deta_L=0;//����ռ�ձȷ����ۼ�ֵ
int PWM_duty_deta_R=0;//�ҵ��ռ�ձȷ����ۼ�ֵ
void PIT0_IRQHandler(void) 
{  

  CntEncoder_left=Encoder_read_L();//��ȡ���ұ�����ʵʱ��
  CntEncoder_right=Encoder_read_R();//��ȡ���ұ�����ʵʱ��
 // Encoder_inc=CntEncoder_right+Encoder_inc;
     motor_Ctrl();//�������
  //Send_16bitData(Encoder_set,CntEncoder_right,PWM_duty_deta_R);//�������Ӧ�������ж����淢�Ͳ��Σ������β�ƽ�����Һ���ʱ����Ҳ��һ��
  PIT_Flag_Clear(PIT0);       //���жϱ�־λ
  
}

5 ���PIT1�����ݣ��ǵ���init.h��������void PIT1_IRQHandler(void);��
int time=0;//���жϴ���
uint8 Timer_n=0;//��������
int Image_num=0;//ͼ�������100ms��
void PIT1_IRQHandler(void)
{
  if(++Timer_n==10)
  {
    Timer_n=0;
    if(++Image_num==1000)
      Image_num=0;
  }
  if(IsDown_flag==1)
  {
    if(++time==200)
    {
      time=0;
      RunState_flag=1;//��������200*10ms���������״̬
      IsDown_flag=0;//���������־
      BeepOFF(); 
    }    
  }
  if(RunState_flag==1)
  {
    if(++time==300 && Run_Model==1)
    {
      RunState_flag=0;
    }
  }
  
  PIT_Flag_Clear(PIT1);       //���жϱ�־λ
}
6 �����Ƶ����ռ�ձȷ��ӵľ��ȣ�·����Chip/MK60_ftm.c/ ->MK60_FTM.h
���Ƶ����FTMģ�飺FTM0��ռ�ձȾ��ȣ� #define FTM0_PRECISON 10000u

6-1 ���FTM0�ĸ�ͨ���� FTM_CH4 ~ FTM_CH7����Ӧ�����ת��;��β��ԣ��н�����

7���PID������޷�

8 �������P��I��D��ֵ��֮ǰ����P=30��I=9��
int Motor_P=30;
int Motor_I=9;
int Motor_D=0;

9 �����������趨������װ���Ӻ���������System_Init�����е��ã�����ǰ��inic.h������
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
10 �����������ϵͳ��ʼ��ʱ��ӷ����������ϣ���������ʾ����ʾ��Ϊ��֪�����еĳ�ʼ���Ƿ�������������������һ����
   ÿ��һ����ʼ������ʾ����ʾxxxinit... ������ĳһ����������֪���ĸ���ʼ��ʧ�ܡ�
10-1  ����һЩϵͳ��ʼ������˳���Ż�
10-2  ��ϵͳ��ʼ��������Ŀ�ʼ����������ʼ�����֮�󣩼ӷ���������ʾϵͳ��ʼ����ʼ��
      ����������һ��ӷ���������ʾϵͳ��ʼ����������
  BeepInit();//��������ʼ��
  BeepON(); //����������ʾϵͳ��ʼ����ʼ
  ...
  ...
  ...
  BeepOFF(); //����������ʾϵͳ��ʼ������
10-3 ��ʾ����ʾ��ʼ������һ��

11���Է�������Ϣ����

11-1������10ms��ʱ���ж��е���Beep����

void Beep()
{
  uint8 num=0;
  /**if(Is_activate)
  {
    Is_activate=0;*/
    if(IsBeep)
      num=1;
    else
      num=0;
    gpio_set (PTC7,num);
  //}
}

12 ��ϵͳ��ʼ���������������ж����ȼ�
��ֹ��ʼ��ʱ�Ϳ���

13 ��������ת����
13-1 ����ȫ�ֱ���
int LRT_flag=0;//��ʼ�������ת��־λ
13-2 ��Ϊ��ʱ�����ݸ���ת���������ƣ��ڶ�ʱ��PIT1�жϵĺ�������
     ����һ�������Ϊ��ʼ����ת�ı�־λ,ʹ��ʱ�����ݸ���ת����
     ����Ϊʱ��Ĵ���������ʶ��
  if(RunState_flag==1)
  {

    if(++time==300)//300*10ms
    {
      LRT_flag=1;//�����󵹼�ʱ3�룬��ʼ������Ƿ��ж�ת
    }
    else;
    
  }

//2019.6.8  ·�̼���
·����Init.h
���
typedef struct sv{              //�� ·�� �ٶ�  �ۼ������� ��ֵ
  float S;                //·�� ��λ  ����  mm
  float V;                //�ٶ�   m/s
  int Add_CntEncoder;   //�ۼ���������ֵ
}SV;
extern SV * sv_L;        //���� S V �ӿ�
extern SV * sv_R;        //���� S V �ӿ�

��PIT0��ʱ���� ���  �ۼƱ����������������  ·�̼������䣨����·����Ҫ���Գ�ģ�߹�һ�ױ������ۼӵ��������� �ٶȼ������ 

���飺
SV sv_l;
SV sv_r;

SV * sv_L=&sv_l;   // ����������� ��������� ·�� �ٶ�  �ۼ������� 

SV * sv_R=&sv_r;    // �����ұ����� ��������� ·�� �ٶ�  �ۼ�������  
void PIT0_IRQHandler(void) 
{  

  CntEncoder_left=Encoder_read_L();//��ȡ���ұ�����ʵʱ��
  CntEncoder_right=Encoder_read_R();//��ȡ���ұ�����ʵʱ��
  
  sv_L->Add_CntEncoder=CntEncoder_left+sv_L->Add_CntEncoder;     //�������ۼ�ֵ
  sv_L->S=(sv_L->Add_CntEncoder)*1.0/4300;                        //�ߵ�·��  ��λm
  sv_L->V=(CntEncoder_left*1.0/4300)/10;                       //�ٶ�  ��λ m/ms
  
  sv_R->Add_CntEncoder=CntEncoder_right+sv_R->Add_CntEncoder;     //�������ۼ�ֵ
  sv_R->S=(sv_R->Add_CntEncoder)*1.0/4300;                        //�ߵ�·��  ��λm
  sv_R->V=(CntEncoder_right*1.0/4300)/10;                       //�ٶ�  ��λ m/ms
  
  // Encoder_inc=CntEncoder_right+Encoder_inc;
  motor_Ctrl();//�������
  //Send_16bitData(Encoder_set,CntEncoder_right,PWM_duty_deta_R);//�������Ӧ�������ж����淢�Ͳ��Σ������β�ƽ�����Һ���ʱ����Ҳ��һ��
  PIT_Flag_Clear(PIT0);       //���жϱ�־λ
  
}



7������һ�� ��ʱ���������� 
λ�ã�Init.c void PIT1_IRQHandler(void)

uint8 Timer_n=0;//��������
int Image_num=0;//ͼ�������100ms��    100�����һ
uint8 image_num_kaiqi=0;    //    image_num_kaiqi  Ϊ 1 ��ʼ  Image_num �ļ���
  if(++Timer_n==10&&image_num_kaiqi)
  {
    Timer_n=0;
    if(++Image_num==1000)
      Image_num=0;
  }
  
  if(IsDown_flag==1)
  {
    if(++time==200)
    {
      time=0;
      RunState_flag=1;//��������200*10ms���������״̬
      IsDown_flag=0;//���������־
      BeepOFF(); 
    }    
  }
  Image_num  Ϊ�ⲿ�ӿ�
  

1-1 �޸Ķ������Ƶ��50Hz->75Hz
 FTM_PWM_init(FTM1,FTM_CH1,75,ServoCentreValue);//�����ʼ��������Ƶ��7
1-2 �޸Ķ����ֵ750->1128
 int ServoCentreValue=1128;    //�����ֵ(1.5ms/((1/75)x1000)x10000
 
 2019.06.11
 ����ͷ��ֵ����������
 �����������ڲ˵���ѭ����������ʱ��Ҫѡ����ֵ����Ż�����趨����ֵ������ʱ��Ϊ�ϴ��趨��ֵ
 ���������ϵͳ��ʼ���У��轫����ͷ��ʼ��������ǰ�棬�ȳ�ʼ�����е�����ͷ�ٽ���flash�Ķ�д��������ʵʱ����
 �� camera_all_init();����init.c����System_Init()����ǰ��
 
 
��������   90ms    GUI 41ms   ��ѹ0+ms   ��ȥѹ����GUI 50ms
 

//2019-6-21 ��ȡ�������ַ������� ���ջ�
����ȫ�ֱ�������char distance_laser[50];


2019��6��23��01��37��

init.c  ����һ������
extern int Laser_Value;   //����⵽�ľ���  Laser_Value   ��λ��mm
init.h  �����˽ӿ�


