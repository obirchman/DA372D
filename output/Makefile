# A mess. Just quick and dirty to get going. Need something a bit
# more generic

PRG         = output_node
MCU         = atmega328p
CPU         = 16000000
PROGRAMMER  = avrispmkII
BAUD        = 9600
PORT        = usb
CC          = avr-gcc.exe
OBJCOPY     = avr-objcopy.exe
BOOT        = 0x0000
CFLAGS      = -Wall -std=c99 -Os -DF_CPU=$(CPU) -mmcu=$(MCU) -Wl,-Map,$(NAME).map -Wl,--section-start=.text=$(BOOT)

default: compile

all:
	compile
	flash
	clean

compile:
	$(CC) $(CFLAGS) -c $(PRG).c -o $(PRG).o
	$(CC) $(CFLAGS) -o $(PRG).elf $(PRG).o
	$(OBJCOPY) -j .text -j .data -O ihex $(PRG).elf $(PRG).hex

flash:
	avrdude -c $(PROGRAMMER) -p $(MCU) -P ${PORT} -b $(BAUD) -U flash:w:$(PRG).hex:i

HFUSE =
LFUSE =
EFUSE =
flash_fuses: 
	avrdude -c $(PROGRAMMER) -p $(MCU) -P ${PORT} -b $(BAUD) -U hfuse:w:$(HFUSE):m -U lfuse:w:$(LFUSE):m -U efuse:w:$(EFUSE):m

clean:
	rm $(PRG).o
	rm $(PRG).elf
	rm $(PRG).hex
