EXECUTABLE=firmware.elf
BIN_IMAGE=firmware.bin

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -O2
CFLAGS += -mlittle-endian -mcpu=cortex-m0  -mthumb
#CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections

# to run from FLASH
CFLAGS+=-Wl,-T,stm/stm32f0.ld

# libs
CFLAGS+=-I../../stm32f0-discovery-basic-template/Libraries/STM32F0xx_StdPeriph_Driver/inc
CFLAGS+=-I../../stm32f0-discovery-basic-template/Libraries/CMSIS/Device/ST/STM32F0xx/Include
CFLAGS+=-I../../stm32f0-discovery-basic-template/Libraries/CMSIS/Include

all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@

$(EXECUTABLE): *.c stm/system_stm32f0xx.c stm/startup_stm32f0xx.s
	$(CC) $(CFLAGS) $^ -o $@  -L../../stm32f0-discovery-basic-template/Libraries -lstm32f0
	
install:
#	st-flash write $(BIN_IMAGE) 0x08000000
	openocd -f /usr/local/share/openocd/scripts/board/stm32f0discovery.cfg -f stm32f0-openocd.cfg -c "stm_flash $(BIN_IMAGE)" -c shutdown

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)

.PHONY: all clean
