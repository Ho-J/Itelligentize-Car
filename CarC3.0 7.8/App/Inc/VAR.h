#ifndef _VAR_H_
#define _VAR_H_
#include "include.h"

#define image_rows CAMERA_H
#define image_cols CAMERA_W

extern uint8 imgbuff[CAMERA_SIZE];//定义存储接收图像的数组
extern uint8 ImageData[image_rows/2][image_cols]; //解压后的图像数据
extern uint8 ImageBuff[CAMERA_SIZE];//定义存储压缩后图像的数组

extern int shijan;

extern int image_finish ;  
extern int image_finish_F_Servo ;  
extern int image_finish_F_Mot ;  

#define before_w 35  //显示屏显示赛道宽度！也是图像控制行！！//35行（46列）40行（53列）30行（42列）
#endif