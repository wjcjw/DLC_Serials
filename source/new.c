#include "new.h"
#include "Sysdef.h"
#include "App_para.h"
#include "uart.h"
#include "init_pro.h"


unsigned char oc_tt=0;
unsigned char uc_tt=0;
unsigned char ov_tt=0;
unsigned char uv_tt=0;
unsigned char ot_tt=0;
unsigned char ot_ttl=0;
unsigned char Alarm_num=0;
unsigned char Alarm_UV_Flag=0;
unsigned char Alarm_OV_Flag=0;
unsigned char Alarm_OC_Flag=0;
unsigned char Alarm_UC_Flag=0;

//********************************************
//函数名: display_led(void)
//描  述: 根据A/D,ARC,XRAY_LED_ON ，点亮告警指示
//参  数: void
//返回值: void        
//注  意: err_flg的应用
//*********************************************
void display_led()
{    unsigned long  temp_d=0;
	if(Alarm_UV_Flag>=1 || Alarm_OV_Flag>=1 || Alarm_UC_Flag>=1 || Alarm_OC_Flag>=1  )
	{
		Alarm_num++;
		if(Alarm_num>=200)
		Alarm_num=200;
	}
   	if(tiaoshi==0)
	 {
      if( HV_ON_FLAG )  
      {  
         temp_d=icur_val1;
	  	   temp_d=(temp_d*118/100);
      	 if(ADC_IA>=temp_d)	  
      	 {
							oc_tt++;
							if(oc_tt>=50)
							{	
								HV_ON_FLAG = 0;
								SET_HV_OFF;
								oc_tt=0;								
								Alarm_OC_Flag++;
								if(Alarm_OC_Flag>=2)
								{
									open_num=0;
									Alarm_OC_Flag=0;
									FAULT_DATA |= 0x08;  //High mA_error
									SET_OC_LED_ON;//OC动作（电流值大于设定值的18%），外部命令错误清除后恢复
								}							
							}
          }
	      else  oc_tt=0;
	      temp_d=icur_val1;
	      temp_d=(temp_d *82/100);
	      
          if(ADC_IA<=temp_d)
          {
									uc_tt++;
									if(uc_tt>=50)
									{
										uc_tt=0;
										Alarm_UC_Flag++;
										if(Alarm_UC_Flag>=2)
										{	
												Alarm_UC_Flag=0;
												FAULT_DATA |= 0x10;  //Low mA_error
												SET_UC_LED_ON;//UC动作（电流值小于设定值的18%），外部命令错误清除后恢复
										}
									}
           }
	  	   else  uc_tt=0;
      	  // temp_d=VALRM_MAX;
		  temp_d=vcur_val1;
	  	  temp_d=(temp_d*109/100);
      	   if(ADC_HV>=(temp_d))
           {
										ov_tt++;
										if(ov_tt>=50)
										{
												HV_ON_FLAG = 0;
												SET_HV_OFF;										
												ov_tt=0;
												Alarm_OV_Flag++;
												if(Alarm_OV_Flag>=2)
												{
													open_num=0;													
													Alarm_OV_Flag=0;
													FAULT_DATA |= 0x40;  //High kV_error
													SET_OV_LED_ON;//OV动作（电压值大于9%），外部命令错误清除后恢复
												}
							
										}
      		}
	  		else  ov_tt=0;
	  		temp_d=vcur_val1;
	  		temp_d=(temp_d*91/100);
	  		
      		if(ADC_HV<=(temp_d))
      		{
								uv_tt++;
								if(uv_tt>=50)
								{
										HV_ON_FLAG = 0;
										SET_HV_OFF;				
										uv_tt=0;
										Alarm_UV_Flag++;
										if(Alarm_UV_Flag>=2)
										{
													open_num=0;																									
													Alarm_UV_Flag=0;
													FAULT_DATA |= 0x20;  //Low kV_error
													SET_UV_LED_ON;//UV动作（电压值小于设定值的9%），外部命令错误清除后恢复
										}
								}
      		}
	  		else  uv_tt=0;      		 	  
   		}
			else
			{
					oc_tt=0;
					uc_tt=0;
					ov_tt=0;
					uv_tt=0;
			}
			}
  
   if(HV_ON_FLAG == 1) //射线开关判断 PD1口状态
   { 
       SET_XRAY_LED_ON;  //射线灯开
	 
	
    }  
		
   else
				{
					SET_XRAY_LED_OFF ;
					arc_num=0 ;
					xray_on_1s=0;
					xray_on_1m=0;
					xray_on_ms = 0;
					WATCH_DOG_TIM=0;
		
				}
 
if	 (ADC_TT>=333)				//333对应60度  338对应65度			  
{  
          ot_tt++;
          if(ot_tt>=33) 
	      {
	                FAULT_DATA |= 0x02;  
	  	        	SET_OT_LED_ON;
					SET_HV_OFF;
					HV_ON_FLAG = 0;
					ot_tt=0;
	      }
 }
 else 
 {
	        if(ADC_TT<=330)
					{
						ot_ttl++;
						if(ot_ttl>=33)
								{
										SET_OT_LED_OFF;  
										ot_ttl=0;
										FAULT_DATA   &= 0xfd;
								}		
					}
 }   
}
 void HV_ON()
 {
  					 	SET_HV_ON;  
						 HV_ON_FLAG=1;
						 EIMSK = 0x00;//关arc外部中断
						 EIFR=0x01;//清外部中断0的标志位，写1清
						 TCCR3B=0x04;//开定时器3
 
 }				  	
 void HV_OFF()
 {
  	  		
  	  		         Alarm_num=0;
 		             Alarm_UV_Flag=0;
		             Alarm_OV_Flag=0;
		             Alarm_OC_Flag=0;
		  			 Alarm_UC_Flag=0; 	  		  		
  	  		  		 open_num=0;
				     SET_HV_OFF; 
					 EIMSK = 0x00;//关arc外部中断  
					 EIFR=0x01;//清外部中断0的标志位，写1清
					 HV_ON_FLAG=0;
 
 }		