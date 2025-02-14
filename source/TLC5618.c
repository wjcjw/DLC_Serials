/***************************************************************
****************************************************************
��  ˾: ֣�������Ƽ���˾����ʵ����
��д��: ���촺                       
��  ��: 2007-5-17 
TLC5618.c��TLC5618��ʼ���������غ���
****************************************************************
***************************************************************/
#include "Sysdef.h"
#include "App_para.h"
#include "tlc5618.h"
#include "DAC_inc.h"

volatile unsigned char *DAC_DIR_PORT  = &DDRD;
volatile unsigned char *DAC_DAT_PORT  = &PORTD;

#define DIN_BIT   7
#define CLK_BIT   6
#define CS_BIT    5

char DAC_DIN_BIT =  BIT(DIN_BIT);
char DAC_CLK_BIT =  BIT(CLK_BIT);
char DAC_CS_BIT  =  BIT(CS_BIT);

#define SET_DAC_DIN   *DAC_DAT_PORT |= DAC_DIN_BIT  //DIN�øߵ�ƽ
#define CLR_DAC_DIN   *DAC_DAT_PORT &= ~DAC_DIN_BIT //DIN�õ͵�ƽ

#define SET_DAC_CLK   *DAC_DAT_PORT |= DAC_CLK_BIT  //CLK�øߵ�ƽ
#define CLR_DAC_CLK   *DAC_DAT_PORT &= ~DAC_CLK_BIT //CLK�õ͵�ƽ

#define SET_DAC_CS    *DAC_DAT_PORT |= DAC_CS_BIT   //CS�øߵ�ƽ
#define CLR_DAC_CS    *DAC_DAT_PORT &= ~DAC_CS_BIT  //CS�õ͵�ƽ

 



//********************************************
//������: void tlc5618_init(void)
//��  ��: tlc5618��ʼ������,��ʼ���Ĵ���       
//��  ��: void
//����ֵ: void       
//ע  ��: 
//*********************************************
void tlc5618_init(void)
{   //DIN,CLK,CSΪ�����ʽ
    SET(*DAC_DIR_PORT,DIN_BIT);
	SET(*DAC_DIR_PORT,CLK_BIT);
	SET(*DAC_DIR_PORT,CS_BIT);
    SET_DAC_CS;
	CLR_DAC_CLK;
}

//********************************************
//������: void tlc5618_output(void)
//��  ��: tlc5618��DA�������     
//��  ��: 1.unsigned char ch �����ͨ�� ch=0ΪAͨ��,=1ΪBͨ��
//        2.unsigned int val �����ֵ
//����ֵ: void       
//ע  ��:  valΪҪ�������ֵ*1000,4.1V = 4100
//         �������С1/2,��4100 =2.05V
//         �ⲿӲ���ɷŴ�
//*********************************************
void tlc5618_output(unsigned char ch,unsigned int val)  //vcur_val*V_DAC_MODULUS/VSET_MAX)
{	 						 	  			  	  			//*2400 /5000=	1966				  						
    unsigned char i = 0;
    unsigned long  svcon = 0;
	svcon = val;
    svcon = svcon*4096;//2.5237V	//wjgbj   2158  8847000= *4096
	 svcon = svcon/4500;   //  1966=  /4500
    if(ch == 0)
	{
      svcon |= 0xc000;
	  svcon &= 0xcfff;
	}
    else if(ch == 1)
    {
      svcon |= 0x4000;
	  svcon &= 0x4fff;	
    }
	
    CLR_DAC_CS;
	for(i=0;i<16;i++)
	{
	   if(svcon&0x8000)
	      SET_DAC_DIN;
	   else
	      CLR_DAC_DIN;  	   
	   svcon <<= 1;
	   delay_nus(2);
	   SET_DAC_CLK;	  
	   delay_nus(4);
	   CLR_DAC_CLK;
	   delay_nus(2);
	} 
	SET_DAC_CLK;
	CLR_DAC_CLK;	
	delay_nus(4);
    SET_DAC_CS;
}