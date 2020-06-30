#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"
#include  "core_cm4.h"
/*
 * Include �û��Զ����ͷ�ļ�
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO�ڲ���
#include  "MK60_uart.h"     //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //�͹��Ķ�ʱ��(��ʱ)
#include  "MK60_i2c.h"      //I2C
#include  "MK60_spi.h"      //SPI
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_rtc.h"      //RTC
#include  "MK60_adc.h"      //ADC
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_sdhc.h"
#include  "MK60_dma.h"

#include  "VCAN_LED.H"          //LED
#include  "VCAN_KEY.H"          //KEY
#include  "VCAN_NRF24L0.h"      //����ģ��NRF24L01+
#include  "VCAN_camera.h"       //����ͷ��ͷ�ļ�
#include  "VCAN_LCD.h"          //Һ����ͷ�ļ�
#include  "ff.h"                //FatFs
#include  "VCAN_key_event.h"    //������Ϣ����
#include  "VCAN_NRF24L0_MSG.h"  //����ģ����Ϣ����

#include "Init.h"
#include "Beep.h"
#include "Computer.h"
#endif  //__INCLUDE_H__
