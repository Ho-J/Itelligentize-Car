//斑马线识别



#include "common.h"
#include "include.h"
#include "VAR.h"
#include "image.h"
#include "PID.h"
#include "elec.h"
#include "GUI.h"
#include "Init.h"

extern PID *PID_Servo;
extern PID *Motor_chasu_PID;

extern volatile IMG_STATUS_e ov7725_eagle_img_flag;

//图像模 块
uint8 imgbuff[CAMERA_SIZE];//定义存储接收图像的数组
uint8 ImageData[image_rows/2][image_cols]; //解压后的图像数据
uint8 ImageBuff[CAMERA_SIZE];//定义存储压缩后图像的数组
uint32 time_1;
int16 ERROR;
uint8 N=0;
   int shijan=0;
   int image_finish=0;    //当图像更新一次    电机  舵機  控制 一次
   int image_finish_F_Servo=0;
   int image_finish_F_Mot=0;
void  main(void)
{
  System_Init();//系统初始化
  while(1)
  {     
    //FTM_PWM_Duty(FTM1,FTM_CH1,650);//输出PWM
    //           
    //获取图像
    Beep();//蜂鸣器消息机制
 
    //pit_time_start(PIT0);  
    camera_get_img();
    
    // shijan=pit_time_get_us(PIT0);
     //shijan=shijan;
    
    //#1将原始的一维图像数据解压成二维数据，并截取掉下面60行的数据  11ms   8.9ms   20ms
//    for(int n=image_rows/2;n>0;n--)
//    {
//      img_extract((uint8*)ImageData,imgbuff+(n-1)*20,1200);
//    }  
     
      img_extract((uint8*)ImageData,imgbuff,CAMERA_SIZE*1/2);  //470us
    
    
  
  
    if(RunState_flag)
    {
      if(key_check(KEY_L)==KEY_DOWN)
      {
        GUI_2_L_F++;
      }
    }
    if(!RunState_flag||GUI_2_L_F>1)  //跑动关闭图像压缩
    {
     img_recontract(  *ImageData, ImageBuff,CAMERA_SIZE);	                //图像压缩
    }
    

    
    //=============================激光测距==========================================
      
   uart_rx_irq_en(UART2);         //开串口0接收中断
    
    
    //=============================激光测距结束======================================
    
    
    if(Send_Model==254)
    {
     SendImgData(image_rows/2,ImageData);   //发送图像数据
    }
    
    if(ov7725_eagle_img_flag==IMG_FINISH)
    {
      vision_algorithm();//视觉处理主函数
    }
    
    
    

    
    if(!RunState_flag||GUI_2_L_F>1)         //跑动关闭GUI 
    {
      GUI();//菜单
    }
//    else
//    {
//      Site_t site = {0,0};  
//      Size_t size = {127,127};
//      LCD_rectangle(site, size, BLACK);
//    }
//  
    
    
    if(Run_Model==1)//纯图像数据跑
    {
      ERROR=-error;
      Servo_P=flash_read(FLASH_SECTOR_NUM-1-0,0,int);
      Servo_D=flash_read(FLASH_SECTOR_NUM-1-1,0,int);
    }
    else if(Run_Model==2)//纯电磁数据跑
    {
      ERROR=chabihe;
      Servo_P=26.0;//32-》25->26->
      Servo_D=30.0;//36->26->25->26->30
      if(abs(chabihe)<50)
        Servo_P=16;
      
    }
    else//(Run_Model=0)电磁，图像数据切换跑
    {
      //duanlu_F=1;
      if(duanlu_F==0)   //断路标志位 开启电磁跑
      {
        ERROR=-error;
        Servo_P=flash_read(FLASH_SECTOR_NUM-1-0,0,int);
        Servo_D=flash_read(FLASH_SECTOR_NUM-1-1,0,int);
      }
      else
      {
        ERROR=chabihe;
        Servo_P=26.0;//32-》25->26->
        Servo_D=30.0;//36->26->25->26->30
      }
      if(outside_flag)
      {
        ERROR=chabihe;
        Servo_P=26.0;//32-》25->26->
        Servo_D=30.0;//36->26->25->26->30
        if(++N==4)
        {
          outside_flag=0;
          N=0;
        }
      }
    }
    
    if(image_finish_F_Servo<image_finish)    //图像更新一次，舵机控制一次
    {
      Servo_PID_Ctrl(PID_Servo,ERROR);//舵机控制
      image_finish_F_Servo=image_finish;
    }
    
    //-----------------------------------电机控制
    

    //----------------------------------
    // Send_16bitData(elec_error,elec_L,elec_R);
    

      
    

  }
}


