# Example Makefile
#
# Exercise 1, TDT4258

LD=arm-none-eabi-gcc
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

LDFLAGS=-nostdlib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g

LINKERSCRIPT=efm32gg.ld

FILE1=ex1_basic
FILE2=ex1_improved

all: ${FILE1} ${FILE2}
.PHONY: all

${FILE1}: ${FILE1}.bin
.PHONY: ${FILE1}
${FILE2}: ${FILE2}.bin
.PHONY: ${FILE2}

.PRECIOUS: %.elf %.o

%.bin : %.elf
	${OBJCOPY} -j .text -O binary $< $@

%.elf : %.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

%.o : %.s
	${AS} ${ASFLAGS} $< -o $@


.PHONY : upload_basic
upload_basic :
	-eACommander.sh -r --address 0x00000000 --speed 200 -f "${FILE1}.bin" -r

.PHONY : upload_improved
upload_improved :
	-eACommander.sh -r --address 0x00000000 --speed 200 -f "${FILE2}.bin" -r

.PHONY : clean
clean :
	-rm -rf *.o *.elf *.bin *.hex