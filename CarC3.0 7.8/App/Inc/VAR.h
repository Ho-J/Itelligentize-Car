#ifndef _VAR_H_
#define _VAR_H_
#include "include.h"

#define image_rows CAMERA_H
#define image_cols CAMERA_W

extern uint8 imgbuff[CAMERA_SIZE];//����洢����ͼ�������
extern uint8 ImageData[image_rows/2][image_cols]; //��ѹ���ͼ������
extern uint8 ImageBuff[CAMERA_SIZE];//����洢ѹ����ͼ�������

extern int shijan;

extern int image_finish ;  
extern int image_finish_F_Servo ;  
extern int image_finish_F_Mot ;  

#define before_w 35  //��ʾ����ʾ������ȣ�Ҳ��ͼ������У���//35�У�46�У�40�У�53�У�30�У�42�У�
#endif