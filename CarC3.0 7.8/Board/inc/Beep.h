#ifndef _BEEP_H
#define _BEEP_H
#include "common.h"
#include "include.h"


#define BeepInit()       gpio_init(PTC7,GPO,0)   //�������˿ڳ�ʼ�����رգ�
#define BeepON()         gpio_set (PTC7,1)         //��������
#define BeepOFF()        gpio_set (PTC7,0)        //�������ر�

extern void Beep();//���������
extern uint8 IsBeep;//��������־
extern uint8 Is_activate;//��Ϣ���ƣ������������Ϣ���У�

#endif


