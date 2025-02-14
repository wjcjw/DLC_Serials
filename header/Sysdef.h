/**************************************************************
***************************************************************
��  ˾  :  ֣�������Ƽ���˾����ʵ����
��д��  :  �Ž���                      
��  ��  :  2008-5-21 
Sysdef.h������ϵͳ����Ŀ�ͷ�ļ����궨�塢����
������  : ICC-AVR application builder : 
������  : M128
ϵͳʱ��: 11.0592M
***************************************************************
***************************************************************/

#include <iom128v.h>
#include <macros.h>

#define TRUE  1
#define FALSE 0

#define SET(a,b) a|=(1<<b) 
#define CLR(a,b) a&=~(1<<b)

//�������ֵ

//AD�Ŵ���
#define  AV_MUL_VAL   10
#define  AI_MUL_VAL   10




//LED ���� ���� 
#define LED_PUT_PORT     PORTA
//���뿪��IO����
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

//��ѹ���IO
#define SET_HV_OFF      CLR(OUTPUT_PORT,6)
#define SET_HV_ON       SET(OUTPUT_PORT,6)

//�ۺϹ���error���
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

//��ǰ���õĸ�ѹ�͵���ֵ:����Ϊint��
extern unsigned long vcur_val;
extern unsigned long icur_val;
extern unsigned long vcur_val1;
extern unsigned long icur_val1;
extern unsigned char FAULT_DATA ;
extern void delay_1us(void);                 //1us��ʱ����
extern void delay_nus(unsigned int n);       //N us��ʱ����
extern void delay_1ms(void) ;                //1ms��ʱ����
extern void delay_nms(unsigned int n) ;      //N ms��ʱ����

extern unsigned int  xray_on_ms;//10���������
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