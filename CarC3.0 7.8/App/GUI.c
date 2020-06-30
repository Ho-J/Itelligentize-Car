#include "GUI.h"
#include "PID.h"
#include "VAR.h"
#include "image.h"
#include "Init.h"
#include "elec.h"

/*
3019.3.30l
颜色显示问题：将显示模块封装成函数调用后，输出的颜色为灰色
问题原因：颜色储存类型为16进制，而形参类型为uint8,导致高八位值丢失
解决方法：uint8 改为 uint16
*/
typedef struct
{
  uint8 *Str;           //所调变量
  uint8 Ischecked;      //是否被选中变色标志
  //uint8 Isflash;        //是否需要写入flash
  int value;            //变量值
  int limit;          //上限值
  int floor;          //下限值
  uint8 size;           //单次调整幅度
  uint8 str[2];         //字符位置         
  uint8 val[2];         //数值位置
  //uint32 color;         //字体颜色
  //uint32 bkcolor;       //背景颜色
}MENU_VAR;

MENU_VAR Menu_Var[Num]; 


int po=0;
extern uint32 time_1;
//=============================定义内部变量=====================================//
unsigned char Image_lcd_num;//wj需要显示数值
unsigned char GUI_flag = 1;     //调试界面变量
int state;                                  //控制变量位
int size_P=1;
static Site_t SiteInit = {0, 0};                              //初始化检测显示位置
uint8 xx=3,yy=60;                                              //列，行  
uint8 *String[Num] = {"Sp","Sd","V","T","Mp","Mi","Md","Sc","Pt"};               //变量字符  

//int *Value_T[Num] = {&Servo_P,&Servo_D,&Encoder_set,&THOD,&Motor_P_L,&Motor_I_L,&Motor_D_L,&ServoCentreValue,&size_P};     //所调变量的地址

int *Value_T[Num] = {&Servo_P,&Servo_D,&Encoder_set,&THOD,&chasu_P,&po,&chasu_D,&po,&size_P};     //所调变量的地址


char GUI_2_L_F=0;               //第二个GUI界面 按下左键的标志位 ，用于打开GUI显示赛道图像
uint8 Run_Model=0;//运行模式，默认图像电磁切换跑
//=============================定义内部变量End=====================================//


//=============================定义本页的函数===================================//

void Menu_Var_init(void)
{
  state = 0;
  Menu_Var[state].Ischecked = 1;
  /*Menu_Var[0].value = Encoder_set;
  Menu_Var[1].value = Servo_P;
  Menu_Var[2].value = Servo_D;
  Menu_Var[3].value = THOD;
  Menu_Var[4].value = ServoCentreValue;
  Menu_Var[5].value = Motor_P;
  Menu_Var[6].value = Motor_I;
  Menu_Var[7].value = Motor_D;*/
  for(uint8 i=0;i<Num;i++)          //菜单初始化
  {
    Menu_Var[i].value = *Value_T[i];
    Menu_Var[i].Str = String[i];
    if(i==7)
    {
      Menu_Var[i].limit = ServoCentreValue+450;
      Menu_Var[i].floor = ServoCentreValue-450;
    }
    else if(i==4 || i==5)
    {
      Menu_Var[i].limit = 999;
    }
    else
    {
      Menu_Var[i].limit = 255;
      Menu_Var[i].floor = 0;
    }
    /*if(i==5 || i==6 || i==4 || i==8)
      Menu_Var[i].size  = 1;
    else if(i==3 || i==2)
      Menu_Var[i].size  = 5;
    else*///所有变量步幅生效
    if(i==8)
      Menu_Var[i].size  = 1;
    else
      Menu_Var[i].size  = size_P;
    //Menu_Var[i].color = RED;
    //Menu_Var[i].bkcolor = YELLOW;
  }
  //初始化分配位置
  /*for(uint8 i=0;i<Num_now;i++)
  {
  Menu_Var[i].str[0]=width;
  Menu_Var[i].str[1]=yy;
  width+=Width*3+10;
}
  for(uint8 i=0;i<Num_now;i++)
  {
  Menu_Var[i].val[0]=Menu_Var[i].str[0]+Width*2;
  Menu_Var[i].val[1]=yy;
}*/
}

void flag_clean(uint8 num)          //清理标志位
{
  for(uint8 i=0;i<Num;i++)
  {
    if(i != num)
      Menu_Var[i].Ischecked = 0;
    else 
      Menu_Var[i].Ischecked = 1;
  }
}

/*
2019.3.30
形参Button_val问题：在自增大于255，自减小于0时出现无法预知的错误
问题原因：uint8类型为无符号且最大值255，自增减时在极限处会出现值超出范围的情况
解决方法：uint8 改为 int
*/
int Button_Set(int Button_val,int *addr,uint8 adjust_size,int limit,int floor)     //按键计数（速度设置）函数
{
  uint8 Iswrite=0;
  if(key_check(KEY_U)==KEY_DOWN)                                
  {
    Iswrite=1;
    Button_val += adjust_size;              
    if(Button_val >= limit)                //限幅
      Button_val -= adjust_size;
  }
  if(key_check(KEY_D)==KEY_DOWN)   
  {     
    Iswrite=1;
    Button_val -= adjust_size;              
    if(Button_val <= floor)                  //限幅
      Button_val = floor;
  }
  *addr=Button_val;
  if(addr==&THOD)
    SCCB_WriteByte(OV7725_CNST,*addr);       //写入阈值寄存器
  if(Iswrite)
  {
    flash_erase_sector(FLASH_SECTOR_NUM-1-state); //清楚扇区
    flash_write(FLASH_SECTOR_NUM-1-state,0,*addr); //写入flash
  }
  return Button_val;
}
//显示边界
void DisplayBlack(void)
{
  
  uint8 Ln=0;
  uint8 i=0;
  Site_t   Boundary;//边界

  for(Ln=59;Ln>=2&&59-Ln<NumbDataleft;Ln--)          //10-59行处理
  {
    Boundary.y=Ln;
    Boundary.x=(int)(Center[i][1]*0.8);                    //打印中线点
    LCD_point(Boundary,GREEN);                         //显示左边的点
    Boundary.x=(int)(BlackLeft[i][1]*0.8);                   //打印左边线
    LCD_point(Boundary,RED);
    Boundary.x=(int)(BlackRight[i][1]*0.8-1);                  //打印右边线
    LCD_point(Boundary,BLUE); 
    i++;
    
  }
  for(int i=0;i<159;i++)
  {
    Boundary.y=12;
    Boundary.x=i;                    //打印中线点
    LCD_point(Boundary,GREEN);  
  }
  for(int i=0;i<59;i++)
  {
    Boundary.y=i;
    Boundary.x=(int)(80*0.8);                    //打印中线点
    LCD_point(Boundary,GREEN);  
  }
  
  
}
//形参分别为：所选显示变量数，字符x，y坐标，数值x,y坐标，显示数值，字体颜色，背景颜色，显示字符
void print(uint8 num,uint8 Xstr,uint8 Ystr,uint8 Xval,uint8 Yval,int val,uint16 color,uint16 bkcolor,uint8 *str)
{
  uint16 BKcolor = bkcolor;
  if(num<Num && Menu_Var[num].Ischecked == 1)
    BKcolor = BLUE;
  SiteInit.x=Xstr;
  SiteInit.y=Ystr;
  LCD_str(SiteInit,str,color,BKcolor);
  SiteInit.x=Xval;
  SiteInit.y=Yval; 
  
  if(val<1000)         //数值调整小于10
  {
      if(val<100)         //数值调整小于10
      {
        if(val<10)
        {
          SiteInit.x=Xval+Width;
          LCD_str(SiteInit," ",color,BKcolor);   //消除残留
        }
        SiteInit.x=Xval+Width*2;
        LCD_str(SiteInit," ",color,BKcolor);   //消除残留
      }
      SiteInit.x=Xval+Width*3;
      LCD_str(SiteInit," ",color,BKcolor);   //消除残留
  }
  SiteInit.x=Xval;
  LCD_num(SiteInit,val,color,BKcolor);     //OLED显示
}

void GUI(void)
{
  if(key_check(KEY_Centre)==KEY_DOWN)
 {
    Site_t site = {0,0};  
    Size_t size = {127,127};
    LCD_rectangle(site, size, BCOLOUR);
    GUI_flag++;
    if(GUI_flag>2)                    //设置界面页数
    GUI_flag=0;
  }
  
  //#2通过LCD显示图像
  Site_t site     = {0, 0};                              //显示图像左上角位置  
  Size_t size     = {LCD_W,CAMERA_H/2};               //图像大小
  Size_t imgsize  = {CAMERA_W,CAMERA_H/2};  
  LCD_Img_Binary_Z(site, size, ImageBuff, imgsize);	       //使用显示屏显示图像
  DisplayBlack();//显示边界
//  
  
 if(GUI_flag==1)               
  {
    
    if(key_check(KEY_R)==KEY_DOWN)   
    {
      state++;
      if(state>Num-1)                
        state=0;
      flag_clean(state);
    }
    if(key_check(KEY_L)==KEY_DOWN)   
    {
      state--;
      if(state<0)                
        state=Num-1;
      flag_clean(state);
    }

    Menu_Var[state].value = Button_Set(Menu_Var[state].value,Value_T[state],Menu_Var[state].size,Menu_Var[state].limit,Menu_Var[state].floor);
    
   // print(99,0,0,0+Width,0,state,RED,YELLOW,"N");//修改变量的序号
  
    print(0,xx,yy+Height,Width*2+2+xx,yy+Height,Menu_Var[0].value,RED,YELLOW,Menu_Var[0].Str);//舵机P值
  
    print(1,Width*5+4+xx,yy+Height,Width*7+6+xx,yy+Height,Menu_Var[1].value,RED,YELLOW,Menu_Var[1].Str);//舵机D值
    
    print(2,Width*10+8+xx,yy+Height,Width*11+10+xx,yy+Height,Menu_Var[2].value,RED,YELLOW,Menu_Var[2].Str);//设定速度
    
    print(3,xx,yy+Height+Height,Width+2+xx,yy+Height+Height,Menu_Var[3].value,RED,YELLOW,Menu_Var[3].Str);//阈值
    
    print(4,Width*4+4+xx,yy+Height+Height,Width*6+6+xx,yy+Height+Height,Menu_Var[4].value,RED,YELLOW,Menu_Var[4].Str);//电机P值
    
    print(5,Width*9+8+xx,yy+Height+Height,Width*11+10+xx,yy+Height+Height,Menu_Var[5].value,RED,YELLOW,Menu_Var[5].Str);//电机I值
    
    print(6,xx,yy+Height+Height+Height,Width*2+xx+2,yy+Height+Height+Height,Menu_Var[6].value,RED,YELLOW,Menu_Var[6].Str);//电机D值
    
    print(8,Width*11+xx+8,yy+Height+Height+Height,Width*13+xx+10,yy+Height+Height+Height,Menu_Var[8].value,RED,YELLOW,Menu_Var[8].Str);//步伐调整
    
    print(7,Width*5+xx+4,yy+Height+Height+Height,Width*7+xx+6,yy+Height+Height+Height,Menu_Var[7].value,RED,YELLOW,Menu_Var[7].Str);//舵机中值
   //print(7,Width*5+xx+4,yy+Height+Height+Height,Width*7+xx+6,yy+Height+Height+Height,Laser_Value,RED,YELLOW,Menu_Var[7].Str);//舵机中值
    print(7,Width*5+xx+4,yy+Height+Height+Height,Width*7+xx+6,yy+Height+Height+Height,sv_L->Add_CntEncoder,RED,YELLOW,Menu_Var[7].Str);//舵机中值
    //print(7,Width*5+xx+4,yy+Height+Height+Height,Width*7+xx+6,yy+Height+Height+Height,shijan/10,RED,YELLOW,Menu_Var[7].Str);//舵机中值
    //欲通过数组批量初始化直接分配位置
    /*for(int i=0;i<Num_now;i++)
    {
    print(i,Menu_Var[i].str[0],Menu_Var[i].str[1],Menu_Var[i].val[0],Menu_Var[i].val[1],Menu_Var[i].value,Menu_Var[i].color,Menu_Var[i].bkcolor,Menu_Var[i].Str);
    }*/
    
    
  }
  
 else if(GUI_flag==0)
 {
   if(key_check(KEY_L)==KEY_DOWN)
   {
     IsDown_flag=1;//开启开车倒计时标志位
     IsBeep=1;     //蜂鸣器响提示开始倒计时
     //GUI_2_L_F++;
   }
   if(key_check(KEY_R)==KEY_DOWN)
   {
     Send_Model=~Send_Model;
   }
   
   print(99,128-Width*2,0,128-Width,0,Image_lcd_num,RED,YELLOW,"A");//wj需要左上角显示数值
   
   print(99,xx,yy,Width*2+xx+2,yy,kuan,RED,YELLOW,"Pw");//电压值
   
   print(99,Width*5+xx+4,yy,Width*6+xx+6,yy,error,RED,YELLOW,"E");//图像的偏差值
    
    print(99,Width*10+xx+8,yy,Width*11+xx+10,yy,Send_Model,RED,YELLOW,"M");//蓝牙发送模式
    
    print(99,Width*5+xx+4,yy+Height+Height+Height,Width*7+xx+6,yy+Height+Height+Height,valuejuli,RED,YELLOW,"X");
    
    print(99,Width*5+4+xx,yy+Height,Width*7+6+xx,yy+Height,elec_error,RED,YELLOW,"Ee");//电感值偏差
   
   print(99,xx,yy+Height+Height,Width+2+xx,yy+Height+Height,elec_L,RED,YELLOW,"L");//左电感值
   
   print(99,Width*9+8+xx,yy+Height+Height,Width*10+10+xx,yy+Height+Height,elec_R,RED,YELLOW,"R");//右电感值
   
   print(99,Width*5+4+xx,yy+Height+Height,Width*6+6+xx,yy+Height+Height,elec_M,RED,YELLOW,"C");//(int)chabihe
  }
  else 
  {
    if(key_check(KEY_U)==KEY_DOWN)   
    {
      if(++Run_Model>2)
        Run_Model=0;
    }
    if(key_check(KEY_D)==KEY_DOWN)   
    {
      if(Run_Model--==0)
        Run_Model=2;
    }
    
    
    
    print(99,xx,yy+Height+Height,Width+2+xx,yy+Height+Height,Run_Model,RED,YELLOW,"M");//阈值
  }
}
