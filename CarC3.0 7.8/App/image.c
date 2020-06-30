//版本1.2        2019年5月12日09:48:59

#include "image.h"
#include "include.h"
#include "common.h"
#include "math.h"
#include "VAR.h"
#include "GUI.h"
#include "elec.h"
#include "Init.h"
#include "stdio.h"

/*--------可用外部接口------------------------

extern int Image_num;                   //图像计数（100ms）

extern uint8 Image_num;                 //    image_num_kaiqi  为 1 开始  Image_num 的计数

extern unsigned char Image_lcd_num;     //传入一个数交给显示屏显示

--------可用外部接口END---------------------*/

//     127   58/48

//=============================全局变量区==================================//
//unsigned char ImageData[IMG_ROWS][IMG_COLS];//图像数据


/*
#define WHITE 255        //宏定义白

#define RED 128        //宏定义红     边界

#define GREEN 100        //宏定义绿     中线

#define BLACK 0          //宏定义黑

#define BLUE 254         //宏定义蓝色
#define GRAY 128          //灰色*/


#define THRESHOLD 100    //宏定义阈值

#define ScanColNumble  10   //向左右扫描的列数范围

//#define CrossFlag1   20    //此宏定义为改变左右边界有多少有效数值，启用十字函数

#define uchar unsigned char 

//作为识十字识别，后续改掉
unsigned char shizhizuidahangkuan=20;           //十字的最大行宽，  47

unsigned char shiziscanfbase=35;				//十字预测中线所在列后，以shiziscanfbase基点做为开始扫描行

int ErrorCha=10;	//偏差的偏差

uchar crossturnnum=15;      //十字的行宽   abs(TRB-TLB)>crossturnnum&&TLB!=99&&TRB!=99)           //crossturnnum为TRB-TLB  多少为十字

unsigned char huandaoF=0;			//环岛标志位，此标志位不初始化，用完手动清零

unsigned char huandaozhuangtai_flag=0;     //环岛进行的状态标志位，此标志位不初始化，用完手动清零

unsigned char before_error_Flag=0;			//下位机使用要采用上一帧的error做为此帧来使用，此标志位不初始化，用完手动清零

uchar liekuan35=66;         //35行的列宽 用于十字确定左右边界的时候使用



uchar center35lie;     //上位机值  35行中线的列数

float left_k=-1.208;       //摄像头检测出来的左边界的 畸变斜率

float right_k=1.208;      //摄像头检测出来的右边界的 畸变斜率  49/45

uchar saidaokuan=126;   //下位机根据摄像头采集的赛道宽度，赛道宽度。  126   c车 2019年6月11日15:03:11
uchar saidaozuidaliekuan=126;

uchar saidaokuanyiban=63;//赛道宽一般

uchar banmaxian_F=0;   //斑马线检测出来的标志位

uchar bizhang_F=0;      //避障标志位

uchar kuan=0;   //gui接口用于显示第一行 的列宽

char Cross_F=0;   //十字的标志位

char suo_F=0;     //方向锁

char saidaoleixing=99; //赛道类型  赛道类型 有 0：半圆转弯  10：十字  11：普通直线  5：赛道尽头转弯  6：普通转弯  注：这种仅仅是一种中线计算方式，而不是中线情况



//=========================================================================//

#define rongcuo_1           10				//容错值



//int before=before_w;        //before     before_w=35
int before=0;        //before     before_w=35
int8 error=0;
int protect_flag=0;
int Started_check_Zebra=0;      //斑马线标志



int Expectation_speed=0;        //所求出的期望速度

//===========================定义变量================================================
unsigned char BlackRight[100][2]={0};  //存储右边界的二维坐标
unsigned char BlackLeft[100][2]={0};   //存储左边界的二维坐标
unsigned char Center[100][2]={0};     //存储中线的二维坐标
unsigned char Center_T[100][2]={0};     //存储中线的二维坐标
unsigned char NumbDataleft=0;			//左边界的有效数值
unsigned char NumbDataright=0;			//右边界的有效数值
unsigned char NumbDatacenter=0;			//中线的有效数值
unsigned char NumbDatacenter_T=0;			//T中线的有效数值

unsigned char BLB=99;		//左边界的最下面的丢边点	Below Left Border
unsigned char TLB=99;		//左边界的最上面的丢边点	Top Left Border
unsigned char BRB=99;		//右边界的最下面的丢边点	Below Right Border
unsigned char TRB=99;		//右边界的最上面的丢边点	Top Right Border


unsigned char BLB2=99;		//左边界的最下面的丢边点	Below Left Border
unsigned char TLB2=99;		//左边界的最上面的丢边点	Top Left Border
unsigned char BRB2=99;		//右边界的最下面的丢边点	Below Right Border
unsigned char TRB2=99;		//右边界的最上面的丢边点	Top Right Border

char duan_diubian_R=0;			//右边界又开始丢边，标志位
char duan_diubian_L=0;			//左边界又开始丢边，标志位

float k_of_left_line[100];	//左边界的斜率
float k_of_right_line[100];	//右边界的斜率

uchar rightguai=0;   	//右拐点
uchar leftguai=0;		//左拐点

//#大类识别
unsigned char Left_Lose_Border_Number;	//左边界丢边行数
unsigned char Right_Lose_Border_Number;	//右边界丢边行数
float k_of_left_line_fangcha=0;	//左边界方差
float k_of_right_line_fangcha=0;//右边界方差
uchar FduanL=0,FduanR=0;//前面3行算基点，共同丢边的标志位：0->没有;1->有
//#十字识别
unsigned char Common_Lose_Flag;		//共同行丢边的标志位

//#赛道类型识别
unsigned char Flag_L_1=0,Flag_R_1=0;//左右边界方向趋势标志位 1右 2左



unsigned char sousuojieshuhang=0;//搜索结束行

uchar zhixingshu=0;				//执行数//#可作为出界保护，待测试2019-4-15

int dataerror[4]={80};  	    //储存此幅，上一幅，上上幅，上上上幅 的error



uchar LRbeyond_F=0;					//拐点相差较大 LRbeyond_F=1； 判断十字或者转弯出现

uchar Common_Lose_Flag_effective=0;   //共同丢边时是否还继续执行边界扫描     0不继续扫描  1继续扫描  默认为0 

uchar yuanhuancrediblehang=0;



uchar saidao_F=0;


float k_of_left_line_pingjun=0;		//左边界斜率

float k_of_right_line_pingjun=0;	//右边界斜率

//-----斑马线要用到的标志位-------
uchar chubanmaxian=0;
uchar chubanmaxianjishu=0;
//-----斑马线要用到的标志位--END-----

unsigned char image_flag=0;            //gui  图像程序测试标志位
unsigned char image_flag_1=0;            //gui  图像程序测试标志位

unsigned char left_diubiancishu=0;      //左丢边次数     init 置0
unsigned char right_diubiancishu=0;     //右丢边次数     init 置0

uchar daiyong_flag=0;                   //      待用标志位   做为执行次数的用

char changzhidao_F=0;   //长直道标志位
char beep_F=0;          //蜂鸣器标志位

char fengmingqi_daiyong=0;      //置1就响一次
char fengmingqi_f=0;
  int lianxuwandao_F=0;
  int zhidaopanduan_F=0;
  
  int center_L=0,center_R=0;            //储存中线的最左最右列的下标
  int jiesujiance_F=0;                  //再扫边的时后因为赛道列宽小于一定值而跳出
  char L_left=0,L_right=0;
char R_left=0,R_right=0;
char C_left=0,C_right=0;
int left_daihao=0,right_daihao=0; 
int zongtiqushi=0;
//=======================定义变量结束================================================

//********************************函数声明****************************************************

//具体函数功能请看函数注释
void pass();
void pass1();
int shu(uchar a);
void Type_0(uchar a);
void Type_0la(char a);   //不跟据斜率来计算而是靠拉一条线
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
void credible();          //error 的可信判断与选取
void protect();

void guaidiancredible();				//根据拐点出现的行数相差判断拐点是否可信

void CrossTypebuxian();

void one_Border_Scan(uchar r,uchar *lc,uchar *rc);

void diubiankexingpanduan();

void imgbefor();

int diubiancredible();

void errorpinghua();

void banmaxian(); //检测斑马线

void huandao();

void huandao_one(); //环岛

void huandao_two(); //环岛



void zuidaliekuan();

void yibianbuxian(uchar flag);  //以一边为边界补线

void fengmingqi1(uchar f);      //响不同 的声音  

void bizhangjiance();                 //避障


void qianzhancontrol();

void shexiangtoutiaojie();      //检测 摄像头是否调正 俯仰角是否正确

void bizhang();

int shizixiediubian();          //斜十字丢边

void Type_5_2(uchar a);         //计算第二条丢边转弯的方法

void suo();                     //方向锁

void SpeedControl();            //通过赛道类型调节速度

void shujucunchu();             //存储 左边界右边界中线数据

void zhidaobuxian(uchar flag);  //直道用一边的补线方式

void beep_f();                  //控制蜂鸣器

void beep_jizhi(char f);        //蜂鸣器响动控制

int left_border_zuiyou();       //返回左边界的最左边数组下标

void center_L_R(int *l,int *r);//返回中线最左边的和最右边的数组下标

int lianxuwandao();             //返回中线的趋势

int zhidaopanduan();            //直道判断

int sanxianqushipanduan();      //趋势判断

void duanlujiance();            //断路检测

int duanluzhengchangsaidaojince();
//*******************************函数声明结束*************************************************

/*----------------------------------------------------------函数编写开始--------------------------------------------------------*/ 

/*==================================================================================
函数名称：zhidaobuxian
函数功能: 
时间：2019年5月16日20:16:34
参数： flag  1 依据右边补线    2 依据左边补线
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
          Center[i][1]=BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k));  //根据直线的普通畸变率 -(i/right_k)
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
函数名称：Type_0la
函数功能: 根据丢边的中点与下面的中线一点连起来做一条直线，在环岛的时候用到
时间：2019年6月27日12点12分
参数：
返回值：
===================================================================================*/
void Type_0la(char a)
{
  uchar x1=0,y1=0;
  
  NumbDatacenter=0;
  uchar i=0;
  zhixingshu++;
  saidaoleixing=0;//赛道类型为半圆转弯
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
函数名称：bizhangjiance
函数功能: 检测障碍物
时间：2019年6月6日17:02:03
参数：bizhang_F 避障标志位
返回值：
===================================================================================*/
char duanlu_F=0;
void bizhangjiance()                 //避障
{
  char saidaozhongduan=0;
  
  if(k_of_right_line_fangcha<800&&k_of_left_line_fangcha<800)       //无赛道跳变，方差较小&&Center[center_R][]center_R
  {
    //Image_lcd_num=1;
    if(Right_Lose_Border_Number==0&&Left_Lose_Border_Number==0&&Common_Lose_Flag==0)       //无丢边
    {
      //Image_lcd_num=2;
      if(NumbDataleft<=56&&NumbDataleft>=40)         //边界数小于56个
      {
        //Image_lcd_num=3;
        
        int sum[4];
        uchar i=0;
        uchar j=0;
        if(abs(Center[NumbDatacenter-2][1]-80)<10)
        {
         
          saidaozhongduan=1;                    //立马减速
          Expectation_speed=-20;
        }
      }
    }
    
  }
  //saidaozhongduan==1
  if(Laser_Value<1000&&Laser_Value>400&&huandaoF==0&&saidaozhongduan==1)   //当距离小于80cm时且为长直道  避障   不能在环岛中
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


void bizhangjiance1()                 //避障
{
  //if()

  char saidaozhongduan=0;
  if(k_of_right_line_fangcha<1000&&k_of_left_line_fangcha<1000)       //无赛道跳变，方差较小
  {
    //Image_lcd_num=1;
    if(Right_Lose_Border_Number==0&&Left_Lose_Border_Number==0&&Common_Lose_Flag==0)       //无丢边
    {
      Image_lcd_num=2;
      if(NumbDataleft<=50&&NumbDataleft>=30)         //边界数小于50个
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
        
        if(abs(sum[3]-sum[0])<10)                     //处于直道         //
        {
          Image_lcd_num=4;
          saidaozhongduan=1;
          
        }
      }
    }
    
  }
  
  if((saidaozhongduan==1||(Laser_Value<800&&Laser_Value>400&&changzhidao_F==1))&&huandaoF==0&&saidaoleixing==11)   //当距离小于80cm时且为长直道  避障   不能在环岛中
  {
    bizhang_F=1;
  }
}



/*==================================================================================
函数名称：bizhang
函数功能: 开始避障
时间：2019年6月6日17:07:55
参数：bizhang_F 避障标志位
返回值：
===================================================================================*/
uchar bizhangjieduan_F=0;
void bizhang()
{
  
  if(Laser_Value<200)  //出界保护
  {
    protect_flag=1;    //停车标志位
    RunState_flag=0;
    
  }
  if(1)         //向左避障
  {
    if(bizhangjieduan_F==0)             //第一阶段向左打死---------------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_R->S=0;                  //路程清零
        sv_R->Add_CntEncoder=0;         //编码器累计值清零
        daiyong_flag=1;
      }
      error=-80;                  //向左打死
      if(sv_R->S>0.31)
      {
        bizhangjieduan_F=1;               //避障阶段 1
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==1)             //第二阶段打直--------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_R->S=0;                  //路程清零
        sv_R->Add_CntEncoder=0;         //编码器累计值清零
        daiyong_flag=1;
      }
      error=0;
      if(sv_R->S>0.5)
      {
        bizhangjieduan_F=2;               //避障阶段 2
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==2)             //第三阶段向右打死------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_R->S=0;                  //路程清零
        sv_R->Add_CntEncoder=0;         //编码器累计值清零
        daiyong_flag=1;
      }
      error=80;
      if(sv_R->S>0.5)
      {
        bizhangjieduan_F=3;               //避障阶段 2
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==3)             //第三阶段向右打死------------------------------------
    {
      error=0;
      if(BlackRight[0][1]-BlackLeft[0][1]>80&&BlackRight[0][1]==157)
      {
        if(ImageData[59][BlackRight[0][1]-1]<100||ImageData[59][BlackRight[0][1]-2]<100||ImageData[59][BlackRight[0][1]-3]<100)//100为阈值 懒得查
        {
          
        }else
        {
          if(ImageData[59][BlackLeft[0][1]+1]<100||ImageData[59][BlackLeft[0][1]+2]<100||ImageData[59][BlackLeft[0][1]+3]<100)  //赛道中有黑出现
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
    
    
  }else                       //向右避障
  {
    if(bizhangjieduan_F==0)             //第一阶段向左打死---------------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_L->S=0;                  //路程清零
        sv_L->Add_CntEncoder=0;         //编码器累计值清零
        daiyong_flag=1;
      }
      error=80;                  //向左打死
      if(sv_L->S>0.3)
      {
        bizhangjieduan_F=1;               //避障阶段 1
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==1)             //第二阶段打直--------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_L->S=0;                  //路程清零
        sv_L->Add_CntEncoder=0;         //编码器累计值清零
        daiyong_flag=1;
      }
      error=0;
      if(sv_R->S>1.2)
      {
        bizhangjieduan_F=2;               //避障阶段 2
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==2)             //第三阶段向右打死------------------------------------
    {
      if(daiyong_flag==0)
      {
        sv_L->S=0;                  //路程清零
        sv_L->Add_CntEncoder=0;         //编码器累计值清零
        daiyong_flag=1;
      }
      error=-80;
      if(sv_L->S>0.5)
      {
        bizhangjieduan_F=3;               //避障阶段 2
        daiyong_flag=0;
      }
    }
    if(bizhangjieduan_F==3)             //第三阶段向右打死------------------------------------
    {
      error=0;
      if(BlackRight[0][1]-BlackLeft[0][1]>80&&BlackLeft[0][1]==2)
      {
        if(ImageData[59][BlackRight[0][1]-1]<100||ImageData[59][BlackRight[0][1]-2]<100||ImageData[59][BlackRight[0][1]-3]<100)//100为阈值 懒得查
        {
          //中间有黑点
        }else
        {
          if(ImageData[59][BlackLeft[0][1]+1]<100||ImageData[59][BlackLeft[0][1]+2]<100||ImageData[59][BlackLeft[0][1]+3]<100)  //赛道中有黑出现
          {
            //中间有黑点
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
函数名称：shexiangtoutiaojie
函数功能: 检测 摄像头是否调正 俯仰角是否正确
时间：2019年6月5日21:02:03
参数：
返回值：
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
        // Image_lcd_num=3;        //GUI显示
      }
    }
  }
}


/*==================================================================================
函数名称：fengmingqi1
函数功能: 蜂鸣器哔哔响
时间：2019年5月25日14:45:32
参数：
返回值：
===================================================================================*/
void fengmingqi1(uchar f)
{
  image_num_kaiqi=1;
  if(f==1)
  {
    if(Image_num%3==0)  //隔300ms 响100ms一次
    {
      BeepON();
    }else
    {
      BeepOFF();
    }
  }else if(f==2)
  {
    if(Image_num%6==0)  //隔600ms 响100ms 
    {
      BeepON();
    }else
    {
      BeepOFF();
    }
  }else if(f==3)
  {
    if(Image_num%10==0)  //隔1000ms 响100ms
    {
      BeepON();
    }else
    {
      BeepOFF();
    }
  }else if(f==4)
  {
    if(Image_num%15==0)  //隔1000ms 响100ms
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
  if(Image_num%6==0)  //响100ms一次
  {
    BeepON();
  }else
  {
    BeepOFF();
  }
}

/*==================================================================================
函数名称：banmaxian
函数功能: 斑马线检测
时间：2019年5月6日20:42:42
参数：
返回值： banmaxian_F   为1检测到斑马线 
===================================================================================*/
char yanhoutingche=0;   //停在斑马线之后
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
  
  //----------------检测49行的是否存在斑马线-------------------------
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
  //----------------检测44行的是否存在斑马线-------------------------
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
  
  
  
  if(BlackRight[0][1]-BlackLeft[0][1]<15&&banmaxian_F==0)    //第一行检测出左边界减右边界小于15,
  {
    if(Flag_L_1==2&&Flag_R_1==1&&abs(leftguai-rightguai)<5)   //出现拐点并且，趋势正确
    {
      uchar i=0;
      if(rightguai>leftguai)
      {
        i=rightguai;
      }else
      {
        i=leftguai;
      }
      
      if(BlackRight[i+1][1]-BlackLeft[i+1][1]>100)   //拐点上方的左右边界距离大于  100    100 是根据斑马线在图像59行处往上占得行宽数量的最上面一行的列宽大致给出
      {
        banmaxian_F=1;
        
      }
    }
  }
  
  if(BlackRight[2][1]-BlackLeft[2][1]<15&&banmaxian_F==0)    //第三行检测出左边界减右边界小于15,
  {
    if(Flag_L_1==2&&Flag_R_1==1&&abs(leftguai-rightguai)<5)   //出现拐点并且，趋势正确
    {
      uchar i=0;
      if(rightguai>leftguai)
      {
        i=rightguai;
      }else
      {
        i=leftguai;
      }
      
      if(BlackRight[i+1][1]-BlackLeft[i+1][1]>100)   //拐点上方的左右边界距离大于  100    100 是根据斑马线在图像59行处往上占得行宽数量的最上面一行的列宽大致给出
      {
        banmaxian_F=1;
        
      }
    }
  }
  
  
  if(banmaxian_F==1)    //检测是否把斑马线过完
  {
    uchar lc,rc;
    uchar lc1,rc1;
    one_Border_Scan(59,&lc,&rc);
    one_Border_Scan(55,&lc1,&rc1);
    if(rc-lc<20||rc1-lc1<20)    //59行压在斑马线上面
    {
      chubanmaxian=1;
    }
    if(rc-lc>80&&chubanmaxian==1)    //已经经过斑马线
    {
      chubanmaxianjishu++;     
    }
    if(chubanmaxianjishu>3)               //确认经过斑马线
    {
      banmaxian_F=0;                      //斑马线标志位置零
      chubanmaxianjishu=0;                //计数置零
      chubanmaxian=0;
      if(Started_check_Zebra==1)
      {
        if(yanhoutingche==0)
        {
          sv_R->S=0;                  //路程清零
          yanhoutingche=1;
        }
      }
    }
  }
  
  if(sv_R->S>0.33&&yanhoutingche==1)
  {
    protect_flag=1;     //停车
    yanhoutingche=0;
    fengmingqi_daiyong=1;
  }
}

/*======================================================================
函数名称：protect
函数功能: 出界停车
时间：2019年5月21日12:52:48
参数：
======================================================================*/
void protect()
{
  uchar pro_flag=0;         //检测到出界的标志位
  if(banmaxian_F==0)    //避免与斑马线碰撞
  {
    if(BlackRight[0][1]-BlackLeft[0][1]<20)
    {
      
      pro_flag=1;
      //      if((TRB-BRB>5&&TRB-BRB<13&&BRB==0)||(TLB-BLB>5&&TLB-BLB<13&&BLB==0))  //转弯的时候出去大部分也让其继续运动
      //      {
      //        pro_flag=0;
      //      }
    }
  }else
  {
    uchar b1=0;    //黑块的数量
    for(uchar i1=1;i1<NumbDatacenter;i1++)    //160/5=32
    {
      if(abs(Center[i1][1]-Center[i1-1][1])>5)  //上下中线相差为5 b1计数1次
      {
        b1++;  
      }
    }
    
    if(b1>5&&banmaxian_F==0)
    {
      pro_flag=1;
    }
    
  }
  if(Laser_Value<200&&Started_check_Zebra==1)   //激光测距检测到小于20cm  停车保护
  {
    pro_flag=1;
    //fengmingqi_daiyong=1;
  }
  if(pro_flag==1&&duanlu_F==0)
  {
    protect_flag=1;    //停车标志位
    RunState_flag=0;
  }
  
}
void protect1()
{
  uchar pro_flag=0;         //检测到出界的标志位
  if(banmaxian_F==0)    //避免与斑马线碰撞
  {
    for(uchar i=0;i<=5;i++)
    {
      if(BlackRight[i][1]-BlackLeft[i][1]<50)     //这里会 与半圆转弯的情会亏冲突  但在锁函数会把这种情况处理掉
      {
        
        pro_flag=1;
        //      if((TRB-BRB>5&&TRB-BRB<13&&BRB==0)||(TLB-BLB>5&&TLB-BLB<13&&BLB==0))  //转弯的时候出去大部分也让其继续运动
        //      {
        //        pro_flag=0;
        //      }
      }
    }
    
  }else
  {
    //    //------------------------------------识别方式1----看跳变------------------------
    //    uchar b1=0,b2=0;    //黑块的数量
    //    for(uchar i1=1;i1<NumbDataleft;i1++)
    //    {
    //      if(abs(BlackLeft[i1][1]-BlackLeft[i1-1][1])>4)  //上下左边界相差为5  跳变大于 4 次 b1计数1次
    //      {
    //        b2++;  
    //      }
    //    }
    //    
    //    
    //    for(uchar i1=1;i1<NumbDatacenter;i1++)    //160/5=32
    //    {
    //      if(abs(Center[i1][1]-Center[i1-1][1])>4)  //上下中线相差为5 b1计数1次
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
    //    //------------------------------------识别方式2----看趋势------------------------
    //    //一般出界之后，赛道外面会反光，所以即使检测出赛道，赛道会有向外扩的趋势
    //    b1=0;
    //    for(int i1=0;i1<NumbDatacenter&&Cross_F!=1;i1+=5)  //处于十字状态不用这种出界保护
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
    //    protect_flag=1;    //停车标志位
    //    RunState_flag=0;
    //  }
    uchar b1=0;    //黑块的数量
    for(uchar i1=1;i1<NumbDatacenter;i1++)    //160/5=32
    {
      if(abs(Center[i1][1]-Center[i1-1][1])>5)  //上下中线相差为5 b1计数1次
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
    protect_flag=1;    //停车标志位
    RunState_flag=0;
  }
  
}

/*======================================================================
函数名称：yibianbuxian
函数功能: 直道的一边进行补线   1 为以右边为边界补线   2 为以左边为边界补线       直道补线
时间：2019年6月14日19:40:02
参数：
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
  Center[i][1]=BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k)+(BlackRight[0][1]-BlackRight[i][1]));  //根据直线的普通畸变率 -(i/right_k)
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
          Center[i][1]=BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k));  //根据直线的普通畸变率 -(i/right_k) +(i/right_k)
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
函数名称：yibianbuxian
函数功能: 直道的一边进行补线   1 为以右边为边界补线   2 为以左边为边界补线       直道补线
时间：2019年5月13日21:15:47
参数：
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
          Center[i][1]=BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k)+(BlackRight[0][1]-(i/right_k)-BlackRight[i][1]));  //根据直线的普通畸变率
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
函数名称：zuidaliekuan
函数功能: 用摄像头检测59行的列宽，以及确定畸变斜率
时间：时间
参数：
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
  //      saidao_F=1;//只执行一次
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
      //saidao_F=1;//只执行一次
    }
  }
  kuan=BlackRight[0][1]-BlackLeft[0][1];
  
  //  if(k_of_right_line_fangcha<150&&k_of_left_line_fangcha<150&&Left_Lose_Border_Number==0&&Right_Lose_Border_Number==0&&NumbDataleft>40&&saidao_F==1)
  //  {
  //    if((BlackRight[30][1]+BlackLeft[30][1])/2+(BlackRight[0][1]+BlackLeft[0][1])/2<6)  //偏差接近中线
  //    {
  //      left_k=k_of_left_line_pingjun/2;    //列除行         //
  //      right_k=k_of_right_line_pingjun/2;    //列除行         //
  //      //image_flag_1=1;
  //      saidao_F=2;//确定后就不再执行
  //    }
  //  }
  
}



/***********************************************************
函数名称：credible 
函数功能: error的的可信判断与选取，滤波
时间：2019年4月13日20:48:24
参数：none        dataerror[4]
************************************************************/
//初步想法              1、方差 x
//                      2、矩阵	x

/*

当前帧error与上一帧error相比，若相差大于10   上帧再与上上帧相比

*/

void credible()
{
  if(before_error_Flag==1)      //右环岛  延用上一帧偏差
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
    
  }else if(before_error_Flag==2)//左环岛  延用上一帧偏差
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
  //errorpinghua();      //error跳变大于20的话限制其跳变过大
  
  
  dataerror[0]=error;
  dataerror[1]=dataerror[0];  //往后覆盖
  dataerror[2]=dataerror[1];  //往后覆盖
  dataerror[3]=dataerror[2];  //往后覆盖
  
  
  
  
}
/***********************************************************
函数名称：imgbefor
函数功能: 把之前图片得到的信息进行处理
时间：2019年5月4日18:00:28
参数：none
************************************************************/
//上一帧的图片信息
void imgbefor()
{
  if(NumbDatacenter>20)
  {
    center35lie=Center[20][1];    //上一帧39行中线所在列
  }else
  {
    center35lie=Center[NumbDatacenter-1][1];
  }
  
}



/*==================================================================================
函数名称：errorpinghua()
函数功能: 把error拟合趋势曲线使之平滑
时间：
参数：
===================================================================================*/
void errorpinghua()
{
  //---------------------限幅滤波----------------------------------------------------
  /*  if(dataerror[0]>error&&(dataerror[0]-error>40))
  {
  error=dataerror[0]-40;
}
  if(error>dataerror[0]&&(error-dataerror[0]>40))
  {
  error=dataerror[0]+40;
}
  */
  //-----------------------平均滤波-------------------------------------------
  error=(int)(error+dataerror[0])/2;  //
  
}





/***********************************************************
函数名称：Three_Border_Scan
函数功能: 59-57行的边界扫描，得出较准确的左右边界，并以最为准确的行数的左右边界作为基点往上扫
时间：2019年3月26日21:33:42
参数：none
************************************************************/

//-------------------------------移植删除函数------------------//
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
函数名称：huandao_one
函数功能: 环岛检测--是否出现环岛
时间：2019年5月10日14:39:41
参数：无
返回值：falg_bord   1右边界出现环岛   2左边界出现环岛
===================================================================================*/
void huandao_one()
{
  //huandaoF=2;
  uchar flag_direction=0;   //圆环可能出现的方向
  if(k_of_left_line_fangcha<500&&k_of_right_line_fangcha>1000&&Right_Lose_Border_Number!=0&&Left_Lose_Border_Number==0||(Right_Lose_Border_Number>18&&k_of_left_line_fangcha<500&&Left_Lose_Border_Number==0&&k_of_right_line_fangcha>500))
  {
    if(Flag_R_1==1&&Flag_L_1==0&&rightguai<35)
    {
      //pass();
      flag_direction=1;//圆环在右边
      huandaoF=1;
    }
  }else if(k_of_left_line_fangcha>1000&&k_of_right_line_fangcha<500&&Left_Lose_Border_Number!=0&&Right_Lose_Border_Number==0||(Left_Lose_Border_Number>18&&k_of_right_line_fangcha<500&&Right_Lose_Border_Number==0&&k_of_left_line_fangcha>500))
  {
    if(Flag_L_1==2&&Flag_R_1==0&&leftguai<35)
    {
      flag_direction=2;//圆环在左边
      //pass();
      huandaoF=2;
      //pass();
    }
  }
  
}


/*==================================================================================
函数名称：huandao_two
函数功能: 进环岛----中线计算
时间：2019年5月10日14:39:35
参数：无
返回值：无
===================================================================================*/
void huandao_two()
{
  //pass();
  //上位机测试把 huandaozhuangtai_flag=；
  //huandaozhuangtai_flag=0;
  //pass();
  if(huandaoF==1)   //处于第一阶段进此
  {
    if((BRB<6||(Flag_R_1==1&&rightguai<5))&&huandaozhuangtai_flag==0&&BRB!=99)
    {
      huandaozhuangtai_flag=1;		//huandaozhuangtai_flag==1 时的状态为正在过第一个横向的赛道   靠最大左边计算中线
      Image_lcd_num=1;
      //DELAY_MS(50);
    }
    if(BRB>10&&huandaozhuangtai_flag==1)    
    {
      huandaozhuangtai_flag=2;		//huandaozhuangtai_flag==2 时的状态为完全经过第一个向横向的赛道
      Image_lcd_num=2;								 //第一段丢边行完全过去了，准备转入第二个丢边行  用Tyep_5 计算中线或者用Tyep_6
    }
    //pass();
    if(Right_Lose_Border_Number>5&&Left_Lose_Border_Number>5&&huandaozhuangtai_flag==2)  
    {
      //BeepON();
      huandaozhuangtai_flag=3;		//进环岛  用type_0 转弯
      //pass1();
      Image_lcd_num=3;
    }
    
    if(huandaozhuangtai_flag==3&&NumbDataright-TRB>15&&Left_Lose_Border_Number<5)
    {
      huandaozhuangtai_flag=4;         //出圆环状态
      Image_lcd_num=4;
    }
    //pass();
    //huandaozhuangtai_flag=4;
    if(huandaozhuangtai_flag==4&&BLB==0&&Left_Lose_Border_Number>10&&BRB==0&&Right_Lose_Border_Number>10)
    {
      huandaozhuangtai_flag=5;	//出完圆环，要忽略第二个丢边  
      Image_lcd_num=5;
    }
    if(((NumbDataright-TRB>10&&BLB==99)||Left_Lose_Border_Number<15)&&huandaozhuangtai_flag==5)//
    {
      huandaozhuangtai_flag=6;	//出完圆环，要忽略第二个丢边  
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
      yibianbuxian(2);            //根据左边界计算中线
      
    }else if(huandaozhuangtai_flag==1)		//处于第二阶段进此
    {
      yibianbuxian(2);
    }else if(huandaozhuangtai_flag==2)
    {	
      //pass();
      if(BRB==99)      //如果还没有出现丢边   继续求最大左边球中线
      {
        yibianbuxian(2);
      }else
      {
        if(Common_Lose_Flag==1)
        {
          //pass();
          before_error_Flag=1;            //此时偏差采用上一帧的偏差
          
        }else   //刚刚转如弯道时会有两边同时丢边的情况，此时用上一帧的error来代替此帧   
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
        
      }else   //刚刚转如弯道时会有两边同时丢边的情况，此时用上一帧的error来代替此帧   
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
  }else if(huandaoF==2)    //==========================--==========================--左=========------======================----------------------
  {
    //pass();
    
    if((BLB<6||(Flag_L_1==2&&leftguai<5))&&huandaozhuangtai_flag==0&&BLB!=99)
    {
      //pass();
      huandaozhuangtai_flag=1;		//huandaozhuangtai_flag==1 时的状态为正在过第一个横向的赛道   靠最大左边计算中线
      Image_lcd_num=1;
      //DELAY_MS(50);
    }
    
    if(BLB>10&&huandaozhuangtai_flag==1)    
    {
      //pass();
      huandaozhuangtai_flag=2;		//huandaozhuangtai_flag==2 时的状态为完全经过第一个向横向的赛道
      Image_lcd_num=2;								 //第一段丢边行完全过去了，准备转入第二个丢边行  用Tyep_5 计算中线或者用Tyep_6
    }
    //pass();
    
    if(Right_Lose_Border_Number>5&&Left_Lose_Border_Number>5&&huandaozhuangtai_flag==2)   //进环岛两边丢边时的计算
    {
      //pass();
      //BeepON();
      huandaozhuangtai_flag=3;		//进环岛  用type_0 转弯
      //pass1();
      Image_lcd_num=3;
    }
    
    if(huandaozhuangtai_flag==3&&NumbDataleft-TLB>15&&Right_Lose_Border_Number<5)
    {
      //pass();
      huandaozhuangtai_flag=4;         //出圆环状态
      Image_lcd_num=4;
    }
    //pass();
    //huandaozhuangtai_flag=4;
    //huandaozhuangtai_flag=4;
    if(huandaozhuangtai_flag==4&&BLB==0&&Left_Lose_Border_Number>10&&BRB==0&&Right_Lose_Border_Number>10)
    {
      //pass();
      huandaozhuangtai_flag=5;	//出完圆环，要忽略第二个丢边  
      Image_lcd_num=5;
    }
    //huandaozhuangtai_flag=5;
    if(((NumbDataleft-TLB>10&&BRB==99)||Right_Lose_Border_Number<15)&&huandaozhuangtai_flag==5)//
    {
      //pass();
      //ImageData[BlackRight[BRB][0]][BlackRight[BRB][1]-2]=RED;
      //pass();
      huandaozhuangtai_flag=6;	//出完圆环，要忽略第二个丢边  
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
      
    }else if(huandaozhuangtai_flag==1)		//处于第二阶段进此
    {
      yibianbuxian(1);
    }else if(huandaozhuangtai_flag==2)
    {	
      //pass();
      if(BLB==99)      //如果还没有出现丢边   继续求最大左边球中线
      {
        yibianbuxian(1);
      }else
      {
        if(Common_Lose_Flag==1)
        {
          //pass();
          before_error_Flag=2;
          
        }else   //刚刚转如弯道时会有两边同时丢边的情况，此时用上一帧的error来代替此帧   
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
        before_error_Flag=2;//刚刚转如弯道时会有两边同时丢边的情况，此时用上一帧的error来代替此帧   
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
函数名称：huandao
函数功能: 环岛检测
时间：2019年5月8日21:15:15
参数：无
===================================================================================*/
void huandao()
{
  //pass();
  if(huandaoF==0)          //当检测出环岛后就不再执行此操作      在完全驶出环岛后 huandaoF 标志位清零
  {
    huandao_one();    //第一步----检测是否出现环岛
    // Common_Lose_Flag_effective=0;           //就算有共同丢边也继续边界扫描标志位
    
  }
  //huandaoF=2;
  
  if(huandaoF!=0)
  {
    Common_Lose_Flag_effective=1;
    //pass();
    huandao_two();          //环岛路径规划
  }
  
}




/*==================================================================================
函数名称：Type_0
函数功能: 半圆弯道计算
时间：2019年4月13日14:03:28
参数：a   1 为右半圆   2 为左半圆
===================================================================================*/
void Type_0(uchar a)
{
  NumbDatacenter=0;
  
  zhixingshu++;
  saidaoleixing=0;//赛道类型为半圆转弯
  if(a==1)
  {
    yibianbuxian(2);
  }if(a==2)
  {
    yibianbuxian(1);
  }
  
}
/*==================================================================================
函数名称：Type_1
函数功能: 直道丢边类型的中线计算
时间：2019年4月13日14:03:28
参数：无
===================================================================================*/
void Type_1()
{
  uchar i=0;
  uchar x1,y1;			//保存下标
  NumbDatacenter=0;		//中线数组的下标置零
  zhixingshu++;			//全局变量执行数
  if(TLB==NumbDataleft-1||TRB==NumbDataright-1)
  {
    Type_11();			//直线计算方法
    return ;
  }
  if(Left_Lose_Border_Number!=0)   //左丢边直道
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
  ///2019年4月10日22:28:53
  
}

/*==================================================================================
函数名称：Type_5la
函数功能: 赛道尽头转弯的中线计算,用于环岛切尽赛道时以及驶出赛道时使用
时间：2019年6月27日12点23分
参数：无
===================================================================================*/
void Type_5la(uchar a)
{
  //pass();
  uchar i;
  uchar x1=0,y1=0;
  zhixingshu++;
  saidaoleixing=5;      //赛道尽头转弯
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
函数名称：Type_5
函数功能: 赛道尽头转弯的中线计算
时间：2019年4月13日14:03:28
参数：无
===================================================================================*/
void Type_5(uchar a)
{
  //pass();
  uchar i;
  uchar x1=0,y1=0;
  zhixingshu++;
  saidaoleixing=5;      //赛道尽头转弯
  
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
函数名称：Type_6
函数功能: 普通转弯的中线计算
时间：2019年4月13日14:03:28
参数：无
===================================================================================*/
void Type_6()         //赛道尽头转弯
{
  int i,k;		//循环变量
  int x1,y1;		//计算出来的中点
  zhixingshu++;	//执行数
  saidaoleixing=6;      //赛道类型为普通转弯 ，也就是不同通过丢边连线
  NumbDatacenter=0;	//中线数下标置零
  if(BlackRight[rightguai][0]>BlackRight[leftguai][0])    //判断左拐还是右拐  此时判断为右拐
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
        x1=(i-2+BlackRight[rightguai][0])/2;   //i为扫出来的行
        y1=BlackRight[rightguai][1];
        ConnectSpot(x1,y1);
        break;
      }
    }
    //ImageData[x1][y1]=RED;
    
  }else                     //左拐
  {
    k=leftguai;
    for(i=0;i<=k;i++)           //计算拐点以下中线
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
函数名称：Type_11
函数功能: 普通直线计算
时间：2019年4月13日14:03:28
参数：
===================================================================================*/
void Type_11()
{
  uchar i;
  zhixingshu++;
  saidaoleixing=11;//普通直线
  for(i=0;i<NumbDataleft;i++)
  {
    Center[NumbDatacenter][0]=BlackRight[i][0];
    Center[NumbDatacenter][1]=(BlackRight[i][1]+BlackLeft[i][1])/2;
    NumbDatacenter++;
  }
}


/***********************************************************
函数名称：Type_12
函数功能: 左边界或者右边界下部稍微丢边，出十字情况，
时间：2019年4月13日10:18:41
参数：flag ：1为右边界丢边   2为左边界丢边
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
函数名称：CrossTypebuxian()
函数功能: 十字处理中的两边丢变且丢变行相差较大的弯道
时间：2019年5月1日10:59:46
参数：
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
函数名称：basescan
函数功能: 找出赛道最可能出现的列,然后找出基点
时间：2019年5月1日14:21:35
参数：  返回 lC  rC  R  左边界列 右边界列 边界所在行
===================================================================================*/
void basescan(uchar *lC,uchar *rC,uchar *R)
{
  uchar saidaosuozailie=80;           //赛道最大可能在的列
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
    for (j = 59; j > 10; j -= 3)   //隔三行取一点   计算白点的数量
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
      rightC = 80+i;		//赛道中线80列
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
  saidaosuozailie=center35lie;//上位机值
  j=saidaosuozailie;
  
  q=liekuan35-10;						//35行的列宽 用于十字确定左右边界的时候使用
  
  while (i > 5)     //连续white行    且行数大于5
  {
    
    //pass();
    for (k = 0; k < q; k++)       //往右边扫
    {
      if ((ImageData[i][j + k] > THRESHOLD&&ImageData[i][j + k + 1] < THRESHOLD)||j+k+1==157)
      {
        n1 = i;    //右边界行
        n2 = j + k + 1;  //右边界列
        break;
      }
    }
    for(k=0;k<q;k++)
    {
      if ((ImageData[i][j - k] > THRESHOLD&&ImageData[i][j - k - 1] < THRESHOLD)||j - k-1==2)
      {
        m1 = i;  //左边界行
        m2 = j - k - 1;	 //左边界列
        break;
      }
    }
    
    if (m2<n2 && n2 - m2 > liekuan35-40  &&n2 - m2 > 20&& n2 != 0 && m2 != 0&&	m1==n1)    //防止溢出
    {
      //pass();
      flag1 = 1;     //		 依靠左右边界 把中线求出来的标志
      
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
函数名称：LeftRight2
函数功能: 在已有扫描到边界的情况下 根据最后扫描得到的左右边界的列数   从第 hang 开始 开始扫描左右边界的函数（共同丢边不跳出）
时间：2019年4月13日13:48:50
参数：hang      要开始扫描的行数

注意：
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
        
        for(;j1<=157;j1++)              //157作为边界
        {
          scancolNumble++;
          //pass();
          if(j1+1>=157)              //检测到右边图像边缘
          {
            //pass();
            //Right_Lose_Border_Number++;				//右丢边行数
            BlackRight[NumbDataright][0]=i1;   //把右边界赋给右边界数组
            BlackRight[NumbDataright][1]=157;
            //ImageData[i1][j1+1]=RED;
            if(BRB==99)                            //如果下部右边界没有赋值这个值给下部的右丢边行
            {
              BRB=NumbDataright;
            }else if(1)                               //如果下部右边界有赋值这把这个值给上部的右丢边行
            {
              TRB=NumbDataright;
            }
            
            break;
          }else
          {
            //pass();
            if((ImageData[i1][j1]>THRESHOLD)&&(ImageData[i1][j1+1]<THRESHOLD))//判断右边那是否连续为黑色边界 
            {
              
              BlackRight[NumbDataright][0]=i1;   //把右边界赋给右边界数组
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
      }else      //59行的右边界的左边一列的上一行如果为黑
      {
        for(;j1>0;j1--)    
        {
          scancolNumble++;
          if(j1-1==0)
          {
            BlackRight[NumbDataright][0]=i1;	//把右边界赋给右边界数组
            BlackRight[NumbDataright][1]=j1-1;
            
            break;
          }
          if((ImageData[i1][j1]<THRESHOLD)&&(ImageData[i1][j1-1]>THRESHOLD))//判断左边那是否连续为黑色边界 ，并加一个Flag_1标志位判断其是否确定
          {
            BlackRight[NumbDataright][0]=i1;	//把右边界赋给右边界数组
            BlackRight[NumbDataright][1]=j1;
            break;
          }
          if(scancolNumble>ScanColNumble&&rightguai==0)
          {
            Flag_R_1=2;						//右边界往左转
            //pass1();
            rightguai=NumbDataright-1;						//保存右拐点行数
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
      
      if(ImageData[i1][j2]<THRESHOLD)         ////下行的左边界的右边一列的上一行如果为黑
      {
        for(;j2<159;j2++)   //往右边行开始判断
        {
          scancolNumble++;
          if(j2+1==157)
          {
            BlackLeft[NumbDataleft][0]=i1;   //把右边界赋给右边界数组
            BlackLeft[NumbDataleft][1]=j2+1;
            
            //pass();
            break;
          }
          if((ImageData[i1][j2]<THRESHOLD)&&(ImageData[i1][j2+1]>THRESHOLD))//判断右边那是否连续为黑色边界 ，并加一个Flag_2标志位判断其是否确定
          {	
            BlackLeft[NumbDataleft][0]=i1;   //把右边界赋给右边界数组
            BlackLeft[NumbDataleft][1]=j2;
            break;
          }
          if(scancolNumble>ScanColNumble&&leftguai==0)
          {
            Flag_L_1=1;               //左边界往右转
            //pass();
            leftguai=NumbDataleft-1;
          }
        }
      }else                             //下行的左边界的右边一列的上一行如果为白
      {
        for(;j2>=2;j2--)    
        {
          scancolNumble++;
          if(j2-1<=2)
          {
            
            BlackLeft[NumbDataleft][0]=i1;  //把右边界赋给右边界数组
            BlackLeft[NumbDataleft][1]=2;
            //Left_Lose_Border_Number++;					//左丢边行数
            if(BLB==99)                            //如果下部左边界没有赋值这个值给下部的左丢边行
            {
              BLB=NumbDataleft;
            }else                                //如果下部左边界有赋值这把这个值给上部的左丢边行
            {
              TLB=NumbDataleft;
            }
            break;
          } 
          
          if((ImageData[i1][j2]>THRESHOLD)&&(ImageData[i1][j2-1]<THRESHOLD))//判断左边那是否连续为黑色边界 ，并加一个Flag_1标志位判断其是否确定
          {
            
            BlackLeft[NumbDataleft][0]=i1;  //把右边界赋给右边界数组
            BlackLeft[NumbDataleft][1]=j2-1;
            break;	
          }
          if(scancolNumble>ScanColNumble&&leftguai==0)
          {
            Flag_L_1=2;               //左边界往左转
            leftguai=NumbDataleft-1;
          }
        }
      }
      scancolNumble=0;
      NumbDataleft++;
      
    }
    
    //---------------------------------Left Black Scan over-----------------------------------------------------------//
    
    /////////////////////////////跳出循环的条件判断///////////////////////////////////////////////////////
    if (BlackLeft[NumbDataleft-1][1]>=BlackRight[NumbDataright-1][1])			//左边界的列数大于或等于右边界的列
    {
      //pass();
      sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//搜索结束行
      NumbDataleft--;
      NumbDataright--;
      break;
    }
    
    if (abs(BlackLeft[NumbDataleft-1][1]-BlackRight[NumbDataright-1][1])<=12)			//赛道宽度太窄
    {
      //pass();
      sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//搜索结束行
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
函数名称：Cross
函数功能: 十字处理
时间：2019年4月13日14:03:28
参数：
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
  if(Common_Lose_Flag==1&&sousuojieshuhang<19||(Right_Lose_Border_Number>0&&Left_Lose_Border_Number>0&&TRB>30&&TLB>30)||(Flag_L_1==2&&Flag_R_1==1))    //可以开启十字的处理 
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
    
    if(abs(TRB-TLB)>crossturnnum&&TLB!=99&&TRB!=99&&(Right_Lose_Border_Number<15||Left_Lose_Border_Number<15))           //crossturnnum为TRB-TLB  多少为十字
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
  saidaoleixing=10;           //赛道类型为十字
  //pass();
  if(Flag_L_1==2&&Flag_R_1==1&&NumbDataleft>0)          //十字在中间
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
    q=liekuan35-10;						//35行的列宽 用于十字确定左右边界的时候使用
    
    while (i> 0)     //连续white行    且行数大于5
    {
      
      //pass();
      for (k = 0; k < q; k++)       //往右边扫
      {
        if ((ImageData[i][j + k] > THRESHOLD&&ImageData[i][j + k + 1] < THRESHOLD)||j+k+1==157)
        {
          n1 = i;    //右边界行
          n2 = j + k + 1;  //右边界列
          break;
        }
      }
      for(k=0;k<q;k++)
      {
        if ((ImageData[i][j - k] > THRESHOLD&&ImageData[i][j - k - 1] < THRESHOLD)||j - k-1==2)
        {
          m1 = i;  //左边界行
          m2 = j - k - 1;	 //左边界列
          break;
        }
      }
      
      if (m2<n2 && n2 - m2 < liekuan35 &&n2 - m2 > 20&& n2 != 0 && m2 != 0&&	m1==n1&&n2>m2)    //防止溢出
      {
        //pass();
        flag1 = 1;     //		 依靠左右边界 把中线求出来的标志
        
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
    
    
  }else if((BlackRight[0][1]==157&&BlackLeft[0][1]!=2)||(BlackRight[0][1]!=157&&BlackLeft[0][1]==2))    //  左或右边界一开始就丢边
  {
    //pass();
    NumbDatacenter=0;
    //pass();
    
    uchar rnum,lnum,rmax=0,lmax=0;		//每列的最大白点数
    uchar saidaosuozailie=80;           //赛道最大可能在的列
    uchar leftC=0,rightC=0;
    for (i = 0; i < 75; i += 5)
    {
      rnum=0;
      lnum=0;
      for (j = 59; j > 2; j -= 2)   //隔三行取一点   计算白点的数量
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
        rightC = 80+i;		//赛道中线80列
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
    //saidaosuozailie=center35lie;//上位机值
    
    i=shiziscanfbase;
    j=saidaosuozailie;
    //j=70;
    q=liekuan35-10;						//35行的列宽 用于十字确定左右边界的时候使用
    
    while (i> 0)     //连续white行    且行数大于5
    {
      
      //pass();
      for (k = 0; k < q; k++)       //往右边扫
      {
        if ((ImageData[i][j + k] > THRESHOLD&&ImageData[i][j + k + 1] < THRESHOLD)||j+k+1==157)
        {
          n1 = i;    //右边界行
          n2 = j + k + 1;  //右边界列
          break;
        }
      }
      for(k=0;k<q;k++)
      {
        if ((ImageData[i][j - k] > THRESHOLD&&ImageData[i][j - k - 1] < THRESHOLD)||j - k-1==2)
        {
          m1 = i;  //左边界行
          m2 = j - k - 1;	 //左边界列
          break;
        }
      }
      
      if (m2<n2 && n2 - m2 < liekuan35 &&n2 - m2 > 20&& n2 != 0 && m2 != 0&&	m1==n1&&n2>m2)    //防止溢出
      {
        //pass();
        flag1 = 1;     //		 依靠左右边界 把中线求出来的标志
        
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
  }else if(Common_Lose_Flag==1||(Right_Lose_Border_Number>0&&Left_Lose_Border_Number==0)||(Right_Lose_Border_Number==0&&Left_Lose_Border_Number>0))         //有共同丢边的情况
  {
    //pass();
    //pass1();
    uchar rnum,lnum,rmax=0,lmax=0;		//每列的最大白点数
    uchar saidaosuozailie=80;           //赛道最大可能在的列
    uchar leftC=0,rightC=0;
    for (i = 0; i < 75; i += 5)
    {
      rnum=0;
      lnum=0;
      for (j = 59; j > 0; j -= 3)   //隔三行取一点   计算白点的数量
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
        rightC = 80+i;		//赛道中线80列
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
    q=liekuan35-10;						//35行的列宽 用于十字确定左右边界的时候使用
    
    while (i >0 )     //连续white行    且行数大于5
    {
      
      //pass();
      for (k = 0; k < q; k++)       //往右边扫
      {
        if ((ImageData[i][j + k] > THRESHOLD&&ImageData[i][j + k + 1] < THRESHOLD)||j+k+1==157)
        {
          n1 = i;    //右边界行
          n2 = j + k + 1;  //右边界列
          break;
        }
      }
      for(k=0;k<q;k++)
      {
        if ((ImageData[i][j - k] > THRESHOLD&&ImageData[i][j - k - 1] < THRESHOLD)||j - k-1==2)
        {
          m1 = i;  //左边界行
          m2 = j - k - 1;	 //左边界列
          break;
        }
      }
      
      if (m2<n2 && n2 - m2 < liekuan35 &&n2 - m2 > 20&& n2 != 0 && m2 != 0&&	m1==n1&&n2>m2)    //防止溢出
      {
        //pass();
        flag1 = 1;    
        BlackRight[NumbDataright ][0] = n1; //		 依靠左右边界 把中线求出来的标志
        
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
函数名称：ScanOneRow
函数功能: 扫描出某一行的左右边界
时间：2019年4月13日14:03:28
参数：uchar R,uchar *rc,uchar *lc 
R为扫描的行数   *rc  与  *lc 为返回的 右边界的列数 与左边界的列数
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
  
  MaxWhite=0;  //最大白块
  WhiteCnt=0;  //白块数量
  wStart=W_START;  
  wEnd=W_START;
  W_START=0;
  
  MaxEnd=0;
  MaxWhite=0;
  wStart=2;
  WhiteCnt=0;
  for(W=2;W<157;W++)
  {
    if(ImageData[i][W]>THRESHOLD&&ImageData[i][W+1]>THRESHOLD)//连续为白
    {
      WhiteCnt++;
    }else			//黑白相间
    {
      if(WhiteCnt>=MaxWhite)			//现在的白块数量大于之前的白块数量
      {
        wEnd=W;               //右边界列数赋值
        MaxWhite=WhiteCnt;		//最大替换
        MaxStart=wStart;		
        MaxEnd=wEnd;			//最大白块的结束列
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
函数名称：ConnectSpot
函数功能: 把中线和一点相连
时间：2019年4月13日14:03:28
参数：无
返回值：
===================================================================================*/
void ConnectSpot(int duanR,int duanC)						//把一点与中点连接
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
函数名称：Three_Border_Scan
函数功能: 确定59-57行左右边界，作为基点进行左右边界扫描
时间：2019年4月13日14:03:28
参数：无
返回值： 1  59-57行的左右边界扫描正常       -1  //59-57行的左右边界扫描不正常you跳变
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
    MaxWhite=0;  //最大白块
    WhiteCnt=0;  //白块数量
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
        if(ImageData[i][W]>THRESHOLD&&ImageData[i][W+1]>THRESHOLD)//连续为白
        {
          WhiteCnt++;
        }else			//黑白相间
        {
          if(WhiteCnt>=MaxWhite)			//现在的白块数量大于之前的白块数量
          {
            wEnd=W;               //右边界列数赋值
            MaxWhite=WhiteCnt;		//最大替换
            MaxStart=wStart;		
            MaxEnd=wEnd;			//最大白块的结束列
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
        if(BRB==99)                            //如果下部右边界没有赋值这个值给下部的右丢边行
        {
          BRB=NumbDataright;
        }else if(1)                               //如果下部右边界有赋值这把这个值给上部的右丢边行
        {
          TRB=NumbDataright;
        }
      }
      if(MaxStart==2)
      {
        if(BLB==99)                            //如果下部右边界没有赋值这个值给下部的右丢边行
        {
          BLB=NumbDataleft;
        }else if(1)                               //如果下部右边界有赋值这把这个值给上部的右丢边行
        {
          TLB=NumbDataleft;
        }
      }
      NumbDataright++;
      NumbDataleft++;
      if(MaxEnd==157&&MaxStart==2)		///如果左右边界同时丢边
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
  }else           //在检测出斑马线情况下，用这种 下三行检测    抗干扰极差 ， 丢边情况无法处理，赛道反光干扰也较大
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
函数名称：one_Border_Scan
函数功能: 确定某一行行左右边界
时间：2019年5月1日14:31:42
参数：r  扫描的行数  lc rc  左边界列 右边界列
返回值： 
===================================================================================*/
void one_Border_Scan(uchar r,uchar *lc,uchar *rc)
{
  int WhiteCnt;
  int W;
  int wStart,wEnd,MaxWhite,W_START=0; 
  
  
  MaxWhite=0;  //最大白块
  WhiteCnt=0;  //白块数量
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
    if(ImageData[i][W]>THRESHOLD&&ImageData[i][W+1]>THRESHOLD)//连续为白
    {
      WhiteCnt++;
    }else			//黑白相间
    {
      if(WhiteCnt>=MaxWhite)			//现在的白块数量大于之前的白块数量
      {
        wEnd=W;               //右边界列数赋值
        MaxWhite=WhiteCnt;		//最大替换
        MaxStart=wStart;		
        MaxEnd=wEnd;			//最大白块的结束列
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
函数名称：LeftRight
函数功能: 在已有扫描到边界的情况下 根据最后扫描得到的左右边界的列数   从第 hang 开始 开始扫描左右边界的函数
时间：2019年4月13日13:48:50
参数：hang      要开始扫描的行数

注意：函数未封装好  
左右边界都没扫描出则  要给以下元素赋值  根据需要计算
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
        
        for(;j1<=157;j1++)              //157作为边界
        {
          scancolNumble++;
          //pass();
          if(j1+1>=157)              //检测到右边图像边缘
          {
            //pass();
            //Right_Lose_Border_Number++;				//右丢边行数
            BlackRight[NumbDataright][0]=i1;   //把右边界赋给右边界数组
            BlackRight[NumbDataright][1]=157;
            //ImageData[i1][j1+1]=RED;
            if(BRB==99)                            //如果下部右边界没有赋值这个值给下部的右丢边行
            {
              BRB=NumbDataright;
            }else if(1)                               //如果下部右边界有赋值这把这个值给上部的右丢边行
            {
              TRB=NumbDataright;
            }
            
            break;
          }else
          {
            //pass();
            if((ImageData[i1][j1]>THRESHOLD)&&(ImageData[i1][j1+1]<THRESHOLD))//判断右边那是否连续为黑色边界 
            {
              
              BlackRight[NumbDataright][0]=i1;   //把右边界赋给右边界数组
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
      }else      //59行的右边界的左边一列的上一行如果为黑
      {
        for(;j1>0;j1--)    
        {
          scancolNumble++;
          if(j1-1==0)
          {
            BlackRight[NumbDataright][0]=i1;	//把右边界赋给右边界数组
            BlackRight[NumbDataright][1]=j1-1;
            
            break;
          }
          if((ImageData[i1][j1]<THRESHOLD)&&(ImageData[i1][j1-1]>THRESHOLD))//判断左边那是否连续为黑色边界 ，并加一个Flag_1标志位判断其是否确定
          {
            BlackRight[NumbDataright][0]=i1;	//把右边界赋给右边界数组
            BlackRight[NumbDataright][1]=j1;
            break;
          }
          if(scancolNumble>ScanColNumble&&rightguai==0)
          {
            Flag_R_1=2;						//右边界往左转
            //pass1();
            rightguai=NumbDataright-1;						//保存右拐点行数
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
      
      if(ImageData[i1][j2]<THRESHOLD)         ////下行的左边界的右边一列的上一行如果为黑
      {
        for(;j2<159;j2++)   //往右边行开始判断
        {
          scancolNumble++;
          if(j2+1==157)
          {
            BlackLeft[NumbDataleft][0]=i1;   //把右边界赋给右边界数组
            BlackLeft[NumbDataleft][1]=j2+1;
            
            //pass();
            break;
          }
          if((ImageData[i1][j2]<THRESHOLD)&&(ImageData[i1][j2+1]>THRESHOLD))//判断右边那是否连续为黑色边界 ，并加一个Flag_2标志位判断其是否确定
          {	
            BlackLeft[NumbDataleft][0]=i1;   //把右边界赋给右边界数组
            BlackLeft[NumbDataleft][1]=j2;
            break;
          }
          if(scancolNumble>ScanColNumble&&leftguai==0)
          {
            Flag_L_1=1;               //左边界往右转
            //pass();
            leftguai=NumbDataleft-1;
          }
        }
      }else                             //下行的左边界的右边一列的上一行如果为白
      {
        for(;j2>=2;j2--)    
        {
          scancolNumble++;
          if(j2-1<=2)
          {
            
            BlackLeft[NumbDataleft][0]=i1;  //把右边界赋给右边界数组
            BlackLeft[NumbDataleft][1]=2;
            //Left_Lose_Border_Number++;					//左丢边行数
            if(BLB==99)                            //如果下部左边界没有赋值这个值给下部的左丢边行
            {
              BLB=NumbDataleft;
            }else                                //如果下部左边界有赋值这把这个值给上部的左丢边行
            {
              TLB=NumbDataleft;
            }
            break;
          } 
          
          if((ImageData[i1][j2]>THRESHOLD)&&(ImageData[i1][j2-1]<THRESHOLD))//判断左边那是否连续为黑色边界 ，并加一个Flag_1标志位判断其是否确定
          {
            
            BlackLeft[NumbDataleft][0]=i1;  //把右边界赋给右边界数组
            BlackLeft[NumbDataleft][1]=j2-1;
            break;	
          }
          if(scancolNumble>ScanColNumble&&leftguai==0)
          {
            Flag_L_1=2;               //左边界往左转
            leftguai=NumbDataleft-1;
          }
        }
      }
      scancolNumble=0;
      NumbDataleft++;
      
    }
    
    //---------------------------------Left Black Scan over-----------------------------------------------------------//
    
    /////////////////////////////跳出循环的条件判断///////////////////////////////////////////////////////
    if (BlackLeft[NumbDataleft-1][1]>=BlackRight[NumbDataright-1][1])			//左边界的列数大于或等于右边界的列
    {
      //pass();
      sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//搜索结束行
      NumbDataleft--;
      NumbDataright--;
      break;
    }
    
    if (abs(BlackLeft[NumbDataleft-1][1]-BlackRight[NumbDataright-1][1])<=12)			//赛道宽度太窄
    {
      //pass();
      sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//搜索结束行
      NumbDataleft--;
      NumbDataright--;
      break;
    }
    
    if (ImageData[BlackLeft[NumbDataleft-1][0]][BlackLeft[NumbDataleft-1][1]]>THRESHOLD&&ImageData[BlackRight[NumbDataright-1][0]][BlackRight[NumbDataright-1][1]]>THRESHOLD)
    {
      sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//两侧同时丢边，搜索结束行
      
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
函数名称：BorderScan
函数功能: 
A、跟踪搜索边界：从基点往上扫描得到左右边界，以2作为左丢边列，以157为右丢边列
B、BLB TLB BRB TRB 各记录左右边界的最下面的丢变行数与最下面的丢边行数
C、
时间：
参数：none
************************************************************/
void BorderScan()
{
  //Common_Lose_Flag_effective=1;       //当检测到环岛时，就算共同丢边也不会，左右别介
  unsigned char scancolNumble=0;
  char LoseBlack_Flag_R=0,LoseBlack_Flag_L=0;
  int i1,j1,j2;
  
  Common_Lose_Flag_effective=1;
  if(Three_Border_Scan()!=-1||Common_Lose_Flag_effective||1)            //左右边界基点都找到
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
          
          for(j1;j1<=157;j1++)              //157作为边界
          {
            scancolNumble++;
            //pass();
            if(j1+1>=157)              //检测到右边图像边缘
            {
              //pass();
              //Right_Lose_Border_Number++;				//右丢边行数
              BlackRight[NumbDataright][0]=i1;   //把右边界赋给右边界数组
              BlackRight[NumbDataright][1]=157;
              //ImageData[i1][j1+1]=RED;
              if(duan_diubian_R==0)
              {
                if(BRB==99)                            //如果下部右边界没有赋值这个值给下部的右丢边行
                {
                  BRB=NumbDataright;
                }
                if(NumbDataright-1==TRB||TRB==99)                               //如果下部右边界有赋值这把这个值给上部的右丢边行
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
                if(BRB2==99)                            //如果下部右边界没有赋值这个值给下部的右丢边行
                {
                  BRB2=NumbDataright;
                  //pass();
                }else if(NumbDataright-1==TRB2||TRB2==99)                               //如果下部右边界有赋值这把这个值给上部的右丢边行
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
              if((ImageData[i1][j1]>THRESHOLD)&&(ImageData[i1][j1+1]<THRESHOLD))//判断右边那是否连续为黑色边界 
              {
                
                BlackRight[NumbDataright][0]=i1;   //把右边界赋给右边界数组
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
        }else      //59行的右边界的左边一列的上一行如果为黑
        {
          for(j1;j1>0;j1--)    
          {
            scancolNumble++;
            if(j1-1==0)
            {
              BlackRight[NumbDataright][0]=i1;	//把右边界赋给右边界数组
              BlackRight[NumbDataright][1]=j1-1;
              
              break;
            }
            if((ImageData[i1][j1]<THRESHOLD)&&(ImageData[i1][j1-1]>THRESHOLD))//判断左边那是否连续为黑色边界 ，并加一个Flag_1标志位判断其是否确定
            {
              BlackRight[NumbDataright][0]=i1;	//把右边界赋给右边界数组
              BlackRight[NumbDataright][1]=j1;
              break;
            }
            if(scancolNumble>ScanColNumble&&rightguai==0)
            {
              Flag_R_1=2;						//右边界往左转
              //pass1();
              rightguai=NumbDataright-1;						//保存右拐点行数
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
        
        if(ImageData[i1][j2]<THRESHOLD)         ////下行的左边界的右边一列的上一行如果为黑
        {
          for(j2;j2<159;j2++)   //往右边行开始判断
          {
            scancolNumble++;
            if(j2+1==157)
            {
              BlackLeft[NumbDataleft][0]=i1;   //把右边界赋给右边界数组
              BlackLeft[NumbDataleft][1]=j2+1;
              
              //pass();
              break;
            }
            if((ImageData[i1][j2]<THRESHOLD)&&(ImageData[i1][j2+1]>THRESHOLD))//判断右边那是否连续为黑色边界 ，并加一个Flag_2标志位判断其是否确定
            {	
              BlackLeft[NumbDataleft][0]=i1;   //把右边界赋给右边界数组
              BlackLeft[NumbDataleft][1]=j2;
              break;
            }
            if(scancolNumble>ScanColNumble&&leftguai==0)
            {
              Flag_L_1=1;               //左边界往右转
              //pass();
              leftguai=NumbDataleft-1;
            }
          }
        }else                             //下行的左边界的右边一列的上一行如果为白
        {
          for(j2;j2>=2;j2--)    
          {
            scancolNumble++;
            if(j2-1<=2)
            {
              
              BlackLeft[NumbDataleft][0]=i1;  //把右边界赋给右边界数组
              BlackLeft[NumbDataleft][1]=2;
              //Left_Lose_Border_Number++;					//左丢边行数
              if(duan_diubian_L==0)
              {
                if(BLB==99)                            //如果下部左边界没有赋值这个值给下部的左丢边行
                {
                  BLB=NumbDataleft;
                }
                if(NumbDataleft-1==TLB||TLB==99)                               //如果下部左边界有赋值这把这个值给上部的左丢边行
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
                if(BLB2==99)                            //如果下部左边界没有赋值这个值给下部的左丢边行
                {
                  BLB2=NumbDataleft;
                }else if(NumbDataleft-1==TLB2||TLB2==99)                               //如果下部左边界有赋值这把这个值给上部的左丢边行
                {
                  TLB2=NumbDataleft;
                }
              }
              
              break;
            } 
            
            if((ImageData[i1][j2]>THRESHOLD)&&(ImageData[i1][j2-1]<THRESHOLD))//判断左边那是否连续为黑色边界 ，并加一个Flag_1标志位判断其是否确定
            {
              
              BlackLeft[NumbDataleft][0]=i1;  //把右边界赋给右边界数组
              BlackLeft[NumbDataleft][1]=j2-1;
              break;	
            }
            if(scancolNumble>ScanColNumble&&leftguai==0)
            {
              Flag_L_1=2;               //左边界往左转
              leftguai=NumbDataleft-1;
            }
          }
        }
        scancolNumble=0;
        NumbDataleft++;
        
      }
      
      //---------------------------------Left Black Scan over-----------------------------------------------------------//
      
      /////////////////////////////跳出循环的条件判断///////////////////////////////////////////////////////
      if (BlackLeft[NumbDataleft-1][1]>=BlackRight[NumbDataright-1][1])			//左边界的列数大于或等于右边界的列
      {
        //pass();
        jiesujiance_F=1;
        if(sousuojieshuhang==0)
          sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//搜索结束行
        NumbDataleft--;
        NumbDataright--;
        break;
      }
      if (abs(BlackLeft[NumbDataleft-1][1]-BlackRight[NumbDataright-1][1])<=20&&i1<45)			//赛道宽度太窄
      {
        //pass();
        if(sousuojieshuhang==0)
        {
          
          sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//搜索结束行
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
          sousuojieshuhang=BlackLeft[NumbDataleft-1][0];//两侧同时丢边，搜索结束行
        //NumbDataleft--;
        //NumbDataright--;
        //if(Common_Lose_Flag_effective==0)//Common_Lose_Flag_effective 就算同时丢边也不结束扫描
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
      
    }//i1循环
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
函数名称：p_rintf
函数功能: 大印左右中边界，左右拐点
时间：2019年4月13日13:48:50
参数：
======================================================================*/
void p_rintf()
{
  /*                                    //下位机注释掉
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
函数名称：xielvfangcha
函数功能: 计算左右边界斜率，计算左右边界丢边数
时间：2019年4月13日13:48:50
参数：
======================================================================*/
void xielvfangcha()
{
  //shu(2);
  //shu(10);
  //shu(4);
  uchar i=0;    //循环变量
  if(Common_Lose_Flag==1&&Common_Lose_Flag_effective==0)	        //如果有同时丢边不执行且Common_Lose_Flag_effective==0为0的时候
  {
    return;
  }
  
  uchar fangchashu=0;					//方差数
  //uchar tongji_k_right_add=0;			//斜率方差
  
  //////左边界斜率///////////////////////////////
  for(i=0;i<NumbDataleft-3;i++)
  {
    if(BlackLeft[i][1]>2)
    {
      k_of_left_line[fangchashu]=(BlackLeft[i+3][1]-BlackLeft[i][1])*2/(BlackLeft[i+3][0]-BlackLeft[i][0]);
      k_of_left_line_pingjun+=k_of_left_line[i];//统计出总值
      fangchashu++;
    }
  }
  
  k_of_left_line_pingjun=k_of_left_line_pingjun/fangchashu;		//左边界斜率平均值   此值是乘以2的值
  
  for(i=0;i<fangchashu;i++)
  {
    k_of_left_line_fangcha+=(k_of_left_line[i]-k_of_left_line_pingjun)*(k_of_left_line[i]-k_of_left_line_pingjun);//左边界方差
  }
  ///////////右边界斜率/////////////////////////////
  fangchashu=0;
  for(i=0;i<NumbDataright-3;i++)
  {
    //if(BlackRight[i][1]<157)
    {
      k_of_right_line[fangchashu]=(BlackRight[i+3][1]-BlackRight[i][1])*2/(BlackRight[i+3][0]-BlackRight[i][0]);
      k_of_right_line_pingjun+=k_of_right_line[i];//统计出总值
      fangchashu++;
    }
  }
  //ImageData[5][fangchashu]=RED;
  k_of_right_line_pingjun=k_of_right_line_pingjun/fangchashu;
  for(i=0;i<fangchashu;i++)
  {
    k_of_right_line_fangcha+=(k_of_right_line[i]-k_of_right_line_pingjun)*(k_of_right_line[i]-k_of_right_line_pingjun); //右边界方差
  }
  
  Left_Lose_Border_Number=0;		//统计左边界丢边数
  i=0;
  
  for(;i<NumbDataleft-1;i++)
  {
    if(BlackLeft[i][1]<=2)
    {
      Left_Lose_Border_Number++;    //计算左丢边数
      //pass();
      if(BlackLeft[i+1][1]>2)
      {
        left_diubiancishu++;        //计算左丢边次数   
      }
      
    }
  }
  
  
  
  Right_Lose_Border_Number=0;		//统计右丢边数
  
  for(i=0;i<NumbDataright;i++)
  {
    if(BlackRight[i][1]>=157)
    {
      Right_Lose_Border_Number++;     //计算右丢边数
      if(BlackRight[i+1][1]<157)
      {
        right_diubiancishu++;         //计算右丢边次数
      }
      
      
    }
  }
  if(right_diubiancishu==0&&BRB!=99)    //防止当一直丢边的时候此值不会变为1
  {
    right_diubiancishu=1;
  }
  if(left_diubiancishu==0&&BLB!=99)     //防止当一直丢边的时候此值不会变为1
  {
    left_diubiancishu=1;
  }
  
}

/*==================================================================================
函数名称：guaidiancredible
函数功能: 根据拐点出现的行数相差判断拐点是否可信
时间：2019年4月28日18:34:10
参数：
返回值：LRbeyond_F    0 不可信    1 可信
===================================================================================*/
void guaidiancredible()
{
  if(leftguai!=0&&rightguai!=0)
  {
    if(abs(leftguai-rightguai)<shizhizuidahangkuan)  //20  十字最大行宽
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
函数名称：diubiancredible
函数功能: 此函数适用于转弯时，一方的丢边在最开始丢边，在另一边弯道尽头丢边，此时判断出这种情况然后把另一边的丢边数置零
时间：2019年5月5日19:44:56
参数：
返回值： 0   1   不可信    可信
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
函数名称：diubiankexingpanduan
函数功能: 判断丢变是否应该采用，TLB-BLB<10  BLB>3   如果符合丢边行置零
时间：2019年5月25日12:00:09
参数：
返回值：
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
函数名称：shizixiediubian
函数功能: 解决斜十字单丢边情况
时间：
参数：
返回值：1检测到  0 未检测到
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
函数名称：Type_5_2
函数功能: 赛道尽头转弯的中线计算  计算第二条丢边的计算
时间：2019年6月16日16:39:36
参数：无
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
函数名称：Type_Judgement
函数功能: 根据标志位方差，左右边界进行赛道类型判断
时间：2019年4月13日14:03:28
参数：
返回值：
===================================================================================*/
void Type_Judgement()
{
  if(shizixiediubian()==1)  //处理斜十字单丢边
  {
    return;
  }
  diubiancredible();
  guaidiancredible();
  
  diubiankexingpanduan();
  zuidaliekuan();           //求最大列宽  及 畸变斜率
  
  
  huandao();                    //环岛处理
  
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
      if (BlackLeft[BLB][0] > 55)     //半圆类型
      {
        //pass();
        
        if (BlackLeft[TLB][0] - BlackLeft[NumbDataleft - 1][0] < 8)   //有效数据在丢边行8行内  ||Left_Lose_Border_Number>25
        {
          shu(0);
          Type_0(2);
          //pass();
          return ;
        }
        else                  //  有效数据在丢边行10行外     //会出现情况   十字    直线丢边   半圆十字    
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
          } else if((int)k_of_right_line_fangcha < 700 && (int)k_of_left_line_fangcha < 700)    //直线丢边
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
            
          }else if((int)k_of_right_line_fangcha>1000&&(int)k_of_left_line_fangcha <1000)    //斜十字    斜十字  右边界上部边界不可信，左边界下半部丢边上半部边界置信        待解决       2019年4月13日09:58:47
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
          }else    //以十字处理      
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
      else             //BlackLeft[BLB][0] < 55   丢边行在上方
      {
        //pass();
        //pass();
        //if()
        if (BlackLeft[TLB][0] - BlackLeft[NumbDataleft - 1][0] < 8)   //有效数据在丢边行8行内     赛道尽头弯道   十字
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
            Type_11();    //用直线赛道函数计算
            //Type_11();
            return ;
          }
          else               //此处不可信 2019年4月12日20:22:29
          {
            /*//pass();
            shu(10);
            Cross();
            return ;*/
          }
          
        }
        else                //有效数据在丢边行5行外   十字类型的弯道赛道尽头弯道  直线丢边   2019年4月11日22:31:12
        {
          //pass();
          if(NumbDataleft<46)  //有效数小于46个
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
          if((int)k_of_right_line_fangcha < 500 && (int)k_of_left_line_fangcha < 500)      //两边方差都小
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
      
    }////------------------------------------右丢边开始----------------------------------------------------
    else if (Left_Lose_Border_Number < 5 && Right_Lose_Border_Number > 3)    //右丢边
    {
      
      //pass();
      if (BlackRight[BRB][0] > 55)     //在最下面开始丢变
      {
        //pass();
        if (BlackRight[TRB][0] - BlackRight[NumbDataright - 1][0] < 8)   //有效数据在丢边行8行内  ||Right_Lose_Border_Number>25
        {
          shu(0);
          Type_0(1);
          return ;
          //pass();
        }
        else                  //  有效数据在丢边行5行外     //会出现情况   十字    直线丢边   半圆十字
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
          }else if ((int)k_of_right_line_fangcha < 700 && (int)k_of_left_line_fangcha < 700)    //直线丢边
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
          }else if((int)k_of_left_line_fangcha>1000&&(int)k_of_right_line_fangcha <1000)    //斜十字    斜十字  左边界上部边界不可信，右边界下半部丢边上半部边界置信        待解决       2019年4月13日09:58:47
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
      else             //BlackLeft[BLB][0] < 55   丢边行在上方
      {
        if(NumbDataleft<46)  //有效数小于46个
        {
          if(Flag_R_1 == 2 && Flag_L_1 == 1)
          {
            shu(5);
            Type_5(1); 
            return;
          }
        }
        //pass();
        if (BlackRight[TRB][0] - BlackRight[NumbDataright - 1][0] < 8)   //有效数据在丢边行8行内     赛道尽头弯道   十字
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
          }else               //此处不可信 2019年4月12日20:22:29
          {
            /*//pass();
            shu(10);
            Cross();
            return ;*/
          }//pass();
          //pass();
          
        }
        else                //有效数据在丢边行10行外   十字类型的弯道赛道尽头弯道  直线丢边   2019年4月11日22:31:12
        {
          //pass();
          if((int)k_of_right_line_fangcha < 500 && (int)k_of_left_line_fangcha < 500)      //两边方差都小
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
        Type_11();   //////普通直线
        return ;
        
      }
      //pass();
      ////////////////2019年3月31日18:30:01////////////////////////////
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
      Type_11();   //////普通直线
      return ;
    }
  }
}


/*==================================================================================
函数名称：init
函数功能: 各种变量初始化
时间：2019年4月13日14:03:28
参数：
返回值：
===================================================================================*/
void init()           //初始化
{
  for(uchar i=0;i<60;i++)          //左右边界及中线初始化
  {
    BlackLeft[i][0]=59-i;  
    BlackLeft[i][1]=2;
    BlackRight[i][0]=59-i;
    BlackRight[i][1]=157;
    Center[i][0]=59-i;
    Center[i][1]=80;
  }
  NumbDataleft=0;				//左边界的有效数值
  NumbDataright=0;			//右边界的有效数值
  NumbDatacenter=0;			//中线的有效数值
  
  Flag_L_1=0;		//1右 2左
  Flag_R_1=0;		//1右 2左
  
  k_of_left_line_fangcha=0;	//左边界方差
  k_of_right_line_fangcha=0;	//右边界方差
  
  sousuojieshuhang=0;			//搜索结束行
  
  zhixingshu=0;				//类型判断次数
  
  Left_Lose_Border_Number=0;	//左边界丢失行数
  Right_Lose_Border_Number=0;	//右边界丢失行数	
  
  TRB=99;						//top right border
  BRB=99;						//bottom right border		
  TLB=99;						//top left border
  BLB=99;						//bottom left border
  
  
  BLB2=99;		//左边界的最下面的丢边点	Below Left Border
  TLB2=99;		//左边界的最上面的丢边点	Top Left Border
  BRB2=99;		//右边界的最下面的丢边点	Below Right Border
  TRB2=99;		//右边界的最上面的丢边点	Top Right Border
  
  duan_diubian_R=0;			//右边界又开始丢边，标志位
  duan_diubian_L=0;			//左边界又开始丢边，标志位
  
  rightguai=0;   //右拐点
  leftguai=0;		//左拐点
  
  Common_Lose_Flag=0;			//共同丢边标志位
  
  LRbeyond_F=0;	     //右边界减左边界大于	最大列宽加10 标志位  
  
  k_of_left_line_pingjun=0;		//左边界斜率
  
  k_of_right_line_pingjun=0;	//右边界斜率
  
  left_diubiancishu=0;      //左丢边次数     init 置0
  right_diubiancishu=0;     //右丢边次数     init 置0
  
  saidaoleixing=99;
  zhidaopanduan_F=0;
  lianxuwandao_F=0;
  jiesujiance_F=0;                  //再扫边的时后因为赛道列宽小于一定值而跳出
  NumbDatacenter_T=0;
  
  
  L_left=0,L_right=0;   //左边界有的趋势
  R_left=0,R_right=0;   //右边界有的趋势
  C_left=0,C_right=0;   //中线
  left_daihao=0,right_daihao=0;         //左边界的左右趋势先后
  zongtiqushi=0;        //总趋势
}



void out_error()
{
  //  int i=0;
  //  uchar j=0;
  //  i=Encoder_set;   //Encoder_set为设定速度
  
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
  //  before=10;      //前瞻为10
  //  while(i>80)
  //  {
  //    i=i-5;       //速度上升5  则前瞻加1
  //    j++;
  //  }
  
  //  if(Encoder_set>105)           //before  默认15
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
  //在59行的赛道列宽为118的情况下  斜率 k=35/58    此斜率k可以在图像算法中动态监测
  float ko=58/33;    //斜率k:左边赛道或者右边赛道行差与列差的比值  斜率k与一周期车预计走的路程对应的前瞻有关系
  double v;     //速度
  double s;     //路程
  v=sv_R->V;            //10ms累计的脉冲数    此车 6225脉冲数/米     v的单位为:m/ms
  v=v*100;    //把v的单位换算成 cm/ms
  s=v*90;//此时整套程序执行周期为 T=90ms 在一个程序周期将走 s=v*90 （cm）  s 单位为cm
  //  s=s*100;      //把m 换算成cm
  
  before=(int)((s-9.5)*ko);    //s=v*T 00        s=before/k+9.5    before:前瞻
  //现在处理 现实赛道与 摄像头采集到的图像信息进行 处理换算
  //预计由于图形畸变，随着赛道逐渐远离，相应的行宽对应的实际距离的比值将会增加，比值如何确定 1、初步考虑 是否和斜率有关系  2、
  
  
  if(before<10)
  {
    before=10;     //防止程序出错
  }
  if(before>55)
  {
    before=55;   //防止程序出错
  }

}

/*==================================================================================
函数名称：SpeedControl
函数功能: 通过判断赛道类型以及中线计算情况，来调节不同的速度   通过方差
时间：2019年6月21日21:58:40
参数：
返回值：  Expectation_speed
===================================================================================*/

void SpeedControl()
{
  
  //int Expectation_speed=0;
  int i,j;
  int sum=0;
  float averge=0;  //平均中线列
  float zhongxianfangcha=0;			//中线方差   如果中线方差小于15  =_= 嗨起来
  
  for(i=0;i<NumbDatacenter;i++)			//当中线算到了边界之后 把中线数组的有效数多余的给去掉  
  {
    if(Center[i][1]>=157)				//这会引起一个问题，就是会不会有一种情况，前瞻下面丢边但前瞻不丢边,预估出现这种问题的概率很低很低
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
    changzhidao_F=1;  //长直道
    //Expectation_speed=130;       //期望速度赋值
  }
  //if(fangcha)
}

/*==================================================================================
函数名称：suo
函数功能: 当车子出界之后，把状态锁住 舵机打死 直到回到正常赛道上
时间：2019年6月22日02:37:10
参数：  char suo=0;  
返回值：
===================================================================================*/
void suo()
{
  //============================半圆转弯锁===================================//   出界   还有阳光
  if(BlackRight[0][1]-BlackLeft[0][1]<=70&&BlackRight[0][1]==157&&Right_Lose_Border_Number>5&&error>=77)
  {
    suo_F=1;   //锁住右边
  }
  if(BlackRight[0][1]-BlackLeft[0][1]<=70&&BlackLeft[0][1]==2&&Left_Lose_Border_Number>5&&error<=-77)
  {
    suo_F=2;   //锁住右边
  }
  if(BlackRight[0][1]-BlackLeft[0][1]>=100)  //回到正常赛道上
  {
    suo_F=0;
  }
  if(suo_F==1)
  {
    error=80;  //舵机向右打死
    
  }else if(suo_F==2)
  {
    error=-80;  //向左打死
  }
  //============================半圆转弯锁END================================
}


/*==================================================================================
函数名称：shujucunchu
函数功能: 用链表储存10帧左边界右边界中线的值
时间：2019年6月28日12点31分
参数：  xianpleft    xianpright  xianpcenter   头节点尾节点
返回值：
===================================================================================*/


//uchar leftborder[10][2][65];//存储10个左边界数组
//uchar centerborder[10][2][65];//存储10个中线数组
//uchar rightborder[10][2][65];//存储10个右边界数组
void shujucunchu()
{
  Xian * a=NULL;
  //-----------------左边界保存-----------------------------------------------
  if(xianpleft->front==NULL)            //头指针为空
  {
    xianpleft->front=(Xian *)malloc(sizeof(Xian));      //分配空间地址
    xianpleft->rear=(Xian *)malloc(sizeof(Xian));      //分配空间地址
    a=xianpleft->rear;
    for(int i=0;i<NumbDataleft;i++)                     //左边界赋值
    {
      a->border[i][0]=BlackLeft[i][0];
      a->border[i][1]=BlackLeft[i][1];
    }
    xianpleft->number=1;
  }else
  {
    
    if(a->number<10)           //保存10帧数据
    {
      a=xianpleft->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpleft->rear=a;                //尾指针跟新
      for(int i=0;i<NumbDataleft;i++)                     //左边界赋值
      {
        a->border[i][0]=BlackLeft[i][0];
        a->border[i][1]=BlackLeft[i][1];
      }
      xianpleft->number++;
    }else                       //tou
    {
      a=xianpleft->front;
      xianpleft->front=a->next;
      free(a);   //释放内存
      
      a=xianpleft->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpleft->rear=a;                //尾指针跟新
      for(int i=0;i<NumbDataleft;i++)                     //左边界赋值
      {
        a->border[i][0]=BlackRight[i][0];
        a->border[i][1]=BlackRight[i][1];
      }
      xianpleft->number=10;
    }
  }
  //-----------------右边界保存-----------------------------------------------
  if(xianpright->front==NULL)            //头指针为空
  {
    xianpright->front=(Xian *)malloc(sizeof(Xian));      //分配空间地址
    xianpright->rear=(Xian *)malloc(sizeof(Xian));      //分配空间地址
    a=xianpright->rear;
    for(int i=0;i<NumbDataright;i++)                     //左边界赋值
    {
      a->border[i][0]=BlackRight[i][0];
      a->border[i][1]=BlackRight[i][1];
    }
    xianpright->number=1;
  }else
  {
    
    if(a->number<10)           //保存10帧数据
    {
      a=xianpright->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpright->rear=a;                //尾指针跟新
      for(int i=0;i<NumbDataleft;i++)                     //左边界赋值
      {
        a->border[i][0]=BlackRight[i][0];
        a->border[i][1]=BlackRight[i][1];
      }
      xianpright->number++;
    }else                       //tou
    {
      a=xianpright->front;
      xianpright->front=a->next;
      free(a);   //释放内存
      
      a=xianpright->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpright->rear=a;                //尾指针跟新
      for(int i=0;i<NumbDataright;i++)                     //左边界赋值
      {
        a->border[i][0]=BlackRight[i][0];
        a->border[i][1]=BlackRight[i][1];
      }
      xianpright->number=10;
    }
  }
  //---------------------中线保存-----------------------------------------------
  
  if(xianpright->front==NULL)            //头指针为空
  {
    xianpcenter->front=(Xian *)malloc(sizeof(Xian));      //分配空间地址
    xianpcenter->rear=(Xian *)malloc(sizeof(Xian));      //分配空间地址
    a=xianpcenter->rear;
    for(int i=0;i<NumbDatacenter;i++)                     //中线赋值
    {
      a->border[i][0]=Center[i][0];
      a->border[i][1]=Center[i][1];
    }
    xianpcenter->number=1;
  }else
  {
    if(a->number<10)           //保存10帧数据
    {
      a=xianpcenter->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpcenter->rear=a;                //尾指针跟新
      for(int i=0;i<NumbDatacenter;i++)                     //中线赋值
      {
        a->border[i][0]=Center[i][0];
        a->border[i][1]=Center[i][1];
      }
      xianpcenter->number++;
    }else                       //tou
    {
      a=xianpcenter->front;
      xianpcenter->front=a->next;
      free(a);   //释放内存
      
      a=xianpcenter->rear;
      a->next=(Xian *)malloc(sizeof(Xian));
      a=a->next;
      xianpcenter->rear=a;                //尾指针跟新
      for(int i=0;i<NumbDatacenter;i++)                     //中线赋值
      {
        a->border[i][0]=Center[i][0];
        a->border[i][1]=Center[i][1];
      }
      xianpcenter->number=10;
    }
  }
}



/*==================================================================================
函数名称：beep_f
函数功能: 控制蜂鸣器
时间：2019年6月29日16点34分
参数：  f
返回值：
===================================================================================*/
void beep_f()
{
  if(beep_F==1)
  {
    IsBeep=1;           //开蜂鸣器
    
  }else if(beep_F==0)
  {
    IsBeep=0;           //关蜂鸣器
  }
}


/*==================================================================================
函数名称：beep_jizhi
函数功能: 蜂鸣器长响或是 响一下
时间：2019年6月29日16点34分
参数：  f  fengmingqi_daiyong
返回值：
===================================================================================*/

void beep_jizhi(char f)
{
  
  if(f==1&&beep_F==0&&fengmingqi_daiyong==1)    //只响一声      蜂鸣器处于未开启状态 开启 蜂鸣器
  {
    fengmingqi_daiyong=2;
    Image_num=0;
    
    beep_F=1;
    fengmingqi_f=1;
    
  }
  if(Image_num>25&&fengmingqi_f==1)           //响0.2s
  {
    beep_F=0;                 //蜂鸣器关闭
    fengmingqi_daiyong=3;
    fengmingqi_f=0;
    Image_num=0;
  }
}


/*==================================================================================
函数名称：sanjiaojiance
函数功能:断路的三角检测
时间:
参数：
返回值：
===================================================================================*/
void sanjiaojiance()
{
  int i=0,num=0;
  int w1=0,w2=0,w3=0;
  if(NumbDataleft<=45&&Left_Lose_Border_Number<20&&Left_Lose_Border_Number<20)           //边界数小于45
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
函数名称：duanlujiance
函数功能: 断路检测  检测到断路到之后切换成电磁跑
时间:
参数：  duanlu_F
返回值：
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
    // fengmingqi_daiyong=1;          //蜂鸣器   
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
函数名称：left_border_zuiyou
函数功能: 找到右边界最左边的点的下标
时间:
参数：
返回值：    j  左边界最左边列的数组下标
===================================================================================*/
int left_border_zuiyou()
{
  int j=0,val=159;
  for(int i=0;i<NumbDataright;i++)
  {
    if(BlackRight[i][1]<val)     //遍历一遍  右边界最左边列的大小
    {
      val=BlackRight[i][1];
      j=i;
    }
  }
  return j;
}


/*==================================================================================
函数名称：center_L_R
函数功能: 寻找最左和最右边的点
时间:
参数：
返回值：   
===================================================================================*/
void center_L_R(int *l,int *r)
{
  int val1=159,val2=0;
  int j1=0,j2=0;
  for(int i=0;i<NumbDatacenter;i++)
  {
    if(Center[i][1]<val1)     //遍历一遍  最左边列的大小
    {
      val1=BlackRight[i][1];
      j1=i;
    }
    if(Center[i][1]>val2)       //找最右的点
    {
      val2=BlackRight[i][1];
      j2=i;
    }
  }
  *l=j1;
  *r=j2;
}





/*==================================================================================
函数名称：speed_control
函数功能: 根据左边赛道固定一行的列相差的值来控制此时的速度，分为三档速度，期望脉冲数为150（直道）   130（较直道--大圆环，大弯）    115（小圆环与小弯）
根据有效的边界数量，以及第10行的左边界与第9行左边界的列差值来判断期望脉冲数应该给多少
时间:
参数：
返回值：
===================================================================================*/
int speed_dingshi=0;    //速度定时
int speed_dingshi_F=0;    //速度定时
int zhuan=0;
void speed_control()
{

  if(lianxuwandao_F==3&&Expectation_speed>=-5)         //检测小s弯
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
  
  
  //磨轮胎专业户---------------------------------------------------------
  //  if(Image_num1>1000)           //10s
  //  {
  //    zhuan++;
  //    Image_num1=0;
  //    if(Expectation_speed<50)    //期望脉冲数  +50
  //    {
  //      Expectation_speed=Expectation_speed+10;
  //    }
  //  }
  //  
  //  if(zhuan>18)          //3分钟后停车
  //  {
  //    protect_flag=1;
  //  }
  //磨轮胎专业户---------------------------------------------------------

}


/*==================================================================================
函数名称：lianxuwandao
函数功能: 判断中线的趋势
时间：2019年7月5日00点54分
参数：
返回值：   3 表示既有向左的趋势又有向右的趋势 是一个s弯  2表示只有向左的趋势  1表示只有向右的趋势 
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
  
  if((TRB-BRB>45&&NumbDataright-TRB>4)||(TLB-BLB>45&&NumbDataleft-TLB>4)||(left==right==1&&TRB-BRB>45))           //小 s弯丢边特比严重  速度更加减小
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
函数名称：sanxianqushipanduan
函数功能: 判断左右弯道的趋势
时间：
参数：
返回值：   3 表示既有向左的趋势又有向右的趋势 是一个s弯  2表示只有向左的趋势  1表示只有向右的趋势 
===================================================================================*/
//  L_left=0,L_right=0;   //左边界有的趋势
//  R_left=0,R_right=0;   //右边界有的趋势
//  C_left=0,C_right=0;   //中线
//  left_daihao=0,right_daihao=0;         //左边界的左右趋势先后
//  zongtiqushi=0;        //总趋势

int sanxianqushipanduan()
{
  int i=0;
  
  int youxian_L=0,youxian_R=0;
  for(i=0;i<NumbDataleft-6;i+=2)
  {
    if(BlackLeft[i][1]>BlackLeft[i+2][1]&&BlackLeft[i+1][1]>BlackLeft[i+3][1]&&BlackLeft[i+2][1]>BlackLeft[i+4][1]&&BlackLeft[i+3][1]>BlackLeft[i+5][1]&&BlackLeft[i+4][1]>BlackLeft[i+6][1])	//NAODAIZHUANBUDONGLEA CHUNDEYIPI
    {
      
      if(youxian_L==0)   //看谁的趋势最先出现
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
    zongtiqushi=1;							//总体趋势向右
  }
  if(L_right==0&&L_left==1&&R_right==0&&R_left==1)
  {
    zongtiqushi=2;							//总体趋势向左
  }
  if(L_right==1&&L_left==1)
  {
    if(youxian_L==1)							//左边界有向左的趋势也有向右的趋势
    {
      left_daihao=1;						//先向右，再向左
    }else
    {
      left_daihao=2;					//先向左，再向右
    }
  }
  
  if(R_right==1&&R_left==1)
  {
    if(youxian_R==1)							//右边界有向左的趋势也有向右的趋势
    {
      right_daihao=1;					//先向右，再向左
    }else
    {
      right_daihao=2;					//先向左，再向右
    }
  }
  return 0;
}

/*==================================================================================
函数名称：zhidaopanduan
函数功能: 直道判断
时间：2019年7月6日00点07分
参数：
返回值： 1  是长直道   0不是长直道
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
  return 1;     //是长直道
}


/*==================================================================================
函数名称：zhongxianqueren
函数功能: 中线判断，在断路时算出的中线与一边补线算出来的中线进行对比
时间：2019年7月7日17点24分
参数：
返回值： 
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
          Center_T[i][1]=BlackRight[i][1]-(int)(saidaokuanyiban-(i/right_k));  //根据直线的普通畸变率 -(i/right_k) +(i/right_k)
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
函数名称：zhongxianqueren
函数功能: 中线判断，在断路时算出的中线与一边补线算出来的中线进行对比
时间：2019年7月7日17点24分
参数：
返回值： 
===================================================================================*/
int duanluzhengchangsaidaojince()
{
  int lc1=0,rc1=0;
  int lc2=0,rc2=0;
  
  
  ///抗反光的的检测回到真唱赛道的程序
  
  
  
  ///
  //one_Border_Scan(uchar r,uchar *lc,uchar *rc);               //验证这两个值的
//  one_Border_Scan(59,&lc1,&rc1);
//  one_Border_Scan(59,&lc2,&rc2);
  if(rc1-lc1>100&&abs(rc1-rc2<3)&&abs(lc1-lc2<3))
  {
    return 1;
  }
  return 0;
}

uchar diaoyong_flag2=0;
uchar error_beg=0;    //按键按下开始计算偏差标志位
int left_border_F=0;    //右边界最左列 的下标

void vision_algorithm()
{
  
  init(); 
  
  Three_Border_Scan();
  
  BorderScan();
  
  
  
  xielvfangcha();
  
  
  if(bizhang_F==0&&duanlu_F==0)   
  {
    banmaxian();        //斑马线
    
    Type_Judgement();   //类型识别
    
    sanxianqushipanduan();      //趋势判断
    
    lianxuwandao_F=lianxuwandao();      //连续弯道标志位
    zhidaopanduan_F=zhidaopanduan();    //直道标志位
    //int center_L=0,center_R=0;
    center_L_R(&center_L,&center_R);     //存储中线最大左右列的数组下标
    
    //shujucunchu();      //保存数据     开辟内存空间有BUG  可能与芯片有关也可能是程序BUG   1、指针为NULL时但是其地址不为空
    
    protect();  //出界保护
    
    if(IsDown_flag==1)   //按键按下开始计算偏差
    {
      error_beg=1;
    }
    
    if(error_beg)
    {
      imgbefor();         //上一帧中线所在位置   储存在center35lie
      
      //p_rintf();
      
      out_error();
      
      suo();
      
      credible();
      
      SpeedControl();
      
      //speed_control();          //速度控制
      
      bizhangjiance();          //障碍物检测
      
     //duanlujiance();
      
      
    }
  }else if(bizhang_F==1)              //执行避障
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


//2019年5月5日21:01:12