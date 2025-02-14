#include "Sysdef.h"
#include "App_para.h"
#include "Uart.h"
#include "init_pro.h"
#include "TLC5618.h"
#include "TLC2543.h"

//射线开计时器 0xffffffffs
unsigned int  init_10ms = 0;//10ms
unsigned int  arc_on_flg = 0;//1秒钟计数器
//********************************************
//函数名: void int0_isr(void)
//描  述: 单片机外部中断0 触发中断服务函数
//参  数: void
//功  能：记录中断标志，保存中断发生时间，判定ARC告警信号
//返回值: void        
//注  意: 
//*********************************************



#pragma interrupt_handler int0_isr:2
void int0_isr(void)
{
 
 //stop errant interrupts until set up
  CLI(); 
  arc_on_flg=0x55; 
  arc_number++; //监控ARC次数显示变量
  SEI(); 

 //all peripherals are now initialized
}


//********************************************
//函数名: void int1_isr(void)
//描  述: 单片机外部中断1 触发中断服务函数
//参  数: void
//功  能：记录中断标志，保存中断发生时间，判定xray_on信号
//返回值: void        
//注  意: 
//*********************************************

unsigned char int1_flg =0;

#pragma interrupt_handler int1_isr:3
void int1_isr(void)
{
 //stop errant interrupts until set up
 
 //all peripherals are now initialized
}
//********************************************
//函数名: void timer0_init(void)
//描  述: 单片机定时器0初始化
//参  数: void
//返回值: void        
//注  意: 
//*********************************************
void timer0_init(void)
{


}

//********************************************
//函数名: void timer0_ovf_isr(void)
//描  述: 单片机定时器0溢出中断服务函数
//参  数: void
//返回值: void        
//注  意: 
//*********************************************
#pragma interrupt_handler timer0_ovf_isr:17
void timer0_ovf_isr(void)
{
 TCNT0 = 0x00 /*INVALID SETTING*/; //reload counter value
}


//********************************************
//函数名: void timer1_init(void)
//描  述: 单片机定时器1初始化
//参  数: void
//返回值: void        
//注  意: 8分频
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
//函数名: void timer1_ovf_isr(void)
//描  述: 单片机定时器1溢出中断服务函数
//参  数: void
//返回值: void        
//注  意: 定时10ms
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
//函数名: void timer2_init(void)
//描  述: 单片机定时器2初始化
//参  数: void
//返回值: void        
//注  意: 
//*********************************************
void timer2_init(void)
{
 TCCR2 = 0x00; //stop
 TCNT2 = 0x00 /*INVALID SETTING*/; //setup
 OCR2  = 0x00 /*INVALID SETTING*/;
 TCCR2 = 0x00; //start
}

//********************************************
//函数名: void timer2_ovf_isr(void)
//描  述: 单片机定时器2溢出中断服务函数
//参  数: void
//返回值: void        
//注  意: 
//*********************************************
#pragma interrupt_handler timer2_ovf_isr:11
void timer2_ovf_isr(void)
{
 TCNT2 = 0x00 /*INVALID SETTING*/; //reload counter value
}


//********************************************
//函数名: void timer3_init(void)
//描  述: 单片机定时器3初始化
//参  数: void
//返回值: void        
//注  意: arc延时功能
//*********************************************
void timer3_init(void)
{ TCCR3A = 0x00;//普通模式
 //TCCR3B = 0x04; //256分频
 TCNT3H = 0x58 /*INVALID SETTING*/; //setup
 TCNT3L = 0x28 /*INVALID SETTING*/;  //装入数据1s
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
//函数名: void timer3_ovf_isr(void)
//描  述: 单片机定时器3溢出中断服务函数
//参  数: void
//返回值: void        
//注  意: 
//*********************************************
#pragma interrupt_handler timer3_ovf_isr:30
void timer3_ovf_isr(void)
{EIFR=0x01;//清外部中断0的标志位，写1清
EIMSK = 0x01;//开arc外部中断
 //TIMER3 has overflowed
 TCNT3H = 0x58 /*INVALID SETTING*/; //setup
 TCNT3L = 0x28 /*INVALID SETTING*/;  //装入数据
 TCCR3B=0x00;//关定时器3
}


//********************************************
//函数名: void init_devices(void)
//描  述: 单片机系统初始化
//参  数: void
//返回值: void        
//注  意: 
//*********************************************

void init_devices(void)
{
 //stop errant interrupts until set up
 CLI(); //disable all interrupts
 XDIV  = 0x00; //晶振分频控制寄存器00-无分频
 XMCRA = 0x00; //external memory控制寄存器
 port_init();  //pA--pG预定义
 timer0_init();
 timer1_init();
 timer2_init();
 timer3_init();
 uart0_init();  //串口初始化：8bit 9600bps o-check 
 tlc2543_init();//AD
 tlc5618_init();//DA
 MCUCR = 0x00;  //无外部RAM； 无睡眠模式；中断向量指向FLASH起始地址
 EICRA = 0x02;  //extended ext ints  INT0下降沿沿产生中断
 EICRB = 0x00;  //extended ext ints  INT1-INT7 无外部中断定义
 EIMSK = 0x00;  //外部中断屏蔽寄存器 int0，int1 enable
 TIMSK = 0x45;  //timer interrupt sources定时器1中断屏蔽寄存器
                //t/c2溢出中断使能，t/c1溢出中断使能，t/c0溢出中断使能
 ETIMSK = 0x04; //extended timer interrupt sources扩展  定时器中断屏蔽寄存器
                //t/c3溢出中断使能
 SEI(); //re-enable interrupts
 //all peripherals are now initialized
}

//********************************************
//函数名: void init_xray(void)
//描  述: X射线初始化=>发复位命令.关高压,初始化高压和束流值
//参  数: void
//返回值: void        
//注  意: 
//*********************************************
void init_xray(void)
{ 
  SET_HV_OFF   ;             //关高压
  delay_nms(5);
  tlc5618_output(0,VNOR_VAL*V_DAC_MODULUS/VSET_MAX);
  delay_nms(2);
  tlc5618_output(1,INOR_VAL*I_DAC_MODULUS/ISET_MAX);
}

//********************************************
//函数名: void port_init(void)
//描  述: 单片机IO口初始化
//参  数: void
//返回值: void        
//注  意: 
//*********************************************
void port_init(void)
{
/*
DDxn   PORTxn   PUD (in SFIOR)   I/O   上拉电阻说明
0   0   X   输入   No 高阻态 (Hi-Z)
0   1   0   输入   Yes被外部电路拉低时将输出电流
0   1   1   输入   No高阻态(Hi-Z)
1   0   X   输出   No输出低电平 ( 漏电流)
1   1   X   输出   No输出高电平 ( 源电流)*/

  // PORTA  0 开入（空闲） 1:7 LED 开出
  //  PA0     PA1     PA2     PA3      PA4     PA5     PA6    PA7
 //  NULL  XRAY_ON  OC_LED  UC_LED  ARC_LED  OT_LED   UV_LED  OV_LED
 PORTA = 0xFF;//d0     1个开入
 DDRA  = 0xFF;//d1-d7  7个开出
 //  PB0-7    
 //  NULL  
 PORTB = 0x00;
 DDRB  = 0x00;
 //  PC0     PC1    PC2    PC3    PC4          PC6    PC7
 //AD_ECO  AD_CLK  AD_DI  AD_DO  AD_CS       HV_SW    ERROR
 PORTC = 0xFF; //m103 output only
 DDRC  = 0xf6; //4个开出
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
//函数名: init_led(void)
//描  述: 根据初始化结果，点亮告警指示
//参  数: void
//返回值: void        
//注  意: err_flg的应用
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
