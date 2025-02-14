#include "Sysdef.h"
#include "App_para.h"
#include "Uart.h"
#include "init_pro.h"
#include "TLC5618.h"
#include "TLC2543.h"

//���߿���ʱ�� 0xffffffffs
unsigned int  init_10ms = 0;//10ms
unsigned int  arc_on_flg = 0;//1���Ӽ�����
//********************************************
//������: void int0_isr(void)
//��  ��: ��Ƭ���ⲿ�ж�0 �����жϷ�����
//��  ��: void
//��  �ܣ���¼�жϱ�־�������жϷ���ʱ�䣬�ж�ARC�澯�ź�
//����ֵ: void        
//ע  ��: 
//*********************************************



#pragma interrupt_handler int0_isr:2
void int0_isr(void)
{
 
 //stop errant interrupts until set up
  CLI(); 
  arc_on_flg=0x55; 
  arc_number++; //���ARC������ʾ����
  SEI(); 

 //all peripherals are now initialized
}


//********************************************
//������: void int1_isr(void)
//��  ��: ��Ƭ���ⲿ�ж�1 �����жϷ�����
//��  ��: void
//��  �ܣ���¼�жϱ�־�������жϷ���ʱ�䣬�ж�xray_on�ź�
//����ֵ: void        
//ע  ��: 
//*********************************************

unsigned char int1_flg =0;

#pragma interrupt_handler int1_isr:3
void int1_isr(void)
{
 //stop errant interrupts until set up
 
 //all peripherals are now initialized
}
//********************************************
//������: void timer0_init(void)
//��  ��: ��Ƭ����ʱ��0��ʼ��
//��  ��: void
//����ֵ: void        
//ע  ��: 
//*********************************************
void timer0_init(void)
{


}

//********************************************
//������: void timer0_ovf_isr(void)
//��  ��: ��Ƭ����ʱ��0����жϷ�����
//��  ��: void
//����ֵ: void        
//ע  ��: 
//*********************************************
#pragma interrupt_handler timer0_ovf_isr:17
void timer0_ovf_isr(void)
{
 TCNT0 = 0x00 /*INVALID SETTING*/; //reload counter value
}


//********************************************
//������: void timer1_init(void)
//��  ��: ��Ƭ����ʱ��1��ʼ��
//��  ��: void
//����ֵ: void        
//ע  ��: 8��Ƶ
//*********************************************
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xCA /*INVALID SETTING*/; //setup
 TCNT1L = 0x01 /*INVALID SETTING*/;
 OCR1AH = 0x00 /*INVALID SETTING*/;
 OCR1AL = 0x00 /*INVALID SETTING*/;
 OCR1BH = 0x00 /*INVALID SETTING*/;
 OCR1BL = 0x00 /*INVALID SETTING*/;
 OCR1CH = 0x00 /*INVALID SETTING*/;
 OCR1CL = 0x00 /*INVALID SETTING*/;
 ICR1H  = 0x00 /*INVALID SETTING*/;
 ICR1L  = 0x00 /*INVALID SETTING*/;
 TCCR1A = 0x00;
 TCCR1B = 0x02; //start Timer
}


//********************************************
//������: void timer1_ovf_isr(void)
//��  ��: ��Ƭ����ʱ��1����жϷ�����
//��  ��: void
//����ֵ: void        
//ע  ��: ��ʱ10ms
//*********************************************


#pragma interrupt_handler timer1_ovf_isr:15
void timer1_ovf_isr(void)
{
 //TIMER1 has overflowed
 TCNT1H = 0xCA /*INVALID SETTING*/; //reload counter high value
 TCNT1L = 0x01 /*INVALID SETTING*/; //reload counter low value
 
   init_10ms++; 
   if(init_10ms>=100)  init_10ms=0;  
  
}

//********************************************
//������: void timer2_init(void)
//��  ��: ��Ƭ����ʱ��2��ʼ��
//��  ��: void
//����ֵ: void        
//ע  ��: 
//*********************************************
void timer2_init(void)
{
 TCCR2 = 0x00; //stop
 TCNT2 = 0x00 /*INVALID SETTING*/; //setup
 OCR2  = 0x00 /*INVALID SETTING*/;
 TCCR2 = 0x00; //start
}

//********************************************
//������: void timer2_ovf_isr(void)
//��  ��: ��Ƭ����ʱ��2����жϷ�����
//��  ��: void
//����ֵ: void        
//ע  ��: 
//*********************************************
#pragma interrupt_handler timer2_ovf_isr:11
void timer2_ovf_isr(void)
{
 TCNT2 = 0x00 /*INVALID SETTING*/; //reload counter value
}


//********************************************
//������: void timer3_init(void)
//��  ��: ��Ƭ����ʱ��3��ʼ��
//��  ��: void
//����ֵ: void        
//ע  ��: arc��ʱ����
//*********************************************
void timer3_init(void)
{ TCCR3A = 0x00;//��ͨģʽ
 //TCCR3B = 0x04; //256��Ƶ
 TCNT3H = 0x58 /*INVALID SETTING*/; //setup
 TCNT3L = 0x28 /*INVALID SETTING*/;  //װ������1s
 OCR3AH = 0x00 /*INVALID SETTING*/;
 OCR3AL = 0x00 /*INVALID SETTING*/;
 OCR3BH = 0x00 /*INVALID SETTING*/;
 OCR3BL = 0x00 /*INVALID SETTING*/;
 OCR3CH = 0x00 /*INVALID SETTING*/;
 OCR3CL = 0x00 /*INVALID SETTING*/;
 ICR3H  = 0x00 /*INVALID SETTING*/;
 ICR3L  = 0x00 /*INVALID SETTING*/;
  TCCR3B=0x00;//stop


}

//********************************************
//������: void timer3_ovf_isr(void)
//��  ��: ��Ƭ����ʱ��3����жϷ�����
//��  ��: void
//����ֵ: void        
//ע  ��: 
//*********************************************
#pragma interrupt_handler timer3_ovf_isr:30
void timer3_ovf_isr(void)
{EIFR=0x01;//���ⲿ�ж�0�ı�־λ��д1��
EIMSK = 0x01;//��arc�ⲿ�ж�
 //TIMER3 has overflowed
 TCNT3H = 0x58 /*INVALID SETTING*/; //setup
 TCNT3L = 0x28 /*INVALID SETTING*/;  //װ������
 TCCR3B=0x00;//�ض�ʱ��3
}


//********************************************
//������: void init_devices(void)
//��  ��: ��Ƭ��ϵͳ��ʼ��
//��  ��: void
//����ֵ: void        
//ע  ��: 
//*********************************************

void init_devices(void)
{
 //stop errant interrupts until set up
 CLI(); //disable all interrupts
 XDIV  = 0x00; //�����Ƶ���ƼĴ���00-�޷�Ƶ
 XMCRA = 0x00; //external memory���ƼĴ���
 port_init();  //pA--pGԤ����
 timer0_init();
 timer1_init();
 timer2_init();
 timer3_init();
 uart0_init();  //���ڳ�ʼ����8bit 9600bps o-check 
 tlc2543_init();//AD
 tlc5618_init();//DA
 MCUCR = 0x00;  //���ⲿRAM�� ��˯��ģʽ���ж�����ָ��FLASH��ʼ��ַ
 EICRA = 0x02;  //extended ext ints  INT0�½����ز����ж�
 EICRB = 0x00;  //extended ext ints  INT1-INT7 ���ⲿ�ж϶���
 EIMSK = 0x00;  //�ⲿ�ж����μĴ��� int0��int1 enable
 TIMSK = 0x45;  //timer interrupt sources��ʱ��1�ж����μĴ���
                //t/c2����ж�ʹ�ܣ�t/c1����ж�ʹ�ܣ�t/c0����ж�ʹ��
 ETIMSK = 0x04; //extended timer interrupt sources��չ  ��ʱ���ж����μĴ���
                //t/c3����ж�ʹ��
 SEI(); //re-enable interrupts
 //all peripherals are now initialized
}

//********************************************
//������: void init_xray(void)
//��  ��: X���߳�ʼ��=>����λ����.�ظ�ѹ,��ʼ����ѹ������ֵ
//��  ��: void
//����ֵ: void        
//ע  ��: 
//*********************************************
void init_xray(void)
{ 
  SET_HV_OFF   ;             //�ظ�ѹ
  delay_nms(5);
  tlc5618_output(0,VNOR_VAL*V_DAC_MODULUS/VSET_MAX);
  delay_nms(2);
  tlc5618_output(1,INOR_VAL*I_DAC_MODULUS/ISET_MAX);
}

//********************************************
//������: void port_init(void)
//��  ��: ��Ƭ��IO�ڳ�ʼ��
//��  ��: void
//����ֵ: void        
//ע  ��: 
//*********************************************
void port_init(void)
{
/*
DDxn   PORTxn   PUD (in SFIOR)   I/O   ��������˵��
0   0   X   ����   No ����̬ (Hi-Z)
0   1   0   ����   Yes���ⲿ��·����ʱ���������
0   1   1   ����   No����̬(Hi-Z)
1   0   X   ���   No����͵�ƽ ( ©����)
1   1   X   ���   No����ߵ�ƽ ( Դ����)*/

  // PORTA  0 ���루���У� 1:7 LED ����
  //  PA0     PA1     PA2     PA3      PA4     PA5     PA6    PA7
 //  NULL  XRAY_ON  OC_LED  UC_LED  ARC_LED  OT_LED   UV_LED  OV_LED
 PORTA = 0xFF;//d0     1������
 DDRA  = 0xFF;//d1-d7  7������
 //  PB0-7    
 //  NULL  
 PORTB = 0x00;
 DDRB  = 0x00;
 //  PC0     PC1    PC2    PC3    PC4          PC6    PC7
 //AD_ECO  AD_CLK  AD_DI  AD_DO  AD_CS       HV_SW    ERROR
 PORTC = 0xFF; //m103 output only
 DDRC  = 0xf6; //4������
 //  PD0         PD1         PD2-PD4     PD5           PD6          PD7
 //INT0_ARC  INT1_XRAYLED     NULL     TLC5618_CS  TLC5618_CLK  TLC5618_DIN
 PORTD = 0x00;
 DDRD  = 0x00;
 //   PE0          PE1     PE2 - PE7
 //  RS232_RXD  RS232_TXD    NULL
 PORTE = 0xFF;
 DDRE  = 0xFC;
 //JATG INTFACE
 PORTF = 0x11;
 DDRF  = 0x11;
 //NULL
 PORTG = 0x00;
 DDRG  = 0x03;
}
//********************************************
//������: init_led(void)
//��  ��: ���ݳ�ʼ������������澯ָʾ
//��  ��: void
//����ֵ: void        
//ע  ��: err_flg��Ӧ��
//*********************************************
#define EEPROM_ADDR      20
void init_led()
{
  unsigned char i=0,uc =0;  
  
    for(uc=7;uc>0;uc--)
    {
      CLR(LED_PUT_PORT,uc);   
      delay_nms(150);
      SET(LED_PUT_PORT,uc);
	  delay_nms(150);	 
	 }  
}
