
/******************************************************
公  司: 郑州蓝晶科技公司---电子实验室
编写者: 余庆春     
main.c：主程序、系统初始化、定时器中断、外部中断、
Crystal: 7.3728Mhz 
Target : M16
ICC-AVR application builder : 2008-5-20 17:21:25
20090627  增加温度功能
20091022  增加功能：（接罗工电话通知）修改参数设置命令为单帧命令
		  保留原通信协议，相应不变
**************************************************************** 
***************************************************************/

#include "Sysdef.h"
#include "App_para.h"
#include "init_pro.h"
#include "Uart.h"
//#include "ADC.h"
#include "TLC5618.h"
#include "TLC2543.h"
#include "new.h"
//ARC timer flage
unsigned int  arc_num = 0;//关闭xray时清零
unsigned int  arc_err_flg = 0;//软件置位，外部命令清除

unsigned int  run_led_flg = 0;//

unsigned int  datset_flg = 0 ;//设置参数后调整告警方案

unsigned long  arc_time1 = 0;//arc-1动作时间
unsigned long  arc_time2 = 0;//arc-2动作时间
unsigned long  arc_time3 = 0;//arc-3动作时间
unsigned long  arc_time4 = 0;//arc-4动作时间  
unsigned char xray_flg = 0;
unsigned char led_77 = 0;
unsigned char arc_tt=0;
unsigned long  init_t0=0;
unsigned int  xray_on_ms=0;//10毫秒计数器
unsigned int xray_on_1m =0;//1分钟计数器
unsigned int xray_on_1s =0;//1秒钟计数器
unsigned int  arc_number=0;
unsigned int  arc_number1=0;

unsigned long arc_led_tim = 0;//arc led on 1min
unsigned int ADC_HV = 0;
unsigned int ADC_IA = 0;
unsigned int ADC_TT = 0;
//********************************************
//函数名: display_led(void)
//描  述: 根据A/D,ARC,XRAY_LED_ON ，点亮告警指示
//参  数: void
//返回值: void        
//注  意: err_flg的应用
//*********************************************
void ADC_HUB(void)
{   
   
   unsigned long  temp_adc=0;
   temp_adc = ADC_data[8];
   temp_adc=(temp_adc*118/T_ADC_MODULUS);
   ADC_TT=temp_adc;   
   temp_adc = ADC_data[7];
   ADC_IA=(temp_adc*ISET_MAX /I_ADC_MODULUS*4500/4095);
   							
   temp_adc = ADC_data[6];
   ADC_HV=(temp_adc*VSET_MAX /V_ADC_MODULUS*4500/4095);
  //=3276*1600/ 3000   *4500/4095

}

//********************************************
//函数名: arc_pro(void)
//描  述: 根据arc中断响应程序
//参  数: void
//返回值: void        
//注  意: arc_timer的应用
//*********************************************
void arc_pro(void)
{  	
	//arc 自恢复（复位）
	if  (arc_led_tim>=3000)//自恢复为30秒
	    {									
			 SET_ARC_LED_OFF;//arc动作（1分钟内4次放电脉冲），外部命令错误清除后恢复	 
	         FAULT_DATA &= 0xfb;  //arc_error
			 arc_num=0;
			 arc_led_tim=0;
         
	    }
	//arc中断执行程序
	if (arc_on_flg==0x55)
	  {		
			 
			  arc_on_flg=0; 
 			  datset_flg=40;
 			  arc_time1 = (((xray_on_1m*60)+xray_on_1s)*100+xray_on_ms);//单位是10ms
 			  if((arc_time1-arc_time2)>=10)
 			  {    
   			      arc_num++;   // 指示加1，指示arc动作次数
    		      FAULT_DATA |= 0x04;  //arc_error
 			      SET_ARC_LED_ON;//arc动作（1分钟内4次放电脉冲），外部命令错误清除后恢复
 			      arc_led_tim=0;
				  if ((arc_num>=4)&&((arc_time1-arc_time4)<=1000))//10S内动作4次
   			  	  {		       
						  SET_HV_OFF;	
						  HV_ON_FLAG = 0;
   				   }
				     arc_time4 = arc_time3; 
     		 	     arc_time3 = arc_time2;
     		 		 arc_time2 = arc_time1;//更新arc_time2，3，4动作时间
  			  }
			  //启动录播	
			  star_lb_pro();
    		  
		} 
}
//********************************************
//函数名: time_pro(void)
//描  述: 根据定时器中断响应各标志时间程序
//参  数: void
//返回值: void        
//注  意: xray_on_ms的应用
//*********************************************

void time_pro(void)
{
   unsigned long  init_t0=0;
   if(init_t0!=init_10ms)  //init_10ms程序在init_pro.c里
     init_t0 = init_10ms;
   if(xray_on_ms!= init_t0)
   { 
				if( xray_on_ms>= init_t0)
				{   
								xray_on_1s++;
								if(xray_on_1s >= 60 )
								{
										xray_on_1s = 0;
										xray_on_1m++;	
									}
				if((WATCH_DOG_FLG == 0x55)&&(XRAY_LED_PORT))
   	 	       WATCH_DOG_TIM+=(init_t0+100)-xray_on_ms; 
        else WATCH_DOG_TIM = 0;
				if( (FAULT_DATA & 0x04)   ==  0x04 )
   	 	     arc_led_tim +=(init_t0+100)-xray_on_ms; 
        else arc_led_tim = 0;	
				}
	   else	
	   
	   {
	      if((WATCH_DOG_FLG == 0x55)&&(XRAY_LED_PORT))
   	 	     WATCH_DOG_TIM+=(init_t0-xray_on_ms); 
        else WATCH_DOG_TIM = 0;
				if( (FAULT_DATA & 0x04 )  ==  0x04 )      
   	 	     arc_led_tim +=(init_t0-xray_on_ms); 
        else arc_led_tim= 0;	    
		  xray_on_ms = init_t0;
	   }  
	   xray_on_ms = init_t0;
   }
}


//********************************************
//函数名: void main(void)
//描  述: 单片机系统主函数
//参  数: void
//返回值: void        
//注  意: 
//*********************************************

void main(void)
{  
 unsigned char i = 0,j=1;
 
  init_devices() ;//init_pro.c
  
  SET_ERROR_OFF;
  
  init_xray();//init_pro.c
  
  ADC_read();
  ADC_HUB();
  init_led();//循环显示LED并实现开机延时功能  
 
//主循环
  while(1)
  {    
		 run_led_flg++;
        if(run_led_flg>=30)
		{
		   SET_RUN_LED_ON;
		   delay_nms(1);	
		   run_led_flg=0;	   
		}
		
		//递推采样取平均值
		/*******************************	*/
        ADC_read(); //tlc2543.c
		    ADC_HUB();
		
		//处理串口收到的数据
		/*******************************	*/
        if(uart0_recv_flg == 0x55) uart0_Parse_data();  //uart.c
		
		//arc响应程序
		/*******************************	*/
	  arc_pro(); //main.c
		
	    //看门狗动作执行程序
		/*******************************	*/
		if	((XRAY_LED_PORT) && (WATCH_DOG_FLG == 0x55) && (WATCH_DOG_TIM>=100))
		     {SET_HV_OFF;
			  HV_ON_FLAG = 0;}
		//根据AD采样，ARC_FLG,XRAY_LED_LFG显示led，并开出m_error  
		/*******************************	*/
	   display_led();  //main.c
		/*******************************	*/
		SET_RUN_LED_OFF;
		time_pro();
	
   }
}
