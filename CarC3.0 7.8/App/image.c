//�汾1.2        2019��5��12��09:48:59

#include "image.h"
#include "include.h"
#include "common.h"
#include "math.h"
#include "VAR.h"
#include "GUI.h"
#include "elec.h"
#include "Init.h"
#include "stdio.h"

/*--------�����ⲿ�ӿ�------------------------

extern int Image_num;                   //ͼ�������100ms��

extern uint8 Image_num;                 //    image_num_kaiqi  Ϊ 1 ��ʼ  Image_num �ļ���

extern unsigned char Image_lcd_num;     //����һ����������ʾ����ʾ

--------�����ⲿ�ӿ�END---------------------*/

//     127   58/48

//=============================ȫ�ֱ�����==================================//
//unsigned char ImageData[IMG_ROWS][IMG_COLS];//ͼ������


/*
#define WHITE 255        //�궨���

#define RED 128        //�궨���     �߽�

#define GREEN 100        //�궨����     ����

#define BLACK 0          //�궨���

#define BLUE 254         //�궨����ɫ
#define GRAY 128          //��ɫ*/


#define THRESHOLD 100    //�궨����ֵ

#define ScanColNumble  10   //������ɨ���������Χ

//#define CrossFlag1   20    //�˺궨��Ϊ�ı����ұ߽��ж�����Ч��ֵ������ʮ�ֺ���

#define uchar unsigned char 

//��Ϊʶʮ��ʶ�𣬺����ĵ�
unsigned char shizhizuidahangkuan=20;           //ʮ�ֵ�����п�  47

unsigned char shiziscanfbase=35;				//ʮ��Ԥ�����������к���shiziscanfbase������Ϊ��ʼɨ����

int ErrorCha=10;	//ƫ���ƫ��

uchar crossturnnum=15;      //ʮ�ֵ��п�   abs(TRB-TLB)>crossturnnum&&TLB!=99&&TRB!=99)           //crossturnnumΪTRB-TLB  ����Ϊʮ��

unsigned char huandaoF=0;			//������־λ���˱�־λ����ʼ���������ֶ�����

unsigned char huandaozhuangtai_flag=0;     //�������е�״̬��־λ���˱�־λ����ʼ���������ֶ�����

unsigned char before_error_Flag=0;			//��λ��ʹ��Ҫ������һ֡��error��Ϊ��֡��ʹ�ã��˱�־λ����ʼ���������ֶ�����

uchar liekuan35=66;         //35�е��п� ����ʮ��ȷ�����ұ߽��ʱ��ʹ��



uchar center35lie;     //��λ��ֵ  35�����ߵ�����

float left_k=-1.208;       //����ͷ����������߽�� ����б��

float right_k=1.208;      //����ͷ���������ұ߽�� ����б��  49/45

uchar saidaokuan=126;   //��λ����������ͷ�ɼ���������ȣ�������ȡ�  126   c�� 2019��6��11��15:03:11
uchar saidaozuidaliekuan=126;

uchar saidaokuanyiban=63;//������һ��

uchar banmaxian_F=0;   //�����߼������ı�־λ

uchar bizhang_F=0;      //���ϱ�־λ

uchar kuan=0;   //gui�ӿ�������ʾ��һ�� ���п�

char Cross_F=0;   //ʮ�ֵı�־λ

char suo_F=0;     //������

char saidaoleixing=99; //��������  �������� �� 0����Բת��  10��ʮ��  11����ֱͨ��  5��������ͷת��  6����ͨת��  ע�����ֽ�����һ�����߼��㷽ʽ���������������



//=========================================================================//

#define rongcuo_1           10				//�ݴ�ֵ



//int before=before_w;        //before     before_w=35
int before=0;        //before     before_w=35
int8 error=0;
int protect_flag=0;
int Started_check_Zebra=0;      //�����߱�־



int Expectation_speed=0;        //������������ٶ�

//===========================�������================================================
unsigned char BlackRight[100][2]={0};  //�洢�ұ߽�Ķ�ά����
unsigned char BlackLeft[100][2]={0};   //�洢��߽�Ķ�ά����
unsigned char Center[100][2]={0};     //�洢���ߵĶ�ά����
unsigned char Center_T[100][2]={0};     //�洢���ߵĶ�ά����
unsigned char NumbDataleft=0;			//��߽����Ч��ֵ
unsigned char NumbDataright=0;			//�ұ߽����Ч��ֵ
unsigned char NumbDatacenter=0;			//���ߵ���Ч��ֵ
unsigned char NumbDatacenter_T=0;			//T���ߵ���Ч��ֵ

unsigned char BLB=99;		//��߽��������Ķ��ߵ�	Below Left Border
unsigned char TLB=99;		//��߽��������Ķ��ߵ�	Top Left Border
unsigned char BRB=99;		//�ұ߽��������Ķ��ߵ�	Below Right Border
unsigned char TRB=99;		//�ұ߽��������Ķ��ߵ�	Top Right Border


unsigned char BLB2=99;		//��߽��������Ķ��ߵ�	Below Left Border
unsigned char TLB2=99;		//��߽��������Ķ��ߵ�	Top Left Border
unsigned char BRB2=99;		//�ұ߽��������Ķ��ߵ�	Below Right Border
unsigned char TRB2=99;		//�ұ߽��������Ķ��ߵ�	Top Right Border

char duan_diubian_R=0;			//�ұ߽��ֿ�ʼ���ߣ���־λ
char duan_diubian_L=0;			//��߽��ֿ�ʼ���ߣ���־λ

float k_of_left_line[100];	//��߽��б��
float k_of_right_line[100];	//�ұ߽��б��

uchar rightguai=0;   	//�ҹյ�
uchar leftguai=0;		//��յ�

//#����ʶ��
unsigned char Left_Lose_Border_Number;	//��߽綪������
unsigned char Right_Lose_Border_Number;	//�ұ߽綪������
float k_of_left_line_fangcha=0;	//��߽緽��
float k_of_right_line_fangcha=0;//�ұ߽緽��
uchar FduanL=0,FduanR=0;//ǰ��3������㣬��ͬ���ߵı�־λ��0->û��;1->��
//#ʮ��ʶ��
unsigned char Common_Lose_Flag;		//��ͬ�ж��ߵı�־λ

//#��������ʶ��
unsigned char Flag_L_1=0,Flag_R_1=0;//���ұ߽緽�����Ʊ�־λ 1�� 2��



unsigned char sousuojieshuhang=0;//����������

uchar zhixingshu=0;				//ִ����//#����Ϊ���籣����������2019-4-15

int dataerror[4]={80};  	    //����˷�����һ�������Ϸ��������Ϸ� ��error



uchar LRbeyond_F=0;					//�յ����ϴ� LRbeyond_F=1�� �ж�ʮ�ֻ���ת�����

uchar Common_Lose_Flag_effective=0;   //��ͬ����ʱ�Ƿ񻹼���ִ�б߽�ɨ��     0������ɨ��  1����ɨ��  Ĭ��Ϊ0 

uchar yuanhuancrediblehang=0;



uchar saidao_F=0;


float k_of_left_line_pingjun=0;		//��߽�б��

float k_of_right_line_pingjun=0;	//�ұ߽�б��

//-----������Ҫ�õ��ı�־λ-------
uchar chubanmaxian=0;
uchar chubanmaxianjishu=0;
//-----������Ҫ�õ��ı�־λ--END-----

unsigned char image_flag=0;            //gui  ͼ�������Ա�־λ
unsigned char image_flag_1=0;            //gui  ͼ�������Ա�־λ

unsigned char left_diubiancishu=0;      //�󶪱ߴ���     init ��0
unsigned char right_diubiancishu=0;     //�Ҷ��ߴ���     init ��0

uchar daiyong_flag=0;                   //      ���ñ�־λ   ��Ϊִ�д�������

char changzhidao_F=0;   //��ֱ����־λ
char beep_F=0;          //��������־λ

char fengmingqi_daiyong=0;      //��1����һ��
char fengmingqi_f=0;
  int lianxuwandao_F=0;
  int zhidaopanduan_F=0;
  
  int center_L=0,center_R=0;            //�������ߵ����������е��±�
  int jiesujiance_F=0;                  //��ɨ�ߵ�ʱ����Ϊ�����п�С��һ��ֵ������
  char L_left=0,L_right=0;
char R_left=0,R_right=0;
char C_left=0,C_right=0;
int left_daihao=0,right_daihao=0; 
int zongtiqushi=0;
//=======================�����������================================================

//********************************��������****************************************************

//���庯�������뿴����ע��
void pass();
void pass1();
int shu(uchar a);
void Type_0(uchar a);
void Type_0la(char a);   //������б����������ǿ���һ����
void Type_1();
void Type_5(uchar a);
void Type_5la(uchar a);
void Type_6();
void Type_11();
void Type_12(char flag);
int Cross();
void ScanOneRow(uchar R,uchar *rc,uchar *lc);
void ConnectSpot(int duanR,int duanC);
int Three_Border_Scan();

void BorderScan();
void p_rintf();
void xielvfangcha();
void Type_Judgement();
void init();
int LeftRight1(uchar hang);
void credible();          //error �Ŀ����ж���ѡȡ
void protect();

void guaidiancredible();				//���ݹյ���ֵ���������жϹյ��Ƿ����

void CrossTypebuxian();

void one_Border_Scan(uchar r,uchar *lc,uchar *rc);

void diubiankexingpanduan();

void imgbefor();

int diubiancredible();

void errorpinghua();

void banmaxian(); //��������

void huandao();

void huandao_one(); //����

void huandao_two(); //����



void zuidaliekuan();

void yibianbuxian(uchar flag);  //��һ��Ϊ�߽粹��

void fengmingqi1(uchar f);      //�첻ͬ ������  

void bizhangjiance();                 //����


void qianzhancontrol();

void shexiangtoutiaojie();      //��� ����ͷ�Ƿ���� �������Ƿ���ȷ

void bizhang();

int shizixiediubian();          //бʮ�ֶ���

void Type_5_2(uchar a);         //����ڶ�������ת��ķ���

void suo();                     //������

void SpeedControl();            //ͨ���������͵����ٶ�

void shujucunchu();             //�洢 ��߽��ұ߽���������

void zhidaobuxian(uchar flag);  //ֱ����һ�ߵĲ��߷�ʽ

void beep_f();                  //���Ʒ�����

void beep_jizhi(char f);        //�������춯����

int left_border_zuiyou();       //������߽������������±�

void center_L_R(int *l,int *r);//������������ߵĺ����ұߵ������±�

int lianxuwandao();             //�������ߵ�����

int zhidaopanduan();            //ֱ���ж�

int sanxianqushipanduan();      //�����ж�

void duanlujiance();            //��·���

int duanluzhengchangsaidaojince();
//*******************************������������*************************************************

/*----------------------------------------------------------������д��ʼ--------------------------------------------------------*/ 

/*==================================================================================
�������ƣ�zhidaobuxian
��������: 
ʱ�䣺2019��5��16��20:16:34
������ flag  1 �����ұ߲���    2 ������߲���
===================================================================================*/
void zhidaobuxian(uchar flag)
{
  if(flag==1)
  {
    //pass();
    uchar i=0;
    for(i=0;i<NumbDataright;i++)
    {
      if(BlackRight[i][1]!=157)
      {
        if(BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k))>0)//+(i/right_k)
        {
          Center[i][1]=BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k));  //����ֱ�ߵ���ͨ������ -(i/right_k)
        }else 
        {
          Center[i][1]=0;
        }
      }else
      {
        Center[i][1]=(uchar)(BlackRight[i][1]-saidaokuanyiban+(i/right_k));
      }
      
      
      Center[i][0]=BlackRight[i][0];
    }
    NumbDatacenter=i;
    
  }else if(flag==2)
  {
    uchar i=0;
    for(i=0;i<NumbDataright;i++)
    {
      if(BlackLeft[i][1]!=2)
      {
        if(BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k)<159)
        {
          Center[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k);  //+(i/left_k)
          //Center[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k+(BlackLeft[i][1]-(BlackLeft[0][1]+(i/left_k))); //(BlackLeft[i][1]-(BlackLeft[0][1]-(i/left_k))
        }else
        {
          Center[i][1]=159;
        }
        
        
      }else
      {
        Center[i][1]=(uchar)(BlackLeft[i][1]+saidaokuanyiban+i/left_k);
      }
      Center[i][0]=BlackLeft[i][0];
    }
    NumbDatacenter=i;
  }
}


/*==================================================================================
�������ƣ�Type_0la
��������: ���ݶ��ߵ��е������������һ����������һ��ֱ�ߣ��ڻ�����ʱ���õ�
ʱ�䣺2019��6��27��12��12��
������
����ֵ��
===================================================================================*/
void Type_0la(char a)
{
  uchar x1=0,y1=0;
  
  NumbDatacenter=0;
  uchar i=0;
  zhixingshu++;
  saidaoleixing=0;//��������Ϊ��Բת��
  if(a==1)
  {
    x1=(BlackRight[BRB][0]+BlackRight[TRB][0])/2;
    y1=157;
    for(i=0;i<59-BlackRight[BRB][0];i++)
    {
      Center[NumbDatacenter][0]=BlackRight[i][0];
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      NumbDatacenter++;
    }
    
    if(NumbDatacenter==0)
    {
      //pass1();
      Center[NumbDatacenter][0]=59;
      if(BlackLeft[i][1]+saidaozuidaliekuan/2+15<80)
      {
        Center[NumbDatacenter][1]=80;
      }else
      {
        Center[NumbDatacenter][1]=BlackLeft[i][1]+saidaozuidaliekuan/2+15;
      }
      
      if(BlackLeft[i][1]+saidaozuidaliekuan/2+15>157)Center[NumbDatacenter][1]=157;
      NumbDatacenter++;
      //pass();
      ConnectSpot(x1,y1);
      return;
    }
    
    ConnectSpot(x1,y1);
    
    
  }if(a==2)
  {
    //pass();
    x1=(BlackLeft[BLB][0]+BlackLeft[TLB][0])/2;
    y1=2;
    ImageData[BlackLeft[TLB][0]][3]=BLUE;
    NumbDatacenter=0;
    for(i=0;i<59-BlackLeft[BLB][0];i++)
    {
      //pass();
      Center[NumbDatacenter][0]=BlackLeft[i][0];
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      NumbDatacenter++;
      
      //pass();
    }
    
    if(NumbDatacenter==0)
    {
      //pass();
      Center[NumbDatacenter][0]=59;
      if(BlackRight[i][1]-saidaozuidaliekuan/2-15>80)
      {
        Center[NumbDatacenter][1]=80;
      }else
      {
        Center[NumbDatacenter][1]=BlackRight[i][1]-saidaozuidaliekuan/2-15;
      }
      
      if(BlackRight[i][1]-saidaozuidaliekuan/2-15<2)Center[NumbDatacenter][1]=2;
      NumbDatacenter++;
      
      ConnectSpot(x1,y1);
      
      return;
      
    }
    
    ConnectSpot(x1,y1);
    
  }
}

/*==================================================================================
�������ƣ�bizhangjiance
��������: ����ϰ���
ʱ�䣺2019��6��6��17:02:03
������bizhang_F ���ϱ�־λ
����ֵ��
===================================================================================*/
char duanlu_F=0;
void bizhangjiance()                 //����
{
  char saidaozhongduan=0;
  
  if(k_of_right_line_fangcha<800&&k_of_left_line_fangcha<800)       //���������䣬�����С&&Center[center_R][]center_R
  {
    //Image_lcd_num=1;
    if(Right_Lose_Border_Number==0&&Left_Lose_Border_Number==0&&Common_Lose_Flag==0)       //�޶���
    {
      //Image_lcd_num=2;
      if(NumbDataleft<=56&&NumbDataleft>=40)         //�߽���С��56��
      {
        //Image_lcd_num=3;
        
        int sum[4];
        uchar i=0;
        uchar j=0;
        if(abs(Center[NumbDatacenter-2][1]-80)<10)
        {
         
          saidaozhongduan=1;                    //�������
          Expectation_speed=-20;
        }
      }
    }
    
  }
  //saidaozhongduan==1
  if(Laser_Value<1000&&Laser_Value>400&&huandaoF==0&&saidaozhongduan==1)   //������С��80cmʱ��Ϊ��ֱ��  ����   �����ڻ�����
  {//&&changzhidao_F==1&&saidaoleixing==11&&Right_Lose_Border_Number==0&&Left_Lose_Border_Number==0
    if(Center[NumbDataleft-4][1]<95&&Center[NumbDataleft-4][1]>65)//&&saidaozhongduan==1
    {
      fengmingqi_daiyong=1;
      bizhang_F=1;
      
    }else
    {
      saidaozhongduan=0;
    }
  }else
  {
    saidaozhongduan=0;
  }
}


void bizhangjiance1()                 //����
{
  //if()

  char saidaozhongduan=0;
  if(k_of_right_line_fangcha<1000&&k_of_left_line_fangcha<1000)       //���������䣬�����С
  {
    //Image_lcd_num=1;
    if(Right_Lose_Border_Number==0&&Left_Lose_Border_Number==0&&Common_Lose_Flag==0)       //�޶���
    {
      Image_lcd_num=2;
      if(NumbDataleft<=50&&NumbDataleft>=30)         //�߽���С��50��
      {
        Image_lcd_num=3;
        int sum[4];
        uchar i=0;
        uchar j=0;
        for(i=0;i<20;i+=5)
        {
          sum[j]=(BlackRight[i][1]+BlackLeft[i][1])/2;   
          j++;
        }
        
        if(abs(sum[3]-sum[0])<10)                     //����ֱ��         //
        {
          Image_lcd_num=4;
          saidaozhongduan=1;
          
        }
      }
    }
    
  }
  
  if((saidaozhongduan==1||(Laser_Value<800&&Laser_Value>400&&changzhidao_F==1))&&huandaoF==0&&saidaoleixing==11)   //������С��80cmʱ��Ϊ��ֱ��  ����   �����ڻ�����
  {
    bizhang_F=1;
  }
}



/*==================================================================================
�������ƣ�bizhang
��������: ��ʼ����
ʱ�䣺2019��6��6��17:07:55
������bizhang_F ���ϱ�־λ
����ֵ��
===================================================================================*/
uchar bizhangjieduan_F=0;
void bizhang()
{
  
  if(Laser_Value<200)  //���籣��
  {
    protect_flag=1;    //ͣ����־λ
    RunState_flag=0;
    
  }
  if(1)         //�������
  {
    if(bizhangjieduan_F==0)             //��һ�׶��������---------------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_R->S=0;                  //·������
        sv_R->Add_CntEncoder=0;         //�������ۼ�ֵ����
        daiyong_flag=1;
      }
      error=-80;                  //�������
      if(sv_R->S>0.31)
      {
        bizhangjieduan_F=1;               //���Ͻ׶� 1
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==1)             //�ڶ��׶δ�ֱ--------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_R->S=0;                  //·������
        sv_R->Add_CntEncoder=0;         //�������ۼ�ֵ����
        daiyong_flag=1;
      }
      error=0;
      if(sv_R->S>0.5)
      {
        bizhangjieduan_F=2;               //���Ͻ׶� 2
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==2)             //�����׶����Ҵ���------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_R->S=0;                  //·������
        sv_R->Add_CntEncoder=0;         //�������ۼ�ֵ����
        daiyong_flag=1;
      }
      error=80;
      if(sv_R->S>0.5)
      {
        bizhangjieduan_F=3;               //���Ͻ׶� 2
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==3)             //�����׶����Ҵ���------------------------------------
    {
      error=0;
      if(BlackRight[0][1]-BlackLeft[0][1]>80&&BlackRight[0][1]==157)
      {
        if(ImageData[59][BlackRight[0][1]-1]<100||ImageData[59][BlackRight[0][1]-2]<100||ImageData[59][BlackRight[0][1]-3]<100)//100Ϊ��ֵ ���ò�
        {
          
        }else
        {
          if(ImageData[59][BlackLeft[0][1]+1]<100||ImageData[59][BlackLeft[0][1]+2]<100||ImageData[59][BlackLeft[0][1]+3]<100)  //�������кڳ���
          {
          }else
          {
            bizhang_F=0;
            daiyong_flag=0;
          }
        }
      }
      if(Right_Lose_Border_Number>=5&&BlackRight[0][1]-BlackLeft[0][1]>65)
      {
        bizhang_F=0;
        daiyong_flag=0;
      }
    }
    
    
  }else                       //���ұ���
  {
    if(bizhangjieduan_F==0)             //��һ�׶��������---------------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_L->S=0;                  //·������
        sv_L->Add_CntEncoder=0;         //�������ۼ�ֵ����
        daiyong_flag=1;
      }
      error=80;                  //�������
      if(sv_L->S>0.3)
      {
        bizhangjieduan_F=1;               //���Ͻ׶� 1
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==1)             //�ڶ��׶δ�ֱ--------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_L->S=0;                  //·������
        sv_L->Add_CntEncoder=0;         //�������ۼ�ֵ����
        daiyong_flag=1;
      }
      error=0;
      if(sv_R->S>1.2)
      {
        bizhangjieduan_F=2;               //���Ͻ׶� 2
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==2)             //�����׶����Ҵ���------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_L->S=0;                  //·������
        sv_L->Add_CntEncoder=0;         //�������ۼ�ֵ����
        daiyong_flag=1;
      }
      error=-80;
      if(sv_L->S>0.5)
      {
        bizhangjieduan_F=3;               //���Ͻ׶� 2
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==3)             //�����׶����Ҵ���------------------------------------
    {
      error=0;
      if(BlackRight[0][1]-BlackLeft[0][1]>80&&BlackLeft[0][1]==2)
      {
        if(ImageData[59][BlackRight[0][1]-1]<100||ImageData[59][BlackRight[0][1]-2]<100||ImageData[59][BlackRight[0][1]-3]<100)//100Ϊ��ֵ ���ò�
        {
          //�м��кڵ�
        }else
        {
          if(ImageData[59][BlackLeft[0][1]+1]<100||ImageData[59][BlackLeft[0][1]+2]<100||ImageData[59][BlackLeft[0][1]+3]<100)  //�������кڳ���
          {
            //�м��кڵ�
          }else
          {
            bizhang_F=0;
            daiyong_flag=0;
          }
        }
      }
      if(Left_Lose_Border_Number>=5&&BlackRight[0][1]-BlackLeft[0][1]>65)
      {
        bizhang_F=0;
        daiyong_flag=0;
      }
    }
  }
  
}

/*==================================================================================
�������ƣ�shexiangtoutiaojie
��������: ��� ����ͷ�Ƿ���� �������Ƿ���ȷ
ʱ�䣺2019��6��5��21:02:03
������
����ֵ��
===================================================================================*/
void shexiangtoutiaojie()
{
  //if(abs(159-BlackRight[0][1]-BlackLeft[0][1])<2)
  {
    //Image_lcd_num=1;
    if(abs(BlackRight[0][1]-BlackLeft[0][1]-saidaokuan)<2)
    {
      //Image_lcd_num=2;
      if(abs(Center[10][1]-Center[0][1])<4)
      {
        // Image_lcd_num=3;        //GUI��ʾ
      }
    }
  }
}


/*==================================================================================
�������ƣ�fengmingqi1
��������: ������������
ʱ�䣺2019��5��25��14:45:32
������
����ֵ��
===================================================================================*/
void fengmingqi1(uchar f)
{
  image_num_kaiqi=1;
  if(f==1)
  {
    if(Image_num%3==0)  //��300ms ��100msһ��
    {
      BeepON();
    }else
    {
      BeepOFF();
    }
  }else if(f==2)
  {
    if(Image_num%6==0)  //��600ms ��100ms 
    {
      BeepON();
    }else
    {
      BeepOFF();
    }
  }else if(f==3)
  {
    if(Image_num%10==0)  //��1000ms ��100ms
    {
      BeepON();
    }else
    {
      BeepOFF();
    }
  }else if(f==4)
  {
    if(Image_num%15==0)  //��1000ms ��100ms
    {
      BeepON();
    }else
    {
      BeepOFF();
    }
  }
  
}

void fengmingqi2()
{
  image_num_kaiqi=1;
  if(Image_num%6==0)  //��100msһ��
  {
    BeepON();
  }else
  {
    BeepOFF();
  }
}

/*==================================================================================
�������ƣ�banmaxian
��������: �����߼��
ʱ�䣺2019��5��6��20:42:42
������
����ֵ�� banmaxian_F   Ϊ1��⵽������ 
===================================================================================*/
char yanhoutingche=0;   //ͣ�ڰ�����֮��
void banmaxian()
{
  uchar i,k;
  uchar lec,ric,r;
  int tiaobian=0;
//  for(int q=0;q<NumbDataleft-2;q+=2)
//  {
//    if(abs(BlackLeft[q][1]-BlackLeft[q+2][1])>5)
//    {
//      tiaobian
//    }
//  }

  for(uchar j=59;j>49;j--)
  {
    k=0;
    for(uchar i=10;i<149;i++)
    {
      if(ImageData[j][i]>THRESHOLD&& ImageData[j][i+1]<THRESHOLD)
      {
        k++;
      }
    }
    if(k>5)
    {
      banmaxian_F=1;
      break;
    }
  }
  
  //----------------���49�е��Ƿ���ڰ�����-------------------------
  r=BlackRight[10][0];
  ric=BlackRight[10][1];
  lec=BlackLeft[10][1];
  k=0;
  if(ric!=157&&lec!=2&&banmaxian_F==0)
  {
    for(i=lec;i<ric;i++)
    {
      if(ImageData[r][i]>THRESHOLD && ImageData[r][i+1]<THRESHOLD)
      {
        k++;
      }
    }
    if(k>4)
    {
      //Started_check_Zebra=1;
      
      banmaxian_F=1;
    }
  }
  //----------------���44�е��Ƿ���ڰ�����-------------------------
  r=BlackRight[15][0];
  ric=BlackRight[15][1];
  lec=BlackLeft[15][1];
  k=0;
  if(ric!=157&&lec!=2&&banmaxian_F==0)
  {
    for(i=lec;i<ric;i++)
    {
      if(ImageData[r][i]>THRESHOLD && ImageData[r][i+1]<THRESHOLD)
      {
        k++;
      }
    }
    if(k>4)
    {
      //Started_check_Zebra=1;
      
      banmaxian_F=1;
    }
  }
  
  
  
  if(BlackRight[0][1]-BlackLeft[0][1]<15&&banmaxian_F==0)    //��һ�м�����߽���ұ߽�С��15,
  {
    if(Flag_L_1==2&&Flag_R_1==1&&abs(leftguai-rightguai)<5)   //���ֹյ㲢�ң�������ȷ
    {
      uchar i=0;
      if(rightguai>leftguai)
      {
        i=rightguai;
      }else
      {
        i=leftguai;
      }
      
      if(BlackRight[i+1][1]-BlackLeft[i+1][1]>100)   //�յ��Ϸ������ұ߽�������  100    100 �Ǹ��ݰ�������ͼ��59�д�����ռ���п�������������һ�е��п���¸���
      {
        banmaxian_F=1;
        
      }
    }
  }
  
  if(BlackRight[2][1]-BlackLeft[2][1]<15&&banmaxian_F==0)    //�����м�����߽���ұ߽�С��15,
  {
    if(Flag_L_1==2&&Flag_R_1==1&&abs(leftguai-rightguai)<5)   //���ֹյ㲢�ң�������ȷ
    {
      uchar i=0;
      if(rightguai>leftguai)
      {
        i=rightguai;
      }else
      {
        i=leftguai;
      }
      
      if(BlackRight[i+1][1]-BlackLeft[i+1][1]>100)   //�յ��Ϸ������ұ߽�������  100    100 �Ǹ��ݰ�������ͼ��59�д�����ռ���п�������������һ�е��п���¸���
      {
        banmaxian_F=1;
        
      }
    }
  }
  
  
  if(banmaxian_F==1)    //����Ƿ�Ѱ����߹���
  {
    uchar lc,rc;
    uchar lc1,rc1;
    one_Border_Scan(59,&lc,&rc);
    one_Border_Scan(55,&lc1,&rc1);
    if(rc-lc<20||rc1-lc1<20)    //59��ѹ�ڰ���������
    {
      chubanmaxian=1;
    }
    if(rc-lc>80&&chubanmaxian==1)    //�Ѿ�����������
    {
      chubanmaxianjishu++;     
    }
    if(chubanmaxianjishu>3)               //ȷ�Ͼ���������
    {
      banmaxian_F=0;                      //�����߱�־λ����
      chubanmaxianjishu=0;                //��������
      chubanmaxian=0;
      if(Started_check_Zebra==1)
      {
        if(yanhoutingche==0)
        {
          sv_R->S=0;                  //·������
          yanhoutingche=1;
        }
      }
    }
  }
  
  if(sv_R->S>0.33&&yanhoutingche==1)
  {
    protect_flag=1;     //ͣ��
    yanhoutingche=0;
    fengmingqi_daiyong=1;
  }
}

/*======================================================================
�������ƣ�protect
��������: ����ͣ��
ʱ�䣺2019��5��21��12:52:48
������
======================================================================*/
void protect()
{
  uchar pro_flag=0;         //��⵽����ı�־λ
  if(banmaxian_F==0)    //�������������ײ
  {
    if(BlackRight[0][1]-BlackLeft[0][1]<20)
    {
      
      pro_flag=1;
      //      if((TRB-BRB>5&&TRB-BRB<13&&BRB==0)||(TLB-BLB>5&&TLB-BLB<13&&BLB==0))  //ת���ʱ���ȥ�󲿷�Ҳ��������˶�
      //      {
      //        pro_flag=0;
      //      }
    }
  }else
  {
    uchar b1=0;    //�ڿ������
    for(uchar i1=1;i1<NumbDatacenter;i1++)    //160/5=32
    {
      if(abs(Center[i1][1]-Center[i1-1][1])>5)  //�����������Ϊ5 b1����1��
      {
        b1++;  
      }
    }
    
    if(b1>5&&banmaxian_F==0)
    {
      pro_flag=1;
    }
    
  }
  if(Laser_Value<200&&Started_check_Zebra==1)   //�������⵽С��20cm  ͣ������
  {
    pro_flag=1;
    //fengmingqi_daiyong=1;
  }
  if(pro_flag==1&&duanlu_F==0)
  {
    protect_flag=1;    //ͣ����־λ
    RunState_flag=0;
  }
  
}
void protect1()
{
  uchar pro_flag=0;         //��⵽����ı�־λ
  if(banmaxian_F==0)    //�������������ײ
  {
    for(uchar i=0;i<=5;i++)
    {
      if(BlackRight[i][1]-BlackLeft[i][1]<50)     //����� ���Բת���������ͻ  �������������������������
      {
        
        pro_flag=1;
        //      if((TRB-BRB>5&&TRB-BRB<13&&BRB==0)||(TLB-BLB>5&&TLB-BLB<13&&BLB==0))  //ת���ʱ���ȥ�󲿷�Ҳ��������˶�
        //      {
        //        pro_flag=0;
        //      }
      }
    }
    
  }else
  {
    //    //------------------------------------ʶ��ʽ1----������------------------------
    //    uchar b1=0,b2=0;    //�ڿ������
    //    for(uchar i1=1;i1<NumbDataleft;i1++)
    //    {
    //      if(abs(BlackLeft[i1][1]-BlackLeft[i1-1][1])>4)  //������߽����Ϊ5  ������� 4 �� b1����1��
    //      {
    //        b2++;  
    //      }
    //    }
    //    
    //    
    //    for(uchar i1=1;i1<NumbDatacenter;i1++)    //160/5=32
    //    {
    //      if(abs(Center[i1][1]-Center[i1-1][1])>4)  //�����������Ϊ5 b1����1��
    //      {
    //        b1++;  
    //      }
    //    }
    //    
    //    
    //    if(b1>4||b2>4)
    //    {
    //      pro_flag=1;
    //    }
    //    //----------------------------------------------------------------------------------
    //    
    //    //------------------------------------ʶ��ʽ2----������------------------------
    //    //һ�����֮����������ᷴ�⣬���Լ�ʹ������������������������������
    //    b1=0;
    //    for(int i1=0;i1<NumbDatacenter&&Cross_F!=1;i1+=5)  //����ʮ��״̬�������ֳ��籣��
    //    {
    //      if(kuan<BlackRight[i1][1]-BlackLeft[i1][1])
    //      {
    //        b1++;
    //      }
    //      
    //    }
    //    if(b1>=6)   //  60/5==12    
    //    {
    //      pro_flag=1;
    //    }
    //    //saidaoleixing
    //    //-------------------------------------------------------------------------------
    //    
    //    
    //  }
    //  if(pro_flag==1)
    //  {
    //    protect_flag=1;    //ͣ����־λ
    //    RunState_flag=0;
    //  }
    uchar b1=0;    //�ڿ������
    for(uchar i1=1;i1<NumbDatacenter;i1++)    //160/5=32
    {
      if(abs(Center[i1][1]-Center[i1-1][1])>5)  //�����������Ϊ5 b1����1��
      {
        b1++;  
      }
    }
    
    if(b1>5&&banmaxian_F==0)
    {
      pro_flag=1;
    }
    
  }
  if(pro_flag==1&&bizhang_F==0)
  {
    protect_flag=1;    //ͣ����־λ
    RunState_flag=0;
  }
  
}

/*======================================================================
�������ƣ�yibianbuxian
��������: ֱ����һ�߽��в���   1 Ϊ���ұ�Ϊ�߽粹��   2 Ϊ�����Ϊ�߽粹��       ֱ������
ʱ�䣺2019��6��14��19:40:02
������
======================================================================*/

void yibianbuxian(uchar flag)
{
  //uchar saidaoyiban=55;
  
  /* if(flag==1)
  {
  //pass();
  uchar i=0;
  for(i=0;i<NumbDataright;i++)
  {
  if(BlackRight[i][1]!=157)
  {
  if(BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k)+(BlackRight[0][1]-BlackRight[i][1]))>0)//+(i/right_k)
  {
  Center[i][1]=BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k)+(BlackRight[0][1]-BlackRight[i][1]));  //����ֱ�ߵ���ͨ������ -(i/right_k)
}else 
  {
  Center[i][1]=0;
}
}else
  {
  Center[i][1]=(uchar)(BlackRight[i][1]-saidaokuanyiban+(i/right_k));
}
  
  
  Center[i][0]=BlackRight[i][0];
}
  NumbDatacenter=i;
  
}else if(flag==2)
  {
  uchar i=0;
  for(i=0;i<NumbDataright;i++)
  {
  if(BlackLeft[i][1]!=2)
  {
  if(BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k+(BlackLeft[i][1]-BlackLeft[0][1]))<159)
  {
  Center[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k+(BlackLeft[i][1]-BlackLeft[0][1]));  //+(i/left_k)
  //Center[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k+(BlackLeft[i][1]-(BlackLeft[0][1]+(i/left_k))); //(BlackLeft[i][1]-(BlackLeft[0][1]-(i/left_k))
}else
  {
  Center[i][1]=159;
}
  
  
}else
  {
  Center[i][1]=(uchar)(BlackLeft[i][1]+saidaokuanyiban+i/left_k);
}
  Center[i][0]=BlackLeft[i][0];
}
  NumbDatacenter=i;
}*/
  if(flag==1)
  {
    float kk=right_k-47/75;
    //right_k=right_k+47/75;
    //right_k=47/75;
    //pass();
    uchar i=0;
    for(i=0;i<NumbDataright;i++)
    {
      if(BlackRight[i][1]!=157)//-(i/right_k)
      {
        if(BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k))>0)
          //if(BlackRight[i][1]-(int)(saidaokuanyiban+(BlackRight[0][1]-BlackRight[i][1]))>0)//+(i/right_k)  +(BlackRight[0][1]-BlackRight[i][1])
        {
          Center[i][1]=BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k));  //����ֱ�ߵ���ͨ������ -(i/right_k) +(i/right_k)
        }else 
        {
          Center[i][1]=0;
        }
      }else
      {
        Center[i][1]=(uchar)(BlackRight[i][1]-saidaokuanyiban+(i/right_k));
      }
      
      
      Center[i][0]=BlackRight[i][0];
    }
    NumbDatacenter=i;
    
  }else if(flag==2)
  {
    //pass();
    uchar i=0;
    for(i=0;i<NumbDataright;i++)
    {
      if(BlackLeft[i][1]!=2)
      {
        if(BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k)<159)//+(BlackLeft[i][1]-BlackLeft[0][1])
        {
          Center[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k);  //+(i/left_k) +(BlackLeft[i][1]-BlackLeft[0][1])
          //Center[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k+(BlackLeft[i][1]-(BlackLeft[0][1]+(i/left_k))); //(BlackLeft[i][1]-(BlackLeft[0][1]-(i/left_k))
        }else
        {
          Center[i][1]=159;
        }
        
        
      }else
      {
        Center[i][1]=(uchar)(BlackLeft[i][1]+saidaokuanyiban+i/left_k);
      }
      Center[i][0]=BlackLeft[i][0];
    }
    NumbDatacenter=i;
  }
}

/*======================================================================
�������ƣ�yibianbuxian
��������: ֱ����һ�߽��в���   1 Ϊ���ұ�Ϊ�߽粹��   2 Ϊ�����Ϊ�߽粹��       ֱ������
ʱ�䣺2019��5��13��21:15:47
������
======================================================================*/
void yibianbuxian1(uchar flag)
{
  //uchar saidaoyiban=55;
  
  if(flag==1)
  {
    uchar i=0;
    for(i=0;i<NumbDataright;i++)
    {
      if(BlackRight[i][1]!=157)
      {
        if(BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k)+(BlackRight[0][1]-(i/right_k)-BlackRight[i][1]))>0)
        {
          Center[i][1]=BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k)+(BlackRight[0][1]-(i/right_k)-BlackRight[i][1]));  //����ֱ�ߵ���ͨ������
        }else 
        {
          Center[i][1]=0;
        }
      }else
      {
        Center[i][1]=(uchar)(BlackRight[i][1]-saidaokuanyiban+(i/right_k));
      }
      
      
      Center[i][0]=BlackRight[i][0];
    }
    NumbDatacenter=i;
    
  }else if(flag==2)
  {
    uchar i=0;
    for(i=0;i<NumbDataright;i++)
    {
      if(BlackLeft[i][1]!=2)
      {
        if(BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k+(BlackLeft[i][1]-(BlackLeft[0][1]-(i/left_k))))<159)
        {
          Center[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k+(BlackLeft[i][1]-(BlackLeft[0][1]-(i/left_k))));  
        }else
        {
          Center[i][1]=159;
        }
        
        
      }else
      {
        Center[i][1]=(uchar)(BlackLeft[i][1]+saidaokuanyiban+i/left_k);
      }
      Center[i][0]=BlackLeft[i][0];
    }
    NumbDatacenter=i;
  }
}


/***********************************************************
�������ƣ�zuidaliekuan
��������: ������ͷ���59�е��п��Լ�ȷ������б��
ʱ�䣺ʱ��
������
************************************************************/
void zuidaliekuan()
{
  //  if(k_of_right_line_fangcha<150&&k_of_left_line_fangcha<150&&Left_Lose_Border_Number==0&&Right_Lose_Border_Number==0&&NumbDataleft>40&&saidao_F==0)
  //  {
  //    saidaokuan=BlackRight[0][1]-BlackLeft[0][1];
  //    saidaokuanyiban=saidaokuan/2;
  //    
  //    if(abs(saidaokuan-110)<5)
  //    {
  //      saidao_F=1;//ִֻ��һ��
  //    }
  //    
  //  }
  if(k_of_right_line_fangcha<150&&k_of_left_line_fangcha<150&&Left_Lose_Border_Number==0&&Right_Lose_Border_Number==0&&NumbDataleft>40&&saidao_F==0)
  {
    if(abs(saidaokuan-(BlackRight[0][1]-BlackLeft[0][1]))<2)
    {
      //Image_lcd_num=8;
      //BeepON();
    }else{
      //Image_lcd_num=4;
      saidaokuanyiban=saidaokuan/2;
      //saidao_F=1;//ִֻ��һ��
    }
  }
  kuan=BlackRight[0][1]-BlackLeft[0][1];
  
  //  if(k_of_right_line_fangcha<150&&k_of_left_line_fangcha<150&&Left_Lose_Border_Number==0&&Right_Lose_Border_Number==0&&NumbDataleft>40&&saidao_F==1)
  //  {
  //    if((BlackRight[30][1]+BlackLeft[30][1])/2+(BlackRight[0][1]+BlackLeft[0][1])/2<6)  //ƫ��ӽ�����
  //    {
  //      left_k=k_of_left_line_pingjun/2;    //�г���         //
  //      right_k=k_of_right_line_pingjun/2;    //�г���         //
  //      //image_flag_1=1;
  //      saidao_F=2;//ȷ����Ͳ���ִ��
  //    }
  //  }
  
}



/***********************************************************
�������ƣ�credible 
��������: error�ĵĿ����ж���ѡȡ���˲�
ʱ�䣺2019��4��13��20:48:24
������none        dataerror[4]
************************************************************/
//�����뷨              1������ x
//                      2������	x

/*

��ǰ֡error����һ֡error��ȣ���������10   ��֡��������֡���

*/

void credible()
{
  if(before_error_Flag==1)      //�һ���  ������һ֡ƫ��
  {
    //    if(dataerror[0]>40)
    //    {
    //      error=dataerror[0];
    //    }else
    //    {
    //      error=78;
    //    }
    error=60;
    before_error_Flag=0;
    
  }else if(before_error_Flag==2)//�󻷵�  ������һ֡ƫ��
  {
    //    if(dataerror[0]<-40)
    //    {
    //      error=dataerror[0];
    //    }else
    //    {
    //      error=-78;
    //    }
    error=-60;
    before_error_Flag=0;
  }
  //errorpinghua();      //error�������20�Ļ��������������
  
  
  dataerror[0]=error;
  dataerror[1]=dataerror[0];  //���󸲸�
  dataerror[2]=dataerror[1];  //���󸲸�
  dataerror[3]=dataerror[2];  //���󸲸�
  
  
  
  
}
/***********************************************************
�������ƣ�imgbefor
��������: ��֮ǰͼƬ�õ�����Ϣ���д���
ʱ�䣺2019��5��4��18:00:28
������none
************************************************************/
//��һ֡��ͼƬ��Ϣ
void imgbefor()
{
  if(NumbDatacenter>20)
  {
    center35lie=Center[20][1];    //��һ֡39������������
  }else
  {
    center35lie=Center[NumbDatacenter-1][1];
  }
  
}



/*==================================================================================
�������ƣ�errorpinghua()
��������: ��error�����������ʹ֮ƽ��
ʱ�䣺
������
===================================================================================*/
void errorpinghua()
{
  //---------------------�޷��˲�----------------------------------------------------
  /*  if(dataerror[0]>error&&(dataerror[0]-error>40))
  {
  error=dataerror[0]-40;
}
  if(error>dataerror[0]&&(error-dataerror[0]>40))
  {
  error=dataerror[0]+40;
}
  */
  //-----------------------ƽ���˲�-------------------------------------------
  error=(int)(error+dataerror[0])/2;  //
  
}





/***********************************************************
�������ƣ�Three_Border_Scan
��������: 59-57�еı߽�ɨ�裬�ó���׼ȷ�����ұ߽磬������Ϊ׼ȷ�����������ұ߽���Ϊ��������ɨ
ʱ�䣺2019��3��26��21:33:42
������none
************************************************************/

//-------------------------------��ֲɾ������------------------//
void pass()
{
  /*
  ImageData[0][80]=RED;
  ImageData[0][81]=RED;
  ImageData[0][79]=RED;
  */
}
void pass1()
{
  /*
  ImageData[0][5]=BLUE;
  ImageData[0][6]=BLUE;
  ImageData[0][7]=BLUE;
  */
}

int shu(uchar a)
{
  return 0;
}



/*==================================================================================
�������ƣ�huandao_one
��������: �������--�Ƿ���ֻ���
ʱ�䣺2019��5��10��14:39:41
��������
����ֵ��falg_bord   1�ұ߽���ֻ���   2��߽���ֻ���
===================================================================================*/
void huandao_one()
{
  //huandaoF=2;
  uchar flag_direction=0;   //Բ�����ܳ��ֵķ���
  if(k_of_left_line_fangcha<500&&k_of_right_line_fangcha>1000&&Right_Lose_Border_Number!=0&&Left_Lose_Border_Number==0||(Right_Lose_Border_Number>18&&k_of_left_line_fangcha<500&&Left_Lose_Border_Number==0&&k_of_right_line_fangcha>500))
  {
    if(Flag_R_1==1&&Flag_L_1==0&&rightguai<35)
    {
      //pass();
      flag_direction=1;//Բ�����ұ�
      huandaoF=1;
    }
  }else if(k_of_left_line_fangcha>1000&&k_of_right_line_fangcha<500&&Left_Lose_Border_Number!=0&&Right_Lose_Border_Number==0||(Left_Lose_Border_Number>18&&k_of_right_line_fangcha<500&&Right_Lose_Border_Number==0&&k_of_left_line_fangcha>500))
  {
    if(Flag_L_1==2&&Flag_R_1==0&&leftguai<35)
    {
      flag_direction=2;//Բ�������
      //pass();
      huandaoF=2;
      //pass();
    }
  }
  
}


/*==================================================================================
�������ƣ�huandao_two
��������: ������----���߼���
ʱ�䣺2019��5��10��14:39:35
��������
����ֵ����
===================================================================================*/
void huandao_two()
{
  //pass();
  //��λ�����԰� huandaozhuangtai_flag=��
  //huandaozhuangtai_flag=0;
  //pass();
  if(huandaoF==1)   //���ڵ�һ�׶ν���
  {
    if((BRB<6||(Flag_R_1==1&&rightguai<5))&&huandaozhuangtai_flag==0&&BRB!=99)
    {
      huandaozhuangtai_flag=1;		//huandaozhuangtai_flag==1 ʱ��״̬Ϊ���ڹ���һ�����������   �������߼�������
      Image_lcd_num=1;
      //DELAY_MS(50);
    }
    if(BRB>10&&huandaozhuangtai_flag==1)    
    {
      huandaozhuangtai_flag=2;		//huandaozhuangtai_flag==2 ʱ��״̬Ϊ��ȫ������һ������������
      Image_lcd_num=2;								 //��һ�ζ�������ȫ��ȥ�ˣ�׼��ת��ڶ���������  ��Tyep_5 �������߻�����Tyep_6
    }
    //pass();
    if(Right_Lose_Border_Number>5&&Left_Lose_Border_Number>5&&huandaozhuangtai_flag==2)  
    {
      //BeepON();
      huandaozhuangtai_flag=3;		//������  ��type_0 ת��
      //pass1();
      Image_lcd_num=3;
    }
    
    if(huandaozhuangtai_flag==3&&NumbDataright-TRB>15&&Left_Lose_Border_Number<5)
    {
      huandaozhuangtai_flag=4;         //��Բ��״̬
      Image_lcd_num=4;
    }
    //pass();
    //huandaozhuangtai_flag=4;
    if(huandaozhuangtai_flag==4&&BLB==0&&Left_Lose_Border_Number>10&&BRB==0&&Right_Lose_Border_Number>10)
    {
      huandaozhuangtai_flag=5;	//����Բ����Ҫ���Եڶ�������  
      Image_lcd_num=5;
    }
    if(((NumbDataright-TRB>10&&BLB==99)||Left_Lose_Border_Number<15)&&huandaozhuangtai_flag==5)//
    {
      huandaozhuangtai_flag=6;	//����Բ����Ҫ���Եڶ�������  
      Image_lcd_num=6;
    }
    
    if(((BLB>25&&BLB!=99||Right_Lose_Border_Number==0))&&huandaozhuangtai_flag==6)//Left_Lose_Border_Number==0||
    {
      huandaoF=0;
      huandaozhuangtai_flag=0;
      Image_lcd_num=7;
    }
    
    if(huandaozhuangtai_flag==0)
    {
      yibianbuxian(2);            //������߽��������
      
    }else if(huandaozhuangtai_flag==1)		//���ڵڶ��׶ν���
    {
      yibianbuxian(2);
    }else if(huandaozhuangtai_flag==2)
    {	
      //pass();
      if(BRB==99)      //�����û�г��ֶ���   ������������������
      {
        yibianbuxian(2);
      }else
      {
        if(Common_Lose_Flag==1)
        {
          //pass();
          before_error_Flag=1;            //��ʱƫ�������һ֡��ƫ��
          
        }else   //�ո�ת�����ʱ��������ͬʱ���ߵ��������ʱ����һ֡��error�������֡   
        {
          if(BRB!=99)//pass();
          {
            if(BRB>4)
            {
              Type_5la(1);
            }else
            {
              Type_0la(1);
            }
          }
        }
      }
    }else if(huandaozhuangtai_flag==3)
    {
      //pass();
      
      if(Common_Lose_Flag==1)
      {
        //pass();
        before_error_Flag=1;
        
      }else   //�ո�ת�����ʱ��������ͬʱ���ߵ��������ʱ����һ֡��error�������֡   
      {
        if(BRB!=99)//pass();
        {
          if(BRB>4)
          {
            Type_5(1);
          }else
          {
            Type_0(1);  
          }
        }else
        {
          //yibianbuxian(2);
          Type_11();
        }
        
      }
      
    }else if(huandaozhuangtai_flag==4)
    {
      if(Common_Lose_Flag==1)
      {
        before_error_Flag=1;
      }else if(BRB>=5&&BRB!=99)
      {
        //pass1();
        Type_5la(1);
      }else if(BRB<5)
      {
        if(BRB!=99)
          Type_0la(1);
      }else 
      {
        yibianbuxian(2);
      }
      
      
    }else if(huandaozhuangtai_flag==5)
    {
      if(Common_Lose_Flag==1)
      {
        before_error_Flag=1;
        //Type_0(1);
      }
      else{
        Type_0la(1);
      }
      
    }else if(huandaozhuangtai_flag==6)
    {
      yibianbuxian(2);
      
    }
  }else if(huandaoF==2)    //==========================--==========================--��=========------======================----------------------
  {
    //pass();
    
    if((BLB<6||(Flag_L_1==2&&leftguai<5))&&huandaozhuangtai_flag==0&&BLB!=99)
    {
      //pass();
      huandaozhuangtai_flag=1;		//huandaozhuangtai_flag==1 ʱ��״̬Ϊ���ڹ���һ�����������   �������߼�������
      Image_lcd_num=1;
      //DELAY_MS(50);
    }
    
    if(BLB>10&&huandaozhuangtai_flag==1)    
    {
      //pass();
      huandaozhuangtai_flag=2;		//huandaozhuangtai_flag==2 ʱ��״̬Ϊ��ȫ������һ������������
      Image_lcd_num=2;								 //��һ�ζ�������ȫ��ȥ�ˣ�׼��ת��ڶ���������  ��Tyep_5 �������߻�����Tyep_6
    }
    //pass();
    
    if(Right_Lose_Border_Number>5&&Left_Lose_Border_Number>5&&huandaozhuangtai_flag==2)   //���������߶���ʱ�ļ���
    {
      //pass();
      //BeepON();
      huandaozhuangtai_flag=3;		//������  ��type_0 ת��
      //pass1();
      Image_lcd_num=3;
    }
    
    if(huandaozhuangtai_flag==3&&NumbDataleft-TLB>15&&Right_Lose_Border_Number<5)
    {
      //pass();
      huandaozhuangtai_flag=4;         //��Բ��״̬
      Image_lcd_num=4;
    }
    //pass();
    //huandaozhuangtai_flag=4;
    //huandaozhuangtai_flag=4;
    if(huandaozhuangtai_flag==4&&BLB==0&&Left_Lose_Border_Number>10&&BRB==0&&Right_Lose_Border_Number>10)
    {
      //pass();
      huandaozhuangtai_flag=5;	//����Բ����Ҫ���Եڶ�������  
      Image_lcd_num=5;
    }
    //huandaozhuangtai_flag=5;
    if(((NumbDataleft-TLB>10&&BRB==99)||Right_Lose_Border_Number<15)&&huandaozhuangtai_flag==5)//
    {
      //pass();
      //ImageData[BlackRight[BRB][0]][BlackRight[BRB][1]-2]=RED;
      //pass();
      huandaozhuangtai_flag=6;	//����Բ����Ҫ���Եڶ�������  
      Image_lcd_num=6;
    }
    //huandaozhuangtai_flag=6;
    if(((BLB>25&&(BLB<3)&&BLB!=99)||Left_Lose_Border_Number==0)&&huandaozhuangtai_flag==6)//(Right_Lose_Border_Number==0||
    {
      huandaoF=0;
      huandaozhuangtai_flag=0;
      Image_lcd_num=7;
    }
    
    if(huandaozhuangtai_flag==0)
    {
      yibianbuxian(1);
      
    }else if(huandaozhuangtai_flag==1)		//���ڵڶ��׶ν���
    {
      yibianbuxian(1);
    }else if(huandaozhuangtai_flag==2)
    {	
      //pass();
      if(BLB==99)      //�����û�г��ֶ���   ������������������
      {
        yibianbuxian(1);
      }else
      {
        if(Common_Lose_Flag==1)
        {
          //pass();
          before_error_Flag=2;
          
        }else   //�ո�ת�����ʱ��������ͬʱ���ߵ��������ʱ����һ֡��error�������֡   
        {
          if(BLB!=99)//pass();
          {
            if(BLB>4)
            {
              Type_5la(2);
            }else
            {
              Type_0la(2);
            }
          }
        }
      }
    }else if(huandaozhuangtai_flag==3)
    {
      //pass();
      
      if(Common_Lose_Flag==1)
      {
        //pass();
        before_error_Flag=2;//�ո�ת�����ʱ��������ͬʱ���ߵ��������ʱ����һ֡��error�������֡   
        //if()
        //Type_0(2);
      }else   
      {
        //pass();
        if(BLB!=99)//pass();
        {
          if(BLB>4)
          {
            Type_5(2);
          }else
          {
            Type_0(2);
          }
        }else
        {
          //yibianbuxian(1);
          Type_11();
        }
        
      }
      
    }else if(huandaozhuangtai_flag==4)
    {
      if(Common_Lose_Flag==1)
      {
        before_error_Flag=2;
      }else if(BLB>=5&&BLB!=99)
      {
        //pass1();
        Type_5la(2);
      }else if(BLB<5)
      {
        if(BLB!=99)
          Type_0la(2);
      }else 
      {
        yibianbuxian(1);
        
      }
      
      
    }else if(huandaozhuangtai_flag==5)
    {
      if(Common_Lose_Flag==1)
      {
        before_error_Flag=2;
      }else
      {
        Type_0la(2);
      }
      
    }else if(huandaozhuangtai_flag==6)
    {
      yibianbuxian(1);
      //Type_11();
      
    }
  }
  
}

/*uchar i=TRB;
uchar max_c=159,max_r;
for(i;i<NumbDataleft-2;i++)
{
if(BlackRight[i][1]<max_c)
{
max_c=BlackRight[i][1];
if(BlackRight[i][1]<BlackRight[i+1][1]&&BlackRight[i][1]<BlackRight[i+2][1])
{
max_r=BlackRight[i][0];
break;
		}
	}

}
ImageData[max_r][max_c-1]=RED;*/

/*==================================================================================
�������ƣ�huandao
��������: �������
ʱ�䣺2019��5��8��21:15:15
��������
===================================================================================*/
void huandao()
{
  //pass();
  if(huandaoF==0)          //������������Ͳ���ִ�д˲���      ����ȫʻ�������� huandaoF ��־λ����
  {
    huandao_one();    //��һ��----����Ƿ���ֻ���
    // Common_Lose_Flag_effective=0;           //�����й�ͬ����Ҳ�����߽�ɨ���־λ
    
  }
  //huandaoF=2;
  
  if(huandaoF!=0)
  {
    Common_Lose_Flag_effective=1;
    //pass();
    huandao_two();          //����·���滮
  }
  
}




/*==================================================================================
�������ƣ�Type_0
��������: ��Բ�������
ʱ�䣺2019��4��13��14:03:28
������a   1 Ϊ�Ұ�Բ   2 Ϊ���Բ
===================================================================================*/
void Type_0(uchar a)
{
  NumbDatacenter=0;
  
  zhixingshu++;
  saidaoleixing=0;//��������Ϊ��Բת��
  if(a==1)
  {
    yibianbuxian(2);
  }if(a==2)
  {
    yibianbuxian(1);
  }
  
}
/*==================================================================================
�������ƣ�Type_1
��������: ֱ���������͵����߼���
ʱ�䣺2019��4��13��14:03:28
��������
===================================================================================*/
void Type_1()
{
  uchar i=0;
  uchar x1,y1;			//�����±�
  NumbDatacenter=0;		//����������±�����
  zhixingshu++;			//ȫ�ֱ���ִ����
  if(TLB==NumbDataleft-1||TRB==NumbDataright-1)
  {
    Type_11();			//ֱ�߼��㷽��
    return ;
  }
  if(Left_Lose_Border_Number!=0)   //�󶪱�ֱ��
  {
    if(BLB==0)			
    {
      x1=BlackRight[TLB][0];
      y1=(BlackRight[TLB][1]+BlackLeft[TLB][1])/2;
      Center[NumbDatacenter][0]=59;
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      NumbDatacenter++;
      ConnectSpot(x1,y1);
      for(i=TLB;i<NumbDataleft;i++)
      {
        Center[i][0]=BlackLeft[i][0];
        Center[i][1]=(BlackLeft[i][1]+BlackRight[i][1])/2;
      }
      NumbDatacenter=i;
    }else
    {
      for(i=0;i<BLB;i++)
      {
        Center[NumbDatacenter][0]=BlackLeft[NumbDatacenter][0];
        Center[NumbDatacenter][1]=(BlackLeft[NumbDatacenter][1]+BlackRight[NumbDatacenter][1])/2;
        NumbDatacenter++;
      }
      
      
      x1=(uchar)(BlackLeft[TLB][0]+BlackLeft[BLB][0])/2;
      y1=BlackLeft[TLB][1];
      NumbDatacenter++;
      ConnectSpot(x1,y1);
      for(i=BLB;i<NumbDataleft-1;i++)
      {
        Center[i][0]=BlackLeft[i][0];
        Center[i][1]=(BlackLeft[i][1]+BlackRight[i][1])/2;
      }
      
      NumbDatacenter=i;
      
      
    }
  }else if(Right_Lose_Border_Number!=0)              
  {
    if(BRB==0)
    {
      x1=BlackRight[TRB][0];
      y1=(BlackRight[TRB][1]+BlackLeft[TRB][1])/2;
      Center[NumbDatacenter][0]=59;
      Center[NumbDatacenter][1]=(BlackRight[0][1]+BlackLeft[0][1])/2;
      
      NumbDatacenter++;
      ConnectSpot(x1,y1);
      
      for(i=NumbDatacenter;i<NumbDataleft;i++)
      {
        Center[NumbDatacenter][0]=BlackLeft[NumbDatacenter][0];
        Center[NumbDatacenter][1]=(BlackLeft[NumbDatacenter][1]+BlackRight[NumbDatacenter][1])/2;
        NumbDatacenter++;
        
      }
      
      
      
    }else
    {
      for(i=0;i<BRB;i++)
      {
        Center[NumbDatacenter][0]=BlackLeft[NumbDatacenter][0];
        Center[NumbDatacenter][1]=(BlackLeft[NumbDatacenter][1]+BlackRight[NumbDatacenter][1])/2;
        NumbDatacenter++;
        
      }
      x1=(BlackRight[TRB][0]+BlackRight[BRB][0])/2;
      y1=BlackRight[TRB][1];
      ConnectSpot(x1,y1);
      
      for(i=BRB;i<NumbDataleft-1;i++)
      {
        Center[i][0]=BlackLeft[i][0];
        Center[i][1]=(BlackLeft[i][1]+BlackRight[i][1])/2;
      }
      
      NumbDatacenter=i;
      
      
    }
  }
  ///2019��4��10��22:28:53
  
}

/*==================================================================================
�������ƣ�Type_5la
��������: ������ͷת������߼���,���ڻ����о�����ʱ�Լ�ʻ������ʱʹ��
ʱ�䣺2019��6��27��12��23��
��������
===================================================================================*/
void Type_5la(uchar a)
{
  //pass();
  uchar i;
  uchar x1=0,y1=0;
  zhixingshu++;
  saidaoleixing=5;      //������ͷת��
  if(a==1)
  {
    NumbDatacenter=0;
    for(i=0;i<59-BlackRight[BRB][0];i++)
    {
      Center[NumbDatacenter][0]=BlackRight[i][0];
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      NumbDatacenter++;
    }
    
    if(NumbDatacenter>0)
      NumbDatacenter--;
    if((BlackRight[BRB][0]-BlackRight[TRB][0]<10||TRB==99)&&rightguai!=0||huandaoF!=0)
    {
      for(i=BlackRight[rightguai][0];i>0&&i>(BlackRight[rightguai][0]-20);i--)
      {
        if(ImageData[i][BlackRight[rightguai][1]]>THRESHOLD&&ImageData[i-1][BlackRight[rightguai][1]]<THRESHOLD)
        {
          x1=(i-2+BlackRight[rightguai][0])/2;
          y1=BlackRight[rightguai][1];
          break;
        }
      }
    }
    x1=(BlackRight[BRB][0]+BlackRight[TRB][0])/2;
    y1=157;
    
    ConnectSpot(x1,y1);
    
  }else if(a==2)
  {
    //pass();
    for(i=0;i<59-BlackLeft[BLB][0];i++)
    {
      Center[NumbDatacenter][0]=BlackLeft[i][0];
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      NumbDatacenter++;
    }
    if(NumbDatacenter>0)
      NumbDatacenter--;
    //pass();
    //ImageData[Center[NumbDatacenter-1][0]][Center[NumbDatacenter-1][1]-1]=BLUE;
    if((BlackLeft[BLB][0]-BlackLeft[TLB][0]<10||TLB==99)&&leftguai!=0)
    {
      //pass();
      
      for(i=BlackLeft[leftguai][0];i>0&&i>(BlackLeft[leftguai][0]-20);i--)
      {
        if(ImageData[i][BlackLeft[leftguai][1]]>THRESHOLD&&ImageData[i-1][BlackLeft[leftguai][1]]<THRESHOLD)
        {
          x1=(i-2+BlackLeft[leftguai][0])/2;
          y1=BlackLeft[leftguai][1];
          
          break;
        }
      }
    }else
    {
      //pass();
      x1=(uchar)(BlackLeft[BLB][0]+BlackLeft[TLB][0])/2;
      y1=2;
      //pass();
    }
    //pass();
    //ImageData[BlackLeft[TLB][0]][4]=RED;
    ConnectSpot(x1,y1);
  }
  
}

/*==================================================================================
�������ƣ�Type_5
��������: ������ͷת������߼���
ʱ�䣺2019��4��13��14:03:28
��������
===================================================================================*/
void Type_5(uchar a)
{
  //pass();
  uchar i;
  uchar x1=0,y1=0;
  zhixingshu++;
  saidaoleixing=5;      //������ͷת��
  
  if(a==1)
  {
    NumbDatacenter=0;
    for(i=0;i<59-BlackRight[BRB][0];i++)
    {
      Center[NumbDatacenter][0]=BlackRight[i][0];
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      NumbDatacenter++;
    }
    
    if(NumbDatacenter>0)
      NumbDatacenter--;
    for(i=NumbDatacenter+1;i<NumbDataleft;i++)
    {
      
      if(Center[NumbDatacenter][1]+(int)((BlackLeft[i][1]-BlackLeft[BRB][1]))<159)
      {
        Center[i][1]=Center[NumbDatacenter][1]+(int)((BlackLeft[i][1]-BlackLeft[BRB][1]));  //+(i/left_k)  i/left_k
        //Center[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k+(BlackLeft[i][1]-(BlackLeft[0][1]+(i/left_k))); //(BlackLeft[i][1]-(BlackLeft[0][1]-(i/left_k))
      }else
      {
        Center[i][1]=159;
      }
      Center[i][0]=BlackLeft[i][0];
    }
    //pass();
    NumbDatacenter=i;
  }else if(a==2)
  {
    //pass();
    for(i=0;i<59-BlackLeft[BLB][0];i++)
    {
      Center[NumbDatacenter][0]=BlackLeft[i][0];
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      NumbDatacenter++;
    }
    if(NumbDatacenter>0)
      NumbDatacenter--;
    //pass();
    //ImageData[Center[NumbDatacenter-1][0]][Center[NumbDatacenter-1][1]-1]=BLUE;
    for(i=NumbDatacenter+1;i<NumbDataleft;i++)
    {
      
      if(Center[NumbDatacenter][1]+(int)((BlackRight[i][1]-BlackRight[BLB][1]))>2)
      {
        Center[i][1]=Center[NumbDatacenter][1]+(int)((BlackRight[i][1]-BlackRight[BLB][1]));  //+(i/left_k)  i/left_k
        //Center[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k+(BlackLeft[i][1]-(BlackLeft[0][1]+(i/left_k))); //(BlackLeft[i][1]-(BlackLeft[0][1]-(i/left_k))
      }else
      {
        Center[i][1]=2;
      }
      Center[i][0]=BlackLeft[i][0];
    }
    //pass();
    NumbDatacenter=i;
  }
  
}

/*==================================================================================
�������ƣ�Type_6
��������: ��ͨת������߼���
ʱ�䣺2019��4��13��14:03:28
��������
===================================================================================*/
void Type_6()         //������ͷת��
{
  int i,k;		//ѭ������
  int x1,y1;		//����������е�
  zhixingshu++;	//ִ����
  saidaoleixing=6;      //��������Ϊ��ͨת�� ��Ҳ���ǲ�ͬͨ����������
  NumbDatacenter=0;	//�������±�����
  if(BlackRight[rightguai][0]>BlackRight[leftguai][0])    //�ж���ջ����ҹ�  ��ʱ�ж�Ϊ�ҹ�
  {
    k=rightguai;
    for(i=0;i<=k;i++)
    {
      Center[i][0]=BlackRight[i][0];
      Center[i][1]=(BlackLeft[i][1]+BlackRight[i][1])/2;
    }
    NumbDatacenter=i;
    for(i=BlackRight[rightguai][0];i>0&&i>(BlackRight[rightguai][0]-20);i--)
    {
      if(ImageData[i][BlackRight[rightguai][1]]>THRESHOLD&&ImageData[i-1][BlackRight[rightguai][1]]<THRESHOLD)
      {
        x1=(i-2+BlackRight[rightguai][0])/2;   //iΪɨ��������
        y1=BlackRight[rightguai][1];
        ConnectSpot(x1,y1);
        break;
      }
    }
    //ImageData[x1][y1]=RED;
    
  }else                     //���
  {
    k=leftguai;
    for(i=0;i<=k;i++)           //����յ���������
    {
      Center[i][0]=BlackRight[i][0];
      Center[i][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
    }
    NumbDatacenter=i;
    for(i=BlackLeft[leftguai][0];i>0&&i>(BlackLeft[leftguai][0]-20);i--)
    {
      if(i-1==BlackLeft[leftguai][0]-20)
      {
        break;
      }
      if(ImageData[i][BlackLeft[leftguai][1]]>THRESHOLD&&ImageData[i-1][BlackLeft[leftguai][1]]<THRESHOLD)
      {
        x1=(i-2+BlackLeft[leftguai][0])/2;
        y1=BlackLeft[leftguai][1];
        ConnectSpot(x1,y1);
        break;
      }
      
    }
    
    //pass();
    //ImageData[x1][y1]=RED;
  }
  
  
  
  
  //NumbDatacenter++;
  
  
}


/*==================================================================================
�������ƣ�Type_11
��������: ��ֱͨ�߼���
ʱ�䣺2019��4��13��14:03:28
������
===================================================================================*/
void Type_11()
{
  uchar i;
  zhixingshu++;
  saidaoleixing=11;//��ֱͨ��
  for(i=0;i<NumbDataleft;i++)
  {
    Center[NumbDatacenter][0]=BlackRight[i][0];
    Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
    NumbDatacenter++;
  }
}


/***********************************************************
�������ƣ�Type_12
��������: ��߽�����ұ߽��²���΢���ߣ���ʮ�������
ʱ�䣺2019��4��13��10:18:41
������flag ��1Ϊ�ұ߽綪��   2Ϊ��߽綪��
************************************************************/
void Type_12(char flag)
{
  
  uchar i;
  zhixingshu++;
  if(flag==1)
  {
    NumbDatacenter=TRB+1;
    for(i=TRB+1;i<NumbDataleft;i++)
    {
      Center[NumbDatacenter][0]=BlackRight[i][0];
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      NumbDatacenter++;
    }
    for(i=0;i<TRB+1;i++)
    {
      Center[i][0]=BlackRight[i][0];
      Center[i][1]=Center[TRB+1][1];
    }
    
  }else if(flag==2)
  {
    NumbDatacenter=TLB+1;
    for(i=TLB+1;i<NumbDataleft;i++)
    {
      Center[NumbDatacenter][0]=BlackRight[i][0];
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      NumbDatacenter++;
    }
    for(i=0;i<TLB+1;i++)
    {
      Center[i][0]=BlackRight[i][0];
      Center[i][1]=Center[TLB+1][1];
    }
  }
}

/*==================================================================================
�������ƣ�CrossTypebuxian()
��������: ʮ�ִ����е����߶����Ҷ��������ϴ�����
ʱ�䣺2019��5��1��10:59:46
������
===================================================================================*/
void CrossTypebuxian()
{
  uchar i,j;
  /*for(i=1;i<NumbDatacenter;i++)
  {
  if(abs(Center[i][1]-Center[i-1][1])>5)
  {
  j=Center[i][1];
  while(i>0)
  {
  Center[i][1]=j;
  i--;
}
  
  break;
}
}*/
  //pass();
  if(TRB>TLB)
  {
    j=Center[TLB+1][1];
    for(i=0;i<=TLB;i++)
    {
      Center[i][1]=j;
    }
  }else
  {
    j=Center[TRB+1][1];
    for(i=0;i<=TRB;i++)
    {
      Center[i][1]=j;
    }
  }
  
}

/*==================================================================================
�������ƣ�basescan
��������: �ҳ���������ܳ��ֵ���,Ȼ���ҳ�����
ʱ�䣺2019��5��1��14:21:35
������  ���� lC  rC  R  ��߽��� �ұ߽��� �߽�������
===================================================================================*/
void basescan(uchar *lC,uchar *rC,uchar *R)
{
  uchar saidaosuozailie=80;           //�����������ڵ���
  uchar leftC=0,rightC=0;
  uchar i,j,k,q;
  uchar flag1=0;
  //uchar x1,y1;
  uchar n1=0,n2=0,m1=0,m2=0;
  uchar rnum,lnum;
  uchar lmax,rmax;
  for (i = 0; i < 75; i += 5)
  {
    rnum=0;
    lnum=0;
    for (j = 59; j > 10; j -= 3)   //������ȡһ��   ����׵������
    {
      if (ImageData[j][80 + i] > THRESHOLD)
      {
        rnum++;
      }
      if (ImageData[j][80 - i] > THRESHOLD)
      {
        lnum++;
      }
    }
    if (lmax < lnum)
    {
      lmax = lnum;
      leftC=80 - i;
    }
    
    if (rmax < rnum)
    {
      
      rmax = rnum;
      rightC = 80+i;		//��������80��
    }
  }
  if(lmax>rmax)
  {
    saidaosuozailie = leftC;
    
  }else
  {
    saidaosuozailie = rightC;
  }
  
  //ImageData[44][saidaosuozailie]=RED;
  NumbDataleft=0;
  NumbDataright=0;
  
  i=shiziscanfbase;
  saidaosuozailie=center35lie;//��λ��ֵ
  j=saidaosuozailie;
  
  q=liekuan35-10;						//35�е��п� ����ʮ��ȷ�����ұ߽��ʱ��ʹ��
  
  while (i > 5)     //����white��    ����������5
  {
    
    //pass();
    for (k = 0; k < q; k++)       //���ұ�ɨ
    {
      if ((ImageData[i][j + k] > THRESHOLD&&ImageData[i][j + k + 1] < THRESHOLD)||j+k+1==157)
      {
        n1 = i;    //�ұ߽���
        n2 = j + k + 1;  //�ұ߽���
        break;
      }
    }
    for(k=0;k<q;k++)
    {
      if ((ImageData[i][j - k] > THRESHOLD&&ImageData[i][j - k - 1] < THRESHOLD)||j - k-1==2)
      {
        m1 = i;  //��߽���
        m2 = j - k - 1;	 //��߽���
        break;
      }
    }
    
    if (m2<n2 && n2 - m2 > liekuan35-40  &&n2 - m2 > 20&& n2 != 0 && m2 != 0&&	m1==n1)    //��ֹ���
    {
      //pass();
      flag1 = 1;     //		 �������ұ߽� ������������ı�־
      
      //pass();
      BlackLeft[NumbDataleft ][0] = m1;
      BlackLeft[NumbDataleft ][1] = m2;
      
      BlackRight[NumbDataright ][0] = n1;
      BlackRight[NumbDataright ][1] = n2;
      
      //ImageData[m1][m2+2]=RED;
      //ImageData[n1][n2+2]=RED;
      NumbDataright++;
      NumbDataleft++;
      
      
    }
    
    //pass();
    
    
    if(flag1 == 1)
    {//pass();
      
      uchar pp;
      uchar pp1=(n2+m2)/2;
      //LeftRight(n1,m2,n2);
      //ImageData[55][pp1]=RED;
      for(pp=59;pp>=BlackRight[0][0];pp--)
      {
        Center[NumbDatacenter][0] = pp;
        Center[NumbDatacenter][1]=pp1;
        NumbDatacenter++;
        //pass();
      }
      
      break;
    }
    
    i--;
    
  }
}



/*======================================================================
�������ƣ�LeftRight2
��������: ������ɨ�赽�߽������� �������ɨ��õ������ұ߽������   �ӵ� hang ��ʼ ��ʼɨ�����ұ߽�ĺ�������ͬ���߲�������
ʱ�䣺2019��4��13��13:48:50
������hang      Ҫ��ʼɨ�������

ע�⣺
========================================================================*/
int LeftRight2(uchar hang)
{
  //pass();
  unsigned char scancolNumble=0;
  //char LoseBlack_Flag_R=0,LoseBlack_Flag_L=0;
  int i1,j1,j2;
  
  
  Right_Lose_Border_Number=0;
  Left_Lose_Border_Number=0;
  
  rightguai=0;
  leftguai=0;
  //Common_Lose_Flag=0;
  
  Flag_R_1=0;
  Flag_L_1=0;
  j1=BlackRight[NumbDataright-1][1];
  j2=BlackLeft[NumbDataleft-1][1];
  for(i1=hang;i1>0;i1--)
  {
    if(BlackRight[NumbDataright-1][0]>i1)
    {
      if(ImageData[i1][j1]>THRESHOLD)
      {
        
        for(;j1<=157;j1++)              //157��Ϊ�߽�
        {
          scancolNumble++;
          //pass();
          if(j1+1>=157)              //��⵽�ұ�ͼ���Ե
          {
            //pass();
            //Right_Lose_Border_Number++;				//�Ҷ�������
            BlackRight[NumbDataright][0]=i1;   //���ұ߽縳���ұ߽�����
            BlackRight[NumbDataright][1]=157;
            //ImageData[i1][j1+1]=RED;
            if(BRB==99)                            //����²��ұ߽�û�и�ֵ���ֵ���²����Ҷ�����
            {
              BRB=NumbDataright;
            }else if(1)                               //����²��ұ߽��и�ֵ������ֵ���ϲ����Ҷ�����
            {
              TRB=NumbDataright;
            }
            
            break;
          }else
          {
            //pass();
            if((ImageData[i1][j1]>THRESHOLD)&&(ImageData[i1][j1+1]<THRESHOLD))//�ж��ұ����Ƿ�����Ϊ��ɫ�߽� 
            {
              
              BlackRight[NumbDataright][0]=i1;   //���ұ߽縳���ұ߽�����
              BlackRight[NumbDataright][1]=j1+1;
              
              break;	
            }
            
          }
          if(scancolNumble>ScanColNumble&&rightguai==0)
          {
            Flag_R_1=1;
            rightguai=NumbDataright-1;
          }
          
          
        }
      }else      //59�е��ұ߽�����һ�е���һ�����Ϊ��
      {
        for(;j1>0;j1--)    
        {
          scancolNumble++;
          if(j1-1==0)
          {
            BlackRight[NumbDataright][0]=i1;	//���ұ߽縳���ұ߽�����
            BlackRight[NumbDataright][1]=j1-1;
            
            break;
          }
          if((ImageData[i1][j1]<THRESHOLD)&&(ImageData[i1][j1-1]>THRESHOLD))//�ж�������Ƿ�����Ϊ��ɫ�߽� ������һ��Flag_1��־λ�ж����Ƿ�ȷ��
          {
            BlackRight[NumbDataright][0]=i1;	//���ұ߽縳���ұ߽�����
            BlackRight[NumbDataright][1]=j1;
            break;
          }
          if(scancolNumble>ScanColNumble&&rightguai==0)
          {
            Flag_R_1=2;						//�ұ߽�����ת
            //pass1();
            rightguai=NumbDataright-1;						//�����ҹյ�����
          }
        }
      }
      NumbDataright++;
      scancolNumble=0;
      
    }
    //pass();
    //---------------------------------Right Black Scan over-----------------------------------------------------------//
    
    //---------------------------------Left Black Scan ---------------------------------------------------------------//
    if(BlackLeft[NumbDataleft-1][0]>i1)
    {
      //pass();
      
      if(ImageData[i1][j2]<THRESHOLD)         ////���е���߽���ұ�һ�е���һ�����Ϊ��
      {
        for(;j2<159;j2++)   //���ұ��п�ʼ�ж�
        {
          scancolNumble++;
          if(j2+1==157)
          {
            BlackLeft[NumbDataleft][0]=i1;   //���ұ߽縳���ұ߽�����
            BlackLeft[NumbDataleft][1]=j2+1;
            
            //pass();
            break;
          }
          if((ImageData[i1][j2]<THRESHOLD)&&(ImageData[i1][j2+1]>THRESHOLD))//�ж��ұ����Ƿ�����Ϊ��ɫ�߽� ������һ��Flag_2��־λ�ж����Ƿ�ȷ��
          {	
            BlackLeft[NumbDataleft][0]=i1;   //���ұ߽縳���ұ߽�����
            BlackLeft[NumbDataleft][1]=j2;
            break;
          }
          if(scancolNumble>ScanColNumble&&leftguai==0)
          {
            Flag_L_1=1;               //��߽�����ת
            //pass();
            leftguai=NumbDataleft-1;
          }
        }
      }else                             //���е���߽���ұ�һ�е���һ�����Ϊ��
      {
        for(;j2>=2;j2--)    
        {
          scancolNumble++;
          if(j2-1<=2)
          {
            
            BlackLeft[NumbDataleft][0]=i1;  //���ұ߽縳���ұ߽�����
            BlackLeft[NumbDataleft][1]=2;
            //Left_Lose_Border_Number++;					//�󶪱�����
            if(BLB==99)                            //����²���߽�û�и�ֵ���ֵ���²����󶪱���
            {
              BLB=NumbDataleft;
            }else                                //����²���߽��и�ֵ������ֵ���ϲ����󶪱���
            {
              TLB=NumbDataleft;
            }
            break;
          } 
          
          if((ImageData[i1][j2]>THRESHOLD)&&(ImageData[i1][j2-1]<THRESHOLD))//�ж�������Ƿ�����Ϊ��ɫ�߽� ������һ��Flag_1��־λ�ж����Ƿ�ȷ��
          {
            
            BlackLeft[NumbDataleft][0]=i1;  //���ұ߽縳���ұ߽�����
            BlackLeft[NumbDataleft][1]=j2-1;
            break;	
          }
          if(scancolNumble>ScanColNumble&&leftguai==0)
          {
            Flag_L_1=2;               //��߽�����ת
            leftguai=NumbDataleft-1;
          }
        }
      }
      scancolNumble=0;
      NumbDataleft++;
      
    }
    
    //---------------------------------Left Black Scan over-----------------------------------------------------------//
    
    /////////////////////////////����ѭ���������ж�///////////////////////////////////////////////////////
    if (BlackLeft[NumbDataleft-1][1]>=BlackRight[NumbDataright-1][1])			//��߽���������ڻ�����ұ߽����
    {
      //pass();
      sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//����������
      NumbDataleft--;
      NumbDataright--;
      break;
    }
    
    if (abs(BlackLeft[NumbDataleft-1][1]-BlackRight[NumbDataright-1][1])<=12)			//�������̫խ
    {
      //pass();
      sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//����������
      NumbDataleft--;
      NumbDataright--;
      break;
    }
    if(i1<2)
    {
      sousuojieshuhang=i1;
      break;
    }
  }
  return 1;
}

/*==================================================================================
�������ƣ�Cross
��������: ʮ�ִ���
ʱ�䣺2019��4��13��14:03:28
������
===================================================================================*/
int Cross1()
{
  
  return 0;
}

int Cross()
{
  //pass();
  uchar i,j,k,q;
  uchar flag1=0;
  uchar x1,y1;
  uchar n1=0,n2=0,m1=0,m2=0;
  NumbDatacenter=0;
  
  
  //pass();
  if(Common_Lose_Flag==1&&sousuojieshuhang<19||(Right_Lose_Border_Number>0&&Left_Lose_Border_Number>0&&TRB>30&&TLB>30)||(Flag_L_1==2&&Flag_R_1==1))    //���Կ���ʮ�ֵĴ��� 
  {
    Cross_F=1;//
    //pass();
  }
  if(Right_Lose_Border_Number==0&&Left_Lose_Border_Number==0&&!(Flag_L_1==2&&Flag_R_1==1))
  {
    Cross_F=0;
  }
  //Cross_F=1;
  if(Cross_F==1&&Common_Lose_Flag==1)
  {
  }
  
  if(Cross_F==0)
  {
    //pass();
    //if(Common_Lose_Flag==1&&sousuojieshuhang==59)//
    {
      //pass();
      if(BLB<2&&(TLB-BLB<10||TLB==99)&&BRB<2&&TRB!=99&&Left_Lose_Border_Number<10&&TRB-BRB>30&&Flag_L_1!=2&&BlackRight[NumbDataright-3][1]-BlackLeft[NumbDataright-3][1]<70)// 
      {
        //LeftRight2(58);
        //pass();
        shu(0);
        Type_0(1);
        return 0;
      }else if(BLB<2&&TLB-BLB>30&&BRB<2&&(TRB-BRB<10||TRB==99)&&Right_Lose_Border_Number<10&&Flag_R_1!=1&&Flag_L_1!=2&&BlackRight[NumbDataright-3][1]-BlackLeft[NumbDataright-3][1]<70)
      {
        //LeftRight2(58);
        //pass();
        //pass();
        shu(0);
        Type_0(2);
        return 0;
      }
      //return 0;	
    }
    
    if(abs(TRB-TLB)>crossturnnum&&TLB!=99&&TRB!=99&&(Right_Lose_Border_Number<15||Left_Lose_Border_Number<15))           //crossturnnumΪTRB-TLB  ����Ϊʮ��
    {
      //pass();
      if(TRB>TLB&&TRB-BRB>5||(Flag_L_1==1&&Flag_R_1==1))
      {
        Type_5(1);
        CrossTypebuxian();
        //pass();
        return 0;
        
      }else if(TRB<TLB&&TLB-BLB>5||(Flag_L_1==2&&Flag_R_1==2))	//
      {
        //pass();
        Type_5(2);
        CrossTypebuxian();
        return 0;
      }
    }
    return 0;
  }
  
  zhixingshu++;
  saidaoleixing=10;           //��������Ϊʮ��
  //pass();
  if(Flag_L_1==2&&Flag_R_1==1&&NumbDataleft>0)          //ʮ�����м�
  {
    //pass();
    for(i=0;i<NumbDataleft&&i<=rightguai&&i<=leftguai;i++)
    {
      Center[i][0]=BlackLeft[i][0];
      Center[i][1]=(BlackLeft[i][1]+BlackRight[i][1])/2;
    }
    NumbDatacenter=i-1;
    //ImageData[Center[NumbDatacenter][0]][Center[NumbDatacenter][1]+1]=BLUE;
    j=Center[NumbDatacenter-1][1];
    
    i=Center[NumbDatacenter-1][0];
    //ImageData[0+1][BlackLeft[NumbDatacenter-2][1]]=BLUE;
    //ImageData[i][j+1]=BLUE;
    q=liekuan35-10;						//35�е��п� ����ʮ��ȷ�����ұ߽��ʱ��ʹ��
    
    while (i> 0)     //����white��    ����������5
    {
      
      //pass();
      for (k = 0; k < q; k++)       //���ұ�ɨ
      {
        if ((ImageData[i][j + k] > THRESHOLD&&ImageData[i][j + k + 1] < THRESHOLD)||j+k+1==157)
        {
          n1 = i;    //�ұ߽���
          n2 = j + k + 1;  //�ұ߽���
          break;
        }
      }
      for(k=0;k<q;k++)
      {
        if ((ImageData[i][j - k] > THRESHOLD&&ImageData[i][j - k - 1] < THRESHOLD)||j - k-1==2)
        {
          m1 = i;  //��߽���
          m2 = j - k - 1;	 //��߽���
          break;
        }
      }
      
      if (m2<n2 && n2 - m2 < liekuan35 &&n2 - m2 > 20&& n2 != 0 && m2 != 0&&	m1==n1&&n2>m2)    //��ֹ���
      {
        //pass();
        flag1 = 1;     //		 �������ұ߽� ������������ı�־
        
        //pass();
        BlackLeft[NumbDataleft ][0] = m1;
        BlackLeft[NumbDataleft ][1] = m2;
        
        BlackRight[NumbDataright ][0] = n1;
        BlackRight[NumbDataright ][1] = n2;
        
        //ImageData[m1][m2+2]=RED;
        //ImageData[n1][n2+2]=RED;
        NumbDataright++;
        NumbDataleft++;
        
        
      }
      
      //pass();
      
      
      if(flag1 == 1)
      {//pass();
        
        uchar pp=Center[NumbDatacenter-1][0]-1;
        uchar pp1=(n2+m2)/2;
        //LeftRight(n1,m2,n2);
        //ImageData[55][pp1]=RED;
        for(pp;pp>=m1;pp--)
        {
          Center[NumbDatacenter][0] = pp;
          Center[NumbDatacenter][1]=pp1;
          NumbDatacenter++;
          //pass();
        }
        
        break;
      }
      
      i--;
      
    }
    if(flag1 == 1)
    {
      i=NumbDataleft;
      
      LeftRight1(n1);
      //pass();
      
      for (i; i <NumbDataleft; i++)
      {
        //pass();
        Center[NumbDatacenter][0] = BlackLeft[i][0];
        Center[NumbDatacenter][1] = (BlackLeft[i][1] + BlackRight[i][1]) / 2;
        NumbDatacenter++;
        //pass();
        
      }
    }
    
    
  }else if((BlackRight[0][1]==157&&BlackLeft[0][1]!=2)||(BlackRight[0][1]!=157&&BlackLeft[0][1]==2))    //  ����ұ߽�һ��ʼ�Ͷ���
  {
    //pass();
    NumbDatacenter=0;
    //pass();
    
    uchar rnum,lnum,rmax=0,lmax=0;		//ÿ�е����׵���
    uchar saidaosuozailie=80;           //�����������ڵ���
    uchar leftC=0,rightC=0;
    for (i = 0; i < 75; i += 5)
    {
      rnum=0;
      lnum=0;
      for (j = 59; j > 2; j -= 2)   //������ȡһ��   ����׵������
      {
        if (ImageData[j][80 + i] > THRESHOLD)
        {
          rnum++;
        }
        if (ImageData[j][80 - i] > THRESHOLD)
        {
          lnum++;
        }
      }
      if (lmax < lnum)
      {
        lmax = lnum;
        leftC=80 - i;
      }
      
      if (rmax < rnum)
      {
        
        rmax = rnum;
        rightC = 80+i;		//��������80��
      }
    }
    if(lmax>rmax)
    {
      saidaosuozailie = leftC;
      
    }else
    {
      saidaosuozailie = rightC;
    }
    
    ImageData[44][saidaosuozailie]=RED;
    NumbDataleft=0;
    NumbDataright=0;
    //uchar center35lie=60;
    //saidaosuozailie=center35lie;//��λ��ֵ
    
    i=shiziscanfbase;
    j=saidaosuozailie;
    //j=70;
    q=liekuan35-10;						//35�е��п� ����ʮ��ȷ�����ұ߽��ʱ��ʹ��
    
    while (i> 0)     //����white��    ����������5
    {
      
      //pass();
      for (k = 0; k < q; k++)       //���ұ�ɨ
      {
        if ((ImageData[i][j + k] > THRESHOLD&&ImageData[i][j + k + 1] < THRESHOLD)||j+k+1==157)
        {
          n1 = i;    //�ұ߽���
          n2 = j + k + 1;  //�ұ߽���
          break;
        }
      }
      for(k=0;k<q;k++)
      {
        if ((ImageData[i][j - k] > THRESHOLD&&ImageData[i][j - k - 1] < THRESHOLD)||j - k-1==2)
        {
          m1 = i;  //��߽���
          m2 = j - k - 1;	 //��߽���
          break;
        }
      }
      
      if (m2<n2 && n2 - m2 < liekuan35 &&n2 - m2 > 20&& n2 != 0 && m2 != 0&&	m1==n1&&n2>m2)    //��ֹ���
      {
        //pass();
        flag1 = 1;     //		 �������ұ߽� ������������ı�־
        
        //pass();
        BlackLeft[NumbDataleft ][0] = m1;
        BlackLeft[NumbDataleft ][1] = m2;
        
        BlackRight[NumbDataright ][0] = n1;
        BlackRight[NumbDataright ][1] = n2;
        
        //ImageData[m1][m2+2]=RED;
        //ImageData[n1][n2+2]=RED;
        NumbDataright++;
        NumbDataleft++;
        
        
      }
      
      //pass();
      
      
      if(flag1 == 1)
      {//pass();
        
        uchar pp;
        uchar pp1=(n2+m2)/2;
        if(pp1>157)pp1=80;
        if(pp1<2)pp1=80;
        //LeftRight(n1,m2,n2);
        //ImageData[55][pp1]=RED;
        for(pp=59;pp>=BlackRight[0][0];pp--)
        {
          Center[NumbDatacenter][0] = pp;
          Center[NumbDatacenter][1]=pp1;
          NumbDatacenter++;
          //pass();
        }
        
        break;
      }
      
      i--;
      
    }
    //LeftRight1(Center[NumbDatacenter-1][0]);
    if(flag1 == 1)
    {
      i=NumbDataleft;
      
      LeftRight1(n1);
      //pass();
      
      for (i; i <NumbDataleft; i++)
      {
        //pass();
        Center[NumbDatacenter][0] = BlackLeft[i][0];
        Center[NumbDatacenter][1] = (BlackLeft[i][1] + BlackRight[i][1]) / 2;
        NumbDatacenter++;
        //pass();
        
      }
    }
    
    
    ////////////////
    
    //pass();
  }else if(Common_Lose_Flag==1||(Right_Lose_Border_Number>0&&Left_Lose_Border_Number==0)||(Right_Lose_Border_Number==0&&Left_Lose_Border_Number>0))         //�й�ͬ���ߵ����
  {
    //pass();
    //pass1();
    uchar rnum,lnum,rmax=0,lmax=0;		//ÿ�е����׵���
    uchar saidaosuozailie=80;           //�����������ڵ���
    uchar leftC=0,rightC=0;
    for (i = 0; i < 75; i += 5)
    {
      rnum=0;
      lnum=0;
      for (j = 59; j > 0; j -= 3)   //������ȡһ��   ����׵������
      {
        if (ImageData[j][80 + i] > THRESHOLD)
        {
          rnum++;
        }
        if (ImageData[j][80 - i] > THRESHOLD)
        {
          lnum++;
        }
      }
      if (lmax < lnum)
      {
        lmax = lnum;
        leftC=80 - i;
      }
      
      if (rmax < rnum)
      {
        
        rmax = rnum;
        rightC = 80+i;		//��������80��
      }
    }
    if(lmax>rmax)
    {
      saidaosuozailie = leftC;
      
    }else
    {
      saidaosuozailie = rightC;
    }
    
    ImageData[44][saidaosuozailie]=RED;
    NumbDataleft=0;
    NumbDataright=0;
    
    i=shiziscanfbase;
    j=saidaosuozailie;
    //j=61;
    q=liekuan35-10;						//35�е��п� ����ʮ��ȷ�����ұ߽��ʱ��ʹ��
    
    while (i >0 )     //����white��    ����������5
    {
      
      //pass();
      for (k = 0; k < q; k++)       //���ұ�ɨ
      {
        if ((ImageData[i][j + k] > THRESHOLD&&ImageData[i][j + k + 1] < THRESHOLD)||j+k+1==157)
        {
          n1 = i;    //�ұ߽���
          n2 = j + k + 1;  //�ұ߽���
          break;
        }
      }
      for(k=0;k<q;k++)
      {
        if ((ImageData[i][j - k] > THRESHOLD&&ImageData[i][j - k - 1] < THRESHOLD)||j - k-1==2)
        {
          m1 = i;  //��߽���
          m2 = j - k - 1;	 //��߽���
          break;
        }
      }
      
      if (m2<n2 && n2 - m2 < liekuan35 &&n2 - m2 > 20&& n2 != 0 && m2 != 0&&	m1==n1&&n2>m2)    //��ֹ���
      {
        //pass();
        flag1 = 1;    
        BlackRight[NumbDataright ][0] = n1; //		 �������ұ߽� ������������ı�־
        
        //pass();
        BlackLeft[NumbDataleft ][0] = m1;
        BlackLeft[NumbDataleft ][1] = m2;
        
        BlackRight[NumbDataright ][1] = n2;
        
        //ImageData[m1][m2+2]=RED;
        //ImageData[n1][n2+2]=RED;
        NumbDataright++;
        NumbDataleft++;
        
        
      }
      
      //pass();
      
      
      if(flag1 == 1)
      {//pass();
        
        uchar pp;
        uchar pp1=(n2+m2)/2;
        //LeftRight(n1,m2,n2);
        //ImageData[55][pp1]=RED;
        for(pp=59;pp>=BlackRight[0][0];pp--)
        {
          //if()
          Center[NumbDatacenter][0] = pp;
          Center[NumbDatacenter][1]=pp1;
          NumbDatacenter++;
          //pass();
        }
        
        break;
      }
      
      i--;
      
    }
    
    if(flag1 == 1)
    {
      i=NumbDataleft;
      
      LeftRight1(n1);
      //pass();
      
      for (i; i <NumbDataleft; i++)
      {
        //pass();
        Center[NumbDatacenter][0] = BlackLeft[i][0];
        Center[NumbDatacenter][1] = (BlackLeft[i][1] + BlackRight[i][1]) / 2;
        NumbDatacenter++;
        //pass();
        
      }
    }
    return 0;
  }
  
  if(NumbDatacenter<3)
  {
    if(Right_Lose_Border_Number>Left_Lose_Border_Number)
    {
      for (i=0; i <15; i++)
      {
        Center[NumbDatacenter][0] = BlackLeft[i][0];
        Center[NumbDatacenter][1] = 157;
        NumbDatacenter++;
        //pass();
        
      }
    }else
    {
      for (i=0; i <15; i++)
      {
        Center[NumbDatacenter][0] = BlackLeft[i][0];
        Center[NumbDatacenter][1] = 2;
        NumbDatacenter++;
        //pass();
        
      }
    }
  }
  
  return 0;
  
}

/*==================================================================================
�������ƣ�ScanOneRow
��������: ɨ���ĳһ�е����ұ߽�
ʱ�䣺2019��4��13��14:03:28
������uchar R,uchar *rc,uchar *lc 
RΪɨ�������   *rc  ��  *lc Ϊ���ص� �ұ߽������ ����߽������
===================================================================================*/
void ScanOneRow(uchar R,uchar *rc,uchar *lc)
{
  int WhiteCnt;
  int W;
  int wStart,wEnd,MaxWhite,W_START=0; 
  //int W_END=157;
  int MaxStart=0;
  int MaxEnd=0;
  uchar i=R;
  //uchar Right_c=0,Left_c=0;
  
  MaxWhite=0;  //���׿�
  WhiteCnt=0;  //�׿�����
  wStart=W_START;  
  wEnd=W_START;
  W_START=0;
  
  MaxEnd=0;
  MaxWhite=0;
  wStart=2;
  WhiteCnt=0;
  for(W=2;W<157;W++)
  {
    if(ImageData[i][W]>THRESHOLD&&ImageData[i][W+1]>THRESHOLD)//����Ϊ��
    {
      WhiteCnt++;
    }else			//�ڰ����
    {
      if(WhiteCnt>=MaxWhite)			//���ڵİ׿���������֮ǰ�İ׿�����
      {
        wEnd=W;               //�ұ߽�������ֵ
        MaxWhite=WhiteCnt;		//����滻
        MaxStart=wStart;		
        MaxEnd=wEnd;			//���׿�Ľ�����
      }
      WhiteCnt=0;
      wStart=W;
    }
  }
  if(WhiteCnt>=MaxWhite)
  {
    wEnd=W;
    MaxWhite=WhiteCnt;
    MaxStart=wStart;
    MaxEnd=wEnd;
  }
  
  if(MaxEnd+1>157)MaxEnd=157;
  
  if(MaxStart<2)MaxStart=2;
  
  *rc=MaxEnd;
  
  
  *lc=MaxStart;
  
  
}


/*==================================================================================
�������ƣ�ConnectSpot
��������: �����ߺ�һ������
ʱ�䣺2019��4��13��14:03:28
��������
����ֵ��
===================================================================================*/
void ConnectSpot(int duanR,int duanC)						//��һ�����е�����
{
  int hangxiangcha,liexiangcha;
  int i,k;
  hangxiangcha=Center[NumbDatacenter-1][0]-duanR;
  liexiangcha=duanC-Center[NumbDatacenter-1][1];
  
  //ImageData[6][Center[NumbDatacenter-1][1]]=RED;
  k=NumbDatacenter-1;
  //ImageData[Center[k][0]-1][Center[k][1]]=BLUE;
  if(liexiangcha>0)
  {
    for(i=1;i<=hangxiangcha;i++)
    {
      if((int)(Center[k][1]+i*(liexiangcha)/hangxiangcha)>157)
      {
        Center[NumbDatacenter][1]=157;
      }else
      {
        Center[NumbDatacenter][1]=(int)(Center[k][1]+i*(liexiangcha)/hangxiangcha);
        //pass();
      }
      
      Center[NumbDatacenter][0]=Center[NumbDatacenter-1][0]-1;
      //ImageData[Center[NumbDatacenter][0]][Center[NumbDatacenter][1]]=RED;
      NumbDatacenter++;
      
    }
    //pass();
  }else if(liexiangcha<=0)
  {
    liexiangcha=-(liexiangcha);
    
    //pass();
    for(i=1;i<=hangxiangcha;i++)
    {
      if((int)(Center[k][1]-i*(liexiangcha)/hangxiangcha)<2)
      {
        Center[NumbDatacenter][1]=2;
      }else
      {
        Center[NumbDatacenter][1]=(int)(Center[k][1]-i*(liexiangcha)/hangxiangcha);
        //pass();
      }
      
      
      Center[NumbDatacenter][0]=Center[NumbDatacenter-1][0]-1;
      
      NumbDatacenter++;
      
    }
    
  }
  //pass();
}



/*==================================================================================
�������ƣ�Three_Border_Scan
��������: ȷ��59-57�����ұ߽磬��Ϊ����������ұ߽�ɨ��
ʱ�䣺2019��4��13��14:03:28
��������
����ֵ�� 1  59-57�е����ұ߽�ɨ������       -1  //59-57�е����ұ߽�ɨ�費����you����
===================================================================================*/
int  Three_Border_Scan()
{
  if(banmaxian_F==0)
  {
    int WhiteCnt;
    int W;
    int wStart,wEnd,MaxWhite,W_START; 
    
    
    TRB=99;
    BRB=99;
    TLB=99;
    BLB=99;
    
    sousuojieshuhang=0;
    
    NumbDataleft=0;
    NumbDataright=0;
    MaxWhite=0;  //���׿�
    WhiteCnt=0;  //�׿�����
    wStart=W_START;  
    wEnd=W_START;
    W_START=0;
    int W_END=157;
    int MaxStart=0;
    int MaxEnd=0;
    //pass();
    for(uchar i=59;i>56;i--)
    {
      //MaxStart=0;
      MaxEnd=0;
      MaxWhite=0;
      wStart=2;
      WhiteCnt=0;
      for(W=2;W<157;W++)
      {
        if(ImageData[i][W]>THRESHOLD&&ImageData[i][W+1]>THRESHOLD)//����Ϊ��
        {
          WhiteCnt++;
        }else			//�ڰ����
        {
          if(WhiteCnt>=MaxWhite)			//���ڵİ׿���������֮ǰ�İ׿�����
          {
            wEnd=W;               //�ұ߽�������ֵ
            MaxWhite=WhiteCnt;		//����滻
            MaxStart=wStart;		
            MaxEnd=wEnd;			//���׿�Ľ�����
          }
          WhiteCnt=0;
          wStart=W;
        }
      }
      if(WhiteCnt>=MaxWhite)
      {
        wEnd=W;
        MaxWhite=WhiteCnt;
        MaxStart=wStart;
        MaxEnd=wEnd;
      }
      
      if(MaxEnd+1>157)MaxEnd=157;
      
      
      if(MaxStart<=2)MaxStart=2;
      
      
      
      BlackRight[NumbDataright][1]=MaxEnd;
      BlackRight[NumbDataright][0]=i;
      BlackLeft[NumbDataleft][1]=MaxStart;
      BlackLeft[NumbDataleft][0]=i;
      
      if(MaxEnd==157)
      {
        if(BRB==99)                            //����²��ұ߽�û�и�ֵ���ֵ���²����Ҷ�����
        {
          BRB=NumbDataright;
        }else if(1)                               //����²��ұ߽��и�ֵ������ֵ���ϲ����Ҷ�����
        {
          TRB=NumbDataright;
        }
      }
      if(MaxStart==2)
      {
        if(BLB==99)                            //����²��ұ߽�û�и�ֵ���ֵ���²����Ҷ�����
        {
          BLB=NumbDataleft;
        }else if(1)                               //����²��ұ߽��и�ֵ������ֵ���ϲ����Ҷ�����
        {
          TLB=NumbDataleft;
        }
      }
      NumbDataright++;
      NumbDataleft++;
      if(MaxEnd==157&&MaxStart==2)		///������ұ߽�ͬʱ����
      {
        //pass();
        
        sousuojieshuhang=BlackRight[NumbDataright-1][0]=i;
        Common_Lose_Flag=1;
        //Common_Lose_Flag_effective=1;
        if(Common_Lose_Flag_effective==1)
        {
          return -1;
        }
      }
      
    }
  }else           //�ڼ�������������£������� �����м��    �����ż��� �� ��������޷����������������Ҳ�ϴ�
  {
    for(uchar i=59;i>56;i--)
    {
      for(uchar j=2;j<157;j++)
      {
        if(ImageData[i][j]<THRESHOLD&&ImageData[i][j+1]>THRESHOLD)
        {
          
          BlackLeft[NumbDataleft][1]=j;
          BlackLeft[NumbDataleft][0]=i;
          NumbDataleft++;
          break;
        }
        if(j==120)
        {
          BlackLeft[NumbDataleft][1]=2;
          BlackLeft[NumbDataleft][0]=i;
          NumbDataleft++;
          break;
        }
      }
      for(uchar j2=157;j2>2;j2--)
      {
        if(ImageData[i][j2]<THRESHOLD&&ImageData[i][j2-1]>THRESHOLD)
        {
          BlackRight[NumbDataright][1]=j2;
          BlackRight[NumbDataright][0]=i;
          NumbDataright++;
          break;
        }
        if(j2==20)
        {
          BlackRight[NumbDataright][1]=157;
          BlackRight[NumbDataright][0]=i;
          NumbDataright++;
          break;
        }
        
      }
    }
  }
  
  
  
  uchar i=0;
  for(i=1;i<3;i++)
  {
    if(BlackRight[i][1]-BlackRight[i-1][1]>ScanColNumble)
    {
      rightguai=i;
      Flag_R_1=1;
    }else if(BlackRight[i][1]-BlackRight[i-1][1]<-ScanColNumble)
    {
      rightguai=i;
      Flag_R_1=2;
    }
    if(BlackLeft[i][1]-BlackLeft[i-1][1]>ScanColNumble)
    {
      leftguai=i;
      Flag_L_1=1;
    }else if(BlackLeft[i][1]-BlackLeft[i-1][1]<-ScanColNumble)
    {
      leftguai=i;
      Flag_L_1=2;
    }
  }
  return 1;
}


/*==================================================================================
�������ƣ�one_Border_Scan
��������: ȷ��ĳһ�������ұ߽�
ʱ�䣺2019��5��1��14:31:42
������r  ɨ�������  lc rc  ��߽��� �ұ߽���
����ֵ�� 
===================================================================================*/
void one_Border_Scan(uchar r,uchar *lc,uchar *rc)
{
  int WhiteCnt;
  int W;
  int wStart,wEnd,MaxWhite,W_START=0; 
  
  
  MaxWhite=0;  //���׿�
  WhiteCnt=0;  //�׿�����
  wStart=W_START;  
  wEnd=W_START;
  W_START=0;
  //int W_END=157;
  int MaxStart=0;
  int MaxEnd=0;
  
  MaxEnd=0;
  MaxWhite=0;
  wStart=2;
  WhiteCnt=0;
  uchar i=r;
  for(W=2;W<157;W++)
  {
    if(ImageData[i][W]>THRESHOLD&&ImageData[i][W+1]>THRESHOLD)//����Ϊ��
    {
      WhiteCnt++;
    }else			//�ڰ����
    {
      if(WhiteCnt>=MaxWhite)			//���ڵİ׿���������֮ǰ�İ׿�����
      {
        wEnd=W;               //�ұ߽�������ֵ
        MaxWhite=WhiteCnt;		//����滻
        MaxStart=wStart;		
        MaxEnd=wEnd;			//���׿�Ľ�����
      }
      WhiteCnt=0;
      wStart=W;
    }
  }
  if(WhiteCnt>=MaxWhite)
  {
    wEnd=W;
    MaxWhite=WhiteCnt;
    MaxStart=wStart;
    MaxEnd=wEnd;
  }
  
  if(MaxEnd+1>157)MaxEnd=157;
  
  
  if(MaxStart<=2)MaxStart=2;
  
  *lc=MaxStart;
  *rc=MaxEnd;
  
}


/*======================================================================
�������ƣ�LeftRight
��������: ������ɨ�赽�߽������� �������ɨ��õ������ұ߽������   �ӵ� hang ��ʼ ��ʼɨ�����ұ߽�ĺ���
ʱ�䣺2019��4��13��13:48:50
������hang      Ҫ��ʼɨ�������

ע�⣺����δ��װ��  
���ұ߽綼ûɨ�����  Ҫ������Ԫ�ظ�ֵ  ������Ҫ����
========================================================================*/
int LeftRight1(uchar hang)
{
  //pass();
  unsigned char scancolNumble=0;
  
  int i1,j1,j2;
  
  
  Right_Lose_Border_Number=0;
  Left_Lose_Border_Number=0;
  
  rightguai=0;
  leftguai=0;
  //Common_Lose_Flag=0;
  
  Flag_R_1=0;
  Flag_L_1=0;
  j1=BlackRight[NumbDataright-1][1];
  j2=BlackLeft[NumbDataleft-1][1];
  for(i1=hang;i1>0;i1--)
  {
    if(BlackRight[NumbDataright-1][0]>i1)
    {
      if(ImageData[i1][j1]>THRESHOLD)
      {
        
        for(;j1<=157;j1++)              //157��Ϊ�߽�
        {
          scancolNumble++;
          //pass();
          if(j1+1>=157)              //��⵽�ұ�ͼ���Ե
          {
            //pass();
            //Right_Lose_Border_Number++;				//�Ҷ�������
            BlackRight[NumbDataright][0]=i1;   //���ұ߽縳���ұ߽�����
            BlackRight[NumbDataright][1]=157;
            //ImageData[i1][j1+1]=RED;
            if(BRB==99)                            //����²��ұ߽�û�и�ֵ���ֵ���²����Ҷ�����
            {
              BRB=NumbDataright;
            }else if(1)                               //����²��ұ߽��и�ֵ������ֵ���ϲ����Ҷ�����
            {
              TRB=NumbDataright;
            }
            
            break;
          }else
          {
            //pass();
            if((ImageData[i1][j1]>THRESHOLD)&&(ImageData[i1][j1+1]<THRESHOLD))//�ж��ұ����Ƿ�����Ϊ��ɫ�߽� 
            {
              
              BlackRight[NumbDataright][0]=i1;   //���ұ߽縳���ұ߽�����
              BlackRight[NumbDataright][1]=j1+1;
              
              break;	
            }
            
          }
          if(scancolNumble>ScanColNumble&&rightguai==0)
          {
            Flag_R_1=1;
            rightguai=NumbDataright-1;
          }
          
          
        }
      }else      //59�е��ұ߽�����һ�е���һ�����Ϊ��
      {
        for(;j1>0;j1--)    
        {
          scancolNumble++;
          if(j1-1==0)
          {
            BlackRight[NumbDataright][0]=i1;	//���ұ߽縳���ұ߽�����
            BlackRight[NumbDataright][1]=j1-1;
            
            break;
          }
          if((ImageData[i1][j1]<THRESHOLD)&&(ImageData[i1][j1-1]>THRESHOLD))//�ж�������Ƿ�����Ϊ��ɫ�߽� ������һ��Flag_1��־λ�ж����Ƿ�ȷ��
          {
            BlackRight[NumbDataright][0]=i1;	//���ұ߽縳���ұ߽�����
            BlackRight[NumbDataright][1]=j1;
            break;
          }
          if(scancolNumble>ScanColNumble&&rightguai==0)
          {
            Flag_R_1=2;						//�ұ߽�����ת
            //pass1();
            rightguai=NumbDataright-1;						//�����ҹյ�����
          }
        }
      }
      NumbDataright++;
      scancolNumble=0;
      
    }
    //pass();
    //---------------------------------Right Black Scan over-----------------------------------------------------------//
    
    //---------------------------------Left Black Scan ---------------------------------------------------------------//
    if(BlackLeft[NumbDataleft-1][0]>i1)
    {
      //pass();
      
      if(ImageData[i1][j2]<THRESHOLD)         ////���е���߽���ұ�һ�е���һ�����Ϊ��
      {
        for(;j2<159;j2++)   //���ұ��п�ʼ�ж�
        {
          scancolNumble++;
          if(j2+1==157)
          {
            BlackLeft[NumbDataleft][0]=i1;   //���ұ߽縳���ұ߽�����
            BlackLeft[NumbDataleft][1]=j2+1;
            
            //pass();
            break;
          }
          if((ImageData[i1][j2]<THRESHOLD)&&(ImageData[i1][j2+1]>THRESHOLD))//�ж��ұ����Ƿ�����Ϊ��ɫ�߽� ������һ��Flag_2��־λ�ж����Ƿ�ȷ��
          {	
            BlackLeft[NumbDataleft][0]=i1;   //���ұ߽縳���ұ߽�����
            BlackLeft[NumbDataleft][1]=j2;
            break;
          }
          if(scancolNumble>ScanColNumble&&leftguai==0)
          {
            Flag_L_1=1;               //��߽�����ת
            //pass();
            leftguai=NumbDataleft-1;
          }
        }
      }else                             //���е���߽���ұ�һ�е���һ�����Ϊ��
      {
        for(;j2>=2;j2--)    
        {
          scancolNumble++;
          if(j2-1<=2)
          {
            
            BlackLeft[NumbDataleft][0]=i1;  //���ұ߽縳���ұ߽�����
            BlackLeft[NumbDataleft][1]=2;
            //Left_Lose_Border_Number++;					//�󶪱�����
            if(BLB==99)                            //����²���߽�û�и�ֵ���ֵ���²����󶪱���
            {
              BLB=NumbDataleft;
            }else                                //����²���߽��и�ֵ������ֵ���ϲ����󶪱���
            {
              TLB=NumbDataleft;
            }
            break;
          } 
          
          if((ImageData[i1][j2]>THRESHOLD)&&(ImageData[i1][j2-1]<THRESHOLD))//�ж�������Ƿ�����Ϊ��ɫ�߽� ������һ��Flag_1��־λ�ж����Ƿ�ȷ��
          {
            
            BlackLeft[NumbDataleft][0]=i1;  //���ұ߽縳���ұ߽�����
            BlackLeft[NumbDataleft][1]=j2-1;
            break;	
          }
          if(scancolNumble>ScanColNumble&&leftguai==0)
          {
            Flag_L_1=2;               //��߽�����ת
            leftguai=NumbDataleft-1;
          }
        }
      }
      scancolNumble=0;
      NumbDataleft++;
      
    }
    
    //---------------------------------Left Black Scan over-----------------------------------------------------------//
    
    /////////////////////////////����ѭ���������ж�///////////////////////////////////////////////////////
    if (BlackLeft[NumbDataleft-1][1]>=BlackRight[NumbDataright-1][1])			//��߽���������ڻ�����ұ߽����
    {
      //pass();
      sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//����������
      NumbDataleft--;
      NumbDataright--;
      break;
    }
    
    if (abs(BlackLeft[NumbDataleft-1][1]-BlackRight[NumbDataright-1][1])<=12)			//�������̫խ
    {
      //pass();
      sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//����������
      NumbDataleft--;
      NumbDataright--;
      break;
    }
    
    if (ImageData[BlackLeft[NumbDataleft-1][0]][BlackLeft[NumbDataleft-1][1]]>THRESHOLD&&ImageData[BlackRight[NumbDataright-1][0]][BlackRight[NumbDataright-1][1]]>THRESHOLD)
    {
      sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//����ͬʱ���ߣ�����������
      
      //ImageData[BlackLeft[NumbDataleft-1][1]][0]=RED;
      //pass();
      NumbDataleft--;
      NumbDataright--;
      Common_Lose_Flag=1;
      break;
    }
    if(i1<2)
    {
      sousuojieshuhang=i1;
      break;
    }
  }
  return 1;
}



/***********************************************************
�������ƣ�BorderScan
��������: 
A�����������߽磺�ӻ�������ɨ��õ����ұ߽磬��2��Ϊ�󶪱��У���157Ϊ�Ҷ�����
B��BLB TLB BRB TRB ����¼���ұ߽��������Ķ���������������Ķ�������
C��
ʱ�䣺
������none
************************************************************/
void BorderScan()
{
  //Common_Lose_Flag_effective=1;       //����⵽����ʱ�����㹲ͬ����Ҳ���ᣬ���ұ��
  unsigned char scancolNumble=0;
  char LoseBlack_Flag_R=0,LoseBlack_Flag_L=0;
  int i1,j1,j2;
  
  Common_Lose_Flag_effective=1;
  if(Three_Border_Scan()!=-1||Common_Lose_Flag_effective||1)            //���ұ߽���㶼�ҵ�
  {
    //pass();
    j1=BlackRight[NumbDataright-1][1];
    j2=BlackLeft[NumbDataleft-1][1];
    //ImageData[0][j1]=RED;
    
    
    //ImageData[0][j1]=RED;
    for(i1=59;i1>0;i1--)
    {
      //pass();
      
      //---------------------------------Right Black Scan ---------------------------------------------------------------//
      //ImageData[0][BlackRight[NumbDataright-1][0]]=RED;
      if(BlackRight[NumbDataright-1][0]>i1)
      {
        //pass();
        //pass1();
        
        if(ImageData[i1][j1]>THRESHOLD)
        {
          
          for(j1;j1<=157;j1++)              //157��Ϊ�߽�
          {
            scancolNumble++;
            //pass();
            if(j1+1>=157)              //��⵽�ұ�ͼ���Ե
            {
              //pass();
              //Right_Lose_Border_Number++;				//�Ҷ�������
              BlackRight[NumbDataright][0]=i1;   //���ұ߽縳���ұ߽�����
              BlackRight[NumbDataright][1]=157;
              //ImageData[i1][j1+1]=RED;
              if(duan_diubian_R==0)
              {
                if(BRB==99)                            //����²��ұ߽�û�и�ֵ���ֵ���²����Ҷ�����
                {
                  BRB=NumbDataright;
                }
                if(NumbDataright-1==TRB||TRB==99)                               //����²��ұ߽��и�ֵ������ֵ���ϲ����Ҷ�����
                {
                  TRB=NumbDataright;
                  //ImageData[BlackRight[TRB][0]][BlackRight[TRB][1]+1]=BLUE;
                  //if(ImageData[BlackRight[TRB][0]-1][])
                }else if(NumbDataright-1!=TRB&&TRB!=99)
                {
                  //pass();
                  duan_diubian_R=1;
                }
              }
              if(duan_diubian_R==1)
              {
                if(BRB2==99)                            //����²��ұ߽�û�и�ֵ���ֵ���²����Ҷ�����
                {
                  BRB2=NumbDataright;
                  //pass();
                }else if(NumbDataright-1==TRB2||TRB2==99)                               //����²��ұ߽��и�ֵ������ֵ���ϲ����Ҷ�����
                {
                  //pass();
                  
                  TRB2=NumbDataright;
                  //ImageData[BlackRight[TRB2][0]][BlackRight[TRB2][1]+2]=RED;
                }
              }
              
              
              
              break;
            }else
            {
              //pass();
              if((ImageData[i1][j1]>THRESHOLD)&&(ImageData[i1][j1+1]<THRESHOLD))//�ж��ұ����Ƿ�����Ϊ��ɫ�߽� 
              {
                
                BlackRight[NumbDataright][0]=i1;   //���ұ߽縳���ұ߽�����
                BlackRight[NumbDataright][1]=j1+1;
                
                break;	
              }
              
            }
            if(scancolNumble>ScanColNumble&&rightguai==0)
            {
              Flag_R_1=1;
              rightguai=NumbDataright-1;
            }
            
            
          }
        }else      //59�е��ұ߽�����һ�е���һ�����Ϊ��
        {
          for(j1;j1>0;j1--)    
          {
            scancolNumble++;
            if(j1-1==0)
            {
              BlackRight[NumbDataright][0]=i1;	//���ұ߽縳���ұ߽�����
              BlackRight[NumbDataright][1]=j1-1;
              
              break;
            }
            if((ImageData[i1][j1]<THRESHOLD)&&(ImageData[i1][j1-1]>THRESHOLD))//�ж�������Ƿ�����Ϊ��ɫ�߽� ������һ��Flag_1��־λ�ж����Ƿ�ȷ��
            {
              BlackRight[NumbDataright][0]=i1;	//���ұ߽縳���ұ߽�����
              BlackRight[NumbDataright][1]=j1;
              break;
            }
            if(scancolNumble>ScanColNumble&&rightguai==0)
            {
              Flag_R_1=2;						//�ұ߽�����ת
              //pass1();
              rightguai=NumbDataright-1;						//�����ҹյ�����
            }
          }
        }
        NumbDataright++;
        scancolNumble=0;
        
      }
      
      //---------------------------------Right Black Scan over-----------------------------------------------------------//
      
      //---------------------------------Left Black Scan ---------------------------------------------------------------//
      if(BlackLeft[NumbDataleft-1][0]>i1)
      {
        //pass();
        
        if(ImageData[i1][j2]<THRESHOLD)         ////���е���߽���ұ�һ�е���һ�����Ϊ��
        {
          for(j2;j2<159;j2++)   //���ұ��п�ʼ�ж�
          {
            scancolNumble++;
            if(j2+1==157)
            {
              BlackLeft[NumbDataleft][0]=i1;   //���ұ߽縳���ұ߽�����
              BlackLeft[NumbDataleft][1]=j2+1;
              
              //pass();
              break;
            }
            if((ImageData[i1][j2]<THRESHOLD)&&(ImageData[i1][j2+1]>THRESHOLD))//�ж��ұ����Ƿ�����Ϊ��ɫ�߽� ������һ��Flag_2��־λ�ж����Ƿ�ȷ��
            {	
              BlackLeft[NumbDataleft][0]=i1;   //���ұ߽縳���ұ߽�����
              BlackLeft[NumbDataleft][1]=j2;
              break;
            }
            if(scancolNumble>ScanColNumble&&leftguai==0)
            {
              Flag_L_1=1;               //��߽�����ת
              //pass();
              leftguai=NumbDataleft-1;
            }
          }
        }else                             //���е���߽���ұ�һ�е���һ�����Ϊ��
        {
          for(j2;j2>=2;j2--)    
          {
            scancolNumble++;
            if(j2-1<=2)
            {
              
              BlackLeft[NumbDataleft][0]=i1;  //���ұ߽縳���ұ߽�����
              BlackLeft[NumbDataleft][1]=2;
              //Left_Lose_Border_Number++;					//�󶪱�����
              if(duan_diubian_L==0)
              {
                if(BLB==99)                            //����²���߽�û�и�ֵ���ֵ���²����󶪱���
                {
                  BLB=NumbDataleft;
                }
                if(NumbDataleft-1==TLB||TLB==99)                               //����²���߽��и�ֵ������ֵ���ϲ����󶪱���
                {
                  TLB=NumbDataleft;
                  //ImageData[BlackLeft[TLB][0]][BlackLeft[TLB][1]+1]=BLUE;
                  
                }else if(NumbDataright-1!=TLB&&TLB!=99)
                {
                  //ImageData[BlackLeft[TLB][0]][BlackLeft[TLB][1]+1]=BLUE;
                  duan_diubian_L=1;
                }
              }
              if(duan_diubian_L==1)
              {
                if(BLB2==99)                            //����²���߽�û�и�ֵ���ֵ���²����󶪱���
                {
                  BLB2=NumbDataleft;
                }else if(NumbDataleft-1==TLB2||TLB2==99)                               //����²���߽��и�ֵ������ֵ���ϲ����󶪱���
                {
                  TLB2=NumbDataleft;
                }
              }
              
              break;
            } 
            
            if((ImageData[i1][j2]>THRESHOLD)&&(ImageData[i1][j2-1]<THRESHOLD))//�ж�������Ƿ�����Ϊ��ɫ�߽� ������һ��Flag_1��־λ�ж����Ƿ�ȷ��
            {
              
              BlackLeft[NumbDataleft][0]=i1;  //���ұ߽縳���ұ߽�����
              BlackLeft[NumbDataleft][1]=j2-1;
              break;	
            }
            if(scancolNumble>ScanColNumble&&leftguai==0)
            {
              Flag_L_1=2;               //��߽�����ת
              leftguai=NumbDataleft-1;
            }
          }
        }
        scancolNumble=0;
        NumbDataleft++;
        
      }
      
      //---------------------------------Left Black Scan over-----------------------------------------------------------//
      
      /////////////////////////////����ѭ���������ж�///////////////////////////////////////////////////////
      if (BlackLeft[NumbDataleft-1][1]>=BlackRight[NumbDataright-1][1])			//��߽���������ڻ�����ұ߽����
      {
        //pass();
        jiesujiance_F=1;
        if(sousuojieshuhang==0)
          sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//����������
        NumbDataleft--;
        NumbDataright--;
        break;
      }
      if (abs(BlackLeft[NumbDataleft-1][1]-BlackRight[NumbDataright-1][1])<=20&&i1<45)			//�������̫խ
      {
        //pass();
        if(sousuojieshuhang==0)
        {
          
          sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//����������
        }
          
        NumbDataleft--;
        NumbDataright--;
        break;
      }
      //pass();
      if ((BlackLeft[NumbDataleft-1][1]==2&&BlackRight[NumbDataright-1][1]==157))
      {
        //pass();
        Common_Lose_Flag=1;
        if(sousuojieshuhang==0)
          sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//����ͬʱ���ߣ�����������
        //NumbDataleft--;
        //NumbDataright--;
        //if(Common_Lose_Flag_effective==0)//Common_Lose_Flag_effective ����ͬʱ����Ҳ������ɨ��
        {
          //ImageData[BlackLeft[NumbDataleft-1][1]][0]=RED;
          //pass();
          //break;
        }
        
        //pass();
      }
      //ImageData[BlackLeft[NumbDataleft-1][0]][BlackLeft[NumbDataleft-1][1]+1]=RED;
      /*	*/
      if(i1<2)
      {
        if(sousuojieshuhang==0)
          sousuojieshuhang=i1;
        break;
      }
      
    }//i1ѭ��
    //pass();
  }
  //pass();
  //if(BLB2!=0)pass();
  //ImageData[BlackRight[BRB2][0]][BlackRight[BRB2][1]+1]=RED;
  //ImageData[BlackRight[TRB2][0]][BlackRight[TRB2][1]+1]=RED;
  //if(BRB2!=99)pass();
  if(BRB2!=99&&TRB2-BRB2>TRB-BRB)
  {
    BRB=BRB2;
    TRB=TRB2;
    //BRB2=99;
    //TRB2=99;
  }
  if(BLB2!=99&&TLB2-BLB2>TLB-BLB)
  {
    //pass();
    BLB=BLB2;
    TLB=TLB2;
    //BLB2=99;
    //TLB2=99;
  }
}


/*======================================================================
�������ƣ�p_rintf
��������: ��ӡ�����б߽磬���ҹյ�
ʱ�䣺2019��4��13��13:48:50
������
======================================================================*/
void p_rintf()
{
  /*                                    //��λ��ע�͵�
  uchar i;
  for(i=0;i<NumbDataleft;i++)
  {
  if(i<3)
  {
  ImageData[BlackLeft[i][0]][BlackLeft[i][1]]=GREEN;
  
}else
  ImageData[BlackLeft[i][0]][BlackLeft[i][1]]=GREEN;
}
  for(i=0;i<NumbDataright;i++)
  {
  if(i<3)
  {
  ImageData[BlackRight[i][0]][BlackRight[i][1]]=GREEN;
  
}else
  
  ImageData[BlackRight[i][0]][BlackRight[i][1]]=GREEN;
}
  for(i=0;i<NumbDatacenter;i++)
  {
  
  ImageData[Center[i][0]][Center[i][1]]=RED;
  
}
  
  if(rightguai!=0)
  {
  ImageData[BlackRight[rightguai][0]][BlackRight[rightguai][1]]=RED;
  
}
  
  
  if(leftguai!=0)
  {
  ImageData[BlackLeft[leftguai][0]][BlackLeft[leftguai][1]]=RED;
}
  
  */
}




/*======================================================================
�������ƣ�xielvfangcha
��������: �������ұ߽�б�ʣ��������ұ߽綪����
ʱ�䣺2019��4��13��13:48:50
������
======================================================================*/
void xielvfangcha()
{
  //shu(2);
  //shu(10);
  //shu(4);
  uchar i=0;    //ѭ������
  if(Common_Lose_Flag==1&&Common_Lose_Flag_effective==0)	        //�����ͬʱ���߲�ִ����Common_Lose_Flag_effective==0Ϊ0��ʱ��
  {
    return;
  }
  
  uchar fangchashu=0;					//������
  //uchar tongji_k_right_add=0;			//б�ʷ���
  
  //////��߽�б��///////////////////////////////
  for(i=0;i<NumbDataleft-3;i++)
  {
    if(BlackLeft[i][1]>2)
    {
      k_of_left_line[fangchashu]=(BlackLeft[i+3][1]-BlackLeft[i][1])*2/(BlackLeft[i+3][0]-BlackLeft[i][0]);
      k_of_left_line_pingjun+=k_of_left_line[i];//ͳ�Ƴ���ֵ
      fangchashu++;
    }
  }
  
  k_of_left_line_pingjun=k_of_left_line_pingjun/fangchashu;		//��߽�б��ƽ��ֵ   ��ֵ�ǳ���2��ֵ
  
  for(i=0;i<fangchashu;i++)
  {
    k_of_left_line_fangcha+=(k_of_left_line[i]-k_of_left_line_pingjun)*(k_of_left_line[i]-k_of_left_line_pingjun);//��߽緽��
  }
  ///////////�ұ߽�б��/////////////////////////////
  fangchashu=0;
  for(i=0;i<NumbDataright-3;i++)
  {
    //if(BlackRight[i][1]<157)
    {
      k_of_right_line[fangchashu]=(BlackRight[i+3][1]-BlackRight[i][1])*2/(BlackRight[i+3][0]-BlackRight[i][0]);
      k_of_right_line_pingjun+=k_of_right_line[i];//ͳ�Ƴ���ֵ
      fangchashu++;
    }
  }
  //ImageData[5][fangchashu]=RED;
  k_of_right_line_pingjun=k_of_right_line_pingjun/fangchashu;
  for(i=0;i<fangchashu;i++)
  {
    k_of_right_line_fangcha+=(k_of_right_line[i]-k_of_right_line_pingjun)*(k_of_right_line[i]-k_of_right_line_pingjun); //�ұ߽緽��
  }
  
  Left_Lose_Border_Number=0;		//ͳ����߽綪����
  i=0;
  
  for(;i<NumbDataleft-1;i++)
  {
    if(BlackLeft[i][1]<=2)
    {
      Left_Lose_Border_Number++;    //�����󶪱���
      //pass();
      if(BlackLeft[i+1][1]>2)
      {
        left_diubiancishu++;        //�����󶪱ߴ���   
      }
      
    }
  }
  
  
  
  Right_Lose_Border_Number=0;		//ͳ���Ҷ�����
  
  for(i=0;i<NumbDataright;i++)
  {
    if(BlackRight[i][1]>=157)
    {
      Right_Lose_Border_Number++;     //�����Ҷ�����
      if(BlackRight[i+1][1]<157)
      {
        right_diubiancishu++;         //�����Ҷ��ߴ���
      }
      
      
    }
  }
  if(right_diubiancishu==0&&BRB!=99)    //��ֹ��һֱ���ߵ�ʱ���ֵ�����Ϊ1
  {
    right_diubiancishu=1;
  }
  if(left_diubiancishu==0&&BLB!=99)     //��ֹ��һֱ���ߵ�ʱ���ֵ�����Ϊ1
  {
    left_diubiancishu=1;
  }
  
}

/*==================================================================================
�������ƣ�guaidiancredible
��������: ���ݹյ���ֵ���������жϹյ��Ƿ����
ʱ�䣺2019��4��28��18:34:10
������
����ֵ��LRbeyond_F    0 ������    1 ����
===================================================================================*/
void guaidiancredible()
{
  if(leftguai!=0&&rightguai!=0)
  {
    if(abs(leftguai-rightguai)<shizhizuidahangkuan)  //20  ʮ������п�
    {
      LRbeyond_F=1;
    }else
    {
      LRbeyond_F=0;
    }
  }else
  {
    LRbeyond_F=0;
  }
  
}


/*==================================================================================
�������ƣ�diubiancredible
��������: �˺���������ת��ʱ��һ���Ķ������ʼ���ߣ�����һ�������ͷ���ߣ���ʱ�жϳ��������Ȼ�����һ�ߵĶ���������
ʱ�䣺2019��5��5��19:44:56
������
����ֵ�� 0   1   ������    ����
===================================================================================*/
int diubiancredible()
{
  //pass();
  if((BLB!=99&&BRB!=99)&&abs(BLB-BRB)>20||(BLB==99&&BRB!=99)||(BLB!=99&&BRB==99))
  {
    //pass();
    if(BLB<5&&Left_Lose_Border_Number<10)
    {
      //pass();
      Left_Lose_Border_Number=0;
      BLB=99;
      TLB=99;
      
    }else if(BRB<5&&Right_Lose_Border_Number<10)//
    {
      //pass();
      //pass1();
      Right_Lose_Border_Number=0;
      BRB=99;
      TRB=99;
    }
    
  }
  if(abs(BLB-BRB)>20&&(Left_Lose_Border_Number<5||Right_Lose_Border_Number<5))
  {
    return 0;
  }else
  {
    return 1;
  }
}
/*==================================================================================
�������ƣ�diubiankexingpanduan
��������: �ж϶����Ƿ�Ӧ�ò��ã�TLB-BLB<10  BLB>3   ������϶���������
ʱ�䣺2019��5��25��12:00:09
������
����ֵ��
===================================================================================*/
void diubiankexingpanduan()
{
  /*if(TLB-BLB<10&&BLB>3&&BRB==99)
  {
  
  Left_Lose_Border_Number=0;
}
  if(TRB-BRB<10&&BRB>3&&BLB==99)
  {
  
  Right_Lose_Border_Number=0;
}
  //if(TRB-BRB<10&&BRB<3&&BLB==99)
  //pass(); */
}


/*==================================================================================
�������ƣ�shizixiediubian
��������: ���бʮ�ֵ��������
ʱ�䣺
������
����ֵ��1��⵽  0 δ��⵽
===================================================================================*/
int shizixiediubian()
{
  if(Flag_L_1==0&&Flag_R_1==2)
  {
    if(BRB<3&&TRB-BRB<10&&BLB==99)
    {
      Cross();
      return 1;
    }
  }
  if(Flag_L_1==1&&Flag_R_1==0)
  {
    if(BLB<3&&TLB-BLB<10&&BRB==99)
    {
      Cross();
      return 1;
    }
  }
  return 0;
}

/*==================================================================================
�������ƣ�Type_5_2
��������: ������ͷת������߼���  ����ڶ������ߵļ���
ʱ�䣺2019��6��16��16:39:36
��������
===================================================================================*/
void Type_5_2(uchar a)
{
  //pass();
  uchar i;
  uchar x1=0,y1=0;
  zhixingshu++;
  if(a==1)
  {
    NumbDatacenter=0;
    for(i=0;i<59-BlackRight[BRB2][0];i++)
    {
      Center[NumbDatacenter][0]=BlackRight[i][0];
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      //pass();
      NumbDatacenter++;
    }
    NumbDatacenter--;
    
    x1=(BlackRight[BRB2][0]+BlackRight[TRB2][0])/2;
    y1=157;
    
    ConnectSpot(x1,y1);
    
  }else if(a==2)
  {
    //pass();
    for(i=0;i<59-BlackLeft[BLB2][0];i++)
    {
      Center[NumbDatacenter][0]=BlackLeft[i][0];
      Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
      NumbDatacenter++;
    }
    NumbDatacenter--;
    //pass();
    //ImageData[Center[NumbDatacenter-1][0]][Center[NumbDatacenter-1][1]-1]=BLUE;
    if((BlackLeft[BLB][0]-BlackLeft[TLB][0]<5||TLB==99)&&leftguai!=0)
    {
      //pass();
      
      for(i=BlackLeft[leftguai][0];i>0&&i>(BlackLeft[leftguai][0]-20);i--)
      {
        if(ImageData[i][BlackLeft[leftguai][1]]>THRESHOLD&&ImageData[i-1][BlackLeft[leftguai][1]]<THRESHOLD)
        {
          x1=(i-2+BlackLeft[leftguai][0])/2;
          y1=BlackLeft[leftguai][1];
          break;
        }
      }
    }else
    {
      //pass();
      x1=(uchar)(BlackLeft[BLB2][0]+BlackLeft[TLB2][0])/2;
      y1=2;
      //pass();
    }
    //ImageData[BlackLeft[TLB][0]][4]=RED;
    ConnectSpot(x1,y1);
  }
  
}

/*==================================================================================
�������ƣ�Type_Judgement
��������: ���ݱ�־λ������ұ߽�������������ж�
ʱ�䣺2019��4��13��14:03:28
������
����ֵ��
===================================================================================*/
void Type_Judgement()
{
  if(shizixiediubian()==1)  //����бʮ�ֵ�����
  {
    return;
  }
  diubiancredible();
  guaidiancredible();
  
  diubiankexingpanduan();
  zuidaliekuan();           //������п�  �� ����б��
  
  
  huandao();                    //��������
  
  //pass();
  if(huandaoF!=0)
  {
    //BeepON();
    //pass();
    return ;
  }
  //pass();
  if (Common_Lose_Flag == 1)
  {
    //pass1();
    //pass();
    shu(10);
    Cross();
    return ;
  }else
  {
    //pass1();
    //if(BRB2!=99)pass();
    //pass();
    /*	if(BRB2!=99&&TRB2-BRB2>5&&Flag_L_1==1)
    {
    //pass1();
    shu(5);
    Type_5_2(1);
    //pass();
    return;
  }else if(BLB2!=99&&TLB2-BRB2>5&&Flag_R_1==2)
    {
    //pass();
    shu(5);
    Type_5_2(2);
    //pass();
    return;
  }*/
    
    if (Left_Lose_Border_Number > 3&& Right_Lose_Border_Number < 5 )//
    {
      //pass1();
      if (BlackLeft[BLB][0] > 55)     //��Բ����
      {
        //pass();
        
        if (BlackLeft[TLB][0] - BlackLeft[NumbDataleft - 1][0] < 8)   //��Ч�����ڶ�����8����  ||Left_Lose_Border_Number>25
        {
          shu(0);
          Type_0(2);
          //pass();
          return ;
        }
        else                  //  ��Ч�����ڶ�����10����     //��������   ʮ��    ֱ�߶���   ��Բʮ��    
        {
          //pass();
          if(TLB-BLB>25)
          {
            //pass();
            if(NumbDataleft>45)
            {
              //pass1();
              if(Flag_L_1==1&&Flag_R_1==1)
              {
                shu(6);
                Type_6();
              }else if(Flag_L_1==1||Flag_R_1==1)
              {
                if(Left_Lose_Border_Number>40)
                {
                  shu(0);
                  Type_0(2);
                }else
                {
                  shu(1);
                  Type_11();
                }
                return ;
              }else
              {
                //                shu(0);
                //                //yibianbuxian(1);
                //                Type_0(2);
                //Type_0(2);
                shu(1);
                Type_11();
              }
            }else
            {
              shu(0);
              Type_0(2);
            }
            
            return;
          }else if((int)k_of_right_line_fangcha > 1000 && (int)k_of_left_line_fangcha>1000)
          {
            //pass();
            if(NumbDataleft-TLB>25)
            {
              shu(1);
              Type_11();
              return;
            }else
            {
              shu(0);
              Type_0(2);
            }
            return ;
          } else if((int)k_of_right_line_fangcha < 700 && (int)k_of_left_line_fangcha < 700)    //ֱ�߶���
          {
            //pass();
            //pass();
            if(BlackLeft[TLB+1][1]-BlackLeft[TLB][1]>8)    //12  Type_12(2);
            {
              //pass();
              shu(10);
              Cross();
              //pass();
              return ;
              
            }else
            {
              //pass();
              shu(1);
              Type_11();
              return ;
            }
            //pass();
            
          }else if((int)k_of_right_line_fangcha>1000&&(int)k_of_left_line_fangcha <1000)    //бʮ��    бʮ��  �ұ߽��ϲ��߽粻���ţ���߽��°벿�����ϰ벿�߽�����        �����       2019��4��13��09:58:47
          {
            //pass();
            if(Flag_L_1==1&&Flag_R_1==1&&LRbeyond_F==1)
            {
              
              shu(6);
              Type_6();
              //pass();
              return ;
              
            }else
            {
              if((int)k_of_left_line_fangcha <800&&Flag_R_1==1)
              {
                shu(1);
                //yibianbuxian(1);
                Type_11();
              }else
              {
                //pass();
                shu(10);
                //pass();
                Cross();
                //pass();
              }
              return ;
              
            }
          }else    //��ʮ�ִ���      
          {
            //pass();
            if(BlackRight[TLB][1]-BlackLeft[TLB][1]<saidaokuan-10)
            {
              shu(1);
              Type_11();
              
            }else
            {
              shu(10);
              Cross();
              //pass();
            }
            return ;
          }
          //pass();
        }
        //pass();
        
      }
      else             //BlackLeft[BLB][0] < 55   ���������Ϸ�
      {
        //pass();
        //pass();
        //if()
        if (BlackLeft[TLB][0] - BlackLeft[NumbDataleft - 1][0] < 8)   //��Ч�����ڶ�����8����     ������ͷ���   ʮ��
        {
          //pass();
          if ((int)k_of_right_line_fangcha < 1000|| (Flag_L_1==2 && (int)k_of_right_line_fangcha < 1500))   //&& Flag_R_1 == 2
          {
            shu(5);
            Type_5(2);
            return ;
          }else if(Flag_R_1 == 2 && Flag_L_1 == 2&&LRbeyond_F==1)
          {
            shu(5);
            Type_5(2);
            return ;
          }else if (Flag_R_1 == 1 && Flag_L_1 == 2&&LRbeyond_F==1)
          {
            //pass();
            shu(10);
            Cross();
            return ;
          }else if((int)k_of_right_line_fangcha < 500&&(int)k_of_left_line_fangcha < 500)
          {
            shu(1);
            Type_11();    //��ֱ��������������
            //Type_11();
            return ;
          }
          else               //�˴������� 2019��4��12��20:22:29
          {
            /*//pass();
            shu(10);
            Cross();
            return ;*/
          }
          
        }
        else                //��Ч�����ڶ�����5����   ʮ�����͵����������ͷ���  ֱ�߶���   2019��4��11��22:31:12
        {
          //pass();
          if(NumbDataleft<46)  //��Ч��С��46��
          {
            if(Flag_R_1 == 2 && Flag_L_1 == 2)
            {
              shu(5);
              Type_5(2); 
              return;
            }
          }
          //pass();
          // pass();
          if((int)k_of_right_line_fangcha < 500 && (int)k_of_left_line_fangcha < 500)      //���߷��С
          {
            //pass();
            if(Left_Lose_Border_Number>=10&&Flag_R_1==2)
            {
              shu(5);
              Type_5(2);
              return;
            }else
            {
              shu(1);
              Type_11(); 
              return ;
            }
            
          }else if((int)k_of_right_line_fangcha < 1000)//&& Flag_R_1 == 2
          {
            //pass();
            if(Flag_R_1 == 2)
            {
              shu(5);
              Type_5(2);
            }else if((int)k_of_left_line_fangcha < 1000&&NumbDataleft>45)
            {
              shu(1);
              Type_11();
            }else
            {
              shu(5);
              Type_5(2);
            }
            
            return ;
            
          }else if((int)k_of_right_line_fangcha > 1000 &&(int)k_of_left_line_fangcha > 1000 )
          {
            //pass();
            if(Flag_R_1 == 1 && Flag_L_1 == 2||LRbeyond_F==0)
            {
              //pass();
              shu(10);
              Cross();
              
              
              return ;
            }else
            {
              //pass();
              shu(5);
              Type_5(2);
              return ;
            }
            
          }else if((int)k_of_left_line_fangcha < 1000&&(int)k_of_right_line_fangcha > 1000)
          {
            //pass();
            if(Flag_R_1 == 2 && Flag_L_1 == 1)
            {
              shu(5);
              Type_5(2);
              return;
            }else
            {
              shu(1);
              Type_11();
              return ;
            }
            //pass();
            
            
          }else if((int)k_of_left_line_fangcha > 1000&&(int)k_of_right_line_fangcha < 1000)
          {
            //pass();
            if(Flag_R_1 == 2 && Flag_L_1 == 1)
            {
              shu(5);
              Type_5(2);
              return;
            }
          }
          //pass();
        }
        
        
        
      }
      
    }////------------------------------------�Ҷ��߿�ʼ----------------------------------------------------
    else if (Left_Lose_Border_Number < 5 && Right_Lose_Border_Number > 3)    //�Ҷ���
    {
      
      //pass();
      if (BlackRight[BRB][0] > 55)     //�������濪ʼ����
      {
        //pass();
        if (BlackRight[TRB][0] - BlackRight[NumbDataright - 1][0] < 8)   //��Ч�����ڶ�����8����  ||Right_Lose_Border_Number>25
        {
          shu(0);
          Type_0(1);
          return ;
          //pass();
        }
        else                  //  ��Ч�����ڶ�����5����     //��������   ʮ��    ֱ�߶���   ��Բʮ��
        {
          //pass();
          //pass();
          if(TRB-BRB>25)
          {
            if(NumbDataleft>45)
            {
              //pass1();
              if(Flag_L_1==2&&Flag_R_1==2)
              {
                shu(6);
                Type_6();
              }else if(Flag_L_1==2||Flag_R_1==2)
              {
                if(Right_Lose_Border_Number>40)
                {
                  shu(0);
                  Type_0(1);
                }else
                {
                  shu(1);
                  Type_11();
                }
                return ;
                //yibianbuxian(2);
              }else
              {
                //                //pass1();
                //                shu(1);
                //                //yibianbuxian(2);
                //                Type_0(1);
                //                //Type_0(1);
                shu(1);
                Type_11();
              }
              
              
            }else
            {
              shu(0);
              Type_0(1);
              //yibianbuxian(2);
            }
            
            return;
          }else if((int)k_of_right_line_fangcha > 1000 && (int)k_of_left_line_fangcha>1000)//||Right_Lose_Border_Number>25
          {
            if(NumbDataright-TRB>25)
            {
              shu(1);
              Type_11();
              return;
            }else
            {
              shu(0);
              Type_0(1);
            }
            //pass();
          }else if ((int)k_of_right_line_fangcha < 700 && (int)k_of_left_line_fangcha < 700)    //ֱ�߶���
          {
            if(BlackRight[TRB][1]-BlackRight[TRB+1][1]>8)    //  12  Type_12(1);
            {
              shu(10);
              //pass();
              Cross();
              return ;
              
            }else
            {
              shu(1);
              Type_11(); 
              return ;
              
            }
            
          }else if((int)k_of_right_line_fangcha > 1000 && (int)k_of_left_line_fangcha>1000)
          {
            shu(0);
            Type_0(1);
            return ;
          }else if((int)k_of_left_line_fangcha>1000&&(int)k_of_right_line_fangcha <1000)    //бʮ��    бʮ��  ��߽��ϲ��߽粻���ţ��ұ߽��°벿�����ϰ벿�߽�����        �����       2019��4��13��09:58:47
          {
            
            if(Flag_L_1==2&&Flag_R_1==2&&LRbeyond_F==1)
            {
              shu(6);
              Type_6();
              return ;
              
            }else
            {
              if((int)k_of_right_line_fangcha <800&&Flag_L_1==2)
              {
                shu(1);
                //yibianbuxian(1);
                Type_11();
              }else
              {
                //pass();
                shu(10);
                //pass();
                Cross();
                //pass();
                
              }		
            }
          }else
          {
            //pass();
            if(BlackRight[TRB][1]-BlackLeft[TRB][1]<saidaokuan-10)
            {
              shu(1);
              Type_11();
              
            }else
            {
              shu(10);
              Cross();
              //pass();
            }
            return ;
          }
        }
        
      }
      else             //BlackLeft[BLB][0] < 55   ���������Ϸ�
      {
        if(NumbDataleft<46)  //��Ч��С��46��
        {
          if(Flag_R_1 == 2 && Flag_L_1 == 1)
          {
            shu(5);
            Type_5(1); 
            return;
          }
        }
        //pass();
        if (BlackRight[TRB][0] - BlackRight[NumbDataright - 1][0] < 8)   //��Ч�����ڶ�����8����     ������ͷ���   ʮ��
        {
          //pass();
          if ((int)k_of_left_line_fangcha < 1000 || (Flag_L_1==1 && (int)k_of_left_line_fangcha < 1500))
          {
            shu(5);
            Type_5(1);
            return ;
          }else if(Flag_R_1 == 1 && Flag_L_1 == 1)
          {
            shu(5);
            Type_5(1);
            return ;
          }else if((int)k_of_right_line_fangcha < 500&&(int)k_of_left_line_fangcha < 500)
          {
            shu(1);
            Type_11(); 
            return ;
          }else if (Flag_R_1 == 1 && Flag_L_1 == 2)
          {
            //pass();
            shu(10);
            Cross();
            return ;
          }else               //�˴������� 2019��4��12��20:22:29
          {
            /*//pass();
            shu(10);
            Cross();
            return ;*/
          }//pass();
          //pass();
          
        }
        else                //��Ч�����ڶ�����10����   ʮ�����͵����������ͷ���  ֱ�߶���   2019��4��11��22:31:12
        {
          //pass();
          if((int)k_of_right_line_fangcha < 500 && (int)k_of_left_line_fangcha < 500)      //���߷��С
          {
            if(Right_Lose_Border_Number>=10&&Flag_L_1==1)
            {
              shu(5);
              Type_5(1);
              return;
            }else
            {
              shu(1);
              Type_11(); 
              return ;
            }
          }else if ((int)k_of_left_line_fangcha < 1000)
          {
            if(Flag_L_1 == 1)
            {
              shu(5);
              Type_5(1);
            }else if((int)k_of_right_line_fangcha < 1000&&NumbDataright>45)
            {
              shu(1);
              Type_11();
            }else
            {
              shu(5);
              Type_5(1);
            }
            
            
            return ;
          }else if((int)k_of_right_line_fangcha > 1000 &&(int)k_of_left_line_fangcha > 1000 )
          {
            if(Flag_R_1 == 1 && Flag_L_1 == 2||LRbeyond_F==0)
            {
              shu(10);
              Cross();
              return ;
            }else
            {
              //pass();
              shu(5);
              Type_5(1);
              return ;
            }
            
          }else if((int)k_of_right_line_fangcha < 1000&&(int)k_of_left_line_fangcha > 1000)
          {
            shu(1);
            Type_11();
            return ;
          }
        }
        
        
        
      }
    }
    else if (Left_Lose_Border_Number == 0 && Right_Lose_Border_Number == 0)
    {
      //pass();
      if ((Flag_R_1 == 1 && Flag_L_1 == 1) || (Flag_R_1 == 2 && Flag_L_1 == 2))
      {
        shu(6); 
        Type_6();
        return ;
      }
      else if (Flag_R_1 == 1 && Flag_L_1 == 2)
      {
        //pass();
        shu(10);
        Cross();
        return ;
      }
      else
      {
        //pass();
        shu(1);
        Type_11();   //////��ֱͨ��
        return ;
        
      }
      //pass();
      ////////////////2019��3��31��18:30:01////////////////////////////
    }else if(Left_Lose_Border_Number != 0 && Right_Lose_Border_Number != 0)
    {
      //pass1();
      //if(Left_Lose_Border_Number<5)
      if(BRB<3&&BLB-TRB>0&&Flag_R_1==2)
      {
        shu(5);
        Type_5(2);
        return;
      }else if(BLB<3&&BRB-TLB>0&&Flag_L_1==1)
      {
        shu(5);
        Type_5(1);
        return;
      }else if(BLB<3&&BRB-TLB>20&&BLB2==99)
      {
        //pass();
        shu(5);
        Type_5(1);
        return;
      }else if(BRB<3&&BLB-TRB>20)
      {
        shu(5);
        Type_5(2);
        return;
      }else if((int)k_of_right_line_fangcha< 1000 &&(int)k_of_left_line_fangcha < 1000 )
      {
        if(BLB<3&&BRB-TLB>0)
        {
          shu(1);
          Type_11();
        }else if(BRB<3&&BLB-TRB>0)
        {
          shu(1);
          Type_11();
        }else
        {
          shu(10);
          Cross();
          //pass();
          return ;
        }
      }
      else
      {
        shu(10);
        Cross();
        //pass();
        return ;
      }
      
    }
    
  }
  //pass();
  if(zhixingshu!=1)
  {
    //pass1();
    //if(zhixingshu==0)
    {
      //pass();
      Type_11();   //////��ֱͨ��
      return ;
    }
  }
}


/*==================================================================================
�������ƣ�init
��������: ���ֱ�����ʼ��
ʱ�䣺2019��4��13��14:03:28
������
����ֵ��
===================================================================================*/
void init()           //��ʼ��
{
  for(uchar i=0;i<60;i++)          //���ұ߽缰���߳�ʼ��
  {
    BlackLeft[i][0]=59-i;  
    BlackLeft[i][1]=2;
    BlackRight[i][0]=59-i;
    BlackRight[i][1]=157;
    Center[i][0]=59-i;
    Center[i][1]=80;
  }
  NumbDataleft=0;				//��߽����Ч��ֵ
  NumbDataright=0;			//�ұ߽����Ч��ֵ
  NumbDatacenter=0;			//���ߵ���Ч��ֵ
  
  Flag_L_1=0;		//1�� 2��
  Flag_R_1=0;		//1�� 2��
  
  k_of_left_line_fangcha=0;	//��߽緽��
  k_of_right_line_fangcha=0;	//�ұ߽緽��
  
  sousuojieshuhang=0;			//����������
  
  zhixingshu=0;				//�����жϴ���
  
  Left_Lose_Border_Number=0;	//��߽綪ʧ����
  Right_Lose_Border_Number=0;	//�ұ߽綪ʧ����	
  
  TRB=99;						//top right border
  BRB=99;						//bottom right border		
  TLB=99;						//top left border
  BLB=99;						//bottom left border
  
  
  BLB2=99;		//��߽��������Ķ��ߵ�	Below Left Border
  TLB2=99;		//��߽��������Ķ��ߵ�	Top Left Border
  BRB2=99;		//�ұ߽��������Ķ��ߵ�	Below Right Border
  TRB2=99;		//�ұ߽��������Ķ��ߵ�	Top Right Border
  
  duan_diubian_R=0;			//�ұ߽��ֿ�ʼ���ߣ���־λ
  duan_diubian_L=0;			//��߽��ֿ�ʼ���ߣ���־λ
  
  rightguai=0;   //�ҹյ�
  leftguai=0;		//��յ�
  
  Common_Lose_Flag=0;			//��ͬ���߱�־λ
  
  LRbeyond_F=0;	     //�ұ߽����߽����	����п��10 ��־λ  
  
  k_of_left_line_pingjun=0;		//��߽�б��
  
  k_of_right_line_pingjun=0;	//�ұ߽�б��
  
  left_diubiancishu=0;      //�󶪱ߴ���     init ��0
  right_diubiancishu=0;     //�Ҷ��ߴ���     init ��0
  
  saidaoleixing=99;
  zhidaopanduan_F=0;
  lianxuwandao_F=0;
  jiesujiance_F=0;                  //��ɨ�ߵ�ʱ����Ϊ�����п�С��һ��ֵ������
  NumbDatacenter_T=0;
  
  
  L_left=0,L_right=0;   //��߽��е�����
  R_left=0,R_right=0;   //�ұ߽��е�����
  C_left=0,C_right=0;   //����
  left_daihao=0,right_daihao=0;         //��߽�����������Ⱥ�
  zongtiqushi=0;        //������
}



void out_error()
{
  //  int i=0;
  //  uchar j=0;
  //  i=Encoder_set;   //Encoder_setΪ�趨�ٶ�
  
  qianzhancontrol();
  before=5;
  if(NumbDatacenter>before)
  {
    error=(int)Center[before][1]-80;
  }else{
    error=(int)Center[NumbDatacenter-1][1]-80;
  }
  
  /*
  if(NumbDatacenter>5)
  {
  error=(int)((Center[5][1]+Center[0][1])/2-80);
}else{
  error=(int)Center[NumbDatacenter-1][1]-80;
}
  */
  //  before=10;      //ǰհΪ10
  //  while(i>80)
  //  {
  //    i=i-5;       //�ٶ�����5  ��ǰհ��1
  //    j++;
  //  }
  
  //  if(Encoder_set>105)           //before  Ĭ��15
  //  {
  //    before=before+j;
  //  }
  //  if(NumbDatacenter>before)
  //  {
  //    error=(int)Center[before][1]-80;
  //  }else
  //  {
  //    error=(int)Center[NumbDatacenter-1][1];
  //  }
  
  //----------------------------------------------------
  //  if(NumbDatacenter>before+5)
  //  {
  //    
  //    error =(int)((0.2*(Center[before-5][1]-80))+(0.5*(Center[before][1]-80))+(0.3*( Center[before+5][1]-80)));
  //
  //  }
  //  else
  //  {
  //    before=NumbDatacenter-1;
  //    error =(int)((0.3*(Center[before*4/5][1]-80))+(0.5*(Center[before][1]-80))+(0.2*( Center[before*3/5][1]-80)));
  //  
  //  }
}


void qianzhancontrol()
{
  //��59�е������п�Ϊ118�������  б�� k=35/58    ��б��k������ͼ���㷨�ж�̬���
  float ko=58/33;    //б��k:������������ұ������в����в�ı�ֵ  б��k��һ���ڳ�Ԥ���ߵ�·�̶�Ӧ��ǰհ�й�ϵ
  double v;     //�ٶ�
  double s;     //·��
  v=sv_R->V;            //10ms�ۼƵ�������    �˳� 6225������/��     v�ĵ�λΪ:m/ms
  v=v*100;    //��v�ĵ�λ����� cm/ms
  s=v*90;//��ʱ���׳���ִ������Ϊ T=90ms ��һ���������ڽ��� s=v*90 ��cm��  s ��λΪcm
  //  s=s*100;      //��m �����cm
  
  before=(int)((s-9.5)*ko);    //s=v*T 00        s=before/k+9.5    before:ǰհ
  //���ڴ��� ��ʵ������ ����ͷ�ɼ�����ͼ����Ϣ���� ������
  //Ԥ������ͼ�λ��䣬����������Զ�룬��Ӧ���п��Ӧ��ʵ�ʾ���ı�ֵ�������ӣ���ֵ���ȷ�� 1���������� �Ƿ��б���й�ϵ  2��
  
  
  if(before<10)
  {
    before=10;     //��ֹ�������
  }
  if(before>55)
  {
    before=55;   //��ֹ�������
  }

}

/*==================================================================================
�������ƣ�SpeedControl
��������: ͨ���ж����������Լ����߼�������������ڲ�ͬ���ٶ�   ͨ������
ʱ�䣺2019��6��21��21:58:40
������
����ֵ��  Expectation_speed
===================================================================================*/

void SpeedControl()
{
  
  //int Expectation_speed=0;
  int i,j;
  int sum=0;
  float averge=0;  //ƽ��������
  float zhongxianfangcha=0;			//���߷���   ������߷���С��15  =_= ������
  
  for(i=0;i<NumbDatacenter;i++)			//�������㵽�˱߽�֮�� �������������Ч������ĸ�ȥ��  
  {
    if(Center[i][1]>=157)				//�������һ�����⣬���ǻ᲻����һ�������ǰհ���涪�ߵ�ǰհ������,Ԥ��������������ĸ��ʺܵͺܵ�
    {
      //pass();
      NumbDatacenter=i+1;
      break;
    }
    if(Center[i][1]<=2)
    {
      //pass();
      NumbDatacenter=i+1;
      break;
    }
  }
  
  
  for(i=0;i<NumbDatacenter;i++)
  {
    sum+=abs(Center[i][1]-80); 
  }
  averge=sum/(NumbDatacenter-1);
  for(i=0;i<NumbDatacenter;i++)
  {
    zhongxianfangcha+=pow(Center[i][1]-80,2);
  }
  zhongxianfangcha=(zhongxianfangcha/(NumbDatacenter-1));
  //if(fangcha>100)pass();
  if(zhongxianfangcha>159)zhongxianfangcha=159;
  //ImageData[10][(int)fangcha]=BLUE;
  if(zhongxianfangcha<=15)
  {
    changzhidao_F=1;  //��ֱ��
    //Expectation_speed=130;       //�����ٶȸ�ֵ
  }
  //if(fangcha)
}

/*==================================================================================
�������ƣ�suo
��������: �����ӳ���֮�󣬰�״̬��ס ������� ֱ���ص�����������
ʱ�䣺2019��6��22��02:37:10
������  char suo=0;  
����ֵ��
===================================================================================*/
void suo()
{
  //============================��Բת����===================================//   ����   ��������
  if(BlackRight[0][1]-BlackLeft[0][1]<=70&&BlackRight[0][1]==157&&Right_Lose_Border_Number>5&&error>=77)
  {
    suo_F=1;   //��ס�ұ�
  }
  if(BlackRight[0][1]-BlackLeft[0][1]<=70&&BlackLeft[0][1]==2&&Left_Lose_Border_Number>5&&error<=-77)
  {
    suo_F=2;   //��ס�ұ�
  }
  if(BlackRight[0][1]-BlackLeft[0][1]>=100)  //�ص�����������
  {
    suo_F=0;
  }
  if(suo_F==1)
  {
    error=80;  //������Ҵ���
    
  }else if(suo_F==2)
  {
    error=-80;  //�������
  }
  //============================��Բת����END================================
}


/*==================================================================================
�������ƣ�shujucunchu
��������: ��������10֡��߽��ұ߽����ߵ�ֵ
ʱ�䣺2019��6��28��12��31��
������  xianpleft    xianpright  xianpcenter   ͷ�ڵ�β�ڵ�
����ֵ��
===================================================================================*/


//uchar leftborder[10][2][65];//�洢10����߽�����
//uchar centerborder[10][2][65];//�洢10����������
//uchar rightborder[10][2][65];//�洢10���ұ߽�����
void shujucunchu()
{
  Xian * a=NULL;
  //-----------------��߽籣��-----------------------------------------------
  if(xianpleft->front==NULL)            //ͷָ��Ϊ��
  {
    xianpleft->front=(Xian *)malloc(sizeof(Xian));      //����ռ��ַ
    xianpleft->rear=(Xian *)malloc(sizeof(Xian));      //����ռ��ַ
    a=xianpleft->rear;
    for(int i=0;i<NumbDataleft;i++)                     //��߽縳ֵ
    {
      a->border[i][0]=BlackLeft[i][0];
      a->border[i][1]=BlackLeft[i][1];
    }
    xianpleft->number=1;
  }else
  {
    
    if(a->number<10)           //����10֡����
    {
      a=xianpleft->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpleft->rear=a;                //βָ�����
      for(int i=0;i<NumbDataleft;i++)                     //��߽縳ֵ
      {
        a->border[i][0]=BlackLeft[i][0];
        a->border[i][1]=BlackLeft[i][1];
      }
      xianpleft->number++;
    }else                       //tou
    {
      a=xianpleft->front;
      xianpleft->front=a->next;
      free(a);   //�ͷ��ڴ�
      
      a=xianpleft->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpleft->rear=a;                //βָ�����
      for(int i=0;i<NumbDataleft;i++)                     //��߽縳ֵ
      {
        a->border[i][0]=BlackRight[i][0];
        a->border[i][1]=BlackRight[i][1];
      }
      xianpleft->number=10;
    }
  }
  //-----------------�ұ߽籣��-----------------------------------------------
  if(xianpright->front==NULL)            //ͷָ��Ϊ��
  {
    xianpright->front=(Xian *)malloc(sizeof(Xian));      //����ռ��ַ
    xianpright->rear=(Xian *)malloc(sizeof(Xian));      //����ռ��ַ
    a=xianpright->rear;
    for(int i=0;i<NumbDataright;i++)                     //��߽縳ֵ
    {
      a->border[i][0]=BlackRight[i][0];
      a->border[i][1]=BlackRight[i][1];
    }
    xianpright->number=1;
  }else
  {
    
    if(a->number<10)           //����10֡����
    {
      a=xianpright->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpright->rear=a;                //βָ�����
      for(int i=0;i<NumbDataleft;i++)                     //��߽縳ֵ
      {
        a->border[i][0]=BlackRight[i][0];
        a->border[i][1]=BlackRight[i][1];
      }
      xianpright->number++;
    }else                       //tou
    {
      a=xianpright->front;
      xianpright->front=a->next;
      free(a);   //�ͷ��ڴ�
      
      a=xianpright->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpright->rear=a;                //βָ�����
      for(int i=0;i<NumbDataright;i++)                     //��߽縳ֵ
      {
        a->border[i][0]=BlackRight[i][0];
        a->border[i][1]=BlackRight[i][1];
      }
      xianpright->number=10;
    }
  }
  //---------------------���߱���-----------------------------------------------
  
  if(xianpright->front==NULL)            //ͷָ��Ϊ��
  {
    xianpcenter->front=(Xian *)malloc(sizeof(Xian));      //����ռ��ַ
    xianpcenter->rear=(Xian *)malloc(sizeof(Xian));      //����ռ��ַ
    a=xianpcenter->rear;
    for(int i=0;i<NumbDatacenter;i++)                     //���߸�ֵ
    {
      a->border[i][0]=Center[i][0];
      a->border[i][1]=Center[i][1];
    }
    xianpcenter->number=1;
  }else
  {
    if(a->number<10)           //����10֡����
    {
      a=xianpcenter->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpcenter->rear=a;                //βָ�����
      for(int i=0;i<NumbDatacenter;i++)                     //���߸�ֵ
      {
        a->border[i][0]=Center[i][0];
        a->border[i][1]=Center[i][1];
      }
      xianpcenter->number++;
    }else                       //tou
    {
      a=xianpcenter->front;
      xianpcenter->front=a->next;
      free(a);   //�ͷ��ڴ�
      
      a=xianpcenter->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpcenter->rear=a;                //βָ�����
      for(int i=0;i<NumbDatacenter;i++)                     //���߸�ֵ
      {
        a->border[i][0]=Center[i][0];
        a->border[i][1]=Center[i][1];
      }
      xianpcenter->number=10;
    }
  }
}



/*==================================================================================
�������ƣ�beep_f
��������: ���Ʒ�����
ʱ�䣺2019��6��29��16��34��
������  f
����ֵ��
===================================================================================*/
void beep_f()
{
  if(beep_F==1)
  {
    IsBeep=1;           //��������
    
  }else if(beep_F==0)
  {
    IsBeep=0;           //�ط�����
  }
}


/*==================================================================================
�������ƣ�beep_jizhi
��������: ������������� ��һ��
ʱ�䣺2019��6��29��16��34��
������  f  fengmingqi_daiyong
����ֵ��
===================================================================================*/

void beep_jizhi(char f)
{
  
  if(f==1&&beep_F==0&&fengmingqi_daiyong==1)    //ֻ��һ��      ����������δ����״̬ ���� ������
  {
    fengmingqi_daiyong=2;
    Image_num=0;
    
    beep_F=1;
    fengmingqi_f=1;
    
  }
  if(Image_num>25&&fengmingqi_f==1)           //��0.2s
  {
    beep_F=0;                 //�������ر�
    fengmingqi_daiyong=3;
    fengmingqi_f=0;
    Image_num=0;
  }
}


/*==================================================================================
�������ƣ�sanjiaojiance
��������:��·�����Ǽ��
ʱ��:
������
����ֵ��
===================================================================================*/
void sanjiaojiance()
{
  int i=0,num=0;
  int w1=0,w2=0,w3=0;
  if(NumbDataleft<=45&&Left_Lose_Border_Number<20&&Left_Lose_Border_Number<20)           //�߽���С��45
  {
    for(i=NumbDataleft-1;i>2;i--)
    {
      w1=BlackRight[i][1]-BlackLeft[i][1];
      w2=BlackRight[i-1][1]-BlackLeft[i-1][1];
      w3=BlackRight[i-2][1]-BlackLeft[i-2][1];
      if(w1<w2&&w2<w3)
      {
        num++;
      }
      if(num>4)
      {
        
        break;
      }
    }
  }
  
}



/*==================================================================================
�������ƣ�duanlujiance
��������: ��·���  ��⵽��·��֮���л��ɵ����
ʱ��:
������  duanlu_F
����ֵ��
===================================================================================*/
void duanlujiance()
{
  int left_num=0;
  if(BLB==0&&TLB!=99&&TLB>10&&NumbDataleft-TLB>10&&NumbDataleft<=50&&Right_Lose_Border_Number==0)
  {
    //pass();
    for(int i=TLB;i<NumbDataleft-2;i++)
    {
      if(BlackLeft[i][1]-BlackLeft[i+2][1]<0)
      {
        left_num++;
      }
    }
    if(left_num>NumbDataleft-TLB-4)
    {
      duanlu_F=1;
    }
  }
  if(Right_Lose_Border_Number==0&&Left_Lose_Border_Number==0&&NumbDataleft<=50)
  {
    duanlu_F=1;
  }
  if(duanlu_F==1&&Laser_Value>1200)
  {
    // fengmingqi_daiyong=1;          //������   
    duanlu_F=1;
  }else
  {
    duanlu_F=0;
  }
  
  if(duanlu_F==1)
  {
    Expectation_speed=-50;
    fengmingqi_daiyong=1;
  }
 
}



/*==================================================================================
�������ƣ�left_border_zuiyou
��������: �ҵ��ұ߽�����ߵĵ���±�
ʱ��:
������
����ֵ��    j  ��߽�������е������±�
===================================================================================*/
int left_border_zuiyou()
{
  int j=0,val=159;
  for(int i=0;i<NumbDataright;i++)
  {
    if(BlackRight[i][1]<val)     //����һ��  �ұ߽�������еĴ�С
    {
      val=BlackRight[i][1];
      j=i;
    }
  }
  return j;
}


/*==================================================================================
�������ƣ�center_L_R
��������: Ѱ����������ұߵĵ�
ʱ��:
������
����ֵ��   
===================================================================================*/
void center_L_R(int *l,int *r)
{
  int val1=159,val2=0;
  int j1=0,j2=0;
  for(int i=0;i<NumbDatacenter;i++)
  {
    if(Center[i][1]<val1)     //����һ��  ������еĴ�С
    {
      val1=BlackRight[i][1];
      j1=i;
    }
    if(Center[i][1]>val2)       //�����ҵĵ�
    {
      val2=BlackRight[i][1];
      j2=i;
    }
  }
  *l=j1;
  *r=j2;
}





/*==================================================================================
�������ƣ�speed_control
��������: ������������̶�һ�е�������ֵ�����ƴ�ʱ���ٶȣ���Ϊ�����ٶȣ�����������Ϊ150��ֱ����   130����ֱ��--��Բ�������䣩    115��СԲ����С�䣩
������Ч�ı߽��������Լ���10�е���߽����9����߽���в�ֵ���ж�����������Ӧ�ø�����
ʱ��:
������
����ֵ��
===================================================================================*/
int speed_dingshi=0;    //�ٶȶ�ʱ
int speed_dingshi_F=0;    //�ٶȶ�ʱ
int zhuan=0;
void speed_control()
{

  if(lianxuwandao_F==3&&Expectation_speed>=-5)         //���Сs��
  {
    speed_dingshi=0;
    Expectation_speed=-5;
    speed_dingshi_F=1;
    //fengmingqi_daiyong=1;
  }
  

  
  if(lianxuwandao_F==4&&Expectation_speed>=-10)
  {
    speed_dingshi=0;
    Expectation_speed=-10;
    speed_dingshi_F=1;
    //fengmingqi_daiyong=1;
  }
  if(left_daihao==0&&right_daihao==0)
  {
    speed_dingshi=0;
    Expectation_speed=10;
    speed_dingshi_F=1;
  }
  
  if(speed_dingshi>25&&speed_dingshi_F==1&&Expectation_speed>=10)
  {
    Expectation_speed=10;
    speed_dingshi_F=0;
  }
  

  
  if(zhidaopanduan_F==1)
  {
    Expectation_speed=35;
  }
  
  
  //ĥ��̥רҵ��---------------------------------------------------------
  //  if(Image_num1>1000)           //10s
  //  {
  //    zhuan++;
  //    Image_num1=0;
  //    if(Expectation_speed<50)    //����������  +50
  //    {
  //      Expectation_speed=Expectation_speed+10;
  //    }
  //  }
  //  
  //  if(zhuan>18)          //3���Ӻ�ͣ��
  //  {
  //    protect_flag=1;
  //  }
  //ĥ��̥רҵ��---------------------------------------------------------

}


/*==================================================================================
�������ƣ�lianxuwandao
��������: �ж����ߵ�����
ʱ�䣺2019��7��5��00��54��
������
����ֵ��   3 ��ʾ��������������������ҵ����� ��һ��s��  2��ʾֻ�����������  1��ʾֻ�����ҵ����� 
===================================================================================*/
int lianxuwandao()
{
  int i=0;
  char left=0,right=0;
  for(i=0;i<NumbDatacenter-6;i+=2)
  {
    if(Center[i][1]>Center[i+2][1]&&Center[i+1][1]>Center[i+3][1]&&Center[i+2][1]>Center[i+4][1]&&Center[i+3][1]>Center[i+5][1]&&Center[i+4][1]>Center[i+6][1])	//NAODAIZHUANBUDONGLEA CHUNDEYIPI
    {
      left=1;	//
    }
    if(Center[i][1]<Center[i+2][1]&&Center[i+1][1]<Center[i+3][1]&&Center[i+2][1]<Center[i+4][1]&&Center[i+3][1]<Center[i+5][1]&&Center[i+4][1]<Center[i+6][1])	//NAODAIZHUANBUDONGLEA CHUNDEYIPI
    {
      right=1;
    }
  }
  
  if((TRB-BRB>45&&NumbDataright-TRB>4)||(TLB-BLB>45&&NumbDataleft-TLB>4)||(left==right==1&&TRB-BRB>45))           //С s�䶪���ر�����  �ٶȸ��Ӽ�С
  {
    return 4;
  }
  
  
  if(left==right==1)
  {
    return 3;
  }
  if(left==1)
  {
    return 2;
  }
  if(right==1)
  {
    return 1;
  }
  return 0;
}


/*==================================================================================
�������ƣ�sanxianqushipanduan
��������: �ж��������������
ʱ�䣺
������
����ֵ��   3 ��ʾ��������������������ҵ����� ��һ��s��  2��ʾֻ�����������  1��ʾֻ�����ҵ����� 
===================================================================================*/
//  L_left=0,L_right=0;   //��߽��е�����
//  R_left=0,R_right=0;   //�ұ߽��е�����
//  C_left=0,C_right=0;   //����
//  left_daihao=0,right_daihao=0;         //��߽�����������Ⱥ�
//  zongtiqushi=0;        //������

int sanxianqushipanduan()
{
  int i=0;
  
  int youxian_L=0,youxian_R=0;
  for(i=0;i<NumbDataleft-6;i+=2)
  {
    if(BlackLeft[i][1]>BlackLeft[i+2][1]&&BlackLeft[i+1][1]>BlackLeft[i+3][1]&&BlackLeft[i+2][1]>BlackLeft[i+4][1]&&BlackLeft[i+3][1]>BlackLeft[i+5][1]&&BlackLeft[i+4][1]>BlackLeft[i+6][1])	//NAODAIZHUANBUDONGLEA CHUNDEYIPI
    {
      
      if(youxian_L==0)   //��˭���������ȳ���
      {
        youxian_L=1;
      }
      L_right=1;	//
    }
    if(BlackLeft[i][1]<BlackLeft[i+2][1]&&BlackLeft[i+1][1]<BlackLeft[i+3][1]&&BlackLeft[i+2][1]<BlackLeft[i+4][1]&&BlackLeft[i+3][1]<BlackLeft[i+5][1]&&Center[i+4][1]<BlackLeft[i+6][1])	//NAODAIZHUANBUDONGLEA CHUNDEYIPI
    {
      if(youxian_L==0)
      {
        youxian_L=2;
      }
      L_left=1;
    }
    if(BlackRight[i][1]>BlackRight[i+2][1]&&BlackRight[i+1][1]>BlackRight[i+3][1]&&BlackRight[i+2][1]>BlackRight[i+4][1]&&BlackRight[i+3][1]>BlackRight[i+5][1]&&BlackRight[i+4][1]>BlackRight[i+6][1])	//NAODAIZHUANBUDONGLEA CHUNDEYIPI
    {
      if(youxian_R==0)
      {
        youxian_R=1;
      }
      R_right=1;	//
    }
    if(BlackRight[i][1]<BlackRight[i+2][1]&&BlackRight[i+1][1]<BlackRight[i+3][1]&&BlackRight[i+2][1]<BlackRight[i+4][1]&&BlackRight[i+3][1]<BlackRight[i+5][1]&&BlackRight[i+4][1]<BlackRight[i+6][1])	//NAODAIZHUANBUDONGLEA CHUNDEYIPI
    {
      if(youxian_R==0)
      {
        youxian_R=2;
      }
      R_left=1;
    }
  }
  
  
  if(L_right==1&&L_left==0&&R_right==1&&R_left==0)
  {
    zongtiqushi=1;							//������������
  }
  if(L_right==0&&L_left==1&&R_right==0&&R_left==1)
  {
    zongtiqushi=2;							//������������
  }
  if(L_right==1&&L_left==1)
  {
    if(youxian_L==1)							//��߽������������Ҳ�����ҵ�����
    {
      left_daihao=1;						//�����ң�������
    }else
    {
      left_daihao=2;					//������������
    }
  }
  
  if(R_right==1&&R_left==1)
  {
    if(youxian_R==1)							//�ұ߽������������Ҳ�����ҵ�����
    {
      right_daihao=1;					//�����ң�������
    }else
    {
      right_daihao=2;					//������������
    }
  }
  return 0;
}

/*==================================================================================
�������ƣ�zhidaopanduan
��������: ֱ���ж�
ʱ�䣺2019��7��6��00��07��
������
����ֵ�� 1  �ǳ�ֱ��   0���ǳ�ֱ��
===================================================================================*/
int zhidaopanduan()
{
  int i=0;
  if(NumbDatacenter<52)return 0;
  for(i=0;i<NumbDatacenter;i+=2)
  {
    if(abs(Center[i][1]-80)>10)
    {
      return 0;
    }
  }
  return 1;     //�ǳ�ֱ��
}


/*==================================================================================
�������ƣ�zhongxianqueren
��������: �����жϣ��ڶ�·ʱ�����������һ�߲�������������߽��жԱ�
ʱ�䣺2019��7��7��17��24��
������
����ֵ�� 
===================================================================================*/
void zhongxianqueren()
{
  int flag=0;
  if(Left_Lose_Border_Number==0)
  {
    flag=2;
  }else if(Right_Lose_Border_Number=0)
  {
    flag=1;
  }
  if(flag==1)
  {
    float kk=right_k-47/75;
    //right_k=right_k+47/75;
    //right_k=47/75;
    //pass();
    uchar i=0;
    for(i=0;i<NumbDataright;i++)
    {
      if(BlackRight[i][1]!=157)//-(i/right_k)
      {
        if(BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k))>0)
          //if(BlackRight[i][1]-(int)(saidaokuanyiban+(BlackRight[0][1]-BlackRight[i][1]))>0)//+(i/right_k)  +(BlackRight[0][1]-BlackRight[i][1])
        {
          Center_T[i][1]=BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k));  //����ֱ�ߵ���ͨ������ -(i/right_k) +(i/right_k)
        }else 
        {
          Center_T[i][1]=0;
        }
      }else
      {
        Center_T[i][1]=(uchar)(BlackRight[i][1]-saidaokuanyiban+(i/right_k));
      }
      
      
      Center_T[i][0]=BlackRight[i][0];
    }
    NumbDatacenter_T=i;
    
  }else if(flag==2)
  {
    //pass();
    uchar i=0;
    for(i=0;i<NumbDataright;i++)
    {
      if(BlackLeft[i][1]!=2)
      {
        if(BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k)<159)//+(BlackLeft[i][1]-BlackLeft[0][1])
        {
          Center_T[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k);  //+(i/left_k) +(BlackLeft[i][1]-BlackLeft[0][1])
          //Center[i][1]=BlackLeft[i][1]+(int)(saidaokuanyiban+i/left_k+(BlackLeft[i][1]-(BlackLeft[0][1]+(i/left_k))); //(BlackLeft[i][1]-(BlackLeft[0][1]-(i/left_k))
        }else
        {
          Center_T[i][1]=159;
        }
        
        
      }else
      {
        Center_T[i][1]=(uchar)(BlackLeft[i][1]+saidaokuanyiban+i/left_k);
      }
      Center_T[i][0]=BlackLeft[i][0];
    }
    NumbDatacenter_T=i;
  }
}



/*==================================================================================
�������ƣ�zhongxianqueren
��������: �����жϣ��ڶ�·ʱ�����������һ�߲�������������߽��жԱ�
ʱ�䣺2019��7��7��17��24��
������
����ֵ�� 
===================================================================================*/
int duanluzhengchangsaidaojince()
{
  int lc1=0,rc1=0;
  int lc2=0,rc2=0;
  
  
  ///������ĵļ��ص��泪�����ĳ���
  
  
  
  ///
  //one_Border_Scan(uchar r,uchar *lc,uchar *rc);               //��֤������ֵ��
//  one_Border_Scan(59,&lc1,&rc1);
//  one_Border_Scan(59,&lc2,&rc2);
  if(rc1-lc1>100&&abs(rc1-rc2<3)&&abs(lc1-lc2<3))
  {
    return 1;
  }
  return 0;
}

uchar diaoyong_flag2=0;
uchar error_beg=0;    //�������¿�ʼ����ƫ���־λ
int left_border_F=0;    //�ұ߽������� ���±�

void vision_algorithm()
{
  
  init(); 
  
  Three_Border_Scan();
  
  BorderScan();
  
  
  
  xielvfangcha();
  
  
  if(bizhang_F==0&&duanlu_F==0)   
  {
    banmaxian();        //������
    
    Type_Judgement();   //����ʶ��
    
    sanxianqushipanduan();      //�����ж�
    
    lianxuwandao_F=lianxuwandao();      //���������־λ
    zhidaopanduan_F=zhidaopanduan();    //ֱ����־λ
    //int center_L=0,center_R=0;
    center_L_R(&center_L,&center_R);     //�洢������������е������±�
    
    //shujucunchu();      //��������     �����ڴ�ռ���BUG  ������оƬ�й�Ҳ�����ǳ���BUG   1��ָ��ΪNULLʱ�������ַ��Ϊ��
    
    protect();  //���籣��
    
    if(IsDown_flag==1)   //�������¿�ʼ����ƫ��
    {
      error_beg=1;
    }
    
    if(error_beg)
    {
      imgbefor();         //��һ֡��������λ��   ������center35lie
      
      //p_rintf();
      
      out_error();
      
      suo();
      
      credible();
      
      SpeedControl();
      
      //speed_control();          //�ٶȿ���
      
      bizhangjiance();          //�ϰ�����
      
     //duanlujiance();
      
      
    }
  }else if(bizhang_F==1)              //ִ�б���
  {
    bizhang();
  }else if(duanlu_F==1)
  {
    if(duanluzhengchangsaidaojince()==1)
    {
      duanlu_F=0;
    }
  }
  
  beep_jizhi(1);
  beep_f();
  image_finish++;
  if(image_finish>9999)
  {
    image_finish=0;
    image_finish_F_Servo=0;
  }
  
  
}


//2019��5��5��21:01:12