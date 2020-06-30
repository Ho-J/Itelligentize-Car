#ifndef _ELEC_H
#define _ELEC_H
#include "include.h"
#include "common.h"
#include "Beep.h"
#define LEFT 0
#define RIGHT 1


//============#给外部调用的变量接口声明Start===============//
extern uint16 elec_L;
extern uint16 elec_R;
extern uint16 elec_M;
extern int16 elec_error;
extern int16 chabihe;
extern uint8 outside_flag;
//============给外部调用的变量接口声明End=================//

//====================内部函数声明Start===============//
void Get_Value();
uint16 Value_calc(uint16 *num);

//====================内部函数声明End//===============//




            
//============#给外部调用的函数接口声明Start#===============//


extern void Get_Value();
extern uint16 Value_calc(uint16 *num);
extern void ElecValue_dispose();


//============给外部调用的函数接口声明End=================//































#endif