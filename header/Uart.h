#ifndef _UART_H
#define _UART_H

//全局变量区
extern unsigned char uart0_recv_buf[30];
extern unsigned char uart0_recv_len    ;
extern unsigned char uart0_recv_count  ;
extern unsigned char uart0_recv_flg    ;
extern unsigned int  datset_flg;
extern unsigned char  HV_ON_FLAG;
extern unsigned int arc_number;
extern unsigned int arc_number1;

//函数区
extern void uart0_init(void);
extern void uart0_Parse_data(void);
extern void Uart0_String( char* buf,unsigned int len);
extern void Uart0_ConstString(const char* buf,unsigned int len);
extern unsigned char  EX_ADD_CRC(char* buf,unsigned char len);
extern unsigned char AsciiToHex(unsigned char num);
extern unsigned int HexToAscii(unsigned char num);

#endif