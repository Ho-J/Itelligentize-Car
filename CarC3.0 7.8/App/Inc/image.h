#include "stdio.h"
#ifndef _IMAGE_H
#define _IMAGE_H
#include "common.h"
#define IMG_ROWS 60      //图像的总行数，即高
#define IMG_COLS 160     //图像的总列数，即宽
#define THLD 100    	 //宏定义阈值

//============#给外部调用的变量接口声明Start#===============//
extern int protect_flag;					//出界保护
extern unsigned char BlackRight[100][2];  //存储右边界的二维坐标
extern unsigned char BlackLeft[100][2];   //存储左边界的二维坐标
extern unsigned char Center[100][2];     //存储中线的二维坐标
extern int before;						//控制行前瞻
extern unsigned char NumbDatacenter;	//中线的有效数，GUI打印
extern unsigned char NumbDataleft;	//左边界的有效数，GUI打印
extern unsigned char NumbDataright;	//右边界有效数，GUI打印
extern int8 error;	                //所有有效行取平均的偏差
extern int Started_check_Zebra;      //斑马线标志
extern unsigned char image_flag;            //gui  图像程序测试标志位   在显示屏左上角打印一条红线
extern unsigned char image_flag_1;            //gui  图像程序测试标志位    //显示屏显示左上角一条红线

extern unsigned char kuan;//第一行的列宽

extern char duanlu_F;  //断路标志位

extern int Expectation_speed;   //图像算出来的期望速度

extern unsigned char bizhang_F;      //避障标志位


extern int speed_dingshi;               //速度定时

//============给外部调用的变量接口声明End=================//


extern uint32 shijian;


//============#给外部调用的函数接口声明Start#===============//

extern void vision_algorithm(); 	//图像的主函数，main函数调用

//============给外部调用的函数接口声明End=================//


#endif