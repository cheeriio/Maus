CC = arm-eabi-gcc
OBJCOPY = arm-eabi-objcopy
FLAGS = -mthumb -mcpu=cortex-m4
CPPFLAGS = -DSTM32F411xE
CFLAGS = $(FLAGS) -Wall -g \
-O2 -ffunction-sections -fdata-sections \
-Iexternal/inc \
-Iexternal/CMSIS/Include \
-Iexternal/CMSIS/Device/ST/STM32F4xx/Include
LDFLAGS = $(FLAGS) -Wl,--gc-sections -nostartfiles \
-Lexternal/lds -Tstm32f411re.lds

vpath %.c external/src

OBJECTS = main.o accelerometer.o i2c.o messages.o startup_stm32.o gpio.o
TARGET = main

.SECONDARY: $(TARGET).elf $(OBJECTS)

all: $(TARGET).bin

%.elf : $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

%.bin : %.elf
	$(OBJCOPY) $< $@ -O binary

clean :
	rm -f *.bin *.elf *.hex *.d *.o *.bak *~