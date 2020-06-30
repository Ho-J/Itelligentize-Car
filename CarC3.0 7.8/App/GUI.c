#include "GUI.h"
#include "PID.h"
#include "VAR.h"
#include "image.h"
#include "Init.h"
#include "elec.h"

/*
3019.3.30l
��ɫ��ʾ���⣺����ʾģ���װ�ɺ������ú��������ɫΪ��ɫ
����ԭ����ɫ��������Ϊ16���ƣ����β�����Ϊuint8,���¸߰�λֵ��ʧ
���������uint8 ��Ϊ uint16
*/
typedef struct
{
  uint8 *Str;           //��������
  uint8 Ischecked;      //�Ƿ�ѡ�б�ɫ��־
  //uint8 Isflash;        //�Ƿ���Ҫд��flash
  int value;            //����ֵ
  int limit;          //����ֵ
  int floor;          //����ֵ
  uint8 size;           //���ε�������
  uint8 str[2];         //�ַ�λ��         
  uint8 val[2];         //��ֵλ��
  //uint32 color;         //������ɫ
  //uint32 bkcolor;       //������ɫ
}MENU_VAR;

MENU_VAR Menu_Var[Num]; 


int po=0;
extern uint32 time_1;
//=============================�����ڲ�����=====================================//
unsigned char Image_lcd_num;//wj��Ҫ��ʾ��ֵ
unsigned char GUI_flag = 1;     //���Խ������
int state;                                  //���Ʊ���λ
int size_P=1;
static Site_t SiteInit = {0, 0};                              //��ʼ�������ʾλ��
uint8 xx=3,yy=60;                                              //�У���  
uint8 *String[Num] = {"Sp","Sd","V","T","Mp","Mi","Md","Sc","Pt"};               //�����ַ�  

//int *Value_T[Num] = {&Servo_P,&Servo_D,&Encoder_set,&THOD,&Motor_P_L,&Motor_I_L,&Motor_D_L,&ServoCentreValue,&size_P};     //���������ĵ�ַ

int *Value_T[Num] = {&Servo_P,&Servo_D,&Encoder_set,&THOD,&chasu_P,&po,&chasu_D,&po,&size_P};     //���������ĵ�ַ


char GUI_2_L_F=0;               //�ڶ���GUI���� ��������ı�־λ �����ڴ�GUI��ʾ����ͼ��
uint8 Run_Model=0;//����ģʽ��Ĭ��ͼ�����л���
//=============================�����ڲ�����End=====================================//


//=============================���屾ҳ�ĺ���===================================//

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
  for(uint8 i=0;i<Num;i++)          //�˵���ʼ��
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
    else*///���б���������Ч
    if(i==8)
      Menu_Var[i].size  = 1;
    else
      Menu_Var[i].size  = size_P;
    //Menu_Var[i].color = RED;
    //Menu_Var[i].bkcolor = YELLOW;
  }
  //��ʼ������λ��
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

void flag_clean(uint8 num)          //�����־λ
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
�β�Button_val���⣺����������255���Լ�С��0ʱ�����޷�Ԥ֪�Ĵ���
����ԭ��uint8����Ϊ�޷��������ֵ255��������ʱ�ڼ��޴������ֵ������Χ�����
���������uint8 ��Ϊ int
*/
int Button_Set(int Button_val,int *addr,uint8 adjust_size,int limit,int floor)     //�����������ٶ����ã�����
{
  uint8 Iswrite=0;
  if(key_check(KEY_U)==KEY_DOWN)                                
  {
    Iswrite=1;
    Button_val += adjust_size;              
    if(Button_val >= limit)                //�޷�
      Button_val -= adjust_size;
  }
  if(key_check(KEY_D)==KEY_DOWN)   
  {     
    Iswrite=1;
    Button_val -= adjust_size;              
    if(Button_val <= floor)                  //�޷�
      Button_val = floor;
  }
  *addr=Button_val;
  if(addr==&THOD)
    SCCB_WriteByte(OV7725_CNST,*addr);       //д����ֵ�Ĵ���
  if(Iswrite)
  {
    flash_erase_sector(FLASH_SECTOR_NUM-1-state); //�������
    flash_write(FLASH_SECTOR_NUM-1-state,0,*addr); //д��flash
  }
  return Button_val;
}
//��ʾ�߽�
void DisplayBlack(void)
{
  
  uint8 Ln=0;
  uint8 i=0;
  Site_t   Boundary;//�߽�

  for(Ln=59;Ln>=2&&59-Ln<NumbDataleft;Ln--)          //10-59�д���
  {
    Boundary.y=Ln;
    Boundary.x=(int)(Center[i][1]*0.8);                    //��ӡ���ߵ�
    LCD_point(Boundary,GREEN);                         //��ʾ��ߵĵ�
    Boundary.x=(int)(BlackLeft[i][1]*0.8);                   //��ӡ�����
    LCD_point(Boundary,RED);
    Boundary.x=(int)(BlackRight[i][1]*0.8-1);                  //��ӡ�ұ���
    LCD_point(Boundary,BLUE); 
    i++;
    
  }
  for(int i=0;i<159;i++)
  {
    Boundary.y=12;
    Boundary.x=i;                    //��ӡ���ߵ�
    LCD_point(Boundary,GREEN);  
  }
  for(int i=0;i<59;i++)
  {
    Boundary.y=i;
    Boundary.x=(int)(80*0.8);                    //��ӡ���ߵ�
    LCD_point(Boundary,GREEN);  
  }
  
  
}
//�βηֱ�Ϊ����ѡ��ʾ���������ַ�x��y���꣬��ֵx,y���꣬��ʾ��ֵ��������ɫ��������ɫ����ʾ�ַ�
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
  
  if(val<1000)         //��ֵ����С��10
  {
      if(val<100)         //��ֵ����С��10
      {
        if(val<10)
        {
          SiteInit.x=Xval+Width;
          LCD_str(SiteInit," ",color,BKcolor);   //��������
        }
        SiteInit.x=Xval+Width*2;
        LCD_str(SiteInit," ",color,BKcolor);   //��������
      }
      SiteInit.x=Xval+Width*3;
      LCD_str(SiteInit," ",color,BKcolor);   //��������
  }
  SiteInit.x=Xval;
  LCD_num(SiteInit,val,color,BKcolor);     //OLED��ʾ
}

void GUI(void)
{
  if(key_check(KEY_Centre)==KEY_DOWN)
 {
    Site_t site = {0,0};  
    Size_t size = {127,127};
    LCD_rectangle(site, size, BCOLOUR);
    GUI_flag++;
    if(GUI_flag>2)                    //���ý���ҳ��
    GUI_flag=0;
  }
  
  //#2ͨ��LCD��ʾͼ��
  Site_t site     = {0, 0};                              //��ʾͼ�����Ͻ�λ��  
  Size_t size     = {LCD_W,CAMERA_H/2};               //ͼ���С
  Size_t imgsize  = {CAMERA_W,CAMERA_H/2};  
  LCD_Img_Binary_Z(site, size, ImageBuff, imgsize);	       //ʹ����ʾ����ʾͼ��
  DisplayBlack();//��ʾ�߽�
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
    
   // print(99,0,0,0+Width,0,state,RED,YELLOW,"N");//�޸ı��������
  
    print(0,xx,yy+Height,Width*2+2+xx,yy+Height,Menu_Var[0].value,RED,YELLOW,Menu_Var[0].Str);//���Pֵ
  
    print(1,Width*5+4+xx,yy+Height,Width*7+6+xx,yy+Height,Menu_Var[1].value,RED,YELLOW,Menu_Var[1].Str);//���Dֵ
    
    print(2,Width*10+8+xx,yy+Height,Width*11+10+xx,yy+Height,Menu_Var[2].value,RED,YELLOW,Menu_Var[2].Str);//�趨�ٶ�
    
    print(3,xx,yy+Height+Height,Width+2+xx,yy+Height+Height,Menu_Var[3].value,RED,YELLOW,Menu_Var[3].Str);//��ֵ
    
    print(4,Width*4+4+xx,yy+Height+Height,Width*6+6+xx,yy+Height+Height,Menu_Var[4].value,RED,YELLOW,Menu_Var[4].Str);//���Pֵ
    
    print(5,Width*9+8+xx,yy+Height+Height,Width*11+10+xx,yy+Height+Height,Menu_Var[5].value,RED,YELLOW,Menu_Var[5].Str);//���Iֵ
    
    print(6,xx,yy+Height+Height+Height,Width*2+xx+2,yy+Height+Height+Height,Menu_Var[6].value,RED,YELLOW,Menu_Var[6].Str);//���Dֵ
    
    print(8,Width*11+xx+8,yy+Height+Height+Height,Width*13+xx+10,yy+Height+Height+Height,Menu_Var[8].value,RED,YELLOW,Menu_Var[8].Str);//��������
    
    print(7,Width*5+xx+4,yy+Height+Height+Height,Width*7+xx+6,yy+Height+Height+Height,Menu_Var[7].value,RED,YELLOW,Menu_Var[7].Str);//�����ֵ
   //print(7,Width*5+xx+4,yy+Height+Height+Height,Width*7+xx+6,yy+Height+Height+Height,Laser_Value,RED,YELLOW,Menu_Var[7].Str);//�����ֵ
    print(7,Width*5+xx+4,yy+Height+Height+Height,Width*7+xx+6,yy+Height+Height+Height,sv_L->Add_CntEncoder,RED,YELLOW,Menu_Var[7].Str);//�����ֵ
    //print(7,Width*5+xx+4,yy+Height+Height+Height,Width*7+xx+6,yy+Height+Height+Height,shijan/10,RED,YELLOW,Menu_Var[7].Str);//�����ֵ
    //��ͨ������������ʼ��ֱ�ӷ���λ��
    /*for(int i=0;i<Num_now;i++)
    {
    print(i,Menu_Var[i].str[0],Menu_Var[i].str[1],Menu_Var[i].val[0],Menu_Var[i].val[1],Menu_Var[i].value,Menu_Var[i].color,Menu_Var[i].bkcolor,Menu_Var[i].Str);
    }*/
    
    
  }
  
 else if(GUI_flag==0)
 {
   if(key_check(KEY_L)==KEY_DOWN)
   {
     IsDown_flag=1;//������������ʱ��־λ
     IsBeep=1;     //����������ʾ��ʼ����ʱ
     //GUI_2_L_F++;
   }
   if(key_check(KEY_R)==KEY_DOWN)
   {
     Send_Model=~Send_Model;
   }
   
   print(99,128-Width*2,0,128-Width,0,Image_lcd_num,RED,YELLOW,"A");//wj��Ҫ���Ͻ���ʾ��ֵ
   
   print(99,xx,yy,Width*2+xx+2,yy,kuan,RED,YELLOW,"Pw");//��ѹֵ
   
   print(99,Width*5+xx+4,yy,Width*6+xx+6,yy,error,RED,YELLOW,"E");//ͼ���ƫ��ֵ
    
    print(99,Width*10+xx+8,yy,Width*11+xx+10,yy,Send_Model,RED,YELLOW,"M");//��������ģʽ
    
    print(99,Width*5+xx+4,yy+Height+Height+Height,Width*7+xx+6,yy+Height+Height+Height,valuejuli,RED,YELLOW,"X");
    
    print(99,Width*5+4+xx,yy+Height,Width*7+6+xx,yy+Height,elec_error,RED,YELLOW,"Ee");//���ֵƫ��
   
   print(99,xx,yy+Height+Height,Width+2+xx,yy+Height+Height,elec_L,RED,YELLOW,"L");//����ֵ
   
   print(99,Width*9+8+xx,yy+Height+Height,Width*10+10+xx,yy+Height+Height,elec_R,RED,YELLOW,"R");//�ҵ��ֵ
   
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
    
    
    
    print(99,xx,yy+Height+Height,Width+2+xx,yy+Height+Height,Run_Model,RED,YELLOW,"M");//��ֵ
  }
}
