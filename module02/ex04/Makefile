# NAME  = main
# OBJ = main.o usart.o
# CPU = atmega328p
# F_CPU = 16000000UL
# BAUD = 115200
# PORT = /dev/ttyUSB0

# all: hex flash

# hex:
# 	avr-gcc -mmcu=$(CPU) -DF_CPU=$(F_CPU) -Os -o $(NAME).elf $(NAME).c
# 	avr-objcopy -O ihex $(NAME).elf $(NAME).hex
# flash: hex
# 	avrdude -c arduino -p m328p -U flash:w:main.hex:i -P $(PORT) -b $(BAUD)
# clean:
# 	rm -f *.hex *.bin *.elf *.o

# # https://embedthreads.com/wp-content/uploads/2024/04/Make_-AVR-Programming_-Learning-to-Write-Software-for-Hardware-PDFDrive-.pdf

NAME  = main
CPU = atmega328p
F_CPU = 16000000UL
BAUD = 115200
PORT = /dev/ttyUSB0

SRC = $(NAME).c usart.c led.c  # ✅ Ajout des fichiers source
OBJ = $(SRC:.c=.o)  # ✅ Génère la liste des fichiers objets (.o)

CFLAGS = -mmcu=$(CPU) -DF_CPU=$(F_CPU) -Os  # ✅ Options de compilation
LDFLAGS =  # ✅ Pas de librairies additionnelles pour l'instant

all: flash  # ✅ Compilation et flash en une seule commande

$(NAME).elf: $(OBJ)  
	avr-gcc $(CFLAGS) -o $(NAME).elf $(OBJ) $(LDFLAGS)  # ✅ Linker tous les objets

%.o: %.c
	avr-gcc $(CFLAGS) -c $< -o $@  # ✅ Compilation séparée de chaque fichier source

hex: $(NAME).elf
	avr-objcopy -O ihex $(NAME).elf $(NAME).hex  # ✅ Conversion en format .hex

flash: hex
	avrdude -c arduino -p m328p -U flash:w:$(NAME).hex:i -P $(PORT) -b $(BAUD)  # ✅ Flash via avrdude

clean:
	rm -f *.hex *.elf *.o  # ✅ Suppression propre des fichiers objets et binaires
