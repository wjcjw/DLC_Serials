#ifndef _INIT_H
#define _INIT_H

//全局变量区
extern unsigned char uart0_recv_buf[30];
extern unsigned int  arc_num;
extern unsigned int  arc_err_flg;
extern unsigned char WATCH_DOG_FLG ;
extern unsigned char WATCH_DOG_TIM ;

extern unsigned int  init_10ms;//10毫秒计数器
extern unsigned int  arc_on_flg ;//1秒钟计数器

//函数区
extern void port_init(void);
extern void int0_isr(void);
extern void int1_isr(void);
extern void timer0_init(void);
extern void timer0_ovf_isr(void);
extern void timer1_init(void);
extern void timer1_ovf_isr(void);
extern void timer2_init(void);
extern void timer2_ovf_isr(void);
extern void timer3_init(void);
extern void timer3_ovf_isr(void);
extern void init_devices(void);
extern void init_xray(void);
extern void init_led(void);

#endif