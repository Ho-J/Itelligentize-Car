#ifndef _GUI_H
#define _GUI_H
#include "common.h"
//#include "include.h"

#define Num 9       //显示数值数量
#define Height 17   //单字符高
#define Width   8   //单字符宽 

 
extern int *Value_T[Num];
extern unsigned char Image_lcd_num;
extern unsigned char GUI_flag;
extern uint8 Run_Model;
extern char GUI_2_L_F;                  //第二个GUI界面 按下左键的标志位 ，用于打开GUI显示赛道图像
//============#给外部调用的函数接口声明Start#===============//
extern void GUI(void);
extern void Menu_Var_init(void);
//============#给外部调用的函数接口声明End#===============//
#endif