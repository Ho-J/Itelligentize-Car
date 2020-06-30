#include "elec.h"
#include "image.h"

uint16 elec_L;
uint16 elec_R;
uint16 elec_M;
uint16 Value_L[5];
uint16 Value_R[5];
uint16 Value_M[5];
int16 elec_error;
int16 chabihe;
//uint16 elec_min_L=190;
//uint16 elec_min_R=620;
uint8 outside_flag=0;
uint16 elec_max=4095;


void Get_Value()
{
  for(uint8 i=0;i<5;i++)//�ɼ���ε��ֵ
  {
    Value_L[i]=adc_once(ADC0_SE17,ADC_12bit);//�ɼ��������ֵ
    Value_R[i]=adc_once(ADC0_SE18,ADC_12bit);//�ɼ�����ҵ��ֵ
    Value_M[i]=adc_once(ADC1_SE6a,ADC_12bit);//�ɼ�����ҵ��ֵ
  }
}

uint16 Value_calc(uint16 *num)
{
  uint16 value,temp;
  for(uint8 i=0;i<5;i++)//�����޳������Сֵ
  {
    for(uint8 j=0+i;j<5;j++)
    {
      if(num[i]<num[j])
      {
        temp=num[i];
        num[i]=num[j];
        num[j]=temp;
      }
    }
  }
  value=(uint16)(num[1]+num[2]+num[3])/3;//��ƽ��ֵ
  //value=(uint16)((value*100)/elec_max);//��һ������
  return value;
}

void ElecValue_dispose()
{
  
  Get_Value();
  
 elec_L=Value_calc(Value_L);
 elec_R=Value_calc(Value_R);
 elec_M=Value_calc(Value_M);
 
 
 if(elec_L<150&&elec_R<150)
 {
   if(bizhang_F==0)
  {
    //protect_flag=1;             //��ų��籣��
//    protect_flag=1;    //ͣ����־λ
//    RunState_flag=0;
  }
 }
 elec_error=elec_L-elec_R;
 if(abs(elec_error)>3400&&outside_flag==0)
   outside_flag=1;
 chabihe=(elec_error*100/(elec_L+elec_R));
 //Send_16bitData(elec_L,elec_R,chabihe);
 
}



/*
*/







