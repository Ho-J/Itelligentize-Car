#ifndef _ELEC_H
#define _ELEC_H
#include "include.h"
#include "common.h"
#include "Beep.h"
#define LEFT 0
#define RIGHT 1


//============#���ⲿ���õı����ӿ�����Start===============//
extern uint16 elec_L;
extern uint16 elec_R;
extern uint16 elec_M;
extern int16 elec_error;
extern int16 chabihe;
extern uint8 outside_flag;
//============���ⲿ���õı����ӿ�����End=================//

//====================�ڲ���������Start===============//
void Get_Value();
uint16 Value_calc(uint16 *num);

//====================�ڲ���������End//===============//




            
//============#���ⲿ���õĺ����ӿ�����Start#===============//


extern void Get_Value();
extern uint16 Value_calc(uint16 *num);
extern void ElecValue_dispose();


//============���ⲿ���õĺ����ӿ�����End=================//































#endif