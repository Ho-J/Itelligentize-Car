#include "Computer.h"



/*

@note       ���ڷ���ͼ�������ⲿ���ã�

@brief      ����ԭʼͼ�񵽺���ΰҵ��λ��

@param      rows    ���͵�����

@param      Img     ͼ���ά����

*/
void SendImgData(int rows,uint8 Img[][CAMERA_W])
{
  int i,j;
  uart_putchar(UART0,0x01);
  for(i=0;i<rows;i++)//�������� 60
  {  
    for(j=0;j<CAMERA_W;j++)//�������� 160
    {
      if(Img[i][j]==255)
        Img[i][j]=0xfe;
      if(Img[i][j]==0)
        Img[i][j]=0x00;
      
      
      uart_putchar (UART0,Img[i][j]);
      
    } 
  }
  uart_putchar (UART0,0xff);  
}


//==============================================================================
//����ͼ��ɽ����λ����ʾ
//��ͬ����λ������ͬ���������ʹ��ɽ�⴮�ڵ��� ���
//���ʹ��������λ��������Ҫ�޸Ĵ��� 
//==============================================================================
void sendimg(uint8 *imgaddr, uint32 imgsize)
{
    uint8 CmdStart[2] = {0x01,0xfe};    //ɽ�⴮�����������ֽ�
    uint8 CmdStop[2] = {0xfe,0x01};
	
    uart_putbuff(VCAN_PORT, CmdStart, sizeof(CmdStart));    //�ȷ�����ʼ����
    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //�ٷ���ͼ��
    uart_putbuff(VCAN_PORT, CmdStop, sizeof(CmdStop));    //�ȷ��ͽ�������
}


//void SendData(int8 data1,int8 data2)
void SendData(int16 data1,int16 data2)
{
  
      uart_putchar(UART0,0x03);
      uart_putchar(UART0,0xfc);
      
      uart_putchar(UART0,data1);
      
      uart_putchar(UART0,data2);
      
//      uart_putchar(UART0,CntEncoder%255);
//      uart_putchar(UART0,CntEncoder/255);
//      
//      uart_putchar(UART0,Encoder_set%255);
//      uart_putchar(UART0,Encoder_set/255);
//      
      uart_putchar(UART0,0xfc);
      uart_putchar(UART0,0x03);   
     
////      uart_putchar(UART0,data1%255);
////      uart_putchar(UART0,data1/255);
//      
//      uart_putchar(UART0,data2%255);
//      uart_putchar(UART0,data2/255);
//
//      uart_putchar(UART0,out_motor%255);
//      uart_putchar(UART0,out_motor/255);
//////      

//
//      
 
}
      
//===========================================================================//
// brief: ����16λ���ݵ� ɽ����λ��������ʾ����
//        ���Կ����PID�Ĳ���
// 
//
//==========================================================================//
void Send_16bitData(int16 data1,int16 data2,int16 data3)
{
 
    uint8 var_8bit;//��16λ���ݲ��2��8λ����,������
    
    //����ͷ
    uart_putchar(UART0,0x03);
    uart_putchar(UART0,0xfc);
    
     ////////////////////////////////// 
    var_8bit = data1 & 0x00FF;
    uart_putchar(UART0,var_8bit);//�Ͱ�λ
    
    var_8bit = data1 >> 8;
    uart_putchar(UART0,var_8bit);//�߰�λ
     //////////////////////////////////
    var_8bit=data2 & 0x00FF;
    uart_putchar(UART0,var_8bit);
    
    var_8bit = data2 >> 8;
    uart_putchar(UART0,var_8bit);
     //////////////////////////////////
    var_8bit=data3 & 0x00FF;
    uart_putchar(UART0,var_8bit);
    
    var_8bit = data3 >> 8;
    uart_putchar(UART0,var_8bit);



    //����β
    uart_putchar(UART0,0xfc);
    uart_putchar(UART0,0x03); 
}



//===========================================================================//
// brief: ����16λ���ݵ� ɽ����λ��������ʾ����  ����6������
//        ���Կ����PID�Ĳ���
// 
//
//==========================================================================//
void Send_16bitData_var6(int16 data1,int16 data2,int16 data3,int16 data4,int16 data5,int16 data6)
{
 
    uint8 var_8bit;//��16λ���ݲ��2��8λ����,������
    
    //����ͷ
    uart_putchar(UART0,0x03);
    uart_putchar(UART0,0xfc);
    
     ////////////////////////////////// 
    var_8bit = data1 & 0x00FF;
    uart_putchar(UART0,var_8bit);//�Ͱ�λ
    
    var_8bit = data1 >> 8;
    uart_putchar(UART0,var_8bit);//�߰�λ
     //////////////////////////////////
    var_8bit=data2 & 0x00FF;
    uart_putchar(UART0,var_8bit);
    
    var_8bit = data2 >> 8;
    uart_putchar(UART0,var_8bit);
     //////////////////////////////////
    var_8bit=data3 & 0x00FF;
    uart_putchar(UART0,var_8bit);
    
    var_8bit = data3 >> 8;
    uart_putchar(UART0,var_8bit);
    
    var_8bit=data4 & 0x00FF;
    uart_putchar(UART0,var_8bit);
    
    var_8bit = data4 >> 8;
    uart_putchar(UART0,var_8bit);
    
    var_8bit=data5 & 0x00FF;
    uart_putchar(UART0,var_8bit);
    
    var_8bit = data5 >> 8;
    uart_putchar(UART0,var_8bit);
    
    var_8bit=data6 & 0x00FF;
    uart_putchar(UART0,var_8bit);
    
    var_8bit = data6 >> 8;
    uart_putchar(UART0,var_8bit);
    
    



    //����β
    uart_putchar(UART0,0xfc);
    uart_putchar(UART0,0x03); 
}