NAME  = main
CPU = atmega328p
F_CPU = 16000000UL
BAUD = 115200
PORT = /dev/ttyUSB0

all: hex flash

hex:
	avr-gcc -mmcu=$(CPU) -DF_CPU=$(F_CPU) -Os -o $(NAME).elf $(NAME).c
	avr-objcopy -O ihex $(NAME).elf $(NAME).hex
flash: hex
	avrdude -c arduino -p m328p -U flash:w:main.hex:i -P $(PORT) -b $(BAUD)
clean:
	rm -f *.hex *.bin *.elf *.o

# https://www.pololu.com/docs/0J31/5
