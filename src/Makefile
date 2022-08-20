PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc

CFLAGS = -c -MMD -O0 -mcpu=cortex-m3 -mthumb -Wall -Wno-unused-but-set-variable -g

C_SOURCES = $(wildcard *.c */*.c */*/*.c)
OBJECT_FILES = $(C_SOURCES:.c=.o)

../bin/main.bin: ../bin/main.elf
	$(PREFIX)objcopy -O binary $< $@

../bin/main.elf: $(OBJECT_FILES)
	$(PREFIX)ld $^ -T./stm32f103c8t6.ld -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

-include *.d

upload: ../bin/main.bin
	st-flash write $< 0x8000000

download:
	st-flash read ../bin/download.bin 0x8000000 0x1ff00

debug: ../bin/main.elf
	-openocd &
	-gdb-multiarch -tui -q $<
	kill $$(pgrep openocd)

all: upload debug

kill_ocd:
	kill $$(pgrep openocd)
	
clean:
	-rm *.o
	-rm *.d
	-rm ../bin/*.elf
	-rm ../bin/*.bin
