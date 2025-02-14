/**************************************************************
***************************************************************
公  司  :  郑州蓝晶科技公司电子实验室
编写者  :  张建国                      
日  期  :  2008-5-21 
Sysdef.h：定义系统必须的库头文件、宏定义、函数
编译器  : ICC-AVR application builder : 
处理器  : M128
系统时钟: 11.0592M
***************************************************************
***************************************************************/

#include <iom128v.h>
#include <macros.h>

#define TRUE  1
#define FALSE 0

#define SET(a,b) a|=(1<<b) 
#define CLR(a,b) a&=~(1<<b)

//设置最大值

//AD放大倍数
#define  AV_MUL_VAL   10
#define  AI_MUL_VAL   10




//LED 控制 定义 
#define LED_PUT_PORT     PORTA
//开入开出IO定义
#define OUTPUT_PORT  PORTC
#define SET_RUN_LED_ON   CLR(LED_PUT_PORT,0)
#define SET_RUN_LED_OFF  SET(LED_PUT_PORT,0)

#define SET_XRAY_LED_ON   CLR(LED_PUT_PORT,1)
#define SET_XRAY_LED_OFF  SET(LED_PUT_PORT,1)

#define SET_OC_LED_ON   CLR(LED_PUT_PORT,2)
#define SET_OC_LED_OFF  SET(LED_PUT_PORT,2)

#define SET_UC_LED_ON   CLR(LED_PUT_PORT,3)
#define SET_UC_LED_OFF  SET(LED_PUT_PORT,3)

#define SET_ARC_LED_ON   CLR(LED_PUT_PORT,4)
#define SET_ARC_LED_OFF  SET(LED_PUT_PORT,4)

#define SET_OT_LED_ON   CLR(LED_PUT_PORT,5)
#define SET_OT_LED_OFF  SET(LED_PUT_PORT,5)

#define SET_UV_LED_ON   CLR(LED_PUT_PORT,6)
#define SET_UV_LED_OFF  SET(LED_PUT_PORT,6)

#define SET_OV_LED_ON   CLR(LED_PUT_PORT,7)
#define SET_OV_LED_OFF  SET(LED_PUT_PORT,7)

//高压输出IO
#define SET_HV_OFF      CLR(OUTPUT_PORT,6)
#define SET_HV_ON       SET(OUTPUT_PORT,6)

//综合故障error输出
#define SET_ERROR_ON   CLR(OUTPUT_PORT,7)
#define SET_ERROR_OFF  SET(OUTPUT_PORT,7)

#define XRAY_LED_PORT   (PIND&0x02)

#define SAVE_ADDR  0x10
#define SAVE_vcur_val_ADDR  0x100
#define SAVE_icur_val_ADDR  0x110


typedef  unsigned char uchar;
typedef  unsigned int  uint;
typedef  unsigned long ulong;

extern unsigned int ADC_data[9] ;

//当前设置的高压和电流值:不能为int型
extern unsigned long vcur_val;
extern unsigned long icur_val;
extern unsigned long vcur_val1;
extern unsigned long icur_val1;
extern unsigned char FAULT_DATA ;
extern void delay_1us(void);                 //1us延时函数
extern void delay_nus(unsigned int n);       //N us延时函数
extern void delay_1ms(void) ;                //1ms延时函数
extern void delay_nms(unsigned int n) ;      //N ms延时函数

extern unsigned int  xray_on_ms;//10毫秒计数器
extern unsigned int  HexToAscii(unsigned char num);
extern unsigned char AsciiToHex(unsigned char num);

extern unsigned char Read_EEPROM(unsigned int addr);
extern void Write_EEPROM(unsigned char p_buff,unsigned int addr);

extern unsigned int ADC_IA_DATA[256];
extern unsigned int ADC_HV_DATA[256];

extern unsigned char ADC_lb_star;
extern unsigned char ADC_lb_SAVE;
extern unsigned char ADC_lb_flg;
extern void star_lb_pro(void);
extern unsigned int ADC_HV ;
extern unsigned int ADC_IA ;
extern unsigned int ADC_TT ;