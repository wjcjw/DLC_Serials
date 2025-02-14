#ifndef _NEW_H
#define _NEW_H
#define SBBH_ADDR 0
#define data_rep_count 10
#define FAIL -1

typedef unsigned char   u8;     //  8 bits 
typedef unsigned int    u16;    // 16 bits 
typedef unsigned long   u32;    // 32 bits 
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef long int32_t;
typedef unsigned long uint32_t;

extern unsigned char CRC8_Table(unsigned char *p,char counter);
extern void EEPROM_write_n(uint32_t EE_address,uint8_t *DataAddress,uint16_t number);
extern void EEPROM_read_n(uint32_t EE_address,uint8_t *DataAddress,uint16_t number);
extern u32 EEPROM_READDATA_CRC(u32 Read_addr,u8 count) ;
extern void HV_ON(void);
extern void HV_OFF(void);
extern void display_led(void);
extern unsigned int xray_on_1m;//1分钟计数器
extern unsigned int xray_on_1s;//1秒钟计数器
extern unsigned char Alarm_num;
extern unsigned char Alarm_UV_Flag;
extern unsigned char Alarm_OV_Flag;
extern unsigned char Alarm_OC_Flag;
extern unsigned char Alarm_UC_Flag;
extern unsigned char open_num;
#endif