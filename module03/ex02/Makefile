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

SRC = $(NAME).c usart.c led.c timer1.c  # ✅ Ajout des fichiers source
OBJ = $(SRC:.c=.o)  # ✅ Génère la liste des fichiers objets (.o)

CFLAGS = -mmcu=$(CPU) -DF_CPU=$(F_CPU) -Os  # ✅ Options de compilation
LDFLAGS =  # ✅ Pas de librairies additionnelles pour l'instant

all: flash  # ✅ Compilation et flash en une seule commande

$(NAME).elf: $(OBJ)  
	@echo "🔗✨ Linker tous les objets"
	avr-gcc $(CFLAGS) -o $(NAME).elf $(OBJ) $(LDFLAGS)  

%.o: %.c
	@echo "🛠️📦 Compilation séparée de chaque fichier source"
	avr-gcc $(CFLAGS) -c $< -o $@  

hex: $(NAME).elf
	@echo "📦➡️🔢 Conversion en format .hex"
	avr-objcopy -O ihex $(NAME).elf $(NAME).hex  

flash: hex
	@echo "🚀⚡ Flash via avrdude" 
	avrdude -c arduino -p m328p -U flash:w:$(NAME).hex:i -P $(PORT) -b $(BAUD)  

clean:
	@echo "🧹🗑️ Suppression propre des fichiers objets et binaires"
	rm -f *.hex *.elf *.o   
