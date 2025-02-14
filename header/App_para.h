#define HIGH_HV  803
#if (HIGH_HV == 80)
//设定最大值
#define VSET_MAX    800  //200KV
#define ISET_MAX    1000    //1500uA
#define VI_MAX      VSET_MAX*ISET_MAX   //

//设置最小值
#define VSET_MIN     400
#define ISET_MIN     300

//开机默认值
#define VNOR_VAL   600
#define INOR_VAL   500
//ADC 计算系数
#define V_ADC_MODULUS    2800   //0～4000mv   对应 0～VSET_MAX电压
#define T_ADC_MODULUS   900//0～4000mv   对应 0～VSET_MAX电压
#define I_ADC_MODULUS    2800 //3000  //0～4500mv   对应 0～ISET_MAX电流
//DAC 计算系数
#define I_DAC_MODULUS    2800 //0～2400mv   对应 0～ISET_MAX电流
#define V_DAC_MODULUS    2400  //0～2400mv   对应 0～VSET_MAX电压

#elif (HIGH_HV == 801)
//设定最大值
#define VSET_MAX    800  //200KV
#define ISET_MAX    1000    //1500uA
#define VI_MAX      VSET_MAX*ISET_MAX   //

//设置最小值
#define VSET_MIN     1
#define ISET_MIN     1

//开机默认值
#define VNOR_VAL   600
#define INOR_VAL   500
//ADC 计算系数
#define V_ADC_MODULUS    2800   //0～4000mv   对应 0～VSET_MAX电压
#define T_ADC_MODULUS   900//0～4000mv   对应 0～VSET_MAX电压
#define I_ADC_MODULUS    2800 //3000  //0～4500mv   对应 0～ISET_MAX电流
//DAC 计算系数
#define I_DAC_MODULUS    2800 //0～2400mv   对应 0～ISET_MAX电流
#define V_DAC_MODULUS    2400  //0～2400mv   对应 0～VSET_MAX电压
#elif (HIGH_HV == 802)
//设定最大值
#define VSET_MAX    800  //200KV
#define ISET_MAX    1000    //1500uA
#define VI_MAX      VSET_MAX*ISET_MAX   //

//设置最小值
#define VSET_MIN     400
#define ISET_MIN     300

//开机默认值
#define VNOR_VAL   600
#define INOR_VAL   500
//ADC 计算系数
#define V_ADC_MODULUS    2800   //0～4000mv   对应 0～VSET_MAX电压
#define T_ADC_MODULUS   900//0～4000mv   对应 0～VSET_MAX电压
#define I_ADC_MODULUS    1633 //3000  //0～4500mv   对应 0～ISET_MAX电流
//DAC 计算系数
#define I_DAC_MODULUS    1633 //0～2400mv   对应 0～ISET_MAX电流
#define V_DAC_MODULUS    2400  //0～2400mv   对应 0～VSET_MAX电压
#elif (HIGH_HV == 803)
//设定最大值
#define VSET_MAX    800  //200KV
#define ISET_MAX    1500    //1500uA
#define VI_MAX      VSET_MAX*ISET_MAX   //

//设置最小值
#define VSET_MIN     400
#define ISET_MIN     300

//开机默认值
#define VNOR_VAL   600
#define INOR_VAL   500
//ADC 计算系数
#define V_ADC_MODULUS    1400   //0～4000mv   对应 0～VSET_MAX电压
#define T_ADC_MODULUS  	 900//0～4000mv   对应 0～VSET_MAX电压
#define I_ADC_MODULUS    4200 //3000  //0～4500mv   对应 0～ISET_MAX电流
//DAC 计算系数
#define I_DAC_MODULUS    4200 //0～2400mv   对应 0～ISET_MAX电流
#define V_DAC_MODULUS    2400  //0～2400mv   对应 0～VSET_MAX电压

#elif   (HIGH_HV == 120)

//设定最大值
#define VSET_MAX    1200  //120KV
#define ISET_MAX    1000    //1mA
#define VI_MAX      VSET_MAX*ISET_MAX   //

//设置最小值
#define VSET_MIN     800
#define ISET_MIN     100

//开机默认值
#define VNOR_VAL   1000
#define INOR_VAL   500
//ADC 计算系数
#define V_ADC_MODULUS    2800   //0～4000mv   对应 0～VSET_MAX电压
#define T_ADC_MODULUS   900//0～4000mv   对应 0～VSET_MAX电压
#define I_ADC_MODULUS    2800 //3000  //0～4500mv   对应 0～ISET_MAX电流
//DAC 计算系数
#define I_DAC_MODULUS    2800 //0～2400mv   对应 0～ISET_MAX电流
#define V_DAC_MODULUS    2800  //0～2400mv   对应 0～VSET_MAX电压

#elif   (HIGH_HV == 100)
//设定最大值
#define VSET_MAX    1000  //200KV
#define ISET_MAX    1000    //1500uA
#define VI_MAX      VSET_MAX*ISET_MAX   //
//设置最小值
#define VSET_MIN     800
#define ISET_MIN     300
//开机默认值
#define VNOR_VAL   800
#define INOR_VAL   500
//ADC 计算系数
#define V_ADC_MODULUS    2800   //0～4000mv   对应 0～VSET_MAX电压
#define T_ADC_MODULUS   900//0～4000mv   对应 0～VSET_MAX电压
#define I_ADC_MODULUS    2800 //3000  //0～4500mv   对应 0～ISET_MAX电流
//DAC 计算系数
#define I_DAC_MODULUS    2800 //0～2400mv   对应 0～ISET_MAX电流
#define V_DAC_MODULUS    2800  //0～2400mv   对应 0～VSET_MAX电

#elif   (HIGH_HV == 140)
//设定最大值
#define VSET_MAX    1400  //200KV
#define ISET_MAX    800    //1500uA
#define VI_MAX      VSET_MAX*ISET_MAX   //
//设置最小值
#define VSET_MIN     1300
#define ISET_MIN     300
//开机默认值
#define VNOR_VAL   1400
#define INOR_VAL   500
//ADC 计算系数
#define V_ADC_MODULUS    3000   //0～4000mv   对应 0～VSET_MAX电压
#define T_ADC_MODULUS   900//0～4000mv   对应 0～VSET_MAX电压
#define I_ADC_MODULUS    2200 //3000  //0～4500mv   对应 0～ISET_MAX电流
//DAC 计算系数
#define I_DAC_MODULUS    2800 //0～2400mv   对应 0～ISET_MAX电流
#define V_DAC_MODULUS    2400  //0～2400mv   对应 0～VSET_MAX电压 
#elif   (HIGH_HV == 141)
//设定最大值
#define VSET_MAX    1540  //200KV
#define ISET_MAX    1200    //1500uA
#define VI_MAX      VSET_MAX*ISET_MAX   //
//设置最小值
#define VSET_MIN     1300
#define ISET_MIN     300
//开机默认值
#define VNOR_VAL   1400
#define INOR_VAL   500
//ADC 计算系数
#define V_ADC_MODULUS    3000   //0～4000mv   对应 0～VSET_MAX电压
#define T_ADC_MODULUS   900//0～4000mv   对应 0～VSET_MAX电压
#define I_ADC_MODULUS    2200 //3000  //0～4500mv   对应 0～ISET_MAX电流
//DAC 计算系数
#define I_DAC_MODULUS    2800 //0～2400mv   对应 0～ISET_MAX电流
#define V_DAC_MODULUS    2400  //0～2400mv   对应 0～VSET_MAX电压 
#endif

//告警电压过压  
#define VALRM_MAX     VSET_MAX+50
//小于数值显示0
#define  Display_V_MIN			 20
#define  Display_I_MIN			 20
#define  tiaoshi				 0
// ==0  是加保护   ==1是不加保护