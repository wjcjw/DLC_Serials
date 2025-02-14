#ifndef _ADC_H
#define _ADC_H
//定义ADC最大通道数
#define ADC_MAX_CHANNEL  4 
//定义采集的点数
#define ADC_COLLECT_NUM  100 



							

extern void adc_init(void);
extern void adc_start_fun(void);
#endif