#ifndef _GUI_H
#define _GUI_H
#include "common.h"
//#include "include.h"

#define Num 9       //��ʾ��ֵ����
#define Height 17   //���ַ���
#define Width   8   //���ַ��� 

 
extern int *Value_T[Num];
extern unsigned char Image_lcd_num;
extern unsigned char GUI_flag;
extern uint8 Run_Model;
extern char GUI_2_L_F;                  //�ڶ���GUI���� ��������ı�־λ �����ڴ�GUI��ʾ����ͼ��
//============#���ⲿ���õĺ����ӿ�����Start#===============//
extern void GUI(void);
extern void Menu_Var_init(void);
//============#���ⲿ���õĺ����ӿ�����End#===============//
#endif