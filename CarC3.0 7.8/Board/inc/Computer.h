#ifndef _COMPUTER_H
#define _COMPUTER_H
#include  "common.h"
#include "include.h"



//============#���ⲿ���õĺ����ӿ�����Start#===============//

extern void SendImgData(int rows,uint8 Img[][OV7725_EAGLE_W]);//����ԭʼͼ�񵽺���ΰҵ��λ��

extern void sendimg(uint8 *imgaddr, uint32 imgsize);
//extern void SendData(int8 data1,int8 data2);
extern void SendData(int16 data1,int16 data2);
extern void Send_16bitData_var6(int16 data1,int16 data2,int16 data3,int16 data4,int16 data5,int16 data6);//����16λ���ݵ� ɽ����λ��������ʾ����

//============���ⲿ���õĺ����ӿ�����End=================//



#endif