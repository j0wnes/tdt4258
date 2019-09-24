# Example Makefile
#
# Exercise 1, TDT4258

LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld

all: ex1.bin ex1_basic.bin
.PHONY: all

ex1: ex1.bin
.PHONY: ex1
ex1_basic: ex1_basic.bin
.PHONY: ex1_basic

ex1.bin : ex1.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1.elf : ex1.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

ex1.o : ex1.s
	${AS} ${ASFLAGS} $< -o $@

ex1_basic.bin : ex1_basic.elf
	${OBJCOPY} -j .text -O binary $< $@

ex1_basic.elf : ex1_basic.o 
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

ex1_basic.o : ex1_basic.s
	${AS} ${ASFLAGS} $< -o $@

.PHONY : upload
upload :
#	-eACommander.sh -r --address 0x00000000 -f "ex1.bin" -r
	-commander flash "ex1.bin" --address 0x00000000 --speed 200

.PHONY : upload_basic
upload_basic :
#	-eACommander.sh -r --address 0x00000000 -f "ex1_basic.bin" -r
	-commander flash "ex1_basic.bin" --address 0x00000000 --speed 200

.PHONY : clean
clean :
#	-rm -rf *.o *.elf *.bin *.hex
	-del *.o *.elf *.bin *.hex