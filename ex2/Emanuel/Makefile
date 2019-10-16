# Example Makefile
#
# Exercise 2, TDT4258

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-mcpu=cortex-m3 -mthumb -g -std=c99 -Wall
LDFLAGS=-mcpu=cortex-m3 -mthumb -g -lgcc -lc -lcs3 -lcs3unhosted -lefm32gg -Llib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g
LINKERSCRIPT=lib/efm32gg.ld

ex2.bin : ex2.elf
	${OBJCOPY} -O binary $< $@

ex2.elf : ex2.o timer.o dac.o gpio.o interrupt_handlers.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

%.o : %.c
	${CC} ${CFLAGS} -c $< -o $@

.PHONY : pretty
pretty :
	-indent *.c *.h

.PHONY : upload
upload :
#	-eACommander.sh -r --address 0x00000000 -f "ex2.bin" -r
	-commander flash "ex2.bin" --address 0x00000000 --speed 200

and_upload: ex2.bin upload
.PHONY: all

.PHONY : clean
clean :
#	-rm -rf *.o *.elf *.bin *.hex
	-del *.o *.elf *.bin *.hex
