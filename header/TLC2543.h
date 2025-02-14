#ifndef _TLC2453_H
#define _TLC2543_H

extern void  tlc2543_init(void);
unsigned int tlc2543_read(unsigned char port);
void ADC_read(void);
#endif