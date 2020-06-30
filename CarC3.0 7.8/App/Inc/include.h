#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"
#include  "core_cm4.h"
/*
 * Include 用户自定义的头文件
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO口操作
#include  "MK60_uart.h"     //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //低功耗定时器(延时)
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
#include  "VCAN_NRF24L0.h"      //无线模块NRF24L01+
#include  "VCAN_camera.h"       //摄像头总头文件
#include  "VCAN_LCD.h"          //液晶总头文件
#include  "ff.h"                //FatFs
#include  "VCAN_key_event.h"    //按键消息处理
#include  "VCAN_NRF24L0_MSG.h"  //无线模块消息处理

#include "Init.h"
#include "Beep.h"
#include "Computer.h"
#endif  //__INCLUDE_H__
