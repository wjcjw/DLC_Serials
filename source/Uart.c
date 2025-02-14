/***************************************************************
****************************************************************
公  司: 郑州蓝晶科技公司电子实验室
编写者: 余庆春                       
日  期: 2007-5-17 
uart.c：单片机串口收、发、处理相关函数
****************************************************************
***************************************************************/
#include "Sysdef.h"
#include "App_para.h"
#include "uart.h"
#include "TLC5618.h"
#include "adc_inc.h"
#include "new.h"
//定义串口波特率
#define  BPS4800    0
#define  BPS9600    1
#define  BPS19200   2
#define  BPS38400   3
#define  BPS57600   4
#define  BPS115200  5
//设置串口波特
#define  BAUD_SET   BPS9600
//串口波特率寄存器的值
const char BPS_REG[] = {0x8f,0x47,0x23,0x11,0x0b,0x5};

//定义检验方式
#define  ODD_MODE   0   //奇检验
#define  EVEN_MODE  1   //偶检验
#define  NONE_MODE  2   //无检验

//设置校验方式
#define  CHECK_SET  1 
const char CHECK_REG[] = {0x30,0x20,0x00};
//设定高压值
#define VREF_VAL_CMD   0x56524546
#define VREF_SET_CMD   0x56534554
//设定电流
#define IREF_VAL_CMD   0x49524546
#define IREF_SET_CMD   0x49534554
//读取高压值
#define VMON_SET_CMD   0x564D4F4E
//读取电流值
#define IMON_SET_CMD   0x494D4F4E
//错误清除命令
#define C_CHAR         0x43
#define CLR_CMD        0x52
//错误报告命令
#define F_CHAR         0x46
#define FLT_CMD        0x54
//高压状态
#define STAT_SET_CMD   0x53544154
//射线使能
#define ENBL_SET_CMD   0x454E424C
//看门狗定时器使能
#define WDTE_SET_CMD   0x57445445
//看门狗重新清零
#define WDTT_SET_CMD   0x57445454
//ARC监控
#define checkARC  0x4152434E
//测试命令
#define TEST_CMD       0x54455354
//温度数据
#define TMON_TT_CMD    0x544D4F4E

//录播数据
#define LUBO_DAT_CMD    0x4C55424F
						  
//版本号
#define	VERSION_CMD		0x56455253
//读取设备编号
#define SBBH_CMD        0x53424248
//设置设备编号
#define SZBH_CMD         0x535a4248


//设备号存储
const unsigned char  EQUIPMENT_NUMBER_eeprom[7]={0x01,0x02,0x03,0x04,0x05,0x06,0x07};
unsigned char  EQUIPMENT_NUMBER[7];
unsigned int length;
unsigned int temp;

//串口0接收缓冲区
unsigned char uart0_recv_buf[30] ={0};
//串口0接收长度
unsigned char uart0_recv_len     = 0 ;
unsigned int v_t=0;
unsigned int i_t=0;
//串口0接收计数
unsigned char uart0_recv_count   = 0 ;
//串口0接收标志 =0x55表示接收到有效的数据帧
unsigned char uart0_recv_flg     = 0;
//高压状态
unsigned char hv_staus = 0;
unsigned char open_num = 0;
//高压,电流设置标志
unsigned char vset_flg = 0;
unsigned char iset_flg = 0;
unsigned char HV_ON_FLAG = 0;
//当前设置的高压和电流值:不能为int型
unsigned long vcur_val = VNOR_VAL;
unsigned long vcur_val1 = VNOR_VAL;
unsigned long icur_val = INOR_VAL;
unsigned long icur_val1 = INOR_VAL;
//IO数据序号
unsigned char ERR_FLT = 0;
unsigned char FAULT_DATA = 0;

//Uart通信灯
unsigned char uart_rx = 0;
unsigned char uart_tx = 0;

//WDOG FLAGE & TIME
 unsigned char WATCH_DOG_FLG =0;
 unsigned char WATCH_DOG_TIM =0;
//********************************************
//函数名: void uart0_init(void)
//描  述: 串口0初始化函数,初始化波特率、数据位、
//        停止位,接收标志、接收长度、接收计数
//参  数: void
//返回值: void        
//注  意: 
//*********************************************
void uart0_init(void)
{
 UCSR0B = 0x00;                //disable while setting baud rate
 UCSR0A = 0x00;
 UCSR0C = 0x06;                //8bit data
 UCSR0C |= CHECK_REG[CHECK_SET];
 UBRR0L =  BPS_REG[BAUD_SET]; //set baud rate lo
 UBRR0H = 00;                 //set baud rate hi
 UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
 uart0_recv_flg   = 0 ; 
 uart0_recv_len   = 15;
 uart0_recv_count = 0 ;
}

//********************************************
//函数名: void Uart0_Transmit( char i)
//描  述: 串口0单字节发送函数       
//参  数: char i 是要发送的字节
//返回值: void
//        
//注  意: 
//*********************************************
void Uart0_Transmit( char i) 
{
	/* 等待发送缓冲器为空*/
	while (!(UCSR0A & (1<<UDRE0)));
	/* 将数据放入缓冲器，发送数据*/
	UDR0 = i;
}

//********************************************
//函数名: void Uart0_ConstString(const char* buf,unsigned int len)
//描  述: 串口0多个常量字节发送函数       
//参  数: 1.const char* buf 是要发送的常量数组指针 
//        2.char len 是要发送的字节长度
//返回值: void        
//注  意: buf 必须是常量数组的指针(fash)
//*********************************************
void Uart0_ConstString(const char* buf,unsigned int len)
{
   unsigned int i = 0;
   for(i=0;i<len;i++)
   {
      Uart0_Transmit(buf[i]);
   }
}

//********************************************
//函数名: void Uart0_String(char* buf,unsigned int len)
//描  述: 串口0多个字节发送函数       
//参  数: 1. char* buf 是要发送的数组指针 
//        2.char len 是要发送的字节长度
//返回值: void        
//注  意: buf 必须是数组的指针(ram)
//*********************************************

void Uart0_String( char* buf,unsigned int len)
{
   unsigned int i = 0;
   for(i=0;i<len;i++)
   {
      Uart0_Transmit(buf[i]);
   }
}
//********************************************
//函数名: void uart0_rx_isr(void)
//描  述: 串口0接收中断服务函数,中断号19,处理
//        接收到字节,拼成完整的数据帧       
//参  数: void
//返回值: void        
//注  意: 函数代码尽量短
//*********************************************

//规约关键字
#define  STX_CHAR  0x02
#define  END_CHAR  0x3B
#define  CR_CHAR   0x0D
#define  LF_CHAR   0x0A

#pragma interrupt_handler uart0_rx_isr:19
void uart0_rx_isr(void)
{
 //uart has received a character in UDR
   unsigned char ch =0;
   ch = UDR0;
   uart_rx = ~uart_rx;   
   switch(uart0_recv_count)
   {
   case 0:
        if(ch == STX_CHAR)
		   uart0_recv_buf[uart0_recv_count++] = ch;
        break;
   default:
      	uart0_recv_buf[uart0_recv_count++] = ch;
		if(ch == END_CHAR)
		     uart0_recv_len = uart0_recv_count + 3;
		if(uart0_recv_count == uart0_recv_len)
		{
		    uart0_recv_flg = 0x55;
		}
		else if(uart0_recv_count >= 30)
		{
		  uart0_recv_len   = 15; 
          uart0_recv_count = 0 ;
		}   	
   } 
}

//********************************************
//函数名: void uart0_Parse_data(void)
//描  述: 串口0接收数据解析
//参  数: void
//返回值: void        
//注  意: 
//*********************************************
/**********************************************
//设定高压值
#define VREF_SET_CMD   0x56524546
//设定电流
#define IREF_SET_CMD   0x49524546
//读取高压值
#define VMON_SET_CMD   0x564D4F4E
//读取电流值
#define IMON_SET_CMD   0x494D4F4E
//错误清除命令
#define C_CHAR         0x43
#define CLR_CMD        0x52
//错误报告命令
#define F_CHAR         0x46
#define FLT_CMD        0x54
//高压状态
#define STAT_SET_CMD   0x53544154
//射线使能
#define ENBL_SET_CMD   0x454E424C
//看门狗定时器使能
#define WDTE_SET_CMD   0x57445445
//看门狗重新设置
#define WDTT_SET_CMD   0x57445454
***********************************************/

void uart0_Parse_data(void)
{
  uchar i = 0,j=0,k=0;
  uchar buf[230] = {0};
  uint  temp_val = 0;
  ulong temp_data  = 0;
  ulong parse_cmd = 0;
  ulong addr = 0;
  //判断接收到的数据帧校验码
  WATCH_DOG_TIM = 0;
  SET_RUN_LED_ON;
  delay_nms(1);
  if(uart0_recv_buf[uart0_recv_len - 3] == EX_ADD_CRC(&uart0_recv_buf[1],uart0_recv_len - 4))
  {
   //短幀的处理，包括：错误清除，错误报告
     if(uart0_recv_buf[1] == F_CHAR || uart0_recv_buf[1] == C_CHAR)
	 {
	    switch(uart0_recv_buf[3])
	    {
	       case CLR_CMD://错误清除
		        //ADD code:				
				FAULT_DATA = 0;//清除故障总告警标志
				HV_ON_FLAG=0;//清除高压判断标志位
				for(j=7;j>1;j--)//清除指示灯告警
    			{      
      			   SET(LED_PUT_PORT,j);	  
	 			}
				HV_OFF();
		        buf[i++] = STX_CHAR;
		        break;
	      case FLT_CMD://错误报告
		        buf[i++] = STX_CHAR;
				if(FAULT_DATA == 0)
				{  ERR_FLT  =0 ;
				   buf[i++] = 0x30;
				   buf[i++] = 0x30;
				   buf[i++] = 0x30;
				}
				else
				{
				   for(j=ERR_FLT;j<8;j++)
				   {
				      ERR_FLT = j+1 ;
					  if(ERR_FLT >= 8)
						    ERR_FLT = 0;	
					  if((FAULT_DATA>>j)&0x01)
				      {
				         temp_val = HexToAscii(j);//000为无故障
				         buf[i++] = 0x30;
				         buf[i++] = temp_val>>8;
					     buf[i++] = temp_val;						 
						 //ERR_FLT++;
						 					 
						 break;
				      }
				   }	
					 ERR_FLT = 0;
		        }
		   break;		 
	    }
		if(i > 0)
		{   
		    buf[i++] =  END_CHAR;
		    buf[i++] =  EX_ADD_CRC(&buf[1],i-2);
		    buf[i++] =  CR_CHAR;
			buf[i++] =  LF_CHAR;
		    Uart0_String(buf,i);
		}	
	 }
	 else
	 {
	    parse_cmd  = uart0_recv_buf[1];
	    parse_cmd <<= 24;
	    temp_data  =  uart0_recv_buf[2];
	    parse_cmd |= (temp_data<<16);
	    parse_cmd |= ((uart0_recv_buf[3]<<8)&0xff00);
	    parse_cmd |= uart0_recv_buf[4];
	    switch(parse_cmd)
	    {
	        case  VREF_VAL_CMD://高压设置值
				  j = uart0_recv_len - 10;
				  if(j > 4) return;
				  temp_data = 1;
				  for(i=j;i>0;i--)
				  {
				     temp_val+= AsciiToHex(uart0_recv_buf[i+5])*temp_data;
					 temp_data *= 10;
				  }
				  //现在的判据:只要超过最大值,最小值或超过最大功率,就不允许设置
				  temp_data = temp_val*icur_val;//先算出当前功率
			      if(temp_val > VSET_MAX || temp_data > VI_MAX || temp_val < VSET_MIN)
				     return;
				  vcur_val = temp_val;
				  vset_flg = 0x55;	
				   datset_flg=40;
				   
				  
				  
			         tlc5618_output(0,vcur_val*V_DAC_MODULUS/VSET_MAX);
					 delay_1ms();
			         tlc5618_output(0,vcur_val*V_DAC_MODULUS/VSET_MAX);
					 vcur_val1=vcur_val;		  
				  i= 0; 
				  buf[i++] =  STX_CHAR;  
	              break;
			case  VREF_SET_CMD://高压设置
			  
			      if(vset_flg == 0x55)
				   {
				     datset_flg=40;
					// for (k=0;k<20;k++){
			        /* tlc5618_output(0,vcur_val*V_DAC_MODULUS/VSET_MAX);
					 delay_1ms();
			         tlc5618_output(0,vcur_val*V_DAC_MODULUS/VSET_MAX);
					// }  */
					 vcur_val1=vcur_val;
				  }
				  vset_flg = 0;	 
				  buf[i++] =  STX_CHAR;
				  break;  
		    case  IREF_VAL_CMD://束流设置值
			      j = uart0_recv_len - 10;
				  if(j > 4) return;
				  if (VSET_MAX==500) return;
				  temp_data = 1;
				  for(i=j;i>0;i--)
				  {
				     temp_val+= AsciiToHex(uart0_recv_buf[i+5])*temp_data;
					 temp_data *= 10;
				  }
				  //现在的判据:超过最大值,但不超过最大功率,也允许设置
				  temp_data = temp_val*vcur_val;//先算出当前功率
				  if(temp_val > ISET_MAX || temp_data > VI_MAX || temp_val < ISET_MIN)
				        return;
				  icur_val = temp_val ;
				  iset_flg = 0x55;
				   datset_flg=40;
					// for(k=0;k<20;k++){
					 tlc5618_output(1,icur_val*I_DAC_MODULUS/ISET_MAX);
					 delay_1ms();
			         tlc5618_output(1,icur_val*I_DAC_MODULUS/ISET_MAX);
					// }
					 icur_val1=icur_val;
				  i= 0;	
				  buf[i++] =  STX_CHAR;
	              break;
			case  IREF_SET_CMD://束流设置
			      if(iset_flg == 0x55)
				  {
				   /*  datset_flg=40;
					// for(k=0;k<20;k++){
					 tlc5618_output(1,icur_val*I_DAC_MODULUS/ISET_MAX);
					 delay_1ms();
			         tlc5618_output(1,icur_val*I_DAC_MODULUS/ISET_MAX);
					// }*/
					 icur_val1=icur_val;
				  }
				  iset_flg = 0;
				  buf[i++] =  STX_CHAR;
				  break; 	  
		    case  VMON_SET_CMD://高压监视
			      buf[i++] =  STX_CHAR;
				  temp_data = ADC_HV;//WJG001
				 /* if((v_t<temp_data)&&(v_t>temp_data-10))
				  temp_data=(v_t+ADC_HV)/2;
				  if((v_t>temp_data)&&(v_t<temp_data+10))
				  temp_data=(v_t+ADC_HV)/2;
				  v_t=temp_data;	*/		
				  /************************/
				  /*增加功能：小于20显示为0*/
				  if(temp_data<=Display_V_MIN)  temp_data = 0;	 
				  j  = temp_data/1000;
				  buf[i++]  =  HexToAscii(j);
				  temp_val  = temp_data%1000;
				  j = temp_val/100;
				  buf[i++]  =  HexToAscii(j);
				  temp_val  = temp_val%100;
				  j = temp_val/10;
				  buf[i++]  =  HexToAscii(j);
				  j =  temp_val%10;
				  buf[i++]  =  HexToAscii(j);
	              break;
		    case  IMON_SET_CMD://束流监视
				  
			      buf[i++] =  STX_CHAR;
				  temp_data = ADC_IA;
				 /* if((i_t<temp_data)&&(i_t>temp_data-10))
				  temp_data=(i_t+ADC_IA)/2;
				  if((i_t>temp_data)&&(i_t<temp_data+10))
				  temp_data=(i_t+ADC_IA)/2;
				  i_t=temp_data;		*/		
				   if(temp_data<=Display_I_MIN)  temp_data = 0;	   
				  j  = temp_data/1000;
				  buf[i++]  =  HexToAscii(j);
				  temp_val  = temp_data%1000;
				  j = temp_val/100;
				  buf[i++]  =  HexToAscii(j);
				  temp_val  = temp_val%100;
				  j = temp_val/10;
				  buf[i++]  =  HexToAscii(j);
				  j =  temp_val%10;
				  buf[i++]  =  HexToAscii(j);
	              break;
				  
			     
		    case  TMON_TT_CMD://温度监视
				   buf[i++] =  STX_CHAR;
				  temp_data = ADC_TT;//WJG001   -273  +   65 = 2853
				 // temp_val  = (uint)(temp_data*1196/10000);				  
				  //temp_data = temp_val;			
				  if (temp_data>=273)
				  { 
				      temp_val  = (uint)(temp_data-273);
				      temp_data = temp_val;
				  }				  
				  else 
				  { 
				      temp_val  = (uint)(1273-temp_data);
				      temp_data = temp_val;
				  }
				  j  = temp_data/1000;
				  buf[i++]  =  HexToAscii(j);
				  temp_val  = temp_data%1000;
				  j = temp_val/100;
				  buf[i++]  =  HexToAscii(j);
				  temp_val  = temp_val%100;
				  j = temp_val/10;
				  buf[i++]  =  HexToAscii(j);
				  j =  temp_val%10;
				  buf[i++]  =  HexToAscii(j);
	              break;
		    case  STAT_SET_CMD://高压状态
			      buf[i++] =  STX_CHAR;
				  buf[i++] =  HV_ON_FLAG==1 ? 0x31:0x30;  
	              break;
		    case  ENBL_SET_CMD://高压使能
			    if(uart0_recv_buf[6] == 0x31 )
				  {   
				     if(FAULT_DATA==0 )
					 {  
						if(open_num==0)
						{
						 		HV_ON();
						}
						
						else if(open_num>=1)
						{
								open_num=1;
								if(Alarm_num>=30)
								{
								 	HV_ON();
								}
						}
					 }
					 open_num++;
				  }
				 
				  else if(uart0_recv_buf[6] == 0x30)
				  {  
				  	HV_OFF();
				  } 
				  buf[i++] =  STX_CHAR;  	 
	              break;
	        case  WDTE_SET_CMD:
			      //ADD code:
				  if(uart0_recv_buf[5] == 0x31 )
				  {   
				     WATCH_DOG_FLG = 0x55;   
				  }
				  else if(uart0_recv_buf[5] == 0x30)
				  {   
				      WATCH_DOG_FLG = 0x00;   
				  } 				  
			      buf[i++] =  STX_CHAR; 
	              break;
		    case  WDTT_SET_CMD:
			      //ADD code:
				  WATCH_DOG_TIM = 0;//WatchDog Timer Tickle
			      buf[i++] =  STX_CHAR; 
	              break;	
			case  LUBO_DAT_CMD:
			      //ADD code:
				    buf[i++] =  STX_CHAR;				    
				  if (ADC_lb_flg==0xaa)
				  { 				    
					for(j=0;j<=50;j++)		
					{
				       if(ADC_lb_SAVE!=ADC_lb_star)
					   {	  
				          buf[i++] =  ADC_IA_DATA[ADC_lb_star];
				       	  buf[i++] =  ADC_IA_DATA[ADC_lb_star]>>8;
					   	  buf[i++] =  ADC_HV_DATA[ADC_lb_star];
				       	  buf[i++] =  ADC_HV_DATA[ADC_lb_star]>>8;
					   	  ADC_lb_star++;
						  if(ADC_lb_star>=250)ADC_lb_star=0;
					   }
					   else ADC_lb_flg=0x00;
					 }
					 for(j=0;j<=i;j++)		
					 {
				       if(buf[j]==0x3b)buf[j]=0x3a;					   
					 }
				  }				  
				  break;  	           	   	   	  
			//02 54 45 53 54 3B 45 0d 0a(调试命令AD原始数)		  
		    case  TEST_CMD:
				  buf[i++] =  STX_CHAR;
				  buf[i++] =  0x30;
				  buf[i++] =  ADC_data[6];
				  buf[i++] =  ADC_data[6]>>8;
				  buf[i++] =  0x35;
				  buf[i++] =  ADC_data[7];
				  buf[i++] =  ADC_data[7]>>8;
				  buf[i++] =  0x36;
				  buf[i++] =  ADC_data[8];
				  buf[i++] =  ADC_data[8]>>8;
				  break; 
			case  VERSION_CMD: 	
				  buf[i++] =  STX_CHAR;
				  buf[i++] =  0x31;
				  buf[i++] =  0x2E;
				  buf[i++] =  0x33;
				  buf[i++] =  0x32;
				  break; 
        			    case  SBBH_CMD:
				  buf[i++] =  STX_CHAR;			 
				  addr=SBBH_ADDR;
						for( j=0;j<data_rep_count;j++)
						{						
						  
							if(EEPROM_READDATA_CRC(addr,7) != FAIL)
							{
									EEPROM_read_n(addr,&buf[i],7);
									break;
							}
							else	addr+=8;
						}
					i+=7;	
				  break; 
			case  SZBH_CMD:
				  buf[i++] =  STX_CHAR;
				 addr=SBBH_ADDR;
				 temp=CRC8_Table(&uart0_recv_buf[5],7);
				 buf[i++] =  temp;
	
						for( j=0;j<data_rep_count;j++)
						{
								EEPROM_write_n(addr,&uart0_recv_buf[5],7);		
								addr+=7;
								Write_EEPROM(temp,addr) ;						//	EEPROM_write_n(addr,&temp,1);	
								addr+=1;
						}	 
				
			
			     break; 		
		    case  checkARC:
			      buf[i++] =  0x54;   //以下ASCII码翻译结果：The ARC number is  
				  buf[i++] =  0x68;
				  buf[i++] =  0x65;
				  buf[i++] =  0x20;
				  buf[i++] =  0x41;
				  buf[i++] =  0x52;
				  buf[i++] =  0x43;
				  buf[i++] =  0x20;
				  buf[i++] =  0x6E;
				  buf[i++] =  0x75;
				  buf[i++] =  0x6D;
				  buf[i++] =  0x62;
				  buf[i++] =  0x65;
				  buf[i++] =  0x72;
				  buf[i++] =  0x20;
				  buf[i++] =  0x69;
				  buf[i++] =  0x73;
				  buf[i++] =  0x20;
				  j = arc_number/1000;
				  arc_number1=arc_number;
				  buf[i++]  =  HexToAscii(j);
				  arc_number1  = arc_number1%1000;
				  j = arc_number1/100;
				  buf[i++]  =  HexToAscii(j);
				  arc_number1  = arc_number1%100;
				  j = arc_number1/10;
				  buf[i++]  =  HexToAscii(j);
				  j =  arc_number1%10;
				  buf[i++]  =  HexToAscii(j);  //以下ASCII码翻译结果： ;The software number is 1.05
				  buf[i++] =  0x3B;
				  buf[i++] =  0x54;
				  buf[i++] =  0x68;
				  buf[i++] =  0x65;
				  buf[i++] =  0x20;
				  buf[i++] =  0x73;
				  buf[i++] =  0x6F;
				  buf[i++] =  0x66;
				  buf[i++] =  0x74;
				  buf[i++] =  0x77;
				  buf[i++] =  0x61;
				  buf[i++] =  0x72;
				  buf[i++] =  0x65;
				  buf[i++] =  0x20;
				  buf[i++] =  0x6E;
				  buf[i++] =  0x75;
				  buf[i++] =  0x6D;
				  buf[i++] =  0x62;
				  buf[i++] =  0x65;
				  buf[i++] =  0x72;
				  buf[i++] =  0x20;
				  buf[i++] =  0x69;
				  buf[i++] =  0x73;
				  buf[i++] =  0x20;
				  buf[i++] =  0x31;
				  buf[i++] =  0x2E;
				  buf[i++] =  0x30;
				  buf[i++] =  0x35;
				  break;           	   	   	   	   	      
	    }
		if(i > 0)
		{   
		    buf[i++] =  END_CHAR;
		    buf[i++] =  EX_ADD_CRC(&buf[1],i-2);
		    buf[i++] =  CR_CHAR;
			buf[i++] =  LF_CHAR;
		    Uart0_String(buf,i);
			//SET(LED_PORT,UART_TX_LED);
		}	
	 }
  }
  uart0_recv_flg = 0;
  uart0_recv_len   = 15; 
  uart0_recv_count = 0 ;
}

//********************************************
//函数名: unsigned char  EX_ADD_CRC(char* buf,unsigned char len)
//描  述: 特殊的算术和函数
//参  数: 1. char* buf 要求和的缓冲区
//        2. unsigned char len 求和的数据长度
//返回值: unsigned char ret 是返回的求和结果       
//注  意: 先求和,再求补,再将结果D7位清0,D6位置1
//*********************************************
unsigned char  EX_ADD_CRC(char* buf,unsigned char len)
{
   unsigned char i = 0;
   unsigned char ret = 0;
   
   for(i=0;i<len;i++)
      ret += buf[i];
   ret =(~ret) + 1 ;
   SET(ret,6);
   CLR(ret,7);
   return ret;
}


//********************************************
//函数名: unsigned char AsciiToHex(unsigned char num)
//描  述: ASCII转换成16进制数的函数
//参  数: 1. num 是要转换的ASCII
//返回值: unsigned char ret 是返回的16进制数结果       
//注  意: ASCII只能为0-F
unsigned char AsciiToHex(unsigned char num)
{
     unsigned char ret = 0;
	 ret = (num>=0x41?num-0x37:num-0x30);
     return ret ;		 
}
//********************************************
//函数名: unsigned int HexToAscii(unsigned char num)
//描  述: 16进制数转换成ASCII的函数
//参  数: 1. num 是要转换的ASCII
//返回值: unsigned int ret 是返回的两个的ASCII     
//注  意: 一个字节的16进制数转换成两个ASCII
unsigned int HexToAscii(unsigned char num)
{
     unsigned int ret = 0;
	 unsigned char ah =0,al =0;
	 ah = (num>>4)&0x0f;
	 al = num & 0x0f;
	 ah = (ah>9?ah+0x37:ah+0x30);
	 al = (al>9?al+0x37:al+0x30);
	 ret = (ah<<8)|al;
     return ret ;		 
}
