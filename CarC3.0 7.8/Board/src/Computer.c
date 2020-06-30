#include "Computer.h"



/*

@note       串口发送图像函数（外部调用）

@brief      发送原始图像到红树伟业上位机

@param      rows    发送的行数

@param      Img     图像二维数组

*/
void SendImgData(int rows,uint8 Img[][CAMERA_W])
{
  int i,j;
  uart_putchar(UART0,0x01);
  for(i=0;i<rows;i++)//采样行数 60
  {  
    for(j=0;j<CAMERA_W;j++)//采样点数 160
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
//发送图像到山外上位机显示
//不同的上位机，不同的命令，这里使用山外串口调试 软件
//如果使用其他上位机，则需要修改代码 
//==============================================================================
void sendimg(uint8 *imgaddr, uint32 imgsize)
{
    uint8 CmdStart[2] = {0x01,0xfe};    //山外串口助手命令字节
    uint8 CmdStop[2] = {0xfe,0x01};
	
    uart_putbuff(VCAN_PORT, CmdStart, sizeof(CmdStart));    //先发送起始命令
    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //再发送图像
    uart_putbuff(VCAN_PORT, CmdStop, sizeof(CmdStop));    //先发送结束命令
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
// brief: 发送16位数据到 山外上位机的虚拟示波器
//        可以看电机PID的波形
// 
//
//==========================================================================//
void Send_16bitData(int16 data1,int16 data2,int16 data3)
{
 
    uint8 var_8bit;//将16位数据拆分2个8位数据,即缓存
    
    //数据头
    uart_putchar(UART0,0x03);
    uart_putchar(UART0,0xfc);
    
     ////////////////////////////////// 
    var_8bit = data1 & 0x00FF;
    uart_putchar(UART0,var_8bit);//低八位
    
    var_8bit = data1 >> 8;
    uart_putchar(UART0,var_8bit);//高八位
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



    //数据尾
    uart_putchar(UART0,0xfc);
    uart_putchar(UART0,0x03); 
}



//===========================================================================//
// brief: 发送16位数据到 山外上位机的虚拟示波器  发送6个数据
//        可以看电机PID的波形
// 
//
//==========================================================================//
void Send_16bitData_var6(int16 data1,int16 data2,int16 data3,int16 data4,int16 data5,int16 data6)
{
 
    uint8 var_8bit;//将16位数据拆分2个8位数据,即缓存
    
    //数据头
    uart_putchar(UART0,0x03);
    uart_putchar(UART0,0xfc);
    
     ////////////////////////////////// 
    var_8bit = data1 & 0x00FF;
    uart_putchar(UART0,var_8bit);//低八位
    
    var_8bit = data1 >> 8;
    uart_putchar(UART0,var_8bit);//高八位
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
    
    



    //数据尾
    uart_putchar(UART0,0xfc);
    uart_putchar(UART0,0x03); 
}