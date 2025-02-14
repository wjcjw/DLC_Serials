
/******************************************************
��  ˾: ֣�������Ƽ���˾---����ʵ����
��д��: ���촺     
main.c��������ϵͳ��ʼ������ʱ���жϡ��ⲿ�жϡ�
Crystal: 7.3728Mhz 
Target : M16
ICC-AVR application builder : 2008-5-20 17:21:25
20090627  �����¶ȹ���
20091022  ���ӹ��ܣ������޹��绰֪ͨ���޸Ĳ�����������Ϊ��֡����
		  ����ԭͨ��Э�飬��Ӧ����
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
unsigned int  arc_num = 0;//�ر�xrayʱ����
unsigned int  arc_err_flg = 0;//�����λ���ⲿ�������

unsigned int  run_led_flg = 0;//

unsigned int  datset_flg = 0 ;//���ò���������澯����

unsigned long  arc_time1 = 0;//arc-1����ʱ��
unsigned long  arc_time2 = 0;//arc-2����ʱ��
unsigned long  arc_time3 = 0;//arc-3����ʱ��
unsigned long  arc_time4 = 0;//arc-4����ʱ��  
unsigned char xray_flg = 0;
unsigned char led_77 = 0;
unsigned char arc_tt=0;
unsigned long  init_t0=0;
unsigned int  xray_on_ms=0;//10���������
unsigned int xray_on_1m =0;//1���Ӽ�����
unsigned int xray_on_1s =0;//1���Ӽ�����
unsigned int  arc_number=0;
unsigned int  arc_number1=0;

unsigned long arc_led_tim = 0;//arc led on 1min
unsigned int ADC_HV = 0;
unsigned int ADC_IA = 0;
unsigned int ADC_TT = 0;
//********************************************
//������: display_led(void)
//��  ��: ����A/D,ARC,XRAY_LED_ON �������澯ָʾ
//��  ��: void
//����ֵ: void        
//ע  ��: err_flg��Ӧ��
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
//������: arc_pro(void)
//��  ��: ����arc�ж���Ӧ����
//��  ��: void
//����ֵ: void        
//ע  ��: arc_timer��Ӧ��
//*********************************************
void arc_pro(void)
{  	
	//arc �Իָ�����λ��
	if  (arc_led_tim>=3000)//�Իָ�Ϊ30��
	    {									
			 SET_ARC_LED_OFF;//arc������1������4�ηŵ����壩���ⲿ������������ָ�	 
	         FAULT_DATA &= 0xfb;  //arc_error
			 arc_num=0;
			 arc_led_tim=0;
         
	    }
	//arc�ж�ִ�г���
	if (arc_on_flg==0x55)
	  {		
			 
			  arc_on_flg=0; 
 			  datset_flg=40;
 			  arc_time1 = (((xray_on_1m*60)+xray_on_1s)*100+xray_on_ms);//��λ��10ms
 			  if((arc_time1-arc_time2)>=10)
 			  {    
   			      arc_num++;   // ָʾ��1��ָʾarc��������
    		      FAULT_DATA |= 0x04;  //arc_error
 			      SET_ARC_LED_ON;//arc������1������4�ηŵ����壩���ⲿ������������ָ�
 			      arc_led_tim=0;
				  if ((arc_num>=4)&&((arc_time1-arc_time4)<=1000))//10S�ڶ���4��
   			  	  {		       
						  SET_HV_OFF;	
						  HV_ON_FLAG = 0;
   				   }
				     arc_time4 = arc_time3; 
     		 	     arc_time3 = arc_time2;
     		 		 arc_time2 = arc_time1;//����arc_time2��3��4����ʱ��
  			  }
			  //����¼��	
			  star_lb_pro();
    		  
		} 
}
//********************************************
//������: time_pro(void)
//��  ��: ���ݶ�ʱ���ж���Ӧ����־ʱ�����
//��  ��: void
//����ֵ: void        
//ע  ��: xray_on_ms��Ӧ��
//*********************************************

void time_pro(void)
{
   unsigned long  init_t0=0;
   if(init_t0!=init_10ms)  //init_10ms������init_pro.c��
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
//������: void main(void)
//��  ��: ��Ƭ��ϵͳ������
//��  ��: void
//����ֵ: void        
//ע  ��: 
//*********************************************

void main(void)
{  
 unsigned char i = 0,j=1;
 
  init_devices() ;//init_pro.c
  
  SET_ERROR_OFF;
  
  init_xray();//init_pro.c
  
  ADC_read();
  ADC_HUB();
  init_led();//ѭ����ʾLED��ʵ�ֿ�����ʱ����  
 
//��ѭ��
  while(1)
  {    
		 run_led_flg++;
        if(run_led_flg>=30)
		{
		   SET_RUN_LED_ON;
		   delay_nms(1);	
		   run_led_flg=0;	   
		}
		
		//���Ʋ���ȡƽ��ֵ
		/*******************************	*/
        ADC_read(); //tlc2543.c
		    ADC_HUB();
		
		//�������յ�������
		/*******************************	*/
        if(uart0_recv_flg == 0x55) uart0_Parse_data();  //uart.c
		
		//arc��Ӧ����
		/*******************************	*/
	  arc_pro(); //main.c
		
	    //���Ź�����ִ�г���
		/*******************************	*/
		if	((XRAY_LED_PORT) && (WATCH_DOG_FLG == 0x55) && (WATCH_DOG_TIM>=100))
		     {SET_HV_OFF;
			  HV_ON_FLAG = 0;}
		//����AD������ARC_FLG,XRAY_LED_LFG��ʾled��������m_error  
		/*******************************	*/
	   display_led();  //main.c
		/*******************************	*/
		SET_RUN_LED_OFF;
		time_pro();
	
   }
}
