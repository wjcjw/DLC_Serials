/***************************************************************
****************************************************************
公  司: 郑州蓝晶科技公司电子实验室
编写者: 余庆春                       
日  期: 2007-5-17 
TLC5618.c：TLC5618初始化、输出相关函数
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

#define SET_DAC_DIN   *DAC_DAT_PORT |= DAC_DIN_BIT  //DIN置高电平
#define CLR_DAC_DIN   *DAC_DAT_PORT &= ~DAC_DIN_BIT //DIN置低电平

#define SET_DAC_CLK   *DAC_DAT_PORT |= DAC_CLK_BIT  //CLK置高电平
#define CLR_DAC_CLK   *DAC_DAT_PORT &= ~DAC_CLK_BIT //CLK置低电平

#define SET_DAC_CS    *DAC_DAT_PORT |= DAC_CS_BIT   //CS置高电平
#define CLR_DAC_CS    *DAC_DAT_PORT &= ~DAC_CS_BIT  //CS置低电平

 



//********************************************
//函数名: void tlc5618_init(void)
//描  述: tlc5618初始化函数,初始化寄存器       
//参  数: void
//返回值: void       
//注  意: 
//*********************************************
void tlc5618_init(void)
{   //DIN,CLK,CS为输出方式
    SET(*DAC_DIR_PORT,DIN_BIT);
	SET(*DAC_DIR_PORT,CLK_BIT);
	SET(*DAC_DIR_PORT,CS_BIT);
    SET_DAC_CS;
	CLR_DAC_CLK;
}

//********************************************
//函数名: void tlc5618_output(void)
//描  述: tlc5618的DA输出函数     
//参  数: 1.unsigned char ch 输出的通道 ch=0为A通道,=1为B通道
//        2.unsigned int val 输出的值
//返回值: void       
//注  意:  val为要输出的数值*1000,4.1V = 4100
//         软件中缩小1/2,即4100 =2.05V
//         外部硬件可放大
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