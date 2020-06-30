#include "common.h"
#include "include.h"
#include "VCAN_camera.h"
uint8 black=1,white=0; //0 和 1 分别对应的颜色
//==============================================================================
//压缩的二值化图像     解压（空间 换 时间 解压）
//srclen 是二值化图像的占用空间大小
//【鹰眼解压】鹰眼图像解压，转为 二维数组 - 智能车资料区 - 山外论坛 http://vcan123.com/forum.php?mod=viewthread&tid=17&ctid=6
//解压的时候，里面有个数组，配置黑、白对应的值是多少。
//==============================================================================
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255,0}; //0 和 1 分别对应的颜色
                                //注：山外的摄像头 0 表示 白色，1表示 黑色
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

/*!
*  @brief      二值化图像压缩（空间 换 时间 压缩）
*  @param      dst             图像压缩目的地址
*  @param      src             图像压缩源地址
*  @param      srclen          二值化图像的占用空间大小
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_recontract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 tmpsrc;
  uint8 i, j, k;
  uint32 Count = 0;
  
  for (i = 0; i < CAMERA_H/2; i++)
  {
    for (j = 0; j < CAMERA_W/8; j++)
    {
      Count = (i)*CAMERA_W+j*8;
      for (k = 0; k < 8; k++)		  
      {
        tmpsrc <<= 1;
        if (!dst[Count++])  //0
        {
          tmpsrc |= 1;
        }
      }
      *src++ = tmpsrc;
      
    }
  }
  
  //  while(srclen --)
//  {
//    tmpsrc=0;
//    if(*dst++ == colour[black])
//    {
//      tmpsrc = tmpsrc + 0x80;
//    }
//    
//    if(*dst++ == colour[black])
//    {
//      tmpsrc = tmpsrc + 0x40;
//    }
//    
//    if(*dst++ == colour[black])
//    {
//      tmpsrc = tmpsrc + 0x20;
//    }
//    
//    if(*dst++ == colour[black])
//    {
//      tmpsrc = tmpsrc + 0x10;
//    }
//    
//    if(*dst++ == colour[black])
//    {
//      tmpsrc = tmpsrc + 0x08;
//    }
//    
//    if(*dst++ == colour[black])
//    {
//      tmpsrc = tmpsrc + 0x04;
//    }
//    
//    if(*dst++ == colour[black])
//    {
//      tmpsrc = tmpsrc + 0x02;
//    }
//    
//    if(*dst++ == colour[black])
//    {
//      tmpsrc = tmpsrc + 0x01;
//    }
//    
//    *src++ = tmpsrc;
//  }
}