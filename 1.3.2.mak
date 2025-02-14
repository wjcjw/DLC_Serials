CC = iccavr
LIB = ilibw
CFLAGS =  -IE:\工作\数字板程序\DLC_Serials\header -e -D__ICC_VERSION=722 -DATMega128  -l -g -MLongJump -MHasMul -MEnhanced -Wf-use_elpm 
ASFLAGS = $(CFLAGS) 
LFLAGS =  -g -e:0x20000 -ucrtatmega.o -bfunc_lit:0x8c.0x20000 -dram_end:0x10ff -bdata:0x100.0x10ff -dhwstk_size:30 -beeprom:0.4096 -fihx_coff -S2
FILES = delay.o EEPROM.o init_pro.o main.o new.o TLC2543.o TLC5618.o Uart.o 

1.3.2:	$(FILES)
	$(CC) -o 1.3.2 $(LFLAGS) @1.3.2.lk   -lcatm128
delay.o:
delay.o:	F:\数字板程序\DLC_Serials\source\delay.c
	$(CC) -c $(CFLAGS) F:\数字板程序\DLC_Serials\source\delay.c
EEPROM.o:
EEPROM.o:	F:\数字板程序\DLC_Serials\source\EEPROM.c
	$(CC) -c $(CFLAGS) F:\数字板程序\DLC_Serials\source\EEPROM.c
init_pro.o:
init_pro.o:	F:\数字板程序\DLC_Serials\source\init_pro.c
	$(CC) -c $(CFLAGS) F:\数字板程序\DLC_Serials\source\init_pro.c
main.o:
main.o:	F:\数字板程序\DLC_Serials\source\main.c
	$(CC) -c $(CFLAGS) F:\数字板程序\DLC_Serials\source\main.c
new.o:
new.o:	F:\数字板程序\DLC_Serials\source\new.c
	$(CC) -c $(CFLAGS) F:\数字板程序\DLC_Serials\source\new.c
TLC2543.o:
TLC2543.o:	F:\数字板程序\DLC_Serials\source\TLC2543.C
	$(CC) -c $(CFLAGS) F:\数字板程序\DLC_Serials\source\TLC2543.C
TLC5618.o:
TLC5618.o:	F:\数字板程序\DLC_Serials\source\TLC5618.c
	$(CC) -c $(CFLAGS) F:\数字板程序\DLC_Serials\source\TLC5618.c
Uart.o:
Uart.o:	F:\数字板程序\DLC_Serials\source\Uart.c
	$(CC) -c $(CFLAGS) F:\数字板程序\DLC_Serials\source\Uart.c
