#ifndef _COMPUTER_H
#define _COMPUTER_H
#include  "common.h"
#include "include.h"



//============#给外部调用的函数接口声明Start#===============//

extern void SendImgData(int rows,uint8 Img[][OV7725_EAGLE_W]);//发送原始图像到红树伟业上位机

extern void sendimg(uint8 *imgaddr, uint32 imgsize);
//extern void SendData(int8 data1,int8 data2);
extern void SendData(int16 data1,int16 data2);
extern void Send_16bitData_var6(int16 data1,int16 data2,int16 data3,int16 data4,int16 data5,int16 data6);//发送16位数据到 山外上位机的虚拟示波器

//============给外部调用的函数接口声明End=================//



#endif