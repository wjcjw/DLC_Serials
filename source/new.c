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
//������: display_led(void)
//��  ��: ����A/D,ARC,XRAY_LED_ON �������澯ָʾ
//��  ��: void
//����ֵ: void        
//ע  ��: err_flg��Ӧ��
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
									SET_OC_LED_ON;//OC����������ֵ�����趨ֵ��18%�����ⲿ������������ָ�
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
												SET_UC_LED_ON;//UC����������ֵС���趨ֵ��18%�����ⲿ������������ָ�
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
													SET_OV_LED_ON;//OV��������ѹֵ����9%�����ⲿ������������ָ�
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
													SET_UV_LED_ON;//UV��������ѹֵС���趨ֵ��9%�����ⲿ������������ָ�
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
  
   if(HV_ON_FLAG == 1) //���߿����ж� PD1��״̬
   { 
       SET_XRAY_LED_ON;  //���ߵƿ�
	 
	
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
 
if	 (ADC_TT>=333)				//333��Ӧ60��  338��Ӧ65��			  
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
						 EIMSK = 0x00;//��arc�ⲿ�ж�
						 EIFR=0x01;//���ⲿ�ж�0�ı�־λ��д1��
						 TCCR3B=0x04;//����ʱ��3
 
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
					 EIMSK = 0x00;//��arc�ⲿ�ж�  
					 EIFR=0x01;//���ⲿ�ж�0�ı�־λ��д1��
					 HV_ON_FLAG=0;
 
 }		