/***************************************************************
****************************************************************
公  司: 郑州蓝晶科技公司电子实验室
编写者: 余庆春                       
日  期: 2008-5-17 
TLC2543.c：TLC2543初始化、12bit A/D采样 输出相关函数
****************************************************************
***************************************************************/
#include "Sysdef.h"
#include "tlc2543.h"


#define SET_TCL2543_CLK    PORTC |= (1 << PC1)          //位置高
#define CLR_TCL2543_CLK    PORTC &= ~(1 << PC1)          //位置低
#define SET_TCL2543_ADIN    PORTC |= (1 << PC2)          //位置高
#define CLR_TCL2543_ADIN    PORTC &= ~(1 << PC2)          //位置低
#define SET_TCL2543_CS    PORTC |= (1 << PC4)          //位置高
#define CLR_TCL2543_CS    PORTC &= ~(1 << PC4)          //位置低
//#define LED_COM    PORTD ^= (1 << PD5)           //位取反
#define M_AD_EOC          PINC & (1 << PC0)             //位读取
#define TCL2543_DOUT      PINC & (1 << PC3)             //位读取

unsigned int ADC_IA_DATA[256] = {0};
unsigned int ADC_HV_DATA[256] = {0};

unsigned char ADC_lb_star = 0;
unsigned char ADC_lb_SAVE = 0;
unsigned char ADC_lb_flg = 0;



unsigned int ADC_data[9] = {0};
unsigned int ADC_D_dt[9][20] = {0};
unsigned char ADC_D = 0;
unsigned char adc_ss  = 0;
unsigned char ADC_D_flg = 0;
//********************************************
//函数名: void tlc2543_init(void)
//描  述: tlc2543初始化函数,初始化寄存器       
//参  数: void
//返回值: void       
//注  意: 
//*********************************************
void tlc2543_init(void)
{  
   CLR_TCL2543_CLK; 
   delay_nus(2);
   SET_TCL2543_CLK;
   SET_TCL2543_CS;   
}
//********************************************
//函数名: void star_lb_pro(void)
//描  述: 启动录播程序ADC_lb_flg=0x55,录播中。。。，录播完成后为ADC_lb_flg=0xaa
//参  数: void
//返回值: void       
//注  意: 
//*********************************************
 void star_lb_pro(void)
 {
    if(ADC_lb_flg == 0)
	{
	 	ADC_lb_flg=0x55;
		if(ADC_lb_SAVE>=75)ADC_lb_star=(ADC_lb_SAVE-75);
		else ADC_lb_star=(ADC_lb_SAVE+175);
	}
 }
//********************************************
//函数名: void tlc2543_output(unsigned char port)
//描  述: tlc2543的AD输出函数     
//参  数: 1. unsigned char port 对应某通道的A/D转换函数，port是采样的通道号  
//          d7-d4  0-----10转换通道  0000B-1010B
//          d3-d2  output data longth  01-8bit; x0-12bit;  11-16bit;
//          d1     output data format  0-MSB first;  1-LSB first
//          d0     0-测量值为单极性（二进制）1-测量值为双极性（二进制补码）
//          
//        
//返回值: unsigned int val //当前返回的测量值是上一次输入的结果
//注  意:  val为要输出的数值*0xfff, 5.0V = 4095
//         
//         外部硬件可放大
//*********************************************

unsigned int tlc2543_read(unsigned char port1)
{
    unsigned int i = 0, ad = 0;   	
	unsigned char port = 0;
	port=port1;
	if(M_AD_EOC!=0)
	{
	   CLR_TCL2543_CLK;   
       CLR_TCL2543_CS;   
       port<<=4;
       delay_nus(1);
       for(i=0 ;i < 12;i++)   
       {  	 
	     ad<<=1; 
         if(TCL2543_DOUT)   ad |= 0x01;   
        
         if(port&0x80)  
		    SET_TCL2543_ADIN;
         else    
		    CLR_TCL2543_ADIN;   
	     SET_TCL2543_CLK;	
		 delay_nus(1);
		 CLR_TCL2543_CLK;
		 delay_nus(1);
         port<<=1;  
		              
       }   
       SET_TCL2543_CS;    
	}
    return(ad);  
}


//********************************************
//函数名: void ADC_read(unsigned char port)
//描  述: AD采样函数     
//参  数: 
//返回值:  
//注  意:   ADC_data[port]为多次采样平均值，见#define adc_tim 10 
//        
//*********************************************
#define adc_tim 45
#define adc_tim1 20
 void ADC_read(void)
{
  unsigned char port=0,j=0,k=0;
  unsigned long temp_data = 0;
  unsigned int temp_max = 0,temp_min = 0;
  unsigned int temp_max1 = 0,temp_min1 = 0;
  unsigned int temp_max2 = 0,temp_min2 = 0;
  unsigned int adc_data_buf[9][50] = {0};
     for(port = 6;port < 9 ; port++)//采样AD数据
  	 {
	    adc_data_buf[port][0] = tlc2543_read(port);
        for(j=0;j<adc_tim;j++)
	 	{
	       adc_data_buf[port][j] = tlc2543_read(port); 
	    }
     }
  	 for(port=0;port < 9 ; port++)//add 平均数字滤波
  	 {
        temp_data = 0;
		temp_max = 0;
		temp_min = 4095;
		for(j=0;j<adc_tim;j++)
	 	{
	         if(temp_max <= adc_data_buf[port][j])
		  	 {
		          
		    	  temp_max = adc_data_buf[port][j];
		      }		  
		      if(temp_min >= adc_data_buf[port][j])
		      {
		         
		    	  temp_min = adc_data_buf[port][j];
			   }
	           temp_data += adc_data_buf[port][j];		
	    }
		
	    ADC_data[port] = (uint)((temp_data-temp_min-temp_max)/(adc_tim-2));
     }	  	

   if(ADC_lb_flg!=0xaa)
   {
	    if(ADC_lb_flg==0)
		{
 	       ADC_lb_SAVE ++;
	       if(ADC_lb_SAVE >=250)	
		   { 
		      ADC_lb_SAVE = 0;
			  SET_RUN_LED_ON; 
			  delay_nms(1);
		   }	 
	 	   ADC_HV_DATA[ADC_lb_SAVE] = ADC_IA;
 	 	   ADC_IA_DATA[ADC_lb_SAVE] =ADC_HV;
		}
		if(ADC_lb_flg==0x55)
		{
 	       ADC_lb_SAVE ++;
	       if(ADC_lb_SAVE >=250)	 ADC_lb_SAVE = 0;	
		   if( 	 ADC_lb_SAVE!=ADC_lb_star)
		   {
	 	   		 ADC_HV_DATA[ADC_lb_SAVE] = ADC_IA;
 	 	         ADC_IA_DATA[ADC_lb_SAVE] =ADC_HV;
			}
			else 
			{
			     ADC_lb_flg=0xaa;
				 if(ADC_lb_SAVE==0)ADC_lb_SAVE=249;
				 else  ADC_lb_SAVE--;
			}
		}
	}
  
}
	 
  
	 
	  
