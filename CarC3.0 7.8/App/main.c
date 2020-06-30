//������ʶ��



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

//ͼ��ģ ��
uint8 imgbuff[CAMERA_SIZE];//����洢����ͼ�������
uint8 ImageData[image_rows/2][image_cols]; //��ѹ���ͼ������
uint8 ImageBuff[CAMERA_SIZE];//����洢ѹ����ͼ�������
uint32 time_1;
int16 ERROR;
uint8 N=0;
   int shijan=0;
   int image_finish=0;    //��ͼ�����һ��    ���  ��C  ���� һ��
   int image_finish_F_Servo=0;
   int image_finish_F_Mot=0;
void  main(void)
{
  System_Init();//ϵͳ��ʼ��
  while(1)
  {     
    //FTM_PWM_Duty(FTM1,FTM_CH1,650);//���PWM
    //           
    //��ȡͼ��
    Beep();//��������Ϣ����
 
    //pit_time_start(PIT0);  
    camera_get_img();
    
    // shijan=pit_time_get_us(PIT0);
     //shijan=shijan;
    
    //#1��ԭʼ��һάͼ�����ݽ�ѹ�ɶ�ά���ݣ�����ȡ������60�е�����  11ms   8.9ms   20ms
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
    if(!RunState_flag||GUI_2_L_F>1)  //�ܶ��ر�ͼ��ѹ��
    {
     img_recontract(  *ImageData, ImageBuff,CAMERA_SIZE);	                //ͼ��ѹ��
    }
    

    
    //=============================������==========================================
      
   uart_rx_irq_en(UART2);         //������0�����ж�
    
    
    //=============================���������======================================
    
    
    if(Send_Model==254)
    {
     SendImgData(image_rows/2,ImageData);   //����ͼ������
    }
    
    if(ov7725_eagle_img_flag==IMG_FINISH)
    {
      vision_algorithm();//�Ӿ�����������
    }
    
    
    

    
    if(!RunState_flag||GUI_2_L_F>1)         //�ܶ��ر�GUI 
    {
      GUI();//�˵�
    }
//    else
//    {
//      Site_t site = {0,0};  
//      Size_t size = {127,127};
//      LCD_rectangle(site, size, BLACK);
//    }
//  
    
    
    if(Run_Model==1)//��ͼ��������
    {
      ERROR=-error;
      Servo_P=flash_read(FLASH_SECTOR_NUM-1-0,0,int);
      Servo_D=flash_read(FLASH_SECTOR_NUM-1-1,0,int);
    }
    else if(Run_Model==2)//�����������
    {
      ERROR=chabihe;
      Servo_P=26.0;//32-��25->26->
      Servo_D=30.0;//36->26->25->26->30
      if(abs(chabihe)<50)
        Servo_P=16;
      
    }
    else//(Run_Model=0)��ţ�ͼ�������л���
    {
      //duanlu_F=1;
      if(duanlu_F==0)   //��·��־λ ���������
      {
        ERROR=-error;
        Servo_P=flash_read(FLASH_SECTOR_NUM-1-0,0,int);
        Servo_D=flash_read(FLASH_SECTOR_NUM-1-1,0,int);
      }
      else
      {
        ERROR=chabihe;
        Servo_P=26.0;//32-��25->26->
        Servo_D=30.0;//36->26->25->26->30
      }
      if(outside_flag)
      {
        ERROR=chabihe;
        Servo_P=26.0;//32-��25->26->
        Servo_D=30.0;//36->26->25->26->30
        if(++N==4)
        {
          outside_flag=0;
          N=0;
        }
      }
    }
    
    if(image_finish_F_Servo<image_finish)    //ͼ�����һ�Σ��������һ��
    {
      Servo_PID_Ctrl(PID_Servo,ERROR);//�������
      image_finish_F_Servo=image_finish;
    }
    
    //-----------------------------------�������
    

    //----------------------------------
    // Send_16bitData(elec_error,elec_L,elec_R);
    

      
    

  }
}


