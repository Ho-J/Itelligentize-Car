#ifndef _BEEP_H
#define _BEEP_H
#include "common.h"
#include "include.h"


#define BeepInit()       gpio_init(PTC7,GPO,0)   //蜂鸣器端口初始化（关闭）
#define BeepON()         gpio_set (PTC7,1)         //蜂鸣器打开
#define BeepOFF()        gpio_set (PTC7,0)        //蜂鸣器关闭

extern void Beep();//蜂鸣器检测
extern uint8 IsBeep;//蜂鸣器标志
extern uint8 Is_activate;//消息机制（激活蜂鸣器消息队列）

#endif


