/***************************************************************
****************************************************************
公  司: 郑州蓝晶科技公司电子实验室
编写者: 余庆春                       
日  期: 2007-5-17 
ADC.c：单片机内部EEPROM读写相关函数
****************************************************************
***************************************************************/
#include "sysdef.h"
#include "new.h"
const char table[256]={
0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,//15
157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,//31
35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,//47
190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,//63
70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,//79
219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,//95
101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,//111
248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,//127
140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,//143
17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,//159
175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,//175
50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,//191
202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};
/*EEPROM读取函数*/
/*addr：地址i：读出数据*/
unsigned char Read_EEPROM(unsigned int addr)
{
    unsigned char i;
	while(EECR & (1 << EEWE));
	EEARH = addr>>8;
	EEARL = addr&0x00ff;
    EECR |= (1 << EERE);
    i = EEDR;
	return i;	
}

/*EEPROM写入函数*/
/*addr：地址；p_buff：写入数据*/
void Write_EEPROM(unsigned char p_buff,unsigned int addr) 
{
    while(EECR & (1 << EEWE));
	EEARH = addr>>8;
	EEARL = addr & 0x00ff;
	EEDR = p_buff;
	EECR |= (1 << EEMWE);
	EECR |= (1 << EEWE);	
}
unsigned char CRC8_Table(unsigned char *p,char counter)
{		
		unsigned char res=0x00;
		for(;counter>0;counter--)
				{
						res	= table[res ^ *p];
						p++;
				}
		return res;
}
void EEPROM_write_n(uint32_t EE_address,uint8_t *DataAddress,uint16_t number)
{
	
		do
		{	
			Write_EEPROM(*DataAddress,EE_address);
			EE_address++;                     //下一个地址
			DataAddress++;                    //下一个数据
		}while(--number);                   //直到结束
	
}
void EEPROM_read_n(uint32_t EE_address,uint8_t *DataAddress,uint16_t number)
{
	
		
		do
		{
			*DataAddress = Read_EEPROM(EE_address);            //读出的数据送往
			EE_address++;                     //下一个地址
			DataAddress++;                    //下一个数据
		}
		while(--number);
	
}
//==========================================================================
//                           读取EEPROM的冗余设计
//
//==========================================================================
u32 EEPROM_READDATA_CRC(u32 Read_addr,u8 count)  
{
 	 u8 temp[8];
	 u8 data_crc=0,i=0;
 	 u32 temp_val=0,temp_data=0;
	EEPROM_read_n((u32)Read_addr,&temp[0],count+1);
	data_crc=CRC8_Table(&temp[0],count);
	
	if(data_crc == temp[count])  
	{		

				{
						temp_val =0 ;
						temp_data=1;
						for( ;count>0;count--)
								{
									temp_val+= AsciiToHex(temp[count-1])*temp_data;
									temp_data *= 10;
								} 
		
						return temp_val;
				}
	}
	else return FAIL;
}