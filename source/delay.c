/*-----------------------------------------------------------------------
��ʱ����
ϵͳʱ�ӣ�11.0592M
-----------------------------------------------------------------------*/
#include "sysdef.h"


#define XTAL 11.0592

//��ʱ1us
void delay_1us(void)
{
   asm("nop");
   asm("nop");
   asm("nop");
}

//��ʱn us
void delay_nus(unsigned int n)       
{
   unsigned int i=0;
   for (i=0;i<n;i++)
      delay_1us();
}

//��ʱ1ms 
void delay_1ms(void)
{
  unsigned int i;
  for (i=1;i<(unsigned int) (XTAL*143-2);i++)
  ;
} 

//��ʱn ms    
void delay_nms(unsigned int n)       
{
   unsigned int i=0;
   for (i=0;i<n;i++)
   {
      delay_1ms();
   }   
}
