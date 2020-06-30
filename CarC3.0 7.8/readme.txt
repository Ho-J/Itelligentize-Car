#2019-5-9 将并口的LCD_ST7735R移植到串口的LCDST7735S(基于RightNow_chaser_(void)工程)
移植者：曾艺煌
//LCD_ST7735S 与 LCD_ST7735R 外观一样，寄存器操作略有不同

步骤1：修改使用的显示屏
修改位置1：APP/MK60_conf.h
#define USE_LCD        LCD_ST7735R            //选择使用的 LCD
修改为
#define USE_LCD        LCD_ST7735S            //选择使用的 LCD

步骤2：将显示屏初始化添加到 系统初始化函数 中
位置：App/init.c/System_Init函数
添加内容：LCD_init();//LCD初始化

步骤3：全部编译一遍后，
跳到LCD_init函数的定义处，在函数体内找到LCD_INIT函数，进而跳到它的定义处，继续跳到被宏替换的LCD_ST7735S_init函数的定义处
在它的函数体中
注释掉以下语句：（快捷键：选中内容后Ctrl + K）
    uint8 n ;

    for(n = 0; n < 8; n++)
    {
        gpio_init  ((PTXn_e)(PTC0 + n), GPO, 0);
    }

步骤4：修改引脚（打开工程文件夹下的1 SCH文件夹下的car.pdf原理图）
找对应的显示屏ST7735S的引脚：
Lcd SCL引脚对应库文件中LCD_ST7735S_WR引脚
MOSI(SDA)引脚对应库文件LCD_ST7735S_RD引脚
CS引脚对应库文件中     LCD_ST7735S_CS引脚
DC引脚对应库文件中     LCD_ST7735S_RS引脚
RES引脚对应库文件中    LCD_ST7735S_RST引脚

至于背光引脚可要可加可不加（看最后测试是否有背光，没有再取消注释）
//#define     LCD_ST7735S_BL      PTC2    //背光

接着返回VCAN_LCD_ST7735S.h头文件修改引脚：

修改位置：Board/VCAN_LCD_ST7735S.h
//LCD 的管脚定义
#define     LCD_ST7735S_WR      PTC9
#define     LCD_ST7735S_RD      PTC10
#define     LCD_ST7735S_CS      PTC11
#define     LCD_ST7735S_RS      PTC12
#define     LCD_ST7735S_RST     PTC13
修改为
//LCD 的管脚定义
#define     LCD_ST7735S_WR      PTB21   //Lcd SCL
#define     LCD_ST7735S_RD      PTB22   //MOSI(SDA)
#define     LCD_ST7735S_CS      PTC1    //CS
#define     LCD_ST7735S_RS      PTC0    //DC
#define     LCD_ST7735S_RST     PTB23   //RES

//未完待续 时间：2019-5-9 22点45分

步骤5：将并口LCD读/写函数修改成串口的
修改位置：Board/VCAN_LCDST7735S.c的LCD_ST7735S_wr_data函数体
修改内容1：
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
修改为：
//写数据
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

修改内容2：
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
修改为：

//写命令
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

步骤6：修改宏定义
修改位置：Board/VCAN_LCDST7735S.h
修改内容1：
#define LCD_ST7735S_WR_8CMD(cmd)    do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=0;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0=(uint8)(cmd);\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;产生一个上升沿

#define LCD_ST7735S_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=1;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0=(uint8)(data);\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;在这里写入数据到RAM


#define LCD_ST7735S_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((uint8)((data)>>8) );LCD_ST7735S_WR_8DATA( (uint8)(data) );}while(0)       //写数据

修改为：
注释掉上面的宏定义

修改内容2：
//#define LCD_WR_8DATA(data)      LCD_ST7735S_wr_data(data)       //写数据
//#define LCD_WR_8CMD(cmd)        LCD_ST7735S_wr_cmd(cmd)         //命令
修改为：
#define LCD_ST7735S_WR_8DATA(data)      LCD_ST7735S_wr_data(data)       //写数据
#define LCD_ST7735S_WR_8CMD(cmd)        LCD_ST7735S_wr_cmd(cmd)         //写命令


移植结束时间：2019-5-11 21点42分


//2019-5-24 测试龙邱521线mini编码器（迷你增量式 兼容正交解码）可以在淘宝找到属性
1 看原理图：car.pdf
由原理图可知P6的座子中
LSB 2接入隔离排阻，由LSB2接出后接芯片的PTB18引脚
DIR 2接入隔离排阻，由DIR2接出后接芯片的PTB19引脚

2 修改程序中的引脚：
路径：App\Inc\PORT_cfg.h
//正交解码模块通道  端口          可选范围              建议
#define FTM2_QDPHA  PTB18       //PTA10、PTB18
#define FTM2_QDPHB  PTB19       //PTA11、PTB19

3 在山外库文件找正交解码模块怎么使用：
路径：Chip\src\MK60_ftm.c
1)初始化FTM 的正交解码 功能
2)获取FTM 正交解码 的脉冲数
3)清 FTM 正交解码 的脉冲数

4 初始化FTM 的正交解码 功能
路径：App\Inc\Init.c
在系统初始化函数中调用 初始化FTM 的正交解码 功能
调用方式：
void System_Init(void)
{	 
  FTM_QUAD_Init(FTM2); //初始化 FTM2 为正交解码模式
}

5 定义存一个储脉冲数的全局变量
路径：App\main.c
在主函数外面定义变量：int16 count=0;//存储脉冲数

6 获取FTM 正交解码 的脉冲数
路径：App\main.c
在主函数的while(1)的循环体里面调用 获取FTM 正交解码 的脉冲数
调用方式：
void  main(void)
{
  
  System_Init();//系统初始化

  while(1)
  {  
      count = FTM_QUAD_get(FTM2);// //获取  FTM2 交解码 的脉冲数
  }
}

##到此就可以测试是否可以获取编码器的脉冲数了
#方法一：下载程序并仿真（Download and Debug），全速运行，是编码器旋转进入仿真查看count是否有值；
#方法二：用主板上的：LCD显示屏将count的值显示出来。

7 清 FTM 正交解码 的脉冲数
路径：App\main.c
在主函数的while(1)的循环体里面调用 获取FTM 正交解码 的脉冲数之后，再调用 清 FTM 正交解码 的脉冲数
void  main(void)
{
  
  System_Init();//系统初始化

  while(1)
  {  
    
      count = FTM_QUAD_get(FTM2);
      FTM_QUAD_clean(FTM2);
  }
}

//2019-6-6 曾艺煌
0 由RightNow_chaser_CarC_Encoder工程文件修改

1 在main.c中包含GUI.h

2 在init.c中添加编码器引脚的宏定义

#define LSB1 PTA12
#define DIR1 PTA13
#define LSB2 PTB18
#define DIR2 PTB19

2-1 添加电机转速闭环的PID参数结构体
PID Var_PID_motor_L;
PID *PID_motor_L=&Var_PID_motor_L;
PID Var_PID_motor_R;
PID *PID_motor_R=&Var_PID_motor_R;

2-2 添加接收编码器脉冲数的变量
int16  CntEncoder_left;
int16  CntEncoder_right;

3 且在System_Init函数中加入
  pit_init_ms(PIT1, 10);                                
  set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);      
  enable_irq (PIT1_IRQn); 

3-1 添加电机初始化函数，并在System_Init函数中调用，调用前在inic.h中声明
/* 
功能：电机初始化
使能引脚初始化gpio.c(（端口，引脚信号方向（即芯片引脚读取、输出信号），输出初始状态)
通道口初始化：chip/ftm.c(频率为电机型号所决定)
*/
void motor_init(void)
{
  FTM_PWM_init(FTM0, FTM_CH4,10*1000, 0);//T（正转）
  FTM_PWM_init(FTM0, FTM_CH5,10*1000, 0);//F
  FTM_PWM_init(FTM0, FTM_CH6,10*1000, 0);//F
  FTM_PWM_init(FTM0, FTM_CH7,10*1000, 0);//T（正转）
  gpio_init (PTB0, GPO,1);//电机使能
}

3-2 添加编码器脉冲读数初始化子函数，并在System_Init函数中调用，调用前在inic.h中声明
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

3-3 添加PID_init函数，初始化左右电机PID结构体
  PID_init(PID_motor_L);//左电机PID结构体
  PID_init(PID_motor_R);//右电机PID结构体
  
3-4 设定左右电机P、I、D初始值
  PID_kp(PID_motor_L,Motor_P);
  PID_ki(PID_motor_L,Motor_I);
  PID_kd(PID_motor_L,Motor_D);
  PID_kp(PID_motor_R,Motor_P);
  PID_ki(PID_motor_R,Motor_I);
  PID_kd(PID_motor_R,Motor_D);
  
4 添加PIT0的内容,在这之前要先定义一个电机控制函数
4-1 定义电机控制函数
void motor_Ctrl(void)
{
  int Speed;
 //停车||定时起步 
  
  if(RunState_flag&& protect_flag==0)//与上protect_flag  当protect_flag为0时判断车在赛道内  否则为protect_flag=1
  Speed=Encoder_set;//定时起步
  else
  {
    Speed=0;//停车
    LRT_flag=0;
  }
  
  PID_set(PID_motor_L,Speed,CntEncoder_left);//计算PID偏差  
  PWM_duty_deta_L+=(int32)PID_Calc(PID_motor_L);//累加占空比（增量式PID计算）
  if(PWM_duty_deta_L>10000)//占空比限幅
  {
    PWM_duty_deta_L=10000;
  }
  if(PWM_duty_deta_L<-10000) 
  {
    PWM_duty_deta_L=-10000;
  }
  
  PID_set(PID_motor_R,Speed,CntEncoder_right);//计算PID偏差
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
  
  if(key_check(KEY_Centre)==KEY_DOWN)
  {
    IsDown_flag=1;//开启开车倒计时标志位
    IsBeep=1;     //蜂鸣器响提示开始倒计时
  }
  
}
4-2 在PIT0定时器中断的函数体中调用电机控制函数
int PWM_duty_deta_L=0;//左电机占空比分子累加值
int PWM_duty_deta_R=0;//右电机占空比分子累加值
void PIT0_IRQHandler(void) 
{  

  CntEncoder_left=Encoder_read_L();//获取左右编码器实时脉
  CntEncoder_right=Encoder_read_R();//获取左右编码器实时脉
 // Encoder_inc=CntEncoder_right+Encoder_inc;
     motor_Ctrl();//电机控制
  //Send_16bitData(Encoder_set,CntEncoder_right,PWM_duty_deta_R);//看电机响应必须在中断里面发送波形，否则波形不平滑，且横轴时间间隔也不一样
  PIT_Flag_Clear(PIT0);       //清中断标志位
  
}

5 添加PIT1的内容（记得在init.h中声明，void PIT1_IRQHandler(void);）
int time=0;//进中断次数
uint8 Timer_n=0;//计数变量
int Image_num=0;//图像计数（100ms）
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
      RunState_flag=1;//按键按下200*10ms后进入运行状态
      IsDown_flag=0;//清除按键标志
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
  
  PIT_Flag_Clear(PIT1);       //清中断标志位
}
6 检查控制电机的占空比分子的精度，路径：Chip/MK60_ftm.c/ ->MK60_FTM.h
控制电机的FTM模块：FTM0的占空比精度： #define FTM0_PRECISON 10000u

6-1 检查FTM0的各通道（ FTM_CH4 ~ FTM_CH7）对应电机的转向;如何测试？有讲究的

7检查PID计算的限幅

8 给电机的P、I、D赋值，之前调试P=30，I=9；
int Motor_P=30;
int Motor_I=9;
int Motor_D=0;

9 将采样周期设定的语句封装成子函数，并在System_Init函数中调用，调用前在inic.h中声明
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
10 需求分析：在系统初始化时添加蜂鸣器做保障，后续加显示屏显示，为了知道所有的初始化是否正常，不正常卡在哪一步；
   每做一个初始化，显示屏显示xxxinit... 若卡在某一步就能清晰知道哪个初始化失败。
10-1  做了一些系统初始化语句的顺序优化
10-2  在系统初始化函数体的开始（蜂鸣器初始化语句之后）加蜂鸣器响提示系统初始化开始，
      函数体的最后一句加蜂鸣器关提示系统初始化结束，如
  BeepInit();//蜂鸣器初始化
  BeepON(); //蜂鸣器响提示系统初始化开始
  ...
  ...
  ...
  BeepOFF(); //蜂鸣器关提示系统初始化结束
10-3 显示屏显示初始化到哪一步

11测试蜂鸣器消息机制

11-1首先在10ms定时器中断中调用Beep函数

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

12 在系统初始化函数体内设置中断优先级
防止初始化时就卡了

13 加入电机堵转保护
13-1 定义全局变量
int LRT_flag=0;//开始检测电机堵转标志位
13-2 因为起步时的数据跟堵转的数据类似，在定时器PIT1中断的函数体内
     加入一条语句作为开始检测堵转的标志位,使起步时的数据跟堵转的数
     据因为时间的错开而不会误识别。
  if(RunState_flag==1)
  {

    if(++time==300)//300*10ms
    {
      LRT_flag=1;//开车后倒计时3秒，开始检测电机是否有堵转
    }
    else;
    
  }

//2019.6.8  路程计算
路径：Init.h
添加
typedef struct sv{              //存 路程 速度  累计脉冲数 的值
  float S;                //路程 单位  毫米  mm
  float V;                //速度   m/s
  int Add_CntEncoder;   //累计脉冲数的值
}SV;
extern SV * sv_L;        //留出 S V 接口
extern SV * sv_R;        //留出 S V 接口

在PIT0定时器中 添加  累计编码器脉冲数的语句  路程计算的语句（计算路程需要测试车模走过一米编码器累加的脉冲数） 速度计算语句 

语句块：
SV sv_l;
SV sv_r;

SV * sv_L=&sv_l;   // 根据左编码器 计算出来的 路程 速度  累计脉冲数 

SV * sv_R=&sv_r;    // 根据右编码器 计算出来的 路程 速度  累计脉冲数  
void PIT0_IRQHandler(void) 
{  

  CntEncoder_left=Encoder_read_L();//获取左右编码器实时脉
  CntEncoder_right=Encoder_read_R();//获取左右编码器实时脉
  
  sv_L->Add_CntEncoder=CntEncoder_left+sv_L->Add_CntEncoder;     //脉冲数累计值
  sv_L->S=(sv_L->Add_CntEncoder)*1.0/4300;                        //走的路程  单位m
  sv_L->V=(CntEncoder_left*1.0/4300)/10;                       //速度  单位 m/ms
  
  sv_R->Add_CntEncoder=CntEncoder_right+sv_R->Add_CntEncoder;     //脉冲数累计值
  sv_R->S=(sv_R->Add_CntEncoder)*1.0/4300;                        //走的路程  单位m
  sv_R->V=(CntEncoder_right*1.0/4300)/10;                       //速度  单位 m/ms
  
  // Encoder_inc=CntEncoder_right+Encoder_inc;
  motor_Ctrl();//电机控制
  //Send_16bitData(Encoder_set,CntEncoder_right,PWM_duty_deta_R);//看电机响应必须在中断里面发送波形，否则波形不平滑，且横轴时间间隔也不一样
  PIT_Flag_Clear(PIT0);       //清中断标志位
  
}



7、增加一个 定时器变量加数 
位置：Init.c void PIT1_IRQHandler(void)

uint8 Timer_n=0;//计数变量
int Image_num=0;//图像计数（100ms）    100毫秒加一
uint8 image_num_kaiqi=0;    //    image_num_kaiqi  为 1 开始  Image_num 的计数
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
      RunState_flag=1;//按键按下200*10ms后进入运行状态
      IsDown_flag=0;//清除按键标志
      BeepOFF(); 
    }    
  }
  Image_num  为外部接口
  

1-1 修改舵机驱动频率50Hz->75Hz
 FTM_PWM_init(FTM1,FTM_CH1,75,ServoCentreValue);//舵机初始化，驱动频率7
1-2 修改舵机中值750->1128
 int ServoCentreValue=1128;    //舵机中值(1.5ms/((1/75)x1000)x10000
 
 2019.06.11
 摄像头阈值不更新问题
 问题描述：在菜单中循环调整变量时需要选到阈值那里才会更新设定的阈值，其他时候为上次设定阈值
 解决方法：系统初始化中，需将摄像头初始化放在最前面，先初始化所有的摄像头再进行flash的读写即可做到实时更新
 将 camera_all_init();放在init.c的中System_Init()里最前面
 
 
程序周期   90ms    GUI 41ms   解压0+ms   除去压缩与GUI 50ms
 

//2019-6-21 获取激光测距字符串测试 曾艺煌
定义全局变量数组char distance_laser[50];


2019年6月23日01点37分

init.c  增加一个变量
extern int Laser_Value;   //激光测到的距离  Laser_Value   单位：mm
init.h  声明此接口


