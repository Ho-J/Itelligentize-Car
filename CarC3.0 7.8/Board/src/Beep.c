#include "Beep.h"

uint8 IsBeep=0;
uint8 Is_activate;

/*void Beep(uint8 time)
{       
      uint8 i=0;
      for(i=0;i<time;i++)
      {
            BeepON();
            DELAY_MS(50);
            BeepOFF();
            DELAY_MS(50);
      }
}*/

void Beep()
{
  uint8 num=0;
  /**if(Is_activate)
  {
    Is_activate=0;*/
    if(IsBeep)
      num=1;
    else
      num=0;
    gpio_set (PTC7,num);
  //}
}
  